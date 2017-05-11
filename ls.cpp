//
// Created by arsen on 16.03.17.
//

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/program_options.hpp>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <boost/algorithm/string.hpp>

using namespace boost::filesystem;
using namespace std;

bool in_array(const std::string &value, const std::vector<std::string> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}
bool find_mask(std::vector<std::string> *myargs, std::string &mask){
    for (std::string arg : *myargs){
        std::size_t found = arg.rfind('.');
        if (found!=std::string::npos) {
            mask = arg;
            return 1;
        }
    }
    return 0;
}

bool find_dir(std::vector<std::string> *myargs, std::string &dirAns){
    for (std::string arg : *myargs){
    DIR* dir = opendir(arg.c_str());
    if (dir)
    {
        closedir(dir);
        dirAns = arg;
        return 1;
    }
}
    return 0;
}


void parse_mask(std::string mask, std::string &final){
    std::vector<std::string> strs;
    boost::split(strs, mask, boost::is_any_of("."));
    for (int i = 0; i < mask.length();++i){
        if (mask[i] == '?'){
            if (final[final.size()-1]  != '.'){
                final = final + '.' + mask[i] +'.';
            }
            else{
                final = final + mask[i] + '.';
            }
        }
        else if (mask[i] == '*' ){
            if (final[final.size()-1]  != '.'){
                final = final + '.' + '*' + '.' + strs[1];
                break;
            }
            else{
                final = final + '*' + '.' + strs[1];
                break;
            }
        }
        else{
            if (mask[i] == '.' && final[final.size()-1] == '.'){
                continue;
            }
            else{
                final += mask[i];
            }}}}
void l_key(const char *filename) {
    struct stat attr;
    stat(filename, &attr);
    printf("%s %li %s", filename, attr.st_size, ctime(&attr.st_mtime));
}


std::vector<std::string> traverse_throught_dir(std::string dir_path, bool extended_attributes){
    const std::string target_path(dir_path);
    std::vector< std::string > all_matching_files;
    boost::filesystem::directory_iterator end_itr; // Default ctor yields past-the-end
    for( boost::filesystem::directory_iterator i( target_path ); i != end_itr; ++i) {
        // Skip if not a file
        if (!boost::filesystem::is_regular_file(i->status())) cout <<"/" << i->path().filename().c_str() << endl;
        if (!extended_attributes){
            std::cout << i->path().filename().c_str()<< std::endl;
        } else{
            l_key(i->path().filename().c_str());
        }
        all_matching_files.push_back(i->path().filename().c_str());
    }
    return all_matching_files;
}
std::vector<std::string> search_by_mask(std::string mask, std::string dir_path){
    std::string final = "";
    parse_mask(mask, final);
    const std::string target_path(dir_path);
    const boost::regex my_filter(final);
    std::vector< std::string > all_matching_files;
    boost::filesystem::directory_iterator end_itr; // Default ctor yields past-the-end

    for( boost::filesystem::directory_iterator i( target_path ); i != end_itr; ++i ) {
        // Skip if not a file
        if (!boost::filesystem::is_regular_file(i->status())) continue;
        boost::smatch what;
        if (!boost::regex_match(i->path().filename().string(), what, my_filter)) continue;
        // File matches, store it
        all_matching_files.push_back(i->path().filename().c_str());
        std::cout << i->path().filename().c_str()<< std::endl;
    }
    return all_matching_files;
}

int ls_func(std::vector<std::string> &myargs)
{
    //NТут вибиває помилку через те що myargs пустий
   // printf((const char *) myargs[1]);
    std::vector<std::string> var = {"-l", "-h", "--sort=U", "--sort=S ", "--sort=t", "--sort=X", "-r"};
    std::map<std::string,int> variants;
    std::string mask;
    std::string dir;
if (in_array("-h", myargs)){
    std::cout << "Found -h\n";
    return 1;
}
    else if (find_dir(&myargs, dir)){
    traverse_throught_dir(dir, false);
}
    else if (find_mask(&myargs, mask)){
    //Не впевнений чи так можна.
    std::vector<std::string> ans = search_by_mask(myargs[1], boost::filesystem::current_path().string());
//    for (std::string asd: ans){
//        l_key(asd.c_str());
//    }
}
    return 1;
}

//int unknownFunk(std::vector<std::string> argv){
//    DIR *thedirectory;
//    struct dirent *thefile;
//    struct stat thestat;
//    struct stat sb;
//    struct passwd *tf;
//    struct group *gf;
//    char buf[512];
//    cout << "Start: " << argv.at(1);
//
//    if (stat(argv.at(1), &sb) == 0 && S_ISDIR(sb.st_mode)){
//        thedirectory = opendir(argv[1]);
//    }
//
//    else if (strchr(argv[0], '\\')!= NULL){
//        cout << "There is no such directory. Check and try again.";
//        return 0;
//    }
//    cout << "Passed";
//    while((thefile = readdir(thedirectory)) != NULL) {
//        //We sprint "thedirectory/thefile" which defines the path to our file
//        if (strcmp(thefile->d_name,".") == 0 || strcmp(thefile->d_name, "..") == 0)
//            continue;
//        sprintf(buf, "%s/%s", argv[1], thefile->d_name);
//        stat(buf, &thestat);
//        switch (thestat.st_mode & S_IFMT) {
//            case S_IFBLK:  printf("b"); break;
//            case S_IFCHR:  printf("c"); break;
//            case S_IFDIR:  printf("d"); break; //It's a (sub)directory
//            case S_IFIFO:  printf("p"); break; //fifo
//            case S_IFLNK:  printf("l"); break; //Sym link
//            case S_IFSOCK: printf("s"); break;
//                //Filetype isn't identified
//            default:       printf("-"); break;
//        }
//        //[permissions]
//        printf( (thestat.st_mode & S_IRUSR) ? "r" : "-");
//        printf( (thestat.st_mode & S_IWUSR) ? "w" : "-");
//        printf( (thestat.st_mode & S_IXUSR) ? "x" : "-");
//        printf( (thestat.st_mode & S_IRGRP) ? "r" : "-");
//        printf( (thestat.st_mode & S_IWGRP) ? "w" : "-");
//        printf( (thestat.st_mode & S_IXGRP) ? "x" : "-");
//        printf( (thestat.st_mode & S_IROTH) ? "r" : "-");
//        printf( (thestat.st_mode & S_IWOTH) ? "w" : "-");
//        printf( (thestat.st_mode & S_IXOTH) ? "x" : "-");
//
//        // [number of hard links]
//        // Quoting: http://www.gnu.org/software/libc/manual/html_node/Attribute-Meanings.html
//        // "This count keeps track of how many directories have entries for this file.
//        // If the count is ever decremented to zero, then the file itself is discarded as soon as no process still holds it open."
//        // printf("\t%d ", thestat.st_nlink);
//
//
//        printf(" %s " , thefile->d_name);
//        printf("%zu",thestat.st_size);
//        printf(" %s", ctime(&thestat.st_mtime));
//    }
//    closedir(thedirectory);
//};
