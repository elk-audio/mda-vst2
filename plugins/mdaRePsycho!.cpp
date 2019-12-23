#include "mdaRePsycho!.h"

#include <math.h>
#include <float.h>

AudioEffect *createEffectInstance(audioMasterCallback audioMaster)
{
  return new mdaRePsycho(audioMaster);
}

mdaRePsycho::mdaRePsycho(audioMasterCallback audioMaster)	: AudioEffectX(audioMaster, 1, 7)	// programs, parameters
{
  //inits here!
  fParam1 = (float)0.6; //thresh
  fParam2 = (float)0.5; //env
  fParam3 = (float)1.0; //tune
  fParam4 = (float)1.0; //mix
  fParam5 = (float)0.45; //minimum chunk length
  fParam6 = (float)1.0; //fine tune
  fParam7 = (float)0.4; //quality
  size = 22050;
	buffer = new float[size];
  buffer2 = new float[size];

  setNumInputs(2);		  
	setNumOutputs(2);		  
	setUniqueID('mdaY');    // identify here
	DECLARE_VST_DEPRECATED(canMono) ();				      
	canProcessReplacing();	
	strcpy(programName, "Re-PsYcHo!");
	suspend();		// flush buffer

  //calcs here!
  buf = 0.0; buf2 = 0.0;
  tim = size + 1;
  dtim = 441 + int(0.5 * size * fParam5);
  fil = 0.0;
  thr = (float)pow(10.0,(1.5 * fParam1) - 1.5);
  
  if(fParam2>0.5) 
  { env = (float)(1.0 + 0.003 * pow(fParam2 - 0.5,5.0)); }
  else
  { env = (float)(1.0 + 0.025 * pow(fParam2 - 0.5,5.0)); }
  
  tun = (float)(((int(fParam3 * 24.0) - 24.0) + (fParam6 - 1.0)) / 24.0);
  tun = (float)pow(10.0, 0.60206 * tun);
  wet = (float)(0.5 * sqrt(fParam4));
  dry = (float)sqrt(1.0 - fParam4);
}

bool  mdaRePsycho::getProductString(char* text) { strcpy(text, "mda RePsycho!"); return true; }
bool  mdaRePsycho::getVendorString(char* text)  { strcpy(text, "mda"); return true; }
bool  mdaRePsycho::getEffectName(char* name)    { strcpy(name, "RePsycho!"); return true; }

void mdaRePsycho::setParameter(VstInt32 index, float value)
{
	switch(index)
  {
    case 0: fParam3 = value; break;
    case 1: fParam6 = value; break;
    case 2: fParam2 = value; break;
    case 3: fParam1 = value; break;
    case 4: fParam5 = value; break;
    case 5: fParam4 = value; break;
    case 6: fParam7 = value; break;
  }
  //calcs here
  dtim = 441 + int(0.5 * size * fParam5);
  thr = (float)pow(10.0,(1.5 * fParam1) - 1.5);
  
  if(fParam2>0.5) 
  { env = (float)(1.0 + 0.003 * pow(fParam2 - 0.5,5.0)); }
  else
  { env = (float)(1.0 + 0.025 * pow(fParam2 - 0.5,5.0)); }
  
  //if(fParam2>0.5) 
  //{ env = (float)(1.0 + 0.01 * (fParam2 - 0.5)); }
  //else
  //{ env = (float)(1.0 + 0.01 * (fParam2 - 0.5)); }
  
  tun = (float)(((int(fParam3 * 24.0) - 24.0) + (fParam6 - 1.0)) / 24.0);
  tun = (float)pow(10.0, 0.60206 * tun);
  wet = (float)(0.5 * sqrt(fParam4));
  dry = (float)sqrt(1.0 - fParam4);
}

mdaRePsycho::~mdaRePsycho()
{
	if(buffer) delete [] buffer;
	if(buffer2) delete [] buffer2;
}

void mdaRePsycho::suspend()
{
	memset(buffer, 0, size * sizeof(float));
	memset(buffer2, 0, size * sizeof(float));
}

void mdaRePsycho::setProgramName(char *name)
{
	strcpy(programName, name);
}

void mdaRePsycho::getProgramName(char *name)
{
	strcpy(name, programName);
}

bool mdaRePsycho::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* name)
{
	if (index == 0) 
	{
	    strcpy(name, programName);
	    return true;
	}
	return false;
}

