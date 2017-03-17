//
// Created by arsen on 16.03.17.
//
#include "ls.h"
#include <boost/filesystem.hpp>
int main(int argc, char **argv)
{
    boost::filesystem::path full_path(boost::filesystem::current_path() );
    ls_func(full_path);
    return 0;
}
