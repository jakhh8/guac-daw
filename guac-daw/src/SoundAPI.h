#pragma once

#include <gmpch.h>

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

#include "alhelpers.h"

#include <stdio.h>
#include <math.h>

#include <thread>
#include <atomic>

#include <fstream>

#define BUF_SIZE 512*4
#define SAMPLE_RATE 44100

#define AMOUNT_OF_BUFFERS 4

namespace SoundAPI {

	class AudioIO
	{
	public:
		AudioIO()
		{
			m_Run = true;
			m_GlobalTime = 0.0f;

			m_Thread = std::thread(&AudioIO::NoiseMakerMain, this);
		}
		
		void NoiseMakerMain()
		{
			ALuint buffers[AMOUNT_OF_BUFFERS];
			bool queued[AMOUNT_OF_BUFFERS];
			uint lastUnqueued = AMOUNT_OF_BUFFERS - 1;
			size_t bytesToWrite = 1024;
			int error;
			short data[BUF_SIZE] = {0};
			ALCdevice* Device = alcOpenDevice(NULL); // select the "preferred device"
			ALuint source[1];

			ALCcontext* Context;
			if (Device) {
				Context = alcCreateContext(Device, NULL);
				alcMakeContextCurrent(Context);
			}

			// Check for EAX 2.0 support
			ALboolean g_bEAX = alIsExtensionPresent("EAX2.0");
			GM_INFO("Has enumerate all: {0}", alcIsExtensionPresent(Device, "ALC_ENUMERATE_ALL_EXT") ? "true" : "false");

			alGetError(); // clear error code

			// Create listener
			ALfloat listenerPos[]={0.0,0.0,0.0};
			ALfloat listenerVel[]={0.0,0.0,0.0};
			ALfloat listenerOri[]={0.0,0.0,-1.0, 0.0,1.0,0.0};

			// Position ...
			alListenerfv(AL_POSITION,listenerPos);
			if ((error = alGetError()) != AL_NO_ERROR)
			{
				GM_ERROR("alListenerfv POSITION : {0}", error);
				return;
			}

			// Velocity ...
			alListenerfv(AL_VELOCITY,listenerVel);
			if ((error = alGetError()) != AL_NO_ERROR)
			{
				GM_ERROR("alListenerfv VELOCITY : {0}", error);
				return;
			}

			// Orientation ...
			alListenerfv(AL_ORIENTATION,listenerOri);
			if ((error = alGetError()) != AL_NO_ERROR)
			{
				GM_ERROR("alListenerfv ORIENTATION : {0}", error);
				return;
			}

			// Generate Buffers
			alGenBuffers(AMOUNT_OF_BUFFERS, buffers);
			if ((error = alGetError()) != AL_NO_ERROR)
			{
				GM_ERROR("alGenBuffers buffer {0} : {1}", "all", alGetString(error));
				return;
			}

			for(uint i = 0; i < AMOUNT_OF_BUFFERS; i++)
			{
				alBufferData(buffers[i], AL_FORMAT_MONO16, &data, sizeof(data), SAMPLE_RATE);
				if ((error = alGetError()) != AL_NO_ERROR)
				{
					GM_ERROR("alBufferData buffer {0} : {1}", i, alGetString(error));
					alDeleteBuffers(1, &buffers[i]);
					return;
				}

				queued[i] = true;
			}

			// Generate Sources
			alGenSources(1, source);
			if ((error = alGetError()) != AL_NO_ERROR)
			{
				GM_ERROR("alGenSources 1 : {0}", alGetString(error));
				return;
			}
			
			alSourcei(source[0], AL_LOOPING, AL_FALSE);

			// Attach buffers to source
			alSourceQueueBuffers(source[0], AMOUNT_OF_BUFFERS, buffers);
			if ((error = alGetError()) != AL_NO_ERROR)
			{
				GM_ERROR("alSourceQueueBuffer AL_BUFFER {0} : {1}", "all", alGetString(error));
			}

			alSourcePlay(source[0]);

			double timestep = 1.0 / (double)SAMPLE_RATE;

			while(m_Run)
			{
				ALint processed;
				alGetSourcei(source[0], AL_BUFFERS_PROCESSED, &processed);

				// Play something
				for(unsigned int i = 0; i < BUF_SIZE; ++i)
				{
					if(m_UserFunc == nullptr)
						data[i] = 0;
					else
						data[i] = clip(round(m_UserFunc(m_GlobalTime + (double)i * timestep) * (SHRT_MAX - 500.0)), SHRT_MIN, SHRT_MAX);
				}

				while(processed > 1)
				{
					lastUnqueued = (lastUnqueued + 1) % AMOUNT_OF_BUFFERS;
					
					alSourceUnqueueBuffers(source[0], 1, &buffers[lastUnqueued]);
					if ((error = alGetError()) != AL_NO_ERROR)
					{
						GM_ERROR("alSourceUnqueueBuffer AL_BUFFER {0} : {1}", lastUnqueued, alGetString(error));
					}

					queued[lastUnqueued] = false;

					alBufferData(buffers[lastUnqueued], AL_FORMAT_MONO16, &data, sizeof(data), SAMPLE_RATE);
					if ((error = alGetError()) != AL_NO_ERROR)
					{
						GM_ERROR("alBufferData buffer {0} : {1}", lastUnqueued, alGetString(error));
						alDeleteBuffers(1, &buffers[lastUnqueued]);
						return;
					}

					alSourceQueueBuffers(source[0], 1, &buffers[lastUnqueued]);
					if ((error = alGetError()) != AL_NO_ERROR)
					{
						GM_ERROR("alSourceQueueBuffer AL_BUFFER {0} : {1}", lastUnqueued, alGetString(error));
					}

					m_GlobalTime = m_GlobalTime + (double)BUF_SIZE * timestep;
					queued[lastUnqueued] = true;
					processed--;

				}

				ALint state;
				alGetSourcei(source[0], AL_SOURCE_STATE, &state);

				if(state != AL_PLAYING)
				{
					alSourceStop(source[0]);
					alSourcePlay(source[0]);
					GM_WARN("Source restarted");
				}
			}

			// Exit
			Context = alcGetCurrentContext();
			Device = alcGetContextsDevice(Context);
			alcMakeContextCurrent(NULL);
			alcDestroyContext(Context);
			alcCloseDevice(Device);
		}

		void Stop()
		{
			m_Run = false;
			m_Thread.join();
		}

		void SetUserFunc(double(*userFunc)(double time))
		{
			m_UserFunc = userFunc;
		}

		double GetTime()
		{
			return m_GlobalTime;
		}

		int clip(int value, int min, int max)
		{
			if(value >= 0)
			{
				if(value > max)
					return max;
			}
			else
			{
				if(value < min)
					return min;
			}

			return value;
		}

	private:
		double(*m_UserFunc)(double time) = nullptr;
		std::atomic<bool> m_Run;
		std::atomic<double> m_GlobalTime;

		std::thread m_Thread;
	};

}