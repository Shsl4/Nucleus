#include <Nucleus/Serialization/Json.h>
#include <Nucleus/File.h>
#include <Nucleus/Exceptions.h>
#include <Nucleus/Function.h>
#include <Nucleus/Map.h>

namespace Nucleus {

    const Map<Json::DataType, Function<bool(String const&, size_t, size_t&, size_t&)>> Json::funcMap = {
                
        { DataType::Boolean, &nextBool },
        { DataType::Number, &nextNumber },
        { DataType::Null, &nextNull },
        { DataType::String, &nextString },
        { DataType::Array, &nextArray },
        { DataType::Object, &nextObject },
                
    };
        
    const Map<Json::DataType, Function<void(Container*, String const&, String const&)>> Json::storeMap = {
                
        { DataType::Boolean, &storeBool },
        { DataType::Number, &storeNumber },
        { DataType::Null, &storeNull },
        { DataType::String, &storeString },
        { DataType::Array, &storeArray },
        { DataType::Object, &storeObject },
                
    };
    
    bool startsWith(String const& str, String const& test, const size_t from = 0) {

        if(test.size() > str.size()) return false;

        const size_t size = str.size();
        const size_t otherSize = test.size();

        size_t i = from, j = 0;

        while(i != size) {

            if(tolower(str[i++]) == tolower(test[j])) {

                if(++j == otherSize) { return true; }

                continue;
                
            }
            
            return false;

        }

        return false;
        
    }

        
    void removeExtraSpaces(String& str) {

        size_t begin = 0;
        size_t end = str.size();

        for(auto const& c : str) {
            if(c == ' ') ++begin;
            else { break; }
        }
        
        for(size_t i = end; i > 0; --i) {
            char c = str[i - 1];
            if(c == ' ') --end;
            else { break; }
        }

        str = str.substring(begin, end);
        
    }    

    String typeName(Json::DataType type) {
        switch (type) {
        case Json::DataType::String: return "string";
        case Json::DataType::Object: return "Object";
        case Json::DataType::Boolean: return "bool";
        case Json::DataType::Number: return "number";
        case Json::DataType::Null: return "null";
        case Json::DataType::Array: return "nested";
        }

        return "Unknown";
    }
    
    
    bool Json::nextObject(String const& data, const size_t from, size_t& begin, size_t& end) {

        size_t openBraces = 0;
        size_t closedBraces = 0;
        size_t bIdx = 0;

        for (size_t i = from; i < data.size(); ++i) {
            
            if (data[i] == '{') {
                if(openBraces == 0) {
                    bIdx = i;
                }
                ++openBraces;
            }

            if (data[i] == '}') {
                ++closedBraces;

                if(closedBraces == openBraces) {
                    begin = bIdx; end = i + 1; return true;
                }
                
            }
            
        }

        return false;
        
    }
    
    bool Json::nextArray(String const& data, const size_t from, size_t& begin, size_t& end) {

        size_t openBrackets = 0;
        size_t closedBrackets = 0;
        size_t bIdx = 0;

        for (size_t i = from; i < data.size(); ++i) {
            
            if (data[i] == '[') {
                if(openBrackets == 0) {
                    bIdx = i;
                }
                ++openBrackets;
            }

            if (data[i] == ']') {
                ++closedBrackets;

                if(closedBrackets == openBrackets) {
                    begin = bIdx; end = i + 1; return true;
                }
                
            }
            
        }

        return false;
        
    }

    bool Json::nextString(String const& data, const size_t start, size_t& begin, size_t& end) {

        bool found = false;
        size_t bIdx = 0;
        
        for (size_t i = start; i < data.size(); ++i) {

            if(data[i] == '"') {

                if(found) { begin = bIdx + 1; end = i; return true; }
                found = true;
                bIdx = i;
                
            }

            if(data[i] == '\\' && data[i+1] == '"') {
                found = false;
            }
            
        }
        
        return false;
        
    }

    bool Json::nextBool(String const& data, const size_t from, size_t& begin, size_t& end) {
        
        for (size_t i = from; i < data.size(); ++i) {

            if(startsWith(data, "true", i)) {
                begin = i; end = i + 4; return true;
            }

            if(startsWith(data, "false", i)) {
                begin = i; end = i + 5; return true;
            }

        }

        return false;
        
    }

    bool Json::nextNull(String const& data, const size_t from, size_t& begin, size_t& end) {
        
        for (size_t i = from; i < data.size(); ++i) {

            if(startsWith(data, "null", i)) {
                begin = i; end = i + 4; return true;
            }

        }

        return false;
        
    }

    bool Json::nextNumber(String const& data, const size_t from, size_t& begin, size_t& end) {

        bool start = false;
        bool dot = false;
        size_t bIdx = 0;
        
        for (size_t i = from; i < data.size(); ++i) {

            if(String::isInteger(data[i])) {
                
                if(!start) { start = true; bIdx = i; }
                
            }
            else if(start) {
                
                if(!dot && data[i] == '.') { dot = true; continue; }
                begin = bIdx; end = i; return true;
                
            }

        }

        if(start) {
            begin = bIdx;
            end = data.size();
            return true;
        }

        return false;
        
    }

    void Json::storeBool(Container* container, String const& tag, String const& data) {
        container->add(tag, data.toBool());
    }
    
    void Json::storeNumber(Container* container, String const& tag, String const& data) {
        container->add(tag, data.toFloat64());
    }
    void Json::storeObject(Container* container, String const& tag, String const& data) {
        parseObject(data, container->objectContainer(tag));
    }

    void Json::storeString(Container* container, String const& tag, String const& data) {
        container->add(tag, data);
    }
    
