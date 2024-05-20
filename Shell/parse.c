#include <stdio.h>  
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h> 

#define MAXLINE 1024
#define MAXARGS 128

char prompt[] = "vansh> "; 

struct command {
  int argc;              
  char *argv[MAXARGS];   
  enum builtin_t {       
    NONE, QUIT, JOBS, BG, FG } builtin;
};

enum builtin_t parseBuiltin(struct command *cmd) {
    if (!strcmp(cmd->argv[0], "quit")) { 
        return QUIT;
    } else if (!strcmp(cmd->argv[0], "jobs")) { 
        return JOBS;
    } else if (!strcmp(cmd->argv[0], "bg")) { 
        return BG;
    } else if (!strcmp(cmd->argv[0], "fg")) { 
        return FG;
    } else {
        return NONE;
    }
}

void error(char *msg) {
  printf("Error: %s", msg);
  exit(0);
}

int parse(const char *cmdline, struct command *cmd) {
    static char array[MAXLINE];          
    const char delims[10] = " \t\r\n";   
    char *line = array;                   
    char *token;                          
    char *endline;                        
    int is_bg;                           

    if (cmdline == NULL) {
        (void) fprintf(stderr, "Error: command line is NULL\n");
        return -1;
    }

    (void) strncpy(line, cmdline, MAXLINE);
    endline = line + strlen(line);
    cmd->argc = 0;

    while (line < endline) {
        line += strspn (line, delims);
        if (line >= endline) break;

        token = line + strcspn (line, delims);

        *token = '\0';

        cmd->argv[cmd->argc++] = line;

        if (cmd->argc >= MAXARGS-1) break;

        line = token + 1;
    }

    cmd->argv[cmd->argc] = NULL;

    if (cmd->argc == 0)  
        return 1;

    cmd->builtin = parseBuiltin(cmd);

    if ((is_bg = (*cmd->argv[cmd->argc-1] == '&')) != 0)
        cmd->argv[--cmd->argc] = NULL;

    return is_bg;
}

void eval(char *cmdline) {
  int bg;
  struct command cmd;

  printf("Evaluating '%s'\n", cmdline);

  bg = parse(cmdline, &cmd); 
  printf("Found command %s\n", cmd.argv[0]);
  printf("Builtin %d\n", cmd.builtin);

  if (bg == -1) return;
}

int main(int argc, char **argv) {
  char cmdline[MAXLINE]; 
  char c;

  while (1) {

    printf("%s", prompt);

    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
      error("fgets error");

    if (feof(stdin)) { 
      printf("\n");
      exit(0);
    }
    
    cmdline[strlen(cmdline)-1] = '\0';
    
    eval(cmdline);
  }
}
