
#ifndef SIMPLESHELL_H
#define SIMPLESHELL_H

#define SEPARATORS " \t\n"


int parsing(char *input, char *tokens[], int max_length); /* defining function where 
i will be putting the command into tokens*/
int internal_command(char *tokens[], int nums, char *input_redirect, char *output_redirect, int add);
void I_O_redirection(char *tokens[], int *token_number, char **input_redirect, char **output_redirect, int *add);
void help_redirection(char *input_redirect, char *output_redirect, int add);
#endif