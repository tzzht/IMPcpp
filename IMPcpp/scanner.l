/* 这里的代码直接拷贝到生成的.cpp文件中 */
%{
	#include <iostream>
	#include <cstdlib>
	#include "scanner.h"
	#include "interpreter.h"
	#include "parser.hpp"
	#include "location.hh"

	using namespace std;

	/* flex默认的yyterminate()就是return 0, int型. 但是如果用Bison3的话, 就需要返回值是Parser::semantic_type类型, 所以这里要重新定义yyerminate() */
	#define yyterminate() tzzht::Parser::make_END(tzzht::location());

	// This will track current scanner location.
	// Action is called when length of the token is known.
	#define YY_USER_ACTION m_driver_.increaseLocation(yyleng);
	
%}

/* 使语法分析器不要添加默认规则 */
%option nodefault
/* 不使用yywrap()(在到达yyin(是文件句柄)的结束位置时, 会自动调用yywrap(). 目的是为了调整yyin的值并继续进行词法分析.) */
%option noyywrap

/* 生成c++ scanner类 */
%option c++
/* flex默认会生成的两个类, FlexLexer 和 yyFlexLexer, 后者是前者的子类(在flex自带的FlexLexer.h中定义的). 在我们写的Scanner.h中会定义一个yyFlexLexer的子类Scanner. 这条指令会让flex产生的cpp文件中定义Scanner::yylex()方法(通过#define YY_DECL int Scanner::yylex()实现)(yylex()是词法分析的主要函数)(也会定义yyFlexLexer方法(虚函数), 这个方法会报错). 但是yylex()的返回值默认是int, bison3 使用symbol_type, 需要返回值是Parser::symbol_type, 所以在scanner.h中我们#undef YY_DECL, 然后#define YY_DECL Parser::symbol_type Scanner::get_next_token(), 所以我们词法分析的主要函数就从yylex()变成了get_next_token().*/
%option yyclass="Scanner"

/* 将类yyFlexLexer更名为tzzht_FlexLexer */
%option prefix="tzzht_"

IDENTIFER		[a-z]
NUMBER     		[0-2]
PLUS   			"+"
MINUS   		"-"
TIMES   		"*"
TRUE			"true"
FALSE			"false"
EQUAL			"=="
LAE				"<="
NOT 			"!"
CONJUNCTION		"and"
DISCONJUNCTION	"or"
COBEGIN			"cobegin"
CONCUR			"||"
COEND			"coend"
SKIP			"skip"
ASSIGNMENT		":="
SEMICOLON		";"
WAIT			"wait"
LPAREN			"("
RPAREN			")"
IF 				"if"
THEN			"then"
ELSE			"else"
ENDIF			"endif"
WHILE			"while"
DO				"do"
ENDDO			"enddo"
WS			    [ \r\n\t]*

%%
{IDENTIFER}			{ //cout << "Scanner: IDENTIFER '" << yytext << "'." << endl; 
						return tzzht::Parser::make_IDENTIFER(yytext[0], tzzht::location()); }
{NUMBER}			{ //cout << "Scanner: 'NUMBER' " << atoi(yytext) << endl; 
						return tzzht::Parser::make_NUMBER(atoi(yytext), tzzht::location()); }
{PLUS}				{ //cout << "Scanner: 'PLUS'" << endl; 
						return tzzht::Parser::make_PLUS(tzzht::location()); }
{MINUS}				{ //cout << "Scanner: 'MINUS'" << endl; 
						return tzzht::Parser::make_MINUS(tzzht::location()); }
{TIMES}				{ //cout << "Scanner: 'TIMES'" << endl; 
						return tzzht::Parser::make_TIMES(tzzht::location()); }
{TRUE}				{ //cout << "Scanner: 'TRUE'" << endl; 
						return tzzht::Parser::make_TRUE(tzzht::location()); }
{FALSE}				{ //cout << "Scanner: 'FALSE'" << endl;
						return tzzht::Parser::make_FALSE(tzzht::location()); }
{EQUAL}				{ //cout << "Scanner: 'EQUAL'" << endl; 
						return tzzht::Parser::make_EQUAL(tzzht::location()); }
{LAE}				{ //cout << "Scanner: 'LAE'" << endl;
						return tzzht::Parser::make_LAE(tzzht::location()); }
{NOT}				{ //cout << "Scanner: 'NOT'" << endl; 
						return tzzht::Parser::make_NOT(tzzht::location()); }
{CONJUNCTION}		{ //cout << "Scanner: 'CONJUNCTION'" << endl; 
						return tzzht::Parser::make_CONJUNCTION(tzzht::location()); }
{DISCONJUNCTION}	{ //cout << "Scanner: 'DISCONJUNCTION'" << endl; 
						return tzzht::Parser::make_DISCONJUNCTION(tzzht::location()); }
{COBEGIN}			{ //cout << "Scanner: 'COBEGIN'" << endl; 
						return tzzht::Parser::make_COBEGIN(tzzht::location()); }
{CONCUR}			{ //cout << "Scanner: 'CONCUR'" << endl; 
						return tzzht::Parser::make_CONCUR(tzzht::location()); }
{COEND}				{ //cout << "Scanner: 'COEND'" << endl;
						return tzzht::Parser::make_COEND(tzzht::location()); }
{SKIP}				{ //cout << "Scanner: 'SKIP'" << endl;
						return tzzht::Parser::make_SKIP(tzzht::location()); }
{ASSIGNMENT}		{ //cout << "Scanner: 'ASSIGNMENT'" << endl;
						return tzzht::Parser::make_ASSIGNMENT(tzzht::location()); }
{SEMICOLON}			{ //cout << "Scanner: 'SEMICOLON'" << endl;
						return tzzht::Parser::make_SEMICOLON(tzzht::location()); }
{WAIT}				{ //cout << "Scanner: 'WAIT'" << endl;
						return tzzht::Parser::make_WAIT(tzzht::location()); }
{LPAREN}			{ //cout << "Scanner: 'LPAREN'" << endl; 
						return tzzht::Parser::make_LPAREN(tzzht::location()); }
{RPAREN}			{ //cout << "Scanner: 'RPAREN'" << endl; 
						return tzzht::Parser::make_RPAREN(tzzht::location()); }						
{IF}				{ //cout << "Scanner: 'IF'" << endl;
						return tzzht::Parser::make_IF(tzzht::location()); }
{THEN}				{ //cout << "Scanner: 'THEN'" << endl;
						return tzzht::Parser::make_THEN(tzzht::location()); }
{ELSE}				{ //cout << "Scanner: 'ELSE'" << endl;
						return tzzht::Parser::make_ELSE(tzzht::location()); }
{ENDIF}				{ //cout << "Scanner: 'ENDIF'" << endl;
						return tzzht::Parser::make_ENDIF(tzzht::location()); }
{WHILE}				{ //cout << "Scanner: 'WHILE'" << endl;
						return tzzht::Parser::make_WHILE(tzzht::location()); }
{DO}				{ //cout << "Scanner: 'DO'" << endl;
						return tzzht::Parser::make_DO(tzzht::location()); }
{ENDDO}				{ //cout << "Scanner: 'ENDDO'" << endl;
						return tzzht::Parser::make_ENDDO(tzzht::location()); }
{WS}      			{ /* ignored */ }
.           		{ cout << "Scanner: unknown character '" << yytext << "'" << endl; }
<<EOF>>         	{ return yyterminate(); }

%%
