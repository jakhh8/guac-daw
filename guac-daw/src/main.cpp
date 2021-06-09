/*#include "SoundAPI.h"

std::atomic<float> freq;

int MakeNoise(float time)
{
	return SinInt(2.0f * M_PI * freq * time);
	//sin((2.0f * M_PI * 440.0f * time)) > 0.0f ? 100 : -100;
}

int main()
{
	AudioIO noiseMaker;

	noiseMaker.SetUserFunc(MakeNoise);

	bool run = true;
	while(run)
	{
		switch(c)
		{
			case (int)'c': freq = 440.0f;
			case -1: freq = 0.0f;
		}
	}

	noiseMaker.Stop();

	return 0;
}*/