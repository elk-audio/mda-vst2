#ifndef __mdaBeatBox_H
#define __mdaBeatBox_H

#include "audioeffectx.h"

class mdaBeatBox : public AudioEffectX
{
public:
	mdaBeatBox(audioMasterCallback audioMaster);
	~mdaBeatBox();

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
  virtual void synth();

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
  float fParam9;
  float fParam10;
  float fParam11;
  float fParam12;
  float hthr, hfil, sthr, kthr, kfil1, kfil2, mix;
  float klev, hlev, slev;
  float  ww,  wwx,  sb1,  sb2,  sf1,  sf2, sf3;
  float kww, kwwx, ksb1, ksb2, ksf1, ksf2;
  float dyne, dyna, dynr, dynm;

  float *hbuf;
  float *kbuf;
  float *sbuf, *sbuf2;
	VstInt32 hbuflen, hbufpos, hdel;
	VstInt32 sbuflen, sbufpos, sdel, sfx;
  VstInt32 kbuflen, kbufpos, kdel, ksfx;
  VstInt32 rec, recx, recpos;

	char programName[32];
};

#endif
