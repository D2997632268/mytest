'''
笔记：
1、image[第几行][第几列]
2、总行数, 总列数=image.shape
3、a=[[1,2,3],[4,6,7]]------>a[1][0]=4

'''
import numpy as np
import cv2
import matplotlib.pyplot as plt
from QR_Version import version_info

#*********************************************************#    
#**************************洪水种子法***********************#
#*********************************************************#  
def flood_fill_seed(q, x, y, pixel, num, func, stondseed,scores):
    seed=[x,y]#记录下当前的种子
    pixel = q[y][x]
    count = 0
    k=1
    LEN=9999
    X=[]
    Y=[]
    LEFT=[]
    RIGHT = []
    p = q.copy()
    row =  p.reshape(-1)
    h = img.shape[0]
    w = img.shape[1]
    while (1):
        left = x
        right = x
        pos = y*w
        
        while (left>0 and pos+left-1>0 and row[pos+left-1] == pixel):
            left = left-1                       

        while (right<w-1 and pos+right+1 <w*h-1 and row[pos+right+1] == pixel):
            right = right+1
        for i in range(left, right+1):
            row[pos+i] = num
        if func:           
            count = func(count, y, left,right, stondseed,scores)

        while (1):
            if y<=h and y>=0 and x<=w and x>=0 and k>=1 and k<=LEN+1:
                if (y>0):
                    pos = (y-1)*w
                    recurse = False
                    for i in range(left,right+1):
                        if (row[pos+i] == pixel):
                            k=1
                            X.append(i)
                            Y.append(y)
                            LEFT.append(left)
                            RIGHT.append(right)
                            x = i
                            y = y - 1
                            recurse = True
                            LEN = len(X)
                            break
                    if recurse:
                        break
                if (y<h-1):
                    pos = (y+1)*w
                    recurse = False
                    for i in range(left, right+1):
                        if (row[pos+i] == pixel):
                            k=1
                            X.append(i)
                            Y.append(y)
                            LEFT.append(left)
                            RIGHT.append(right)
                            LEN = len(X)
                            x = i
                            y = y + 1
                            recurse = True
                            break
                    if recurse:
                        break
                if y>h or y<0 or x>w or x<0 or (LEN-k)==9998:
                    return 1,0,0,0,0,0
                x = X[LEN-k]
                y = Y[LEN-k]
                left = LEFT[LEN-k]
                right = RIGHT[LEN-k]
                k=k+1  #检测第几次未找到相同行
            else:
                return count, X, Y, LEFT, RIGHT, seed

#*********************************************************#    
#************************累计点个数总和*********************#
#*********************************************************#
def area_count(count, y,left,right,seed,scores):
    count += right-left+1
    return count

#*********************************************************#    
#***************寻找finder pattern的一个角点坐标************#
#*********************************************************#
def find_one_corner(count, y, left,right, seed, scores):
    global LeftX, LeftY
    xs = [left, right]
    dy = y - seed[1]
    for i in range(0,2):
        dx = xs[i] - seed[0]
        d = dx*dx+dy*dy

        if d > scores[0]:
            scores[0] = d
            cornerX[0] = xs[i]
            cornerY[0] = y
    return 0

#*********************************************************#    
#***********寻找finder pattern的另外三个角点坐标************#
#*********************************************************#
def find_other_corner(count, y, left, right, seed, scores):
    # print('other_scores=',scores)
    psd_ref_x = seed[0]
    psd_ref_y = seed[1]
    xs=[left, right]
    # print('y, left, right, seed=', y, left, right, seed)
    sco=[0,0,0,0]
    for i in range(0,2):
        up = xs[i]*psd_ref_x + y*psd_ref_y
        rightr = -psd_ref_y*xs[i]+y*psd_ref_x
        sco[0]=up
        sco[1]=rightr
        sco[2]=-up
        sco[3]=-rightr
        # print('soc=',sco)

        for j in range(0,4):
            # print('j,sco,scores',j,sco[j],scores[j])

            if sco[j]>scores[j]:
                scores[j]=sco[j]
                cornerX[j]=xs[i]
                cornerY[j]=y
            # print('cornerX,cornerY',j,cornerX[j],cornerY[j])
                # print('j,sco,scores',j,sco[j],scores[j])
            # else:
            #     print('---------------------------')
            #     print('j,sco,scores',j,sco[j],scores[j])
        # print('j,sco,score,---scornerX,cornerY',0,sco[0],scores[0], cornerX[0],cornerY[0])
    return 0

def region_code(image, x, y, num):
    if x<0 or y<0 or x>image.shape[1] or y>image.shape[0]:
        return -1,0,0,0,0,0
    pixel = image[y][x]
    if pixel >= 2:
        return -1,0,0,0,0,0
    if pixel == 1:
        return -1,0,0,0,0,0
    resultt = flood_fill_seed(image, x, y, image[y][x], num, area_count,[0,0],0)
    return resultt

def test_capstone(image, X, Y):
    global temp_rect,b,cc
    ratio = 0
    ring_left = region_code(image, X[0]-1, Y , 1)
    stone = region_code(image, X[2]-1, Y , 2)
    ring_right = region_code(image, X[4]-1, Y , 3)
    if ring_left[0] == -1 or ring_right[0]== -1 or stone[0]== -1: 
        return 0
    if ring_left[0] <1 or ring_right[0]<1 or stone[0]<1:
        return 0
    if ring_left[0] != ring_right[0]:
        return 0
    if ring_right[0] == stone[0]:
        return 0
    ratio = stone[0]*100/ring_left[0]
    if ratio<10 or ratio>70:
        return 0

    rect = record_capstone(image, ring_left, stone, X,Y)#寻找角点---原始方法
    # rect = record_capstone2(image, ring_left, stone, X,Y)#改进后方法
    if rect != 0:
        if rect in temp_rect: #找到的角点出现两次以上即保存

            if rect not in cc:
                cc.append(rect)
    temp_rect.append(rect)

