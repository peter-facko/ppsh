cmake_minimum_required(VERSION 3.16)

add_compile_options(-Wall -Wextra -pedantic -Werror)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

bison_target(parser main.y ${CMAKE_CURRENT_SOURCE_DIR}/parser.c DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/parser.h)

flex_target(lexer main.lex ${CMAKE_CURRENT_SOURCE_DIR}/lexer.c)
add_flex_bison_dependency(lexer parser)

add_executable(PPshell ${BISON_parser_OUTPUTS} ${FLEX_lexer_OUTPUTS}
	"main.c"
	"string.c"
	"strings.c"
	"redirections.c"
	"redirection.c"
	"command.c"
	"pipeline.c"
	"pipelines.c"
	"internal_commands.c"
	"internal_exit.c"
	"internal_cd.c"
 	"pipes.h"
	"pipes.c"
	"file_descriptor.c")

target_link_libraries(PPshell readline)
target_link_libraries(PPshell ${FLEX_LIBRARIES})
