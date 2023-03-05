#include <Nucleus/Json.h>
#include <Nucleus/File.h>
#include <Nucleus/Exceptions.h>
#include <Nucleus/Console.h>
#include <Nucleus/Function.h>
#include <Nucleus/Map.h>

namespace Nucleus {
    
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

    bool Json::asFloatArray(Container* container, String const& tag, MutableArray<String> const& data) {
        
        try {

            auto floats = MutableArray<Float64>(data.size());

            for (auto const& str : data) {

                floats += str.toFloat64();
                
            }

            container->add(tag, floats);
            
            return true;
            
        }
        catch (std::exception const&) {
            return false;
        }
        
    }

    bool Json::asBoolArray(Container* container, String const& tag, MutableArray<String> const& data) {
        
        try {

            auto booleans = MutableArray<bool>(data.size());

            for (auto const& str : data) {

                booleans += str.toBool();
                
            }

            container->add(tag, booleans);
            
            return true;
            
        }
        catch (std::exception const&) {
            return false;
        }
        
    }

    bool Json::asObjectArray(Container* container, String const& tag, MutableArray<String> const& data) {
        
        try {
            
            for (auto const& str : data) {

                if(str[0] != '{' || str[str.size() - 1] != '}') { return false; }
                
            }

            DataArray* array = container->createArray(tag);
            
            for (auto const& str : data) {
                
                parseObject(str, array->createContainer());
                
            }
            
            return true;
            
        }
        catch (std::exception const&) {
            return false;
        }
        
    }
    
    void Json::parseObject(String const& data, Container* storage) {

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

            switch (type) {
                case DataType::Object:
                    
                    parseObject(proc.substring(from,  to), storage->createContainer(tag));
                    break;
                
                case DataType::String:
                    
                    storage->add(tag, proc.substring(from,  to));
                    break;
                
                case DataType::Boolean:
                    
                    storage->add(tag, proc.substring(from,  to).toBool());
                    break;
                
                case DataType::Number:
                    
                    storage->add(tag, proc.substring(from,  to).toFloat64());
                    break;
                
                case DataType::Null:
                    
                    storage->add(tag, nullptr);
                    break;
                
                case DataType::List:
                    
                    const auto e = proc.substring(from,  to);
                    auto array = e.substring(1, e.size() - 1).removeOccurrences("\n").split(",");

                    for(auto& element : array) {
                        removeExtraSpaces(element);
                    }

                    if(asBoolArray(storage, tag, array)) break;
                
                    if(asFloatArray(storage, tag, array)) break;
                
                    if(asObjectArray(storage, tag, array)) break;
                
                    storage->add(tag, array);
                
                    break;
                
            }

        }
        
    }
    
    void Json::writeElement(Element* element, String& str, const size_t indent) {

        auto ind = String(4 * indent, ' ');

        if(dynamic_cast<Field<String>*>(element)) {
            str += String::format(R"({}"{}": "{}")", ind, element->name(), element->stringValue());
        }
        else {
            str += String::format(R"({}"{}": {})", ind, element->name(), element->stringValue());
        }
        
    }

    template<typename T>
    void forEachBeforeLast(SmartArray<T> const& e, Function<void(T*)> const& f, Function<void()> const& all) {

        for (size_t i = 0; i < e.size(); ++i) {
            f(e[i]);
            if(i != e.size() - 1) { all(); }
        }
        
    }
    
    void Json::writeArray(const DataArray* array, String& str, const size_t indent) {

        auto ind = String(4 * indent, ' ');

        str += String::format("{}\"{}\": [ \n", ind, array->name());

        const Function<void(Element*)> func = [&](Element* p) {

            if(const auto* ct = dynamic_cast<Container*>(p)) {
                writeContainer(ct, str, indent + 1, true);
            }
            else if(const auto* arr = dynamic_cast<DataArray*>(p)) {
                writeArray(arr, str, indent + 1);
            }
            else {
                writeElement(p, str, indent + 1);
            }
            
        };
        
        forEachBeforeLast(array->elements(), func, [&] {
            str += ",\n";            
        });

        str += String::format(" \n{}]", ind);
        
    }
    
    void Json::writeContainer(const Container* container, String& str, const size_t indent, const bool useIndent) {

        auto ind = String(4 * indent * useIndent, ' ');
        
        if(container->name().isEmpty()) {
            str += String::format("{}{ \n", ind);
        }
        else {
            str += String::format("{}\"{}\": { \n", ind, container->name());
        }

        const Function<void(Element*)> func = [&](Element* p) {

            if(const auto* ct = dynamic_cast<Container*>(p)) {
                writeContainer(ct, str, indent + 1, true);
            }
            else if(const auto* arr = dynamic_cast<DataArray*>(p)) {
                writeArray(arr, str, indent + 1);
            }
            else {
                writeElement(p, str, indent + 1);
            }
            
        };
        
        forEachBeforeLast(container->elements(), func, [&] {
            str += ",\n";            
        });
        
        str += String::format(" \n{}}", ind);
        
    }

    void Json::write(String const& fileName) const {

        String str;
        writeContainer(storage.rootContainer(), str, 0, true);
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

        parseObject(data, json.archive().rootContainer());
        
        return json;
        
    }
    
}
