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
						string_construct_cstring_copy(&s, yytext);
						yylval.IDF = s;
						return IDF;
					}

#.*					{ /* ignore comment */ }

\n					{ line_number_increment(); return EOL; }

.					{ /* ignore */ }

%%

int parse_line(const char* input)
{
	YY_BUFFER_STATE bp = yy_scan_string(input);

	yy_switch_to_buffer(bp);

	int status = yyparse();

	yy_delete_buffer(bp);

	return status;
}
