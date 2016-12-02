# Systems-Shell
Systems Project by Brian Yang, Niels Graham, and Yikai Wang. Shell program.

Features:
Forks and Executes multiple commands separated by semicolons
	Can run CD and Exit commands
	Can run the shell inside the shell creating a shell-ception
	Trims the whitespace from commands
	Is able to handle error inputs and gives back error statements
  Returns information as to what and the number of commands put in


Attempted
	Tried to stop control C form exiting from the shell. Instead it was supposed to create a
new bash
Allow the shell to run cd when the file was separated by multiple spaces  


Bugs
	Cannot handle CD when separated by multiple spaces


Files & Function Headers:
cd.c
Handles the cd terminal command

####void print_working_dir()
	Inputs: None
	Returns: current working directory


####void chdir_with_tilde()
	Inputs: New working directory with a tilde
	Returns: Nothing


	If a correct directory is give the program changes the
	current working directory to *dir


####int cd()
	Inputs: New working directory
	Returns: Nothing


	If a correct directory is give the program changes the
	current working directory to *dir


Redirect Functions:
Tests to see if there is a redirect symbol and if so send the
arguments to their proper place

####char chkrdrect()
	Inputs: Arguments
	Returns: Nothing


	Checks for specific redirect symbols and if one is
	found it turns on a series of boolean holders that will
	influence how the program will run


####int dupFD()
	Input: Command Argument
	Returns: A file


	Allows for read, write, create, and append commands
	to be run.


####void revertFD()
	Input: The file that needs to be changed
	Returns: Nothing


	Changes standard in and out
