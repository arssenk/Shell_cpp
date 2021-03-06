// new commit
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BOOST_NO_CXX11_SCOPED_ENUMS

#include <boost/filesystem.hpp>

#undef BOOST_NO_CXX11_SCOPED_ENUMS

#include "ls.h"
#include <iostream>

using namespace std;
using namespace boost::filesystem;
namespace fs = boost::filesystem;

/*
  Function Declarations for builtin shell commands:
 */
int lsh_cd(char **args);

int lsh_exit(char **args);

int lsh_pwd(char **args);

int global_int;
/*
  List of builtin commands, followed by their corresponding functions.
 */
const char *builtin_str[] = {
        "cd",
        "exit",
        "pwd"
};


const char *additional_str[] = {
        "mkdir",
        "ls",
        "cp",
        "mv",
        "rm",
        "ls_func"
};
using additional_func_t = void (*)(fs::path p, fs::path name);

additional_func_t additional_funcs[] = {
        // &mkdir_func
};


int (*builtin_func[])(char **) = {
        &lsh_cd,
        &lsh_exit,
        &lsh_pwd

};


int lsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/**Bultin command: shows the current direcory
 * List of args, where agrs[0] is "pwd"
 */

int lsh_pwd(char **args) {
    boost::filesystem::path full_path(boost::filesystem::current_path());
    string pwd = full_path.string();
    std::cout << pwd << std::endl;
    return 1;
}

/**
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */
int lsh_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("lsh");
        }
    }
    return 1;
}

/**
   @brief Builtin command: print help.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
 */


/**
   @brief Builtin command: exit.
   @param args List of args.  Not examined.
   @return Always returns 0, to terminate execution.
 */
int lsh_exit(char **args) {
    if (args[1]) global_int = atoi(args[1]);
    return 0;
}

/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int lsh_launch(char **args) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp((std::string(args[0])).c_str(), args) == -1) {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("lsh");
    } else {
        // Parent process
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int lsh_execute(char **args) {
    int i;

    if (args[0] == NULL) {
        // An empty command was entered.
        return 1;
    }



    for (i = 0; i < lsh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }


    }

//    if (strcmp((const char *) args[0], "ls_cpp") == 0) {
//        return lsh_launch(args);
//    }
//    if (strcmp((const char *) args[0], "m") == 0) {
//        execvp("./mkdir_cpp", args);
//        return 1;
//    }
    return lsh_launch(args);
}


/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *lsh_read_line(void) {
    char *line = NULL;
    size_t bufsize = 0; // have getline allocate a buffer for us
    getline(&line, &bufsize, stdin);
    return line;
}
//
char* splitbydel(char *args){

    char* token;
    if(args != NULL){
        if ((token = strsep(&args, "/#")) != NULL)
        {

            return token;
        }
    }
    return args;
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
//What do you mean(header)?
char **lsh_split_line(char *line) {
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = (char **) malloc(bufsize * sizeof(char *));
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
            tokens = (char **) realloc(tokens, bufsize * sizeof(char *));
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

/**
   @brief Loop getting input and executing it.
 */
int lsh_loop(void) {
    char *line;
    char **args;
    int status;

    do {
        printf("> ");
        line = lsh_read_line();
        splitbydel(line);
        args = lsh_split_line(line);

        status = lsh_execute(args);
        free(line);
        free(args);
    } while (status);
    return status;
}


void addCurrDirToPass(){
    char* pPath;
    pPath = getenv ("PATH");
    char* ret = new char[strlen(pPath)+2];
    strcpy(ret, pPath);
    char dir[256];
    string s;
    getcwd(dir, 250);
    s = ret;
    s.append(":");
    s.append(dir);
    s.append("/execs/");
    setenv("PATH", s.c_str(), 1);
}
/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv) {
    addCurrDirToPass();
     // Run command loop.
    lsh_loop();
    //ls_func(argc, argv);
    // Perform any shutdown/cleanup.
    if (global_int) exit(global_int);
    return EXIT_SUCCESS;
}