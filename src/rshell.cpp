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
#include <sys/stat.h>
#include <sys/types.h>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <errno.h>
#include <cstdlib>
#include <fcntl.h>
#include <errno.h>
#include <pwd.h>
#include <dirent.h>
#include <signal.h>
#include <queue>
using namespace std;
using namespace boost;

#define SEPARATOR char_separator<char>
#define tok tokenizer<char_separator<char> >
//typedef tokenizer <char_separator<char> > tok

//void bash(int agrc, char const *argv[])
void readcmd (const string &input, char *currentdir);
void make (const string &input, const char sep[]);
void excbash(const string &input);
void piper(const string  &input);
int open_file(const string &file, int flags);
void doredir(const string &input, int std_in, int std_out, int std_err);
void output(const string &input, bool appended, int IN=-1);
void inp(const string &input, bool three);
void SIG_C(int signum);
void SIG_Z(int signum);
string makepath (string file);
void tilda(string &s);

const int SZ = 50;
pid_t CID = 0;
int pipefd[2];
queue<pid_t> IDHOLDER;
pid_t tempC = 0;
void HANDLE_C(int signum)
{
    if(CID != 0)
    {
        if(kill(CID, SIGINT) == -1)
        {
            perror("Control C failed");
        }
        CID= 0;
    }
}

void HANDLE_Z(int signum)
{
    if(CID != 0)
    {
       if( kill(CID, SIGTSTP) == -1)
       {
           perror("control Z failed");
       }
       IDHOLDER.push(CID);
       cout << "Program successfully stopped" << endl;
    }
}

void SIG_C ()
{
    struct sigaction SIGC;
    SIGC.sa_handler = HANDLE_C;
    SIGC.sa_flags = SA_RESTART;
    sigemptyset(&SIGC.sa_mask);

   if(0 >  sigaction(SIGINT, &SIGC, NULL))
   {
       perror("sigaction C failed");
   }
}

void SIG_Z ()
{
    struct sigaction SIGZ;
    SIGZ.sa_handler = HANDLE_Z;
    SIGZ.sa_flags = SA_RESTART;
    sigemptyset(&SIGZ.sa_mask);

    if(0 > sigaction(SIGTSTP, &SIGZ, NULL))
    {
        perror("sigaction Z failed");
    }
}




int main()
{
   // if(SIG_ERR == signal(SIGINT, SIG_C))
   // {
   //     perror("signal failed");
   //     exit(1);
   // }
   // if(SIG_ERR == signal(SIGTSTP, SIG_Z))
   // {
   //     perror("signal failed");
  //      exit(1);
  //  }
    SIG_Z();
    SIG_C();
    //cout << "error here";
    string input;
    //get userid and hostname
    char hostname[SZ];
    //struct passwd *pass = getpwuid(getuid());
	//int host = gethostname(hostname, sizeof(hostname));
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
    char curdir[BUFSIZ];
    if(getcwd(curdir, BUFSIZ) == NULL)
    {
        perror("getcwd failed");
        exit(1);
    }

    while(!escape)
	{
		cout <<loginID << "@" << "hostname" << curdir << " $ ";

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
			readcmd(input, curdir);
		}
    }
	return 0;
}
void readcmd(const string &input, char *currentdir)
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
        else if(input.find("<") != string::npos)
        {
               inp(input, false);
        }
        else if(input.find("<<<") != string::npos)
        {
               inp(input,true);
        }
        else if(input.find(">") != string::npos)
        {
               output(input,false);
        }
        else if(input.find(">>") != string::npos)
        {
               output(input,true);
        }
        else if(input.find("fg") != string::npos)
        {
            cout << CID << endl;
            if(!IDHOLDER.empty())
            {
                tempC = IDHOLDER.front();
                IDHOLDER.pop();
                //kill(CID, SIGTTIN);
                kill(tempC, SIGCONT);
            }
            else
            {
                cout << "No process to fg" << endl;
            }
        }
        else if(input.find("bg") != string::npos)
        {
            if(CID != 0)
            {
                //kill(CID, SIGTTIN);
                kill(CID, SIGCONT);
                CID = 0;
            }
            else
            {
                cout << "no process to bg" << endl;
            }
        }
        else
        {
            make(input,"");
        }
        if(getcwd(currentdir, BUFSIZ) == NULL)
        {
            perror("getcwd failed");
            exit(1);
        }
}

