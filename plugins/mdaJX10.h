//See associated .cpp file for copyright and other info

#ifndef __mdaJX10__
#define __mdaJX10__

#include <string.h>

#include "audioeffectx.h"

#define NPARAMS  24      //number of parameters
#define NPROGS   64      //number of programs
#define NOUTS    2       //number of outputs
#define NVOICES  8       //max polyphony
#define SILENCE  0.001f  //voice choking
#define PI       3.1415926535897932f
#define TWOPI    6.2831853071795864f
#define ANALOG   0.002f  //oscillator drift

class mdaJX10Program
{
  friend class mdaJX10;
public:
	mdaJX10Program();
private:
  float param[NPARAMS];
  char  name[24];
};


struct VOICE  //voice state
{
  float  period;
  float  p;    //sinc position
  float  pmax; //loop length
  float  dp;   //delta
  float  sin0; //sine osc
  float  sin1;
  float  sinx;
  float  dc;   //dc offset
  
  float  detune;
  float  p2;    //sinc position
  float  pmax2; //loop length
  float  dp2;   //delta
  float  sin02; //sine osc
  float  sin12;
  float  sinx2;
  float  dc2;   //dc offset
  
  float  fc;  //filter cutoff root
  float  ff;  //filter cutoff
  float  f0;  //filter buffers
  float  f1;
  float  f2;
  
  float  saw;
  //float  vca;  //current level  ///eliminate osc1 level when separate amp & filter envs?
  //float  env;  //envelope
  //float  att;  //attack
  //float  dec;  //decay
  float  env;
  float  envd;
  float  envl;
  float  fenv;
  float  fenvd;
  float  fenvl;
  
  float  lev;  //osc levels
  float  lev2;
  float  target; //period target
  VstInt32   note; //remember what note triggered this
};


class mdaJX10 : public AudioEffectX
{
public:
	mdaJX10(audioMasterCallback audioMaster);
	~mdaJX10();

	virtual void process(float **inputs, float **outputs, VstInt32 sampleframes);
	virtual void processReplacing(float **inputs, float **outputs, VstInt32 sampleframes);
	virtual VstInt32 processEvents(VstEvents* events);

	virtual void setProgram(VstInt32 program);
	virtual void setProgramName(char *name);
	virtual void getProgramName(char *name);
	virtual void setParameter(VstInt32 index, float value);
	virtual float getParameter(VstInt32 index);
	virtual void getParameterLabel(VstInt32 index, char *label);
	virtual void getParameterDisplay(VstInt32 index, char *text);
	virtual void getParameterName(VstInt32 index, char *text);
	virtual void setSampleRate(float sampleRate);
	virtual void setBlockSize(VstInt32 blockSize);
  virtual void suspend();
  virtual void resume();

	virtual bool getOutputProperties (VstInt32 index, VstPinProperties* properties);
	virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);
	virtual bool copyProgram (VstInt32 destination);
	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion () {return 1;}
	virtual VstInt32 canDo (char* text);

private:
	void update();  //my parameter update
  void noteOn(VstInt32 note, VstInt32 velocity);
  void fillpatch(VstInt32 p, char *name,
                 float p0,  float p1,  float p2,  float p3,  float p4,  float p5, 
                 float p6,  float p7,  float p8,  float p9,  float p10, float p11,
                 float p12, float p13, float p14, float p15, float p16, float p17, 
                 float p18, float p19, float p20, float p21, float p22, float p23);
  
  mdaJX10Program* programs;
  float Fs;

  #define EVENTBUFFER 120
  #define EVENTS_DONE 99999999
  VstInt32 notes[EVENTBUFFER + 8];  //list of delta|note|velocity for current block
  #define KMAX 32

  ///global internal variables
  #define SUSTAIN -1
  VstInt32 sustain, activevoices;
  VOICE voice[NVOICES];

  float semi, cent;
  float tune, detune;
  float filtf, fzip, filtq, filtlfo, filtenv, filtvel, filtwhl;
  float oscmix, noisemix;
  float att, dec, sus, rel, fatt, fdec, fsus, frel;
  float lfo, dlfo, modwhl, press, pbend, ipbend, rezwhl;
  float velsens, volume, voltrim;
  float vibrato, pwmdep, lfoHz, glide, glidedisp;
  VstInt32  K, lastnote, veloff, mode;
  unsigned int noise;
};

#endif
