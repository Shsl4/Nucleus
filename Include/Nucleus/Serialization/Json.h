#pragma once

#include <Nucleus/String.h>
#include <Nucleus/Function.h>

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
            Array
        };

        Json() = default;

        Json(Archive const& archive) : storage(archive) {
            
        }

        static Json parse(String const& fileName);

        void write(String const& fileName) const;

        NODISCARD FORCEINLINE Archive& archive() { return this->storage; }
        
    private:
        
        static DataType nextType(String const& data, size_t from);

        static void parseObject(String const& data, Container* storage);

        static void writeValue(const Value* element, String& str, String const& name, size_t indent);
        
        static void writeArray(const ArrayContainer* array, String& str, String const& name, size_t indent);

        static void writeObject(const ObjectContainer* container, String& str, String const& name, size_t indent);
        
        static bool nextObject(String const& data, size_t from, size_t& begin, size_t& end);

        static bool nextArray(String const& data, size_t from, size_t& begin, size_t& end);

        static bool nextString(String const& data, size_t start, size_t& begin, size_t& end);

        static bool nextBool(String const& data, size_t from, size_t& begin, size_t& end);

        static bool nextNull(String const& data, size_t from, size_t& begin, size_t& end);

        static bool nextNumber(String const& data, size_t from, size_t& begin, size_t& end);
        
        static void storeBool(Container* container, String const& tag, String const& data);

        static void storeNumber(Container* container, String const& tag, String const& data);

        static void storeObject(Container* container, String const& tag, String const& data);

        static void storeString(Container* container, String const& tag, String const& data);
        
        static void storeNull(Container* container, String const& tag, String const& data);

        static void storeArray(Container* container, String const& tag, String const& data);

        static const Map<DataType, Function<void(Container*, String const&, String const&)>> storeMap;
        
        static const Map<DataType, Function<bool(String const&, size_t, size_t&, size_t&)>> funcMap;
        
        Archive storage;
        
    };

}
