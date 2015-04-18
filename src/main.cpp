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
    int flag&& = 0;
    int flag|| = 0;
	char* commands = new char [input.length() + 1];
    strcpy(commands, input.c_str());
    for(int pos = 0; pos < strlen(commands); pos++)
    {
        if (commands[pos] == '&')
        {
            if (commands[pos + 1] == '&')
            {
                flag&& = 1;
            }
        }
        if(commands[pos] == '|')
        {
            if commands[pos + 1]
            {
                flag|| = 1;
            }
        }
    }
    
    char connector[] = ";||&&);
    	
}

//void bash(int agrc, char const*argv[])
//{
//
//}
