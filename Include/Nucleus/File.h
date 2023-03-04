#pragma once

#include <Nucleus/String.h>

namespace Nucleus {

    class File {

    public:

        static String load(String const& path);

        static bool write(String const& path, String const& data);
        
    };

}