def record_capstone2(image, ring_left, stone, X, Y):#改进后方法
    count = ring_left[0]
    x = ring_left[1]
    y = ring_left[2]
    left = ring_left[3]
    right = ring_left[4]

    rect=[(),(),(),()]
    sum_pts_max = -99999
    sum_pts_min = 99999
    reduct_pts_max = -99999
    reduct_pts_min = 99999

    for ii in range(0,len(y)):
        i = y[ii]
        for j in (left[ii],right[ii]):
            if i+j>sum_pts_max:
                sum_pts_max = i+j
                rect[2] = (j,i)
            if i+j<sum_pts_min:
                sum_pts_min = i+j
                rect[0] = (j,i)
            if j-i>reduct_pts_max:
                reduct_pts_max = j-i
                rect[1] = (j,i)
            if j-i<reduct_pts_min:
                reduct_pts_min = j-i
                rect[3] = (j,i)
    return rect 

def order_points2(x,y):
    rect=[(),(),(),()]
    sum_pts_max = -99999
    sum_pts_min = 99999
    reduct_pts_max = -99999
    reduct_pts_min = 99999
    for z in range(0,4):
        j = x[z]
        i = y[z]
        if i+j>sum_pts_max:
            sum_pts_max = i+j
            rect[2] = (j,i)
        if i+j<sum_pts_min:
            sum_pts_min = i+j
            rect[0] = (j,i)
        if j-i>reduct_pts_max:
            reduct_pts_max = j-i
            rect[1] = (j,i)
        if j-i<reduct_pts_min:
            reduct_pts_min = j-i
            rect[3] = (j,i)          
    return rect

def find_region_corners(q, ring_left, stone_seedXY, originalX, originalY):
    rect=[(),(),(),()]
    scores=[-1,0,0,0]
    ringX=ring_left[-1][0]
    ringY=ring_left[-1][1]

    refX = stone_seedXY[0]
    refY = stone_seedXY[1]
    flood_fill_seed(q, ringX, ringY, q[ringY][ringX], 10, find_one_corner, stone_seedXY, scores)
    a = cornerX[0]
    b = cornerY[0]

    psd_ref_x = cornerX[0] - refX
    psd_ref_y = cornerY[0] - refY

    for i in range(0,4):
        cornerX[i] = ringX
        cornerY[i] = ringY
    

    i = -psd_ref_x * ringX + ringY * psd_ref_y
    scores[0] = i
    scores[2] = -i

    i = -psd_ref_y * ringX + psd_ref_x * ringY
    scores[1] = i
    scores[3] = -i
    flood_fill_seed(q, ringX, ringY, q[ringY][ringX], 20, find_other_corner, (psd_ref_x, psd_ref_y), scores)
    
    cornerX[0] = a
    cornerY[0] = b

    rectt = order_points2(cornerX,cornerY)
    return rectt   

def record_capstone(image, ring_left, stone, X, Y):
    scores=[-1,0,0,0]
    LeftX=0
    LeftY=0
    RightX=0
    RightY=0
    a = find_region_corners(image, ring_left, stone[-1], X, Y)
    return a

#*********************************************************#    
#********************测试数据的黑白宽度*********************#
#*********************************************************#  
def image_width(y, pixel):
    width = []
    i = 1
    xNum = 1
    row = []
    for x in range(2, len(pixel)-1):
        if pixel[x - 1] == pixel[x]:
            xNum += 1
        else:
            width.append(xNum)
            row.append(x)
            i += 1
            xNum = 1
        if x == len(pixel) - 1:
            width.append(xNum)
            row.append(x)
    return row, width
#*********************************************************#    
#********************寻找垂直1:1:3：1：1*********************#
#*********************************************************#   

def test_vertical(image, x, y, pd, pd1, pd2, pd3, pd4, xcenter, X):
    check = [1, 1, 3, 1, 1]
    focus = []
    new_focus = []
    new_XX = []
    XX = []
    Summ = x + pd + pd1 + pd2 + pd3 + pd4
    Ycenter = 0
    n = 0
    k = 0
    if x + pd + pd1 + pd2 + pd3 + pd4>=image.shape[1]:
        return 0,0
    for i in range(x-1, x + pd + pd1 + pd2 + pd3 + pd4-1):
        k += n
        n = 0
        if 1:
            y_col, Ywidth = image_width(i, image[:, i])

            for yw in range(0, len(Ywidth) - 4):
                pd = [Ywidth[yw], Ywidth[yw + 1], Ywidth[yw + 2],
                      Ywidth[yw + 3], Ywidth[yw + 4]]
                avg = (pd[0] + pd[1] + pd[3] + pd[4]) / 4
                err = avg * 3 / 4
                ok = 1
                for j in range(0, 5):
                    if pd[j] < check[j] * avg - err or pd[j] > check[j] * avg + err + 1:
                        ok = 0
                        break

                if ok == 1:
                    n = 1
                    Ycenter = y_col[yw + 1] + \
                        (y_col[yw + 2] - y_col[yw + 1]) // 2
                    focus.append((xcenter, Ycenter))
                    XX = tuple(X)
    for x in focus:
        if x not in new_focus:
            new_focus.append(x)

    return new_focus, XX

