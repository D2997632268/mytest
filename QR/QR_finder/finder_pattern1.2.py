import numpy as np

import cv2

import matplotlib.pyplot as plt
# from numpy import *
import sys  
sys.setrecursionlimit(1000000)  
# import panads  as pd
np.set_printoptions(threshold=np.inf)

amplitude_amount = np.int32(0)  


#*********************************************************#    
#**************************水漫金山***********************#
#*********************************************************#  
def FloodFill2(image,x,y,book,n,cont):
    # print('cont=',cont)
    # print('n=',n)
    cont=cont+1
    if cont>n:
        return 0
    # print('cont=',cont)
    next=[[0,1],[1,0],[0,-1],[-1,0]]#8连通区域 ,[-1,-1],[1,-1],[-1,1],[1,1]   
    #枚举四个方向
    for i in range(0,4):
        tx = x + next[i][0]
        ty = y + next[i][1]
        # print(tx,ty)
        # cv2.circle(img, (tx,ty),1,(0,255,0),1)
        # cv2.circle(img, (x[0],y),1,(0,0,255),1)

        if tx <1 or tx >=img.shape[1] or ty<1 or ty>=img.shape[0]:
            continue
        if (image[tx][ty]==image[x][y]) and (book[tx][ty]==0):
            # cv2.circle(img, (tx,ty),1,(0,255,0),1)
            book[tx][ty]=1
            kk.append([tx,ty])
            FloodFill2(image,tx,ty,book,n,cont)
        else:
            continue
    return kk
def Add_Diff(k):
    print('k',k[0])
    XYAdd=[]
    XYDiff=[]
    for x in range(0,len(k)):
        XYAdd.append(k[x][0]+k[x][1])
        XYDiff.append(k[x][0]-k[x][1])
    # print('XYAdd=',XYAdd)
    return XYAdd,XYDiff
def FloodFill_Jinshan(image,x,y,n):
    cont=0
    book = np.zeros((dst.shape[0], dst.shape[1]), np.uint8)
    book[x][y]=1
    kk.clear()
    k=[] 
    # kk=[]
    # print('kk=',len(kk))
    # print('k=',len(k))
    rect = np.zeros((4,2),np.uint8)   
    # print('n=',n)
    k = FloodFill2(image,x,y,book,n,cont) 

    
    return k


#*********************************************************#    
#********************测试数据的黑白宽度*********************#
#*********************************************************#  
def image_width(y, pixel):
    # print('******************',y)
    # cv2.line(img,(0,y),(img.shape[1],y),(0,0,255),2)
    # print('pixel', pixel)
    
    width = []
    i = 1
    xNum = 1
    row = []
    for x in range(2, len(pixel)):
        if pixel[x - 1] == pixel[x]:
            # print('x',x)
            xNum += 1

        else:
            # print('Num',xNum)
            width.append(xNum)
            row.append(x)
            i += 1
            xNum = 1
            # print('******************',x)
        if x == len(pixel) - 1:
            # print("111111111111")
            width.append(xNum)
            row.append(x)
    return row, width

