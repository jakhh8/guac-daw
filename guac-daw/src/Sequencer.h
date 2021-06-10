#pragma once

#include "Synth.h"
#include <cstdint>
#include <vector>

namespace Synth {

    class MidiSequence
    {
    public:
        MidiSequence() = default;
    private:
    };

    class Track
    {
    public:
        Track(Instrument* instrument)
            : m_Instrument(instrument) {}
        Track() = default;

        void SetInstrument(Instrument* instrument) { std::string tag = m_Instrument->Tag; m_Instrument = instrument; m_Instrument->Tag = tag; }
        Instrument* GetInstrument() { return m_Instrument; }
    private:
        Instrument* m_Instrument;

        std::vector<MidiSequence> m_Sequences;
    };

    class Sequencer
    {
    public:
        Sequencer(std::vector<Track> tracks)
            : m_Tracks(tracks) {}
        Sequencer() = default;

        std::vector<Track>* GetTracks() { return &m_Tracks; }
    private:
        uint64_t m_Offset = 0;

        std::vector<Track> m_Tracks;
    };
    
}