#*********************************************************#    
#**********************计算透视矩阵************************#
#*********************************************************#  
def perspectivate_setup(c,rect,w,h):
    x0 = int(rect[0][0])
    y0 = int(rect[0][1])
    x1 = int(rect[1][0])
    y1 = int(rect[1][1])
    x2 = int(rect[2][0])
    y2 = int(rect[2][1])
    x3 = int(rect[3][0])
    y3 = int(rect[3][1])

    wden = w * (x2*y3 - x3*y2 + (x3-x2)*y1 + x1*(y2-y3))
    hden = h * (x2*y3 + x1*(y2-y3) - x3*y2 + (x3-x2)*y1)

    c[0] = (x1*(x2*y3-x3*y2) + x0*(-x2*y3+x3*y2+(x2-x3)*y1) + x1*(x3-x2)*y0) / wden
    c[1] = -(x0*(x2*y3+x1*(y2-y3)-x2*y1) - x1*x3*y2 + x2*x3*y1+ (x1*x3-x2*x3)*y0) / hden
    c[2] = x0
    c[3] = (y0*(x1*(y3-y2)-x2*y3+x3*y2) + y1*(x2*y3-x3*y2) +x0*y1*(y2-y3)) / wden
    c[4] = (x0*(y1*y3-y2*y3) + x1*y2*y3 - x2*y1*y3 +y0*(x3*y2-x1*y2+(x2-x3)*y1)) / hden
    c[5] = y0
    c[6] = (x1*(y3-y2) + x0*(y2-y3) + (x2-x3)*y1 + (x3-x2)*y0) / wden
    c[7] = (-x2*y3 + x1*y3 + x3*y2 + x0*(y1-y2) - x3*y1 + (x2-x1)*y0) /hden
    c[8] = 1

    return 1

#*********************************************************#    
#************************水平扫描*************************#
#*********************************************************#  
def finder_scan(image):
    check = [1, 1, 3, 1, 1]
    ok = 0
    # print('image.shape=', image.shape)
    for y in range(0, image.shape[0]-1): 
        if 1:  
            x_row, Xwidth = image_width(y, image[y, :])
            for xW in range(0, len(Xwidth) - 4):
                pd = [Xwidth[xW], Xwidth[xW + 1], Xwidth[xW + 2],
                      Xwidth[xW + 3], Xwidth[xW + 4]]
                x = [x_row[xW], x_row[xW + 1], x_row[xW + 2],
                     x_row[xW + 3], x_row[xW + 4]]
                avg = (pd[0] + pd[1] + pd[3] + pd[4]) / 4
                err = avg * 3 / 4
                ok = 1
                for i in range(0, 5):
                    if pd[i] < check[i] * avg - err or pd[i] > check[i] * avg + err + 1:
                        ok = 0
                if ok == 1:  # 水平扫描结束    
                    Xcenter = x_row[xW + 1] + \
                        (x_row[xW + 2] - x_row[xW + 1]) // 2
                    focus, XX = test_vertical(image, x_row[xW], y, pd[0], pd[1],
                                              pd[2], pd[3], pd[4], Xcenter, x)

                    if focus==0 and XX==0:
                        break
                    if len(XX)>3 and y != [] :

                        #方法三：
                        # print('-----in------')
                        # print(XX, y)
                        # cv2.circle(img, (XX[0],y), 1, (255, 0, 0), -1)
                        # cv2.circle(img, (XX[1],y), 1, (255, 0, 0), -1)
                        # cv2.circle(img, (XX[2],y), 1, (255, 0, 0), -1)
                        # cv2.circle(img, (XX[3],y), 1, (255, 0, 0), -1)
                        result = test_capstone(image, XX, y)                                         
                        # print('switch',switch)
                        # cv2.circle(img, c[1], 3, (0, 0, 255), 1)
                        # cv2.circle(img, c[2], 3, (0, 255, 0), 1)
                        # cv2.circle(img, c[3], 3, (255, 0, 0), 1)
                        # cv2.circle(img, c[4], 3, (100, 100, 0), 1)

                        # print('result=',result)

def centerFun(rect,ii):
    centerX=0
    centerY=0
    x0 = int(rect[0][0])
    y0 = int(rect[0][1])
    x1 = int(rect[1][0])
    y1 = int(rect[1][1])
    x2 = int(rect[2][0])
    y2 = int(rect[2][1])
    x3 = int(rect[3][0])
    y3 = int(rect[3][1])

    centerX = int((x0+x1+x2+x3)/4)
    centerY = int((y0+y1+y2+y3)/4)
    # print('centerX,centerY=', centerX,centerY)
    cv2.circle(img, (centerX,centerY), 1, (0, 255, 0), 1)
    return centerX, centerY

#*********************************************************#    
#************************坐标转换*************************#
#*********************************************************#
def perspective_mapQFit(c, Q18_X, Q18_Y,FB_Cell_32):
    s11 = Q18_X * c[0]
    s12 = Q18_Y * c[1]
    s13 = (c[2] << 18)

    s21 = Q18_X * c[3]
    s22 = Q18_Y * c[4]
    s23 = (c[5] << 18)

    s31 = Q18_X * c[6]
    s32 = Q18_Y * c[7]
    s33 = (c[8] << 18)

    sum_1_2s = s11 + s12 + s13
    sum_2_2s = s21 + s22 + s23
    sum_3_2s = s31 + s32 + s33
    FB_Cell_32.append([int(sum_1_2s /sum_3_2s),int(sum_2_2s /sum_3_2s)])
    centerX = (sum_1_2s / sum_3_2s)
    centerY = (sum_2_2s / sum_3_2s)

