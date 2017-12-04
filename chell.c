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

#include <stdlib.h>
#include <sys/stat.h>

#include <math.h>
#include <time.h>

char *trim(char *);
void cls_cmd();
void formatInteger(unsigned n);
void formatIntegerFigures(unsigned n);
void dir_cmd(DIR *dir);
void mkdir_cmd(char *folder);


// Function to read user input and
// redirect them to the proper functions
void read_cmd(DIR *dir, char *cmd) {
  cmd = trim(cmd);
  char *cmdtok = strtok (cmd, " ");

  // Simple CMD command
  if (strcmp(cmdtok, "cmd") == 0) {
    printf("Microsoft Windows [Version 10.0.15063]\n(c) 2017 Microsoft Corporation. All rights reserved.\n\n");
  } else if (strcmp(cmdtok, "cls") == 0) {
    cls_cmd();
  } else if (strcmp(cmdtok, "cd") == 0) {
    char *cmd_dest = cmd + strlen(cmdtok) + 1;

    char *dest_null = strtok(NULL, " ");

    if (dest_null == NULL) {
      char cwd[1024];
      getcwd(cwd, sizeof(cwd));
      printf("%s\n", cwd);
    } else if (chdir(trim(cmd_dest)) != 0) {
        printf("The system cannot find the path specified.\n");
    }
  } else if (strcmp(cmdtok, "cd..") == 0) { // special case
    chdir("..");
  } else if (strcmp(cmdtok, "dir") == 0) {
    dir_cmd(dir);
  } else if (strcmp(cmdtok, "chdir") == 0) {

    char *cmd_dest = cmd + strlen(cmdtok) + 1;
    if (chdir(trim(cmd_dest)) != 0) {
      printf("The system cannot find the path specified.\n");
    }
  } else if (strcmp(cmdtok, "mkdir") == 0) {
    char *folder = cmd + strlen(cmdtok) + 1;
    mkdir_cmd(folder);
  }

}

// Function to trim unnecessary spaces
char *trim(char *str) {
  char *lastChar;

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

void cls_cmd() {
    system("@cls||clear");
}

void dir_cmd(DIR *dir) {
  struct dirent *dirnt;
  dir = opendir(".");

  struct stat attr;
  char buff[20];
  struct tm *time_info;

  int file_count = 0;
  int dir_count = 0;

  if (dir) {
    dirnt = readdir(dir);
    while (dirnt != NULL) {
      stat(dirnt->d_name, &attr);
      time_info = localtime(&attr.st_mtime);
      strftime(buff, sizeof(buff), "%m/%d/%Y %I:%M %p", time_info);
      printf("%s", buff);

      if(S_ISDIR(attr.st_mode)) {
        printf("\t\t  <DIR>\t\t");
      } else {
        int count = (int)log10(attr.st_size) + 1;
        count += (count-1)/3; // adjust with the spaces and commas

        int i = 0;
        for (; i < 20-count; i++) {
            printf(" ");
        }
        formatInteger((unsigned)attr.st_size);
        printf("\t");
      }

      printf("\t%s\n", dirnt->d_name);

      dirnt = readdir(dir);
    }
    closedir(dir);
  }
  else perror("Error: ");
}

void formatInteger (unsigned n) {
  // Negative number formatting
  if (n < 0) {
      printf ("-");
      n = -n;
  }

  formatIntegerFigures(n);

}

void formatIntegerFigures (unsigned n) {
  // Recursive implementation for numbers more than 999
  if (n < 1000) {
    printf("%d", n);
    return;
  }

  formatIntegerFigures(n/1000);
  printf(",%03d", n%1000);
}

void mkdir_cmd(char *folder) {
    if (folder == NULL) {
      printf("usage: mkdir [-pv] [-m mode] directory ...\n");
    } else {
        int mkdir_status = mkdir(folder, 0777);
        if(mkdir_status != 0) {
            printf("Directory already exists.\n");
        }


    }
}


int main(void) {
  char cwd[1024];
  char input[1024];

  DIR *dir;

  int boolSample = 1;

  while (boolSample < 4) {
      printf("hello there");
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

      ++boolSample;


  }

  return(0);
}
