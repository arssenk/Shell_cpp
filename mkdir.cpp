//
// Created by oksana on 17.03.17.
//
#include "mkdir.h"
#include <boost/filesystem.hpp>
#include <iostream>

using namespace std;
using namespace boost::filesystem;
namespace fs = boost::filesystem;

void mkdir_func(fs::path p, fs::path name)
{
    fs::path dir (p);
    fs::path file (name);
    fs::path full_path = dir / file;
    std::cout << full_path << std::endl;


    if(boost::filesystem::create_directory(full_path))
        std::cout << "Success" << "\n";

}
