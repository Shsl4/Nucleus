#pragma once

#include <Nucleus/String.h>
#include <fstream>

namespace Nucleus {

    class File {

    public:

        void load(String const& file){

            // Create our file stream in binary mode.
            std::ifstream fStream(file.begin().get(), std::ios::binary | std::ios::ate);

            // If the file does not exist or is inaccessible.
            if (!fStream.is_open()) { throw Exceptions::Exception("Failed to open file."); }

            // Get the file size
            const size_t size = fStream.tellg();

            // Create a string.
            this->storage = String(size + 1, 0);

            // Go to the beginning of the file
            fStream.seekg(0, std::ios::beg);

            // Store the file contents in the array.
            fStream.read(storage.begin().get(), size);

            this->path = file;

        }

        template<typename T> requires std::is_fundamental_v<T>
        MutableArray<T> readMultiple(size_t count) {

            if(cursor + sizeof(T) * count > storage.size()) throwException("Error");

            MutableArray<T> arr = MutableArray<T>::filled(count);

            const size_t pos = cursor;

            cursor += sizeof(T) * count;

            memcpy(arr.data(), storage.begin().get() + pos, sizeof(T) * count);

            return arr;

        }

        template<typename T> requires std::is_fundamental_v<T>
        T read() {

            if(cursor + sizeof(T) > storage.size()) throwException("Error");

            const size_t pos = cursor;

            cursor += sizeof(T);

            return *reinterpret_cast<T*>(&storage[pos]);

        }

        bool openForWriting(String const& filePath){

            // Create our file stream in binary mode.
            std::ofstream fStream(filePath.begin().get(), std::ios::binary | std::ios::ate);

            // If the file does not exist or is inaccessible, return.
            if (!fStream.is_open()) { return false; }

            this->writeStream = std::move(fStream);

            return true;

        }

        template<typename T> requires std::is_fundamental_v<T>
        void write(T const& obj){

            nthrowif(!this->writeStream.is_open(), "Must open file before writing!");

            this->writeStream.write(reinterpret_cast<const char*>(&obj), sizeof(T));

        }

        template<typename T> requires std::is_fundamental_v<T>
        void write(T* obj, size_t size){

            nthrowif(!this->writeStream.is_open(), "Must open file before writing!");

            this->writeStream.write(reinterpret_cast<const char*>(obj), size * sizeof(T));

        }

        void rewind() { this->cursor = 0; }

        NODISCARD String& data() { return this->storage; }

        NODISCARD bool valid() const { return path != nullptr; }

    private:

        String path = nullptr;
        String storage = nullptr;
        size_t cursor = 0;
        std::ofstream writeStream;

    };

}
