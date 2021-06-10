#pragma once

#include "Guacamole/Core/Base.h"
#include "../Synth.h"
#include "../Sequencer.h"

namespace Guacamole {

	class SequencerPanel
	{
	public:
		SequencerPanel() = default;
		SequencerPanel(Synth::Sequencer& sequencer)
			: m_Sequencer(sequencer) {}

		void OnImGuiRender();

		void SetSequencer(Synth::Sequencer& sequencer) { m_Sequencer = sequencer; }

		uint32_t GetSelectedTrack() const { return m_SelectionContext; }
		void SetSelectedTrack(uint32_t track) { m_SelectionContext = track; }
	private:
		void DrawTrackNode(uint32_t track);
	private:
		Synth::Sequencer& m_Sequencer;
		uint32_t m_SelectionContext = UINT32_MAX;
	};
	
}