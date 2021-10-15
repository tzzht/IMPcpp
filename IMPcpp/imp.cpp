/* 
 * 实现相关函数
 * File:   imp.cpp
 * Author: tzzht
 *
 * Created on Oct. 15, 2021, 16:54
 */
#include "imp.h"

#include <iostream>
#include <sstream>

using namespace tzzht;

int global_label = 0;
std::vector<FirstOrderFormula> FirstOrderFormula::all_first_order_formula = FirstOrderFormula::CreateAll_first_order_formula();
std::set<ConcurrentFirstOrderFormula> ConcurrentFirstOrderFormula::all_concurrent_first_order_formula = ConcurrentFirstOrderFormula::CreateAll_concurrent_first_order_formula();
std::set<State> State::all_states = State::CreateAll_states();
std::set<ConcurrentState> ConcurrentState::all_concurrent_states = ConcurrentState::CreateAll_concurrent_states();

std::vector<Transition> Transition::all_transitions = Transition::CreateAll_transitions();
std::vector<ConcurrentTransition> ConcurrentTransition::all_concurrent_transitions = ConcurrentTransition::CreateAll_concurrent_transitions();

std::map<char, int> Identifer::all_identifer = Identifer::CreateAll_identifer();



// std::map<char, int> Identifer::char_to_int = Identifer::CreateChar_to_int();
// std::map<int, char> Identifer::int_to_char = Identifer::CreateInt_to_char();
// std::vector<int> Identifer::all_identifer_value = Identifer::CreateAll_identifer_value();

// int Identifer::identifer_num = 0;


int Identifer::IdentiferValue(char identifer)
{
   // return all_identifer_value[char_to_int[identifer]];
   if(all_identifer.find(identifer) == all_identifer.end())
   {
       std::cout << "error" << std::endl;
       return -1;
   }
   else
   {
       return all_identifer[identifer];
   }
}

void Identifer::IdentiferEvaluation(char identifer, AVar *avar)
{
    // if(char_to_int.find(identifer) == char_to_int.end())//如果这个identifier是新的
    // {
        
    //     char_to_int[identifer] = identifer_num;
    //     int_to_char[identifer_num] = identifer;
        
    //     all_identifer_value.push_back(avar->value());
    //     identifer_num ++;
        
    // }
    // else//如果这个identifer已经有了
    // {
    //     all_identifer_value[char_to_int[identifer]] = avar->value();
    // }
    all_identifer[identifer] = avar->value();
}




AVar::AVar() :
    m_constant_(),
    m_identifer_(),
    m_left_(),
    m_right_(){}

AVar::AVar(const AVarType avartype, int num)
{
    if(avartype == Constant)
    {
        m_avartype_ = avartype;
        m_constant_ = num;
        
    }
    else
    {
        std::cout << "error!" << std::endl;
    }
}

AVar::AVar(const AVarType avertype, char identifer)
{
    if(avertype == Variable)
    {
        m_avartype_ = avertype;
        m_identifer_ = identifer;
    }
    else
    {
        std::cout << "error!" << std::endl;
    }
}

AVar::AVar(const AVarType avartype, AVar *left, AVar *right)
{
    m_avartype_ = avartype;
    m_left_ = left;
    m_right_ = right;
}

std::string AVar::tostring() const
{
    std::stringstream ts;
    switch (m_avartype_)
    {
    case Constant:
        ts << m_constant_;
        break;
    case Variable:
        ts << m_identifer_;
        break;
    case Plus:
        ts << "(" << m_left_->tostring();
        ts << " + ";
        ts << m_right_->tostring() << ")";
        break;
    case Minus:
        ts << "(" << m_left_->tostring();
        ts << " - ";
        ts << m_right_->tostring() << ")";
        break;
    case Times:
        ts << "(" << m_left_->tostring();
        ts << " * ";
        ts << m_right_->tostring() << ")";
        break;
    default:
        std::cout << "error" << std::endl;
        break;
    }
    return ts.str();
}

int AVar::value()
{
    int ret;
    
    switch (m_avartype_)
    {
    case Constant:
        ret = m_constant_%3;
        
        break;
    case Variable:
        ret = (Identifer::IdentiferValue(m_identifer_))%3;
        break;
    case Plus:
        ret = (m_left_->value() + m_right_->value())%3;
        break;
    case Minus:
        ret = (m_left_->value() - m_right_->value())%3;
        break;
    case Times:
        ret = (m_left_->value() * m_right_->value())%3;
        break;
    default:
        std::cout << "error" << std::endl;
        ret = -1;
        break;
    }
    return ret;
}



