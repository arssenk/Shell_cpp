//
// Created by bohdan on 6/2/17.
//
#include <iostream>
#include <vector>
#include "rm.h"
#include <boost/filesystem.hpp>

using namespace std;

int main(int argc, char **argv) {
    int args_size = 0;
    while(argv[args_size] != NULL) {
        args_size += 1;
    }
    bool is_R = false;
    bool is_f = false;
    bool is_help = false;

    vector<string> tokens(argv, argv + args_size);
    if(args_size == 1)
        return deleter(tokens, true, is_R, is_f);


    if(find(tokens.begin(), tokens.end(), "-f") != tokens.end()) {
        is_f = true;
        tokens.erase(remove(tokens.begin(), tokens.end(),"-f")), tokens.end();
    }
    if(find(tokens.begin(), tokens.end(), "-R") != tokens.end()) {
        is_R = true;
        tokens.erase(remove(tokens.begin(), tokens.end(),"-R")), tokens.end();
    }
    if(find(tokens.begin(), tokens.end(), "-h") != tokens.end()) {
        is_help = true;
        tokens.erase(remove(tokens.begin(), tokens.end(),"-h")), tokens.end();
    } else if(find(tokens.begin(), tokens.end(), "--help") != tokens.end()) {
        is_help = true;
        tokens.erase(remove(tokens.begin(), tokens.end(),"--help")), tokens.end();
    }

    return deleter(tokens, is_help, is_R, is_f);
}