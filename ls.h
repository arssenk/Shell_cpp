//
// Created by arsen on 16.03.17.
//

#ifndef SHELL_CPP_LS_H
#define SHELL_CPP_LS_H

#include <boost/filesystem/path.hpp>
#include <boost/regex.hpp>
#include <string.h>
#include <vector>
int ls_func(std::vector<std::string> &vec);
std::vector<std::string> search_by_mask(std::string mask, std::string dir_path);
#endif //SHELL_CPP_LS_H
