import random

num = 0; m = 0; n = 0
st = [0,0]; fi = [0,0]
maze = []
dir = [[-1,0],[1,0],[0,1],[0,-1]]
gamma = 0.5
max_step = 0

def input(t):
    global num, m, n, st, fi, maze, max_step
    
    file = "FrozenLake_"+str(t)+".txt"
    fp = open(file, 'r')
    
    line = fp.readline()
    element = line.split()
    
    num = int(element[0])
    m = int(element[1])
    n = int(element[2])
    max_step = m * n * 100000
    
    maze = []
    for i in range(m):
        line = fp.readline()
        temp = []
        for j in range(n):
            temp.append(line[j])
            if line[j]== 'S':
                st = [i,j]
            elif line[j] == 'G':
                fi = [i,j]
        maze.append(temp)

    fp.close()

def q_learning():
    global m, n, st, maze, dir, gamma, max_step
    
    q = [[[0.0 for k in range(4)] for j in range(n)] for i in range(m)]
    
    now = st
    for i in range(max_step):
        while True:
            target = random.randrange(0,4)
            temp = [now[0]+dir[target][0], now[1]+dir[target][1]]
            if temp[0] >= 0 and temp[0] < m and temp[1] >= 0 and temp[1] < n:
                break

        q[now[0]][now[1]][target] = gamma * max(q[temp[0]][temp[1]])
        if maze[temp[0]][temp[1]] != 'F':
            if maze[temp[0]][temp[1]] == 'H':
                q[now[0]][now[1]][target] -= 1
            elif maze[temp[0]][temp[1]] == 'G':
                q[now[0]][now[1]][target] += 1
            temp = st
            
        now = temp
    
    now = st
    while now != fi:
        max_val = 0.0
        ind = 0
        for i in range(4):
            if max_val < q[now[0]][now[1]][i]:
                max_val = q[now[0]][now[1]][i]
                ind = i
        now[0] += dir[ind][0]
        now[1] += dir[ind][1]
        if now != fi:
            maze[now[0]][now[1]] = 'R'
            
def output():
    global num, m, n, maze
    
    file = "FrozenLake_"+str(num)+"_output.txt"
    fp = open(file, 'w')
    
    fp.write(str(num) + " " + str(m) + " " + str(n) + "\n")
    
    for i in range(m):
        for j in range(n):
            fp.write(maze[i][j])
        fp.write("\n")
    
    fp.close()

def main():
    for i in range(1,4):
        input(i)
        q_learning()
        output()

if __name__ == "__main__":
    main()
