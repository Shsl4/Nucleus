#pragma once

#include <Nucleus/String.h>
#include <fstream>

namespace Nucleus {

    class File{

    public:

        static String load(String const& path){

            // Create our file stream in binary mode.
            std::ifstream fStream(path.begin().get(), std::ios::binary | std::ios::ate);

            // If the file does not exist or is inaccessible, return.
            if (!fStream.is_open()) { return {}; }

            // Get the file size
            const std::ifstream::pos_type size = fStream.tellg();

            // Create a string.
            auto data = String(size, 0);

            // Go to the beginning of the file
            fStream.seekg(0, std::ios::beg);

            // Store the file contents in the array.
            fStream.read(data.begin().get(), size);

            // Return the array.
            return data;

        }

        static bool write(String const& path, String const& data){

            // Create our file stream in binary mode.
            std::ofstream fStream(path.begin().get(), std::ios::binary | std::ios::ate);

            // If the file does not exist or is inaccessible, return.
            if (!fStream.is_open()) { return false; }

            // Write to our file
            fStream.write(data.begin().get(), static_cast<std::streamsize>(data.size() + 1));

            return true;

        }


    };

}
