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
    cout << "Parse complete. Result = " << res << endl;
    i.PrintLabeledProgram();
    i.PrintFirstOrderFormula();
    return res;
}
