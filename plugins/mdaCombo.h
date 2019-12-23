#ifndef __mdaCombo_H
#define __mdaCombo_H

#include "audioeffectx.h"

class mdaCombo : public AudioEffectX
{
public:
	mdaCombo(audioMasterCallback audioMaster);
	~mdaCombo();

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
  virtual float filterFreq(float hz);
  virtual void suspend();

	virtual bool getEffectName(char *name);
	virtual bool getVendorString(char *text);
	virtual bool getProductString(char *text);
	virtual VstInt32 getVendorVersion() { return 1000; }

protected:
	float fParam1;
  float fParam2;
  float fParam3;
  float fParam4;
  float fParam5;
  float fParam6;
  float fParam7;

  float clip, drive, trim, lpf, hpf, mix1, mix2;
  float ff1, ff2, ff3, ff4, ff5, bias;
  float ff6, ff7, ff8, ff9, ff10;
  float hhf, hhq, hh0, hh1; //hpf

  float *buffer, *buffe2;
	VstInt32 size, bufpos, del1, del2;
  int mode, ster;

	char programName[32];
};

#endif
