cmake_minimum_required(VERSION 3.27)

find_package(Boost 1.74 REQUIRED COMPONENTS program_options)

add_executable(
  ppsh
  "main.cpp"
  "builtins.cpp"
  "builtins/cd.cpp"
  "builtins/exit.cpp"
  "command.cpp"
  "commands/command.cpp"
  "commands/interactive.cpp"
  "commands/help.cpp"
  "commands/file.cpp"
  "exceptions.cpp"
  "file_descriptor.cpp"
  "pipes.cpp"
  "process.cpp"
  "redirection.cpp"
  "redirections.cpp"
  "pipeline.cpp")

target_compile_options(ppsh PUBLIC "-Wall" "-Wextra" "-pedantic" "-Werror")
target_compile_features(ppsh PUBLIC cxx_std_23)
target_include_directories(ppsh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                       ${Boost_INCLUDE_DIRS})

target_link_libraries(ppsh readline Boost::program_options)

include(GNUInstallDirs)

install(TARGETS ppsh DESTINATION ${CMAKE_INSTALL_BINDIR})
