#include <Nucleus/Json.h>
#include <Nucleus/File.h>
#include <Nucleus/Exceptions.h>
#include <Nucleus/Console.h>
#include <Nucleus/Function.h>
#include <Nucleus/Map.h>

namespace Nucleus {
    class Container;

    Json::Field::Field(String name, String data, DataType type): name(std::move(name)),
                                                                 data(std::move(data)), type(type) {}

    bool Json::Field::operator==(const Field& other) const {

        return name == other.name && data == other.data; 
                
    }

    void Json::Field::write(String& str, const size_t indent) const {

        auto ind = String(4 * indent, ' ');

        if(type == DataType::String) {
            str += String::format(R"({}"{}": "{}")", ind, name, data);
        }
        else {
            str += String::format(R"({}"{}": {})", ind, name, data);
        }
        
    }

    Json::Object::Object(String name): name(std::move(name)) {
                
    }

    bool Json::Object::operator==(const Object& other) const {

        return name == other.name && fields == other.fields && objects == other.objects; 
                
    }

    bool Json::Object::getBool(String const& name) const {

        for (auto const& field : fields) {
                    
            if(field.name == name) {
                        
                if (field.type == DataType::Boolean) {

                    return field.data.toBool();
                            
                }

                throw Exceptions::BadType("The variable is not a boolean.");
                        
            }
                    
        }

        throw Exceptions::BadType("The variable does not exist.");
                
    }

    String Json::Object::getString(String const& name) const {

        for (auto const& field : fields) {
                    
            if(field.name == name) {
                        
                if (field.type == DataType::String) {

                    return field.data;
                            
                }

                throw Exceptions::BadType("The variable is not a string.");
                        
            }
                    
        }

        throw Exceptions::BadType("The variable does not exist.");
        
    }

    void Json::Object::write(String& str, const size_t indent, const bool useIndent) const {

        auto ind = String(4 * indent * useIndent, ' ');
        
        if(name.isEmpty()) {
            str += String::format("{}{ \n", ind);
        }
        else {
            str += String::format("{}\"{}\": { \n", ind, name);
        }
        
        if(objects.size() > 0) {
            
            for (size_t i = 0; i < objects.size() - 1; ++i) {
                objects[i].write(str, indent + useIndent);
                str += ",\n";
            }

            objects[objects.size() - 1].write(str, indent + useIndent);
            
        }

        if(fields.size() > 0) {

            if(objects.size() > 0) { str += ",\n"; }
            
            for (size_t i = 0; i < fields.size() - 1; ++i) {
                fields[i].write(str, indent + useIndent);
                str += ",\n";
            }

            fields[fields.size() - 1].write(str, indent + useIndent);
            
        }
        
        str += String::format(" \n{}}", ind);
        
    }

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
    
    void Json::nextObject(String const& data, const size_t from, size_t& begin, size_t& end) {

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
                    begin = bIdx; end = i + 1; return;
                }
                
            }
            
        }
        
    }
    
    void Json::nextList(String const& data, const size_t from, size_t& begin, size_t& end) {

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
                    begin = bIdx; end = i + 1; return;
                }
                
            }
            
        }
        
    }

    void Json::nextString(String const& data, const size_t start, size_t& begin, size_t& end) {

        bool found = false;
        size_t bIdx = 0;
        
        for (size_t i = start; i < data.size(); ++i) {

            if(data[i] == '"') {

                if(found) { begin = bIdx + 1; end = i; return; }
                found = true;
                bIdx = i;
                
            }

            if(data[i] == '\\' && data[i+1] == '"') {
                found = false;
            }
            
        }
        
    }

    void Json::nextBool(String const& data, const size_t from, size_t& begin, size_t& end) {
        
        for (size_t i = from; i < data.size(); ++i) {

            if(startsWith(data, "true", i)) {
                begin = i; end = i + 4; return;
            }

            if(startsWith(data, "false", i)) {
                begin = i; end = i + 5; return;
            }

        }
        
    }

    void Json::nextNull(String const& data, const size_t from, size_t& begin, size_t& end) {
        
        for (size_t i = from; i < data.size(); ++i) {

            if(startsWith(data, "null", i)) {
                begin = i; end = i + 4; return;
            }

        }
        
    }

    void Json::nextNumber(String const& data, const size_t from, size_t& begin, size_t& end) {

        bool start = false;
        bool dot = false;
        size_t bIdx = 0;
        
        for (size_t i = from; i < data.size(); ++i) {

            if(String::isInteger(data[i])) {
                
                if(!start) { start = true; bIdx = i; }
                
            }
            else if(start) {
                
                if(!dot && data[i] == '.') { dot = true; continue; }
                begin = bIdx; end = i; return;
                
            }

        }
        
    }
    
    Json::DataType Json::nextType(String const& data, const size_t from) {

        for (size_t i = from; i < data.size(); ++i) {
            if(data[i] == '"') { return DataType::String; }
            if(data[i] == '{') { return DataType::Object; }
            if(data[i] == '[') { return DataType::List; }
            if(startsWith(data, "true", i)) { return DataType::Boolean; }
            if(startsWith(data, "false", i)) { return DataType::Boolean; }
            if(startsWith(data, "null", i)) { return DataType::Null; }
            if(String::isInteger(data[i])) { return DataType::Number; }
            
        }

        throw Exceptions::Exception("Invalid format");
        
    }
    
    void Json::parseObject(String const& data, Object& storage) {

        static Map<DataType, Function<void(String const&, size_t, size_t&, size_t&)>> funcMap = {
                
            { DataType::Boolean, &nextBool },
            { DataType::Number, &nextNumber },
            { DataType::Null, &nextNull },
            { DataType::String, &nextString },
            { DataType::List, &nextList },
            { DataType::Object, &nextObject },
                
        };

        const auto proc = data.substring(data.firstIndexOf('{') + 1, data.lastIndexOf('}'));

        size_t current = 0;

        while (current < data.size()) {
            
            size_t from = 0;
            size_t to = 0;
            nextString(proc, current, from, to);

            if(from == 0 && to == 0) return;

            const auto tag = proc.substring(from, to);
            const auto type = nextType(proc, to + 1);

            funcMap[type](proc, to + 1, from, to);

            current = to + 1;
            
            if(type == DataType::Object) {
                
                storage.objects += Object(tag);
                parseObject(proc.substring(from,  to), storage.objects[storage.objects.size() - 1]);
                continue;
                
            }
            
            storage.fields += Field(tag, proc.substring(from,  to), type);

        }
        
    }

    void Json::write(String const& fileName) const {

        String str;
        rootObject.write(str, 0);
        File::write(fileName, str);
        
    }

    Json Json::parse(String const& fileName) {

        constexpr auto invalidString = "The file being parsed is an invalid JSON file.";

        const String data = File::load(fileName);

        if(data.isEmpty()) return {};
                
        if(data.numberOccurrences('{') != data.numberOccurrences('}')) {
            throw Exceptions::Exception(invalidString);
        }

        if(data.numberOccurrences('[') != data.numberOccurrences(']')) {
            throw Exceptions::Exception(invalidString);
        }

        Json json = {};

        parseObject(data, json.rootObject);
        
        return json;
        
    }
    
}
