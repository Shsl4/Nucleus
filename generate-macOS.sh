if [ ! command -v cmake &> /dev/null ]
then
    echo "Could not find cmake. Make sure it is installed."
    exit
fi

if [ ! -d ".cmake" ]
then mkdir .cmake
fi

cd .cmake
cmake .. -G Xcode
cd ..
ln -sf .cmake/Nucleus.xcodeproj Nucleus.xcodeproj
