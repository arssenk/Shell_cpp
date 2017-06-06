#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "mygrep.h"

using namespace std;

//
// Created by arsen on 04.06.17.
//
int main(int argc, char **argv){

    std::vector<std::string> vec;
    for (int i = 1; i < argc; ++i) {
        std::string str(argv[i]);
        vec.push_back(str);
    }
    parseInput(vec);


}