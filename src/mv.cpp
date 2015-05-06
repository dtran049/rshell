#include <unistd.h>
#include<fcntl.h>
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)

using namespace std;

bool is_dir(string file)
{
    struct stat s;
    int success = stat(file.c_str(),  &s);
    if (success == -1)
    {
        perror("stat:" S__LINE__);
    }

    return S_ISDIR(s.st_mode);
}

bool exists(string file, bool quiet = false)
{
    struct stat s;
    int success = stat(file.c_str(), &s);
    if (success == -1)
    {
        if (!quiet)
        {
            cerr << "error on " << file << endl;
            perror("stat:" S__LINE__);
        }
        return false;
    }

    return true;
}

/*void get_files(string d, vector<string> &files)
{
    DIR *dir = opendir(d.c_str());
    dirent prev;
    dirent *curr = nullptr;
    int success = 0;
    while ((success = readdir_r(dir, &prev, &curr)) == 0)
    {
        if (curr == nullptr)
        {
            break;
        }
        if (strcmp(curr->d_name,".") != 0 && strcmp(curr->d_name, "..") != 0)
        {
            files.push_back(curr->d_name); 
        }
    }

    if (success > 0)
    {
        cerr << "readdir_error" << endl;
    }
}*/

/*
void recurse(string d)
{
    if (is_dir(d))
    {
        vector<string> files;
        get_files(d, files);
        for (auto f : files)
        {
            if (d.at(d.size() - 1) == '/')
            {
                recurse(d + f);
            }
            else
            {
                recurse(d + "/" + f);
            }
        }
        if (rmdir(d.c_str()) == -1)
        {
            perror("rmdir");      
        }
        
    }
    else
    {
        if (unlink(d.c_str()) == -1)
        {
            perror("unlink");
        }
    }
}
*/

void mv(string file1, string file2)
{
    if (link(file1.c_str(), file2.c_str()) == -1)
    {
        perror("link");
        exit(1);
    }
    if (unlink(file1.c_str()))
    {
        perror("unlink");
        exit(1);
    }
}

int main(int argc, char **argv)
{
    string file1 = argv[1];
    string file2 = argv[2];

    if (!exists(file1))
    {
        return 1;
    }

    if (exists(file2, true))
    {
        if (is_dir(file2))
        {
            //do things
            if (file2.at(file2.size() - 1) == '/')
            {
                mv(file1, file2 + file1);
            }
            else
            {
                mv(file1, file2 + "/" + file1);
            }
 
        }
        else
        {
            cout << "error:" << file2 << " exists but not a directory" << endl;
            return 1;
        }
    }
    else
    {
        //do things
        mv(file1, file2);
    }
    return 0;
}

