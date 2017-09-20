# -*- coding: utf-8 -*-

from pointcover import *
import matplotlib.pyplot as plt


if __name__ == "__main__":
    clen, circles = point_body()
    print len(circles)

    c_count = 0

    dst_circles = []
    # for c in circles:
    #     dst_circles.append(plt.Circle((c.x, c.y), c.raduis, color='r', fill=False, clip_on=False))

    ax = plt.gca()
    ax.cla()  # clear things for fresh plot

    # change default range so that new circles will work
    ax.set_xlim((135000, 200000))
    ax.set_ylim((135000, 200000))
    # some data
    for i in g_need_node_coord:
        x = i[CorrdIndex.X]
        y = i[CorrdIndex.Y]
        # plt.plot(x/1000, y/1000, '.b', markersize=4)
        ax.plot(x, y, '.b', markersize=4)

    onelevel = set()
    for i in range(4):
        x = g_all_need_node_coord[i][CorrdIndex.X]
        y = g_all_need_node_coord[i][CorrdIndex.Y]

        ax.plot(x, y, '+b', markersize=5)

        min_i = 0
        # min_i_sec = min_i
        min_c = circles[0]
        # min_c_sec = min_c
        min_d = math.sqrt(math.pow(min_c.x - x, 2) + math.pow(min_c.y - y, 2))
        # min_d_sec = min_d
        for i in range(1, len(circles)):
            c = circles[i]
            d = math.sqrt(math.pow(c.x - x, 2) + math.pow(c.y - y, 2))
            if d < min_d:
                min_c = c
                min_d = d
                min_i = i
            # elif d < min_d_sec:
            #     min_c_sec = c
            #     mind_d_sec = d
            #     min_i_sec = i

        onelevel.add(min_i)
        flag = 0
        if flag == 0:
            # onelevel.add(random.randint(1, 110))
            # onelevel.add()
            flag = 1
        print "==="
        print min_i
        ax.plot(min_c.x, min_c.y, '*r', markersize=6)
        c_count += 1
        # ax.plot((5), (5), 'o', color='y')

    onelevel.add(5)
    ax.plot(circles[5].x, circles[5].y, '*r', markersize=6)
    c_count += 1
    for i in range(len(circles)):
        if i not in onelevel:
            c = circles[i]
            ax.plot(c.x, c.y, '*y', markersize=6)
            c_count += 1
    # for i in dst_circles:
    #     ax.add_artist(i)

    plt.axis('scaled')
    plt.axis('equal')  # changes limits of x or y axis so that equal increments of x and y have the same length
    plt.title('The number of nodes {}'.format(clen))
    plt.xlabel('km')
    plt.ylabel('km')
    plt.show()

    print c_count
    print onelevel
