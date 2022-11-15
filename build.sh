
build_root=$(dirname "$0")/cmake-build-release

if [ $# -gt 0 ] && [ "${1,,}" == "icc" ]; then
    cmake_options="-DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=icc -DCMAKE_CXX_COMPILER=icpc"
else
    cmake_options="-DCMAKE_BUILD_TYPE=Release"
fi

# Create directory
if [ ! -d "${build_root}" ]; then
    mkdir -p "${build_root}"
fi
cd "${build_root}" || exit

# Build
eval cmake .. "${cmake_options}" && make -j4