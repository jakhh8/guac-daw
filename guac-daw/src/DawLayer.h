#pragma once

#include <Guacamole.h>
#include "Panels/SequencerPanel.h"
#include "SoundAPI.h"
#include "Synth.h"
#include "Sequencer.h"

namespace Guacamole {

	class DawLayer : public Layer
	{
	public:
		DawLayer();
		virtual ~DawLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

		static double MakeNoise(double time);
	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveScene();
		void SaveSceneAs();
	private:
        bool m_ViewportFocused = false, m_ViewportHovered = false;

		std::string m_CurrentFile = "";

		// Audio/Synth
		SoundAPI::AudioIO m_AudioIO;
		static Synth::Sequencer s_Sequencer;

		Entity m_HoveredEntity;

		// Panels
		SequencerPanel m_SequencerPanel;
	};

}