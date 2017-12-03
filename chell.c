/* Copyright [2017] [Goldy Mariz C. Lunesa]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <stdio.h>

#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>

int main(void) {
    char cwd[1024];
    char input[1024];

    DIR *dir;

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

// Function to read user input and
// redirect them to the proper functions
void read_cmd(DIR *dir, char *cmd) {


}

// Function to trim unnecessary spaces
char *trim(char *str) {
    char *lastChar;

    if (*str == 0) {
      return str;
    }

    // Trim preceding spaces
    while(isspace((unsigned char) *str)) {
      str++;
    }

    // Trim proceeding spaces
    lastChar = str + strlen(str) - 1;
    while(lastChar > str && isspace((unsigned char)*lastChar)) {
      lastChar--;
    }

    // Setting a new null char terminator
    *(lastChar+1) = 0;

    return str;
}