float mdaRePsycho::getParameter(VstInt32 index)
{
	float v=0;

  switch(index)
  {
    case 0: v = fParam3; break;
    case 1: v = fParam6; break;
    case 2: v = fParam2; break;
    case 3: v = fParam1; break;
    case 4: v = fParam5; break;
    case 5: v = fParam4; break;
    case 6: v = fParam7; break;
  }
  return v;
}

void mdaRePsycho::getParameterName(VstInt32 index, char *label)
{
	switch(index)
  {
    case 0: strcpy(label, "Tune"); break;
    case 1: strcpy(label, "Fine"); break;
    case 2: strcpy(label, "Decay"); break;
    case 3: strcpy(label, "Thresh"); break;
    case 4: strcpy(label, "Hold"); break;
    case 5: strcpy(label, "Mix"); break;
    case 6: strcpy(label, "Quality"); break;
  }
}

#include <stdio.h>
void int2strng(VstInt32 value, char *string) { sprintf(string, "%d", value); }

void mdaRePsycho::getParameterDisplay(VstInt32 index, char *text)
{
	switch(index)
  {
    case 3: int2strng((VstInt32)((30.0 * fParam1) - 30.0), text); break;
    case 2: int2strng((VstInt32)((fParam2 - 0.5) * 100.0), text); break;
    case 0: int2strng((VstInt32)(int(24.0 * fParam3) - 24.0), text); break;
    case 5: int2strng((VstInt32)(100.0 * fParam4), text); break;
    case 4: int2strng((VstInt32)(1000.0 * dtim / getSampleRate()), text); break;
    case 1: int2strng((VstInt32)(int(99.0 * fParam6) - 99.0), text); break;
    case 6: if(fParam7>0.5) strcpy(text, "HIGH");
            else strcpy(text, "LOW"); break;
  }
}

void mdaRePsycho::getParameterLabel(VstInt32 index, char *label)
{
	switch(index)
  {
    case 0: strcpy(label, "semi"); break;
    case 1: strcpy(label, "cent"); break;
    case 2: strcpy(label, "%"); break; 
    case 3: strcpy(label, "dB"); break; 
    case 4: strcpy(label, "ms"); break; 
    case 5: strcpy(label, "%"); break; 
    case 6: strcpy(label, ""); break; 
  }
}

//--------------------------------------------------------------------------------
// process

void mdaRePsycho::process(float **inputs, float **outputs, VstInt32 sampleFrames)
{
	float *in1 = inputs[0];
	float *in2 = inputs[1];
	float *out1 = outputs[0];
	float *out2 = outputs[1];
	float a, b, c, d;	
  float we=wet, dr=dry, tu=tun, en=env;
  float ga=gai, x=0.0f, x2=0.0f, xx=buf, xx2=buf2;
  float it1, it2;
  VstInt32 ti=tim, dti=dtim, of1, of2;
  
  --in1;	
	--in2;	
	--out1;
	--out2;

  if(fParam7>0.5) //high quality
  {
    we=(float)(we*2.0);
    while(--sampleFrames >= 0)
	  {
		  a = *++in1;		
		  b = *++in2;
		  
      c = out1[1];
		  d = out2[1]; //process from here...
  
      if ((a+b > thr) && (ti > dti)) //trigger
      {
        ga = 1.0;
        ti = 0;
      }

      if (ti<22050) //play out
      {  
        if(ti<80) //fade in
        {
          if(ti==0) { xx=x; xx2=x2; }

          *(buffer + ti) = a;
          *(buffer2 + ti) = b;
          x = *(buffer + int(ti * tu));
          x2 = *(buffer2 + int(ti * tu));
        
          x = (float)(xx * (1.0 - (0.0125 * ti)) + (x * 0.0125 * ti)); 
          x2 = (float)(xx2 * (1.0 - (0.0125 * ti)) + (x2 * 0.0125 * ti)); 
        }
        else
        {
          //update to/from buffer
          *(buffer + ti) = a;
          *(buffer2 + ti) = b;

          it1 = (float)(ti * tu); //interpolation
          of1 = (int)it1; of2 = of1 + 1; it1 = it1 - of1; it2 = (float)(1.0 - it1);

            x = (it2* *(buffer + of1)) + (it1* *(buffer + of2));
            x2 = (it2* *(buffer2 + of1)) + (it1* *(buffer2 + of2));
        }

        ti++;
        ga*= en;
      }
      else //mute
      {
        ga = 0;
      }
        
		  c += (a * dr) + (x * ga * we); // output
		  d += (b * dr) + (x2 * ga * we);
		      
      *++out1 = c;	
		  *++out2 = d;
	  }
  }
  else
  {
    while(--sampleFrames >= 0)
	  {
		  a = *++in1;		
		  b = *++in2;
		  
      c = out1[1];
		  d = out2[1]; //process from here...
  
      if ((a+b > thr) && (ti > dti)) //trigger
      {
        ga = 1.0;
        ti = 0;
      }

      if (ti<22050) //play out
      {  
        if(ti<80) //fade in
        {
          if(ti==0) xx = x;

          *(buffer + ti) = (a + b);
          x = *(buffer + int(ti * tu));
        
          x = (float)(xx * (1.0 - (0.0125 * ti)) + (x * 0.0125 * ti));
        }
        else
        {
          //update to/from buffer
          *(buffer + ti) = (a + b);
          x = *(buffer + int(ti * tu));
        }

        ti++;
        ga*= en;
      }
      else //mute
      {
        ga = 0;
      }
        
		  c += (a * dr) + (x * ga * we); // output
		  d += (b * dr) + (x * ga * we);
		      
      *++out1 = c;	
		  *++out2 = d;
	  }
  }
  tim = ti;
  gai = ga;
  buf = xx;
  buf2 = xx2;
}

