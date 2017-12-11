# Project_VC_SP
github: https://github.com/luciferxiaozhi/Project_VC_SP
## 1. Introduction
The project is to help the local police department with their installation of security cameras at traffic intersections. It solve a particular kind of optimization problem, called the **Vertex Cover** problem. The idea is for the police to be able to minimize the number of cameras they need to install, and still be as effective as possible with their monitoring. It uses **BFS** algorithm to solve a shortest path problem as well.
### 1.1 General structure
It generates several commands at the beginning from random generator, then the Python script will read these commands and generate a graph. After that, another program will read the graph and calculate the vertex cover of the graph in 3 different ways. It achieve information transfermation through **pipe**. Meanwhile, it can also generate shortest path from one vertex to another if user enters a certain command. The Driver has the overall contral of **.cpp** and **.py** files. The project achieves concurrently executing files by **multi-process** and **multi-threads**. 
### 1.2 Files include

1. Driver.cpp
2. rgen.cpp
3. GenVEFromRgen.py, compareStreet.py, intersection.py
4. SPandVC.cpp
5. directory: minisat
6. CMakeList.txt

### 1.3 Compile steps

1. Open terminal from Linux.
2. Clone the whole repo from https://github.com/luciferxiaozhi/Project_VC_SP
3. Create a directory, called 'build', by 'mkdir build', then 'cd build'.
4. Create makefiles and compile by cmake. Using 'cmake ../' and 'make install'.
5. Use 'cd run/bin/' and run our file Driver by using './Driver'

## 2. Features

### 2.1 Driver

The driver is the beginning launcher in the project. 

It has the overall contral. There are three programs that run concurrently:(1) the random generator, (2) the Python script (3) the program that takes as input a graph-specification and computes vertex cover and shortest paths. The parent process is for reading some commands from keyboard. We use **fork()** to create child process and **pipe()** to exchange information between processes or files. After we enter EOF, the program will stop executing and quit.

### 2.2 Random input generator

The random input generator **rgen** generate random inputs of street specifications for the Python script. It takes four command-line arguments. All are optional.
1. -s k —— where k is an integer >= 2. The number of streets is a random integer in [2;k]. If this option is not specied, it will use a default of k = 3; that is, the number of streets is a random integer in [2,3].
2. -n k —— where k is an integer >= 1. The number of line-segments in each street is a random integer in [1,k]. Default: k = 3.
3. -l k —— where k is an integer >= 5. The process wait a random number w seconds, where w is in [5,k] before generating the next (random) input.  Default: k = 5.
4. -c k —— where k is an integer >= 1. The process generate (x, y) coordinates such that every x and y value is in the range [-k,k]. For example, if k = 15, all of the coordinate values should be integers between 15 and 15.  Default: k = 8.

Also, the input does not have errors. For example, if it generates a line-segment that overlaps with a line-segment (across all streets) generated earlier, it will regenerate that line-segment. Similarly, it will not have any zero-length line segments. 

The random generator will not go into a infinite loop looking for a valid specification, because it fails to generate a valid specification for a continuous 5000 number of attempts, it will **exit()** with an error message reported on stderr.

The file uses **/dev/urandom** as the source of the random data.

### 2.3 Python scripts
For this program, it takes as input a series of commands that describe streets. And it uses that input to construct a particular kind of undirected graph.

The Python scripts in our project includes 3 files, 'GenVEFromRgen.py', 'compareStreet.py' and 'intersection.py'. 'compareStreet.py' and 'intersection.py' includes severval functions that we call in 'GenVEFromRgen.py'.

#### 2.3.1 Sample Input
The input comprises lines each of which specified a command. There are 4 kinds of commands. (1) add a street, (2) change a street, (3) remove a street, and (4) generate a graph. Here is an example:

```
a "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)
a "King Street S" (4,2) (4,8)
a "Davenport Road" (1,4) (5,8)
g
V 9
E {<0,2>,<1,2>,<2,3>,<2,5>,<6,5>,<5,4>,<8,5>,<5,7>,<5,9>}
c "Weber Street" (2,1) (2,2)
g
V 5
E {<0,2>,<2,1>,<2,3>,<2,4>}
r "King Street S"
g
V 0
E {}
```

When parsing input line, the program uses **regular expression**, in order to find the key content in the input.

#### 2.3.2 Commands
1. **a** is used to add a street. It is specified as: "**a "Street Name" (x1,y1) (x2,y2)...(xn,yn)**". Each (xi,yi) is a GPS coordinate.
2. **c** is used to change the specification of a street.
3. **r** is used to remove a street. It is specified as "**r "Street Name"**".
4. **g** causes the program to output the corresponding graph.

#### 2.3.3 Errors
The program accepts errors in the input. If a line in the input is erroneous, it will immediately output an error message. The program can also recover from from the error as well. That is, the program reject the erroneous line, but continue to accept input.

### 2.4 Vertex Cover & Shortest Path
#### 2.4.1 muti-thread
In this program, I have created 4 threads.
1. I/O and shortest path.
2. CNF-SAT-VC.
3. APPROX-VC-1.
4. APPROX-VC-2.
The first thread is for I/O and calculating shortest path. The other 3 threads are for computing vertex cover in 3 different methods.

#### 2.4.2 I/O & shortest path

##### I/0
Here is an example for input and output:
```
V 5
E {<2,1>,<2,0>,<2,3>,<1,4>,<4,3>}
CNF-SAT-VC: 2,4
APPROX-VC-1: 2,4
APPROX-VC-2: 0,2,3,4
s 2 4
2-1-4
```

##### Shortest Path
The program uses **BFS algorithm** to find the shortest path of input graph.

#### 2.4.3 Vertex Cover
##### APPROX-VC-1
Pick a vertex of highest degree (most incident edges). Add it to vertex cover and throw away all edges incident on that vertex. Repeat until no edges remain. We call this algorithm **APPROX-VC-1**.
##### APPROX-VC-2
Pick an edge <u,v>, and add both u and v to your vertex cover. Throw away all edges attached to u and v. Repeat till no edges remain. We call this algorithm **APPROX-VC-2**.
##### CNF-SAT-VC
I created a polynomial reduction of the decision version of **VERTEX COVER** to **CNF-SAT**. The program implement the reduction and use **MiniSAT solver** as a library to solve the **minimum VERTEX COVER** problem for the graph that are input to the program.

The reduction consists of the following rules:
1. At least one vertex is the ith vertex in the vertex cover.
2. No one vertex can appear twice in a vertex cover.
3. No more than one vertex appears in the same position of the vertex cover.
4. Every edge is incident to at least one vertex in the vertex cover.

**Attention:**

This method takes exponential time as the size of input, so if the size of V is large, it will take a lot of time to compute. However, it is guaranteed to be optimal.

### 2.5 MiniSAT Solver
MiniSAT is one of the most famous modern SAT-solvers.
1. written in C++.
2. designed to be easily understandable and customizable.
3. many new SAT-solvers use MiniSAT as their base.

It takes as input CNF clauses, and output SAT/UNSAT. If satisfiable, it will also output a satisfying assignment.

## 3. References
1. Resources from Learn provided by Professor Gurfinkel.
2. [MiniSAT solver from Arie Aguifinkel's github](https://github.com/agurfinkel/minisat)
3. Web page: http://minisat.se/