    void Json::storeNull(Container* container, String const& tag, String const&) {
        container->add(tag, nullptr);
    }
    
    void Json::storeArray(Container* container, String const& tag, String const& data) {

        size_t from = 0;
        size_t to = 0;

        auto string = data.substring(1, data.size() - 1).removeOccurrences("\n");
        
        removeExtraSpaces(string);

        const auto type = nextType(string, 0);
        
        Container* array = container->arrayContainer(tag);
                
        while (funcMap[type](string, from, from, to)) {
                    
            storeMap[type](array, nullptr, string.substring(from, to));
            from = to + 1;
                    
        }
        
    }

    Json::DataType Json::nextType(String const& data, const size_t from) {

        for (size_t i = from; i < data.size(); ++i) {
            if(data[i] == '"') { return DataType::String; }
            if(data[i] == '{') { return DataType::Object; }
            if(data[i] == '[') { return DataType::Array; }
            if(startsWith(data, "true", i)) { return DataType::Boolean; }
            if(startsWith(data, "false", i)) { return DataType::Boolean; }
            if(startsWith(data, "null", i)) { return DataType::Null; }
            if(String::isInteger(data[i])) { return DataType::Number; }
        }

        throw Exceptions::Exception("Invalid format");
        
    }

    void Json::parseObject(String const& data, Container* storage) {
        
        const auto proc = data.substring(data.firstIndexOf('{') + 1, data.lastIndexOf('}'));

        size_t current = 0;

        while (current < data.size()) {
            
            size_t from = 0;
            size_t to = 0;
            nextString(proc, current, from, to);

            if(from == 0 && to == 0) return;

            const auto tag = proc.substring(from, to);
            auto type = nextType(proc, to + 1);
            
            assert(funcMap[type](proc, to + 1, from, to));

            current = to + 1;
            
            storeMap[type](storage, tag, proc.substring(from, to));
            
        }
        
    }
    
    void Json::writeValue(const Value* element, String& str, String const& name, const size_t indent) {

        auto ind = String(4 * indent, ' ');

        String value = element->rawValue();
        
        if (name.isEmpty()) {
            
            str += String::format(R"({}{})", ind, value);
        }
        else {
            str += String::format(R"({}"{}": {})", ind, name, value);
        }
        
    }

    template<template<typename Ty> class ArrayType, typename T>
    void forEachBeforeLast(ArrayType<T> const& e, Function<void(T)> const& f, Function<void()> const& all) {

        for (size_t i = 0; i < e.size(); ++i) {
            f(e[i]);
            if(i != e.size() - 1) { all(); }
        }
        
    }
    
    void Json::writeObject(const ObjectContainer* container, String& str, String const& name, const size_t indent) {

        auto ind = String(4 * indent, ' ');
        
        if(name.isEmpty()) {
            str += String::format("{}{ \n", ind);
        }
        else {
            str += String::format("{}\"{}\": { \n", ind, name);
        }

        auto& map = container->dataMap();
        const size_t size = map.size();

        for (size_t i = 0; i < size; ++i) {

            auto& key = map.get(i).key();
            auto* value = map.get(i).value();
            
            if(const auto* arr = dynamic_cast<ArrayContainer*>(value)) {
                writeArray(arr, str, key, indent + 1);
            }
            else if(const auto* ct = dynamic_cast<ObjectContainer*>(value)) {
                writeObject(ct, str, key, indent + 1);
            }
            else  if(const auto* v = dynamic_cast<Value*>(value)) {
                writeValue(v, str, key, indent + 1);
            }
            
            if(i != size - 1) { str += ",\n"; }
            
        }
        
        str += String::format(" \n{}}", ind);
        
    }
        
    void Json::writeArray(const ArrayContainer* array, String& str, String const& name, const size_t indent) {

        auto ind = String(4 * indent, ' ');

        if(name.isEmpty()) {
            str += String::format("{}[ \n", ind, name);
        }
        else {
            str += String::format("{}\"{}\": [ \n", ind, name);
        }   

        auto& storage = array->dataArray();
        
        const size_t size = storage.size();

        for (size_t i = 0; i < size; ++i) {

            auto& element = storage[i];
            
            if(const auto* arr = dynamic_cast<ArrayContainer*>(element)) {
                writeArray(arr, str, nullptr, indent + 1);
            }
            else if(const auto* ct = dynamic_cast<ObjectContainer*>(element)) {
                writeObject(ct, str, nullptr, indent + 1);
            }
            else  if(const auto* v = dynamic_cast<Value*>(element)) {
                writeValue(v, str, nullptr, indent + 1);
            }
            
            if(i != size - 1) { str += ",\n"; }
            
        }
        
        
        str += String::format(" \n{}]", ind);
        
    }

    void Json::write(String const& fileName) const {

        String str;
        writeObject(storage.rootContainer(), str, nullptr, 0);

        if(File file; file.openForWriting(fileName)) {
            file.write(str.begin().get(), str.size());
        }
        
    }

    Json Json::parse(String const& fileName) {

        constexpr auto invalidString = "The file being parsed is an invalid JSON file.";

        File file;
        file.load(fileName);
        
        const String data = file.data();

        if(data.isEmpty()) return {};
                
        if(data.numberOccurrences('{') != data.numberOccurrences('}')) {
            throw Exceptions::Exception(invalidString);
        }

        if(data.numberOccurrences('[') != data.numberOccurrences(']')) {
            throw Exceptions::Exception(invalidString);
        }

        Json json = {};

        parseObject(data, json.archive().rootContainer());

        return json;
        
    }
    
}