def image_width1(y, pixel):
    # print('******************',y)
    # cv2.line(img,(0,y),(img.shape[1],y),(0,0,255),2)
    # print('pixel', pixel)
    
    width = []
    i = 1
    xNum = 1
    row = []
    for x in range(2, len(pixel)):
        if pixel[x - 1] == pixel[x]:
            # print('x',x)
            xNum += 1

        else:
            # print('Num',xNum)
            width.append(xNum)
            row.append(x)
            i += 1
            xNum = 1
            # print('******************',x)
        if x == len(pixel) - 1:
            # print("111111111111")
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
    # cv2.line(img, (x, y), (Summ, y), (255, 50, 0), 2)
    Ycenter = 0

    n = 0
    k = 0
    for i in range(x, x + pd + pd1 + pd2 + pd3 + pd4):
        # print(i)
        k += n
        n = 0

        if 1:
            y_col, Ywidth = image_width1(i, image[:, i])
            # print('image[:, 50]=', image[:, i])

            for yw in range(0, len(Ywidth) - 4):
                # cv2.circle(img, (i, j), 8, (255, 0, 0), 1)
                pd = [Ywidth[yw], Ywidth[yw + 1], Ywidth[yw + 2],
                      Ywidth[yw + 3], Ywidth[yw + 4]]
                avg = (pd[0] + pd[1] + pd[3] + pd[4]) / 4
                err = avg * 3 / 4
                # print('Yavg', avg)
                # print('Yerr', err)
                ok = 1
                for j in range(0, 5):
                    if pd[j] < check[j] * avg - err or pd[j] > check[j] * avg + err + 1:
                        ok = 0
                        break
                #     print(check[j] * avg - err - 1, '...',
                #           Ywidth[yw + j], '..', check[j] * avg + err + 1)
                # print('ok', ok)

                if ok == 1:
                    n = 1
                    # print("print circle")
                    Ycenter = y_col[yw + 1] + \
                        (y_col[yw + 2] - y_col[yw + 1]) // 2
                    # cv2.circle(img, (xcenter, Ycenter), 1, (255, 0, 0), -1)
                    focus.append((xcenter, Ycenter))
                    XX = tuple(X)
                    # print('XX', XX)
                    # print('x', x, 'focus=', focus)
                    # print('focus111', focus)

                # if k == 2:
                #     break

    # if Ycenter != 0:
    #     cv2.circle(img, (xcenter, Ycenter), 1, (255, 0, 0), -1)

    # print('focus=', focus)
    for x in focus:
        if x not in new_focus:
            new_focus.append(x)
    # print('new_focus', new_focus)

    # for i in XX:
    #     if i not in XX:
    #         new_XX.append(x)
    # print('new_XX', new_XX)

    return new_focus, XX


#*********************************************************#    
#***********************漫水填充法************************#
#*********************************************************#  
def FloodFill(image, x, y):
    # left_region_code()
    # stone_region_code()
    # right_region_code()

    # cv2.circle(img, (x[0] - 1, y), 3, (255, 0, 0), 1)
    # print('image[x[0]-1, y]=', image[x[0] - 1, y])
    X = []
    Y = []
    output0 = cv2.floodFill(imgCopy, mask, (x[0] - 1, y),
                            (50, 50, 50), cv2.FLOODFILL_FIXED_RANGE)
    # print('output=', output0[0])

    output1 = cv2.floodFill(imgCopy, mask, (x[1] - 1, y),
                            (100, 100, 100), cv2.FLOODFILL_FIXED_RANGE)
    # print('output=', output1[0])

    output2 = cv2.floodFill(imgCopy, mask2, (x[2] - 1, y),
                            (255, 200, 0), cv2.FLOODFILL_FIXED_RANGE)
    # print('output2=', output2[0])

    output11 = cv2.floodFill(imgCopy, mask2, (x[3] - 1, y),
                             (255, 200, 0), cv2.FLOODFILL_FIXED_RANGE)
    # print('output=', output11[0])

    output00 = cv2.floodFill(imgCopy, mask2, (x[4] - 1, y),
                             (255, 0, 0), cv2.FLOODFILL_FIXED_RANGE)
    # print('output=', output00[0])

    if (output0[0] == output00[0]) and (output1[0] == output11[0]) and (output2[0] > 0) :
        x = tuple(x)
        X.append(x)
        Y.append(y)
        
    return tuple(X), tuple(Y)


