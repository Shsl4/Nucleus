#include <Nucleus/File.h>
#include <Nucleus/Exceptions.h>
#include <fstream>

namespace Nucleus {
    
    String File::load(String const& path) {

        // Create our file stream in binary mode.
        std::ifstream fStream(path.begin().get(), std::ios::binary | std::ios::ate);

        // If the file does not exist or is inaccessible.
        if (!fStream.is_open()) { throw Exceptions::Exception("Failed to open file."); }

        // Get the file size
        const size_t size = fStream.tellg();

        // Create a string.
        auto data = String(size + 1, 0);

        // Go to the beginning of the file
        fStream.seekg(0, std::ios::beg);

        // Store the file contents in the array.
        fStream.read(data.begin().get(), size);

        // Return the array.
        return data;

    }

    bool File::write(String const& path, String const& data) {

        // Create our file stream in binary mode.
        std::ofstream fStream(path.begin().get(), std::ios::binary | std::ios::ate);

        // If the file does not exist or is inaccessible, return.
        if (!fStream.is_open()) { return false; }

        // Write to our file
        fStream.write(data.begin().get(), static_cast<std::streamsize>(data.size()));

        return true;

    }
    
}
