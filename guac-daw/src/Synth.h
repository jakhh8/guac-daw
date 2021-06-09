#pragma once

#include <vector>
#include <math.h>

#define OSC_SINE 0
#define OSC_SQUARE 1
#define OSC_TRIANGLE 2
#define OSC_SAW_ANA 3
#define OSC_SAW_DIG 4
#define OSC_NOISE 5

namespace Synth {

    static const double octaveBaseFrequency = 220.0; // A2
	static const double The12thRootOf2 = pow(2.0, 1.0 / 12.0);

	struct EnvelopeADSR
	{
		double AttackTime;
		double DecayTime;
		double ReleaseTime;

		double SustainAmplitude;
		double StartAmplitude;

		double TriggerOnTime;
		double TriggerOffTime;

		bool IsNoteOn;

		EnvelopeADSR()
		{
			AttackTime = 0.1;
			DecayTime = 0.01;
			ReleaseTime = 0.2;

			SustainAmplitude = 0.8;
			StartAmplitude = 1.0;

			TriggerOnTime = 0.0;
			TriggerOffTime = 0.0;

			IsNoteOn = false;
		}

		double GetAmplitude(double time)
		{
			double amplitude = 0.0;
			double lifetime = time - TriggerOnTime;

			if(IsNoteOn)
			{
				// Attack
				if(lifetime <= AttackTime)
					amplitude = (lifetime / AttackTime) * StartAmplitude;

				// Decay
				if(lifetime > AttackTime && lifetime <= AttackTime + DecayTime)
					amplitude = ((lifetime - AttackTime) / DecayTime) * (SustainAmplitude - StartAmplitude) + StartAmplitude;

				// Sustain
				if(lifetime > AttackTime + DecayTime)
					amplitude = SustainAmplitude;
			}
			else
			{
				// Release
				amplitude = ((time - TriggerOffTime) / ReleaseTime) * (0.0f - SustainAmplitude) + SustainAmplitude;
			}

			if (amplitude < 0.0001f)
				amplitude = 0.0f;
			
			return amplitude;
		}

		void NoteOn(double noteOnTime)
		{
			TriggerOnTime = noteOnTime;
			IsNoteOn = true;
		}

		void NoteOff(double noteOffTime)
		{
			TriggerOffTime = noteOffTime;
			IsNoteOn = false;
		}
	};

	struct NoteADSR : public EnvelopeADSR
	{
		int Note = 0;
	};

	static double calcFrequency(int i)
	{
		return octaveBaseFrequency * pow(The12thRootOf2, i);
	}

	static double w(double hertz)
	{
		return hertz * 2.0 * M_PI;
	}

	static double osc(double hertz, double time, int type = OSC_SINE, double LFOHertz = 0.0, double LFOAmplitude = 0.0)
	{
		double freq = w(hertz) * time + LFOAmplitude * hertz * sin(w(LFOHertz) * time);
		
		switch(type)
		{
			// Sine
			case OSC_SINE: return sin(freq);
			// Square
			case OSC_SQUARE: return sin(freq) > 0.0f ? 1.0 : -1.0;
			// Triangle
			case OSC_TRIANGLE: return asin(sin(freq)) * (2.0 / M_PI);
			// Saw (Analogue)
			case OSC_SAW_ANA:
			{
				double output = 0.0;

				for(double i = 1.0; i < 40.0; i++)
					output += sin(i * freq) / i;

				return output * (2.0 / M_PI);
			}
			// Saw (Digital)
			case OSC_SAW_DIG: return (2.0 / M_PI) * (freq / 2.0 * fmod(time, 1.0 / (freq / (2.0 * M_PI))) - M_PI_2) * 0.5;
			// Noise
			case OSC_NOISE: return (2.0 * ((float)rand() / (float)RAND_MAX) - 1.0);
			// Unknown
			default: return 0.0;
		}
	}

	struct Instrument
	{
		double Volume;
		std::vector<NoteADSR> Notes;

		virtual double Sound(double time) = 0;
	};

}