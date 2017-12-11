#include <iostream>
// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"
#include <sstream>
#include <vector>
#include <queue>
#include <exception>
#include <algorithm>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MAX 0xFFFFFFFF

int countVec = 0;
// vertex and edge structures(V & E) definition
struct V
{
      unsigned order;
      unsigned color;
      unsigned d;
      V *pParentVertex;
};

// typedef
typedef std::vector<std::vector<V*> > Edgetype;
typedef std::vector<std::vector<V*> > AdjType;

// class 'Graph' definition
class Graph
{   
    private:
        std::vector<int> printVector_CNF;
        std::vector<int> printVector_APPROX1;
        std::vector<int> printVector_APPROX2;
        unsigned maxVertex;
        std::vector<V*> verticesVector;
        Edgetype edgesVector;
        
    public:
        AdjType Adj;
        int VEmpFlag = 0;
        int EEmpFlag = 0;
        Edgetype connect(Edgetype, unsigned, unsigned);
        void setVertices(unsigned);
        void setEdges(Edgetype);
        void BFS(unsigned);
        void printResult(unsigned,unsigned);
        bool isOutOfRange(unsigned, unsigned);
        bool isEmpty(void);
        void vertexCover(void);
        void Approx1(void);
        void Approx2(void);
        void printVC(void);
        ~Graph();
};

struct thread_val
{
    Graph Database;
};

Graph::~Graph()
{
    for(int i = 0; i < this->verticesVector.size(); i++)
    {
        delete this->verticesVector[i];
    }
}

// inline functions
void Graph::setVertices(unsigned vertexMaxNum)
{
    
    //clear verticesVector
    for(int i = 0; i < this->verticesVector.size(); i++)
    {
        delete this->verticesVector[i];
    }
    this->verticesVector.clear();
    //clear edgesVector
    this->edgesVector.clear();
    //clear Adj
    this->Adj.clear();

    this->maxVertex = vertexMaxNum;
    //initail verticesVector
    for(unsigned i = 0; i < this->maxVertex;i++)
    {
        V* vertexTmp;
        vertexTmp = new V;
        vertexTmp->order = i;
        this->verticesVector.push_back(vertexTmp);
    }
}

void Graph::setEdges(Edgetype Edges)
{
    // set edges
    this->edgesVector = Edges;
    // set Adj
    V *ver1,*ver2;
    this->Adj.resize(this->maxVertex);//set A to a vector with maxVertex number elements.
    
    for(int i = 0; i < this->edgesVector.size(); i++)
    {
        
        ver1 = this->edgesVector[i][0];
        ver2 = this->edgesVector[i][1];
        this->Adj[ver1->order].push_back(ver2);
        this->Adj[ver2->order].push_back(ver1);
    }

    //print graph
    std::cout << "V " << this->maxVertex << std::endl;
    
    std::cout << "E {";
    if(Edges.size()!=0)
    {
        for(int i = 0; i < Edges.size() - 1;i++)
        {
            std::cout << "<" << Edges[i][0]->order << "," << Edges[i][1]->order << ">" << ",";
        }
        int i = Edges.size() - 1;
        std::cout << "<" << Edges[i][0]->order << "," << Edges[i][1]->order << ">";
    }
    std::cout << "}" << std::endl;
}

Edgetype Graph::connect(Edgetype Edges, unsigned ver1, unsigned ver2)
{
    V *v1, *v2;
    std::vector<V*> oneEdge;
    for(int i = 0 ; i < this->verticesVector.size();i++)
    {
        if(ver1 == verticesVector[i]->order)
        {
            v1 = verticesVector[i];
        }
        if(ver2 == verticesVector[i]->order)
        {
            v2 = verticesVector[i];
        }
    }
    oneEdge.push_back(v1);
    oneEdge.push_back(v2);
    Edges.push_back(oneEdge);
    return Edges;
}


//white = 0, gray = 1, black = 2
void Graph::BFS(unsigned rootOrder)
{
    // initialization
    for(int i = 0; i < this->verticesVector.size();i++)
    {
        // root vertex
        if(i == (int)rootOrder)
        {
            this->verticesVector[i]->color = 1;
            this->verticesVector[i]->d = 0;
            this->verticesVector[i]->pParentVertex = 0;
        }
        else// other vertices
        {
            this->verticesVector[i]->color = 0;
            this->verticesVector[i]->d = MAX;
            this->verticesVector[i]->pParentVertex = 0;
        }
    }
    std::queue<V*> QuereBFS;
    QuereBFS.push(this->verticesVector[rootOrder]);
    V* u;
    while(!QuereBFS.empty())
    {
        u = QuereBFS.front();
        QuereBFS.pop();
        for (int i = 0; i < this->Adj[u->order].size();i++)
        {
            if(this->Adj[u->order][i]->color == 0)
            {
                this->Adj[u->order][i]->color = 1;
                this->Adj[u->order][i]->d += 1;
                this->Adj[u->order][i]->pParentVertex = u;
                QuereBFS.push(this->Adj[u->order][i]);
            }
        }
        u->color = 2;
    }
}

