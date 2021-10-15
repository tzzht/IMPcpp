/* 
 * 定义结构
 * File:   imp.h
 * Author: tzzht
 *
 * Created on Oct. 15, 2021, 16:54
 */
#ifndef IMP_H
#define IMP_H

#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>
#include <set>
#include <map>


namespace tzzht {

class AVar;

class Identifer
{
public:
    static int IdentiferValue(char identifer);
    static void IdentiferEvaluation(char identifer, AVar *avar);
    static std::map<char, int> all_identifer;
    static std::map<char, int> CreateAll_identifer()
    {
        std::map<char, int> tmp_map;
        return tmp_map;
    }
    // static std::map<char, int> CreateChar_to_int()
    // {
    //     std::map<char, int> temp_map;
    //     return temp_map;
    // }
    // static std::map<int, char> CreateInt_to_char()
    // {
    //     std::map<int, char> temp_map;
    //     return temp_map;
    // }
    // static std::vector<int> CreateAll_identifer_value()
    // {
    //     std::vector<int> tmp_vec;
    //     return tmp_vec;
    // }
    // static std::map<char, int> char_to_int;
    // static std::map<int, char> int_to_char;
    // static std::vector<int> all_identifer_value;
    // static int identifer_num;
};

class AVar
{
public:
    enum AVarType
    {
        Constant = 1,
        Variable,
        Plus,
        Minus,
        Times
    };
    AVar();
    AVar(const AVarType avartype, int num);
    AVar(const AVarType avertype, char identifer);
    AVar(const AVarType avartype, AVar *left, AVar *right);
    std::string tostring() const;
    int value();
private:
    AVarType m_avartype_;
    int m_constant_;
    char m_identifer_;
    AVar *m_left_;
    AVar *m_right_;
};





class BVar
{
public:
    enum BVarType
    {
        TOF = 1,
        Equal,
        LAE,
        Not,
        And,
        Or
    };
    BVar();
    BVar(const BVarType bvartype, bool boolean);
    BVar(const BVarType bvartype, AVar *left, AVar *right);
    BVar(const BVarType bvartype, BVar *left);
    BVar(const BVarType bvartype, BVar *left, BVar *right);

    std::string tostring() const;
    bool value();
private:
    AVar *m_left_a_;
    AVar *m_right_a_;
    BVar *m_left_b_;
    BVar *m_right_b_;
    bool m_tof_;
    BVarType m_bvartype_;
};

class Statement
{
public:
    enum StatementType
    {
        Skip = 1,
        Assignment,
        Sequential,//
        Wait,
        Conditional,//
        While//
    };
    Statement();
    Statement(const StatementType statementtype);
    Statement(const StatementType statementtype, char identifer, AVar *avar);
    Statement(const StatementType statementtype, Statement *left, Statement *right);
    Statement(const StatementType statementtype, BVar *bvar);
    Statement(const StatementType statementtype, BVar *bvar, Statement *left, Statement *right);
    Statement(const StatementType statementtype, BVar *bvar, Statement *left);
    void PrintLabeledProgram();
    void PrintStatementtype()
    {
        std::cout << m_statement_type_ << std::endl;
    }
    void GenerateFirstOrderFormula(int pc, int pc_prime);
private:
    Statement *m_left_;
    Statement *m_right_;
    char m_identifer_;
    AVar *m_apointer_;
    BVar *m_bpointer_;
    StatementType m_statement_type_;
    int m_label1_;
    int m_label2_;
};

class FirstOrderFormula
{
public:
    enum FirstOrderFormulaType
    {
        Skip = 1,
        Assignment,
        Branch,
    };
    FirstOrderFormula();
    //Skip
    FirstOrderFormula(FirstOrderFormulaType firstorderformulatype, int pc, int pc_prime);
    //Assignment
    FirstOrderFormula(FirstOrderFormulaType firstorderformulatype, int pc, int pc_prime, char identifer, AVar *apointer);
    //Wait
    FirstOrderFormula(FirstOrderFormulaType firstorderformulatype, int pc, int pc_prime, BVar *bpointer);
    std::string tostring() const;
    static void PrintAllFirstOrderFormula();
    static std::vector<FirstOrderFormula> all_first_order_formula;
    static std::vector<FirstOrderFormula> CreateAll_first_order_formula()
    {
        std::vector<FirstOrderFormula> temp_vec;
        return temp_vec;
    }
    static std::vector<FirstOrderFormula> GetFOFsStartFromPC(int pc);

