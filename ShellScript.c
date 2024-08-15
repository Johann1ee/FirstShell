#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define LINE_BUFFER 1024
#define SHELL_DELIM " \t\r\n\a"

char *shell_read_line(void){
    int buffer_size = LINE_BUFFER;
    int position = 0;
    char *buffer = malloc(sizeof(char) * buffer_size);
    int c;

    if (!buffer) {
        fprintf(stderr, "SHELL: ALLOCATION ERROR\n");
        exit(EXIT_FAILURE);
    }
    
    while (true)
    {
        // read character from input
        c = getchar();

        // if EOF is reached, replace with NULL character and return
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        // if buffer is exceeded, reallocate memory
        if (position >= buffer_size){
            buffer_size += LINE_BUFFER;
            buffer = realloc(buffer, buffer_size);
            if (!buffer){
                fprintf(stderr, "SHELL: ALLOCATION ERROR\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

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

int main (int argc, char **argv){

    shell_loop(); // command loop for shell

    return EXIT_SUCCESS;
}