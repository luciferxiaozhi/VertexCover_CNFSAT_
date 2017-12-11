//c++ rgen.cpp -std=c++11 -o rgen
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

struct Vertex
{
    int x;
    int y;
};

struct Edge
{
    Vertex src;
    Vertex dst;
};

double max(double,double);
double min(double,double);
double abs(double);
bool isOverlap(Edge,Edge);


int main(int argc, char **argv)
{
    /// variables
    int cmd = 0;
    std::string svalue;
    std::string nvalue;
    std::string lvalue;
    std::string cvalue;
    

    // do not print default error message
    opterr = 0;

    // sNum: the number of streets, [2,k], default 4.
    unsigned int sNum = 3;
    // nNum: The number of line-segments, [1,k], default 3.
    unsigned int nNum = 3;
    // lNum: waited second before next input, [5,k], default 5
    unsigned int lNum = 5;
    // cNum: range of (x,y), [-k,k], default 8.
    int cNum = 8;
    // place to save street names.
    std::vector<std::string> streetName;



    // expected options are '-s value', '-n value', '-l value', '-c value'
    while((cmd = getopt (argc, argv, "s:n:l:c:")) != -1)
    {
        switch (cmd)
        {
            case 's':
                svalue = optarg;
                if(atoi(svalue.c_str())<2)
                {
                    std::cerr << "Error: option s -> the argument value cannot be less than 2." << std::endl;
                    exit(1);
                }
                sNum = atoi(svalue.c_str());
                break;
            case 'n':
                nvalue = optarg;
                if(atoi(nvalue.c_str())<1)
                {
                    std::cerr << "Error: option n -> the argument value cannot be less than 1."<< std::endl;
                    exit(1);
                }
                nNum = atoi(nvalue.c_str());
                break;
            case 'l':
                lvalue = optarg;
                if(atoi(lvalue.c_str()) < 5)
                {
                    std::cerr << "Error: option l -> the argument value cannot be less than 5." << std::endl;
                    exit(1);
                }
                lNum = atoi(lvalue.c_str());
                break;
            case 'c':
                cvalue = optarg;
                cNum = atoi(cvalue.c_str());
                break;
            case '?':
                if (optopt == 's' || optopt == 'n' || optopt == 'l' || optopt == 'c')
                {
                    std::cerr << "Error: option -" << char(optopt) << " requires an argument." << std::endl;
                    exit(1);
                }
                else
                {
                    std::cerr << "Error: unknown option: -" << char(optopt) << std::endl;
                    exit(1);
                }
                return 1;
            default:
                return 0;
        }
    }


    // if the value is not followed by cmd.
    if (optind < argc) 
    {
        std::cerr << "Error: Found positional argument. " << "Non-option argumen: ";
        for(int index = optind; index < argc; index++)
            std::cerr  << argv[index] << "  ";
        std::cerr << std::endl;
        exit(1);
    }

    // open /dev/urandom to read
    std::ifstream urandom("/dev/urandom");
    if(urandom.fail())
    {
        std::cerr << "Error: cannot open /dev/urandom\n";
        exit(1);
    }

    while(true)
    {
        // random waiting seconds
        unsigned int numSeconds = 1;
        urandom.read((char*)&numSeconds, sizeof(int));
        numSeconds = numSeconds % (lNum - 4);
        numSeconds += 5;

        /// cout r cmd line
        if(!streetName.empty())
        {
            for(int i = 0; i < streetName.size(); i++)
            {
                std::string rCMD = "r ";
                rCMD += streetName[i];
                std::cout << rCMD << std::endl;
            }
        }

        int numIdentical = 0;
        // clear streetName
        streetName.clear();
        /// generate random street name and save in streetName, e.g. "abcde street"
        unsigned int numST = 1;
        urandom.read((char*)&numST, sizeof(int));
        numST %= (sNum - 1);
        numST += 2;

        while(numST > 0)
        {
            // Variables definition
            std::string randStringVariable = "\"";

            // generate random street name(5 characters)
            for(int i = 0; i < 5; i++)
            {
                unsigned int randNum = 42;
                char lowerChar = 97;
                urandom.read((char*)&randNum, sizeof(int));
                randNum = randNum % 26;
                lowerChar = lowerChar + randNum;
                randStringVariable.push_back(lowerChar);
            }
            randStringVariable += " street\"";
            if(streetName.size()!=0)
            {
                int flag = 0;
                for(int i = 0; i < streetName.size(); i++)
                {
                    if(randStringVariable == streetName[i])
                    {
                        numIdentical += 1;
                        flag = 1;
                        break;
                    }
                }
                /*
                if(numIdentical == 25)
                {
                    std::cerr << "Error: failed to generate valid input for 25 simultaneous attempts.\n";
                    break;
                }
                */
                if(flag == 1) continue;
            }
            streetName.push_back(randStringVariable);
            numST -= 1;
        }
        // if(numIdentical == 25) continue;
        numIdentical = 0;

        /// cout a cmd line
        for(int k = 0; k < streetName.size(); k++)
        {
            /// generate vertices and save in somewhere
            Vertex vTMP,vTMP1;
            Edge eTMP;
            std::vector<Vertex> veticesVector;
            std::vector<Edge> edgeVector;
            int lineidentical = 0;
            int countIden = 0;
            veticesVector.clear();
            edgeVector.clear();
            // give an random int number[-cNum, cNum]
            // push_back first vertex
            int signedRandNum = 23;
            urandom.read((char*)&signedRandNum, sizeof(int));
            signedRandNum %= (cNum + 1);
            vTMP.x = signedRandNum;
            urandom.read((char*)&signedRandNum, sizeof(int));
            signedRandNum %= (cNum + 1);
            vTMP.y = signedRandNum;
            veticesVector.push_back(vTMP);
            eTMP.src = vTMP;

            // push_back second vertex. if it equals the first one, do it again.
            do
            {
                urandom.read((char*)&signedRandNum, sizeof(int));
                signedRandNum %= (cNum + 1);
                vTMP1.x = signedRandNum;
                urandom.read((char*)&signedRandNum, sizeof(int));
                signedRandNum %= (cNum + 1);
                vTMP1.y = signedRandNum;
                countIden += 1;
            }while(vTMP.x == vTMP1.x && vTMP.y == vTMP1.y);
            lineidentical = lineidentical + countIden - 1;
            veticesVector.push_back(vTMP1);
            eTMP.dst = vTMP1;
            edgeVector.push_back(eTMP);

            // calculate nrand
            unsigned int nrand = 1;
            urandom.read((char*)&nrand, sizeof(int));
            nrand %= (nNum + 1);
            nrand += 1;


            int nNumLoop = nrand - 2;
            int i = 1;
            while(nNumLoop > 0)
            {
                int flagIntersect = 0;
                urandom.read((char*)&signedRandNum, sizeof(int));
                signedRandNum %= (cNum + 1);
                vTMP.x = signedRandNum;
                urandom.read((char*)&signedRandNum, sizeof(int));
                signedRandNum %= (cNum + 1);
                vTMP.y = signedRandNum;
                eTMP.dst = vTMP;
                eTMP.src = veticesVector[i];

                
                // if vTMP == some vertices -> flag = 1
                for(int j = 0;j < veticesVector.size(); j++)
                {
                    if(vTMP.x == veticesVector[j].x && vTMP.y == veticesVector[j].y)
                    {
                        flagIntersect = 1;
                    }
                }
                // if overlap -> flag = 1
                // compare eTMP with edges except last edge
                for(int j = 0;j < edgeVector.size() - 1; j++)
                {
                    if(isOverlap(eTMP, edgeVector[j]))
                    {
                        flagIntersect = 1;
                    }
                }
                // compare eTMP with last edge
                int end = edgeVector.size() - 1;
                double l1num,l1den,l2num,l2den;
                l1num = double(eTMP.dst.y - eTMP.src.y);
                l1den = double(eTMP.dst.x - eTMP.src.x);
                l2num = double(edgeVector[end].dst.y - edgeVector[end].src.y);
                l2den = double(edgeVector[end].dst.x - edgeVector[end].src.x);
                if((l1den == 0) && (l2den == 0))
                {
                    flagIntersect = 1;
                }
                else if(abs(l1num/l1den - l2num/l2den) <= 0.001)
                {
                    flagIntersect = 1;
                }

                if(lineidentical == 5000)
                {
                    std::cerr << "Error: failed to generate valid input for 5000 simultaneous attempts.\n";
                    exit(1);
                }

                if(flagIntersect == 1)
                {
                    lineidentical += 1;
                    continue;
                }
                else
                {
                    edgeVector.push_back(eTMP);
                    veticesVector.push_back(vTMP);
                }

                i++;
                nNumLoop -= 1;
            }
            std::cout << "a " << streetName[k]<<" ";
            for(int j = 0; j < veticesVector.size(); j++)
            {
                std::cout << "(" << veticesVector[j].x << "," << veticesVector[j].y << ")";
            }
            std::cout << std::endl;

        }
        /// cout g cmd line
        if(streetName.size()!=0)
        {
            std::cout << "g" << std::endl;
        }
        /// wait for some seconds and repeat next loop
        usleep(numSeconds*1000000);
    }
    // close random stream
    urandom.close();
    return 0;
}

