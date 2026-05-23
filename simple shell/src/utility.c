

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "simpleshell.h"



int parsing(char *input, char *tokens[], int max_length){
        int count = 0; 
        char *token = strtok(input, SEPARATORS); //gets the first part of command
        // keeps going through command and adding it to tokens until it gets to NULL
        while (token != NULL && count < max_length - 1) {
           tokens[count++] = token;
           token = strtok(NULL, SEPARATORS);
        }
        tokens[count] = NULL; // NULL-terminate the list 
        return count;



}







int internal_command(char *tokens[], int number, char *input_redirect, char *output_redirect, int add){
    
    char dir[1024];           // for dir
    char cwd[1024]; 
    if (strcmp(tokens[0], "clr") == 0) {  //if command is clr we clear
        system("clear");
        return 1;  //handle internal command
    }else if(strcmp(tokens[0], "quit") == 0){ // if command is quit we exit
        exit(0);
        return 1; // handles internal command
    }else if (strcmp(tokens[0], "pause") == 0){
        printf("current paused..... press Enter to keep going");
        while (getchar() != '\n'); //getting rid of the left over inputs making sure buffer is empty
        return 1;
    }else if(strcmp(tokens[0], "echo") == 0){
        help_redirection(input_redirect, output_redirect, add); /*before running any of the internal command
        this checks to see if the user typed any redirection symbols and then apply them*/
        for(int i = 1; i < number; i++){ // goes through words after echo
            printf("%s ", tokens[i]); // prints out the words after echo
        }
        printf("\n");
        fflush(stdout);
        freopen("/dev/tty", "w", stdout); // restore stdout back to terminal
        return 1;
    }else if(strcmp(tokens[0], "dir") == 0){
         help_redirection(input_redirect, output_redirect, add); /*before running any of the internal command
         this checks to see if the user typed any redirection symbols and then apply them*/
         if(number > 1){ // if argument is there
             snprintf(dir, sizeof(dir), "ls -al %s", tokens[1]); //creates string  to store folder name

         }else{
             
         snprintf(dir, sizeof(dir), "ls -al ."); // lists current folders
         }

         system(dir);// runs command thats in dir

         fflush(stdout);
         freopen("/dev/tty", "w", stdout); // restore stdout back to terminal
         return 1;
    }else if(strcmp(tokens[0], "environ") == 0){
        help_redirection(input_redirect, output_redirect, add); /*before running any of the internal command
        this checks to see if the user typed any redirection symbols and then apply them*/
        extern char **environ; //gets enviroment variables its an array of strings
        for(int i = 0; environ[i] != NULL; i++){ // goes through the enviroment array
            printf("%s\n", environ[i]); // print eniroment variable at index i
        }
        fflush(stdout);
        freopen("/dev/tty", "w", stdout); // restore stdout back to terminal
        return 1;
    }else if(strcmp(tokens[0], "cd") == 0){
          if(tokens[1] == NULL){ // no directory provided
               getcwd(cwd, sizeof(cwd)); // gets current directory
               printf("%s\n", cwd);
          }else{
               if(chdir(tokens[1]) != 0){  //if it fails print error message
                    printf("unable to change directory\n");

               }else{
                   getcwd(cwd, sizeof(cwd)); //gets new directory
                   setenv("PWD", cwd, 1); // updates enviroment
               }
          }
          return 1;
    }else if (strcmp(tokens[0], "help") == 0){
        help_redirection(input_redirect, output_redirect, add); /*before running any of the internal command
        this checks to see if the user typed any redirection symbols and then apply them*/
        system("more < manual/helpfile"); // displays the context of the helpfile
        
        fflush(stdout);
        freopen("/dev/tty", "w", stdout); // restore stdout back to terminal
        return 1;
    }
    return 0;

}


void I_O_redirection(char *tokens[], int *token_number, char **input_redirect, char **output_redirect, int *add){
    *input_redirect = NULL;
    *output_redirect = NULL;
    *add = 0;
    int num;
    for (int i= 0; i < *token_number; i++){  
            if (strcmp(tokens[i], "<") == 0){  // prints output in the file after <
               num = i + 1;
               if (num < *token_number){
                *input_redirect = tokens[i + 1];
               }
            /*so here i am getting rid of < and the input file so i shift everything after
            it to the left i am replacing it with what comes next which will overite it so
            it will disapear then i change the count by minus 2 because i am removing 2 things*/
               for(int k = i; k< *token_number - 2; k ++){
                 tokens[k] = tokens[k + 2];
               }
               *token_number -= 2;
               i --;
            }else if (strcmp(tokens[i], ">>") == 0){  // instead of clearing the file it adds to the end
                num = i + 1;
                if (num < *token_number){
                  *output_redirect = tokens[i + 1];
                  *add = 1; // change to 1 to know to add to the end instead of overwrite 
                }
                for(int k = i; k< *token_number - 2; k ++){
                tokens[k] = tokens[k + 2];
                }
                *token_number -= 2;
                i --;
            }else if (strcmp(tokens[i], ">") == 0){  //outputs commands in the file after >
               num = i + 1;
               if (num < *token_number){
                 *output_redirect = tokens[i + 1];
               }
               for(int k = i; k< *token_number - 2; k ++){
                  tokens[k] = tokens[k + 2];
               }
               *token_number -= 2;
               i --;
            
            }
    }
    tokens[*token_number] = NULL; //terminates at the end
           
       
}

void help_redirection(char *input_redirect, char *output_redirect, int add){
    if(input_redirect != NULL){ // if the user didnt input < we just skip
        if(freopen(input_redirect, "r", stdin) == NULL){ // open input file and replace it with commands it will reyturn null if error
            printf("redirection failed, try again"); // error message
            exit(1);
        }
    }
    if (output_redirect != NULL){ //check if user typed > or >>
        if(add == 1){ // if add is 1 we append the output to the file
            if (freopen(output_redirect, "a", stdout) == NULL){
                 printf("output redirection failed, try again");
                 exit(1);
            }
        }else{
            if(freopen(output_redirect, "w", stdout) == NULL){ // overwrite the file
                 printf("output redirection failed, try again");
                 exit(1);
            }
        }
    }
}

