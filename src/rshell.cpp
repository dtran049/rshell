#include <iostream>
#include <string>
#include <cstring>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <errno.h>
#include <cstdlib>
using namespace std;
using namespace boost;

#define tok tokenizer<char_separator<char> >
//typedef tokenizer <char_separator<char> > tok

//void bash(int agrc, char const *argv[])
void readcmd (const string &input);
void make (const string &input, const char sep[]);
void excbash(const string &input);
void piper(const string  &input);
int main()
{
    //cout << "error here";
    string input;
    //get userid and hostname
    char hostname[128];
	gethostname(hostname, sizeof(hostname));
    if (gethostname(hostname, sizeof(hostname)))
    {
        perror("gethostname() failed");
    }
    
	string loginID = getlogin();
    if (NULL == getlogin())
    {
        perror("getlogin() failed");
    }
	//cout << "Hostname: " << endl;
    //getline(cin, hostname);
	//cout << "Login ID: " << endl;
	//getline(cin,loginID);
	bool escape = false;
	//cout << "error here";
    while(!escape)
	{
		cout <<loginID << "@" << "hostname" << " $ ";
		getline(cin, input);
		trim(input);
		if(input == "exit")
		{
			exit(1);
		}
		else if (input.find("#") != string::npos)
		{
			input.resize(input.find("#"));
		}
		else
		{
			readcmd(input);
		}
    }
	return 0;
}
void readcmd(const string &input)
{
    char sepand[] = "&&";
    char sepsemi[] = ";";
    char sepor[]  = "||";
    char sepNULL[] = " "; 
        if(input.find("&&") != string::npos)
        {
            make(input,sepand);
        }
        else if(input.find("||") != string::npos)
        {
            make(input,sepor);
        }
        else if(input.find(";") != string::npos)
        {
            make(input,sepsemi);
        }
        else if(input.find(";") != string::npos)
        {
            make(input,sepNULL);
        }
        else if(input.find("|") != string::npos)
        {
            piper(input);
        }

}

void make(const string &input, const char sep[])
{
    char_separator<char> delim(sep);
    tok tokens(input, delim);

    for(tok::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        int pid = fork();
        if(pid == -1)
        {
            perror("fork failed");
            exit(1);
        }
        else if (pid == 0)
        {
            string command = *it;
            excbash(command);
        }
        else
        {
            if(wait(0) == -1)
            {
                perror("wait failed");
                exit(1);
            }
        }
    }
}

void excbash(const string &input)
{
    char *argv[100];
    int count = 0;
    char_separator<char> space(" ");
    
    tok tokens(input, space);
    for(tok::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        argv[count] = new char[(*it).size()];
        strcpy(argv[count], (*it).c_str());
        count++;
    }

    argv[count] = 0;
    if (execvp(argv[0], argv) == -1)
    {
        perror("execvp failed");
        exit(1);
    }
}

void piper(const string &input)
{
    int fd[2]
    int fd2[2]
    string a = input.substr(0, input.find("|"));
    string b = input.substr(input.find("|"));
    
    if(pipe(fd) == -1)
    {
        perror("piping failed");
        exit(1)
    }
    
    int pid = fork();

    if(pid == -1)
    {
        perror("forking failed");
        exit(1);
    }
    else if(pid == 0) 
    {
        int in = -1;
        if(a.find("<<<") == string:npos)
        {
            string file = l.substr(l.find("<")+1);
            l = l.substr(0, l.find("<"));
            in - openFile(file,O_RDONLY);
            if(in == -1)
            {
                exit(1);
            }
            else if (l.find("\"") != string::npos)
            {
                cont = input.substr(input.find("\"")+1);
                if (cont.find("\"") == string::npos)
                {
                    cout << "wrong usuage" << endl;
                    return;
                }
                cont = cont.substr(0,cont.find("\""));
                if(pipe(fd2) == -1)
                {
                    perror("piping failed");
                    exit(1);
                }
                char *buffer = new char[cont.size()];
                strcpy(buffer, cont.c_str());
                if(write(fd2[1], buffer, strlen(buffer)) == -1)
                {
                    perror("writing failed");
                    exit(1);
                 }
                if(close(fd2[1]) == -1)
                {
                    perror("closing failed");
                    exit(1);
                }
                in = fd2[0];
                delete[] buffer;
            }
            else
            {
                cout "error" << endl;
                exit(1);
            }
        }
        excIO(a, in, fd[1], -1);
    }
    else
    {
        if(wait(0) == -1)
        {
            perror("waiting failed");
            exit(1);
        }
        if(close(fd[1]) == -1)
        {
            perror("closing failed");
            exit(1);
        }
        int std_o = dup(0);
        if(std_o == -1)
        {
            perror("duping failed");
            exit(1);
        }
        if(b.find("|") != string::npos)
        {
            if(close(0) == -1)
            {
                perror("closing failed");
                exit(1);
            }
            if(dup(fd[0]) == -1)
            {
                perror("duping failed");
                exit(1);
            }
            piper(b);
        }
        else if (b.find(">") != string::npos)
        {
            bool two> = b.find(">>") != string::npos;
            out(b, two>, fd[0]);
            return;
        }
        else
        {
            int pid2 = fork();
            if(pid2 == -1)
            {
                perror("forking failed");
                exit(1);
            }
            if(pid2 == 0)
            {
                redirect(b, fd[0], -1, -1);
            }
            else
            {
                if(wait(0) == -1)
                {
                    perror("wait failed");
                    exit(1);
                }
            }
        }
        if(close(0) == -1)
        {
            perror("close failed");
            exit(1);
        }
        if(close(fd[0]) == -1)
        {
            perror("close failed");
            exit(1);
        }
        if(dup(std_o) == -1)
        {
            perror("dup failed");
            exit(1);
        }
    }

}