void mdaRePsycho::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames)
{
	float *in1 = inputs[0];
	float *in2 = inputs[1];
	float *out1 = outputs[0];
	float *out2 = outputs[1];
	float a, b, c, d;	
  float we=wet, dr=dry, tu=tun, en=env;
  float ga=gai, x=0.0f, x2=0.0f, xx=buf, xx2=buf2;
  float it1, it2;
  VstInt32 ti=tim, dti=dtim, of1, of2;

	--in1;	
	--in2;	
	--out1;
	--out2;

  if(fParam7>0.5) //high quality
  {
    we=(float)(we*2.0);
    while(--sampleFrames >= 0)
	  {
		  a = *++in1;		
		  b = *++in2; //process from here...
		  
      if ((a+b > thr) && (ti > dti)) //trigger
      {
        ga = 1.0;
        ti = 0;
      }

      if (ti<22050) //play out
      {  
        if(ti<80) //fade in
        {
          if(ti==0) { xx=x; xx2=x2; }

          *(buffer + ti) = a;
          *(buffer2 + ti) = b;
          x = *(buffer + int(ti * tu));
          x2 = *(buffer2 + int(ti * tu));
        
          x = (float)(xx * (1.0 - (0.0125 * ti)) + (x * 0.0125 * ti)); 
          x2 = (float)(xx2 * (1.0 - (0.0125 * ti)) + (x2 * 0.0125 * ti)); 
        }
        else
        {
          //update to/from buffer
          *(buffer + ti) = a;
          *(buffer2 + ti) = b;

          it1 = (float)(ti * tu); //interpolation
          of1 = (int)it1; of2 = of1 + 1; it1 = it1 - of1; it2 = (float)(1.0 - it1);

            x = (it2* *(buffer + of1)) + (it1* *(buffer + of2));
            x2 = (it2* *(buffer2 + of1)) + (it1* *(buffer2 + of2));
        }

        ti++;
        ga*= en;
      }
      else //mute
      {
        ga = 0;
      }
    
		  c = (a * dr) + (x * ga * we); // output
		  d = (b * dr) + (x2 * ga * we);
		      
      *++out1 = c;
		  *++out2 = d;
	  }
  }
  else
  {
	  while(--sampleFrames >= 0)
	  {
		  a = *++in1;		
		  b = *++in2; //process from here...
		  
      if ((a+b > thr) && (ti > dti)) //trigger
      {
        ga = 1.0;
        ti = 0;
      }

      if (ti<22050) //play out
      {  
        if(ti<80) //fade in
        {
          if(ti==0) xx = x;

          *(buffer + ti) = (a + b);
          x = *(buffer + int(ti * tu));
        
          x = (float)(xx * (1.0 - (0.0125 * ti)) + (x * 0.0125 * ti));
        }
        else
        {
          //update to/from buffer
          *(buffer + ti) = (a + b);
          x = *(buffer + int(ti * tu));
        }

        ti++;
        ga*= en;
      }
      else //mute
      {
        ga = 0;
      }
    
		  c = (a * dr) + (x * ga * we); // output
		  d = (b * dr) + (x * ga * we);
		      
      *++out1 = c;
		  *++out2 = d;
	  }
  }
  tim = ti;
  gai = ga;
  buf = xx;
  buf2 = xx2;
}
