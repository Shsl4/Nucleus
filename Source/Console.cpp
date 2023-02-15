#include <Nucleus/Console.h>

namespace Nucleus {

    void Console::write(const String &string, FILE *file) {

#ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        const auto size = static_cast<DWORD>(string.getSize());
        WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), string.begin(), size, nullptr, nullptr);
#else
        fwrite(string.begin().get(), sizeof(char), string.size(), file);
#endif

    }

}
