#include "gmpch.h"
#include "Guacamole/Utils/PlatformUtils.h"

#ifdef GM_PLATFORM_LINUX
    #include <string>

    namespace Guacamole
    {

        static std::string translateFilter(const char* inFilter)
        {
            std::string outFilter;

            for(int i = 0; i < strlen(inFilter); i++)
                outFilter.push_back(inFilter[i]);
            
            outFilter.push_back('|');

            for(int i = strlen(inFilter) + 1; inFilter[i] != '\0'; i++)
                outFilter.push_back(inFilter[i]);
            
            return outFilter;
        }

        std::optional<std::string> FileDialogs::OpenFile(const char* filter)
        {
            const char zenityP[] = "/usr/bin/zenity";
            char call[2048];

            sprintf(call,"%s  --file-selection --file-filter='%s' --file-filter='All files | *' --filename='assets' --title=\"%s\" ", zenityP, translateFilter(filter).c_str(), "Select file");

            char filename[260];
            FILE *f = popen(call,"r");
            fgets(filename, 260, f);

            int ret = pclose(f);

            if(ret == 0)
            {
                std::string file = filename;
                file.replace(file.find_last_of('\n'), 2, "");
                
                return file;
            }
            return std::nullopt;
        }
        
        std::optional<std::string> FileDialogs::SaveFile(const char* filter)
        {
            const char zenityP[] = "/usr/bin/zenity";
            char call[2048];

            sprintf(call,"%s  --file-selection --save --file-filter='%s' --file-filter='All files | *' --filename='Untitled.guac' --confirm-overwrite --title=\"%s\" ", zenityP, translateFilter(filter).c_str(), "Save file");

            char filename[260];
            FILE *f = popen(call,"r");
            fgets(filename, 260, f);

            int ret = pclose(f);

            if(ret == 0)
            {
                std::string file = filename;
                file.replace(file.find_last_of('\n'), 2, "");
                
                return file;
            }
            return std::nullopt;
        }
        
    }
#endif