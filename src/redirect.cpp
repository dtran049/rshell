void doredir(const string &input, int std_in, int std_out, int std_err)
{
	char * argv[SZ];
	SEP sep (" ");
	tok token(input,sep);
	tok::iterator it = tok.begin();
	if(it == tok.end())
	{
		cout << "no commands" << endl;
		exit(1);
	}
	for(int i = 0; it != tok.end(); it++, i++)
	{
		argv[i] = new char[(*it).size()];
		strcpy(argv[i], (*it).c_str();
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
		if(close(0) == -1)
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
		if(close(0) == -1)
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
	execvp(argv[0], argv);
	perror("execvp failed");
	for(int i = 0, it = tok.begin(); it != tok.end(); it++, i++)
	{
		delete[] argv[i];
	}
	exit(1);
}

void in<(const string &input, bool three)
{
	string x = "<";
	string cont;
	int OUT = -1;
	int IN = -1;
	int fd2[2];
	if(three)
	{
	if(input.find("\"") != string::npos)
	{
		x = "<<<";
		cont = input.substr(input.find("\"")+1);
		if(cont.find("\"") == string::npos)
		{
			cout << "error" << endl;
			return;
		}
		cont = cont.substr(0,more.find("\""));
		if(pipe(fd2) == -1)
		{
			perror("piping failed");
			exit(1);
		}
		char *buffer = new char[cont.size()];
		strcpy(buffer, cont.c_str());
		if(write(fd2[1], buffer, strlen(buffer) == -1)
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
	string replaced = c;
	if(!three)
	{
		IN = openFile(replaced, O_RDONLY);
		if(IN == -1)
		{
			return;
		}
	}
	if(c.find(">") != string::npos)
	{
		c = c.substr(0,r.find(">"));
		int signal = O_RDWR | O_CREAT;
		int here = -1;
		if(c.find(">>") != string::npos)
		{
			here |= O_APPEND;
			signal = c.find(">>")+2;
		}
		else
		{
			signal = c.find(">") +1;
			signal |= O_TRUNC;
		}
		c = c.substr(signal);
		OUT = openFile(c, signal);
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
		redirect(a, IN, OUT, -1);
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
	string cmd
	int signal = O_RDWR | O_CREAT;
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
		here--;
	}
	if(input.find("1>") != string::npos)
	{
		here--;
	}
	string a = input.substr(0,here);
	string b = input.substr(input.find(cmd) + cmd.size());
	int fd = openFile(b, signal);
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
			redirect(a, IN, -1, fd);
		}
		else
		{
			redirect(a, IN, fd, -1);
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

	

