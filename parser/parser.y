%{
	#include <iostream>
	#include "Automata.h"
	#include "Automata.h"
	#include "AutomataBuilder.h"
	#include "AutomataPrinter.h"
	#include "Tools.h"
	
	using namespace std;
	
	dupl_t dupl;
	

	void scan_string(char const *arg);
	
	extern "C" {
		int yylex(void);
		int yywrap() { return 1; } 
		void yyerror (char const *s)
	 	{
			fprintf (stderr, "%s\n", s);
	 	}
	}
	int* bracket_list_char = make128zeros();
	
	void dealwith_range()
	{
		
	}
	
	char leftRange;
	char rightRange;
	bool isLeftRange = false;
	bool isRightRange = false;
%}

/* The following tokens are for the Bracket Expression
   grammar common to both REs and EREs. */

%union { char char_value ; int int_value; char string_value[256] ; Automata* automata_value;}

%token    Open_equal Equal_close Open_dot Dot_close Open_colon Colon_close
/*           '[='       '=]'        '[.'     '.]'      '[:'       ':]'  */

%token<string_value>  class_name COLL_ELEM_MULTI
/* class_name is a keyword to the LC_CTYPE locale category */
/* (representing a character class) in the current locale */
/* and is only recognized between [: and :] */

%token<char_value>  ORD_CHAR  QUOTED_CHAR COLL_ELEM_SINGLE META_CHAR
%token<int_value> DUP_COUNT
%type<automata_value> extended_reg_exp ERE_branch ERE_expression one_char_or_coll_elem_ERE bracket_expression
%start start_Exp

%%

	/* --------------------------------------------
   Bracket Expression
   -------------------------------------------
*/
bracket_expression : '[' matching_list ']'	{$$ = makeAutomata_withBracketList(bracket_list_char);  }
               | '[' nonmatching_list ']' 	{$$ = makeAutomata_withBracketList(bracket_list_char); }
               ;
matching_list  : bracket_list	{}
               ;
nonmatching_list : '^' bracket_list	{invertBracketList(bracket_list_char);}
               ;
bracket_list   : follow_list	{}
               | follow_list '-'	{bracket_list_char[(int)'-'] = 1;}
               ;
follow_list    :             expression_term	{}
               | follow_list expression_term	{}
               ;
expression_term : single_expression	{}
               | range_expression	{
               							if(isLeftRange && isRightRange)
												{
													dealwith_range(leftRange, rightRange, bracket_list_char);
												}
												}
               ;
single_expression : end_range	{}
               | character_class	{}
               | equivalence_class	{}
               ;
range_expression : start_range end_range	{}
               | start_range '-'	{
               							
											}
               ;
start_range    : end_range '-'	{
												
											}
               ;
end_range      : COLL_ELEM_SINGLE	{
               							bracket_list_char[(int)$1] = 1;



												if(isLeftRange && isRightRange)

												{

													rightRange = leftRange;

													leftRange = $1;

												}

												if (isLeftRange && !isRightRange)

												{

													rightRange = $1;

													isRightRange = true; 

												}

												else if(!isLeftRange && isRightRange)

												{

													leftRange = $1;

													isLeftRange = true;

												}

												else if(!isLeftRange && !isRightRange)

												{

													leftRange = $1;

													isLeftRange = true;

												}

												}
               | collating_symbol	{}
               ;
collating_symbol : Open_dot COLL_ELEM_SINGLE Dot_close	{dealwith_dot_bracket_single(bracket_list_char, $2);}
               | Open_dot COLL_ELEM_MULTI Dot_close	{dealwith_dot_bracket_multi(bracket_list_char, $2);}
               | Open_dot META_CHAR Dot_close	{dealwith_dot_bracket_meta(bracket_list_char, $2);}
               ;
equivalence_class : Open_equal COLL_ELEM_SINGLE Equal_close	{dealwith_equal_bracket_single(bracket_list_char, $2);}
               | Open_equal COLL_ELEM_MULTI Equal_close	{dealwith_equal_bracket_multi(bracket_list_char, $2);}
               ;
character_class : Open_colon class_name Colon_close {dealwith_colon_bracket(bracket_list_char, $2);}
               ;

/* --------------------------------------------
   Extended Regular Expression
   --------------------------------------------
*/
start_Exp			 :	extended_reg_exp	{startAnalysis($1); resetBracketList(bracket_list_char);}
						 ;

extended_reg_exp   :                      ERE_branch {$$ = $1;}
                   | extended_reg_exp '|' ERE_branch {$$ = AutomataBuilder::unite($1, $3); 
                   												}
                   ;
ERE_branch         :            ERE_expression 		 {$$ = $1;}
                   | ERE_branch ERE_expression		{$$ = AutomataBuilder::concat($1, $2);
                   												}
                   ;
ERE_expression     : one_char_or_coll_elem_ERE		{$$ = $1;}
                   | '^'									{$$ = AutomataBuilder::allWords();}
                   | '$'									{$$ = AutomataBuilder::allWords();}
                   | '(' extended_reg_exp ')'		{$$ = $2 ; }
                   | ERE_expression ERE_dupl_symbol	{$$ = makeDuplAutomata($1, dupl);
                   												}
                   ;
one_char_or_coll_elem_ERE  : ORD_CHAR 				 	  {$$ = AutomataBuilder::oneLetter($1); }

                   | QUOTED_CHAR							  {$$ = AutomataBuilder::oneLetter($1);}
                   
                   | '.'		 				  			  {$$ = AutomataBuilder::allLetters();}
                   | bracket_expression 				  {$$ = $1;}
                   ;
ERE_dupl_symbol    : '*'									  {updateDupl(&dupl, DUPL_STAR, 0, 0); } 
                   | '+'									   {updateDupl(&dupl, DUPL_PLUS, 0, 0); } 
                   | '?'									  {updateDupl(&dupl, DUPL_CHOICE, 0, 0);}
                   | '{' DUP_COUNT               '}' {updateDupl(&dupl, DUPL_MAX_M, 0, $2);}
                   | '{' DUP_COUNT ','           '}' {updateDupl(&dupl, DUPL_MIN_N, $2, 0);}
                   | '{' DUP_COUNT ',' DUP_COUNT '}' {updateDupl(&dupl, DUPL_BET_N_M, $2, $4);}
                   ;

%%
