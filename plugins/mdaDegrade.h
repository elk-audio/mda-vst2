#ifndef __mdaDegrade_H
#define __mdaDegrade_H

#include "audioeffectx.h"

class mdaDegrade : public AudioEffectX
{
public:
	mdaDegrade(audioMasterCallback audioMaster);
	~mdaDegrade();

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
  float fi2, fo2, clp, lin, lin2, g1, g2, g3, mode;
  float buf0, buf1, buf2, buf3, buf4, buf5, buf6, buf7, buf8, buf9;
  int tn, tcount;
  
	char programName[32];
};

#endif
