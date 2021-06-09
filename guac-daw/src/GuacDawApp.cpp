#include <Guacamole.h>
#include <Guacamole/Core/EntryPoint.h>

#include "DawLayer.h"

namespace Guacamole {
	
	class GuacDawApp : public Application
	{
	public:
		GuacDawApp()
			: Application("Guac Daw")
		{
			PushLayer(new DawLayer());
		}

		~GuacDawApp()
		{

		}
	};

	Application* CreateApplication()
	{
		return new GuacDawApp();
	}

}