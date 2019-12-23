#ifndef __mdaTracker_H
#define __mdaTracker_H

#include "audioeffectx.h"

class mdaTracker : public AudioEffectX
{
public:
	mdaTracker(audioMasterCallback audioMaster);
	~mdaTracker();

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
  float fParam8;
  float fi, fo, thr, phi, dphi, ddphi, trans;
  float buf1, buf2, dn, bold, wet, dry;
  float dyn, env, rel, saw, dsaw;
  float res1, res2, buf3, buf4;
  VstInt32  max, min, num, sig, mode;
    
	char programName[32];
};

#endif
