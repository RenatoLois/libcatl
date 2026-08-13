cmake_minimum_required( VERSION 4.2 )
project(libcstl C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories (
  include/
)

link_libraries (
)

file(GLOB SOURCE_FILES
  "src/linked-list/*.c"
  "src/array-list/*.c"
  "src/stack/*.c"
  "src/pair/*.c"
)

add_executable( testing
  ${SOURCE_FILES}
)
