#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "parser.hh"
#include "driver.h"

// #include "build/y.tab.h"
using namespace std;

// Global variable to hold the root of the AST
unique_ptr<TopLevel> root;

int main(int argc, char *argv[])
{

    if (argc == 1)
    {
        cerr << "Usage: " << argv[0] << " <source-file.c>\n";
        return 1;
    }
    Mreow::Driver drv{};
    int n = drv.parse(argv[1]);
    if (n != 0)
        std::cout << "fail\n";
    else
        std::cout << "ok\n";
    root->print();
}