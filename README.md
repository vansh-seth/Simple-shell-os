# Simple-shell-os

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
- WSL

### If folder is stored in c drive

To mount drive, use the following command:

```bash
sudo mkdir /mnt/c
```
```bash
sudo mount -t drvfs C: /mnt/c
```
Now, get that directory:

```bash
cd /mnt/c/shell
```

![image](https://github.com/vansh-seth/Simple-shell-os/assets/111755254/855bc18e-1870-474e-99cc-bc7ac7a7716b)

### Compilation

To compile the project, use the following command:

```bash
sudo gcc shell.c -o shell 
```

![image](https://github.com/vansh-seth/Simple-shell-os/assets/111755254/2c992101-77ca-40e2-9312-c12d804b5f2c)

### Running

Run the compiled program:

```bash
sudo ./shell
```
![image](https://github.com/vansh-seth/Simple-shell-os/assets/111755254/bafc72c8-8761-4bea-88ff-af2b77b77e71)

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
![image](https://github.com/vansh-seth/Simple-shell-os/assets/111755254/468d6c10-a4aa-46f9-8d1f-5cd27970bffa)

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
![image](https://github.com/vansh-seth/Simple-shell-os/assets/111755254/d7e91206-98c3-4d49-9855-667a5715f1a1)

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
