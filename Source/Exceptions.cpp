#include <Nucleus/Exceptions.h>
#include <Nucleus/StackTrace.h>

namespace Nucleus{

    Exceptions::Exception::Exception(const char *format, MutableArray<StackFrame> const& trace) : std::runtime_error(format) {
        
        for(size_t i = 1; i < trace.size(); ++i){
            formattedTrace += String::format("[{}] {}\n", i, trace[i].format());
        }
        
    }
}
