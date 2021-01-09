#pragma once
#include "pipelines.h"

/** Runs the shell on one 'line' of input. */
int parse_line(const char* line);

/** Increments the internal shell line counter */
void line_number_increment();

/** Forwarder for `pipelines_execute_and_destroy`. */
void execute(pipelines_t* pipelines);

// Flex/Bison

int yyparse();
void yyerror(const char* message);
int yylex();
