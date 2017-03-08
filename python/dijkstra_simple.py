# -*- encoding:utf-8 -*-


class BuildGraph(object):
    def __init__(self, filename):
        self.file = filename
        self.matrix = [[float('inf') for _ in range(100)] for _ in range(100)]
        self.v2idict = {}
        self.i2vdict = {}
        self.start = 0
        self.end = 0
        self.__parse_file()
t
    def vcount(self):
        return len(self.v2idict)

    def __parse_file(self):
        with open(self.file, 'r') as f:
            count = int(f.readline().strip('\n\r '))
            for i in range(1, count+1):
                data = f.readline().strip('\n\r ').split(',')
                if data[0] not in self.v2idict:
                    index = len(self.v2idict)
                    self.v2idict[data[0]] = index
                    self.i2vdict[index] = data[0]
                if data[1] not in self.v2idict:
                    index = len(self.v2idict)
                    self.v2idict[data[1]] = index
                    self.i2vdict[index] = data[1]
                line = self.v2idict[data[0]]
                col = self.v2idict[data[1]]
                self.matrix[line][col] = int(data[2])
            self.start = self.v2idict[(f.readline().strip('\n\r '))]
            self.end = self.v2idict['XH']


class DijkstraAlgorithm(object):
    def __init__(self, graph):
        self.matrix = graph.matrix
        self.start = graph.start
        self.end = graph.end
        self.i2vdict = graph.i2vdict
        self.vcount = graph.vcount()
        self.path = []
        self.dist = [float('inf')] * self.vcount
        self.__remain_set = set()
        self.i2vdict = graph.i2vdict
        for i in range(0, self.vcount):
            self.dist[i] = self.matrix[self.start][i]
            self.__remain_set.add(i)
        self.__deal()

    def __get_min(self):
        min_vertex = -1
        min_index = -1
        for i in self.__remain_set:
            if min_vertex == -1:
                min_vertex = i
                min_index = i
            if self.dist[i] < min_vertex:
                min_vertex = self.dist[i]
                min_index = i
        return min_index

    def __deal(self):
        self.path.append(self.start)
        self.__remain_set.remove(self.start)
        for n in range(0, self.vcount-2):
            min = self.__get_min()
            self.__remain_set.remove(min)
            if min == self.end:
                break
            for i in self.__remain_set:
                if self.dist[i] > self.dist[min] + self.matrix[min][i]:
                    self.dist[i] = self.dist[min] + self.matrix[min][i]
                    if i == self.end:
                        self.path.append(min)

    def print_result(self):
        print "Path: ",
        for i in range(0, len(self.path)):
            print self.i2vdict[self.path[i]], "-->",
        print self.i2vdict[self.end]
        print "Cost: ", self.dist[self.end]


if __name__ == "__main__":
    test = BuildGraph("test.txt")
    deal = DijkstraAlgorithm(test)
    deal.print_result()