def perspective_mapQFit2(c, Q18_X, Q18_Y):
    s11 = Q18_X * c[0]
    s12 = Q18_Y * c[1]
    s13 = (c[2] << 18)

    s21 = Q18_X * c[3]
    s22 = Q18_Y * c[4]
    s23 = (c[5] << 18)

    s31 = Q18_X * c[6]
    s32 = Q18_Y * c[7]
    s33 = (c[8] << 18)

    sum_1_2s = s11 + s12 + s13
    sum_2_2s = s21 + s22 + s23
    sum_3_2s = s31 + s32 + s33
    
    centerX = int(sum_1_2s / sum_3_2s)
    centerY = int(sum_2_2s / sum_3_2s)

    return centerX,centerY
     
def perspective_unmap(c, x,y):
    den = -c[0]*c[7]*y + c[1]*c[6]*y + (c[3]*c[7]-c[4]*c[6])*x + c[0]*c[4] - c[1]*c[3]

    u = -(c[1]*(y-c[5]) - c[2]*c[7]*y + (c[5]*c[7]-c[4])*x + c[2]*c[4]) / den

    v = (c[0]*(y-c[5]) - c[2]*c[6]*y + (c[5]*c[6]-c[3])*x + c[2]*c[3]) / den

    return u,v

def rotate_capstone(i, h0_x, h0_y, hd_x, hd_y):
    global capstones,matrixC
    best=0.0
    best_score=0
    for j in range(0,4):
        p=capstones[i]
        score = (p[j][0]-h0_x)*-hd_y +(p[j][1]-h0_y)*hd_x

        if ( j==0 or score<best_score):
            best=j
            best_score=score
    for j in range(0,4):
        capstonesCopy[i][j]=capstones[i][(j+best)%4]
    perspectivate_setup(matrixC[i],capstonesCopy[i],7.0,7.0)
    return 8

#*********************************************************#    
#************确定交点的位置（第四个定位点）******************#
#*********************************************************#
def line_intersect(p0, p1, q0, q1, r):
    cv2.line(img, q0, q1, (255,0,0),1)
    cv2.line(img, p0, p1, (255,0,0),1)
    a = -(p1[1]-p0[1])
    b = p1[0]-p0[0]

    c = -(q1[1]-q0[1])
    d = q1[0]-q0[0]

    e = a*p1[0] + b*p1[1]
    f = c*q1[0] + d*q1[1]

    det = (a*d) - (b*c)

    if det==0:
        return 0

    r[0] = (d*e - b*f)//det
    r[1] = (-c*e + a*f)//det
    cv2.circle(img, (r[0],r[1]), 1,(255,0,0),1)

    return 1

def perspective_map(c, u, v, ret):
    den = c[6]*u + c[7]*v+1.0
    x = (c[0]*u + c[1]*v + c[2])/den
    y = (c[3]*u + c[4]*v + c[5])/den

    ret[0] = x+0.5
    ret[1] = y+0.5

#*********************************************************#    
#*********************深度优先搜索*************************#
#*********************************************************#  
def fourth_Pattern(x,y,size_estimate,book):
    global isGO,Fourth_Align,Fourth_AlignXY

    next=[[0,2],[2,0],[0,-2],[-2,0]]#8连通区域 ,[-1,-1],[1,-1],[-1,1],[1,1]   
    #枚举四个方向
    if isGO==False:
        return Fourth_Align, Fourth_AlignXY 
    for i in range(0,4):
        
        tx = x + next[i][0]
        ty = y + next[i][1]

        if tx <1 or tx >=img.shape[1] or ty<1 or ty>=img.shape[0]:
            continue
        if (book[tx][ty]==0):
            code = region_code(dst, tx, ty, 8)
            if (code[0] < size_estimate/3) or (code[0]>size_estimate*3) :
                book[tx][ty]=1
                fourth_Pattern(tx,ty,size_estimate,book)
            else:
                isGO = False
                Fourth_Align=code[0]
                Fourth_AlignXY = [tx,ty]
                return Fourth_Align, Fourth_AlignXY  

    return Fourth_Align, Fourth_AlignXY 

#********************广度优先搜索************************#    
#*******从一个点一圈一圈往外遍历，找到符合要求的区域**********#
#*********************************************************#  
def coordinate_xy(x,y,size_estimate,book):
    a = [[x,y], ]
    for j in a:
        # print('-----------------------') 
        next=[[0,1],[1,0],[0,-1],[-1,0]]
        for i in range(0,4):
            tx = j[0] + next[i][0]
            ty = j[1] + next[i][1]

            if tx <1 or tx >=img.shape[1] or ty<1 or ty>=img.shape[0]:
                continue
            if book[tx][ty]==0:  
                book[tx][ty]=1
                code = region_code(dst, tx, ty, 8)
                if (code[0] >= size_estimate/3) and (code[0]<=size_estimate*3) :
                    Fourth_Align=code[0]
                    Fourth_AlignXY = [tx,ty]
                    return Fourth_Align, Fourth_AlignXY  
                a.append([tx,ty])                
        a.remove(j)

