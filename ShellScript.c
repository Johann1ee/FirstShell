#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define LINE_BUFFER 1024
#define SHELL_DELIM " \t\r\n\a"

char *shell_read_line(void){
    char *line = NULL;
    ssize_t buffer_size = 0;

    if (getline(&line, &buffer_size, stdin) == -1){
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        } else {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }

    return line;
}

char **shell_split_line(char *line){
    int buffer_size = LINE_BUFFER, position = 0;
    char **tokens = malloc(buffer_size * sizeof(char*));
    char *token;

    if (!tokens){
        fprintf(stderr, "SHELL: ALLOCATION ERROR\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, SHELL_DELIM);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= buffer_size){
            buffer_size += LINE_BUFFER;
            tokens = realloc(tokens, buffer_size * sizeof(char*));
            if (!tokens)
            {
                fprintf(stderr, "SHELL: ALLOCATION ERROR\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, SHELL_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int shell_launch(char **args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1){
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0){
        perror("lsh");
    } else {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

void shell_loop(void){
    char *line; // line given by user through shell input
    char **args; // split arguments based on line
    int status; // decides whether the shell should exit or not

    do { // continues to run as long as status is true
        printf("> ");
        line = shell_read_line();
        args = shell_split_line(line);
        status = shell_execute(args);

        free(line);
        free(args);
    } while (status);
}

// SHELL COMMANDS 
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);

char *builtin_str[] = {
    "cd",
    "help",
    "exit",
};

int (*builtin_func[]) (char **) = {
    &shell_cd,
    &shell_help,
    &shell_exit,
};

int shell_num_builtin() {
    return sizeof(builtin_str) / sizeof(char *);
}

// SHELL COMMAND IMPLEMENTATION
int shell_cd(char **args){
    if (args[1] == NULL) {
        fprintf(stderr, "SHELL: EXPECTED ARGUEMENTT TO \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0){
            perror('lsh');
        }
    }
    return 1;
}

int shell_help(char **args){
    int i;
    printf("Johann Lee's LSH\n");
    printf("Type program anames and arguements and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < shell_num_builtin(); i++){
        printf(" %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int shell_exit(char **args){
    return 0;
}

int shell_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < shell_num_builtin(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return shell_launch(args);
}

int main (int argc, char **argv){

    shell_loop(); // command loop for shell

    return EXIT_SUCCESS;
}