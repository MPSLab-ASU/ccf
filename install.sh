#Before running this bash script, please read REAMME.md

ccf_root=$(pwd)
#Change this to the path you want and make sure it is a true path!
install_path="" 
mapping_debug=1
# ==============================================================================
# Installing OpenSSL if necessary 

openssl_version="$(openssl version | grep -Eo '[+-]?[0-9]+([.][0-9]+)?' | head -1)"
openssl_exist=$(which openssl)
if [ $(echo "$openssl_version > 1.1" | bc) -eq 0 -o -z "$openssl_exist" ]; then
    echo Installing OpenSSL 1.1.1j in case CMake 3.19.5 needs it
    
    wget https://www.openssl.org/source/openssl-1.1.1j.tar.gz --no-check-certificate
    tar zxvf openssl-1.1.1j.tar.gz
    cd openssl-1.1.1j
    
    ./config --prefix=${install_path}
    make -j 8 && make -j 8 install
    
    cd ..
    
    rm -rf openssl-1.1.1j*
fi

# ==============================================================================
# Installing CMake'
cmake_version="$(cmake -version | grep -Eo '[+-]?[0-9]+([.][0-9]+)?' | head -1)"
cmake_exist=$(which cmake)
if [ $(echo "$cmake_version > 3.19" | bc) -eq 0 -o -z "$cmake_exist" ]; then
    echo Installing CMake 3.19.5
    
    wget https://github.com/Kitware/CMake/releases/download/v3.19.5/cmake-3.19.5.tar.gz --no-check-certificate
    tar zxvf cmake-3.19.5.tar.gz    
    cd cmake-3.19.5
    
    ./configure --prefix=${install_path}
    make -j 8 && make -j 8 install
    
    cd ..
    
    rm -rf cmake-3.19.5*
fi
# ==============================================================================
# Installing LLVM
echo Installing Clang/LLVM + LLVM-Gold

cd llvm-project
llvm_dir=$(pwd)
mkdir build
cd build
cmake -DLLVM_ENABLE_PROJECTS="clang" -DLLVM_BINUTILS_INCDIR="${llvm_dir}/binutils/include" --enable-pic -DCMAKE_BUILD_TYPE=Release ../llvm -DCMAKE_INSTALL_PREFIX=${install_path}
make -j 8 && make -j 8 install

cd ../..

# ==============================================================================
# Compiling Mapping Algorithms 
# Add the following lines below when you know what is going on with the directories
#if [ $(echo "$mapping_debug" | bc) -eq 1 ]; then
#fi

cd mappings
echo $(pwd)
find . -maxdepth 1 -mindepth 1 -type d | while read dir; do
    cd $dir/Release
    make
    cd ../DFGFiles
    g++ -O3 nodefile.cpp -o nodefile
    g++ -O3 edgefile.cpp -o edgefile
    cd ../../
done
echo "Mapping compilation complete"
cd ..

# ==============================================================================
# Building Instruction Generation files
cd InstructionGenerator
make clean && make insgen && make falcon_insgen
echo "Instruction Generator compilation complete"
cd ..
# ==============================================================================
# Changing toolchain path of cgracc and cgra++ and copying them 
# into ${install_path}/bin
cd scripts/CGRALib
sed -i "/ccf_root\=/c\ccf_root=\"${ccf_root}\"" cgracc 
sed -i "/ccf_root\=/c\ccf_root=\"${ccf_root}\"" cgra++ 
sed -i "/ccf_root\=/c\ccf_root=\"${ccf_root}\"" cgraexe
sed -i "/ccf_root\=/c\ccf_root=\"${ccf_root}\"" cgradb
sed -i "/install_path\=/c\install_path=\"${install_path}\"" cgracc 
sed -i "/install_path\=/c\install_path=\"${install_path}\"" cgra++ 
sed -i "/install_path\=/c\install_path=\"${install_path}\"" cgraexe
sed -i "/install_path\=/c\install_path=\"${install_path}\"" cgradb
cp cgracc cgra++ cgraexe cgradb ${install_path}/bin
echo "cgracc, cgra++, and cgraexe manipulation and copy complete"
cd ../..
# ==============================================================================
# Build gem5
python3 -m pip install --user scons
cd gem5
scons build/ARM/gem5.opt -j8
sed -i "s|AtomicSimpleCPU|AtomicCGRA,AtomicSimpleCPU|g" ./build/variables/ARM
scons build/ARM/gem5.opt -j8
scons build/ARM/gem5.debug -j8
cd ../
