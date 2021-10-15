/* 使用指令％skeleton "lalr1.cc"选择C++解析器的骨架 */
%skeleton "lalr1.cc"
/* 指定bison的版本 */
%require "3.0"
/* 生成各种头文件  location.hh position.hh  parser.hpp */
%defines

/* 声明命名空间与下面声明的类名结合使用 tzzht::Parser:: */
%define api.namespace { tzzht }
%define api.parser.class { Parser }

%define api.token.constructor
/* 使得类型与token定义可以使用各种复杂的结构与类型 */
%define api.value.type variant
/* 开启断言功能 */
%define parse.assert


/* 这里的代码直接拷贝到.hpp文件中, requires中的内容会放在YYLTYPE与YYSTPYPE定义前 */
%code requires
{
    #include <iostream>
    #include <string>
    #include <vector>
    #include <stdint.h>
    #include "imp.h"

    using namespace std;

    namespace tzzht {
        class Scanner;
        class Interpreter;
    }
}

/* 这里的代码直接拷贝到生成的.cpp文件中, 靠近头部的地方 */
%code top
{
    #include <iostream>
    #include <vector>
    #include "scanner.h"
    #include "parser.hpp"
    #include "interpreter.h"
    #include "location.hh"
    
    /* 注意：这里的参数由%parse-param决定, 在parser.y中被定义 */
    // yylex() arguments are defined in parser.y
    static tzzht::Parser::symbol_type yylex(tzzht::Scanner &scanner, tzzht::Interpreter &driver) {
        return scanner.get_next_token();
    }
    
    using namespace tzzht;
}

/*定义parser传给scanner的参数*/
%lex-param { tzzht::Scanner &scanner }
%lex-param { tzzht::Interpreter &driver }

/*定义driver传给parser的参数*/
%parse-param { tzzht::Scanner &scanner }
%parse-param { tzzht::Interpreter &driver }

%locations
%define parse.trace
/*详细显示错误信息*/
%define parse.error verbose

/* 通过tzzht::Parser::make_XXX(loc)给token添加前缀 */
%define api.token.prefix {TOKEN_}

%token END 0 //end of file
%token <char> IDENTIFER
%token <int> NUMBER
%token PLUS
%token MINUS
%token TIMES
%token TRUE
%token FALSE
%token EQUAL
%token LAE
%token NOT 	
%token CONJUNCTION		
%token DISCONJUNCTION
%token COBEGIN
%token CONCUR		
%token COEND			
%token SKIP		
%token ASSIGNMENT		
%token SEMICOLON	
%token WAIT
%token LPAREN
%token RPAREN	
%token IF 			
%token THEN	
%token ELSE			
%token ENDIF		
%token WHILE	
%token DO			
%token ENDDO		


%type <tzzht::AVar *> aexp
%type <tzzht::BVar *> bexp
%type <tzzht::Statement *> statement

%start process

%%
process: { cout << "*** PARSER BEGIN ***" << endl; }
    | statement { driver.Init($1); }
    | COBEGIN statement CONCUR statement COEND { driver.Init($2, $4); }
    ;
    
aexp:
    NUMBER { $$ = new tzzht::AVar(tzzht::AVar::AVarType::Constant, $1); }
    | IDENTIFER { $$ = new tzzht::AVar(tzzht::AVar::AVarType::Variable, $1); }
    | aexp PLUS aexp { $$ = new tzzht::AVar(tzzht::AVar::AVarType::Plus, $1, $3); }
    | aexp MINUS aexp { $$ = new tzzht::AVar(tzzht::AVar::AVarType::Minus, $1, $3); }
    | aexp TIMES aexp { $$ = new tzzht::AVar(tzzht::AVar::AVarType::Times, $1, $3); }

bexp:
    TRUE { $$ = new tzzht::BVar(tzzht::BVar::BVarType::TOF, true); }
    | FALSE { $$ = new tzzht::BVar(tzzht::BVar::BVarType::TOF, false); }
    | aexp EQUAL aexp { $$ = new tzzht::BVar(tzzht::BVar::BVarType::Equal, $1, $3); }
    | aexp LAE aexp { $$ = new tzzht::BVar(tzzht::BVar::BVarType::LAE, $1, $3); }
    | NOT bexp { $$ = new tzzht::BVar(tzzht::BVar::BVarType::Not, $2); }
    | bexp CONJUNCTION bexp { $$ = new tzzht::BVar(tzzht::BVar::BVarType::And, $1, $3); }
    | bexp DISCONJUNCTION bexp { $$ = new tzzht::BVar(tzzht::BVar::BVarType::Or, $1, $3); }

statement:
    SKIP SEMICOLON { $$ = new tzzht::Statement(tzzht::Statement::StatementType::Skip); }
    | IDENTIFER ASSIGNMENT aexp SEMICOLON { 
            $$ = new tzzht::Statement(tzzht::Statement::StatementType::Assignment,$1, $3); }
    | statement SEMICOLON statement { $$ = new tzzht::Statement(tzzht::Statement::StatementType::Sequential, $1, $3); }
    | WAIT LPAREN bexp RPAREN SEMICOLON { $$ = new tzzht::Statement(tzzht::Statement::StatementType::Wait, $3); }
    | IF bexp THEN statement ELSE statement ENDIF SEMICOLON { $$ = new tzzht::Statement(tzzht::Statement::StatementType::Conditional, $2, $4, $6); }
    | WHILE bexp DO statement ENDDO SEMICOLON { $$ = new tzzht::Statement(tzzht::Statement::StatementType::While, $2, $4); }

%%

/* Parser实现错误处理接口 */
void tzzht::Parser::error(const location &loc , const std::string &message) {
        
        // Location should be initialized inside scanner action, but is not in this example.
        // Let's grab location directly from driver class.
	// cout << "Error: " << message << endl << "Location: " << loc << endl;
	
        cout << "Error: " << message << endl << "Error location: " << driver.location() << endl;
}
