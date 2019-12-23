//See associated .cpp file for copyright and other info


#define NPARAMS  4  ///number of parameters
#define NPROGS   3  ///number of programs
#define BUFMAX   4096

#ifndef __mdaDetune_H
#define __mdaDetune_H

#include "audioeffectx.h"

struct mdaDetuneProgram
{
  friend class mdaDetune;
  float param[NPARAMS];
  char name[32];
};


class mdaDetune : public AudioEffectX
{
public:
  mdaDetune(audioMasterCallback audioMaster);

  virtual void  process(float **inputs, float **outputs, VstInt32 sampleFrames);
  virtual void  processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames);
  virtual void  setProgram(VstInt32 program);
  virtual void  setProgramName(char *name);
  virtual void  getProgramName(char *name);
  virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* name);
  virtual void  setParameter(VstInt32 index, float value);
  virtual float getParameter(VstInt32 index);
  virtual void  getParameterLabel(VstInt32 index, char *label);
  virtual void  getParameterDisplay(VstInt32 index, char *text);
  virtual void  getParameterName(VstInt32 index, char *text);
  virtual void  suspend();

	virtual bool getEffectName(char *name);
	virtual bool getVendorString(char *text);
	virtual bool getProductString(char *text);
	virtual VstInt32 getVendorVersion() { return 1000; }

protected:
	mdaDetuneProgram programs[NPROGS];
	float buf[BUFMAX];
	float win[BUFMAX];

  ///global internal variables
  VstInt32  buflen;           //buffer length
  float bufres;           //buffer resolution display
  float semi;             //detune display
  VstInt32  pos0;             //buffer input
  float pos1, dpos1;      //buffer output, rate
  float pos2, dpos2;      //downwards shift
  float wet, dry;         //ouput levels
};

#endif
