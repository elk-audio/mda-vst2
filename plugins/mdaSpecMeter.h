//see associated .cpp file for copyright and other info

#include "audioeffectx.h"

#include <string.h>

#define NPROGS 4 //can hide decay settings in programs!  fast...slow...peak hold
#define SILENCE 0.00000001f



enum 
{
	_PARAM0, //peak decay
	_PARAM1, //RMS speed
	_PARAM2, //spectrum speed
	_PARAM3, //peak reset?

	NPARAMS
};

class mdaSpecMeter;

class mdaSpecMeterProgram
{
friend class mdaSpecMeter;
public:
	mdaSpecMeterProgram();
private:	
	float param[NPARAMS];
	char  name[24];
};


class mdaSpecMeter : public AudioEffectX
{
public:
	mdaSpecMeter(audioMasterCallback audioMaster);
	~mdaSpecMeter();

	virtual void  process(float **inputs, float **outputs, VstInt32 sampleFrames);
	virtual void  processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames);
	virtual void  setProgram(VstInt32 program);
	virtual void  setProgramName(char *name);
	virtual void  getProgramName(char *name);
	virtual bool  getProgramNameIndexed (VstInt32 category, VstInt32 index, char* name);
	virtual void  setParameter(VstInt32 index, float value);
	virtual float getParameter(VstInt32 index);
	virtual void  getParameterLabel(VstInt32 index, char *label);
	virtual void  getParameterDisplay(VstInt32 index, char *text);
	virtual void  getParameterName(VstInt32 index, char *text);
	virtual void  setSampleRate(float sampleRate);
	virtual void  suspend();

	virtual bool getEffectName(char *name);
	virtual bool getVendorString(char *text);
	virtual bool getProductString(char *text);
	virtual VstInt32 getVendorVersion() { return 1000; }

  //accessible from editor
  VstInt32  counter;
  float Lpeak, Lhold, Lmin, Lrms, Rpeak, Rhold, Rmin, Rrms, Corr; // #11  #12
  float band[2][16]; //8  16  31  64  125  250  500  1k  2k  4k  8k  16k  32k
 
private:
	mdaSpecMeterProgram *programs;

  float iK, lpeak, lmin, lrms, rpeak, rmin, rrms, corr, den;
  float lpp[6][16], rpp[6][16];
  VstInt32  topband, K, kmax;

  float gain;
};
