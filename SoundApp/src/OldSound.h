
/* pa_simple* s;
pa_sample_spec ss;

ss.format = PA_SAMPLE_S16NE;
ss.channels = 2;
ss.rate = 44100;

s = pa_simple_new(NULL, 				// Use the default server.
                "Fooapp",				// Our application's name.
                PA_STREAM_PLAYBACK,
                NULL,					// Use the default device.
                "Music",				// Description of our stream.
                &ss,					// Our sample format.
                NULL,					// Use default channel map
                NULL,					// Use default buffering attributes.
                NULL					// Ignore error code.
                );

double timestep = 1.0 / (double)SAMPLE_RATE;
while(m_Run)
{
    //Play something
    for(unsigned int i = 0; i < BUF_SIZE; ++i)
    {
        if(m_UserFunc == nullptr)
            buffers[i] = 0;
        else
            buffers[i] = clip(round(m_UserFunc(m_GlobalTime + (double)i * timestep) * (SHRT_MAX - 500.0)), SHRT_MIN, SHRT_MAX);
    }

    if(pa_simple_write(s, buffers, bytesToWrite, &error) < 0)
    {
        fprintf(stderr, __FILE__":pa_simple_write() failed: %s\n", pa_strerror(error));
    }

    m_GlobalTime = m_GlobalTime + (double)BUF_SIZE * timestep;
}

pa_simple_free(s); */