#include "mdaDe-ess.h"

#include <math.h>
#include <float.h>

AudioEffect *createEffectInstance(audioMasterCallback audioMaster)
{
  return new mdaDeEss(audioMaster);
}

mdaDeEss::mdaDeEss(audioMasterCallback audioMaster)	: AudioEffectX(audioMaster, 1, 3)	// programs, parameters
{
  //inits here!
  fParam1 = (float)0.15f; //thresh
  fParam2 = (float)0.60f; //f
  fParam3 = (float)0.50f; //drive
  setNumInputs(2);		  
	setNumOutputs(2);		  
	setUniqueID('mdas');   //identify here
	DECLARE_VST_DEPRECATED(canMono) ();				      
	canProcessReplacing();	
	strcpy(programName, "De-esser");

  env=0.f;
  setParameter(0, 0.5f);
}

bool  mdaDeEss::getProductString(char* text) { strcpy(text, "mda De-ess"); return true; }
bool  mdaDeEss::getVendorString(char* text)  { strcpy(text, "mda"); return true; }
bool  mdaDeEss::getEffectName(char* name)    { strcpy(name, "De-ess"); return true; }

void mdaDeEss::setParameter(VstInt32 index, float value)
{
	switch(index)
  {
    case 0: fParam1 = value; break;
    case 1: fParam2 = value; break;
    case 2: fParam3 = value; break;
  }
  //calcs here
  thr=(float)pow(10.0f, 3.f * fParam1 - 3.f);
  att=0.01f;
  rel=0.992f;
  fil=0.05f + 0.94f * fParam2 * fParam2;
  gai=(float)pow(10.0f, 2.f * fParam3 - 1.f);
}

mdaDeEss::~mdaDeEss()
{
}

void mdaDeEss::setProgramName(char *name)
{
	strcpy(programName, name);
}

void mdaDeEss::getProgramName(char *name)
{
	strcpy(name, programName);
}

bool mdaDeEss::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* name)
{
	if (index == 0) 
	{
	    strcpy(name, programName);
	    return true;
	}
	return false;
}

float mdaDeEss::getParameter(VstInt32 index)
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

void mdaDeEss::getParameterName(VstInt32 index, char *label)
{
	switch(index)
  {
    case 0: strcpy(label, "Thresh"); break;
    case 1: strcpy(label, "Freq"); break;
    case 2: strcpy(label, "HF Drive"); break;
  }
}

#include <stdio.h>
void int2strng(VstInt32 value, char *string) { sprintf(string, "%d", value); }

void mdaDeEss::getParameterDisplay(VstInt32 index, char *text)
{
	switch(index)
  {
    case 0: int2strng((VstInt32)(60.f * fParam1 - 60.f), text); break;
    case 1: int2strng((VstInt32)(1000 + 11000 * fParam2), text); break;
    case 2: int2strng((VstInt32)(40.0 * fParam3 - 20.0), text); break;
  }
}

void mdaDeEss::getParameterLabel(VstInt32 index, char *label)
{
	switch(index)
  {
    case 0: strcpy(label, "dB"); break;
    case 1: strcpy(label, "Hz"); break;
    case 2: strcpy(label, "dB"); break;  
  }
}

//--------------------------------------------------------------------------------
// process

void mdaDeEss::process(float **inputs, float **outputs, VstInt32 sampleFrames)
{
	float *in1 = inputs[0];
	float *in2 = inputs[1];
	float *out1 = outputs[0];
	float *out2 = outputs[1];
	float a, b, c, d;	
  float f1=fbuf1, f2=fbuf2, tmp, fi=fil, fo=(1.f-fil);
  float at=att, re=rel, en=env, th=thr, g, gg=gai;
  
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

    tmp = 0.5f * (a + b);       //2nd order crossover
    f1  = fo * f1 + fi * tmp;
    tmp -= f1;
    f2  = fo * f2 + fi * tmp;
    tmp = gg*(tmp - f2);        //extra HF gain
    
    en=(tmp>en)? en+at*(tmp-en) : en*re;             //envelope
    if(en>th) g=f1+f2+tmp*(th/en); else g=f1+f2+tmp; //limit 
    
    c += g;
    d += g;
    *++out1 = c;
		*++out2 = d;
	}
  if(fabs(f1)<1.0e-10) {fbuf1=0.f; fbuf2=0.f;} else {fbuf1=f1; fbuf2=f2;}
  if(fabs(en)<1.0e-10) env=0.f; else env=en;
}

void mdaDeEss::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames)
{
	float *in1 = inputs[0];
	float *in2 = inputs[1];
	float *out1 = outputs[0];
	float *out2 = outputs[1];
	float a, b;	
  float f1=fbuf1, f2=fbuf2, tmp, fi=fil, fo=(1.f-fil);
  float at=att, re=rel, en=env, th=thr, g, gg=gai;

	--in1;	
	--in2;	
	--out1;
	--out2;
	while(--sampleFrames >= 0)
	{
		a = *++in1;		
		b = *++in2; 
		
    tmp = 0.5f * (a + b);       //2nd order crossover
    f1  = fo * f1 + fi * tmp;
    tmp -= f1;
    f2  = fo * f2 + fi * tmp;
    tmp = gg*(tmp - f2);        //extra HF gain
    
    en=(tmp>en)? en+at*(tmp-en) : en*re;             //envelope
    if(en>th) g=f1+f2+tmp*(th/en); else g=f1+f2+tmp; //limit 
                //brackets for full-band!!!
    *++out1 = g;
		*++out2 = g;
	}
  if(fabs(f1)<1.0e-10) {fbuf1=0.f; fbuf2=0.f;} else {fbuf1=f1; fbuf2=f2;}
  if(fabs(en)<1.0e-10) env=0.f; else env=en;
}
