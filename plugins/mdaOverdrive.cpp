#include "mdaOverdrive.h"
  
#include <math.h>


AudioEffect *createEffectInstance(audioMasterCallback audioMaster)
{
  return new mdaOverdrive(audioMaster);
}

mdaOverdrive::mdaOverdrive(audioMasterCallback audioMaster)	: AudioEffectX(audioMaster, 1, 3)	// 1 program, 3 parameters
{
  fParam1 = 0.0f; 		
  fParam2 = 0.0f;
  fParam3 = 0.5f;

  setNumInputs(2);		    
	setNumOutputs(2);		    
	setUniqueID('mdaO');    // identify
	DECLARE_VST_DEPRECATED(canMono) ();				      
	canProcessReplacing();	
	strcpy(programName, "Soft Overdrive");	

  filt1 = filt2 = 0.0f;
  setParameter(0, 0.0f);
}

mdaOverdrive::~mdaOverdrive()
{

}

bool  mdaOverdrive::getProductString(char* text) { strcpy(text, "mda Overdrive"); return true; }
bool  mdaOverdrive::getVendorString(char* text)  { strcpy(text, "mda"); return true; }
bool  mdaOverdrive::getEffectName(char* name)    { strcpy(name, "Overdrive"); return true; }

void mdaOverdrive::setProgramName(char *name)
{
	strcpy(programName, name);
}

void mdaOverdrive::getProgramName(char *name)
{
	strcpy(name, programName);
}

bool mdaOverdrive::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* name)
{
	if (index == 0) 
	{
	    strcpy(name, programName);
	    return true;
	}
	return false;
}

void mdaOverdrive::setParameter(VstInt32 index, float value)
{
	switch(index)
  {
    case 0: fParam1 = value; break;
    case 1: fParam2 = value; break;
    case 2: fParam3 = value; break;
  }
  filt = (float)pow(10.0,-1.6 * fParam2);
  gain = (float)pow(10.0f, 2.0f * fParam3 - 1.0f);
}

float mdaOverdrive::getParameter(VstInt32 index)
{
	float v=0;

  switch(index)
  {
    case 0: v = fParam1; break;
    case 1: v = fParam2; break;
    case 2: v = fParam3; break;
  }
  return v;
}

void mdaOverdrive::getParameterName(VstInt32 index, char *label)
{
	switch(index)
  {
    case 0: strcpy(label, "Drive   "); break;
    case 1: strcpy(label, "Muffle  "); break;
    case 2: strcpy(label, "Output  "); break;
  }
}

#include <stdio.h>
void int2strng(VstInt32 value, char *string) { sprintf(string, "%d", value); }

void mdaOverdrive::getParameterDisplay(VstInt32 index, char *text)
{
	switch(index)
  {
    case 0: int2strng((VstInt32)(100 * fParam1     ), text); break;
    case 1: int2strng((VstInt32)(100 * fParam2     ), text); break;
    case 2: int2strng((VstInt32)( 40 * fParam3 - 20), text); break;
  }
	
}

void mdaOverdrive::getParameterLabel(VstInt32 index, char *label)
{
	switch(index)
  {
    case 0: strcpy(label, "%"); break;
    case 1: strcpy(label, "%"); break;
    case 2: strcpy(label, "dB"); break; 
  }
}

//--------------------------------------------------------------------------------
// process

void mdaOverdrive::process(float **inputs, float **outputs, VstInt32 sampleFrames)
{
	float *in1 = inputs[0];
	float *in2 = inputs[1];
	float *out1 = outputs[0];
	float *out2 = outputs[1];
	float a, b, c, d;
  float i=fParam1, g=gain, aa, bb;
  float f=filt, fa=filt1, fb=filt2;

  --in1;	
	--in2;
	--out1;
	--out2;
	while(--sampleFrames >= 0)
	{
		a = *++in1;
		b = *++in2;
		
    c = out1[1];
		d = out2[1];

    aa = (a>0.0f)? (float)sqrt(a) : (float)-sqrt(-a); //overdrive
    bb = (b>0.0f)? (float)sqrt(b) : (float)-sqrt(-b); 

    fa = fa + f * (i*(aa-a) + a - fa);                //filter 
    fb = fb + f * (i*(bb-b) + b - fb);   

    c += fa * g; 
		d += fb * g;
		    
    *++out1 = c;
		*++out2 = d;
	}
  if(fabs(fa)>1.0e-10) filt1 = fa; else filt1 = 0.0f; //catch denormals
  if(fabs(fb)>1.0e-10) filt2 = fb; else filt2 = 0.0f;
}

void mdaOverdrive::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames)
{
	float *in1 = inputs[0];
	float *in2 = inputs[1];
	float *out1 = outputs[0];
	float *out2 = outputs[1];
	float a, b, c, d;
  float i=fParam1, g=gain, aa, bb;
  float f=filt, fa=filt1, fb=filt2;
  
	--in1;	
	--in2;
	--out1;
	--out2;
	while(--sampleFrames >= 0)
	{
		a = *++in1;		
		b = *++in2;
		
    aa = (a>0.0f)? (float)sqrt(a) : (float)-sqrt(-a); //overdrive
    bb = (b>0.0f)? (float)sqrt(b) : (float)-sqrt(-b); 

    fa = fa + f * (i*(aa-a) + a - fa);                //filter 
    fb = fb + f * (i*(bb-b) + b - fb);   

    c = fa * g; 
		d = fb * g;

    *++out1 = c;	
		*++out2 = d;
	}
  if(fabs(fa)>1.0e-10) filt1 = fa; else filt1 = 0.0f; //catch denormals
  if(fabs(fb)>1.0e-10) filt2 = fb; else filt2 = 0.0f;
}
