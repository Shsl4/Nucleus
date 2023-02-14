#pragma once

#include <stdexcept>

namespace Nucleus {

    class Exceptions {

    public:

    class Exception : public std::runtime_error {

        public:

            explicit Exception(char const* const message) : std::runtime_error(message) {}

        };

        class BadKey : public Exception {

        public:

            explicit BadKey(char const* const message) : Exception(message) {}

        };

        class OutOfRange : public Exception {

        public:

            explicit OutOfRange(char const* const message) : Exception(message) {}

        };

        class BadFormat : public Exception {

        public:

            explicit BadFormat(char const* const message) : Exception(message) {}

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
        
        class ParseError : public Exception {

        public:
            
            explicit ParseError(char const* const message) : Exception(message) {}

        };

        class UnsupportedOperation : public Exception {

        public:

            explicit UnsupportedOperation(char const* const message) : Exception(message) {}

        };
        
    };
    
}