void Graph::printResult(unsigned rootOrder,unsigned pointOrder)
{
    
    if(rootOrder == pointOrder)
    {
        std::cout<< rootOrder << std::endl;
        return;
    }


    V *Tmp;
    std::vector<unsigned> result;
    for(int i = 0; i < this->verticesVector.size(); i++)
    {
        if(pointOrder == this->verticesVector[i]->order)
        {
            Tmp = this->verticesVector[i];
        }
    }
    
    while (Tmp->pParentVertex != NULL)
    {
        result.push_back(Tmp->order);
        Tmp = Tmp->pParentVertex;
    }
    result.push_back(Tmp->order);
    int theLastIndex = result.size()-1;
    if(result[theLastIndex]!=rootOrder)
    {
        throw "the path does not exsit.";
        return;
    }
    else
    {
        for(int i = result.size()-1; i > 0; i--)
        {
            std::cout << result[i] <<'-';
        }
        if(result.size()!=1)
        {
            std::cout << result[0] << std::endl;
        }
    }
}

bool Graph::isOutOfRange(unsigned ver1, unsigned ver2)
{
    if(ver1 < 0 || ver2 >= this->maxVertex || ver2 < 0 || ver1 >= this->maxVertex)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool Graph::isEmpty()
{
    if(this->EEmpFlag && this->VEmpFlag)
    {
        return false;
    }
    return true;
}

void Graph::vertexCover()// k refers to the number of vertex
{   
    // -- allocate on the heap so that we can reset later if needed
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    int k = 0;
    int n = this->maxVertex;
    bool res = false;
    k += 1;
    while(k <= this->maxVertex)
    {   

        // create & initialize x
        std::vector<std::vector<Minisat::Lit> > x(n);

        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < k; j++)
            {
                Minisat::Lit l = Minisat::mkLit(solver->newVar());
                x[i].push_back(l);
            }
        }

        // 1. At least one vertex is the ith vertex in the vertex cover
        for(int i = 0; i < k; i++)
        {
            Minisat::vec<Minisat::Lit> tmpVec;
            for(int j = 0; j < n; j++)
            {
                tmpVec.push(x[j][i]);
            }
            solver->addClause(tmpVec);
            tmpVec.clear();
        }
        // 2. No one vertex can appear twice in a vertex cover
        for(int m = 0; m < n; m++)
        {
            for(int p = 0; p < k - 1; p++)
            {
                for(int q = p+1; q < k; q++)
                {
                    solver->addClause(~x[m][p], ~x[m][q]);
                }
            }
        }

        // 3. No more than one vertex appears in the mth position of the vertex cover.
        for(int m = 0; m < k; m++)
        {
            for(int p = 0; p < n - 1; p++)
            {
                for(int q = p + 1; q < n; q++)
                {
                    solver->addClause(~x[p][m], ~x[q][m]);
                }
            }
        }
        // 4. Every edge is incident to at least one vertex in the vertex cover
        
        for(int i = 0; i < this->edgesVector.size(); i++)
        {
            Minisat::vec<Minisat::Lit> tmpVec;
            for(int m = 0; m < k; m++)
            {
                tmpVec.push(x[this->edgesVector[i][0]->order][m]);
                tmpVec.push(x[this->edgesVector[i][1]->order][m]);
            }
            solver->addClause(tmpVec);
            tmpVec.clear();
        }
        // if UNSAT res == 0, if SAT res == 1
        res = solver->solve();
        if(res)
        {
            // Find vertex cover           
            for(int i = 0; i < n; i++)
            {
                for(int j = 0; j < k; j++)
                {
                    if(Minisat::toInt(solver->modelValue(x[i][j])) == 0)
                    {
                        this->printVector_CNF.push_back(i);
                    }
                }
            }
            //sort
            std::sort(this->printVector_CNF.begin(), this->printVector_CNF.end(), std::less<int>());
            countVec++;
            solver.reset (new Minisat::Solver());
            break;
        }
        x.clear();
        solver.reset (new Minisat::Solver());
        k++;
    }
}

