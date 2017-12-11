#include <iostream>
#include <unistd.h>
#include <vector>
#include <signal.h>

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

    for(pid_t k : kids)
    {
        int status;
        kill(k, SIGTERM);
    }
    return res;
}
/*
a "btklk street" (-3,-5)(4,-3)
a "twmoo street" (-1,2)(4,3)(-1,1)
a "swjbh street" (4,6)(-5,-5)(-1,6)
a "usfrz street" (1,1)(-4,6)(0,6)(4,0)(5,-3)(-1,-2)
a "vxybh street" (0,4)(0,-5)
a "ivotn street" (-3,-2)(-3,5)(5,6)(-3,-3)(-1,0)(-2,5)
a "egsso street" (6,-4)(-3,3)(5,-1)(-4,6)(4,0)
a "ynaza street" (-1,2)(-3,-1)(-4,1)(-3,6)(3,0)
a "zlxxo street" (3,0)(2,1)(-1,5)(-6,-4)(-3,1)



a "zamyk street" (20,-3)(18,6)
a "afxnl street" (15,19)(-11,-4)(-9,5)(-11,13)(-12,0)(-17,-1)(-5,-13)(-13,-11)
a "pvfvl street" (18,4)(3,2)(11,-5)(-8,4)(-11,12)
a "paoyu street" (17,15)(15,-5)(19,-20)(-8,-18)
a "zrmfq street" (16,2)(2,9)(-9,-8)(-13,4)
a "neulf street" (-6,15)(-20,-6)(8,-8)(-1,-6)(11,14)(-12,0)(9,17)(-8,3)
a "ksrun street" (3,6)(-14,-5)(-6,-19)(8,-8)(-12,-6)
a "zndku street" (-6,-4)(6,2)
a "trqrs street" (-17,4)(-16,-6)
a "wicsn street" (-13,14)(-1,9)(-4,-1)(14,14)

*/