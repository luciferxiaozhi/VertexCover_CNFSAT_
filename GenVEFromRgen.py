import sys
import re
from compareStreet import *
from intersection import *

# Class definition
class Street(object):
    
    def __init__(self):
        self.streetName = []
        self.pointsArray = []

    def AddStreet(self, streetName, pointsArray):
        for i in range(0, len(self.streetName)):
            if self.streetName[i].lower() == streetName.lower():
                raise Exception("Street has exsited. Please try again.")
        self.streetName.append(streetName.lower())
        self.pointsArray.append(pointsArray)
    
    def ChangeStreet(self, streetName, pointsArray):
        self.flagSameName = 0
        for self.element in self.streetName:
            if self.element.lower() == streetName.lower():
                self.flagSameName = 1
                self.streetIndex = self.element.index(self.element)
                self.pointsArray[self.streetIndex] = pointsArray

        if self.flagSameName == 0:
            raise Exception("Wrong street name: No such street name in database.")

    def RemoveStreet(self, streetName):
        self.flagSameName = 0
        for self.element in self.streetName:
            if self.element.lower() == streetName.lower():
                self.flagSameName = 1
                self.streetIndex = self.streetName.index(self.element)
                self.streetName.pop(self.streetIndex)
                self.pointsArray.pop(self.streetIndex)

        if self.flagSameName == 0:
            raise Exception("Wrong street name: No such street name in database.")
        
    def GenerateGraph(self):
        self.lineStreetArray = []
        self.verticesArray = []
        self.edgesArray = []
        self.intersectionArray = []
        self.edgesArrayWithIndex = []
        
        # Save street lines in a array
        for i in range(0, len(self.pointsArray)):
            self.lineStreetArray.append([])
            for j in range(1, len(self.pointsArray[i])):
                self.thisLine = Line(self.pointsArray[i][j-1],self.pointsArray[i][j])
                self.lineStreetArray[i].append(self.thisLine)

        # Find intersections and save to intersectionArray and add to verticesArray
        for i in range(0, len(self.lineStreetArray)):
            for j in range(i+1, len(self.lineStreetArray)):
                self.verticesArray = CompareStreet(self.lineStreetArray[i],self.lineStreetArray[j],self.verticesArray)
                self.intersectionArray = CompareStreet(self.lineStreetArray[i],self.lineStreetArray[j],self.intersectionArray)
                self.edgesArray = FindEdges(self.lineStreetArray[i],self.lineStreetArray[j],self.edgesArray)


        # Special conditions for V  -- if two lines coincide with each other
        for i in range(0, len(self.lineStreetArray)):
            for j in range(i+1, len(self.lineStreetArray)):
                self.verticesArray = OverlapLine(self.lineStreetArray[i],self.lineStreetArray[j],self.verticesArray)
                self.intersectionArray = OverlapLine(self.lineStreetArray[i],self.lineStreetArray[j],self.intersectionArray)

        # remove the repetitive intersections
        for i in range(0, len(self.intersectionArray)):
            for j in list(reversed(range(i+1, len(self.intersectionArray)))):
                if self.intersectionArray[i] == self.intersectionArray[j]:
                    self.intersectionArray.pop(j)
        
        # save other cases to verticesArray
        for i in range(0, len(self.lineStreetArray)):
            for j in range(0, len(self.lineStreetArray[i])):
                for k in range(0, len(self.intersectionArray)):
                    if isBetween(self.lineStreetArray[i][j],self.intersectionArray[k]):
                        self.verticesArray.append(self.lineStreetArray[i][j].src)
                        self.verticesArray.append(self.lineStreetArray[i][j].dst)

        # remove the repetitive vertices
        for i in range(0, len(self.verticesArray)):
            for j in list(reversed(range(i+1, len(self.verticesArray)))):
                if self.verticesArray[i].x == self.verticesArray[j].x and self.verticesArray[i].y == self.verticesArray[j].y:
                    self.verticesArray.pop(j)
        
        print "V " + str(len(self.verticesArray))
        
        # delete single line in lineStreetArray
        intersectFlag = 0

        for i in range(0, len(self.lineStreetArray)):
            for l1 in list(reversed(range(0, len(self.lineStreetArray[i])))):
                for j in range(0, len(self.lineStreetArray)):
                    for l2 in range(0, len(self.lineStreetArray[j])):
                        if i == j:
                            continue
                        if isinstance(Intersect(self.lineStreetArray[i][l1],self.lineStreetArray[j][l2]), Point):
                            intersectFlag = 1
                        if isOverlapLine(self.lineStreetArray[i][l1], self.lineStreetArray[j][l2]):
                            intersectFlag = 1

                if intersectFlag == 0:
                    self.lineStreetArray[i].pop(l1)
                intersectFlag = 0

        # separate with intersection
        a = 0
        b = 0
        c = 0
        while c < len(self.verticesArray):
            while a < len(self.lineStreetArray):
                blen = len(self.lineStreetArray[a])
                while b < blen:
                    if isBetweenNoEqual(self.lineStreetArray[a][b], self.verticesArray[c]):
                        self.lineStreetArray[a].append(Line(self.lineStreetArray[a][b].src, self.verticesArray[c]))
                        self.lineStreetArray[a].append(Line(self.verticesArray[c],self.lineStreetArray[a][b].dst))
                        self.lineStreetArray[a].pop(b)
                        blen = len(self.lineStreetArray[a])
                        b = b - 1
                    b = b + 1
                b = 0
                a = a + 1
            a = 0
            c = c + 1


        linev1Flag = 0
        linev2Flag = 0
        for i in range(0, len(self.lineStreetArray)):
            for j in range(0, len(self.lineStreetArray[i])):
                for k in range(0, len(self.verticesArray)):
                    if self.lineStreetArray[i][j].src.x == self.verticesArray[k].x and self.lineStreetArray[i][j].src.y == self.verticesArray[k].y:
                        linev1Flag = 1
                        edgeindex1 = k+1
                    if self.lineStreetArray[i][j].dst.x == self.verticesArray[k].x and self.lineStreetArray[i][j].dst.y == self.verticesArray[k].y:
                        linev2Flag = 1
                        edgeindex2 = k+1
                if(linev1Flag == 1 and linev2Flag ==1):
                    self.edgesArrayWithIndex.append([str(edgeindex1),str(edgeindex2)])
                linev2Flag = 0
                linev1Flag = 0

        # sort edgesArrayWithIndex
        for i in range(0, len(self.edgesArrayWithIndex)):
            if int(self.edgesArrayWithIndex[i][0]) > int(self.edgesArrayWithIndex[i][1]):
                self.tmp = self.edgesArrayWithIndex[i][0]
                self.edgesArrayWithIndex[i][0] = self.edgesArrayWithIndex[i][1]
                self.edgesArrayWithIndex[i][1] = self.tmp

        # remove the repetitive lines
        for i in range(0,len(self.edgesArrayWithIndex)):
            for j in list(reversed(range(i+1,len(self.edgesArrayWithIndex)))):
                if int(self.edgesArrayWithIndex[i][0]) == int(self.edgesArrayWithIndex[j][0]) and int(self.edgesArrayWithIndex[i][1])==int(self.edgesArrayWithIndex[j][1]):
                    self.edgesArrayWithIndex.pop(j)
        
        for i in range(0, len(self.edgesArrayWithIndex)):
            self.edgesArrayWithIndex[i][0] = str(int(self.edgesArrayWithIndex[i][0]) - 1)
            self.edgesArrayWithIndex[i][1] = str(int(self.edgesArrayWithIndex[i][1]) - 1)

        sys.stdout.write("E {")
        for i in range(0, len(self.edgesArrayWithIndex)-1):
            tmp_str = '<'+str(self.edgesArrayWithIndex[i][0])+','+str(self.edgesArrayWithIndex[i][1])+'>,'
            sys.stdout.write(tmp_str)
        if len(self.edgesArrayWithIndex) != 0:
            i = len(self.edgesArrayWithIndex)-1
            tmp_str = '<'+str(self.edgesArrayWithIndex[i][0])+','+str(self.edgesArrayWithIndex[i][1])+'>'
            sys.stdout.write(tmp_str)
        sys.stdout.write('}\n')
        

