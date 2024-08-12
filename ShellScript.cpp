// C++ Program to run a shell 
#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>

// Function required in C++
void clear() {
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    std::cout << "\x1B[2J\x1B[H";
}

// Initial greeting to user upon shell launch
void shell_initialization(){
    clear();
    std::cout << "\n\n\n\n******************"
        "************************";
    std::cout << "\n\n\n\t****MY SHELL****";
    std::cout << "\n\n\t-USE AT YOUR OWN RISK-";
    std::cout << "\n\n\n\n*******************"
        "***********************";
    
    char *username = getenv("USER");
    std::cout << "\n\n\nUser is : " << username;
    std::cout << "\n";
    sleep(3);
    clear();
}

int readInput(char *str){
    char *buffer;

    while (true) {
        // Prompt an input
        buffer = readline("\n>>> ");

        if (strlen(buffer) != 0){
            add_history(buffer);
            strcpy(str, buffer);
            return 0;
        } else {
            return 1;
        }
    }
}

int printDir(){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    std::cout << "\nDir: " << cwd;
}

int main() {
    shell_initialization();
    return 0;
}
