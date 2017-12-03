#include <stdio.h>

#include <unistd.h>
#include <errno.h>

int main(void) {
    char cwd[1024];
    char input[1024];

    while (1) {
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s>", cwd);
            fflush(stdin);
            scanf("%[^\n]", input);
            printf("\n");
        } else {
            perror("Shell does not recognize that command. Please try again.");
            break;
        }
    }

    return(0);
}

