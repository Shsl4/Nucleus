#pragma once

#include <stdexcept>

namespace Nucleus {

    class Exceptions {

    public:

        class BadKey : public std::runtime_error {

        public:
            
            explicit BadKey(char const* const message) : std::runtime_error(message) {}

        };

        class OutOfRange : public std::runtime_error {

        public:

            explicit OutOfRange(char const* const message) : std::runtime_error(message) {}

        };

        class BadFormat : public std::runtime_error {

        public:

            explicit BadFormat(char const* const message) : std::runtime_error(message) {}

        };
        
        class NullPointer : public std::runtime_error {

        public:
            
            explicit NullPointer(char const* const message) : std::runtime_error(message) {}

            NullPointer() : std::runtime_error("Tried to dereference a nullptr.") {}

        };
        
        class ParseError : public std::runtime_error {

        public:
            
            explicit ParseError(char const* const message) : std::runtime_error(message) {}

        };
        
    };
    
}
