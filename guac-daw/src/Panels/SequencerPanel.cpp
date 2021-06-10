#include "SequencerPanel.h"
#include <imgui.h>
#include <imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "Guacamole/Scene/Components.h"
#include <cstring>

#include <bits/stdc++.h>

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */
#ifdef _MSVC_LANG
  #define _CRT_SECURE_NO_WARNINGS
#endif

namespace Guacamole {

	void DrawOscGui(Synth::CustomInstrument* instrument)
	{
		for(int i = 0; i < 4; i++)
		{
			// Use pointer to Oscs[i] as id
			if(ImGui::TreeNodeEx((void*)&instrument->Oscs[i], ImGuiTreeNodeFlags_SpanAvailWidth, "Oscillator %d", i + 1))
			{
				float width = ImGui::GetContentRegionAvail().x / 2.0f;
				
				std::string oscType;

				switch (instrument->Oscs[i].OscType)
				{
					case OSC_NONE: oscType = "None"; break;
					case OSC_SINE: oscType = "Sine"; break;
					case OSC_SQUARE: oscType = "Square"; break;
					case OSC_TRIANGLE: oscType = "Triangle"; break;
					case OSC_SAW_ANA: oscType = "Saw (analog)"; break;
					case OSC_SAW_DIG: oscType = "Saw (digital)"; break;
					case OSC_NOISE: oscType = "Noise"; break;
					
					default: oscType = "Unknown!"; break;
				}
				
				// Instrument type
				std::hash<std::string> hasher;
				ImGui::PushID(hasher(oscType.c_str() + i));
				
				ImGui::Columns(2);

				ImGui::SetColumnWidth(0, width);
				ImGui::Text("Oscillator type");
				ImGui::NextColumn();

				if(ImGui::BeginCombo("", oscType.c_str()))
				{
					float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2;
					ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0.0f, 0.5f});

					if(ImGui::Button("None", {ImGui::GetContentRegionAvail().x, lineHeight}))
						instrument->Oscs[i].OscType = OSC_NONE;
					if(ImGui::Button("Sine", {ImGui::GetContentRegionAvail().x, lineHeight}))
						instrument->Oscs[i].OscType = OSC_SINE;
					if(ImGui::Button("Square", {ImGui::GetContentRegionAvail().x, lineHeight}))
						instrument->Oscs[i].OscType = OSC_SQUARE;
					if(ImGui::Button("Triangle", {ImGui::GetContentRegionAvail().x, lineHeight}))
						instrument->Oscs[i].OscType = OSC_TRIANGLE;
					if(ImGui::Button("Saw (analog)", {ImGui::GetContentRegionAvail().x, lineHeight}))
						instrument->Oscs[i].OscType = OSC_SAW_ANA;
					if(ImGui::Button("Saw (digital)", {ImGui::GetContentRegionAvail().x, lineHeight}))
						instrument->Oscs[i].OscType = OSC_SAW_DIG;
					if(ImGui::Button("Noise", {ImGui::GetContentRegionAvail().x, lineHeight}))
						instrument->Oscs[i].OscType = OSC_NOISE;

					ImGui::PopStyleVar();

					ImGui::EndCombo();
				}

				ImGui::Columns(1);

				ImGui::PopID();

				// Amplitude factor
				ImGui::PushID(hasher("Amplitude" + i));
				
				ImGui::Columns(2);

				ImGui::SetColumnWidth(0, width);
				ImGui::Text("Amplitude factor");
				ImGui::NextColumn();
				float ampFac = instrument->Oscs[i].AmplitudeFactor;
				if(ImGui::DragFloat("", &ampFac, 0.001f, 0.0f, 1.0f))
					instrument->Oscs[i].AmplitudeFactor = ampFac;
				
				ImGui::Columns(1);

				ImGui::PopID();

				// Freq
				ImGui::PushID(hasher("Frequency" + i));
				
				ImGui::Columns(2);

				ImGui::SetColumnWidth(0, width);
				ImGui::Text("Frequency factor");
				ImGui::NextColumn();
				float freqFac = instrument->Oscs[i].FrequencyFactor;
				if(ImGui::DragFloat("", &freqFac, 0.001f, -3.0f, 3.0f))
					instrument->Oscs[i].FrequencyFactor = freqFac;
				
				ImGui::Columns(1);
				
				ImGui::PopID();

				// LFOAmp
				ImGui::PushID(hasher("LFOAmp" + i));
				
				ImGui::Columns(2);

				ImGui::SetColumnWidth(0, width);
				ImGui::Text("LFO amplitude factor");
				ImGui::NextColumn();
				float lfoAmp = instrument->Oscs[i].LFOAmp;
				if(ImGui::DragFloat("", &lfoAmp, 0.001f, 0.0f, 1.0f))
					instrument->Oscs[i].LFOAmp = freqFac;
				
				ImGui::Columns(1);
				
				ImGui::PopID();

				// LFOFreq
				ImGui::PushID(hasher("LFOFreq" + i));
				
				ImGui::Columns(2);

				ImGui::SetColumnWidth(0, width);
				ImGui::Text("LFO frequency");
				ImGui::NextColumn();
				float lfoFreq = instrument->Oscs[i].LFOFreq;
				if(ImGui::DragFloat("", &lfoFreq, 0.001f, -3.0f, 3.0f))
					instrument->Oscs[i].LFOFreq = freqFac;
				
				ImGui::Columns(1);
				
				ImGui::PopID();

				ImGui::TreePop();
			}
		}
	}
	
	void SequencerPanel::OnImGuiRender() 
	{
		auto tracks = m_Sequencer.GetTracks();
		
		ImGui::Begin("Sequencer");
		
		for(int i = 0; i < tracks->size(); i++)
		{
			DrawTrackNode(i);
		}

		if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = UINT32_MAX;

		// Right-click on blank space
		if(ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if(ImGui::BeginMenu("Create"))
			{
				if(ImGui::MenuItem("Empty"))
				{
					m_SelectionContext = tracks->size();
					tracks->push_back(Synth::Track(nullptr));
				}
				
				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if(m_SelectionContext != UINT32_MAX)
		{
			float width = ImGui::GetContentRegionAvail().x / 2.0f;
			auto& tag = tracks->at(m_SelectionContext).GetInstrument()->Tag;

			// Reset
			ImGui::PushID(tag.c_str());

			if(ImGui::BeginCombo("", "Reset to preset"))
			{
				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2;
				ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, {0.0f, 0.5f});

				if(ImGui::Button("Piano", {ImGui::GetContentRegionAvail().x, lineHeight}))
				{
					tracks->at(m_SelectionContext).SetInstrument(new Synth::Piano());
					tracks->at(m_SelectionContext).GetInstrument()->Tag = "Piano";
					ImGui::CloseCurrentPopup();
				}
				if(ImGui::Button("Harmonica", {ImGui::GetContentRegionAvail().x, lineHeight}))
				{
					tracks->at(m_SelectionContext).SetInstrument(new Synth::Harmonica());
					tracks->at(m_SelectionContext).GetInstrument()->Tag = "Harmonica";
					ImGui::CloseCurrentPopup();
				}
				if(ImGui::Button("Bell", {ImGui::GetContentRegionAvail().x, lineHeight}))
				{
					tracks->at(m_SelectionContext).SetInstrument(new Synth::Bell());
					tracks->at(m_SelectionContext).GetInstrument()->Tag = "Bell";
					ImGui::CloseCurrentPopup();
				}
				if(ImGui::Button("Custom", {ImGui::GetContentRegionAvail().x, lineHeight}))
				{
					tracks->at(m_SelectionContext).SetInstrument(new Synth::CustomInstrument());
					tracks->at(m_SelectionContext).GetInstrument()->Tag = "Custom";
					ImGui::CloseCurrentPopup();
				}
				
				ImGui::PopStyleVar();

				ImGui::EndCombo();
			}

			ImGui::PopID();

			// Tag
			ImGui::Columns(2);

			ImGui::SetColumnWidth(0, width);
			ImGui::Text("Label");
			ImGui::NextColumn();

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));
			if(ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}

			ImGui::Columns(1);

			// Draw custom osc gui
			if(!tracks->at(m_SelectionContext).GetInstrument()->IsSample)
				DrawOscGui((Synth::CustomInstrument*)tracks->at(m_SelectionContext).GetInstrument());

			// TODO: Show spline editor
		}
		
		ImGui::End();
	}
	
	void SequencerPanel::DrawTrackNode(uint32_t trackIndex)
	{
		auto tracks = m_Sequencer.GetTracks();
		auto instrument = tracks->at(trackIndex).GetInstrument();
		auto& tag = instrument->Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == trackIndex) ? ImGuiTreeNodeFlags_Selected : 0);
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)trackIndex, flags, tag.c_str());
		if(ImGui::IsItemClicked())
			m_SelectionContext = trackIndex;

		// Right-click on entity
		bool entityDeleted = false;
		if(ImGui::BeginPopupContextItem())
		{
			if(ImGui::BeginMenu("Create"))
			{
				if(ImGui::MenuItem("Empty"))
				{
					m_SelectionContext = tracks->size();
					tracks->push_back(Synth::Track(nullptr));
				}
				
				ImGui::EndMenu();
			}
			if(ImGui::MenuItem("Delete"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if(opened)
		{
			ImGui::TreePop();
		}

		if(entityDeleted)
		{
			delete instrument;
			auto it = tracks->begin();
			std::advance(it, trackIndex);
			tracks->erase(it);
			if(m_SelectionContext == trackIndex)
				m_SelectionContext = {};
		}
	}

}