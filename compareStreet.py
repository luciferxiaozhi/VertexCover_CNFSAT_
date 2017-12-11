from intersection import *

def CompareStreet(s1, s2, verticesArray):
    for i in range(0, len(s1)):
        for j in range(0, len(s2)):
            if isinstance(Intersect(s1[i],s2[j]),Point):
                verticesArray.append(Intersect(s1[i],s2[j]))
    return verticesArray

def FindEdges(s1, s2, edgesArray):
    for i in range(0, len(s1)):
        for j in range(0, len(s2)):
            if isinstance(Intersect(s1[i],s2[j]),Point):
                edgesArray.append(Line(Intersect(s1[i],s2[j]),s1[i].src))
                edgesArray.append(Line(Intersect(s1[i],s2[j]),s1[i].dst))
                edgesArray.append(Line(Intersect(s1[i],s2[j]),s2[j].src))
                edgesArray.append(Line(Intersect(s1[i],s2[j]),s2[j].dst))
    return edgesArray 

def OverlapLine(s1, s2, verticesArray):
    for i in range(0, len(s1)):
        for j in range(0, len(s2)):
            s21src = isBetweenNoEqual(s2[j],s1[i].src)
            s12dst = isBetweenNoEqual(s1[i],s2[j].dst)
            s12src = isBetweenNoEqual(s1[i],s2[j].src)
            s21dst = isBetweenNoEqual(s2[j],s1[i].dst)
            if s21src and s12dst:
                verticesArray.append(s1[i].src)
                verticesArray.append(s2[j].dst)
            if s12src and s21dst:
                verticesArray.append(s2[j].src)
                verticesArray.append(s1[i].dst)
            if s21dst and s12dst:
                verticesArray.append(s1[i].dst)
                verticesArray.append(s2[j].dst)
            if s21src and s12src:
                verticesArray.append(s2[j].src)
                verticesArray.append(s1[i].src)
            if (s1[i].src.x == s2[j].src.x and s1[i].src.y == s2[j].src.y) and s21dst:
                verticesArray.append(s1[i].src)
                verticesArray.append(s1[i].dst)
            if (s1[i].src.x == s2[j].src.x and s1[i].src.y == s2[j].src.y) and s12dst:
                verticesArray.append(s2[j].src)
                verticesArray.append(s2[j].dst)
            if (s1[i].dst.x == s2[j].dst.x and s1[i].dst.y == s2[j].dst.y) and s21src:
                verticesArray.append(s1[i].src)
                verticesArray.append(s1[i].dst)
            if (s1[i].dst.x == s2[j].dst.x and s1[i].dst.y == s2[j].dst.y) and s12src:
                verticesArray.append(s2[j].src)
                verticesArray.append(s2[j].dst)
            if (s1[i].src.x == s2[j].dst.x and s1[i].src.y == s2[j].dst.y) and s21dst:
                verticesArray.append(s1[i].src)
                verticesArray.append(s1[i].dst)
            if (s1[i].src.x == s2[j].dst.x and s1[i].src.y == s2[j].dst.y) and s12dst:
                verticesArray.append(s2[j].src)
                verticesArray.append(s2[j].dst)
            if (s1[i].dst.x == s2[j].src.x and s1[i].dst.y == s2[j].src.y) and s21src:
                verticesArray.append(s1[i].src)
                verticesArray.append(s1[i].dst)
            if (s1[i].dst.x == s2[j].src.x and s1[i].dst.y == s2[j].src.y) and s12src:
                verticesArray.append(s2[j].src)
                verticesArray.append(s2[j].dst)
            if s21src and s21dst:
                verticesArray.append(s1[i].src)
                verticesArray.append(s1[i].dst)
            if s12dst and s12src:
                verticesArray.append(s2[j].src)
                verticesArray.append(s2[j].dst)
            if (s1[i].src.x == s2[j].src.x and s1[i].src.y == s2[j].src.y) and (s1[i].dst.x == s2[j].dst.x and s1[i].dst.y == s2[j].dst.y):
                verticesArray.append(s1[i].src)
                verticesArray.append(s1[i].dst)
            if (s1[i].src.x == s2[j].dst.x and s1[i].src.y == s2[j].dst.y) and (s1[i].dst.x == s2[j].src.x and s1[i].dst.y == s2[j].src.y):
                verticesArray.append(s1[i].src)
                verticesArray.append(s1[i].dst)
    return verticesArray



def FindIndex(p, verticesArray):
    indexflag = 0
    for i in range(0, len(verticesArray)):
        if p.x == verticesArray[i].x and p.y == verticesArray[i].y:
            indexflag = 1
            index = i+1

    if indexflag == 1:
        return index
    else:
        raise Execption("cannot find an index in FindIndex function in CompareStreet.py")

def isOverlapLine(l1,l2):
    s21src = isBetweenNoEqual(l2,l1.src)
    s12dst = isBetweenNoEqual(l1,l2.dst)
    s12src = isBetweenNoEqual(l1,l2.src)
    s21dst = isBetweenNoEqual(l2,l1.dst)
    if s21src and s12dst:
        return True
    if s12src and s21dst:
        return True
    if s21dst and s12dst:
        return True
    if s21src and s12src:
        return True
    if (l1.src.x == l2.src.x and l1.src.y == l2.src.y) and s21dst:
        return True
    if (l1.src.x == l2.src.x and l1.src.y == l2.src.y) and s12dst:
        return True
    if (l1.dst.x == l2.dst.x and l1.dst.y == l2.dst.y) and s21src:
        return True
    if (l1.dst.x == l2.dst.x and l1.dst.y == l2.dst.y) and s12src:
        return True
    if (l1.src.x == l2.dst.x and l1.src.y == l2.dst.y) and s21dst:
        return True
    if (l1.src.x == l2.dst.x and l1.src.y == l2.dst.y) and s12dst:
        return True
    if (l1.dst.x == l2.src.x and l1.dst.y == l2.src.y) and s21src:
        return True
    if (l1.dst.x == l2.src.x and l1.dst.y == l2.src.y) and s12src:
        return True
    if s21src and s21dst:
        return True
    if s12dst and s12src:
        return True
    if (l1.src.x == l2.src.x and l1.src.y == l2.src.y) and (l1.dst.x == l2.dst.x and l1.dst.y == l2.dst.y):
        return True
    if (l1.src.x == l2.dst.x and l1.src.y == l2.dst.y) and (l1.dst.x == l2.src.x and l1.dst.y == l2.src.y):
        return True

    return False
