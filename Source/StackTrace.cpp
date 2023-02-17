#include <Nucleus/StackTrace.h>

#ifdef _WIN64

#include <Windows.h>
#include <DbgHelp.h>
#include <winnt.h>

#else

#include <execinfo.h>
#include <cxxabi.h>

#endif

namespace Nucleus {

    StackFrame::StackFrame(const char* function, const char* file, const char* library, Int64 line) : lineNumber(file ? line : -1) {

        if (function) {
            this->functionName = function;
        }
        else {
            this->functionName = "External function";
        }

        if (file) {

            const String string = file;
            const MutableArray<String> storage = string.split('\\');

            this->filePath = file;
            this->fileName = storage[storage.size() - 1];

        }
        else {
            this->fileName = "External file";
        }

        if (library) {

            const String string = library;
            const MutableArray<String> storage = string.split('\\');

            this->libraryName = storage[storage.size() - 1];

        }
        else {
            this->libraryName = "External library";
        }

    }

    String StackFrame::format() const {

        if(this->lineNumber == -1) {

            return String::format("{} in library {}",
                                  this->functionName,
                                  this->libraryName);

        }

        return String::format("{} in file {} at line {} | ({})",
                              this->functionName,
                              this->fileName,
                              this->lineNumber,
                              this->libraryName);
    }

#ifdef _WIN64

    Array<StackFrame> StackTrace::getStackTrace() {

    Array<StackFrame> stack;
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();
    STACKFRAME64 frame{};
    CONTEXT context;
    DWORD64 displacement = 0;
    DWORD offset = 0;

    char* name = Allocator<char>::alloc(MAX_PATH);
    char* symData = Allocator<char>::alloc(sizeof(IMAGEHLP_SYMBOL64) + MAX_PATH);
    char* lineData = Allocator<char>::alloc(sizeof(IMAGEHLP_LINE64));
    char* moduleData = Allocator<char>::alloc(sizeof(IMAGEHLP_MODULE64));

    const auto symbol = reinterpret_cast<IMAGEHLP_SYMBOL64*>(symData);
    const auto line = reinterpret_cast<IMAGEHLP_LINE64*>(lineData);
    const auto mod = reinterpret_cast<IMAGEHLP_MODULE64*>(moduleData);

    symbol->MaxNameLength = MAX_PATH;
    symbol->SizeOfStruct = sizeof(*symbol);

    line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);

    mod->SizeOfStruct = sizeof(IMAGEHLP_MODULE64);

    RtlCaptureContext(&context);

    frame.AddrPC.Offset = context.Rip;
    frame.AddrStack.Offset = context.Rsp;
    frame.AddrFrame.Offset = context.Rbp;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrStack.Mode = AddrModeFlat;
    frame.AddrFrame.Mode = AddrModeFlat;

    if (SymInitialize(process, nullptr, true)) {

        constexpr DWORD machineType = IMAGE_FILE_MACHINE_AMD64;

        bool skipped = false;

        while (StackWalk64(machineType, process, thread, &frame, &context, nullptr,
                           SymFunctionTableAccess64, SymGetModuleBase64, nullptr)) {

            // Skip the first frame which corresponds to this function.
            if(!skipped) { skipped = true; continue; }

            const bool foundSymbol = SymGetSymFromAddr64(process, frame.AddrPC.Offset, &displacement, symbol);

            const bool foundModule = SymGetModuleInfo64(process, frame.AddrPC.Offset, mod);

            const bool foundLine = SymGetLineFromAddr64(process, frame.AddrPC.Offset, &offset, line);

            UnDecorateSymbolName(symbol->Name, name, sizeof(name), UNDNAME_COMPLETE);

            stack += StackFrame(foundSymbol ? symbol->Name : nullptr,
                                foundLine ? line->FileName : nullptr,
                                foundModule ? mod->ImageName : nullptr,
                                foundLine ? line->LineNumber : 0);

        }

    }

    Allocator<char>::release(name);
    Allocator<char>::release(symData);
    Allocator<char>::release(lineData);
    Allocator<char>::release(moduleData);

    return stack;

}

#elif defined(__APPLE__)

    MutableArray<StackFrame> StackTrace::getStackTrace() {

        constexpr Int32 maxSize = 128;
        void* allocData[maxSize];

        Int32 frameCount = backtrace(allocData, maxSize);
        char** symbols = backtrace_symbols(allocData, frameCount);

        MutableArray<String> symArray;

        for (size_t i = 0; i < frameCount; ++i) {
            symArray += symbols[i];
        }

        auto stack = MutableArray<StackFrame>(maxSize);
        
        for (const auto& sym : symArray) {

            MutableArray<String> parts = sym.split(' ');

            String libName = parts[1];
            String symName = parts[3];

            int status = -1;

            char* demangled = abi::__cxa_demangle(symName.begin().get(), nullptr, nullptr, &status);

            stack += StackFrame(status == 0 ? demangled : symName.begin().get(), nullptr, libName.begin().get(), -1);

            if (status == 0) {
                free(demangled);
            }

        }

        return stack;

    }

#else

    MutableArray<StackFrame> StackTrace::getStackTrace() {

    return {};

}

#endif

}
