#include "interpreter.h"
//#include "imp.h"
#include <sstream>

using namespace tzzht;


/* 构造函数, 使用初始化列表 */
Interpreter::Interpreter() :
    m_scanner_(*this),
    m_parser_(m_scanner_, *this),
    m_location_(0),
    m_statement_()
{}




int Interpreter::parse() {
    m_location_ = 0;
    /* Run the syntactic analysis, and return 0 on success, 1 otherwise. Both routines are equivalent, operator() being more C++ish. */
    return m_parser_.parse();
}


void Interpreter::increaseLocation(unsigned int loc) {
    m_location_ += loc;
    //cout << "increaseLocation(): " << loc << ", total = " << m_location_ << endl;
}

unsigned int Interpreter::location() const {
    return m_location_;
}

void Interpreter::PrintLabeledProgram()
{
    std::cout << std::endl << "LabeledProgram: " << std::endl;
    std::cout << "l0:" << std::endl;
    m_statement_->PrintLabeledProgram();
    std::cout << "l-1:" << std::endl;
    std::cout << std::endl;
    //std::cout << "LabeledProgram generation success!" << std::endl << std::endl;
}

void Interpreter::PrintFirstOrderFormula()
{
    m_statement_->GenerateFirstOrderFormula(0, -1);
    //std::cout << "First Order Formula generation success!" << std::endl << std::endl;
    std::cout << "First Order Formula: " << std::endl;
    std::cout << FirstOrderFormula::all_first_order_formula.size() << std::endl;
    FirstOrderFormula::PrintAllFirstOrderFormula();
}