//
// Created by arsen on 16.03.17.
//

#include <stdio.h>
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/program_options.hpp>
#include <sys/stat.h>
#include <dirent.h>
#include <boost/algorithm/string.hpp>


struct Pair {
    std::string name;
    long int size;
    time_t timestamp;
};

Pair make_Pair(const std::string &name, const long int &size, time_t &timestamp) {
    Pair ret;
    ret.name = name;
    ret.size = size;
    ret.timestamp = timestamp;
    return ret;
}

const char *findExt(const char *filename) {
    const char *e = strrchr(filename, '.');
    if (e == NULL)
        e = ""; // fast method, could also use &(fspec[strlen(fspec)]).
    return e;
}

bool sort_by_size(Pair one, Pair two) {
    return (one.size >= two.size);
}

bool sort_by_date(Pair one, Pair two) {
    return ((one.timestamp - two.timestamp) >= 0);
}


bool sort_by_extn(Pair one, Pair two) {
    return strcmp(findExt(one.name.c_str()), findExt(two.name.c_str())) <= 0;
}

bool in_array(const std::string &value, const std::vector<std::string> &array) {
    return std::find(array.begin(), array.end(), value) != array.end();
}


bool find_mask(std::vector<std::string> *myargs, std::string &mask) {
    for (std::string arg : *myargs) {
        std::size_t found = arg.rfind('.');
        if (found != std::string::npos) {
            mask = arg;
            return 1;
        }
    }
    return 0;
}

bool find_dir(std::vector<std::string> *myargs, std::string &dirAns) {
    for (std::string arg : *myargs) {
        DIR *dir = opendir(arg.c_str());
        if (dir) {
            closedir(dir);
            dirAns = arg;
            return 1;
        }
    }
    return 0;
}


void parse_mask(std::string mask, std::string &final) {
    std::vector<std::string> strs;
    boost::split(strs, mask, boost::is_any_of("."));
    for (int i = 0; i < mask.length(); ++i) {
        if (mask[i] == '?') {
            if (final[final.size() - 1] != '.') {
                final = final + '.' + mask[i] + '.';
            } else {
                final = final + mask[i] + '.';
            }
        } else if (mask[i] == '*') {
            if (final[final.size() - 1] != '.') {
                final = final + '.' + '*' + '.' + strs[1];
                break;
            } else {
                final = final + '*' + '.' + strs[1];
                break;
            }
        } else {
            if (mask[i] == '.' && final[final.size() - 1] == '.') {
                continue;
            } else {
                final += mask[i];
            }
        }
    }
}

Pair l_key(const char *filename, bool print_name = false, bool print_choice = false) {
    struct stat attr;
    stat(filename, &attr);
    if (print_name) printf("%s\n", filename);
    if (print_choice) printf("%s %li %s\n", filename, attr.st_size, ctime(&attr.st_mtime));
    return make_Pair(filename, attr.st_size, attr.st_mtime);
}


std::vector<std::string> traverse_throught_dir(std::string dir_path, bool print_choice = false) {
    const std::string target_path(dir_path);
    std::vector<std::string> all_matching_files;
    boost::filesystem::directory_iterator end_itr; // Default ctor yields past-the-end
    for (boost::filesystem::directory_iterator i(target_path); i != end_itr; ++i) {
        // Skip if not a file
        if (!boost::filesystem::is_regular_file(i->status())) {
            if (print_choice) std::cout << "/" << i->path().filename().c_str() << std::endl;
            all_matching_files.push_back(("/" + i->path().filename().string()).c_str());
            continue;
        }
        if (print_choice) std::cout << i->path().filename().c_str() << std::endl;
        all_matching_files.push_back(i->path().filename().c_str());
    }
    return all_matching_files;
}

std::vector<std::string> search_by_mask(std::string mask, std::string dir_path, bool print_choice = false) {
    std::string final = "";
    parse_mask(mask, final);
    const std::string target_path(dir_path);
    const boost::regex my_filter(final);
    std::vector<std::string> all_matching_files;
    boost::filesystem::directory_iterator end_itr; // Default ctor yields past-the-end

    for (boost::filesystem::directory_iterator i(target_path); i != end_itr; ++i) {
        // Skip if not a file
        if (!boost::filesystem::is_regular_file(i->status())) continue;
        boost::smatch what;
        if (!boost::regex_match(i->path().filename().string(), what, my_filter)) continue;
        // File matches, store it
        all_matching_files.push_back(i->path().filename().c_str());
        if (print_choice) std::cout << i->path().filename().c_str() << std::endl;
    }
    return all_matching_files;
}

