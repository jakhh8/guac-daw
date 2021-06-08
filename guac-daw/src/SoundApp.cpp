#include <Guacamole.h>
#include <Guacamole/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Guacamole {
	
	class SoundApp : public Application
	{
	public:
		SoundApp()
			: Application("SoundApp")
		{
			PushLayer(new EditorLayer());
		}

		~SoundApp()
		{

		}
	};

	Application* CreateApplication()
	{
		return new SoundApp();
	}

}