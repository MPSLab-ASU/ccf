add_llvm_component_library(LLVMDDGGen
  DDGGen.cpp
  DFG.cpp
  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
  )

add_llvm_component_library(LLVMcomm
  DDGGen.cpp
  DFG.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
  )
