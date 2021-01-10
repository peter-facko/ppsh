%{
	#include <stdbool.h>
	#include <readline/readline.h>
	#include <readline/history.h>
	#include "parser.h"
	#include "main.h"

	#define YY_NO_INPUT
	#define YY_NO_UNPUT
%}

%%

;					{ return SEMIC; }
\<					{ return REDIRI; }
\>					{ return REDIROT; }
\>\>				{ return REDIROA; }
\|					{ return PIPE; }

[^ \t;<>|\r\n#]+	{
						string_t s;
						if (string_construct_cstring_copy(&s, yytext) != 0)
							return YYerror;
						string_construct_move(&yylval.IDF, &s);
						return IDF;
					}

#.*					{ /* ignore comment */ }

\n					{ line_number_increment(); return EOL; }

.					{ /* ignore */ }

%%

int parse_line(const char* input, int* return_value)
{
	YY_BUFFER_STATE bp = yy_scan_string(input);

	yy_switch_to_buffer(bp);

	int status = yyparse(return_value);

	yy_delete_buffer(bp);

	return status;
}
