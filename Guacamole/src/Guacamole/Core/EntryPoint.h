#pragma once

#if defined(_WIN32) && !defined(__GNUC__) // Windows

	extern Guacamole::Application* Guacamole::CreateApplication();

	int main(int argc, char** argv)
	{
        Guacamole::Log::Init();

        GM_PROFILE_BEGIN_SESSION("Startup", "GuacamoleProfile-Startup.json");
		auto app = Guacamole::CreateApplication();
        GM_PROFILE_END_SESSION();

        GM_PROFILE_BEGIN_SESSION("Runtime", "GuacamoleProfile-Runtime.json");
		app->Run();
        GM_PROFILE_END_SESSION();

        GM_PROFILE_BEGIN_SESSION("Shutdown", "GuacamoleProfile-Shutdown.json");
		delete app;
        GM_PROFILE_END_SESSION();
	}

#else // Unix

	extern Guacamole::Application* Guacamole::CreateApplication();

	int main(int argc, char** argv)
	{
		Guacamole::Log::Init();

        GM_PROFILE_BEGIN_SESSION("Startup", "GuacamoleProfile-Startup.json");
		auto app = Guacamole::CreateApplication();
        GM_PROFILE_END_SESSION();

        GM_PROFILE_BEGIN_SESSION("Runtime", "GuacamoleProfile-Runtime.json");
		app->Run();
        GM_PROFILE_END_SESSION();

        GM_PROFILE_BEGIN_SESSION("Shutdown", "GuacamoleProfile-Shutdown.json");
		delete app;
        GM_PROFILE_END_SESSION();
	}

#endif