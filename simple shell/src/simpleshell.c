

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "simpleshell.h"
#include <sys/wait.h>

    
int main(int argc, char*argv[]){
    char cwd1[1024]; // stores current directory
    char path[1024]; // stroes path
    int max_len = 200; // length of the command max length
    char *command = malloc(max_len); // allocating memory for the command before getting it from user

    char *tokens[max_len]; // empty token list
    char *input_redirect, *output_redirect;
    int add;

    realpath(argv[0], path); // gets full path of executable
    setenv("shell", path, 1); // sets enviromnet

    if (!command) { //check if memory allocation failed
    fprintf(stderr, "memory allocation failed\n");
    return 1;
    }

    FILE *command_line; // file pointer
    if(argc > 1){
        command_line = fopen(argv[1], "r");
        if (command_line == NULL) {
             perror("batch file"); // get inputs from batch file
             return 1;
        }
    }else{
        command_line = stdin; // get input from users
    }

    while(1){ //shell keeps going
        input_redirect = NULL;
        output_redirect = NULL;
        add = 0;
        if (command_line == stdin){ //if user input
            getcwd(cwd1, sizeof(cwd1)); // get current directory
            printf("%s> ", cwd1); // print a prompt for user
            fflush(stdout); //prints everything from the buffer and doesnt wait
        }
        if (fgets(command, max_len, command_line) == NULL){ 
            break;
        } // getting input 
        
        // getting rid of new line characters
        for(int i=0; command[i] != '\0'; i++){
            if (command[i] == '\n' || command[i] == '\r') {
                command[i] = '\0';
                break;
            }
        }
        
        int num_tokens = parsing(command, tokens, max_len); //split commands into tokens
        I_O_redirection(tokens, &num_tokens, &input_redirect, &output_redirect, &add);
        if (num_tokens == 0){
            continue;
        }
        if(internal_command(tokens, num_tokens, input_redirect, output_redirect, add)){ // check internal commands
            continue;
        }
        fflush(stdout);

        int back_execution = 0; // if its 0 theres no & at the end 
        if(num_tokens > 0 && strcmp(tokens[num_tokens - 1], "&") == 0){
            back_execution = 1; //if its one then theres & at the end
            tokens[num_tokens - 1] = NULL; // gets rid of & in the command
            num_tokens --; //reducing the count since & is gone 
        }

        pid_t pid = fork(); // creates a copy of my shell process.

        if (pid < 0) {
          perror("fork failed"); // if its negative something went wrong
        } else if (pid == 0) {
         help_redirection(input_redirect, output_redirect, add);
        // returns 0 if it is the child process and gets child process id
         setenv("parent", getenv("shell"), 1);  // sets parent environment variable to shell paths
         execvp(tokens[0], tokens);             // replaces the child process with the command the user puts
         // if execvp returns, it failed
         perror("execvp failed");
         exit(1); // kills child
        } else {
        
          // PARENT PROCESS
          if(back_execution == 0){
            waitpid(pid, NULL, 0);  // wait for child to finish
          }else{
            printf("working with this process %d\n", pid);
          }
          /*restores everything so next command works normally if this wasnt there then 
          the next prompt might be in the output/input file*/
          freopen("/dev/tty", "w", stdout); 
          freopen("/dev/tty", "r", stdin);  
        }
      
    }
    if (command_line != stdin) { // if it was in batch mode
       fclose(command_line); // close file
    }
    free(command); // free memory
    return 0;
}

