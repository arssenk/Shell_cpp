//
// Created by oksana on 17.03.17.
//
#include "mkdir.h"
#include <iostream>
using namespace std;
using namespace boost::filesystem;
namespace fs = boost::filesystem;


int main(int argc, char **argv) {
    std::vector<std::string> strVec;

    for(int i = 0; i < argc ; ++i){
        strVec.push_back(std::string(argv[i]));}


    mkdir_func(strVec, argc);
    return 0;}




