#pragma once

#include <Nucleus/CoreMacros.h>
#include <Nucleus/String.h>

#ifdef _WIN32

#include <Windows.h>

#endif

namespace Nucleus {
    
    class Console {

    public:

        const static inline char* gray = "\33[0;30m";
        const static inline char* red = "\33[0;31m";
        const static inline char* green = "\33[0;32m";
        const static inline char* yellow = "\33[0;33m";
        const static inline char* blue = "\33[0;34m";
        const static inline char* magenta = "\33[0;35m";
        const static inline char* cyan = "\33[0;36m";
        const static inline char* white = "\33[0;37m";
        
        const static inline char* lightGray = "\33[0;90m";
        const static inline char* lightRed = "\33[0;91m";
        const static inline char* lightGreen = "\33[0;92m";
        const static inline char* lightYellow = "\33[0;93m";
        const static inline char* lightBlue = "\33[0;94m";
        const static inline char* lightMagenta = "\33[0;95m";
        const static inline char* lightCyan = "\33[0;96m";
        const static inline char* lightWhite = "\33[0;97m";
        const static inline char* reset = "\33[0m";

        template<typename... Args>
        static void log(String const& fmt, Args&&... args){
            
            write(String::format(fmt, std::forward<Args>(args)...));

        }
        
        template<typename... Args>
        static void error(String const& fmt, Args&&... args){
            
            write(red + String::format(fmt, std::forward<Args>(args)...) + reset, stderr);

        }

    private:

        static void write(String const& string, FILE* file = stdout) {

#ifdef _WIN32
            SetConsoleOutputCP(CP_UTF8);
            const auto size = static_cast<DWORD>(string.getSize());
            WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), string.begin(), size, nullptr, nullptr);
#else
            fwrite(string.begin().get(), sizeof(char), string.size(), file);
#endif

        }

    };
    
}
