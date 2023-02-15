#pragma once

#include <Nucleus/String.h>

namespace Nucleus {

    /**
     *  \brief A struct that holds information of a single stack frame including the function name, file name,
     *  line number and library name.
     */
    struct StackFrame {

        StackFrame() = default;

        StackFrame(const char* function, const char* file, const char* library, Int64 line);

        bool operator==(StackFrame const& other) const = default;

        /*!
         *  \brief Formats the stack frame into a readable string.
         */
        NODISCARD String format() const;

        String functionName;
        String fileName;
        String filePath;
        String libraryName;
        Int64 lineNumber = -1;

    };

    /**
     *  \brief A utility class that allows to get the current stack frames.
     */
    class StackTrace {

    public:

        StackTrace() = delete;

        /*!
         *  \brief Creates a StackFrame array from the current stack.
         *
         *  \return The StackFrame array
         */
        static MutableArray<StackFrame> getStackTrace();

    };

}
