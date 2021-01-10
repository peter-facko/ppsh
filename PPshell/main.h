#pragma once
#include "pipelines.h"

/** Runs the shell on one 'line' of input, storing the 'return_value'. */
int parse_line(const char* line, int* return_value);

/** Increments the internal shell line counter */
void line_number_increment();

// Flex/Bison

int yyparse(int* return_value);
void yyerror(int* return_value, const char* message);
int yylex();
