%{
 	extern "C" {int yylex(void);}
 	#include <string.h>
	#include "Automata.h"
	#include "../include/parser.h"
	
	void scan_string(char const *arg)
	{
		yy_switch_to_buffer(yy_scan_string(arg));
	}

%}

METACHAR		[\^\-\]]
COLL_ELEM_SINGLE	[^METACHAR]
COLL_ELEM_MULTI		[COLL_ELEM_SINGLE]+
SPEC_CHAR		[\^\.\[\$\(\)\|\*\+\?\{\\]
ORD_CHAR		[^SPEC_CHAR]
QUOTED_CHAR		(\\\^)|(\\\.)|(\\\[)|(\\\$)|(\\\()|(\\\))|(\\\|)|(\\\*)|(\\\+)|(\\\?)|(\\\{)|(\\\\)
CLASS_NAME	(alnum)|(alpha)|(blank)|(cntrl)|(digit)|(graph)|(lower)|(print)|(punct)|(space)|(upper)|(xdigit)

%x	BE
%x	FIRST_IN_BE
%x DUP
%x COLL_DOT
%x COLL_EQUAL
%x COLL_COLON
%%

"{"	{ BEGIN(DUP); return yytext[0];}
<DUP>[0-9]+  { yylval.int_value = atoi(yytext); return DUP_COUNT;}
<DUP>","	{return yytext[0];}
<DUP>"}"	{BEGIN(INITIAL); return yytext[0];} 

<FIRST_IN_BE>"^"	{ BEGIN(BE); return yytext[0];}
<FIRST_IN_BE>"-"	{ BEGIN(BE); yylval.char_value = yytext[0]; return COLL_ELEM_SINGLE;}
<FIRST_IN_BE>"]"	{ BEGIN(BE); yylval.char_value = yytext[0]; return COLL_ELEM_SINGLE;}
<FIRST_IN_BE>"[="			{ BEGIN(COLL_EQUAL);	return Open_equal; }
<FIRST_IN_BE>"[."			{ BEGIN(COLL_DOT);	return Open_dot; }
<FIRST_IN_BE>"[:"			{ BEGIN(COLL_COLON);	return Open_colon; }
<FIRST_IN_BE>{COLL_ELEM_SINGLE}	{ BEGIN(BE); yylval.char_value = yytext[0]; return COLL_ELEM_SINGLE;}
{QUOTED_CHAR}		{ yylval.char_value = yytext[1] ; return QUOTED_CHAR; }
"["			{ BEGIN(FIRST_IN_BE); return yytext[0];}
<BE>"[="			{ BEGIN(COLL_EQUAL);	return Open_equal; }
<COLL_EQUAL>"=]"			{ BEGIN(BE);	return Equal_close; }
<BE>"[."			{ BEGIN(COLL_DOT);	return Open_dot; }
<COLL_DOT>".]"			{ BEGIN(BE);	return Dot_close; }
<BE>"[:"			{ BEGIN(COLL_COLON);	return Open_colon; }
<COLL_COLON>":]"			{ BEGIN(BE);	return Colon_close; }
<BE>"]"					{ BEGIN(INITIAL); return yytext[0];}
<BE>"-"					{ return yytext[0];}
<BE>{METACHAR}            { yylval.char_value = yytext[0]; return COLL_ELEM_SINGLE;}
<BE>{COLL_ELEM_SINGLE}        { yylval.char_value = yytext[0]; return COLL_ELEM_SINGLE; }
{SPEC_CHAR}	{ return yytext[0];}
\n	{}
{ORD_CHAR}		{   yylval.char_value = yytext[0]; return ORD_CHAR; }
<COLL_DOT>{METACHAR}	{ yylval.char_value = yytext[0]; return META_CHAR; }
<COLL_DOT>{COLL_ELEM_MULTI}	{  strcpy(yylval.string_value , yytext) ; return COLL_ELEM_MULTI; }
<COLL_DOT>{COLL_ELEM_SINGLE}	{ yylval.char_value = yytext[0];	return COLL_ELEM_SINGLE; }
<COLL_EQUAL>{COLL_ELEM_MULTI}	{ strcpy(yylval.string_value , yytext) ; return COLL_ELEM_MULTI; }
<COLL_EQUAL>{COLL_ELEM_SINGLE}	{ 	yylval.char_value = yytext[0]; return COLL_ELEM_SINGLE; }
<COLL_COLON>{CLASS_NAME}	{ strcpy(yylval.string_value , yytext) ; return class_name;}
%%
