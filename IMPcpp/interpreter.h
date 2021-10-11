#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>

#include "scanner.h"

#include "parser.hpp"

#include "imp.h"

namespace tzzht {


class Interpreter
{
public:
    Interpreter();
    
    /* 调用parser, 成功返回0, 失败返回1 */
    int parse();
    
    void PrintLabeledProgram();
    void PrintFirstOrderFormula();

    void Init(Statement *statement)
    {
        m_statement_ = statement;
        std::cout << std::endl << "Statement tree initialization success!" << std::endl << std::endl;
    }
 
    /**
     * This is needed so that Scanner and Parser can call some
     * methods that we want to keep hidden from the end user.
     */
    friend class Parser;
    friend class Scanner;
    
private:
    // Used internally by Scanner YY_USER_ACTION to update location indicator
    void increaseLocation(unsigned int loc);
    
    // Used to get last Scanner location. Used in error messages.
    unsigned int location() const;
    
private:
    Scanner m_scanner_;
    Parser m_parser_;
    Statement *m_statement_;
    unsigned int m_location_;          // Used by scanner
};

}

#endif // INTERPRETER_H
