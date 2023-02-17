#include <Nucleus/Exceptions.h>
#include <Nucleus/StackTrace.h>

namespace Nucleus{

    Exceptions::Exception::Exception(const char *format, MutableArray<StackFrame> const& trace) : std::runtime_error(format), trace(trace) {
        
    }
    
    String Exceptions::Exception::formattedTrace() const{
        
        String fmt;
        
        for(size_t i = 3; i < trace.size(); ++i){
            fmt += String::format("[{}] {}\n", i - 3, trace[i].format());
        }
        
        return fmt;
        
    }
    
}
