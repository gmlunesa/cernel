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

char *trim(char *);

// Function to read user input and
// redirect them to the proper functions
void read_cmd(DIR *dir, char *cmd) {
  cmd = trim(cmd);
  char *cmdtok = strtok (cmd, " ");

  // Simple CMD command
  if (strcmp(cmdtok, "cmd") == 0) {
    printf("Microsoft Windows [Version 10.0.15063]\n(c) 2017 Microsoft Corporation. All rights reserved.\n\n");
  }

}

// Function to trim unnecessary spaces
char *trim(char *str) {
  char *lastChar;
  printf(str);



  // Trim preceding spaces
  while(isspace((unsigned char) *str)) {
    str++;
  }

  if (*str == 0) {
    return str;
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


int main(void) {
  char cwd[1024];
  char input[1024];

  DIR *dir;

  while (1) {
      if (getcwd(cwd, sizeof(cwd)) != NULL) {
          input[0] != '\0';
          printf("%s>", cwd);
          fflush(stdin);
          scanf("%[^\n]", input);

          if(input[0] != '\0'){
              read_cmd(dir, input);
          }

          printf("\n");
      } else {
          perror("Error: ");
          break;
      }
  }

  return(0);
}