std::string help_describe() {
    std::string answer = "ls [path|mask] [-l] [-h|--help] [--sort=U|S|t|X] [-r]\n"
            "-l - information about file(name, size, modification date)\n"
            "--sort: \nU - don't sort\nS - by size\nt - modification date\nX - extension \nN - name(by default)\n-r - reversed sort";
    return answer;
}

std::vector<Pair> create_vector_of_pairs(std::vector<std::string> args) {
    std::vector<Pair> PairID;
    for (std::string fileName : args) {
        PairID.push_back(l_key(fileName.c_str()));
    }
    return PairID;
}


int ls_func(std::vector<std::string> &myargs) {
    //NТут вибиває помилку через те що myargs пустий
    // printf((const char *) myargs[1]);
    std::vector<std::string> var = {"-l", "-h", "--sort=U", "--sort=S ", "--sort=t", "--sort=X", "-r"};
    std::map<std::string, int> variants;
    std::string mask;
    std::string dir;
    std::vector<std::string> ans;
    std::vector<Pair> PairID;
    if (in_array("-h", myargs) || in_array("--help", myargs)) {
        printf(help_describe().c_str());
        return 1;
    }
    if (myargs.size() == 1) {
        traverse_throught_dir(boost::filesystem::current_path().c_str(), true);
        return 1;
    }
    if (find_dir(&myargs, dir)) {
        ans = traverse_throught_dir(dir);
    } else if (find_mask(&myargs, mask)) {
        //Не впевнений чи так можна.
        ans = search_by_mask(myargs[1], boost::filesystem::current_path().string());
    }
    if (in_array("--sort=U", myargs)) {
        if (ans.size() == 0) {
            ans = traverse_throught_dir(boost::filesystem::current_path().c_str());
        }
    } else if (in_array("--sort=N", myargs) || in_array("--sort", myargs)) {
        if (ans.size() == 0) {
            ans = traverse_throught_dir(boost::filesystem::current_path().c_str(), false);
        }
        sort(ans.begin(), ans.end());
    } else if (in_array("--sort=S", myargs)) {
        if (ans.size() == 0) {
            printf("Found sort -S with ans.size = 0");
            ans = traverse_throught_dir(boost::filesystem::current_path().c_str(), false);
        }
        PairID = create_vector_of_pairs(ans);
        sort(PairID.begin(), PairID.end(), sort_by_size);
    } else if (in_array("--sort=t", myargs)) {
        if (ans.size() == 0) {
            printf("Found sort -t with ans.size = 0");
            ans = traverse_throught_dir(boost::filesystem::current_path().c_str());
        }
        PairID = create_vector_of_pairs(ans);
        sort(PairID.begin(), PairID.end(), sort_by_date);
    } else if (in_array("--sort=X", myargs)) {
        if (ans.size() == 0) {
            printf("Found sort -X with ans.size = 0");
            ans = traverse_throught_dir(boost::filesystem::current_path().c_str());
        }
        PairID = create_vector_of_pairs(ans);
        sort(PairID.begin(), PairID.end(), sort_by_extn);
    }
    if (in_array("-r", myargs)) {
        if (PairID.size() == 0 && ans.size() == 0) {
            ans = traverse_throught_dir(boost::filesystem::current_path().c_str());
            PairID = create_vector_of_pairs(ans);
        } else if (PairID.size() == 0 && ans.size() != 0) {
            reverse(ans.begin(), ans.end());
        }
        reverse(PairID.begin(), PairID.end());
    }
    bool present_l = in_array("-l", myargs);

    if (ans.size() == 0 && PairID.size() == 0) {
        ans = traverse_throught_dir(boost::filesystem::current_path().c_str());
        for (std::string filename: ans) {
            l_key(filename.c_str(), !present_l, present_l);
        }
    } else if (PairID.size() != 0) {
        for (Pair filename: PairID) {
            l_key(filename.name.c_str(), !present_l, present_l);
        }
    } else {
        for (std::string filename: ans) {
            l_key(filename.c_str(), !present_l, present_l);
        }
    }

    return 1;
}

