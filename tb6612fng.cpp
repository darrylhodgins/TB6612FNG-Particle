#include "application.h"
#include "tb6612fng.h"

/**
 * Constructor if you're only using 1 channel of the TB6612FNG.
 */
Tb6612fng::Tb6612fng(int in1, int in2, int pwm) {
	_numChannels = 1;
	_pinStby = -1;
	setupChannelPins(TB6612FNG_CHANNEL_A, in1, in2, pwm);
	setPwmFrequency(0);
	sleep();
}

/**
 * Constructor if you're only using 1 channel of the TB6612FNG, with Standby control.
 */
Tb6612fng::Tb6612fng(int in1, int in2, int pwm, int stby) {
	_numChannels = 1;
	_pinStby = stby;
	pinMode(_pinStby, OUTPUT);
	setupChannelPins(TB6612FNG_CHANNEL_A, in1, in2, pwm);
	setPwmFrequency(0);
	sleep();
}

/**
 * Constructor for both channels of the TB6612FNG without Standby control.
 */
Tb6612fng::Tb6612fng(int aIn1, int aIn2, int pwmA, int bIn1, int bIn2, int pwmB) {
	_numChannels = 2;
	_pinStby = -1;
	setupChannelPins(TB6612FNG_CHANNEL_A, aIn1, aIn2, pwmA);
	setupChannelPins(TB6612FNG_CHANNEL_B, bIn1, bIn2, pwmB);
	setPwmFrequency(0); // Use default PWM frequency
	sleep();
}

/**
 * Constructor for both channels of the TB6612FNG with Standby control.
 */
Tb6612fng::Tb6612fng(int aIn1, int aIn2, int pwmA, int bIn1, int bIn2, int pwmB, int stby) {
	_numChannels = 2;
	_pinStby = stby;
	pinMode(_pinStby, OUTPUT);
	setupChannelPins(TB6612FNG_CHANNEL_A, aIn1, aIn2, pwmA);
	setupChannelPins(TB6612FNG_CHANNEL_B, bIn1, bIn2, pwmB);
	setPwmFrequency(0);
	sleep();
}

/**
 * Set PWM frequency for all motors.  The PWM frequency must be the same
 * for all pins in the same timer group.  See the Particle documentation for more information.
 * @param pwmFrequency PWM frequency in Hz.  Accepts 1..65535Hz.  Default is 500Hz.
 */
void Tb6612fng::setPwmFrequency(int pwmFrequency) {
	for (int channel = 0; channel < _numChannels; channel++) {
		setPwmFrequency(channel, pwmFrequency);
	}
}

/**
 * Set PWM frequency for a given motor channel.  The PWM frequency must be the same
 * for all pins in the same timer group.  See the Particle documentation for more information.
 * @param channel      TB6612FNG_CHANNEL_A or TB6612FNG_CHANNEL_B
 * @param pwmFrequency PWM frequency in Hz.  Accepts 1..65535Hz.  Default is 500Hz.
 */
void Tb6612fng::setPwmFrequency(int channel, int pwmFrequency) {
	if (pwmFrequency < 1 || pwmFrequency > 65535) {
		_pwmFrequency[channel] = 0;
	} else {
		_pwmFrequency[channel] = pwmFrequency;
	}
}

/**
 * Shuts down all motors.  If STBY is connected, also puts chip in Standby mode.
 */
void Tb6612fng::sleep() {
	if (_pinStby != -1) {
		digitalWrite(_pinStby, LOW);
	}

	for (int channel = 0; channel < _numChannels; channel++) {
		analogWrite(_pinPwm[channel], 0);
		digitalWrite(_pinIn1[channel], LOW);
		digitalWrite(_pinIn2[channel], LOW);
	}
}

/**
 * Configure the I/O direction for the pins on the given channel
 * @param channel TB6612FNG_CHANNEL_A or TB6612FNG_CHANNEL_B
 * @param in1     XIN1 pin number (where X is channel A or B)
 * @param in2     XIN2 pin number
 * @param pwm     PWMX pin number
 */
void Tb6612fng::setupChannelPins(int channel, int in1, int in2, int pwm) {
	_pinIn1[channel] = in1;
	_pinIn2[channel] = in2;
	_pinPwm[channel] = pwm;

	pinMode(_pinIn1[channel], OUTPUT);
	pinMode(_pinIn2[channel], OUTPUT);
	pinMode(_pinPwm[channel], OUTPUT);
}

/**
 * Set speed on all channels
 * @param speed -255..255.  Positive values cause forward rotation; negative values cause reverse.  High-impedance at 0.
 */
void Tb6612fng::setSpeed(int speed) {
	for (int channel = 0; channel < _numChannels; channel++) {
		setSpeed(channel, speed);
	}
}

/**
 * Set speed on all channels
 * @param channel TB6612FNG_CHANNEL_A or TB6612FNG_CHANNEL_B.
 * @param speed -255..255.  Positive values cause forward rotation; negative values cause reverse.  High-impedance at 0.
 */
void Tb6612fng::setSpeed(int channel, int speed) {
	if (speed != 0 && _pinStby != -1) {
		digitalWrite(_pinStby, HIGH);
	}

	if (speed == 0) {
		// TODO: allow configuration of short brake vs. high-impedance.
		// For now, this just goes to high-impedance.
		digitalWrite(_pinIn1[channel], LOW);
		digitalWrite(_pinIn2[channel], LOW);
	} else {
		if (speed > 0) {
			// forward
			digitalWrite(_pinIn1[channel], HIGH);
			digitalWrite(_pinIn2[channel], LOW);
		} else {
			// reverse
			digitalWrite(_pinIn1[channel], LOW);
			digitalWrite(_pinIn2[channel], HIGH);
		}

		if (_pwmFrequency[channel] > 0) {
			analogWrite(_pinPwm[channel], abs(speed), _pwmFrequency[channel]);
		} else {
			analogWrite(_pinPwm[channel], abs(speed));
		}
	}
}
