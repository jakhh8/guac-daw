#pragma once

#include <vector>
#include <math.h>
#include <string>

#define OSC_NONE 0
#define OSC_SINE 1
#define OSC_SQUARE 2
#define OSC_TRIANGLE 3
// NOTE: DO NOT USE!!!! This is very slow and will sound horrible in real time
#define OSC_SAW_ANA 4
#define OSC_SAW_DIG 5
#define OSC_NOISE 6

namespace Synth {

    static const double octaveBaseFrequency = 220.0; // A2
	static const double The12thRootOf2 = pow(2.0, 1.0 / 12.0);

	struct EnvelopeADSR
	{
		double AttackTime = 0.1;
		double DecayTime = 0.01;
		double ReleaseTime = 0.2;

		double SustainAmplitude = 0.8;
		double StartAmplitude = 1.0;

		double TriggerOnTime = 0.0;
		double TriggerOffTime = 0.0;

		bool IsNoteOn = false;

		EnvelopeADSR() = default;

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
			else if(TriggerOffTime != 0.0)
			{
				// Release
				amplitude = ((time - TriggerOffTime) / ReleaseTime) * (0.0 - SustainAmplitude) + SustainAmplitude;
			}

			if (amplitude < 0.0001)
				amplitude = 0.0;
			
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
			// None
			case OSC_NONE: return 0.0;
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
			case OSC_SAW_DIG: return (2.0 / M_PI) * (hertz * M_PI * fmod(time, 1.0 / hertz) - (M_PI / 2.0));
			// Noise
			case OSC_NOISE: return (2.0 * ((float)rand() / (float)RAND_MAX) - 1.0);
			// Unknown
			default: return 0.0;
		}
	}

	struct Instrument
	{
	public:
		double Volume = 1.0;
		std::vector<NoteADSR> Notes;
		bool IsSample = false;

		std::string Tag = "";

		virtual double Sound(double time) = 0;
	};

	struct CustomInstrument : public Synth::Instrument
	{
		struct OscData
		{
			uint32_t OscType = OSC_NONE;
			double FrequencyFactor = 1.0;
			double AmplitudeFactor = 1.0;

			double LFOFreq = 0.0;
			double LFOAmp = 0.0;
		};

		OscData Oscs[4];

		CustomInstrument()
		{
			for(int i = 0; i < 16; i++)
			{
				Synth::NoteADSR note;

				note.Note = i;

				Notes.push_back(note);
			}

			Tag = "Custom";
		}
		
		virtual double Sound(double time) override
		{
			double output = 0.0;

			for(auto note : Notes)
			{
				output += note.GetAmplitude(time) *
				(
					  Oscs[0].AmplitudeFactor * Synth::osc(Synth::calcFrequency(note.Note) * Oscs[0].FrequencyFactor, time, Oscs[0].OscType, Oscs[0].LFOFreq, Oscs[0].LFOAmp)
					+ Oscs[1].AmplitudeFactor * Synth::osc(Synth::calcFrequency(note.Note) * Oscs[1].FrequencyFactor, time, Oscs[1].OscType, Oscs[1].LFOFreq, Oscs[1].LFOAmp)
					+ Oscs[2].AmplitudeFactor * Synth::osc(Synth::calcFrequency(note.Note) * Oscs[2].FrequencyFactor, time, Oscs[2].OscType, Oscs[2].LFOFreq, Oscs[2].LFOAmp)
					+ Oscs[3].AmplitudeFactor * Synth::osc(Synth::calcFrequency(note.Note) * Oscs[3].FrequencyFactor, time, Oscs[3].OscType, Oscs[3].LFOFreq, Oscs[3].LFOAmp)
				);
			}

			return output * Volume;
		}
	};

	struct Bell : public CustomInstrument
	{
		Bell()
		{
			for(int i = 0; i < 16; i++)
			{
				Synth::NoteADSR note;
				
				note.AttackTime = 0.01;
				note.DecayTime = 1.0;
				note.SustainAmplitude = 0.0;
				note.ReleaseTime = 1.0;

				note.Note = i;

				Notes.push_back(note);
			}

			Oscs[0].OscType = OSC_SINE; Oscs[1].OscType = OSC_SINE; Oscs[2].OscType = OSC_SINE;
			Oscs[1].AmplitudeFactor = 0.5; Oscs[2].AmplitudeFactor = 0.25;
			Oscs[1].FrequencyFactor = 2.0; Oscs[2]. FrequencyFactor = 3.0;

			Tag = "Bell";
		}
	};

	struct Harmonica : public CustomInstrument
	{
		Harmonica()
		{
			for(int i = 0; i < 16; i++)
			{
				Synth::NoteADSR note;
				
				note.AttackTime = 0.05;
				note.DecayTime = 1.0;
				note.SustainAmplitude = 0.95;
				note.ReleaseTime = 0.1;

				note.Note = i;

				Notes.push_back(note);
			}

			Oscs[0].OscType = OSC_SQUARE; Oscs[1].OscType = OSC_SQUARE; Oscs[2].OscType = OSC_SQUARE; Oscs[3].OscType = OSC_NOISE;
			Oscs[0].LFOAmp = 0.001; Oscs[0].LFOFreq = 5.0;
			Oscs[1].AmplitudeFactor = 0.5; Oscs[2].AmplitudeFactor = 0.25; Oscs[3].AmplitudeFactor = 0.05;
			Oscs[1].FrequencyFactor = 1.5; Oscs[2].FrequencyFactor = 2.0;

			Tag = "Harmonica";
		}
	};

	struct Piano : public CustomInstrument
	{
		Piano()
		{
			for(int i = 0; i < 16; i++)
			{
				Synth::NoteADSR note;
				
				note.AttackTime = 0.05;
				note.SustainAmplitude = 0.95;

				note.Note = i;

				Notes.push_back(note);
			}

			Oscs[0].OscType = OSC_SINE; Oscs[1].OscType = OSC_SAW_DIG;
			Oscs[1].AmplitudeFactor = 0.5;
			Oscs[0].FrequencyFactor = 0.5;

			Tag = "Piano";
		}
	};

}