BVar::BVar() :
    m_left_a_(),
    m_right_a_(),
    m_left_b_(),
    m_right_b_(),
    m_tof_(),
    m_bvartype_(){}


BVar::BVar(const BVarType bvartype, bool boolean)
{
    m_bvartype_ = bvartype;
    m_tof_ = boolean;
}

BVar::BVar(const BVarType bvartype, AVar *left, AVar *right)
{
    m_left_a_ = left;
    m_right_a_ = right;
    m_bvartype_ = bvartype;
}

BVar::BVar(const BVarType bvartype, BVar *left)
{
    m_bvartype_ = bvartype;
    m_left_b_ = left;
}

BVar::BVar(const BVarType bvartype, BVar *left, BVar *right)
{
    m_bvartype_ = bvartype;
    m_left_b_ = left;
    m_right_b_ = right;
}

std::string BVar::tostring() const
{
    std::stringstream ts;
    switch (m_bvartype_)
    {
    case TOF:
        if(m_tof_ == true) ts << "true";
        else ts << "false";
        break;
    case Equal:
        ts << "(";
        ts << m_left_a_->tostring();
        ts << " == ";
        ts << m_right_a_->tostring();
        ts << ")";
        break;
    case LAE:
        ts << "(";
        ts << m_left_a_->tostring();
        ts << " <= ";
        ts << m_right_a_->tostring();
        ts << ")";
        break;
    case Not:
        ts << "!";
        //ts << "(";
        ts << m_left_b_->tostring();
        //ts << ")";
        break;
    case And:
        ts << "(";
        ts << m_left_b_->tostring();
        ts << " and ";
        ts << m_right_b_->tostring();
        ts << ")";
        break;
    case Or:
        ts << "(";
        ts << m_left_b_->tostring();
        ts << " or ";
        ts << m_right_b_->tostring();
        ts << ")";
        break;
    default:
        std::cout << "error" << std::endl;
        break;
    }
    return ts.str();
}

bool BVar::value()
{
    bool ret = false;
    switch (m_bvartype_)
    {
    case TOF:
        ret = m_tof_;
        break;
    case Equal:
        ret = m_left_a_->value() == m_right_a_->value();
        break;
    case LAE:
        ret = m_left_a_->value() <= m_right_a_->value();
        break;
    case Not:
        ret = !m_left_b_->value();
        break;
    case And:
        ret = m_left_b_->value() && m_right_b_->value();
        break;
    case Or:
        ret = m_left_b_->value() || m_right_b_->value();
        break;
    default:
        std::cout << "error" << std::endl;
        break;
    }
    return ret;
}


Statement::Statement() :
    m_left_(),
    m_right_(),
    m_identifer_(),
    m_apointer_(),
    m_bpointer_(),
    m_statement_type_(),
    m_label1_(),
    m_label2_(){}

//skip
Statement::Statement(const StatementType statementtype)
{
    m_statement_type_ = statementtype;
}

//assignment
Statement::Statement(const StatementType statementtype, char identifer, AVar *avar)
{
    m_statement_type_ = statementtype;
    m_identifer_ = identifer;
    m_apointer_ = avar;
}

//sequential
Statement::Statement(const StatementType statementtype, Statement *left, Statement *right)
{
    m_statement_type_ = statementtype;
    m_left_ = left;
    m_right_ = right;
    m_label1_ = global_label ++;
    //std::cout << m_statement_type_ << std::endl;
}

//Wait
Statement::Statement(const StatementType statementtype, BVar *bvar)
{
    m_statement_type_ = statementtype;
    m_bpointer_ = bvar;
}

//Conditional
Statement::Statement(const StatementType statementtype, BVar *bvar, Statement *left, Statement *right)
{
    m_statement_type_ = statementtype;
    m_bpointer_ =  bvar;
    m_left_ = left;
    m_right_ = right;
    m_label1_ = global_label ++;
    m_label2_ = global_label ++;
}

//While
Statement::Statement(const StatementType statementtype, BVar *bvar, Statement *left)
{
    m_statement_type_ = statementtype;
    m_bpointer_ = bvar;
    m_left_ = left;
    m_label1_ = global_label ++;
}

