#include <Nucleus/Console.h>

namespace Nucleus {
    
    void Console::flush(FILE* file) {
#ifdef _WIN64
        FlushFileBuffers(GetStdHandle(STD_OUTPUT_HANDLE));
#else
        fflush(file);
#endif
    }

    void Console::write(const String &string, FILE *file) {

#ifdef _WIN64
        SetConsoleOutputCP(CP_UTF8);
        const auto size = static_cast<DWORD>(string.size());
        WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), string.begin().get(), size, nullptr, nullptr);
#else
        fwrite(string.begin().get(), sizeof(char), string.size(), file);
#endif

    }

}
