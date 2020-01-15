num = 0; m = 0; n = 0
st = [0,0]; fi = [0,0]; key = [0,0]
maze = []
dir = [[-1,0],[1,0],[0,1],[0,-1]]

def input(t):
    global num, m, n, st, fi, key, maze
    
    maze = []
    
    file = "Maze_"+str(t)+".txt"
    fp = open(file, 'r')
    
    line = fp.readline()
    element = line.split()
    num = int(element[0])
    m = int(element[1])
    n = int(element[2])
    
    for i in range(m):
        line = fp.readline()
        temp = []
        for j in range(n):
            temp.append(int(line[j]))
            if int(line[j])== 3:
                st = [i,j]
            elif int(line[j]) == 4:
                fi = [i,j]
            elif int(line[j]) == 6:
                key = [i,j]
        maze.append(temp)

    fp.close()

def bfs(t):
    global m, n, st, fi, key, maze, dir

    answer = []
    tot_len = 0; tot_time = 0
    
    for i in range(m):
        answer.append(maze[i][:])

    # to key
    path = []; check = []
    for i in range(m):
        temp = []
        for j in range(n):
            temp.append(0)
        path.append(temp)
        check.append(temp[:])

    fincheck = False
    q = [st]
    while fincheck == False:
        now = q.pop(0)
        for i in range(4):
            next = [now[0]+dir[i][0],now[1]+dir[i][1]]
            if next[0] < 0 or next[0] >= m or next[1] < 0 or next[1] >= n:
                continue
            if maze[next[0]][next[1]] == 2 and check[next[0]][next[1]] == 0:
                check[next[0]][next[1]] = 1
                path[next[0]][next[1]] = i
                tot_time += 1
                q.append(next)
            if next == key:
                path[next[0]][next[1]] = i
                fincheck = True
                break

    now = key[:]
    while now != st:
        answer[now[0]][now[1]] = 5
        way = path[now[0]][now[1]]
        now[0] -= dir[way][0]
        now[1] -= dir[way][1]
        tot_len += 1

    # to fin
    path = []; check = []
    for i in range(m):
        temp = []
        for j in range(n):
            temp.append(0)
        path.append(temp)
        check.append(temp[:])
    
    fincheck = False
    q = [key]
    while fincheck == False:
        now = q.pop(0)
        for i in range(4):
            next = [now[0]+dir[i][0],now[1]+dir[i][1]]
            if next[0] < 0 or next[0] >= m or next[1] < 0 or next[1] >= n:
                continue
            if maze[next[0]][next[1]] == 2 and check[next[0]][next[1]] == 0:
                check[next[0]][next[1]] = 1
                path[next[0]][next[1]] = i
                tot_time += 1
                q.append(next)
            if next == fi:
                path[next[0]][next[1]] = i
                fincheck = True
                break

    now = fi[:]
    while now != key:
        way = path[now[0]][now[1]]
        now[0] -= dir[way][0]
        now[1] -= dir[way][1]
        answer[now[0]][now[1]] = 5
        tot_len += 1

    file = "Maze_"+str(t)+"_BFS_output.txt"
    fp = open(file, 'w')

    for i in range(m):
        for j in range(n):
            fp.write(str(answer[i][j]))
        fp.write("\n")
    

    fp.write("---\n"+"length=%d\n"%tot_len+"time=%d\n"%tot_time)
    
    fp.close()