void Statement::PrintLabeledProgram()
{
    switch (m_statement_type_)
    {
    case Skip:
        std::cout << "skip;" << std::endl;
        break;
    case Assignment:
        std::cout << m_identifer_ << ":=" << m_apointer_->tostring() << ";" << std::endl;
        break;
    case Sequential:
        m_left_->PrintLabeledProgram();
        std::cout << "l" << m_label1_ << ":" << std::endl;
        m_right_->PrintLabeledProgram();
        break;
    case Wait:
        std::cout << "wait(" << m_bpointer_->tostring() << ");" << std::endl;
        break;
    case Conditional:
        std::cout << "if " << m_bpointer_->tostring() << " then" << std::endl;
        std::cout << "l" << m_label1_ << ":" << std::endl;
        m_left_->PrintLabeledProgram();
        std::cout << "else" << std::endl;
        std::cout << "l" << m_label2_ << ":" << std::endl;
        m_right_->PrintLabeledProgram();
        std::cout << "endif;" << std::endl;
        break;
    case While:
        std::cout << "while " << m_bpointer_->tostring() << " do" << std::endl;
        std::cout << "l" << m_label1_ << ":" << std::endl;
        m_left_->PrintLabeledProgram();
        std::cout << "enddo;" << std::endl;
        break;
    default:
        std::cout << "PrintLabeledProgram error! Statement type: " << m_statement_type_ << std::endl;
        break;
    }
}

void Statement::GenerateFirstOrderFormula(int pc, int pc_prime)
{
    switch (m_statement_type_)
    {
    case Skip:
        FirstOrderFormula::all_first_order_formula.push_back(FirstOrderFormula(FirstOrderFormula::FirstOrderFormulaType::Skip, pc, pc_prime));
        break;
    case Assignment:
        FirstOrderFormula::all_first_order_formula.push_back(FirstOrderFormula(FirstOrderFormula::FirstOrderFormulaType::Assignment, pc, pc_prime, m_identifer_, m_apointer_));
        break;
    case Sequential:
        m_left_->GenerateFirstOrderFormula(pc, m_label1_);
        m_right_->GenerateFirstOrderFormula(m_label1_, pc_prime);
        break;
    case Wait:
        FirstOrderFormula::all_first_order_formula.push_back(FirstOrderFormula(FirstOrderFormula::FirstOrderFormulaType::Branch, pc, pc, new BVar(BVar::BVarType::Not, m_bpointer_)));
        FirstOrderFormula::all_first_order_formula.push_back(FirstOrderFormula(FirstOrderFormula::FirstOrderFormulaType::Branch, pc, pc_prime, m_bpointer_));
        break;
    case Conditional:
        FirstOrderFormula::all_first_order_formula.push_back(FirstOrderFormula(FirstOrderFormula::FirstOrderFormulaType::Branch, pc, m_label1_, m_bpointer_));
        m_left_->GenerateFirstOrderFormula(m_label1_, pc_prime);
        FirstOrderFormula::all_first_order_formula.push_back(FirstOrderFormula(FirstOrderFormula::FirstOrderFormulaType::Branch, pc, m_label2_, new BVar(BVar::BVarType::Not, m_bpointer_)));
        m_right_->GenerateFirstOrderFormula(m_label2_, pc_prime);
        break;
    case While:
        FirstOrderFormula::all_first_order_formula.push_back(FirstOrderFormula(FirstOrderFormula::FirstOrderFormulaType::Branch, pc, m_label1_, m_bpointer_));
        m_left_->GenerateFirstOrderFormula(m_label1_, pc);
        FirstOrderFormula::all_first_order_formula.push_back(FirstOrderFormula(FirstOrderFormula::FirstOrderFormulaType::Branch, pc, pc_prime, new BVar(BVar::BVarType::Not, m_bpointer_)));
        break;
   default:
        break;
    }
}

FirstOrderFormula::FirstOrderFormula() :
    m_firstorderformulatype_(),
    m_pc_(0),
    m_pc_prime_(0),
    m_identifer_(0),
    m_apointer_(),
    m_bpointer_(){}

FirstOrderFormula::FirstOrderFormula(FirstOrderFormulaType firstorderformulatype, int pc, int pc_prime)
{
    m_firstorderformulatype_ = FirstOrderFormula::FirstOrderFormulaType::Skip;
    m_pc_ = pc;
    m_pc_prime_ = pc_prime;
}

FirstOrderFormula::FirstOrderFormula(FirstOrderFormulaType firstorderformulatype, int pc, int pc_prime, char identifer, AVar *apointer)
{
    m_firstorderformulatype_ = FirstOrderFormula::FirstOrderFormulaType::Assignment;
    m_pc_ = pc;
    m_pc_prime_ = pc_prime;
    m_identifer_ = identifer;
    m_apointer_ = apointer;
}

