/*
   @params: takes a char* dir which is the name of the directory to cd into
   @return: returns nothing
   @purpose: implements the cd terminal command for our custom shell
 */
void cd(char* dir);
void chdir_with_tilde(char* dir);
void print_working_dir();
