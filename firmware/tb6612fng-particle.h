#ifndef TB6612FNG_PARTICLE_H
#define TB6612FNG_PARTICLE_H

#include "application.h"

#define TB6612FNG_CHANNEL_A 0
#define TB6612FNG_CHANNEL_B 1

class Tb6612fng
{
  public:
  	Tb6612fng(int in1, int in2, int pwm);
  	Tb6612fng(int in1, int in2, int pwm, int stby);
  	Tb6612fng(int aIn1, int aIn2, int pwmA, int bIn1, int bIn2, int pwmB);
  	Tb6612fng(int aIn1, int aIn2, int pwmA, int bIn1, int bIn2, int pwmB, int stby);
    void setPwmFrequency(int pwmFrequency);
    void setPwmFrequency(int channel, int pwmFrequency);
	void sleep();
	void setSpeed(int speed);
	void setSpeed(int channel, int speed);
  private:
    void setupChannelPins(int channel, int in1, int in2, int pwm);
    int _numChannels;
    int _pinStby;
    int _pinIn1[2];
    int _pinIn2[2];
    int _pinPwm[2];
    int _pwmFrequency[2];
};


#endif