#*********************************************************#    
#********************第四个点位置精调**********************#
#*********************************************************#
def find_alignment_pattern(cap, align, alignRegion):
    temp_b = {(0,0),}
    c0 = capstones[cap[0]]
    c2 = capstones[cap[2]]

    step_size = 1
    DIR = 0
    dx_map = [-1,1,0,0]
    dy_map = [0,0,1,-1]

    u=0
    v=0
    a=[0,0]
    b=[0,0]
    c=[0,0]
    b = align
    count = 0
    u,v = perspective_unmap(matrixC[cap[0]], b[0], b[1])
    perspective_map(matrixC[cap[0]], u, v+1.0, a)
    u,v = perspective_unmap(matrixC[cap[2]],  b[0], b[1])
    perspective_map(matrixC[cap[2]], u+1.0, v, c)
    size_estimate = round(abs((-(c[1]-b[1]) * (a[0]-b[0])) + (a[1]-b[1])*(c[0]-b[0])))
    book = np.zeros((dst.shape[0], dst.shape[1]), np.uint8)

    #*******************************方法一：不适用(原始方法)*******************************#
    if 0:
        while (step_size*step_size < size_estimate*100):
            count += 1
            for i in range(0,step_size):
                if tuple(b) not in temp_b:
                    temp_b.add(tuple(b))
                    code = region_code(dst, b[0], b[1], 8)
                    if ((code[0] >= size_estimate/3) and (code[0]<=size_estimate*3)):
                        alignRegion=code[0]
                        return alignRegion
                cv2.circle(img, (b[0], b[1]), 1, (200,200,50), -1)
                b[0] += dx_map[DIR]
                b[1] += dy_map[DIR]
            DIR = (DIR+1)%4
            if (DIR and 1)==0:
                step_size += 1
    #***************************************方法二：不适用**********************************************#
    # alignRegion,reg_seed = fourth_Pattern(b[0],b[1],size_estimate,book)#step_size,size_estimate,DIR,

    #****************************************方法三：ok**************************************************#
    book[b[0],b[1]]=1
    alignRegion,reg_seed = coordinate_xy(b[0],b[1],size_estimate,book)
    cv2.circle(img,(reg_seed[0], reg_seed[1]), 1, (0,250,0),-1)

    return alignRegion,reg_seed

def timing_scan(p0,p1):

    nn = round(p1[0]-p0[0])
    dd = round(p1[1]-p0[1])
    n = round(p1[0]-p0[0])
    d = round(p1[1]-p0[1])
    x = round(p0[0])
    y = round(p0[1])
    a = 0
    run_length = 0
    count = 0

    if x<0 or y<0 or x>=dst.shape[1] or y>=dst.shape[0]:
        return -1
    if p1[0]<0 or p1[1]<0 or p1[0]>=dst.shape[1] or p1[1]>=dst.shape[0]:
        return -1
    if abs(n) > abs(d):
        swap = n
        n = d
        d = swap

    if n < 0:
        n = -n
        nondom_step = -1
    else:
        nondom_step = 1

    if d < 0:
        d = -d
        dom_step = -1
    else:
        dom_step = 1

    x = round(p0[0])
    y = round(p0[1])

    for i in range(0, d):
        if (y < 0 or y >= dst.shape[0] or x<0 or x>=dst.shape[1]):
            print('break')
            break

        pixel = dst[y][x]
        if (pixel):
            if run_length>=2:
                count += 1
            run_length = 0
        else:
            run_length += 1
        a += n

        if abs(nn) > abs(dd):
            x += dom_step
        else:
            y += dom_step

        if a >= d:
            if abs(nn) > abs(dd):
                y += nondom_step
            else:
                x += nondom_step
            a -= d 

    return count

def measure_timing_pattern(cap, size):
    tpep = [[0,0],[0,0],[0,0]]

    for i in range(0, 3):
        us = [6.5, 6.5, 0.5]
        vs = [0.5, 6.5, 6.5]       
        perspective_map(matrixC[cap[i]], us[i], vs[i], tpep[i])
    hscan = timing_scan(tpep[1], tpep[2])
    vscan = timing_scan(tpep[1], tpep[0])
    scan = hscan
    if vscan>scan:
        scan = vscan

    if scan<3:
        u,v = perspective_unmap(matrixC[cap[1]], capstones[2][0][0], capstones[2][0][1])
        size = u+6.5
        return 0
    size = scan*2+13
    ver = (size-15)//4
    size = ver*4+17
    return size  

#*********************************************************#    
#********************seed代表方向向量**********************#
#*********************************************************# 
def find_leftmost_to_line(count, y, left,right, seed, scores):
    xs=[left, right]
    for i in range(0,2):
        d = -seed[1] * xs[i] + seed[0] * y
        if (d<=scores[0]):
            scores[0] = d
            cornerX[0] = xs[i]
            cornerY[0] = y
    return 0

def Three_find_leftmost_Line(rect,x,y):
    comp = 9999999
    for a in rect:
        d = -y*a[0]+x*a[1]
        if d<comp:
            comp = d
            leftmostX = a[0]
            leftmostY = a[1]
    return leftmostX,leftmostY

def fitness_ring(cx,cy,radius,Func):
    global Q18_INT
    score = 0
    if Func:
        for i in range(0,2*radius,2):
            score += fitness_cell(cx - Q18_INT[radius] + Q18_INT[i], cy - Q18_INT[radius], Func)
            score += fitness_cell(cx - Q18_INT[radius], cy + Q18_INT[radius] - Q18_INT[i], Func)
            score += fitness_cell(cx + Q18_INT[radius], cy - Q18_INT[radius] + Q18_INT[i], Func)
            score += fitness_cell(cx + Q18_INT[radius] - Q18_INT[i], cy + Q18_INT[radius], Func)
        return score

    else:
        for i in range(0,radius*2,2):
            fitness_cell(cx - Q18_INT[radius] + Q18_INT[i], cy - Q18_INT[radius], Func)
            fitness_cell(cx - Q18_INT[radius], cy + Q18_INT[radius] - Q18_INT[i], Func)
            fitness_cell(cx + Q18_INT[radius], cy - Q18_INT[radius] + Q18_INT[i], Func)
            fitness_cell(cx + Q18_INT[radius] - Q18_INT[i], cy + Q18_INT[radius], Func)
        return 0

