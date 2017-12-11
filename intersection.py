class Point(object):
    def __init__ (self, x, y):
        self.x = float(x)
        self.y = float(y)
    def __str__ (self):
        if self.x == round(self.x) and self.y == round(self.y):
            return '(' + str(int(self.x)) + ',' + str(int(self.y)) + ')'
        elif self.x == round(self.x) and self.y != round(self.y):
            return '(' + str(int(self.x)) + ',' + str("{0:.2f}".format(self.y)) + ')'
        elif self.x != round(self.x) and self.y == round(self.y):
            return '(' + str("{0:.2f}".format(self.x)) + ',' + str(int(self.y)) + ')'
        elif self.x != round(self.x) and self.y != round(self.y):
            return '(' + str("{0:.2f}".format(self.x)) + ',' + str("{0:.2f}".format(self.y)) + ')'
        else:
            return "Error: never happend"
class Line(object):
    def __init__ (self, src, dst):
        self.src = src
        self.dst = dst

    def __str__(self):
        return str(self.src) + '-->' + str(self.dst)

def isBetween(Line, c):
    x1, y1 = Line.src.x, Line.src.y
    x2, y2 = Line.dst.x, Line.dst.y
    x0, y0 = c.x, c.y

    lineNum = y2 - y1
    lineDen = x2 - x1
    cNum = y0 - y1
    cDen = x0 - x1
    ymax = max(y1,y2)
    ymin = min(y1,y2)
    xmax = max(x1,x2)
    xmin = min(x1,x2)

    if lineDen == 0 and cDen == 0 and y0 >= ymin and y0 <= ymax:
        return True
    elif lineDen == 0 and cDen == 0 and y0 < ymin and y0 > ymax:
        return False
    elif (x0 == x1 and y0 == y1) or (x0 == x2 and y0 == y2):
        return True
    elif (lineDen == 0 or cDen==0):
        return False
    else:
        lineCoor = lineNum / lineDen
        cCoor = cNum / cDen
        coorR = abs(lineCoor - cCoor)
        if (coorR<=0.001) and (x0 >= xmin and x0 <= xmax and y0 >= ymin and y0 <= ymax):
            return True
        else:
            return False

def isBetweenNoEqual(Line, c):
    x1, y1 = Line.src.x, Line.src.y
    x2, y2 = Line.dst.x, Line.dst.y
    x0, y0 = c.x, c.y

    lineNum = y2 - y1
    lineDen = x2 - x1
    cNum = y0 - y1
    cDen = x0 - x1
    ymax = max(y1,y2)
    ymin = min(y1,y2)
    xmax = max(x1,x2)
    xmin = min(x1,x2)
    if lineDen == 0 and cDen == 0 and y0 > ymin and y0 < ymax:
        return True
    elif lineDen == 0 or cDen==0:
        return False
    elif lineDen == 0 and cDen == 0 and y0 <= ymin and y0 >= ymax:
        return False
    else:
        lineCoor = lineNum / lineDen
        cCoor = cNum / cDen
        coorR = abs(lineCoor - cCoor)
        if (lineCoor == 0 and cCoor == 0) and (x0 > xmin and x0 < xmax):
            return True
        if ((coorR<=0.001) and (x0 > xmin and x0 < xmax and y0 > ymin and y0 < ymax)):
            return True
        else:
            return False


def Intersect (l1, l2):
    # need to add some conditions to jugde whether the intersection is correct
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y

    xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
    xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
    

    ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
    yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)

    if xden == 0 or yden == 0:
        return -1

    else:
        xcoor =  xnum / xden
        ycoor = ynum / yden


        if ((xcoor >= min(x1,x2)) and (xcoor <= max(x1,x2))) and ((ycoor >=min(y1,y2)) and (ycoor <= max(y1,y2))) and ((xcoor >= min(x3,x4)) and (xcoor <= max(x3,x4))) and ((ycoor >= min(y3,y4)) and (ycoor <= max(y3,y4))):
            return Point (xcoor, ycoor)
        else:
            return -1