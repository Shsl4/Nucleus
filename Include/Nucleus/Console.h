#pragma once

#include <Nucleus/CoreMacros.h>
#include <Nucleus/String.h>

#ifdef _WIN32

#include <Windows.h>

#endif

namespace Nucleus {
    
    class Console {

    public:

        const static inline String gray = "\33[0;30m";
        const static inline String red = "\33[0;31m";
        const static inline String green = "\33[0;32m";
        const static inline String yellow = "\33[0;33m";
        const static inline String blue = "\33[0;34m";
        const static inline String magenta = "\33[0;35m";
        const static inline String cyan = "\33[0;36m";
        const static inline String white = "\33[0;37m";
        
        const static inline String lightGray = "\33[0;90m";
        const static inline String lightRed = "\33[0;91m";
        const static inline String lightGreen = "\33[0;92m";
        const static inline String lightYellow = "\33[0;93m";
        const static inline String lightBlue = "\33[0;94m";
        const static inline String lightMagenta = "\33[0;95m";
        const static inline String lightCyan = "\33[0;96m";
        const static inline String lightWhite = "\33[0;97m";
        const static inline String reset = "\33[0m";

        template<typename... Args>
        static void log(String const& fmt, Args&&... args){
            
            write(String::format(fmt, std::forward<Args>(args)...));

        }
        
        template<typename... Args>
        static void error(String const& fmt, Args&&... args){
            
            write(red + String::format(fmt, std::forward<Args>(args)...) + reset, stderr);

        }

        virtual void init(bool threaded = false) = 0;

    private:

        static void write(String const& string, FILE* file = stdout) {

#ifdef _WIN32
            SetConsoleOutputCP(CP_UTF8);
            const auto size = static_cast<DWORD>(string.getSize());
            WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), string.begin(), size, nullptr, nullptr);
#else
            fwrite(string.begin(), sizeof(char), string.getSize(), file);
#endif

        }

    };
    
}