void Graph::Approx1()
{
    Edgetype edgesVector_approx1 = this->edgesVector;
    AdjType Adj_approx1 = this->Adj;
    /*
    // print edges
    for(int i = 0; i < edgesVector_approx1.size(); i++)
    {
        std::cout<<"(" << edgesVector_approx1[i][0]->order << "," << edgesVector_approx1[i][1]->order << ")" << std::endl;
    }
    */
    
    while(!edgesVector_approx1.empty())
    {
        int max_index = 0;
        // find the highest degree vertex
        int max_Val = Adj_approx1[0].size();
        for(int i = 0; i < Adj_approx1.size(); i++)
        {
            if(Adj_approx1[i].size() > max_Val)
            {
                max_Val = Adj_approx1[i].size();
                max_index = i;
            }
        }
        
        // Add to vertex cover
        this->printVector_APPROX1.push_back(max_index);
        // delete vertices adjacent to max_index vertex in adj
        Adj_approx1[max_index].clear();
        for(int i = 0; i < Adj_approx1.size();i++)
        {
            for(int j = Adj_approx1[i].size() - 1; j >= 0;)
            {
                if(Adj_approx1[i][j]->order == max_index)
                {
                    Adj_approx1[i].erase(Adj_approx1[i].begin()+j);
                }
                j--;
            }
        }

        // delete edges which connect to that vertex
        for(int i = edgesVector_approx1.size()-1; i >= 0 ;)
        {
            if(edgesVector_approx1[i][0]->order == max_index || edgesVector_approx1[i][1]->order == max_index)
            {
                edgesVector_approx1.erase(edgesVector_approx1.begin()+i);
            }
            i--;
        }
    }
    //sort
    std::sort(this->printVector_APPROX1.begin(), this->printVector_APPROX1.end(), std::less<int>());
    countVec++;
}

void Graph::Approx2()
{
    Edgetype edgesVector_approx2 = this->edgesVector;
    AdjType Adj_approx2 = this->Adj;

    while(!edgesVector_approx2.empty())
    {
        // pick an edge
        int u = edgesVector_approx2[0][0]->order;
        int v = edgesVector_approx2[0][1]->order;

        // Add to vertex cover.
        this->printVector_APPROX2.push_back(u);
        this->printVector_APPROX2.push_back(v);

        // delete edges which connect to that vertex
        for(int i = edgesVector_approx2.size()-1; i >= 0 ;)
        {
            if(edgesVector_approx2[i][0]->order == u || edgesVector_approx2[i][1]->order == u || edgesVector_approx2[i][0]->order == v || edgesVector_approx2[i][1]->order == v)
            {
                edgesVector_approx2.erase(edgesVector_approx2.begin()+i);
            }
            i--;
        }
    }
    //sort
    std::sort(this->printVector_APPROX2.begin(), this->printVector_APPROX2.end(), std::less<int>());
    countVec++;
}

void Graph::printVC()
{
    //print result CNF
    std::cout << "CNF-SAT-VC: ";
    for(int i = 0; i < this->printVector_CNF.size() - 1; i++)
    {
        std::cout << this->printVector_CNF[i] << ",";
    }
    std::cout << this->printVector_CNF[this->printVector_CNF.size() - 1]<< std::endl;

    //print result APPROX1
    std::cout << "APPROX-VC-1: ";
    for (int i = 0; i < this->printVector_APPROX1.size() - 1; i++)
    {
        std::cout << this->printVector_APPROX1[i] << ",";
    }
    std::cout << this->printVector_APPROX1[this->printVector_APPROX1.size()-1] << std::endl;

    //print result APPROX2
    std::cout << "APPROX-VC-2: ";
    for (int i = 0; i < this->printVector_APPROX2.size() - 1; i++)
    {
        std::cout << this->printVector_APPROX2[i] << ",";
    }
    std::cout << this->printVector_APPROX2[this->printVector_APPROX2.size()-1] << std::endl;

    // reinitialize
    this->printVector_CNF.clear();
    this->printVector_APPROX1.clear();
    this->printVector_APPROX2.clear();
}


