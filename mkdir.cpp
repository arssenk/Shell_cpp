//
// Created by oksana on 17.03.17.
//
#include "mkdir.h"

#include <iostream>

using namespace std;
using namespace boost::filesystem;
namespace fs = boost::filesystem;
void mkdir_func(std::vector<std::string> strVEC, int argc)
{
    // checking if -h or --help are in input
    bool is_help = false;
    for (int i = 0; i < argc ; ++i){
        if (strVEC[i].compare("-h") == 0){
            is_help = true;}
        if (strVEC[i].compare("--help") == 0){
            is_help = true;}}

    if (is_help){
        std::cout << "Usage: mkdir2 [OPTION]..  . DIRECTORY...\n";
        std::cout << "Create the DIRECTORY(ies), if they do not already exist." <<"\n";
        return;}
    // taking current path
    fs::path name;
    // taking name of the new directory
    name = strVEC[1];
    fs::path full(fs::current_path());
    // checking if directory exists
    if(fs::is_directory(name)){
        std::cout << "Sorry, but directory is already exist." << "\n";

    }

    fs::path dir (full);
    fs::path file (name);
    fs::path full_path = dir / file;
    std::cout << full_path << std::endl;
    if(boost::filesystem::create_directory(full_path))
        std::cout << "Hello from our local shell, the directory has been successfully created. " << "\n";

}
