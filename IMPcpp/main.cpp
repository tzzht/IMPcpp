/* 
 * 主函数
 * File:   imp.cpp
 * Author: tzzht
 *
 * Created on Oct. 15, 2021, 16:54
 */
#include <iostream>
#include "scanner.h"
#include "parser.hpp"
#include "interpreter.h"

using namespace tzzht;
using namespace std;

int main(int argc, char **argv) 
{
    Interpreter i;
    int res = i.parse();
    cout << "*** PARSER END ***" << endl;
    cout << "Parse complete. Result = " << res << endl;
    i.PrintLabeledProgram();
    i.PrintFirstOrderFormula();
    i.GenerateKS();
    i.Draw();
    return res;
}