# Function Definition
def ParseLine(Line):

    if(len(Line) == 1 and Line == '\n'):
        raise Exception("Wrong input -- \"Line Breaker\"")

    ### Variables definition
    stripedLine = Line.strip()
    # A list saves string num of all points
    strNumArray = []
    # A list saves float num of all points
    pointsArray = []

    stringMatch = []

    ### Parse the input line(using RE)
    ## Compare string

    if len(stripedLine) == 1 and stripedLine == 'a' or stripedLine == 'c' or stripedLine == 'r':
        raise Exception("Wrong input line")
    elif len(stripedLine) == 1:
        return stripedLine[0],-1,-1
    else:
        stringMatch_rx = re.compile(r'[a-z][ ]+\".+\"([ ]+([ ]*\([ ]*[-]?[0-9]+[ ]*,[ ]*[-]?[0-9]+[ ]*\))*)?')
        stringMatch = stringMatch_rx.match(stripedLine)
        flagMatch = 0
        if stringMatch == None:
            raise Exception("Wrong input line")
        else:
            if stringMatch.group() == stripedLine:
                    flagMatch = 1
            if flagMatch == 0:
                raise Exception("Wrong input line")
    
    ## Find street name and points
    streetName_rx = re.compile(r'\"[a-zA-z ]+\"');
    pointInStreet_rx = re.compile(r'\([ ]*[-]?[0-9]+[ ]*,[ ]*[-]?[0-9]+[ ]*\)')
    streetName = streetName_rx.findall(stripedLine)
    pointInStreet_unfixed = pointInStreet_rx.findall(stripedLine)
    strNumArray_rx = re.compile(r'[-]?[0-9]+')
    for element in pointInStreet_unfixed:
        strNumArray.append(strNumArray_rx.findall(element))
    
    # convert string to float
    for element in strNumArray:
         pointsArray.append(map(eval, element))

    for i in range(0, len(pointsArray)):
         pointsArray[i] = Point(pointsArray[i][0],pointsArray[i][1])

    if pointInStreet_unfixed == [] and len(stripedLine) != 1 and stripedLine[0] != 'r':
        raise Exception("Wrong input -- no point is included.")

    if pointInStreet_unfixed != [] and len(stripedLine) != 1 and stripedLine[0] == 'r':
        raise Exception("Wrong input -- should not include point.")

    if streetName == []:
        raise Exception("Wrong input -- Wrong street name")

    return stripedLine[0], pointsArray, streetName[0]

def main():

    databaseOfStreet = Street()
    ### sample code to <read from stdin.
    ### make sure to remove all spurious print statements as required
    ### by the assignment
    while True:
        try:
            line = sys.stdin.readline()
            if line == '':
                break
            cmd, pointsArray, streetName = ParseLine(line)
            if cmd == 'a':
                databaseOfStreet.AddStreet(streetName, pointsArray)
            elif cmd == 'c':
                databaseOfStreet.ChangeStreet(streetName, pointsArray)
            elif cmd == 'r':
                databaseOfStreet.RemoveStreet(streetName)
            elif cmd == 'g':
                databaseOfStreet.GenerateGraph()
            else:
                raise Exception('Unknown command.')
        except Exception as ex:
            sys.stderr.write('Error: '+str(ex)+'\n')

        sys.stdout.flush()
    # return exit code 0 on successful termination
    sys.exit(0)

if __name__ == '__main__':
    main()
