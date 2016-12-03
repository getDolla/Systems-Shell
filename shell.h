void pwd(char* cur_dir);

int trim(char* str);
int parse(char* command_ptr, char** args);

void get_terminal_commands(char* command_buffer, int size);
int run_terminal_commands(char* command_ptr, char **array_of_commands, char **array_of_arguments);
int get_num_commands(char* command_ptr, char **array_of_commands);
int check_command_type(char *command_ptr, char** array_of_arguments);
void execvp_commands(char* command_ptr, char** args2);

void print_exit_status(int status);

int parse_pipes(char* command_with_pipes, char** args);
int get_num_pipes(char* command);
char chkrdrect( char * arg );
int dupFD( char* p );
void revertFD( int fd );
