set(LLVM_LINK_COMPONENTS support)

add_chilang_library(chilangIRGen
    IRGenerator.cpp

    LINK_LIBS
    chilangBasic
)