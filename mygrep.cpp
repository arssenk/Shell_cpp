//
// Created by arsen on 04.06.17.
//
#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>


using namespace std;


bool in_array(const std::string &value, const std::vector<std::string> &array) {
    return std::find(array.begin(), array.end(), value) != array.end();
}

std::string help_describe() {
    return "grep [-h|--help] [-v|--invert-match] [-i|--ignore-case] [--file=<filename>] string";
}

void grep(std::string stringToMatch, std::istream &in, int &v, int &i) // just one file
{
    // search file for stringToMatch
    // print line containing stringToMatch
    std::string line;
    while (std::getline(in, line))
    {
        std::istringstream iss(line);
        if (v) {
            if (i){
                if (!boost::iequals(iss.str(), stringToMatch)) cout << "Found: " << iss.str() << endl;} else{
            if (!iss.str().find(stringToMatch) != std::string::npos) cout << "Found: " << iss.str() << endl;
            }
        } else{
            if (i){
                if (boost::iequals(iss.str(), stringToMatch)) cout << "Found: " << iss.str() << endl;
        } else{
            if (iss.str().find(stringToMatch) != std::string::npos) {
                cout << "Found: " << iss.str() << endl;
            }}
        }
    }
}
void removeCharsFromString( string &str, char* charsToRemove ) {
    for ( unsigned int i = 0; i < strlen(charsToRemove); ++i ) {
        str.erase( remove(str.begin(), str.end(), charsToRemove[i]), str.end() );
    }
}

string findFileName(vector<string> &input){
    int i = 0;
    for (string name: input){
       if (name.find("--file=") != std::string::npos){
           if (name.size()  > (name.find("="))) {
               cout << "ASDASD: " << name.find("=") << endl;
               input.erase(input.begin() + i);
                removeCharsFromString(name, (char *) "\"");
               return name.substr(name.find("=")+1, name.size());
           }
           else{
               cout << "No filename were found after --file=" << endl;
           }
        }
        else{
           cout << "No filename were found. " << endl;
       }
        ++i;
    }
    return "";
}
string getString(vector<string> input){
    string fullLine = "";
    for (string str: input){
        fullLine = fullLine + str + " ";
    }
    return fullLine.substr(fullLine.find_first_of("\"") + 1, fullLine.find_last_of("\"")-1);
}
void parseInput(vector<string> input) {
    if (in_array("-h", input) || in_array("--help", input)) {
        input.erase(std::remove(input.begin(), input.end(), "-"), input.end());
        cout << help_describe().c_str() << endl;
    }
    int v, i = 0;
    if(in_array("-v", input) || in_array("--invert-match", input)){
        v = 1;
        cout << "v or invert found" << endl;
    }

    if(in_array("-i", input) || in_array("--ignore-case", input)){
        i = 1;
        cout << "i or ignore case found" << endl;
    }
    string filename = findFileName(input);
    string str = getString(input);


    cout << "filename: " << filename << endl;
    cout << "string to match: " << str << endl;


    if (filename.compare("")){
        std::ifstream file(filename.c_str());
        grep(str, file, v, i);
    }
    else{
        grep(str, std::cin, v, i);
    }
        cout << "Nothing found";

}
//TODO: стирати флажки коли на них попадаєш. Тести!!