FirstOrderFormula::FirstOrderFormula(FirstOrderFormulaType firstorderformulatype, int pc, int pc_prime, BVar *bpointer)
{
    m_firstorderformulatype_ = FirstOrderFormula::FirstOrderFormulaType::Branch;
    m_pc_ = pc;
    m_pc_prime_ = pc_prime;
    m_bpointer_ = bpointer;
}

std::string FirstOrderFormula::tostring() const
{
    std::stringstream ts;
    switch (m_firstorderformulatype_)
    {
    case Skip:
        ts << "pc = " << m_pc_ << " and " << "pc' = " << m_pc_prime_ << " and same(V)";
        break;
    case Assignment:
        ts << "pc = " << m_pc_ << " and " << "pc' = " << m_pc_prime_ << " and " << m_identifer_ << " = ";
        ts << m_apointer_->tostring() << " and same(V\\\{" << m_identifer_ << "\})";
        break;
    case Branch:
        ts << "pc = " << m_pc_ << " and " << "pc' = " << m_pc_prime_ << " and " << m_bpointer_->tostring() << " and same(V)";
        break;
    default:
        std::cout << "error" << std::endl;
        break;
    }
    return ts.str();
}

void FirstOrderFormula::PrintAllFirstOrderFormula()
{
    for(auto it = all_first_order_formula.begin(); it != all_first_order_formula.end(); it ++)
    {
        std::cout << it->tostring() << std::endl;
    }
}

std::vector<FirstOrderFormula> FirstOrderFormula::GetFOFsStartFromPC(int pc)
{
    std::vector<FirstOrderFormula> ret;
    for(int i = 0; i < all_first_order_formula.size(); i ++)
    {
        if(all_first_order_formula[i].m_pc_ == pc)
        {
            ret.push_back(all_first_order_formula[i]);
        }
    }
    return ret;
}


ConcurrentFirstOrderFormula::ConcurrentFirstOrderFormula() :
    m_concurrentfirstorderformulatype_(),
    m_pc1_(),
    m_pc1_prime_(),
    m_pc2_(),
    m_pc2_prime_(),
    m_identifer_(),
    m_apointer_(),
    m_bpointer_() {}

ConcurrentFirstOrderFormula::ConcurrentFirstOrderFormula(FirstOrderFormula firstorderformula, int pc2)
{
    m_concurrentfirstorderformulatype_ = (ConcurrentFirstOrderFormulaType)firstorderformula.m_firstorderformulatype_;
    m_pc1_ = firstorderformula.m_pc_;
    m_pc1_prime_ = firstorderformula.m_pc_prime_;
    m_pc2_ = pc2;
    m_pc2_prime_ = pc2;
    m_identifer_ = firstorderformula.m_identifer_;
    m_apointer_ = firstorderformula.m_apointer_;
    m_bpointer_ = firstorderformula.m_bpointer_;
}
ConcurrentFirstOrderFormula::ConcurrentFirstOrderFormula(int pc1, FirstOrderFormula firstorderformula)
{
    m_concurrentfirstorderformulatype_ = (ConcurrentFirstOrderFormulaType)firstorderformula.m_firstorderformulatype_;
    m_pc1_ = pc1;
    m_pc1_prime_ = pc1;
    m_pc2_ = firstorderformula.m_pc_;
    m_pc2_prime_ = firstorderformula.m_pc_prime_;
    m_identifer_ = firstorderformula.m_identifer_;
    m_apointer_ = firstorderformula.m_apointer_;
    m_bpointer_ = firstorderformula.m_bpointer_;
}


std::string ConcurrentFirstOrderFormula::tostring() const
{
    std::stringstream ts;
    switch (m_concurrentfirstorderformulatype_)
    {
    case Skip:
        ts << "pc1 = " << m_pc1_ << " and " << "pc1' = " << m_pc1_prime_ << " and " << "pc2 = " << m_pc2_ << " and " << "pc2' = " << m_pc2_prime_ << " and same(V)";
        break;
    case Assignment:
        ts << "pc1 = " << m_pc1_ << " and " << "pc1' = " << m_pc1_prime_ << " and " << "pc2 = " << m_pc2_ << " and " << "pc2' = " << m_pc2_prime_ << " and " << m_identifer_ << " = ";
        ts << m_apointer_->tostring() << " and same(V\\\{" << m_identifer_ << "\})";
        break;
    case Branch:
        ts << "pc1 = " << m_pc1_ << " and " << "pc1' = " << m_pc1_prime_ << " and " << "pc2 = " << m_pc2_ << " and " << "pc2' = " << m_pc2_prime_ << " and " << m_bpointer_->tostring() << " and same(V)";
        break;
    default:
        std::cout << "error" << std::endl;
        break;
    }
    return ts.str();
}

