#pragma once

#include <Nucleus/String.h>
#include <Nucleus/Exceptions.h>

#include "Archive.h"

namespace Nucleus {
    
    class Json {
    
    public:
        
        enum class DataType {
            String,
            Object,
            Boolean,
            Number,
            Null,
            List
        };

        Json() = default;

        explicit Json(Archive const& archive) : storage(archive) {
            
        }
        
        static Json parse(String const& fileName);

        static void writeArray(const DataArray* array, String& str, size_t indent);

        static void writeContainer(const Container* container, String& str, size_t indent, bool useIndent);

        NODISCARD void write(String const& fileName) const;

        NODISCARD FORCEINLINE Archive& archive() { return this->storage; }
        
    private:
        
        static DataType nextType(String const& data, size_t from);

        static void parseObject(String const& data, Container* storage);

        static void writeElement(Element* element, String& str, size_t indent);

        static void nextObject(String const& data, size_t from, size_t& begin, size_t& end);

        static void nextList(String const& data, size_t from, size_t& begin, size_t& end);

        static void nextString(String const& data, size_t start, size_t& begin, size_t& end);

        static void nextBool(String const& data, size_t from, size_t& begin, size_t& end);

        static void nextNull(String const& data, size_t from, size_t& begin, size_t& end);

        static void nextNumber(String const& data, size_t from, size_t& begin, size_t& end);
        
        static bool asFloatArray(Container* container, String const& tag, MutableArray<String> const& data);

        static bool asBoolArray(Container* container, String const& tag, MutableArray<String> const& data);

        static bool asObjectArray(Container* container, String const& tag, MutableArray<String> const& data);

        Archive storage;
        
    };

}
