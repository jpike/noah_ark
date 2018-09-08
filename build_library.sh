# CREATE THE BUILD DIRECTORY IF IT DOESN'T EXIST.
mkdir -p build

# BUILD THE LIBRARY.
COMPILATION_FILE="noah_ark_library.project"
MAIN_CODE_DIR="code/library"
$CXX -x c++ $COMPILATION_FILE /I $MAIN_CODE_DIR -o "build/noah_ark_library.lib"
