# -*- coding: utf-8 -*-

# import sys
import math
import random

# DEMAND_CSV = '/Users/wenfeng/Desktop/shuxuejianmo /demand.csv'
COORD_ALL_CSV = '/Users/wenfeng/Desktop/shuxuejianmo /coord.csv'
DEMAND_CSV = '/Users/wenfeng/Desktop/shuxuejianmo /demand_okk.csv'
COORD_CSV = '/Users/wenfeng/Desktop/shuxuejianmo /coord_okk.csv'
# COORD_CSV = '/Users/wenfeng/Desktop/shuxuejianmo /coord.csv'
CONGESTION_RATE = 4


g_demand_matrix = list()
g_need_node_coord = list()
g_all_need_node_coord = list()
g_new_demand_matrix = list()
g_ok_index = set()

RADIUS = 3000              # 圆周半径
SAMPLE_PER_POINT = 10      # 每个点随机选择几个点
SECOND_NODE_T = 3000       # 二级节点容量（吨）

class MyPoint:
    def __init__(self, x=0, y=0, index=0, covered=False, circules=[]):
        self.x = x
        self.y = y
        self.index = index
        self.covered = covered
        self.circles = circules


class MyCircle:
    def __init__(self, x=0, y=0, points=[]):
        self.x = x
        self.y = y
        self.points = points
        self.pointsCount = 0
        self.raduis = RADIUS

    def get_need(self):
        all_need = 0
        for p in self.points:
            all_need += sum(g_demand_matrix[p.index])
            for i in range(len(g_demand_matrix)):
                all_need += g_demand_matrix[i][p.index]
        return all_need


class CorrdIndex():
    NUMBER = 0
    AREAKM = 1
    AREAM = 2
    X = 3
    Y = 4
    CONGEST = 5


def index_to_number(i=0):
    # if i > 3:
    #     return i+787
    # else:
    #     return i+1
    return i+791


def read_csv():
    with open(DEMAND_CSV, 'r') as f:
        for line in f.readlines():
            l = line.strip('\r').strip('\n').split(',')
            if l[0] == 'ZONE':
                continue
            g_demand_matrix.append(map(lambda x: float(x) if x != '' else 0, l[1:]))

    with open(COORD_ALL_CSV, 'r') as f:
        for line in f.readlines():
            l = line.strip('\r').strip('\n').split(',')
            g_all_need_node_coord.append(map(lambda x: float(x) if x != '' else 0, l))

    with open(COORD_CSV, 'r') as f:
        for line in f.readlines():
            l = line.strip('\r').strip('\n').split(',')
            # if line.split(',')[0] == 'ZONE':
            #     continue
            # print l
            g_need_node_coord.append(map(lambda x: float(x) if x != '' else 0, l))

    print "demand_matrix:", len(g_demand_matrix)
    # for i in demand_matrix:
    #     print i
    print "need_node_coord:", len(g_need_node_coord)
    # for i in need_node_coord:
    #     print i



def build_new_demand():
    '''
        (new/old) * rate <= CONGESTION_RATE
        new <=  CONGESTION_RATE/rate * old
    :return:
    '''
    for i in range(len(g_demand_matrix)):
        # g_new_demand_matrix.append(map(lambda x: CONGESTION_RATE / g_need_node_coord[CorrdIndex.CONGEST] * x, i))
        # if g_need_node_coord[i][CorrdIndex.CONGEST] == 0:
        #     print g_need_node_coord[i]
        #     break
        g_new_demand_matrix.append(map(
            lambda x: CONGESTION_RATE / g_need_node_coord[i][CorrdIndex.CONGEST] * x if g_need_node_coord[i][CorrdIndex.CONGEST] > 4 else 0,
            g_demand_matrix[i]))
        # g_new_demand_matrix.append([CONGESTION_RATE / g_need_node_coord[i][CorrdIndex.CONGEST] * x for x in g_demand_matrix[i]])

    # for i in g_new_demand_matrix:
    #     print i

# p1, p2 是否在一个圆内
def isInside(p1=MyPoint(), p2=MyPoint()):
    return math.sqrt(math.pow(p1.x - p2.x, 2) + math.pow(p1.y - p2.y, 2)) <= RADIUS


def point_demand(p=MyPoint()):
    all_need = 0
    all_need_old = 0
    # for p in c.points:
        # p = Point()
    all_need += sum(g_new_demand_matrix[p.index])
    all_need_old += sum(g_demand_matrix[p.index])
    for i in range(len(g_new_demand_matrix)):
        all_need += g_new_demand_matrix[i][p.index]
        all_need_old += g_demand_matrix[i][p.index]
    # print all_need, all_need_old
    return all_need


def p_points(p=[]):
    for i in p:
        # i = Point()
        # if i.covered is True:
        print i.index, "(", index_to_number(i.index), ")",
    print ''


