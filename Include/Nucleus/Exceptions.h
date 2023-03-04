#pragma once

#include <stdexcept>
#include <Nucleus/StackTrace.h>

namespace Nucleus {

    class Exceptions {

    public:

        class Exception : public std::runtime_error {

        public:

            explicit Exception(char const* format, MutableArray<StackFrame> const& trace = StackTrace::getStackTrace());

            NODISCARD String formattedTrace() const;
            
        private:
            
            MutableArray<StackFrame> trace;

        };

        class LightException : public std::runtime_error {

        public:

            explicit LightException(char const* format) : std::runtime_error(format) {}
            
        };

        class OutOfRange : public Exception {

        public:

            explicit OutOfRange(char const* const message) : Exception(message) {}

        };

        
        class BadType : public Exception {

        public:

            explicit BadType(char const* const message) : Exception(message) {}

        };

        class NullPointer : public Exception {

        public:

            explicit NullPointer(char const* const message) : Exception(message) {}

            NullPointer() : Exception("Tried to dereference a nullptr.") {}

        };

        
        class UnsupportedOperation : public Exception {

        public:

            explicit UnsupportedOperation(char const* const message) : Exception(message) {}

        };

        class BadKey : public LightException {

        public:

            explicit BadKey(char const* const message) : LightException(message) {}

        };
        
        class BadFormat : public LightException {

        public:

            explicit BadFormat(char const* const message) : LightException(message) {}

        };
        
        class ParseError : public LightException {

        public:

            explicit ParseError(char const* const message) : LightException(message) {}

        };

    };

}
