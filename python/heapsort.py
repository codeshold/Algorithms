# -*- encoding:utf-8 -*-


class MaxHeapSort(object):
    def __init__(self, srclist):
        self.srclist = srclist

    def adjust_down(self, i, size):
        cindex = i
        tmp = self.srclist[i]
        while cindex < size:
            lindex = cindex*2+1
            rindex = cindex*2+2
            if lindex < size:
                if rindex < size:
                    bindex = lindex if self.srclist[lindex] >= self.srclist[rindex] else rindex
                else:
                    bindex = lindex
                if tmp < self.srclist[bindex]:
                    self.srclist[cindex] = self.srclist[bindex]
                    cindex = bindex
                else:
                    break
            else:
                break
        self.srclist[cindex] = tmp

    def build_max_heap(self):
        for i in range(len(self.srclist)/2 - 1, -1, -1):
            self.adjust_down(i, len(self.srclist))

    def get_result(self):
        n = len(self.srclist)
        self.build_max_heap()
        for i in range(0, n):
            self.srclist[n-1-i], self.srclist[0] = self.srclist[0], self.srclist[n-1-i]
            self.adjust_down(0, n-1-i)
        return self.srclist


if __name__ == "__main__":
    import random
    a = []
    for i in range(1, 20):
        a.append(random.randint(1, 10))
    #a = [2,4,5,2,7,6,5,3,2]
    data = MaxHeapSort(a)
    print "SRC: ", a
    print "RESULT: ", data.get_result()

