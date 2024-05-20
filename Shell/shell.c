#include <stdio.h>  
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 1024
#define MAXARGS 128
#define MAXJOBS 100

char prompt[] = "vansh> ";

struct job {
    char name[MAXLINE];
    int burst_time;
    int arrival_time;
    int remaining_time;
};

int total_jobs = 0;
int bg_jobs = 0;
int fg_jobs = 0;
int time_quantum = 2;
struct job jobs[MAXJOBS];

struct command {
    int argc;              
    char *argv[MAXARGS];   
    enum builtin_t {       
        NONE, QUIT, JOBS, BG, FG, ADDJOB,SCHEDULE,HELP
    } builtin;
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
    } else if (!strcmp(cmd->argv[0], "addjob")) { 
        return ADDJOB;
    } else if (!strcmp(cmd->argv[0], "schedule")) { 
        return SCHEDULE;
    } else if (!strcmp(cmd->argv[0], "help")) {
        return HELP;
    } else if (!strcmp(cmd->argv[0], "ls")) { 
        return NONE;
    } else if (!strcmp(cmd->argv[0], "run")) { 
        return NONE;
    }else {
        return NONE;
    }
}


void error(char *msg) {
    printf("Error: %s\n", msg);
    exit(1);
}

int parse(const char *cmdline, struct command *cmd) {
    static char array[MAXLINE];          
    const char delims[10] = " \t\r\n";   
    char *line = array;                   
    char *token;                         
    char *endline;                        
    int is_bg;                           

    if (cmdline == NULL) 
        error("command line is NULL");

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

int addJob(struct command *cmd) {
    if (cmd->argc != 4) {
        printf("Error: Usage: addjob <name> <burst_time> <arrival_time>\n");
        return -1; 
    }

    if (total_jobs >= MAXJOBS) {
        printf("Error: Maximum number of jobs reached\n");
        return -2;
    }

    struct job new_job;
    strcpy(new_job.name, cmd->argv[1]);
    new_job.burst_time = atoi(cmd->argv[2]);
    new_job.arrival_time = atoi(cmd->argv[3]);
    new_job.remaining_time = new_job.burst_time;

    jobs[total_jobs++] = new_job;
    printf("Job added: %s\n", new_job.name);
    return 0;
}

void runSystemCommand(struct command *cmd, int bg) {
    pid_t childPid;
    if ((childPid = fork()) < 0)
        error("fork() error");
    else if (childPid == 0) {
        // Check if the command is 'ls' or 'run'
        if (strcmp(cmd->argv[0], "ls") == 0) {
            // If 'ls' is found, execute it with the arguments
            execvp("ls", cmd->argv);
            // If execvp returns, it means there was an error
            printf("ls: Command not found\n");
            exit(0);
        } else if (strcmp(cmd->argv[0], "run") == 0) {
            // If 'run' is found, compile and run the.c file
            char *filename = cmd->argv[1];
            char *output_file = strtok(filename, ".");
            char gcc_cmd[100];
            snprintf(gcc_cmd, sizeof(gcc_cmd), "gcc %s.c -o %s.out", output_file, output_file);
            system(gcc_cmd);
            snprintf(gcc_cmd, sizeof(gcc_cmd), "./%s.out", output_file);
            execvp(gcc_cmd, cmd->argv + 1);
            // If execvp returns, it means there was an error
            printf("%s: Command not found\n", cmd->argv[0]);
            exit(0);
        } else {
            // If it's not 'ls' or 'run', execute the command normally
            if (execvp(cmd->argv[0], cmd->argv) < 0) {
                printf("%s: Command not found\n", cmd->argv[0]);
                exit(0);
            }
        }
    }
    else {
       if (bg)
          printf("Child in background [%d]\n",childPid);
       else
          wait(&childPid);
    }
}



void runBuiltinCommand(struct command *cmd, int bg) {
    switch (cmd->builtin) {
        case QUIT:
            printf("vansh: quit\n"); 
            exit(0);
        case JOBS:
            printf("Total number of jobs: %d\n", total_jobs);
            break;
        case BG:
            printf("Number of background jobs: %d\n", bg_jobs);
            break;
        case FG:
            printf("Number of foreground jobs: %d\n", fg_jobs);
            break;
        case ADDJOB:
            addJob(cmd);
            break;
        case SCHEDULE:
            scheduleJobs();
            break;
        case HELP:
            printf("Available commands:\n");
            printf("quit - Quit the shell\n");
            printf("jobs - Display total number of jobs\n");
            printf("bg - Display number of background jobs\n");
            printf("fg - Display number of foreground jobs\n");
            printf("addjob <name> <burst_time> <arrival_time> - Add a new job\n");
            printf("schedule - Schedule and execute jobs\n");
            printf("help - Display this help message\n");
            printf("ls - show files in directory\n");
            printf("run - run c files in directory\n");
            break;
        default:
            error("Unknown builtin command");
    }
}

void executeJob(struct job *job) {
    printf("Executing job: %s\n", job->name);
    int remaining_time = job->remaining_time;

    if (remaining_time > time_quantum) {
        sleep(time_quantum);
        job->remaining_time -= time_quantum;
    } else {
        sleep(remaining_time);
        job->remaining_time = 0;
    }
}

void eval(char *cmdline) {
    int bg;
    struct command cmd;

    printf("Evaluating '%s'\n", cmdline);
    bg = parse(cmdline, &cmd); 

    if (bg == -1) return;
    if (cmd.argv[0] == NULL) return;

    if (cmd.builtin == NONE) {
        if (parseBuiltin(&cmd) == ADDJOB)
        { 
            int result = addJob(&cmd);
            if (result == -1 || result == -2) {
                printf("Error occurred while adding job\n");
                return;
            }
        } else 
        {
            runSystemCommand(&cmd, bg);
            return;
        }
    } else {
        runBuiltinCommand(&cmd, bg);
    }
}



void scheduleJobs(void) {
    printf("Scheduling jobs...\n");

    while (total_jobs > 0) {
        for (int i = 0; i < total_jobs; i++) {
            struct job *current_job = &jobs[i];

            if (current_job->remaining_time > 0) {
                printf("Running job: %s\n", current_job->name);
                executeJob(current_job);

                if (current_job->remaining_time == 0) {
                    printf("Job %s completed\n", current_job->name);
                    for (int j = i; j < total_jobs - 1; j++) {
                        jobs[j] = jobs[j + 1];
                    }
                    total_jobs--;
                    i--;
                }
            }
        }
    }

    printf("All jobs completed\n");
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
