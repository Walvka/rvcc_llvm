set(LLVM_LINK_COMPONENTS support)

add_chilang_library(chilangParser
    Parser.cpp

    LINK_LIBS
    chilangBasic
)