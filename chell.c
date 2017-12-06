/* Copyright 2017 Goldy Mariz C. Lunesa
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

 // TODO (Mariz#1#): Make commands compatible with multiple items

#include <stdio.h>

#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>

#include <stdlib.h>
#include <sys/stat.h>

#include <math.h>
#include <time.h>

#include <windows.h>
#include <winbase.h>

#define bool int
#define true 1
#define false 0

char *trim(char *);

void cls_cmd();
void dir_cmd(DIR *dir);
void mkdir_cmd(char *folder);
void time_cmd(char* cmd);
void date_cmd(char *cmd);
void copy_cmd(char *cmd);
void type_cmd(char *cmd);
void rename_cmd(char *cmd);
void del_cmd(char *cmd);
void move_cmd(char *cmd);
void rmdir_cmd(char *cmd);

void formatInteger(unsigned n);
void formatIntegerFigures(unsigned n);


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
  } else if (strcmp(cmdtok, "time") == 0) {
    time_cmd(cmd);
  } else if (strcmp(cmdtok, "date") == 0) {
    date_cmd(cmd);
  } else if (strcmp(cmdtok, "copy") == 0) {
    copy_cmd(cmd);
  } else if (strcmp(cmdtok, "type") == 0) {
    type_cmd(cmd);
  } else if (strcmp(cmdtok, "rename") == 0) {
    rename_cmd(cmd);
  } else if (strcmp(cmdtok, "del") == 0) {
    del_cmd(cmd);
  } else if (strcmp(cmdtok, "move") == 0) {
    move_cmd(cmd);
  } else if (strcmp(cmdtok, "rmdir") == 0) {
    rmdir_cmd(cmd);
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
    // change
    // system("@cls||clear");

  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;

  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Move the cursor home */
  SetConsoleCursorPosition( hStdOut, homeCoords );
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

        int mkdir_status = mkdir(folder);           // Function for Windows
        // int mkdir_status = mkdir(folder, 0777);  // Function for Mac
        if(mkdir_status != 0) {
            printf("Directory already exists.\n");
        }
    }
}

int change_local_time(int hour, int minute, int second) {
  SYSTEMTIME st, lt;

  GetSystemTime(&st);
  GetLocalTime(&lt);

  printf("The system time is: %02d:%02d\n", st.wHour, st.wMinute);
  printf(" The local time is: %02d:%02d\n", lt.wHour, lt.wMinute);

  SYSTEMTIME sta;
  sta.wYear = st.wYear;
  sta.wMonth = st.wMonth;
  sta.wDayOfWeek = st.wDayOfWeek;
  sta.wDay = st.wDay;
  sta.wHour = hour;
  sta.wMinute = minute;
  sta.wSecond = second;
  sta.wMilliseconds = st.wMilliseconds ;

  SetLocalTime(&sta);

  DWORD dword = GetLastError();
  return (int) dword;
}


int is_time_valid(char *cmd) {
  char *hour = strtok(cmd, ":");
  char *minute = strtok(hour + strlen(hour) + 1, ":");
  char *second = minute + strlen(minute) + 1;

//  int hour = atoi (hour);
  int hour_int = atoi(hour);
  int minute_int = atoi(minute);
  int second_int = atoi(second);


  if (strlen(hour) != 2 || strlen(minute) != 2 || strlen(second) != 2) {
    return -1;
  } else if ((hour_int < 0 && hour_int > 23) || (minute_int < 0 && minute_int > 59) || (second_int < 0 && second_int > 59)) {
    return -1;
  } else {
    return change_local_time(hour_int, minute_int, second_int);
  }
}

void time_cmd(char *cmd){
  char buffer[32];

  cmd = strtok(NULL, " ");
  if(cmd != NULL) {
      int time_result = is_time_valid(cmd);
      if (time_result == -1) {
        printf("The system cannot accept the time entered.\n");
        return;
      } else if (time_result == 1314) {
        printf("A required privilege is not held by the client.\n");
      } else if (time_result == 0) {
        printf("Time updated successfully.");
      }

  } else {
     time_t raw_time = time(NULL);
     struct tm * tm = localtime(&raw_time);
     strftime(buffer, sizeof(buffer), "%X", tm);
     printf("The current time is: %s\n", buffer);
  }
}



void date_cmd(char *cmd){
  char buffer[32];

  cmd = strtok(NULL, " ");
  if(cmd != NULL) {
      printf("The system cannot accept the time entered.\n");
      return;
  }

  time_t raw_time = time(NULL);
  struct tm * tm = localtime(&raw_time);

  strftime(buffer, sizeof(buffer),  "%a %m/%d/%Y", tm);
  printf("The current date is: %s\n", buffer);
}

