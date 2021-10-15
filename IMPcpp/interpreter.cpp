/* 
 * 实现接口相关函数
 * File:   imp.cpp
 * Author: tzzht
 *
 * Created on Oct. 15, 2021, 16:54
 */
#include "interpreter.h"
//#include "imp.h"
#include <sstream>
#include <fstream> 
using namespace tzzht;


/* 构造函数, 使用初始化列表 */
Interpreter::Interpreter() :
    m_scanner_(*this),
    m_parser_(m_scanner_, *this),
    m_location_(0),
    m_statement1_(),
    m_statement2_(),
    m_programtype_()
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
    if(m_programtype_ == Sequential)
    {
        std::cout << "l-1:" << std::endl;
        m_statement1_->PrintLabeledProgram();
        std::cout << "l-2:" << std::endl;
        //std::cout << "LabeledProgram generation success!" << std::endl << std::endl;
    }
    else if(m_programtype_ == Concurrent)
    {
        std::cout << "cobegin" << std::endl;
        std::cout << "l-1:" << std::endl;
        m_statement1_->PrintLabeledProgram();
        std::cout << "l-2:" << std::endl;
        std::cout << "||" << std::endl;
        std::cout << "l-3:" << std::endl;
        m_statement2_->PrintLabeledProgram();
        std::cout << "l-4:" << std::endl;
        std::cout << "coend" << std::endl;
    }
    else
    {
        std::cout << "error!" << std::endl;
    }
    std::cout << std::endl;
}

void Interpreter::PrintFirstOrderFormula()
{
    std::cout << "First Order Formula: " << std::endl;
    if(m_programtype_ == Sequential)
    {
        m_statement1_->GenerateFirstOrderFormula(-1, -2);
        //std::cout << "First Order Formula generation success!" << std::endl << std::endl;
        std::cout << FirstOrderFormula::all_first_order_formula.size() << std::endl;
        FirstOrderFormula::PrintAllFirstOrderFormula();
    }
    else if(m_programtype_ == Concurrent)
    {
        std::vector<FirstOrderFormula> emptyvec;
        m_statement1_->GenerateFirstOrderFormula(-1, -2);
        std::vector<FirstOrderFormula> all_fof1 = FirstOrderFormula::all_first_order_formula;
        FirstOrderFormula::all_first_order_formula.swap(emptyvec);
        m_statement2_->GenerateFirstOrderFormula(-3, -4);
        std::vector<FirstOrderFormula> all_fof2 = FirstOrderFormula::all_first_order_formula;
        FirstOrderFormula::all_first_order_formula.swap(emptyvec);
        ConcurrentFirstOrderFormula::FirstOrderFormulaIntersection(all_fof1, all_fof2);
        ConcurrentFirstOrderFormula::PrintAllConcurrentFirstOrderFormula();
    }
    else
    {
        std::cout << "error!" << std::endl;
    }
    std::cout << std::endl;
}

