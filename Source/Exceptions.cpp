#include <Nucleus/Exceptions.h>
#include <Nucleus/StackTrace.h>

namespace Nucleus {

    Exceptions::Exception::Exception(const char *format, MutableArray<StackFrame> const& trace) : std::runtime_error(format), trace(trace) {
        
    }
    
    String Exceptions::Exception::formattedTrace() const{
        
        String fmt;
        
        for(size_t i = 2; i < trace.size(); ++i){
            fmt += String::format("[{}] {}\n", i - 2, trace[i].format());
        }
        
        return fmt;
        
    }
    
}

void Nucleus::throwException(const char* message) {
    throw Exceptions::Exception(message);
}
