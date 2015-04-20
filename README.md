# rshell
A simple shell program that can take in bash commands and execute them correctly.

# How to run rshell
```
git clone https://github.com/dtran049/rshell.git
cd rshell
git checkout hw0
make
bin/rshell
```

# Known bugs
1. Different connectors used in the same command will give an error.
2. There will be an error message when running commands with flag during script.
3. Commands with # will be treated as comments regardless of where the # is placed.
4. `"` and `'` will be shown in  echo
5. echo command will echo connectors sometimes
6. `||` and `&&` will give bad logic 
7. tabs will not count as white space

