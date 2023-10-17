cmake_minimum_required(VERSION 3.20)

find_package(Boost 1.81 REQUIRED COMPONENTS program_options)

add_executable(
  PPshell
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

target_compile_options(PPshell PUBLIC "-Wall" "-Wextra" "-pedantic" "-Werror")
target_compile_features(PPshell PUBLIC cxx_std_23)
target_include_directories(PPshell PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                          ${Boost_INCLUDE_DIRS})

target_link_libraries(PPshell readline Boost::program_options)
