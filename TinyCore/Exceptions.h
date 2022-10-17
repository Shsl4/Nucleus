#pragma once

#include <stdexcept>

namespace TinyCore {

    class BadKeyException : public std::runtime_error {

    public:
            
        explicit BadKeyException(char const* const message) : std::runtime_error(message) {}

    };

    class NullPointerException : public std::runtime_error {

    public:
            
        explicit NullPointerException(char const* const message) : std::runtime_error(message) {}

        NullPointerException() : std::runtime_error("Tried to dereference a nullptr.") {}

    };
    
}