double max(double a,double b)
{
    if(a>b) return a;
    else return b;
}
double min(double a,double b)
{
    if(a>b) return b;
    else return a;
}

double abs(double a)
{
    if(a > 0) return a;
    else return a*(-1);
}

bool isOverlap(Edge l1, Edge l2)
{
    double x1 = double(l1.src.x), y1 = double(l1.src.y);
    double x2 = double(l1.dst.x), y2 = double(l1.dst.y);
    double x3 = double(l2.src.x), y3 = double(l2.src.y);
    double x4 = double(l2.dst.x), y4 = double(l2.dst.y);
    double xnum,xden,ynum,yden;

    xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4));
    xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4));

    ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4);
    yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);

    if(xden == 0 || yden == 0)
    {
        return false;
    }
    else
    {
        double xcoor,ycoor;
        xcoor = xnum / xden;
        ycoor = ynum / yden;

        if(((xcoor >= min(x1,x2)) && (xcoor <= max(x1,x2))) && ((ycoor >=min(y1,y2)) && (ycoor <= max(y1,y2))) && ((xcoor >= min(x3,x4)) && (xcoor <= max(x3,x4))) && ((ycoor >= min(y3,y4)) && (ycoor <= max(y3,y4))))
        {
            return true;
        }
        else
        {
            return false;
        }
        
    }
}