void Interpreter::GenerateKS()
{
    std::cout << "Kripke Structure: " << std::endl;
    if(m_programtype_ == Sequential)
    {
        std::set<State> not_yet_caled;
        std::map<char, int> empty_map;
        State init_state = State(-1, empty_map);
        State::all_states.insert(init_state);
        not_yet_caled.insert(init_state);
        while(!not_yet_caled.empty())
        {
            State cal_state = *(not_yet_caled.begin());
            not_yet_caled.erase(cal_state);
            std::vector<FirstOrderFormula> fofs =  FirstOrderFormula::GetFOFsStartFromPC(cal_state.m_pc_);
            for(int i = 0; i < fofs.size(); i ++)
            {
                FirstOrderFormula fof = fofs[i];
                State tmp_state = cal_state;
                Identifer::all_identifer = tmp_state.m_all_identifer_;
                switch (fof.m_firstorderformulatype_)
                {
                case FirstOrderFormula::Skip:
                    tmp_state.m_pc_ = fof.m_pc_prime_;
                    //还未计算过的state
                    Transition::all_transitions.push_back(Transition(cal_state, tmp_state));
                    if(State::all_states.find(tmp_state) == State::all_states.end())
                    {
                        State::all_states.insert(tmp_state);
                        not_yet_caled.insert(tmp_state);
                    }
                    break;
                case FirstOrderFormula::Assignment:
                    tmp_state.m_pc_ = fof.m_pc_prime_;
                    Identifer::all_identifer = tmp_state.m_all_identifer_;
                    Identifer::IdentiferEvaluation(fof.m_identifer_, fof.m_apointer_);
                    tmp_state.m_all_identifer_ = Identifer::all_identifer;
                    Transition::all_transitions.push_back(Transition(cal_state, tmp_state));
                    if(State::all_states.find(tmp_state) == State::all_states.end())
                    {
                        State::all_states.insert(tmp_state);
                        not_yet_caled.insert(tmp_state);
                    }
                    break;
                case FirstOrderFormula::Branch:
                    std::cout << fof.m_bpointer_->tostring() << " " << fof.m_bpointer_->value() << std::endl;
                    if(fof.m_bpointer_->value() == true)
                    {
                        tmp_state.m_pc_ = fof.m_pc_prime_;
                        Transition::all_transitions.push_back(Transition(cal_state, tmp_state));
                        if(State::all_states.find(tmp_state) == State::all_states.end())
                        {
                            State::all_states.insert(tmp_state);
                            not_yet_caled.insert(tmp_state);
                        }
                    }
                    break;
                default:
                    std::cout << "error!" << std::endl;
                    break;
                }
            }
        }
        Transition::PrintAllTransitions();
    }
    else if(m_programtype_ == Concurrent)
    {
        std::set<ConcurrentState> not_yet_caled;
        std::map<char, int> empty_map;
        ConcurrentState init_concurrent_state = ConcurrentState(-1, -3, empty_map);
        ConcurrentState::all_concurrent_states.insert(init_concurrent_state);
        not_yet_caled.insert(init_concurrent_state);
        while(!not_yet_caled.empty())
        {
            ConcurrentState cal_concurrent_state = *(not_yet_caled.begin());
            not_yet_caled.erase(cal_concurrent_state);
            std::vector<ConcurrentFirstOrderFormula> cfofs =  ConcurrentFirstOrderFormula::GetCFOFsStartFromPCs(cal_concurrent_state.m_pc1_, cal_concurrent_state.m_pc2_);
            for(int i = 0; i < cfofs.size(); i ++)
            {
                ConcurrentFirstOrderFormula cfof = cfofs[i];
                ConcurrentState tmp_concurrent_state = cal_concurrent_state;
                Identifer::all_identifer = tmp_concurrent_state.m_all_identifer_;
                switch (cfof.m_concurrentfirstorderformulatype_)
                {
                case ConcurrentFirstOrderFormula::Skip:
                    tmp_concurrent_state.m_pc1_= cfof.m_pc1_prime_;
                    tmp_concurrent_state.m_pc2_ = cfof.m_pc2_prime_;
                    //还未计算过的state
                    ConcurrentTransition::all_concurrent_transitions.push_back(ConcurrentTransition(cal_concurrent_state, tmp_concurrent_state));
                    if(ConcurrentState::all_concurrent_states.find(tmp_concurrent_state) == ConcurrentState::all_concurrent_states.end())
                    {
                        ConcurrentState::all_concurrent_states.insert(tmp_concurrent_state);
                        not_yet_caled.insert(tmp_concurrent_state);
                    }
                    break;
                case ConcurrentFirstOrderFormula::Assignment:
                    tmp_concurrent_state.m_pc1_= cfof.m_pc1_prime_;
                    tmp_concurrent_state.m_pc2_ = cfof.m_pc2_prime_;
                    Identifer::all_identifer = tmp_concurrent_state.m_all_identifer_;
                    Identifer::IdentiferEvaluation(cfof.m_identifer_, cfof.m_apointer_);
                    tmp_concurrent_state.m_all_identifer_ = Identifer::all_identifer;
                    ConcurrentTransition::all_concurrent_transitions.push_back(ConcurrentTransition(cal_concurrent_state, tmp_concurrent_state));
                    if(ConcurrentState::all_concurrent_states.find(tmp_concurrent_state) == ConcurrentState::all_concurrent_states.end())
                    {
                        ConcurrentState::all_concurrent_states.insert(tmp_concurrent_state);
                        not_yet_caled.insert(tmp_concurrent_state);
                    }
                    break;
                case ConcurrentFirstOrderFormula::Branch:
                    if(cfof.m_bpointer_->value() == true)
                    {
                        tmp_concurrent_state.m_pc1_= cfof.m_pc1_prime_;
                        tmp_concurrent_state.m_pc2_ = cfof.m_pc2_prime_;
                        ConcurrentTransition::all_concurrent_transitions.push_back(ConcurrentTransition(cal_concurrent_state, tmp_concurrent_state));
                        if(ConcurrentState::all_concurrent_states.find(tmp_concurrent_state) == ConcurrentState::all_concurrent_states.end())
                        {
                            ConcurrentState::all_concurrent_states.insert(tmp_concurrent_state);
                            not_yet_caled.insert(tmp_concurrent_state);
                        }
                    }
                    break;
                default:
                    std::cout << "error!" << std::endl;
                    break;
                }
            }
        }
        ConcurrentTransition::PrintAllConcurrentTransition();
    }
    else
    {
        std::cout << "error!" << std::endl;
    }
    std::cout << std::endl;
}

void Interpreter::Draw()
{
    ofstream outfile("graph.dot", ios::trunc); 
    outfile << "digraph G{" << std::endl;
    
    if(m_programtype_ == Sequential)
    {
        for(int i = 0; i < Transition::all_transitions.size(); i ++)
        {
            outfile << "\"" << Transition::all_transitions[i].m_left_.tostring() << "\"" << " -> " << "\"" << Transition::all_transitions[i].m_right_.tostring() << "\";" << std::endl;
        }
    }
    else if(m_programtype_ == Concurrent)
    {
        for(int i = 0; i < ConcurrentTransition::all_concurrent_transitions.size(); i ++)
        {
            outfile << "\"" << ConcurrentTransition::all_concurrent_transitions[i].m_left_.tostring() << "\"" << " -> " << "\"" << ConcurrentTransition::all_concurrent_transitions[i].m_right_.tostring() << "\";" << std::endl;
        }
    }
    else
    {
        std::cout << "error!" << std::endl;
    }
    

    outfile << "}" << std::endl;
    outfile.close();
    system("dot -Tpng graph.dot -o graph.png");
    system("xdg-open graph.png");
}