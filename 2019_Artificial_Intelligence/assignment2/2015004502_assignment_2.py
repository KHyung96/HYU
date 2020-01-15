from konlpy.tag import Okt
import math

total = 0; positive = 0; negative = 0
pos_word = 0; neg_word = 0
pos_data = []; neg_data = []
pos_cnt = []; neg_cnt = []
dont_need = ['Josa', 'PreEomi']

def train():
    global total, positive, negative, pos_word, neg_word, pos_data, pos_cnt, neg_data, neg_cnt
    
    okt = Okt()
    
    fp = open("ratings_train.txt", 'r')
    
    line = fp.readline()
    
    pos_data_temp = []; neg_data_temp = []
    pos_append = pos_data_temp.append
    neg_append = neg_data_temp.append
    
    while True:
        line = fp.readline()
        if line == '': break;
        
        if line[len(line)-1] == '\n':
            label = int(line[len(line)-2])
            line = line[:-2]
        else:
            label = int(line[len(line)-1])
            line = line[:-1]
            
        element = line.split()
        element.pop(0)
        
        total += 1
        if label == 1: positive += 1
        else: negative += 1
        
        for i in range(len(element)):
            now = okt.pos(element[i])
            for j in range(len(now)):
                if now[j][1] in dont_need:
                    continue
                if label == 1:
                    pos_append(now[j][0])
                else:
                    neg_append(now[j][0])
    
    pos_data_temp.sort()
    neg_data_temp.sort()
    
    pos_data_temp.append('')
    p_len = len(pos_data_temp)
    now = ''; j = -1
    for i in range(p_len):
        if now != pos_data_temp[i]:
            now = pos_data_temp[i]
            pos_data.append(pos_data_temp[i])
            j += 1
            pos_cnt.append(1)
        else:
            pos_cnt[j] += 1
    pos_data = pos_data[:-1]

    neg_data_temp.append('')
    n_len = len(neg_data_temp)
    now = ''; j = -1
    for i in range(n_len):
        if now != neg_data_temp[i]:
            now = neg_data_temp[i]
            neg_data.append(neg_data_temp[i])
            j += 1
            neg_cnt.append(1)
        else:
            neg_cnt[j] += 1
    neg_data = neg_data[:-1]
            
    pos_word = len(pos_data)
    neg_word = len(neg_data)
    
    fp.close()
    
def predict():
    global total, positive, negative, pos_word, neg_word, pos_data, pos_cnt, neg_data, neg_cnt
    
    okt = Okt()
    
    fp1 = open("ratings_test.txt", 'r')
    fp2 = open("ratings_result.txt", 'w')
    
    line = fp1.readline()
    fp2.write(line)
    
    all_line = []; all_data = []
    label_list = []
    cnt = 0
    
    while True:
        line = fp1.readline()
        if line == '': break;
        
        all_line.append(line)
        
        element = line.split()
        element.pop(0)
        
        cnt += 1
        for i in range(len(element)):
            now = okt.pos(element[i])
            for j in range(len(now)):
                if now[j][1] in dont_need:
                    continue
                all_data.append([now[j][0],cnt-1])

    all_data.sort(key=lambda element:element[0])
    tot_word = len(all_data)
    pos_ch = [(math.log(positive+1) - math.log(total+2)) for i in range(cnt)]
    neg_ch = [(math.log(negative+1) - math.log(total+2)) for i in range(cnt)]
    tot_pos = [0 for i in range(cnt)]
    tot_neg = [0 for i in range(cnt)]
    
    ind = 0
    for i in range(pos_word):
        pre_ind = ind
        for j in range(pre_ind,tot_word):
            if pos_data[i] < all_data[j][0]:
                ind = j
                break
            if pos_data[i] == all_data[j][0]:
                pos_ch[all_data[j][1]] += math.log(pos_cnt[i]+1) - math.log(pos_word+2)
            else:
                pos_ch[all_data[j][1]] += math.log(1.0) - math.log(pos_word+2)
                
    for j in range(ind,tot_word):
        pos_ch[all_data[j][1]] += math.log(1.0) - math.log(pos_word+2)
                
    ind = 0
    for i in range(neg_word):
        pre_ind = ind
        for j in range(pre_ind,tot_word):
            if neg_data[i] < all_data[j][0]:
                ind = j
                break
            if neg_data[i] == all_data[j][0]:
                neg_ch[all_data[j][1]] += math.log(neg_cnt[i]+1) - math.log(neg_word+2)
            else:
                neg_ch[all_data[j][1]] += math.log(1.0) - math.log(neg_word+2)
    
    for j in range(ind,tot_word):
        neg_ch[all_data[j][1]] += math.log(1.0) - math.log(neg_word+2)

    for i in range(cnt):
        if pos_ch[i] > neg_ch[i]:
            label = 1
        elif pos_ch[i] < neg_ch[i]:
            label = 0
        else:
            if tot_pos[i] > tot_neg[i]:
                label = 1
            else:
                label = 0
                
        if all_line[i][-1] == "\n":
            all_line[i] = all_line[i][:-1]

        line = all_line[i] + "\t" + str(label) + "\n"

        fp2.write(line)
                
    fp1.close()
    fp2.close()
    
def main():
    train()
    predict()

if __name__ == "__main__":
    main()