def fitness_capstone(x,y,Func):
    if Func:
        return fitness_cell(x,y,Func)+ fitness_ring( x, y, 1, Func) - fitness_ring(x, y, 2, Func) + fitness_ring(x, y, 3, Func)
    else:
        fitness_cell(x,y,Func)
        fitness_ring(x,y,1,Func)
        fitness_ring(x,y,2,Func)
        fitness_ring(x,y,3,Func)
        return 0

def fitness_cell(x,y,Func):
    global FB_Cell_32, Cell_Cnt,offsetsu,offsetsv 
    score = 0
    if Func:
        for i in range(0,5):           
            x,y = FB_Cell_32[Cell_Cnt] 
            Cell_Cnt+=1         
            if x<0 or y>=dst.shape[0] or y<0 or x>=dst.shape[1]:
                continue
            if dst[y][x]:
                score = score+1
            else:
                score = score-1
        return score
    else:
        for i in range(0,5):
            Cell_Cnt+=1
            X = x+offsetsu[i]
            Y = y+offsetsv[i]
            perspective_mapQFit(qr_c, X, Y,FB_Cell_32)
        return 0

def fitness_apat(cx,cy,Func):
    if Func:
        return fitness_cell(cx, cy, Func) - fitness_ring(cx, cy, 1, Func) + fitness_ring(cx, cy, 2, Func)
    else:
        fitness_cell(cx,cy,Func)
        fitness_ring(cx,cy,1,Func)
        fitness_ring(cx,cy,2,Func)

        return 0

def fitness_all(grid_size):
    version = (grid_size-17)/4
    score = 0
    global Cell_Cnt,Q18_INT,FB_Cell_32,GRID_SIZE
    GRID_SIZE = grid_size
    Cell_Cnt = 0
#***************************** False *******************************#
    #***********check the timing pattern****************#
    for i in range(0,int(grid_size-14),2):
        Q18_i = (i+7)*0x3ffff
        fitness_cell(Q18_i, Q18_INT[6],False)
        fitness_cell(Q18_INT[6],Q18_i,False)

    #***************check capstones*********************#
    Q18_i = (grid_size-4)*0x3ffff
    fitness_capstone(Q18_INT[3], Q18_INT[3], False)
    fitness_capstone(Q18_i, Q18_INT[3], False)
    fitness_capstone(Q18_INT[3], Q18_i, False)

    if version<0 or version>40:
        return 00000
    #*************check alignment pattern**************#
    ap_count = 0 #影像中间列alignment pattern的个数
    _, apat, _ = version_info(int(version))
    while ap_count<7 and apat[ap_count]:
        ap_count += 1
    for i in range(1,ap_count-1):
        Q18_i = (apat[i])*0x3ffff
        fitness_apat(Q18_INT[6], Q18_i, False)
        fitness_apat(Q18_i, Q18_INT[6], False)
    for i in range(1,ap_count):
        for j in range(1,ap_count):
            Q18_i = (apat[i])*0x3ffff
            Q18_j = (apat[j])*0x3ffff
            fitness_apat(Q18_i, Q18_j, False)
#****************************True*******************************************#
    Cell_Cnt=0
    #*******************check the timing pattern ********************#
    for i in range(0,int(grid_size-14),2):
        if i==0:
            expect=-1
        else:
            expect=1
        Q18_i = (i+7)*0x3ffff
        score += fitness_cell(Q18_i, Q18_INT[6], True) * expect
        score += fitness_cell(Q18_INT[6], Q18_i, True) * expect
    #********************* Check capstones 取点********************#
    Q18_i = (grid_size-4)*0x3ffff
    score += fitness_capstone(Q18_INT[3], Q18_INT[3], True)
    score += fitness_capstone(Q18_i, Q18_INT[3], True)
    score += fitness_capstone(Q18_INT[3], Q18_i, True)

    if version<0 or version>40:
        return score
    #*********** Check alignment patterns 取点***************#
    ap_count = 0
    while ap_count<7 and apat[ap_count]:
        ap_count+=1
    for i in range(1,ap_count-2):
        Q18_i = (apat[i])*0x3ffff
        score += fitness_apat(Q18_INT[6], Q18_i, True)
        score += fitness_apat(Q18_i, Q18_INT[6], True)
    for i in range(1,ap_count):
        for j in range(1,ap_count):
            Q18_i = (apat[i])*0x3ffff
            Q18_j = (apat[j])*0x3ffff
            score += fitness_apat(Q18_i, Q18_j, True)
    return score

                          

#*********************************************************#    
#**********************微调透视矩阵************************#
#*********************************************************#
def jiggle_perspective(c, grid_size):
    global qr_c
    p = 0
    
    best = fitness_all(grid_size)
    adjustments=[0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0]
    for i in range(0,8):
        adjustments[i] = (int(0.02*0x3ffff*qr_c[i])>>18)
    for p in range(0,2):
        for i in range(0,16):
            j = i>>1
            old = qr_c[j]
            step = adjustments[j]
            if i and 1:
                new = old+step
            else:
                new = old-step
            qr_c[j] = new
            test = fitness_all(grid_size)
            if (test>best):
                print('test-test-test-test-test')
                best = test
                if i==0:
                    i+=1
            else:
                qr_c[j] = old

        for i in range(0,8):
            adjustments[i] = (int(0.5*0x3ffff*adjustments[i])>>18)
    
