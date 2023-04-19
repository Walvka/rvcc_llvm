set(LLVM_LINK_COMPONENTS support)

add_chilang_library(chilangLexer
    Lexer.cpp

    LINK_LIBS
    chilangBasic
)