#pragma once
#include "pipelines.h"

int parse_line(const char* input);

void line_number_increment();

int yyparse();
void yyerror(const char* message);
int yylex();

void execute(pipelines_t* pipelines);
