#pragma once

#include "Guacamole/Core/Layer.h"
#include "Guacamole/Events/ApplicationEvent.h"
#include "Guacamole/Events/KeyEvent.h"
#include "Guacamole/Events/MouseEvent.h"

namespace Guacamole {
	
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

        virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

        void BlockEvents(bool block) { m_BlockEvents = block; }
		
		void SetDarkThemeColors();
	private:
        bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
	
}
