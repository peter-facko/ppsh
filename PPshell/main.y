%code requires
{
	#include <stdio.h>
	#include "main.h"
}

%code
{
	int yylex();
	void yyerror();
}

%define api.value.type union

%token SEMIC
%token<string_t> IDF
%token REDIRI
%token REDIROT
%token REDIROA
%token PIPE
%token EOL

%type<pipelines_t> pipelines_pure 
%type<pipeline_t> pipeline
%type<command_t> command command_arguments_redirections
%type<redirections_t> redirections
%type<redirection_t> redirection
%type<redirection_type_t> redirection_type
%type<string_t> argument command_path

%start script

%%

script:
	pipelines
|	script EOL pipelines;

pipelines:
	%empty
|	pipelines_pure semic_opt { pipelines_execute_and_destroy(&$1); };

pipelines_pure:
	pipeline { pipelines_init_move(&$$, &$1); }
|	pipelines_pure SEMIC pipeline { pipelines_append_move(&$1, &$3); pipelines_construct_move(&$$, &$1); };

semic_opt:
	%empty
|	SEMIC;

pipeline:
	command { pipeline_init_move(&$$, &$1); }
|	pipeline PIPE command { pipeline_append_command_move(&$1, &$3); pipeline_construct_move(&$$, &$1); };

command:
	redirections command_arguments_redirections { command_combine_redirections_move(&$2, &$1); command_construct_move(&$$, &$2); };

redirections:
	%empty { redirections_construct(&$$); }
|	redirections redirection { redirections_register_move(&$1, &$2); redirections_construct_move(&$$, &$1); };

command_arguments_redirections:
	command_path { command_init_move(&$$, &$1); }
|	command_arguments_redirections argument { command_register_argument_move(&$1, &$2); command_construct_move(&$$, &$1); }
|	command_arguments_redirections redirection { command_register_redirection_move(&$1, &$2); command_construct_move(&$$, &$1); };

argument:
	IDF { string_construct_move(&$$, &$1); };

redirection:
	redirection_type IDF { redirection_init_move(&$$, &$2, $1); };

redirection_type:
	REDIRI  { $$ = RT_INPUT; }
|	REDIROT { $$ = RT_OUTPUT_TRUNCATE; }
|	REDIROA { $$ = RT_OUTPUT_APPEND; };

command_path:
	IDF { string_construct_move(&$$, &$1); };

%%

void yyerror(char* s)
{
	fprintf(stderr, "line %d: %s\n", line_number, s);
	return_value = 2;
}
