#pragma once

#include <gmpch.h>

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

#include "alhelpers.h"

#include <math.h>

#include <thread>
#include <atomic>

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
			m_HasExited = false;
			m_GlobalTime = 0.0f;

			m_Thread = std::thread(&AudioIO::NoiseMakerMain, this);
		}
		
		void NoiseMakerMain()
		{
			m_Device = alcOpenDevice(NULL); // Open "preferred device"
			int error;
			bool queued[AMOUNT_OF_BUFFERS];
			uint lastUnqueued = AMOUNT_OF_BUFFERS - 1;
			short data[BUF_SIZE] = {0};

			if (m_Device) {
				m_Context = alcCreateContext(m_Device, NULL);
				alcMakeContextCurrent(m_Context);
			}

			// Check for EAX 2.0 support
			ALboolean g_bEAX = alIsExtensionPresent("EAX2.0");
			GM_INFO("Has enumerate all: {0}", alcIsExtensionPresent(m_Device, "ALC_ENUMERATE_ALL_EXT") ? "true" : "false");

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
			alGenBuffers(AMOUNT_OF_BUFFERS, m_Buffers);
			if ((error = alGetError()) != AL_NO_ERROR)
			{
				GM_ERROR("alGenBuffers buffer {0} : {1}", "all", alGetString(error));
				return;
			}

			for(uint i = 0; i < AMOUNT_OF_BUFFERS; i++)
			{
				alBufferData(m_Buffers[i], AL_FORMAT_MONO16, &data, sizeof(data), SAMPLE_RATE);
				if ((error = alGetError()) != AL_NO_ERROR)
				{
					GM_ERROR("alBufferData buffer {0} : {1}", i, alGetString(error));
					alDeleteBuffers(1, &m_Buffers[i]);
					return;
				}

				queued[i] = true;
			}

			// Generate Sources
			alGenSources(1, m_Source);
			if ((error = alGetError()) != AL_NO_ERROR)
			{
				GM_ERROR("alGenSources 1 : {0}", alGetString(error));
				return;
			}
			
			alSourcei(m_Source[0], AL_LOOPING, AL_FALSE);

			// Attach buffers to source
			alSourceQueueBuffers(m_Source[0], AMOUNT_OF_BUFFERS, m_Buffers);
			if ((error = alGetError()) != AL_NO_ERROR)
			{
				GM_ERROR("alSourceQueueBuffer AL_BUFFER {0} : {1}", "all", alGetString(error));
			}

			alSourcePlay(m_Source[0]);

			double timestep = 1.0 / (double)SAMPLE_RATE;

			while(m_Run)
			{
				ALint processed;
				alGetSourcei(m_Source[0], AL_BUFFERS_PROCESSED, &processed);

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
					
					alSourceUnqueueBuffers(m_Source[0], 1, &m_Buffers[lastUnqueued]);
					if ((error = alGetError()) != AL_NO_ERROR)
					{
						GM_ERROR("alSourceUnqueueBuffer AL_BUFFER {0} : {1}", lastUnqueued, alGetString(error));
					}

					queued[lastUnqueued] = false;

					alBufferData(m_Buffers[lastUnqueued], AL_FORMAT_MONO16, &data, sizeof(data), SAMPLE_RATE);
					if ((error = alGetError()) != AL_NO_ERROR)
					{
						GM_ERROR("alBufferData buffer {0} : {1}", lastUnqueued, alGetString(error));
						alDeleteBuffers(1, &m_Buffers[lastUnqueued]);
						return;
					}

					alSourceQueueBuffers(m_Source[0], 1, &m_Buffers[lastUnqueued]);
					if ((error = alGetError()) != AL_NO_ERROR)
					{
						GM_ERROR("alSourceQueueBuffer AL_BUFFER {0} : {1}", lastUnqueued, alGetString(error));
					}

					m_GlobalTime = m_GlobalTime + (double)BUF_SIZE * timestep;
					queued[lastUnqueued] = true;
					processed--;

				}

				ALint state;
				alGetSourcei(m_Source[0], AL_SOURCE_STATE, &state);

				if(state != AL_PLAYING)
				{
					alSourceStop(m_Source[0]);
					alSourcePlay(m_Source[0]);
					GM_WARN("Source restarted");
				}
			}

			Exit();
		}

		void Exit()
		{
			if(!m_HasExited)
			{
				alSourceStop(m_Source[0]);
				m_Context = alcGetCurrentContext();
				m_Device = alcGetContextsDevice(m_Context);
				alcMakeContextCurrent(NULL);
				alcDestroyContext(m_Context);
				alcCloseDevice(m_Device);
			}
		}

		void Stop()
		{
			m_Run = false;
			m_Thread.join();

			Exit();
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
		std::atomic<bool> m_HasExited;
		std::atomic<double> m_GlobalTime;

		std::thread m_Thread;

		// OpenAL stuff
		ALuint m_Buffers[AMOUNT_OF_BUFFERS];
		ALCdevice* m_Device;
		ALuint m_Source[1];
		ALCcontext* m_Context;
	};

}