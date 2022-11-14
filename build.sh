
build_root="cmake-build-release"

if [ $# -gt 0 ] && [ "${1,,}" == "icc" ]; then
    cmake_options="-DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=icc -DCMAKE_CXX_COMPILER=icpc"
else
    cmake_options="-DCMAKE_BUILD_TYPE=Release"
fi

# Create directory
mkdir ${build_root}
cd ${build_root} || (echo "Error: Folder ${build_root} does not exist"; exit)

# Build
#cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=icc -DCMAKE_CXX_COMPILER=icpc && make -j4
eval cmake .. "${cmake_options}" && make -j4