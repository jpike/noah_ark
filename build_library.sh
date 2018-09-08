# CREATE THE BUILD DIRECTORY IF IT DOESN'T EXIST.
mkdir -p build

# PRINT OUT SOME DEBUG STUFF.
echo $CXX
type clang++
which clang++
type clang++-7
which clang++-7
type g++
which g++
type g++-8
which g++-8

# BUILD THE LIBRARY.
COMPILATION_FILE="noah_ark_library.project"
MAIN_CODE_DIR="code/library"
$CXX -x c++ $COMPILATION_FILE -I $MAIN_CODE_DIR -std=c++17 -fpermissive -o "build/noah_ark_library.lib"