#*********************************************************#    
#**********************计算透视矩阵************************#
#*********************************************************#  
def perspectivate_setup(rect,w,h):
    c=[]
    x0 = int(rect[0,0])
    y0 = int(rect[0,1])
    x1 = int(rect[1,0])
    y1 = int(rect[1,1])
    x2 = int(rect[2,0])
    y2 = int(rect[2,1])
    x3 = int(rect[3,0])
    y3 = int(rect[3,1])
    print('x0=',x0)
    print('y0=',y0)
    print('x1',x1)
    print('y1',y1)
    print('x2',x2)
    print('y2',y2)
    print('x3',x3)
    print('y3',y3)

    wden = w * (x2*y3 - x3*y2 + (x3-x2)*y1 + x1*(y2-y3))
    hden = h * (x2*y3 + x1*(y2-y3) - x3*y2 + (x3-x2)*y1)

    c.append ((x1*(x2*y3-x3*y2) + x0*(-x2*y3+x3*y2+(x2-x3)*y1) + x1*(x3-x2)*y0) // wden)
    c.append(-(x0*(x2*y3+x1*(y2-y3)-x2*y1) - x1*x3*y2 + x2*x3*y1+ (x1*x3-x2*x3)*y0) // hden)
    c.append(x0)#----a31
    c.append((y0*(x1*(y3-y2)-x2*y3+x3*y2) + y1*(x2*y3-x3*y2) +x0*y1*(y2-y3)) // wden)
    c.append((x0*(y1*y3-y2*y3) + x1*y2*y3 - x2*y1*y3 +y0*(x3*y2-x1*y2+(x2-x3)*y1)) // hden)
    c.append (y0)#----a32
    c.append((x1*(y3-y2) + x0*(y2-y3) + (x2-x3)*y1 + (x3-x2)*y0) // wden)
    c.append((-x2*y3 + x1*y3 + x3*y2 + x0*(y1-y2) - x3*y1 + (x2-x1)*y0) //hden)
    c.append(1)

    # a12 = int( ((x1-x2)(y0-y1+y2-y3)-(y1-y2)(x0-x1+x2-x3)) // ((x1-x2)(y3-y2)-(y1-y2)(x3-x2)))
    # a13 = int(((x0-x1+x2-x3)(y3-y2)-(y0-y1+y2-y3)(x3-x2)) // ((x1-x2)(y3-y2)-(x3-x2)(y1-y2)))
    # a11 = int(x1-x0+a12*x1)
    # a21 = int(x3-x0+a12*x3)
    # a31 = int(x0)
    # a23 = int((a21+a31-x3)/x3)
    # a22 = int(y3-y0+a23*y3)
    # a32 = int(y0)
    # a33 = 1
    # print('a=',a11,a12,a13,a21,a22,a23,a31,a32,a33)

    return c


#*********************************************************#    
#**********************回型定位符对比************************#
#*********************************************************# 
def Compare(image,x,y):
    # print('111111')

    X=[]
    Y=[]
    
    n=(image.shape[0]*image.shape[1])//4
    # print('n==',n)

    K0 = FloodFill_Jinshan(image,x[0]-1,y,n)    
    a = len(K0)
    # print('k[0]=',a)
    if K0==0:
           return 0   
    i0,docCnt0 = FourPointContours(image,K0)   
    # print('reshape',docCnt0.shape)
    if i0==0:
        return 0 
    # print('docCont0=',docCnt0)
    # res = cv2.drawContours(img, docCnt0, -1, (0,0,255),1) 
    cv2.circle(img,(docCnt0[0][0],docCnt0[0][1]),4,(255,0,0),1)
    cv2.circle(img,(docCnt0[1][0],docCnt0[1][1]),4,(255,0,0),1)
    cv2.circle(img,(docCnt0[2][0],docCnt0[2][1]),4,(255,0,0),1)
    cv2.circle(img,(docCnt0[3][0],docCnt0[3][1]),4,(255,0,0),1)

    c=perspectivate_setup(docCnt0,7,7)
    print('out_c',c)
    

    # print('2222222')
    K1 = FloodFill_Jinshan(image,x[1]-1,y,a)  
    if K1==0:
        return 0
    b = len(K1)  
    # print('len(k)',b)
    i1,docCnt1 = FourPointContours(image,K1) 
    if i1==0:
        return 0 
    # print('docCont1=',docCnt1)
    # res = cv2.drawContours(img, docCnt1, -1, (0,0,255),1) 
    # cv2.circle(img,(docCnt1[0][0],docCnt1[0][1]),4,(255,0,0),1)
    # cv2.circle(img,(docCnt1[1][0],docCnt1[1][1]),4,(255,0,0),1)
    # cv2.circle(img,(docCnt1[2][0],docCnt1[2][1]),4,(255,0,0),1)
    # cv2.circle(img,(docCnt1[3][0],docCnt1[3][1]),4,(255,0,0),1)


    # print('333333')
    K2 = FloodFill_Jinshan(image,x[2]-1,y,b)  
    if K2==0:
        return 0
    c = len(K2) 
    # print('len(K2)',c)
    i2,docCnt2 = FourPointContours(image,K2) 
    if i2==0:
        return 0 
    # print('docCont2=',docCnt2)
    # res = cv2.drawContours(img, docCnt2, -1, (0,0,255),1) 
    cv2.circle(img,(docCnt2[0][0],docCnt2[0][1]),4,(255,100,0),1)
    cv2.circle(img,(docCnt2[1][0],docCnt2[1][1]),4,(255,100,0),1)
    cv2.circle(img,(docCnt2[2][0],docCnt2[2][1]),4,(255,100,0),1)
    cv2.circle(img,(docCnt2[3][0],docCnt2[3][1]),4,(255,100,0),1)

    c1=perspectivate_setup(docCnt2,7,7)
    print('in_c1',c1)

    # print('44444')
    K3 = FloodFill_Jinshan(image,x[3]-1,y,b) 
    if K3==0:
         return 0 
    d = len(K3)  
    # print('len(K3)',d)
    i3,docCnt3 = FourPointContours(image,K3) 
    if i3==0:
        return 0 
    # print('docCont3=',docCnt3)
    # res = cv2.drawContours(img, docCnt3, -1, (0,0,255),1) 
    # cv2.circle(img,(docCnt3[0][0],docCnt3[0][1]),4,(255,0,0),1)
    # cv2.circle(img,(docCnt3[1][0],docCnt3[1][1]),4,(255,0,0),1)
    # cv2.circle(img,(docCnt3[2][0],docCnt3[2][1]),4,(255,0,0),1)
    # cv2.circle(img,(docCnt3[3][0],docCnt3[3][1]),4,(255,0,0),1)

    # print('555555')
    K4 = FloodFill_Jinshan(image,x[4]-1,y,a)  
    if K4==0:
        return 0
    e = len(K4)  
    # print('k[4]=',e)
    i4,docCnt4 = FourPointContours(image,K4) 
    if i4==0:
        return 0 
    # print('docCont4=',docCnt4)
    # res = cv2.drawContours(img, docCnt4, -1, (0,0,255),1) 
    # cv2.circle(img,(docCnt4[0][0],docCnt4[0][1]),4,(255,0,0),1)
    # cv2.circle(img,(docCnt4[1][0],docCnt4[1][1]),4,(255,0,0),1)
    # cv2.circle(img,(docCnt4[2][0],docCnt4[2][1]),4,(255,0,0),1)
    # cv2.circle(img,(docCnt4[3][0],docCnt4[3][1]),4,(255,0,0),1)

    if  a==e and b==d and b>c:#and K0!=0 and K1!=0 and K2!=0 and K3!=0 and K4!=0
        X.append(x)
        Y.append(y)
        return X,Y,K0,K1,K2,K3,K4
    else:
        return 0
    

#*********************************************************#    
#************************确定外轮廓*************************#
#*********************************************************# 
def FourPointContours(image,stone):
    # docCnt.clear()
    # print('x',x)
    mask_test = np.zeros((image.shape[0], image.shape[1]), np.uint8)
    # print('len.stone',len(stone))
    # mask_test[stone] = 255
    # plt.imshow(mask_test, 'gray'), plt.title('mask_test'), plt.show()
    for i in range(0,len(stone)):
        mask_test[stone[i][0],stone[i][1]]=255

    # plt.imshow(mask_test, 'gray'), plt.title('mask'), plt.show()
    binary, contours, hierarchy = cv2.findContours(mask_test, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

    # res = cv2.drawContours(img, contours, -1, (0,0,255),1)
    # print('for循环')
    for c in contours:
        # 近似
        peri = cv2.arcLength(c, True)
        approx = cv2.approxPolyDP(c, 0.02 * peri, True)

        # 准备做透视变换
        # print('if判定')
        if len(approx) == 4:
            # print('4')
            docCnt = approx
            # docCnt = docCnt.reshape(4,2)
            # print('doccnt=',docCnt)
            return 1,docCnt.reshape(4,2)

    return 0,0


#*********************************************************#    
#************************水平扫描*************************#
#*********************************************************#  
def finder_scan(image):
    check = [1, 1, 3, 1, 1]
    ok = 0
    print('image.shape=', image.shape)
    for y in range(0, image.shape[0]):
        if 1:#y==50:
            # cv2.line(img, (0,y),(img.shape[1],y),(0,255,0),2)

            # print("****************")
            # print('Y1=',y)   
            x_row, Xwidth = image_width(y, image[y, :])
            # print('Y2=',y)ex
            # # print('image[y, :]=', image[y, :])
            # # print('x_row',x_row)
            # # print("****************")
            
            for xW in range(0, len(Xwidth) - 4):
                pd = [Xwidth[xW], Xwidth[xW + 1], Xwidth[xW + 2],
                      Xwidth[xW + 3], Xwidth[xW + 4]]
                x = [x_row[xW], x_row[xW + 1], x_row[xW + 2],
                     x_row[xW + 3], x_row[xW + 4]]
                avg = (pd[0] + pd[1] + pd[3] + pd[4]) / 4
                err = avg * 3 / 4
                # print('avg',avg)
                # print('err',err)
                ok = 1
                for i in range(0, 5):
                    if pd[i] < check[i] * avg - err or pd[i] > check[i] * avg + err + 1:
                        ok = 0
                if ok == 1:  # 水平扫描结束    
                    Xcenter = x_row[xW + 1] + \
                        (x_row[xW + 2] - x_row[xW + 1]) // 2
                    # cv2.circle(img, (x_row[xW + 1], y), 1, (255, 0, 0), 1)
                    focus, XX = test_vertical(image, x_row[xW], y, pd[0], pd[1],
                                              pd[2], pd[3], pd[4], Xcenter, x)

                    if len(XX)>3 and y != [] :
                        WW = Compare(image,XX,y)
                        if WW!=0:
                            Row=WW[0]
                            Col=WW[1]
                            # print('Row,Col', Row, Col)
                            cv2.circle(
                                imgCopy2, (Row[0][0] - 1, Col[0]), 1, (255, 0, 0), -1)
                            cv2.circle(
                                imgCopy2, (Row[0][1] - 1, Col[0]), 1, (255, 0, 0), -1)
                            cv2.circle(
                                imgCopy2, (Row[0][2] - 1, Col[0]), 1, (0, 0, 255), -1)
                            cv2.circle(
                                imgCopy2, (Row[0][3] - 1, Col[0]), 1, (255, 0, 0), -1)
                            cv2.circle(
                                imgCopy2, (Row[0][4] - 1, Col[0]), 1, (255, 0, 0), -1)
                            # cv2.floodFill(imgCopy2, mask3, (Row[0][2] - 10, Col[0]),
                            #           (255, 0, 0), cv2.FLOODFILL_FIXED_RANGE)
                            # print("len(WW[2][0])",len(WW[2][0]))
                            # print("len(WW[4][0])",len(WW[4][0]))
                            # FourPointContours(image,WW[2],WW[4])

                    

img = cv2.imread('1.png')

global kk ,docCnt0, docCnt1,docCnt2,docCnt3,docCnt4 #, K0,K1,K2,K3,K4
kk=[]
# K0=[]
# K1=[]
# K2=[]
# K3=[]
# K4=[]
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

dst = cv2.adaptiveThreshold(gray ,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,25,5)
plt.imshow(dst, 'gray'), plt.title('dst'), plt.show()


mask = np.zeros((dst.shape[0] + 2, dst.shape[1] + 2), np.uint8)

mask2 = np.zeros((dst.shape[0] + 2, dst.shape[1] + 2), np.uint8)

mask3 = np.zeros((dst.shape[0] + 2, dst.shape[1] + 2), np.uint8)

imgCopy = (cv2.cvtColor(dst,cv2.COLOR_GRAY2RGB)).copy()
imgCopy2 = (cv2.cvtColor(dst,cv2.COLOR_GRAY2RGB)).copy()

# print('dat',dst)
finder_scan(dst)
# print('focus', focus)
# cv2.floodFill()
plt.imshow(img, 'gray'), plt.title('gray'), plt.show()
# cv2.imshow('img',img)

plt.imshow(imgCopy2, 'gray'), plt.title('imgCopy2'), plt.show()
# plt.imshow(draw_img, 'gray'), plt.title('draw_img'), plt.show()
# cv2.waitKey()
# cv2.destroyAllWindows()