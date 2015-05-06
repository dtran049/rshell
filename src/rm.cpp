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

bool exists(string file)
{
    struct stat s;
    int success = stat(file.c_str(), &s);
    if (success == -1)
    {
        cerr << "error on " << file << endl;
        perror("stat:" S__LINE__);
        return false;
    }

    return true;
}

void get_files(string d, vector<string> &files)
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
}


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

int main(int argc, char **argv)
{
    vector<string> files;
    
    bool del_dir = false;
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-r") == 0)
        {
            del_dir = true;
        }
        else if (exists(argv[i]))
        {
            files.push_back(argv[i]);
        }
    }

    for (auto const &f : files)
    {
        if (is_dir(f))
        {
            if (del_dir)
            {
                recurse(f);
            }
            else
            {
                cerr << "cannot delete " << f << ": is a directory" << endl;
            }
        }
        else
        {
            if (-1 == unlink(f.c_str()))
            {
                perror("unlink");
            }
        }
    }

}