def ids(t):
    global m, n, st, fi, key, maze, dir
    
    answer = []
    tot_len = 0; tot_time = 0
    
    for i in range(m):
        answer.append(maze[i][:])

    # to key
    fincheck = False
    depth = 0
    while fincheck == False:
        check = []
        for i in range(m):
            temp = []
            for j in range(n):
                temp.append(0)
            check.append(temp)
        depth += 1
        s = [st]
        while s and fincheck == False:
            now = s[len(s)-1]
            pushcheck = False
            for i in range(4):
                next = [now[0]+dir[i][0],now[1]+dir[i][1]]
                if next[0] < 0 or next[0] >= m or next[1] < 0 or next[1] >= n:
                    continue
                if maze[next[0]][next[1]] == 2 and check[next[0]][next[1]] == 0:
                    check[next[0]][next[1]] = 1
                    tot_time += 1
                    pushcheck = True
                    s.append(next)
                    break
                if next == key:
                    s.append(next)
                    fincheck = True
                    break
            if fincheck == True:
                break
            if pushcheck == False or len(s) > depth:
                s.pop()

    s.pop(0)
    while s:
        now = s.pop()
        answer[now[0]][now[1]] = 5
        tot_len += 1

    # to fin
    fincheck = False
    depth = 0
    while fincheck == False:
        check = []
        for i in range(m):
            temp = []
            for j in range(n):
                temp.append(0)
            check.append(temp)
        depth += 1
        s = [key]
        while s and fincheck == False:
            now = s[len(s)-1]
            pushcheck = False
            for i in range(4):
                next = [now[0]+dir[i][0],now[1]+dir[i][1]]
                if next[0] < 0 or next[0] >= m or next[1] < 0 or next[1] >= n:
                    continue
                if maze[next[0]][next[1]] == 2 and check[next[0]][next[1]] == 0:
                    check[next[0]][next[1]] = 1
                    tot_time += 1
                    pushcheck = True
                    s.append(next)
                    break
                if next == fi:
                    fincheck = True
                    break
            if fincheck == True:
                break
            if pushcheck == False or len(s) > depth:
                s.pop()

    while s:
        now = s.pop()
        answer[now[0]][now[1]] = 5
        tot_len += 1

    file = "Maze_"+str(t)+"_IDS_output.txt"
    fp = open(file, 'w')

    for i in range(m):
        for j in range(n):
            fp.write(str(answer[i][j]))
        fp.write("\n")

    fp.write("---\n"+"length=%d\n"%tot_len+"time=%d\n"%tot_time)
    
    fp.close()