void make(const string &input, const char sep[])
{
    char_separator<char> delim(sep);
    tok tokens(input, delim);

    for(tok::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        if(-1 == pipe(pipefd))
        {
            perror("piping failed");
            exit(1);
        }
        int pid = fork();
        CID = pid;
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
            int status = 0;
            if(waitpid(pid, &status, WUNTRACED) == -1)
            {
                perror("wait failed");
                exit(1);
            }
            if(WIFEXITED(status) && WEXITSTATUS(status) == 5)
            {
                char result[BUFSIZ];
                memset(result,0,BUFSIZ);
                if(-1 == close(pipefd[1]))
                {
                    perror("closing failed");
                    exit(1);
                }
                if(-1 == read(pipefd[0], result, BUFSIZ))
                {
                    perror("read failed");
                    exit(1);
                }
                if(-1 == close(pipefd[0]))
                {
                    perror("close failed");
                    exit(1);
                }
                if(-1 == chdir(result))
                {
                    perror("chdir failed");
                    return;
                }
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
    tok::iterator it = tokens.begin();
    if(it != tokens.end())
    {
        if((*it) == "exit")
        {
            exit(10);
        }
        if((*it) == "cd")
        {
            it++;
            if(it == tokens.end())
            {
                string home = "~";
                tilda(home);
                if(-1 == write(pipefd[1], home.c_str(), home.size()))
                {
                    perror("write failed");
                    exit(1);
                }
                if( -1 == close(pipefd[0]))
                {
                    perror("close failed");
                    exit(1);
                }
            }
            else
            {
                string change = *it;
                if(change.at(0) == '~')
                {
                    tilda(change);
                }
                if(-1 == write(pipefd[1], change.c_str(), change.size()))
                {
                    perror("write failed");
                    exit(1);
                }
            }
            exit(5);
        }
    }
    for(; it != tokens.end();count++, it++)
    {
        argv[count] = new char[(*it).size()];
        strcpy(argv[count], (*it).c_str());
        //count++;
    }

    argv[count] = 0;

    string first = makepath(argv[0]);
    if (execvp(first.c_str(), argv) == -1)
    {
        perror(argv[0]);
        for(count = 0, it = tokens.begin(); it != tokens.end(); count++, it++)
        {
            delete[] argv[count];
        }
        exit(1);
    }
}

int open_file(const string &file, int flags)
{
    SEPARATOR sep(" ");
    tok token(file, sep);
    tok::iterator it = token.begin();
    if(it == token.end())
    {
        cout << "file does not exits" << endl;
        return -1;
    }
    int fd = open((*it).c_str(), flags, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if(fd == -1)
    {
        perror("open failed");
    }
    return fd;
}





void doredir(const string &input, int std_in, int std_out, int std_err)
{
	char *argv[SZ];
	SEPARATOR sep (" ");
	tok token(input,sep);
	tok::iterator it = token.begin();
	if(it == token.end())
	{
		cout << "no commands" << endl;
		exit(1);
	}
    int i = 0;
	for(; it != token.end(); ++it, i++)
	{
		argv[i] = new char[(*it).size()];
		strcpy(argv[i], (*it).c_str());
	}
	argv[i] = NULL;
	if(std_in >= 0)
	{
		if(close(0) == -1)
		{
			perror("close failed");
			exit(1);
		}
		if(dup(std_in) == -1)
		{
			perror("dup failed");
			exit(1);
		}
	}
    if(std_out >= 0)
	{
		if(close(1) == -1)
		{
			perror("close failed");
			exit(1);
		}
		if(dup(std_out) == -1)
		{
			perror("dup failed");
			exit(1);
		}
	}
	if(std_err >= 0)
	{
		if(close(2) == -1)
		{
			perror("close failed");
			exit(1);
		}
		if(dup(std_err) == -1)
		{
			perror("dup failed");
			exit(1);
		}
	}
    string path = makepath(argv[0]);
	execvp(path.c_str(), argv);
    perror(argv[0]);
	perror("execvp failed");
	for(i = 0, it = token.begin(); it != token.end(); ++it, i++)
	{
		delete[] argv[i];
	}
	exit(1);
}

void inp(const string &input, bool three)
{
	string x = "<";
	string cont;
	int OUT = -1;
	int IN = -1;
	int fd2[2];
	if(three)
	{
        x = "<<<";
	    if(input.find("\"") != string::npos)
	    {
		    cont = input.substr(input.find("\"")+1);
		    if(cont.find("\"") == string::npos)
		    {
			    cout << "error" << endl;
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
			    perror("write failed");
			    exit(1);
		    }
		    if(close(fd2[1]) == -1)
		    {
			    perror("closing failed");
			    exit(1);
		    }
		    IN = fd2[0];
		    delete[] buffer;
	    }
	    else
	    {
		    cout << "error" << endl;
		    return;
	    }
    }
	string a = input.substr(0,input.find(x));
	string b = input.substr(input.find(x) + x.size());
	string replaced = b;
	if(!three)
    {
		IN = open_file(replaced, O_RDONLY);
		if(IN == -1)
		{
			return;
		}
	}
	if(b.find(">") != string::npos)
	{
		replaced = replaced.substr(0,b.find(">"));
		int signal = O_RDWR|O_CREAT;
		int here = -1;
		if(b.find(">>") != string::npos)
		{
			here |= O_APPEND;
			signal = b.find(">>")+2;
		}
		else
		{
			signal = b.find(">") +1;
			signal |= O_TRUNC;
		}
		b = b.substr(signal);
		OUT = open_file(b, signal);
		if(OUT == -1)
		{
			return;
		}
	}
	int pid = fork();
	if(pid == -1)
	{
		perror("forking failed");
		exit(1);
	}
	else if (pid == 0)
	{
		doredir(a, IN, OUT, -1);
	}
	else
	{
		if(wait(0) == -1)
		{
			perror("wait failed");
			exit(1);
		}
		if(IN != -1)
		{
			if(close(IN) == -1)
			{
                perror("close failed");
				exit(1);
			}
		}
		if(OUT != -1)
		{
			if(close(OUT) == -1)
			{
				perror("close failed");
				exit(1);
			}
		}
	}
}

void output(const string &input, bool appended, int IN)
{
	string cmd;
	int signal = O_RDWR|O_CREAT;
	if(appended)
	{
		cmd = ">>";
		signal |= O_APPEND;
	}
	else
	{
		cmd = ">";
		signal |= O_TRUNC;
	}
	int here = input.find(cmd);
	if(input.find("2>") != string::npos)
	{
		--here;
	}
	if(input.find("1>") != string::npos)
	{
		--here;
	}
	string a = input.substr(0,here);
	string b = input.substr(input.find(cmd) + cmd.size());
	int fd = open_file(b, signal);
	if(fd == -1)
	{
		return;
	}
	int pid = fork();
	if(pid == -1)
	{
		perror("forking failed");
		exit(1);
	}
	else if (pid == 0)
	{
		if(input.find("2>") != string::npos)
		{
			doredir(a, IN, -1, fd);
		}
		else
		{
			doredir(a, IN, fd, -1);
		}
	}
	else
	{
		if(wait(0) == -1)
        {
			perror("wait failed");
			exit(1);
		}
		if(close(fd) == -1)
		{
			perror("close failed");
			exit(1);
		}
	}
}
void piper(const string &input)
{
    int fd[2];
    int fd2[2];
    string a = input.substr(0, input.find("|"));
    string b = input.substr(input.find("|")+1);
    
    if(pipe(fd) == -1)
    {
        perror("piping failed");
        exit(1);
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
        if(a.find("<") != string::npos)
        {
            string cont;
             if(a.find("<<<") == string::npos)
             {
                string file = a.substr(a.find("<")+1);
                a = a.substr(0, a.find("<"));
                in = open_file(file,O_RDONLY);
                if(in == -1)
                {
                    exit(1);
                }
             }
             else if (a.find("\"") != string::npos)
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
                cout << "error" << endl;
                exit(1);
             }
        }
        doredir(a, in, fd[1], -1);
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
            bool twoI = b.find(">>") != string::npos;
            output(b, twoI, fd[0]);
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
               doredir(b, fd[0], -1, -1);
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

string makepath (string file)
{
    char *path = getenv("PATH");
    if(path == NULL)
    {
        perror("getenv failed");
        exit(1);
    }
    string tmp = path;
    SEPARATOR sep(":");
    tok token(tmp, sep);
    tok::iterator it = token.begin();
    for(;it != token.end(); it++)
    {
        string tmp2 = *it + "/" + file;
        struct stat buf;
        if(stat(tmp2.c_str(), &buf))
        {
            continue;
            perror("stat failed");
        }
        else
        {
            return tmp2;
        }
    }
    return file;
}


void tilda(string &s)
{
    char *h = getenv("HOME");
    if(h == NULL)
    {
        perror("getenv failed");
        exit(1);
    }
    string home = h;
    home += "/";
    if(s.find("~/") == 0)
    {
        s.replace(0,2,home,0,home.size());
    }
    else if (s.find("~") == 0)
    {
        s.replace(0,1,home,0,home.size());
    }
}