def setup_qr_perspective(align, grid_size, cap, psd_ref):
    h = psd_ref[0]
    v = psd_ref[1]
    rect=[[0,0],[0,0],[0,0],[0,0]]
    if grid_size>21:
        rect[3] = Three_find_leftmost_Line(capstones[cap[0]],h,v)
        rect[1] = Three_find_leftmost_Line(capstones[cap[2]],h,v)
        rect[0] = Three_find_leftmost_Line(capstones[cap[1]],h,v)
        rect[2] = align
        perspectivate_setup(QR_C,rect, grid_size-7,grid_size-7)
    for i in range(0,9):
        qr_c[i] = QR_C[i]*0x3ffff
    jiggle_perspective(qr_c, grid_size) #--微调透视矩阵

def record_qr_grid(a,b,c):
    fourth_point = False
    align=[0,0]
    grid_size = 0
    h0_x=centerXX[a]
    h0_y=centerYY[a]
    rotate=False

    hd_x = centerXX[c] - centerXX[a]
    hd_y = centerYY[c] - centerYY[a]
    if (centerXX[b]-h0_x)*-hd_y + (centerYY[b]-h0_y)*hd_x > 0:
        swap = a
        a = c
        c = swap
        hd_x = -hd_x
        hd_y = -hd_y
        rotate=True
    cap = [a,b,c]
    align_region = -1
    
    for i in range(0,3):

        rotate_capstone(cap[i], h0_x, h0_y, hd_x, hd_y)#每一个定位符里边的角的位置
    grid_size = measure_timing_pattern(cap, grid_size)

    if (grid_size<0):          
        return 0
    if grid_size>21:
        if line_intersect(capstones[a][0], capstones[a][1], capstones[c][0], capstones[c][3], align)==0:
            return 0
        for i in range(0,3):
            a_num = int(np.sqrt((centerXX[i]-align[0])**2 + (centerYY[i]-align[1])**2))
            if a_num<MIN/2:
                fourth_point = True
                break
        if fourth_point:
            if line_intersect(capstones[a][3], capstones[a][0], capstones[c][3], capstones[c][2], align)==0:
                return 0
        align_region, reg_seed = find_alignment_pattern(cap,align,align_region)
        if align_region>=0:
            x = reg_seed[0]
            y = reg_seed[1]
            psd_ref_x = hd_x#水平向量
            psd_ref_y = hd_y#垂直向量
            scores=[0,0,0,0]
            scores[0] = -hd_y*x + hd_x*y
            fourth_result = flood_fill_seed(dst, x, y, dst[y][x], 2, find_leftmost_to_line, (psd_ref_x, psd_ref_y), scores)
    else:
        if line_intersect(capstones[a][3], capstones[a][2], capstones[c][1], capstones[c][2], align)==0:
            return 0



    setup_qr_perspective((cornerX[0],cornerY[0]), grid_size, cap , (psd_ref_x, psd_ref_y))
    cv2.circle(img,(psd_ref_x, psd_ref_y), 1, (250,0,0),-1)
        
    return 1

def test_neighbours(i,hnIndex,hnDistance,vnIndex,vnDistance,hlistCount,vlistCount):
    best_score = 0.0
    best_h = -1
    best_v = -1
    for j in range(0,hlistCount+1):
        for k in range(0,vlistCount+1):
            score = abs(1.0-hnDistance/vnDistance)

            if best_h<0 or score<best_score:
                best_h = hnIndex
                best_v = vnIndex
                best_score = score
    if best_h<0 or best_v<0:
        return -1
    # print('best_h, i, best_v=',best_h, i, best_v)
    record_qr_grid(best_h, i, best_v)

def test_grouping(matrixC,ii):
    vlistCount = 0
    hlistCount = 0
    ListCount  = 0

    u=0.0
    v=0.0   

    nIndex = 0
    nDistance = 0
    for j in range(0, pattern_NUM):
        c2=capstones[j]
        if (j==ii):
            continue       
        u=0.0
        v=0.0 
        u, v = perspective_unmap(matrixC,centerXX[j],centerYY[j])

        u = abs(u-3.5)
        v = abs(v-3.5)
        if (u<0.2*v):
            hlistCount+=1
            hnIndex = j
            hnDistance = v
        if (v<0.2*u):
            vlistCount+=1
            vnIndex = j
            vnDistance = u
    if (hlistCount==0 or vlistCount==0):
        return 1
    a=test_neighbours(ii,hnIndex,hnDistance,vnIndex,vnDistance,hlistCount,vlistCount)
    return a

def order_points(pts):
    rect=[[],[],[],[]]
    sum_pts = 0
    reduct_pts = 0
    for i in range(0,4):
        if sum(pts[i])>sum_pts:
            sum_pts=sum(pts[i])
            rect[2]=pts[i]
    for i in range(0,4):
        if sum(pts[i])<sum_pts:
            sum_pts=sum(pts[i])
            rect[0]=pts[i]
        
    
    for i in range(0,4):
        reduct_pts = pts[i][0]- pts[i][1]
        if reduct_pts<sum_pts:
            sum_pts=reduct_pts
            rect[3]=pts[i]
    for i in range(0,4):
        reduct_pts = pts[i][0]- pts[i][1]
        if reduct_pts>sum_pts:
            sum_pts=reduct_pts
            rect[1]=pts[i]            
    return rect