void copy_cmd(char *cmd) {
  char *cmd_src = strtok(NULL, " ");
  char *cmd_dest = cmd_src + strlen(cmd_src) + 1;

  cmd_dest = strtok(NULL, " ");
  // char *cmd_dest = strtok(NULL, " ") + 1;
  // char *cmd_dest = cmd_src + strlen(cmdtok) + 1;

  if(cmd_src == NULL) {
    printf("The syntax of the command is incorrect.\n");

  } else if (cmd_dest == NULL){
    printf("The file cannot be copied onto itself.\n");

  } else {
    char ch;
    FILE *source_file, *dest_file;

    source_file = fopen(cmd_src, "r");

    if(source_file == NULL) {
      printf("The system cannot find the file specified.\n");
    } else {

      if(strcmp(cmd_src, cmd_dest) == 0) {
        printf("The file cannot be copied onto itself.\n");
      } else {
        dest_file = fopen(cmd_dest, "w");
        ch = fgetc(source_file);
        while (ch != EOF) {
            fputc(ch, dest_file);
            ch = fgetc(source_file);
        }

        printf("1 file(s) copied.\n");
        fclose(dest_file);
      }

      fclose(source_file);
    }
  }
}

void type_cmd(char *cmd) {
  char *cmd_filename = strtok(NULL, " ");

  if(cmd_filename == NULL) {
    printf("The syntax of the command is incorrect.\n");
  } else {
    FILE *file;
    file = fopen(cmd_filename, "r");
    if(!file) {
      printf("The system cannot find the file specified.\n");
    }

    char buffer[10000];

    while(fgets(buffer, sizeof(buffer), file)) {
      printf("%s", buffer);
    }
    printf("\n");
    fclose(file);
  }
}

void rename_cmd(char *cmd) {
  char *cmd_src = strtok(NULL, " ");
  char *cmd_dest = cmd_src + strlen(cmd_src) + 1;

  cmd_dest = strtok(NULL, " ");

  if(cmd_src == NULL && cmd_dest == NULL) {
    printf("The syntax of the command is incorrect.\n");
  } else {
    int rename_success = rename(cmd_src, cmd_dest);

    if(rename_success == 0) {
      printf("1 file(s) renamed.\n");
    } else {
      printf("The syntax of the command is incorrect.\n");
    }
  }
}


void del_cmd(char *cmd) {
  char *cmd_filename = strtok(NULL, " ");

  if(cmd_filename == NULL) {
    printf("The syntax of the command is incorrect.\n");
  } else {
    FILE *file;
    file = fopen(cmd_filename, "r");

    if(file) {
      fclose(file);
      int remove_success = remove(cmd_filename);

      if(remove_success == 0) {
        printf("1 file(s) deleted.\n");
      } else {
        printf("Could not delete file %s\n", cmd_filename);
      }
    } else {
      printf("Could Not Find %s \n", cmd_filename);
    }
  }

}

void move_cmd(char *cmd) {
  char *cmd_src = strtok(NULL, " ");
  char *cmd_dest = cmd_src + strlen(cmd_src) + 1;

  cmd_dest = strtok(NULL, " ");

  if (cmd_src == NULL && cmd_dest == NULL) {
    printf("The syntax of the command is incorrect.\n");
  } else {
    char ch;
    FILE *source_file, *dest_file;

    source_file = fopen(cmd_src, "r");

    if(source_file == NULL) {
      printf("The system cannot find the file specified.\n");
    } else {

      // Copy the file to another directory
      dest_file = fopen(cmd_dest, "w");
      ch = fgetc(source_file);
      while (ch != EOF) {
          fputc(ch, dest_file);
          ch = fgetc(source_file);
      }

      printf("1 file(s) copied.\n");
      fclose(dest_file);

      fclose(source_file);

      // Delete the file in original directory
      int remove_success = remove(cmd_src);

      if(remove_success == 0) {
        printf("1 file(s) moved.\n");
      } else {
        printf("Could not delete file %s\n", source_file);
      }

    }


  }

}

void rmdir_cmd(char *cmd) {
  char *cmd_dest = cmd + strlen(cmd) + 1;
   DIR *dir = opendir(cmd_dest);

   printf("Here");
   if (dir != NULL) {
     // printf(dir);
     closedir(dir);
        int response = rmdir(cmd_dest);
        if (response) {
            printf("The directory is not empty.\n\n");
        }
   } else {
     // printf(dir);
     printf(cmd_dest);
     printf("The system cannot find the file specified.\n");
   }
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
