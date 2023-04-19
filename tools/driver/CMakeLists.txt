set(LLVM_LINK_COMPONENTS
  Support
)

add_chilang_tool(chilang
  Driver.cpp
)

target_link_libraries(chilang
  PRIVATE
  chilangBasic
)