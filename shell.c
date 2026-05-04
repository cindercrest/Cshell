// creating a custom shell in C !!! less gooo!!

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<signal.h>


#define BUFFER_SIZE 1024
#define HISTORY_SIZE 10

char *history[HISTORY_SIZE];
int history_count = 0 ;

// read input 
char *read_input()
{
    char *input = malloc(BUFFER_SIZE * sizeof(char));
    if(!input)
    {
        fprintf(stderr, "Cshell: allocation error\n"); // exit if memory cannot be allocated
        exit(EXIT_FAILURE);                              // exit command EXIT_FAILURE
    }
    fgets(input,BUFFER_SIZE,stdin);
    input[strcspn(input, "\n")] = '\0'; // necessary newline removal 
    return input ;
}

// now to parse it as let's say vim cat.py is parsedd into vim + cat.py so the parsing is 
// based on the whitespace gotacha !! 

char **parse_input(char *input)
{
    int bufsize = 64 , position = 0 ;
    char **tokens = malloc(bufsize * sizeof(char*)); // an array of pointers of indivisual 
    char *token ;                                    // token 
    
    token = strtok(input," "); // parse it base on delimeter which is " " in this case
    while(token!=NULL)
    {
        tokens[position++] = token ;
        token = strtok (NULL , " " ) ; // counterintutive but NULL starts from where it
                                       // last parsed the string otherwise we be in an 
                                       // infinite loop
    }
    tokens[position] = NULL;     
    return tokens ;                    // return the array pointer( pointer to pointer ) 
}

// there are some built in commands that do not reuire other process so it's good we make 
// them here only less goo 

int cd(char **args)
{
    if(args[1] == NULL)
    {
        fprintf(stderr,"Cshell:expected something-_-\n");
    }
    else
    {
        if(chdir(args[1])!=0)
        {
            perror("Cshell");
        }
    }
    return 1 ;
}

void add_to_history(char *command)
{
    if(history_count < HISTORY_SIZE )
    {
        history[history_count++] = strdup(command);
    }
    else
    {
        free(history[0]);
        for(int i = 1 ; i < HISTORY_SIZE ; i++)
        {
            history[i-1] = history[i];
        }
        history[HISTORY_SIZE - 1 ] = strdup(command);
    }
}

void print_history()
{
    for(int i = 0 ; i < history_count ; i++ )
    {
        printf("%d %s\n", i+1 ,history[i]);
    }
}

void handle_redirection(char **args)
{
    for(int i = 0; args[i] != NULL ; i++)
    {
        if(strcmp(args[i],">")==0)
        {
           int fd = open(args[i+1] , O_WRONLY | O_CREAT | O_TRUNC , 0644);
           if(fd<0)
           {
               perror("Cshell");
               exit(EXIT_FAILURE);
           }
           dup2(fd,STDOUT_FILENO);
           close(fd);
           args[i] = NULL;
        }
        else if(strcmp(args[i],">>") == 0)
        {
            int fd = open(args[i+1] , O_WRONLY | O_CREAT | O_APPEND , 0644);
            if(fd<0)
            {
                perror("Cshell");
                exit(EXIT_FAILURE);
            }
            dup2(fd,STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
        }
        else if(strcmp(args[i],"<")==0)
        {
            int fd = open(args[i+1] , O_RDONLY);
            if(fd<0)
            {
                perror("cshell");
                exit(EXIT_FAILURE);
            }
            dup2(fd,STDIN_FILENO);
            close(fd);
            args[i] = NULL;
        }
    }
}

void execute(char **args, int background)
{
    pid_t pid ;
    int status ;
    pid = fork();
    if(pid == 0)
    {
        handle_redirection(args);
        if(execvp(args[0],args)==-1)
        {
            perror("Cshell");
        }
        exit(EXIT_FAILURE);
    }
    else if(pid<0)
    {
        perror("Cshell");
    }
    else
    {
        if(!background)
        {
            waitpid(pid,&status,0);
        }
        else
        {
            printf("Process is running in background with PID: %d\n",pid);
        }
    }

}

int check_background(char **args)
{
    int i = 0 ;
    while(args[i]!=NULL)
    {
        i++;
    }
    if(i>0 && strcmp(args[i-1],"&") == 0)
    {
        args[i-1] = NULL;
        return 1;
    }
    return 0;
}


// if ctrl+c is pressed instead of quiting the shell we reprompt the user

void handle_signal(int sig)
{
    if(sig == SIGINT)
    {
        printf("\nCshell> ");
        fflush(stdout);
    }
}

int main()
{
    char *input ;
    char **args;
    int background;
    
    signal(SIGINT,handle_signal);
    
    while(1)
    {
        char cwd[1024];
        getcwd(cwd,sizeof(cwd));
        printf("%s@Cshell:%s> ", getenv("USER"),cwd);

        input = read_input();

        if(strlen(input) == 0)
        {
            continue;
        }
        add_to_history(input);
        args = parse_input(input);
        
        if(args[0] == NULL)
        {
            free(input);
            free(args);
            continue;
        }

        if(strcmp(args[0],"exit")==0) 
        {
            break;
        }
        else if(strcmp(args[0],"cd")==0)
        {
            cd(args);
        }
        else if(strcmp(args[0],"history")==0)
        {
            print_history();
        }
        else
        {
            background = check_background(args);
            execute(args , background);
        }
        free(input);
        free(args);
        
    }
    return 0;
}
