#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

#define inputStringLength 100
#define commandLength 100


// for spliting command
char** commandSpliter(char *inputCommand)
{
    if (inputCommand == NULL && strlen(inputCommand)==0)return NULL;

    int i = 0;
    char *command; 
    char **commandList = (char **) malloc(commandLength * sizeof(char *));
    // for seperating each commands with ";"
    while( (command = strsep(&inputCommand,";")) != NULL ){
 if(strlen(command) == 0)continue;
 if (strcmp(command, "exit") == 0 || strcmp(command, "as") == 0) { // we wanna use quit instance of exit
  fprintf(stderr , "\033[1;31m%s: command not found\n\033[0;m",command);
                continue;
        }
        commandList[i] = command;
 i++;
    }
    commandList[i++] = 0;

    // free memory that input commands allocate
    free(inputCommand);

    return commandList;
}

// for checking command's argument
char** argumentDetect(char *inputCommand)
{
    if (inputCommand == NULL)return NULL;

    int i = 0;
    char *token;
    char **tokenList = (char **) malloc(commandLength * sizeof(char *));
    // for seperating token with space bar
    while( (token = strsep(&inputCommand," ")) != NULL ){
 if(strcmp(token,"quit") == 0) { //check quit command
     printf("\033[1;31mExiting Shell...\n");
     exit(0);
 }
 else if(strlen(token)==0) continue; //in case of intpu = "; ; ;"  
        if(token != NULL && token[0] != 0 && token[0] != ' '){
            tokenList[i] = token; 
        } 
        else{
            i--;
        }
        i++;
    }
    tokenList[i++] = '\0'; 

    free(inputCommand); // free memory that input commands allocate

    return tokenList; //tokenList --> {"ls","-a"}
}

// function for sending input to run
void executeCommand(char **commandList)
{
    int i, commandCount = 0, status;
    char **command;

    while (1) {

 command = argumentDetect(commandList[commandCount++]); // seperate command list in to one command 
 
 if(command == NULL) break;
 
 // create child process
        else {
     pid_t forkArgument = fork();

     if (!forkArgument) { // if pid == 0

                int execvpArgument = execvp(command[0], command); // send command to shell 

         if(execvpArgument < 0) {
      fprintf(stderr , "\033[1;31m%s: command not found\n\033[0;m",command[0]);  
                }  
         // cannot find command in shell
                exit(0);

  return 0;
            }
        }
 // wait for each child to finish their process
     for (i = 0; i < commandCount; i++) 
     wait(&status);
    }
    
 
    free(commandList); // free memory that input commands allocate
}

//Handle CTRL+C >> In Shell CTRL+C Do Not Exit The Program
void sigint_handler(int signal){
 printf("\n\033[1;31mCan Not Terminated Program By Using Ctrl+C\033[0;m\n$ ");
 fflush(stdout);
}

int main(int argc , char **argv){
 
    signal(SIGINT,sigint_handler);

    
    //INTERACTIVE MODE
    if(argc == 1){
        printf("\033[1;94mEntering Shell...\n\033[0;m");
        while(1){
  
            char inputCommand[inputStringLength];  
            inputCommand[0] = '\0'; // for pressing enter with no command
            printf("$ ");
            scanf("%[^\n]",inputCommand); // for all input from keyboard with spacebar
            getchar(); // do not want "/n"
     if(strlen(inputCommand) == 0) continue;
            executeCommand(commandSpliter(inputCommand)); // function for sending input to run
 
        }
    }
    //BATCH MODE
    else if(argc==2){
        FILE *fp;   
        fp = fopen(argv[1], "r"); // for reading file
        // for checking if file is really exist
        if(!fp){
            perror(argv[1]); 
            printf("\033[1;31mExiting Shell...\n");
            exit(1);
        }

        char inputCommand[inputStringLength];
        // for reading each line in batch file
        while(fgets(inputCommand, inputStringLength , (FILE*)fp)){
            inputCommand[strlen(inputCommand)-1] = '\0';
            executeCommand(commandSpliter(inputCommand));
        }

        fclose(fp); // for closing file
        exit(0);
    }
    //In case of more than 1 argv input
    else{
 fprintf(stderr , "\033[1;31mError : Only One Batch File Can Be Excecute\n");
 return 0;

    }

    return 0;
     
}