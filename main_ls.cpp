//
// Created by arsen on 16.03.17.
//
#include <map>
#include "ls.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>

namespace
{
    const size_t ERROR_IN_COMMAND_LINE = 1;
    const size_t SUCCESS = 0;
    const size_t ERROR_UNHANDLED_EXCEPTION = 2;

}

int main(int argc,  char **argv)
{
    std::vector<std::string> vec;
    for (int i = 0; i < argc; ++i) {
        std::string str(argv[i]);
        vec.push_back(str);
    }
    ls_func(vec);

    //search_by_mask(boost::filesystem::current_path().c_str(), argv[1]);
//    printf(argv[1]);
//    printf("Using ls main");
//    boost::filesystem::path full_path;
//    if (boost::filesystem::is_directory(argv[1]))
//    {
//        printf("Changing path");
//
//    }
//    else{
//        printf("Current path");
//        boost::filesystem::path full_path(boost::filesystem::current_path());
//
//    }
//
//   //ls_func(full_path);
//
//
//        char **i = argv; //iterator of argv
//    char **e = argv + argc; //end of argv
//
//    for(; i != e; ++i) {
//
//        char *j = *i; //iterator of *i
//
//        //condition: *j mean *j != 0
//        for(; *j; ++j) {
//            printf("%c", *j);
//
//        }
//
//        printf("\n");
//
//    }
//    printf("END!");
    return 1;
}
