#include <iostream>
#include <unistd.h>
#include <vector>
#include <signal.h>
#include <sys/wait.h>

int procCin(void) {
    while (!std::cin.eof()) {
        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);
        std::cout << line << std::endl;
    }
    return 0;
}

int main (int argc, char **argv) {
    std::vector<pid_t> kids;

    //generate pipe
    int RtoA[2];
    int A1BtoA2[2];
    pipe(RtoA);
    pipe(A1BtoA2);
    pid_t child_pid;
    
    child_pid = fork();
    if(child_pid == 0)
    {
        
        dup2(RtoA[1], STDOUT_FILENO);
        close(RtoA[0]);
        close(RtoA[1]);

        int rgen = execv("rgen",argv);
        return rgen;
    }

    kids.push_back(child_pid);
    
    child_pid = fork();
    if(child_pid == 0)
    {
        dup2(RtoA[0], STDIN_FILENO);
        close(RtoA[0]);
        close(RtoA[1]);

        dup2(A1BtoA2[1], STDOUT_FILENO);
        close(A1BtoA2[0]);
        close(A1BtoA2[1]);

        char *argv1[3];

        argv1[0] = (char *)"python";// it is just a program name
        argv1[1] = (char *)"GenVEFromRgen.py";
        argv1[2] = nullptr;

        return execvp("python", argv1);
    }
    kids.push_back(child_pid);

    // conncet a1 to a2
    child_pid = fork();
    if(child_pid == 0)
    {
        dup2(A1BtoA2[0], STDIN_FILENO);
        close(A1BtoA2[0]);
        close(A1BtoA2[1]);

        int a2 = execv("SPandVC", argv);
        return a2;
    }
    kids.push_back(child_pid);
    // keyboard to a2
    dup2(A1BtoA2[1], STDOUT_FILENO);
    close(A1BtoA2[0]);
    close(A1BtoA2[1]);
    int res = procCin();

    // send kill signal to all children
    for(pid_t k : kids)
    {
        int status;
        kill(k, SIGTERM);
        waitpid(k, &status, 0);
    }
    return res;
}