void *Thread1_IO(void *arg)
{
    struct thread_val* p = (struct thread_val*) arg;

    // Variables definition
    char cmd = '0';
    unsigned vertexMaxNum;
    std::string edgesString;
    int VcmdFlag = 0;
    int EcmdFlag = 0;
    int VEflag = 0;
    int vStart = 0;
    int vEnd = 0;

    // output
    if(!p->Database.isEmpty())
    {
        while(true)
        {
            if(countVec == 3)
            {
                countVec = 0;
                break;
            }
        }
        p->Database.printVC();
    }
    // input
    while(VEflag == 0)
    {
        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);
        std::istringstream input(line);

        Edgetype Edges;
        if(std::cin.eof())
        {
            exit(0);
        }
        // while there are characters in the input line
        while (!input.eof()) 
        {
            // read the first character
            char character;
            input >> character;
            if (input.fail())
            {
                break;
            }
            else
            {
                cmd = character;
            }
            try
            {
                switch(cmd)
                {
                    case 'V':
                        input >> vertexMaxNum;
                        if(input.fail())
                        {
                            throw "vertexMaxNum should be an unsigned integar.";
                            break;
                        }
                        p->Database.setVertices(vertexMaxNum);
                        VcmdFlag = 1;
                        p->Database.VEmpFlag = 1;
                        break;
                    case 'E':
                        {
                            input >> edgesString;

                            std::istringstream EInput(edgesString);
                            char c;
                            unsigned ver1,ver2;
                            int breakFlag = 0;

                            EInput >> c;// read '{'

                            while(c != '}')
                            {
                                EInput >> c;// read '<'
                                if(c!='}')
                                {
                                    EInput >> ver1;// read the first vertex
                                    EInput >> c;// read ','
                                    EInput >> ver2;// read the second vertex
                                    EInput >> c;// read '>'
                                    EInput >> c;// read ',' or '}'

                                    //to determine whether the point order is out of range
                                    if(p->Database.isOutOfRange(ver1,ver2))
                                    {
                                        throw "Point order out of range.";
                                        break;
                                    }

                                    Edges = p->Database.connect(Edges,ver1,ver2);
                                }
                                else
                                {
                                    breakFlag = 1;
                                    break;
                                }
                            }
                            if(breakFlag == 1)
                            {
                                p->Database.EEmpFlag = 1;
                                break;
                            }
                            p->Database.setEdges(Edges);
                            EcmdFlag = 1;
                            p->Database.EEmpFlag = 1;
                            break;
                        }
                        case 's':
                        {
                            input >> vStart ;
                            input >> vEnd;
                            //to determine whether the point order is out of range
                            if(p->Database.isOutOfRange(vStart,vEnd))
                            {
                                throw "Point order out of range.";
                                break;
                            }
                            p->Database.BFS(vStart);
                            p->Database.printResult(vStart,vEnd);
                            break;
                        }    
                    default:
                        break;
                }
            }
            catch(const char* msg)
            {
                std::cerr<< "Error: " << msg << std::endl;
            }
            catch(...)
            {
                std::cerr<< "Error: error happens in other cases."<<std::endl;
            }
        }

        // if read V and E
        if(VcmdFlag && EcmdFlag)
        {
            VEflag = 1;
        }
    }
    return NULL;
}
void *Thread2_CNF(void *arg)
{
    struct thread_val* p = (struct thread_val*) arg;
    if(p->Database.isEmpty())
    {
        return NULL;
    }

    // call CNF method
    p->Database.vertexCover();
    return NULL;
}

void *Thread3_APPROX1(void *arg)
{
    struct thread_val* p = (struct thread_val*) arg;
    if(p->Database.isEmpty())
    {
        return NULL;
    }
    // call APPROX1 method
    p->Database.Approx1();
    return NULL;
}

void *Thread4_APPROX2(void *arg)
{
    struct thread_val* p = (struct thread_val*) arg;
    if(p->Database.isEmpty())
    {
        return NULL;
    }
    // call APPROX2 method
    p->Database.Approx2();
    return NULL;

}

int main(int argc, char** argv) {

    struct thread_val ourDatabase;

    int retcode;
    pthread_t thread_1;
    pthread_t thread_2;
    pthread_t thread_3;
    pthread_t thread_4;

    while(true)
    {   
        // Thread 2
        retcode = pthread_create(&thread_2, NULL, Thread2_CNF, (void *)&ourDatabase);
        if(retcode != 0)
        {
            std::cerr << "Error: Create pthread_2 error." << std::endl;
        }
        

        // Thread 3
        retcode = pthread_create(&thread_3, NULL, Thread3_APPROX1, (void *)&ourDatabase);
        if(retcode != 0)
        {
            std::cerr << "Error: Create pthread_3 error." << std::endl;
        }
        

        // Thread 4
        retcode = pthread_create(&thread_4, NULL, Thread4_APPROX2, (void *)&ourDatabase);
        if(retcode != 0)
        {
            std::cerr << "Error: Create pthread_4 error." << std::endl;
        }
        

        // I/O thread
        retcode = pthread_create(&thread_1, NULL, Thread1_IO, (void *)&ourDatabase);
        if(retcode != 0)
        {
            std::cerr << "Error: Create pthread_IO error." << std::endl;
        }
        pthread_join(thread_1, NULL);
        pthread_join(thread_2, NULL);
        pthread_join(thread_3, NULL);
        pthread_join(thread_4, NULL);
    }
    return 0;
}
