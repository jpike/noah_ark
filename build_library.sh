# CREATE THE BUILD DIRECTORY IF IT DOESN'T EXIST.
mkdir -p build

# BUILD THE LIBRARY.
COMPILATION_FILE="noah_ark_library.project"
$CXX -x c++ $COMPILATION_FILE -o "build/noah_ark_library.lib"