img = cv2.imread('test2.png')#rotate120.png  img2.bmp
image = img.copy()
imgCopy2 = img.copy()
isGO = True
Fourth_Align=0
Fourth_AlignXY = [0,0]
QR_C = [0,0,0,0,0,0,0,0,1]
qr_c = [0,0,0,0,0,0,0,0,1]

rect_NUM = []

GRID_SIZE=0
scores=[0,0,0,0]
capstones=[[(0,0),(0,0),(0,0),(0,0)],[(0,0),(0,0),(0,0),(0,0)],[(0,0),(0,0),(0,0),(0,0)]]
capstonesCopy=[[(0,0),(0,0),(0,0),(0,0)],[(0,0),(0,0),(0,0),(0,0)],[(0,0),(0,0),(0,0),(0,0)]]
b={(0,0),}
cc=[]
temp_rect=cc
centerXX=[0,0,0]
centerYY=[0,0,0]
cornerX=[0,0,0,0]
cornerY=[0,0,0,0]
distance=[0,0,0]
temp_rect=[]
matrixC = [[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0]]

Cell_Cnt = 0
FB_Cell_32 = []
offsetsu = [0x13332, 0x13332,0x1ffff,0x2cccc,0x2cccc]
offsetsv = [0x13332, 0x2cccc, 0x1ffff, 0x13332, 0x2cccc]
Q18_INT = [0x0, 0x3ffff, 0x7fffe, 0xBfffd, 0xffffc, 0x13fffb, 0x17fffa]
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
dst = cv2.adaptiveThreshold(gray ,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,25,5)
finder_scan(dst)
template = 0
NUM_SUM=len(cc)
remove = []
coordinate_sum=[0]*NUM_SUM
if NUM_SUM>3:
    for i in range(0,NUM_SUM):
        # print(i,cc[i])
        for j in range(0,4):
            coordinate_sum[i] += sum(cc[i][j])
    ssort = sorted(coordinate_sum)
    for i in range(0,NUM_SUM-1):
        Reduce = abs(ssort[i]-ssort[i+1])
        if Reduce<20:
            z = coordinate_sum.index(ssort[i])
            remove.append(z)
i = 0
for j in range(0,NUM_SUM):
    if j not in remove:
        capstones[i] = cc[j]
        # cv2.circle(imgCopy2, capstones[i][0], 2, (255, 0, 0), 1)
        # cv2.putText(imgCopy2, '0', capstones[i][0], cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 0, 0), 1, cv2.LINE_AA)
        # cv2.circle(imgCopy2, capstones[i][1], 2, (255, 0, 0), 1)
        # cv2.putText(imgCopy2, '1', capstones[i][1], cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 0, 0), 1, cv2.LINE_AA)
        # cv2.circle(imgCopy2, capstones[i][2], 2, (255, 0, 0), 1)
        # cv2.putText(imgCopy2, '2', capstones[i][2], cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 0, 0), 1, cv2.LINE_AA)
        # cv2.circle(imgCopy2, capstones[i][3], 2, (255, 0, 0), 1)
        # cv2.putText(imgCopy2, '3', capstones[i][3], cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 0, 0), 1, cv2.LINE_AA)
        i += 1

pattern_NUM = len(capstones)
for i in range(0,pattern_NUM):
    centerXX[i],centerYY[i] = centerFun(capstones[i],i)
    # if i==0:
    #     cv2.circle(imgCopy2, (centerXX[i],centerYY[i]), 2, (255, 0, 0), 1)
    #     cv2.putText(imgCopy2, '0', (centerXX[i],centerYY[i]), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (250,0,0), 1, cv2.LINE_AA)
    # if i==1:
    #     cv2.circle(imgCopy2, (centerXX[i],centerYY[i]), 2, (255, 0, 0), 1)
    #     cv2.putText(imgCopy2, '1', (centerXX[i],centerYY[i]), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (250,0,0), 1, cv2.LINE_AA)
    # if i==2:
    #     cv2.circle(imgCopy2, (centerXX[i],centerYY[i]), 2, (255, 0, 0), 1)
    #     cv2.putText(imgCopy2, '2', (centerXX[i],centerYY[i]), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (250,0,0), 1, cv2.LINE_AA)

distance[0] = np.sqrt((centerXX[0]-centerXX[1])**2 + (centerYY[0]-centerYY[1])**2)
distance[1] = np.sqrt((centerXX[0]-centerXX[2])**2 + (centerYY[0]-centerYY[2])**2)
distance[2] = np.sqrt((centerXX[2]-centerXX[1])**2 + (centerYY[2]-centerYY[1])**2)
MIN = np.min(distance)
for i in range(0,pattern_NUM):
    perspectivate_setup(matrixC[i], capstones[i],7.0,7.0)
    if pattern_NUM>2:
        result = test_grouping(matrixC[i],i)

#************************************************************************************#
#**************************************网格化*****************************************#
#************************************************************************************#
print('GRID_SIZE=',GRID_SIZE)
mask = np.zeros((GRID_SIZE, GRID_SIZE), np.uint8)
for x in range(0,GRID_SIZE):
    for y in range(0,GRID_SIZE):
        X,Y = perspective_mapQFit2(qr_c,x*0x3ffff+0x1FFFF,y*0x3ffff+0x1FFFF)
        cv2.circle(image, (X,Y),1,(250,0,0),-1)
        mask[y][x] = dst[Y][X]

plt.figure(1)
plt.subplot(133), plt.imshow(mask, 'gray'), plt.title('mask')
plt.subplot(131), plt.imshow(img, 'gray'), plt.title('gray')
plt.subplot(132), plt.imshow(image, 'gray'), plt.title('image')
plt.show()


