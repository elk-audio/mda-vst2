#include "mdaRingMod.h"

#include <math.h>

AudioEffect *createEffectInstance(audioMasterCallback audioMaster)
{
  return new mdaRingMod(audioMaster);
}

mdaRingMod::mdaRingMod(audioMasterCallback audioMaster)	: AudioEffectX(audioMaster, 1, 3)	// 1 program, 3 parameter only
{
	fParam1 = (float)0.0625; //1kHz		
  fParam2 = (float)0.0;
  fParam3 = (float)0.0;
	fPhi = 0.0;			
	twoPi = (float)6.2831853;
  fdPhi = (float)(twoPi * 100.0 * (fParam2 + (160.0 * fParam1))/ getSampleRate()); 
  ffb = 0.f;
  fprev = 0.f;

  setNumInputs(2);		    // stereo in
	setNumOutputs(2);		    // stereo out
	setUniqueID('mdaR');    // identify
	DECLARE_VST_DEPRECATED(canMono) ();				      
	canProcessReplacing();	// supports both accumulating and replacing output
	strcpy(programName, "Ring Modulator");	// default program name
}

mdaRingMod::~mdaRingMod()
{
	// nothing to do here
}

bool  mdaRingMod::getProductString(char* text) { strcpy(text, "mda RingMod"); return true; }
bool  mdaRingMod::getVendorString(char* text)  { strcpy(text, "mda"); return true; }
bool  mdaRingMod::getEffectName(char* name)    { strcpy(name, "RingMod"); return true; }

void mdaRingMod::setProgramName(char *name)
{
	strcpy(programName, name);
}

void mdaRingMod::getProgramName(char *name)
{
	strcpy(name, programName);
}

bool mdaRingMod::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* name)
{
	if (index == 0) 
	{
	    strcpy(name, programName);
	    return true;
	}
	return false;
}

void mdaRingMod::setParameter(VstInt32 index, float value)
{
	switch(index)
  {
    case 0: fParam1 = value; break;
    case 1: fParam2 = value; break;
    case 2: fParam3 = value; break;
  }
	fdPhi = (float) (twoPi * 100.0 * (fParam2 + (160.0 * fParam1))/ getSampleRate());
  ffb = 0.95f * fParam3;
}

float mdaRingMod::getParameter(VstInt32 index)
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

void mdaRingMod::getParameterName(VstInt32 index, char *label)
{
	switch(index)
  {
    case 0: strcpy(label, "Freq"); break;
    case 1: strcpy(label, "Fine"); break;
    case 2: strcpy(label, "Feedback"); break;
  }
}

#include <stdio.h>
void int2strng(VstInt32 value, char *string) { sprintf(string, "%d", value); }

void mdaRingMod::getParameterDisplay(VstInt32 index, char *text)
{
	switch(index)
  {
    case 0: int2strng((VstInt32)(100. * floor(160. * fParam1)), text); break;
    case 1: int2strng((VstInt32)(100. * fParam2), text); break;
    case 2: int2strng((VstInt32)(100. * fParam3), text); break;
  }
	
}

void mdaRingMod::getParameterLabel(VstInt32 index, char *label)
{
	switch(index)
  {
    case 0: strcpy(label, "Hz"); break;
    case 1: strcpy(label, "Hz"); break;
    case 2: strcpy(label, "%"); break;
  }
}

//--------------------------------------------------------------------------------
// process

void mdaRingMod::process(float **inputs, float **outputs, VstInt32 sampleFrames)
{
	float *in1 = inputs[0];
	float *in2 = inputs[1];
	float *out1 = outputs[0];
	float *out2 = outputs[1];
	float a, b, c, d, g;	// use registers in sample loops!
  float p, dp, tp = twoPi, fb, fp, fp2;

	p = fPhi;
  dp = fdPhi;
  fb = ffb;
  fp = fprev;
  
  --in1;	// pre-decrement so we can use pre-increment in the loop
	--in2;	// this is because pre-increment is fast on power pc
	--out1;
	--out2;
	while(--sampleFrames >= 0)
	{
		a = *++in1;		// try to do load operations first...
		b = *++in2;
		
    g = (float)sin(p);              //instantaneous gain
    p = (float)fmod( p + dp, tp );   //oscillator phase
     
    c = out1[1];	// get output, as we need to accumulate
		d = out2[1];
		
    fp =  (fb * fp + a) * g;
    fp2 = (fb * fp + b) * g;
    
    c += fp;		// accumulate to output buss
		d += fp2;
		    
    *++out1 = c;	// ...and store operations at the end,
		*++out2 = d;	// as this uses the cache efficiently.
	}
  fPhi = p;
  fprev = fp;
}

void mdaRingMod::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames)
{
	float *in1 = inputs[0];
	float *in2 = inputs[1];
	float *out1 = outputs[0];
	float *out2 = outputs[1];
	float a, b, c, d, g;	
  float p, dp, tp = twoPi, fb, fp, fp2;

  p = fPhi;
  dp = fdPhi;
  fb = ffb;
  fp = fprev;

	--in1;
	--in2;	
	--out1;
	--out2;
	while(--sampleFrames >= 0)
	{
		a = *++in1;		
		b = *++in2;
		
    g = (float)sin(p);
    p = (float)fmod( p + dp, tp );

    fp = (fb * fp + a) * g;
    fp2 = (fb * fp + b) * g;

		c = fp;		
		d = fp2;

    *++out1 = c;	
		*++out2 = d;	
	}
  fPhi = p;
  fprev = fp;
}
