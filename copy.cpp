//
// Created by Maria Dobko
//

#include <cstdio>
#include <iostream>
#include <cstring>
#include <fstream>
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS
#include <typeinfo>
#include <cstring>
#include <array>
using namespace boost::filesystem;
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

char *lsh_read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0; // have getline allocate a buffer for us
    getline(&line, &bufsize, stdin);
    return line;
}
#define LSH_TOK_BUFSIZE 256
#define LSH_TOK_DELIM " \t\r\n\a"
char **lsh_split_line(char *line)
{
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = (char **) malloc(bufsize * sizeof(char*));
    char *token, **tokens_backup;

    if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }
    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;


        if (position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens_backup = tokens;
            tokens = (char **) realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                free(tokens_backup);
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}



std::string addTwoStrings(const std::string& a, const std::string& b)
{
    return a + b; // works because they are both strings.
}

int main(int argc, char **argv)
{
    printf("> ");
    char *line = lsh_read_line();
    char **args = lsh_split_line(line);
    char *help = "-h";
    char *noPermission = "-f";
    char *helpInfo =  "This is copy function. \n"
            "cp [-h|--help] [-f] <file1> <file2> (copy file1 to file2) \n"
            "cp [-h|--help] [-f]  <file1> <file2> <file3>... <dir> (copy files into directory) \n"
            "-f means copy without asking permission for overwriting";
    int size;
    for (size = 0; args[size] != NULL; size++);

    int check = strcmp (args[1], noPermission);

    int ifTrue = strcmp (args[1], help);
    if (ifTrue == 0){
        cout << helpInfo << endl;
    }
    else if (check == 0) {
        if (size == 4) {
            copy_file(args[2], args[3]);
        }
        else if (size > 4) {
            // file1 file2 file 3 to dir
            for (int a = 2; a < size - 1; a++) {
                char *nameFile = '/file' + (char *) a - 1 + '.pdf';
                string dir = addTwoStrings(args[size], nameFile);
                copy_file(args[a], dir);
            }
            cout << "Entered!";
        }
    }
    else {
        if (size == 3) {
            //ask permission copy file1 to file 2


            cout << "Do you want to rewrite file?[Y/N] ";
            char *lineNew = lsh_read_line();
            if (lineNew[0] == 'Y') {
                remove(args[2]);
                copy_file(args[1], args[2]);
            }
        } else if (size > 3) {
            //ask permission copy files to dir

        }
    }

    return 1;
}
