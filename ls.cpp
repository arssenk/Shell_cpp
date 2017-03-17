//
// Created by arsen on 16.03.17.
//


#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;

int ls_func(boost::filesystem::path argv)
{

    path p (argv);   // p reads clearer than argv[1] in the following code

    try
    {
        if (exists(p))    // does p actually exist?
        {
            if (is_regular_file(p))        // is p a regular file?
                cout << p << " size is " << file_size(p) << '\n';

            else if (is_directory(p))      // is p a directory?
            {
                cout << p << " is a directory containing:\n";

                typedef vector<path> vec;             // store paths,
                vec v;                                // so we can sort them later

                copy(directory_iterator(p), directory_iterator(), back_inserter(v));

                sort(v.begin(), v.end());             // sort, since directory iteration
                // is not ordered on some file systems

                for (vec::const_iterator it(v.begin()), it_end(v.end()); it != it_end; ++it)
                {
                    path fn = it->filename();// extract the filename from the path
                    cout << fn << "\n";
                    v.push_back(fn);
                }
            }
            else
                cout << p << " exists, but is neither a regular file nor a directory\n";
        }
        else
            cout << p << " does not exist\n";
    }

    catch (const filesystem_error& ex)
    {
        cout << ex.what() << '\n';
    }

    return 0;
}