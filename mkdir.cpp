//
// Created by oksana on 17.03.17.
//
#include "mkdir.h"

#include <iostream>
#include <map>


using namespace std;
using namespace boost::filesystem;
namespace fs = boost::filesystem;

std::string help_describe() {
    std::string answer = "Usage: mkdir [OPTION]... DIRECTORY...\n"
            "Create the DIRECTORY(ies), if they do not already exist.\n"
            "--help, -h     display this help and exit";
    return answer;
}
std::string missing_operand(){
    std::string answer = "mkdir: missing operand\n"
            "Try 'mkdir --help' for more information";
    return answer;
}
bool in_array(const std::string &value, const std::vector<std::string> &array) {
    return std::find(array.begin(), array.end(), value) != array.end();
}
int mkdir_func(std::vector<std::string> myargs, int argc)
{
    // checking if -h or --help are in input

    std::vector<std::string> var = {"-h",  "-help"};
    std::map<std::string, int> variant;
    if (in_array("-h", myargs) || in_array("--help", myargs)) {
        printf(help_describe().c_str());
        return 1;
    }
    if (myargs.size() == 0) {
        printf(missing_operand().c_str());
        return 1;
    }


    // taking current path
    fs::path name;
    // taking name of the new directory
    name = myargs[1];
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
    return 1;
}
