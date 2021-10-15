/* 
 * 定义接口结构
 * File:   imp.h
 * Author: tzzht
 *
 * Created on Oct. 15, 2021, 16:54
 */
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
    void GenerateKS();
    void Draw();
    void Init(Statement *statement)
    {
        m_statement1_ = statement;
        m_programtype_ = Sequential;
        std::cout << std::endl << "Sequential Program init success!" << std::endl << std::endl;
    }
    void Init(Statement *statement1, Statement *statement2)
    {
        m_statement1_ = statement1;
        m_statement2_ = statement2;
        m_programtype_ = Concurrent;
        std::cout << std::endl << "Concurrent Program init success!" << std::endl << std::endl;
    }
    
    enum ProgramType
    {
        Sequential = 1,
        Concurrent
    };
 
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
    Statement *m_statement1_;
    Statement *m_statement2_;
    ProgramType m_programtype_;
    unsigned int m_location_;          // Used by scanner
};

}

#endif // INTERPRETER_H
