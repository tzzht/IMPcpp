#ifndef SCANNER_H
#define SCANNER_H


#if ! defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer tzzht_FlexLexer // the trick with prefix
#include <FlexLexer.h>
#endif

// 因为 Bison 3 使用 symbol_type, 所以我们必须更改returne type
#undef YY_DECL
#define YY_DECL tzzht::Parser::symbol_type tzzht::Scanner::get_next_token()

#include "parser.hpp" // this is needed for symbol_type

namespace tzzht {

// Forward declare interpreter to avoid include. Header is added inimplementation file.
class Interpreter; 
    
class Scanner : public yyFlexLexer {
public:
        Scanner(Interpreter &driver) : m_driver_(driver) {}
	virtual ~Scanner() {}
	virtual tzzht::Parser::symbol_type get_next_token();
        
private:
    Interpreter &m_driver_;
};

}

#endif
