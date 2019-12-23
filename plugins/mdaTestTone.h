#ifndef __mdaTestTone_H
#define __mdaTestTone_H

#include "audioeffectx.h"

class mdaTestTone : public AudioEffectX
{
public:
	mdaTestTone(audioMasterCallback audioMaster);
	~mdaTestTone();

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
  virtual void iso2string(float b, char *text);
  virtual void suspend();
  void midi2string(float n, char *text);

	virtual bool getEffectName(char *name);
	virtual bool getVendorString(char *text);
	virtual bool getProductString(char *text);
	virtual VstInt32 getVendorVersion() { return 1000; }

protected:
  int updateTx, updateRx;
  void update();

	float fParam0, fParam1, fParam2;
	float fParam3, fParam4, fParam5;
	float fParam6, fParam7;
  
  float thru, left, right, len;
  float zz0, zz1, zz2, zz3, zz4, zz5, phi, dphi;
  float sw, swd, swx, fscale, cal, calx;
  VstInt32 swt;
  int mode;

	char programName[32], disp1[16], disp2[16];
};

#endif