def greedy_bfs(t):
    global m, n, st, fi, key, maze, dir
    
    answer = []
    tot_len = 0; tot_time = 0
    
    for i in range(m):
        answer.append(maze[i][:])
    
    def percup(now):
        nonlocal q
        if q[now][0] < q[now//2][0]:
            q[now],q[now//2] = q[now//2],q[now]
            percup(now//2)
                
    def percdown(now):
        nonlocal q
        size = len(q)-1
        if now*2 > size:
            return
        if q[now][0] > q[now*2][0]:
            if now*2+1 <= size and q[now*2+1][0] < q[now*2][0]:
                q[now],q[now*2+1] = q[now*2+1],q[now]
                percdown(now*2+1)
            else:
                q[now],q[now*2] = q[now*2],q[now]
                percdown(now*2)
        elif now*2+1 <= size and q[now][0] > q[now*2+1][0]:
            q[now],q[now*2+1] = q[now*2+1],q[now]
            percdown(now*2+1)

    def delete():
        nonlocal q
        now = q[1][1]
        q[1] = q[len(q)-1]
        q.pop()
        percdown(1)
        return now

    def insert(now):
        nonlocal q
        q.append(now)
        percup(len(q)-1)

    def mandist(now,fin):
        return abs(fin[0]-now[0])+abs(fin[1]-now[1])

    # to key
    path = []; check = []
    for i in range(m):
        temp = []
        for j in range(n):
            temp.append(0)
        path.append(temp)
        check.append(temp[:])
    
    fincheck = False
    q = [[0,[0,0]],[0,st]]
    while fincheck == False:
        now = delete()
        for i in range(4):
            next = [now[0]+dir[i][0],now[1]+dir[i][1]]
            if next[0] < 0 or next[0] >= m or next[1] < 0 or next[1] >= n:
                continue
            if maze[next[0]][next[1]] == 2 and check[next[0]][next[1]] == 0:
                check[next[0]][next[1]] = 1
                path[next[0]][next[1]] = i
                tot_time += 1
                insert([mandist(next,key),next])
            if next == key:
                path[next[0]][next[1]] = i
                fincheck = True
                break

    now = key[:]
    while now != st:
        answer[now[0]][now[1]] = 5
        way = path[now[0]][now[1]]
        now[0] -= dir[way][0]
        now[1] -= dir[way][1]
        tot_len += 1

    # to fin
    path = []; check = []
    for i in range(m):
        temp = []
        for j in range(n):
            temp.append(0)
        path.append(temp)
        check.append(temp[:])
    
    fincheck = False
    q = [[0,[0,0]],[0,key]]
    while fincheck == False:
        now = delete()
        for i in range(4):
            next = [now[0]+dir[i][0],now[1]+dir[i][1]]
            if next[0] < 0 or next[0] >= m or next[1] < 0 or next[1] >= n:
                continue
            if maze[next[0]][next[1]] == 2 and check[next[0]][next[1]] == 0:
                check[next[0]][next[1]] = 1
                path[next[0]][next[1]] = i
                tot_time += 1
                insert([mandist(next,fi),next])
            if next == fi:
                path[next[0]][next[1]] = i
                fincheck = True
                break

    now = fi[:]
    while now != key:
        way = path[now[0]][now[1]]
        now[0] -= dir[way][0]
        now[1] -= dir[way][1]
        answer[now[0]][now[1]] = 5
        tot_len += 1

    file = "Maze_"+str(t)+"_GBFS_output.txt"
    fp = open(file, 'w')

    for i in range(m):
        for j in range(n):
            fp.write(str(answer[i][j]))
        fp.write("\n")

    fp.write("---\n"+"length=%d\n"%tot_len+"time=%d\n"%tot_time)
    
    fp.close()

def a_star(t):
    global m, n, st, fi, key, maze, dir
    
    answer = []
    tot_len = 0; tot_time = 0
    
    for i in range(m):
        answer.append(maze[i][:])

    def percup(now):
        nonlocal q
        if q[now][0] < q[now//2][0]:
            q[now],q[now//2] = q[now//2],q[now]
            percup(now//2)

    def percdown(now):
        nonlocal q
        size = len(q)-1
        if now*2 > size:
            return
        if q[now][0] > q[now*2][0]:
            if now*2+1 <= size and q[now*2+1][0] < q[now*2][0]:
                q[now],q[now*2+1] = q[now*2+1],q[now]
                percdown(now*2+1)
            else:
                q[now],q[now*2] = q[now*2],q[now]
                percdown(now*2)
        elif now*2+1 <= size and q[now][0] > q[now*2+1][0]:
            q[now],q[now*2+1] = q[now*2+1],q[now]
            percdown(now*2+1)

    def delete():
        nonlocal q
        now = q[1][1]
        q[1] = q[len(q)-1]
        q.pop()
        percdown(1)
        return now
    
    def insert(now):
        nonlocal q
        q.append(now)
        percup(len(q)-1)

    def mandist(now,fin):
        return abs(fin[0]-now[0])+abs(fin[1]-now[1])
    
    # to key
    path = []; check = []
    for i in range(m):
        temp = []
        for j in range(n):
            temp.append(0)
        path.append(temp)
        check.append(temp[:])
    
    check[st[0]][st[1]] = 1
    fincheck = False
    q = [[0,[0,0]],[0,st]]
    while fincheck == False:
        now = delete()
        for i in range(4):
            next = [now[0]+dir[i][0],now[1]+dir[i][1]]
            if next[0] < 0 or next[0] >= m or next[1] < 0 or next[1] >= n:
                continue
            if maze[next[0]][next[1]] == 2 and check[next[0]][next[1]] == 0:
                check[next[0]][next[1]] = check[now[0]][now[1]]+1
                path[next[0]][next[1]] = i
                tot_time += 1
                insert([check[next[0]][next[1]]+mandist(next,key),next])
            if next == key:
                path[next[0]][next[1]] = i
                fincheck = True
                break

    now = key[:]
    while now != st:
        answer[now[0]][now[1]] = 5
        way = path[now[0]][now[1]]
        now[0] -= dir[way][0]
        now[1] -= dir[way][1]
        tot_len += 1
    
    # to fin
    path = []; check = []
    for i in range(m):
        temp = []
        for j in range(n):
            temp.append(0)
        path.append(temp)
        check.append(temp[:])

    check[key[0]][key[1]] = 1
    fincheck = False
    q = [[0,[0,0]],[0,key]]
    while fincheck == False:
        now = delete()
        for i in range(4):
            next = [now[0]+dir[i][0],now[1]+dir[i][1]]
            if next[0] < 0 or next[0] >= m or next[1] < 0 or next[1] >= n:
                continue
            if maze[next[0]][next[1]] == 2 and check[next[0]][next[1]] == 0:
                check[next[0]][next[1]] = check[now[0]][now[1]]+1
                path[next[0]][next[1]] = i
                tot_time += 1
                insert([check[next[0]][next[1]]+mandist(next,fi),next])
            if next == fi:
                path[next[0]][next[1]] = i
                fincheck = True
                break

    now = fi[:]
    while now != key:
        way = path[now[0]][now[1]]
        now[0] -= dir[way][0]
        now[1] -= dir[way][1]
        answer[now[0]][now[1]] = 5
        tot_len += 1

    file = "Maze_"+str(t)+"_A_star_output.txt"
    fp = open(file, 'w')

    for i in range(m):
        for j in range(n):
            fp.write(str(answer[i][j]))
        fp.write("\n")

    fp.write("---\n"+"length=%d\n"%tot_len+"time=%d\n"%tot_time)
    
    fp.close()

def main():
    for i in range(1,4):
        input(i)
        bfs(i)
        ids(i)
        greedy_bfs(i)
        a_star(i)

if __name__ == "__main__":
    main()
