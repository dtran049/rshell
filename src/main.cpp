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

using namespace std;
using namespace boost;

typedef tokenizer <char_separator<char> > tok

//void bash(int agrc, char const *argv[])
void readcmd (const string input)
void make (const string input, const char sep[])
void excbash(const string input)
int main()
{
    string input;
    //get userid and hostname
	string hostname = gethostname();
	string loginID = getlogin();
	//cout << "Hostname: " << endl;
	//getline(cin, hostname);
	//cout << "Login ID: " << endl;
	//getline(cin,loginID);
	bool exit = true;
	while(!exit)
	{
		cout << loginID << "@" << hostname << " $ ";
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

	return 0;
}
void readcmd(const string input)
{
    char sep& = "&&";
    char sep; == ";";
    char sep|| == "||";
    
        if(input.find("&&") != string::npos)
        {
        excbash(input,sep&&);
        }
        else if(input.find("||") != string::npos)
        {
            excbash(input,sep||);
        }
        else if(input.find(";") != string::npos)
        {
            excbash(input,sep;);
        }
}

void make(const string input, const char sep[])
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

void excbash(const string input)
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





//void readcmd(const string input)
//{
//    int flag&& = 0;
//    int flag|| = 0;
//	char* commands = new char [input.length() + 1];
//    strcpy(commands, input.c_str());
//    for(int pos = 0; pos < strlen(commands); pos++)
//    {
//        if (commands[pos] == '&')
//        {
//           if (commands[pos + 1] == '&')
//            {
//                flag&& = 1;
//            }
//        }
//        if(commands[pos] == '|')
      //  {
    //        if commands[pos + 1]
    //            {
  //              flag|| = 1;
//           }
//        }
//    }
//    
//    char connector[] = ";||&&);
//    	
//}

//void bash(int agrc, char const*argv[])
//{
//
//}