def adjust_circle(c=MyCircle()):
    # print "adjust_circle_before:",
    # p_points(c.points)
    c.points = [p for p in c.points if p.index not in g_ok_index]
    c.pointsCount = len(c.points)
    if c.pointsCount == 0:
        return c

    # 排序
    count = len(c.points)
    for i in range(1, count):
        key = c.points[i]
        # key_value = math.sqrt(math.pow(key.x - c.x, 2) + math.pow(key.y - c.y, 2))
        key_value = math.sqrt(math.pow(key.x, 2) + math.pow(key.y, 2))
        j = i - 1
        while j >= 0:
            value = math.sqrt(math.pow(c.points[j].x, 2) + math.pow(c.points[j].y, 2))
            # value = math.sqrt(math.pow(c.points[j].x - c.x, 2) + math.pow(c.points[j].y - c.y, 2))
            if value > key_value:
                c.points[j+1] = c.points[j]
                c.points[j] = key
            j -= 1
    print "adjust_circle:",
    p_points(c.points)

    # 删除多余节点, 更新半径值
    need = 0
    index = 0
    for i in range(len(c.points)):
        need += point_demand(c.points[i])
        if need < SECOND_NODE_T:
            index = i
        else:
            break
    # over_points = c.points[index+1:]
    c.points = c.points[0:index+1]
    c.raduis = math.floor(math.sqrt(math.pow(c.points[-1].x - c.x, 2) + math.pow(c.points[-1].y - c.y, 2)))
    # c.x = c.points[len(c.points)/2 - 1].x
    # c.y = c.points[len(c.points)/2 - 1].y
    # print "\tradius:", c.raduis, "number:", len(c.points)
    # print "adjust_circle_after:",
    # p_points(c.points)
    return c



def gen_circles():

    points = []           # 顶点
    circles = []          # 存放结果的圆

    # init points
    for i in range(len(g_need_node_coord)):
        points.append(MyPoint(g_need_node_coord[i][CorrdIndex.X], g_need_node_coord[i][CorrdIndex.Y], i, False, []))

    # init circles
    for i in range(len(g_need_node_coord)):
        p = points[i]
        # p = Point()
        for j in range(SAMPLE_PER_POINT):
            r = random.random() * RADIUS
            fi = random.random() * 2 * math.pi
            x = p.x + r * math.cos(fi)
            y = p.y + r * math.sin(fi)
            circles.append(MyCircle(x, y))

    # 初始化circule.points, 同时针对需求做过滤
    new_circles = []
    for i in range(len(circles)):
        c = circles[i]
        # c = Circle()
        c.points = []
        c.pointsCount = 0
        for j in range(len(points)):
            if isInside(c, points[j]):
                c.points.append(points[j])
                c.pointsCount += 1
                points[j].circles.append(c)
        # 添加限制条件, 删除二级节点所不能满足的节点
        # adjust_circle(c)
        # print "raduis:", c.raduis,
        # print "gen_circle:",
        # p_points(c.points)
        if len(c.points) > 0:
            new_circles.append(c)
    circles = new_circles


    new_circles = []
    countt = 0
    g_ok_index.clear()
    rr_set = set()
    while countt != len(g_need_node_coord):
        # print countt, len(g_need_node_coord)
        # 找到覆盖节点最多的圆 c
        c = circles[0]
        # 调整点
        # 随机选择点
        for i in range(len(circles)):
            if c.pointsCount < circles[i].pointsCount:
                c = circles[i]
        # while len(rr_set) != len(circles):
        #     rr = random.randint(0, len(circles)-1)
        #     if rr not in rr_set:
        #         c = circles[rr]
        #         rr_set.add(rr)

        any = False
        # c = Circle()
        adjust_circle(c)
        for j in range(len(c.points)):
            if c.points[j].covered is False:
                any = True
                c.points[j].covered = True
                g_ok_index.add(c.points[j].index)
                countt += 1
                for k in range(len(c.points[j].circles)):
                    c.points[j].circles[k].pointsCount -= 1
        if any is True: # and len(c.points) > 0:
            new_circles.append(c)
    circles = new_circles
    return len(circles), circles


def point_body():
    read_csv()
    build_new_demand()

    min = len(g_need_node_coord)
    min_circles = []
    for i in range(10):
        tmp, c = gen_circles()
        if tmp < min:
            min = tmp
            min_circles = c
    print min
    for i in range(len(min_circles)):
        print "[", i, "]", min_circles[i].x, min_circles[i].y, \
            "[need]:", min_circles[i].get_need(), \
            "[radius]:", min_circles[i].raduis, \
            "[points]: ",
        p_points(min_circles[i].points)
    return min, min_circles


if __name__ == "__main__":
    read_csv()
    build_new_demand()

    min = len(g_need_node_coord)
    min_circles = []
    for i in range(10):
        tmp, c = gen_circles()
        if tmp < min:
            min = tmp
            min_circles = c
    print min
    for i in range(len(min_circles)):
         print "[", i, "]", min_circles[i].x, min_circles[i].y, \
             "[need]:", min_circles[i].get_need(), \
             "[radius]:", min_circles[i].raduis, \
             "[points]: ",
         p_points(min_circles[i].points)


