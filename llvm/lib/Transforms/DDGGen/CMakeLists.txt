add_llvm_loadable_module(LLVMDDGGen
  DDGGen.cpp
  DFG.cpp
  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
  )

add_llvm_loadable_module(LLVMcomm
  DDGGen.cpp
  DFG.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
  )
