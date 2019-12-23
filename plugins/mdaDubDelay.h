#ifndef __mdaDubDelay_H
#define __mdaDubDelay_H

#include "audioeffectx.h"

class mdaDubDelay : public AudioEffectX
{
public:
	mdaDubDelay(audioMasterCallback audioMaster);
	~mdaDubDelay();

	virtual void process(float **inputs, float **outputs, VstInt32 sampleFrames);
	virtual void processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames);
	virtual void setProgramName(char *name);
	virtual void getProgramName(char *name);
	virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* name);
	virtual void setParameter(VstInt32 index, float value);
	virtual float getParameter(VstInt32 index);
	virtual void getParameterLabel(VstInt32 index, char *label);
	virtual void getParameterDisplay(VstInt32 index, char *text);
	virtual void getParameterName(VstInt32 index, char *text);
  virtual void suspend();

	virtual bool getEffectName(char *name);
	virtual bool getVendorString(char *text);
	virtual bool getProductString(char *text);
	virtual VstInt32 getVendorVersion() { return 1000; }

protected:
	float fParam0;
  float fParam1;
  float fParam2;
  float fParam3;
  float fParam4;
  float fParam5;
  float fParam6;

  float *buffer;               //delay
	VstInt32 size, ipos;             //delay max time, pointer, left time, right time
  float wet, dry, fbk;         //wet & dry mix
  float lmix, hmix, fil, fil0; //low & high mix, crossover filter coeff & buffer
  float env, rel;              //limiter (clipper when release is instant)
  float del, mod, phi, dphi;   //lfo
  float dlbuf;                 //smoothed modulated delay

  char programName[32];
};

#endif