    FirstOrderFormulaType m_firstorderformulatype_;
    int m_pc_;
    int m_pc_prime_;
    char m_identifer_;
    AVar *m_apointer_;
    BVar *m_bpointer_;
};


class ConcurrentFirstOrderFormula
{
public:
    enum ConcurrentFirstOrderFormulaType
    {
        Skip = 1,
        Assignment,
        Branch,
    };
    ConcurrentFirstOrderFormula();
    ConcurrentFirstOrderFormula(FirstOrderFormula firstorderformula, int pc2);
    ConcurrentFirstOrderFormula(int pc1, FirstOrderFormula firstorderformula);
    std::string tostring() const;
    static void PrintAllConcurrentFirstOrderFormula();
    static std::set<ConcurrentFirstOrderFormula> all_concurrent_first_order_formula;
    static std::set<ConcurrentFirstOrderFormula> CreateAll_concurrent_first_order_formula()
    {
        std::set<ConcurrentFirstOrderFormula> temp_set;
        return temp_set;
    }
    static void FirstOrderFormulaIntersection(std::vector<FirstOrderFormula> fof1, std::vector<FirstOrderFormula> fof2);
    static std::vector<ConcurrentFirstOrderFormula> GetCFOFsStartFromPCs(int pc1, int pc2);

    bool operator < (const ConcurrentFirstOrderFormula & right) const
    {
        return tostring() < right.tostring();
    }

    ConcurrentFirstOrderFormulaType m_concurrentfirstorderformulatype_;
    int m_pc1_;
    int m_pc1_prime_;
    int m_pc2_;
    int m_pc2_prime_;
    char m_identifer_;
    AVar *m_apointer_;
    BVar *m_bpointer_;
};

class State
{
public:
    State();
    State(int pc, std::map<char, int> all_identifer);

    //void eval(char identifer, AVar *apointer);

    int m_pc_;
    std::map<char, int> m_all_identifer_;
    static std::set<State> all_states;
    static std::set<State> CreateAll_states()
    {
        std::set<State> tmp_set;
        return tmp_set;
    }

    bool operator < (const State & right) const
    {
        return tostring() < right.tostring();
    }

    std::string tostring() const;
};

class ConcurrentState
{
public:
    ConcurrentState();
    ConcurrentState(int pc1, int pc2, std::map<char, int> all_identifer);

    //void eval(char identifer, AVar *apointer);

    int m_pc1_;
    int m_pc2_;
    std::map<char, int> m_all_identifer_;
    static std::set<ConcurrentState> all_concurrent_states;
    static std::set<ConcurrentState> CreateAll_concurrent_states()
    {
        std::set<ConcurrentState> tmp_set;
        return tmp_set;
    }

    bool operator < (const ConcurrentState & right) const
    {
        return tostring() < right.tostring();
    }

    std::string tostring() const;
};

class Transition
{
public:
    Transition();
    Transition(State left, State right);
    static std::vector<Transition> all_transitions; 
    static std::vector<Transition> CreateAll_transitions()
    {
        std::vector<Transition> tmp_vec;
        return tmp_vec;
    }
    static void PrintAllTransitions();
    std::string tostring() const;
    State m_left_;
    State m_right_;
};

class ConcurrentTransition
{
public:
    ConcurrentTransition();
    ConcurrentTransition(ConcurrentState left, ConcurrentState right);
    static std::vector<ConcurrentTransition> all_concurrent_transitions; 
    static std::vector<ConcurrentTransition> CreateAll_concurrent_transitions()
    {
        std::vector<ConcurrentTransition> tmp_vec;
        return tmp_vec;
    }
    static void PrintAllConcurrentTransition();
    std::string tostring() const;
    ConcurrentState m_left_;
    ConcurrentState m_right_;
};




}

#endif //IMP_H