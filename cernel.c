#include <stdio.h>

#include <unistd.h>
#include <errno.h>

// this is the loop that waits for any input
void standby() {


}

void cd() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("%s> ", cwd);
    // printf("%s\n", getenv("USERNAME"));
}

int main() {
    cd();
    return 0;
}