void ConcurrentFirstOrderFormula::PrintAllConcurrentFirstOrderFormula()
{
    for(auto it = all_concurrent_first_order_formula.begin(); it != all_concurrent_first_order_formula.end(); it ++)
    {
        std::cout << it->tostring() << std::endl;
    }
}

void ConcurrentFirstOrderFormula::FirstOrderFormulaIntersection(std::vector<FirstOrderFormula> all_fof1, std::vector<FirstOrderFormula> all_fof2)
{
    // for(int i = 0; i < all_fof1.size(); i ++) std::cout << all_fof1[i].tostring() << std::endl;
    // std::cout << std::endl;
    // for(int i = 0; i < all_fof2.size(); i ++) std::cout << all_fof2[i].tostring() << std::endl;
    for(int i = 0; i < all_fof1.size(); i ++)
    {
        for(int j = 0; j < all_fof2.size(); j ++)
        {
            FirstOrderFormula fof1 = all_fof1[i];
            FirstOrderFormula fof2 = all_fof2[j];
            ConcurrentFirstOrderFormula::all_concurrent_first_order_formula.insert(ConcurrentFirstOrderFormula(fof1, fof2.m_pc_));
            ConcurrentFirstOrderFormula::all_concurrent_first_order_formula.insert(ConcurrentFirstOrderFormula(fof1.m_pc_, fof2));
        }
    }
}

std::vector<ConcurrentFirstOrderFormula> ConcurrentFirstOrderFormula::GetCFOFsStartFromPCs(int pc1, int pc2)
{
    std::vector<ConcurrentFirstOrderFormula> ret;
    for(auto it = all_concurrent_first_order_formula.begin(); it != all_concurrent_first_order_formula.end(); it ++)
    {
        if(it->m_pc1_ == pc1 && it->m_pc2_ == pc2)
        {
            ret.push_back(*it);
        }
    }
    return ret;
}
    


State::State() :
    m_pc_(),
    m_all_identifer_() {}


State::State(int pc, std::map<char, int> all_identifer)
{
    m_pc_ = pc;
    m_all_identifer_ = all_identifer;
}

std::string State::tostring() const
{
    std::stringstream ts;
    ts << m_pc_ << std::endl;
    for(auto it = m_all_identifer_.begin(); it != m_all_identifer_.end(); it ++)
    {
        ts << ", " << it->first << " = " << it->second; 
    }
    return ts.str();
}

// void State::eval(char identifer, AVar *apointer)
// {
//     m_all_identifer_[identifer] = apointer->value();
// }


ConcurrentState::ConcurrentState() :
    m_pc1_(),
    m_pc2_(),
    m_all_identifer_() {}


ConcurrentState::ConcurrentState(int pc1, int pc2, std::map<char, int> all_identifer)
{
    m_pc1_ = pc1;
    m_pc2_ = pc2;
    m_all_identifer_ = all_identifer;
}

std::string ConcurrentState::tostring() const
{
    std::stringstream ts;
    ts << m_pc1_ << ", " << m_pc2_;
    for(auto it = m_all_identifer_.begin(); it != m_all_identifer_.end(); it ++)
    {
        ts << ", " << it->first << " = " << it->second; 
    }
    return ts.str();
}

Transition::Transition() :
    m_left_(),
    m_right_() {}

Transition::Transition(State left, State right)
{
    m_left_ = left;
    m_right_ = right;
}

void Transition::PrintAllTransitions()
{
    for(int i = 0; i < all_transitions.size(); i ++)
    {
        std::cout << all_transitions[i].tostring() << std::endl;
    }
}

std::string Transition::tostring() const
{
    std::stringstream ts;
    ts << "(" << m_left_.tostring() << ") -> (" << m_right_.tostring() << ")";
    return ts.str();
}

ConcurrentTransition::ConcurrentTransition() :
    m_left_(),
    m_right_() {}

ConcurrentTransition::ConcurrentTransition(ConcurrentState left, ConcurrentState right)
{
    m_left_ = left;
    m_right_ = right;
}

void ConcurrentTransition::PrintAllConcurrentTransition()
{
    for(int i = 0; i < all_concurrent_transitions.size(); i ++)
    {
        std::cout << all_concurrent_transitions[i].tostring() << std::endl;
    }
}

std::string ConcurrentTransition::tostring() const
{
    std::stringstream ts;
    ts << "(" << m_left_.tostring() << ") -> (" << m_right_.tostring() << ")";
    return ts.str();
}