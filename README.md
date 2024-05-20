# Simple-shell-os

# Simple Job Scheduler

This project is a simple job scheduler implemented in C. It supports adding, scheduling, and executing jobs with specific commands. The job scheduler is capable of handling foreground and background jobs, with a focus on round-robin scheduling.

## Features

- **Command Line Interface:** Interactive prompt to enter commands.
- **Builtin Commands:** Support for commands like `quit`, `jobs`, `bg`, `fg`, `addjob`, `schedule`, and `help`.
- **Job Management:** Add and schedule jobs with specified burst times and arrival times.
- **Round-Robin Scheduling:** Jobs are executed based on a round-robin scheduling algorithm.
- **System Commands:** Execute system commands like `ls` and run C programs.

## Getting Started

### Prerequisites

- GCC compiler
- Unix-like operating system (Linux, macOS, etc.)

### Compilation

To compile the project, use the following command:

```bash
sudo gcc shell.c -o shell 
```

### Running

Run the compiled program:

```bash
sudo ./shell
```

You will be presented with a prompt `vansh>` where you can enter commands.

## Usage

### Builtin Commands

- **quit:** Exit the shell.
- **jobs:** Display the total number of jobs.
- **bg:** Display the number of background jobs.
- **fg:** Display the number of foreground jobs.
- **addjob \<name> \<burst_time> \<arrival_time>:** Add a new job.
- **schedule:** Schedule and execute jobs.
- **help:** Display the help message with available commands.
- **ls:** Show files in the current directory.
- **run \<filename>:** Compile and run a C file.

### Examples

- **Adding a Job:**

  ```bash
  vansh> addjob job1 5 0
  ```

- **Scheduling Jobs:**

  ```bash
  vansh> schedule
  ```

- **Running a System Command:**

  ```bash
  vansh> ls
  ```

- **Compiling and Running a C Program:**

  ```bash
  vansh> run program.c
  ```

- **Showing What all commands are there:**

  ```bash
  vansh> help
  ```

- **For going back to the terminal**

  ```bash
  vansh> quit
  ```
- **Displaying number of jobs:**

  ```bash
  vansh> jobs
  ```

### Error Handling

The shell provides feedback for incorrect usage or errors encountered during command execution. For example:

- Trying to add a job with incorrect arguments will display a usage message.
- Exceeding the maximum number of jobs will display an error message.

## Code Structure

- **main.c:** Contains the main function, command parsing, and job scheduling logic.
- **job.c:** Manages job structures and scheduling functions.
- **command.c:** Handles command parsing and execution.

## Future Enhancements

- Improve job scheduling algorithm to support more complex scenarios.
- Add support for job priorities.
- Enhance command parsing to handle more complex command structures.

## License

This project is licensed under the MIT License.

## Author

Vansh - [GitHub](https://github.com/vansh-seth)
