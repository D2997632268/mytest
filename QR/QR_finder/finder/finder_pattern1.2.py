import numpy as np

import cv2

import matplotlib.pyplot as plt

import sys  

import FloodFillJinShan as FFJS

np.set_printoptions(threshold=np.inf)

#*********************************************************#    
#********************测试数据的黑白宽度*********************#
#*********************************************************#  

def image_width(y, pixel):
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
            y_col, Ywidth = image_width(i, image[:, i])
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
#***********************漫水填充法************************#
#*********************************************************#  
def CompareFloodFill(image, x, y):
    
    X = []
    Y = []

    K1 =FFJS.FloodFill_Jinshan(image,x[0]-1,y)
   
    print('len(K1)=', len(K1))

    # output1 = cv2.floodFill(imgCopy, mask, (x[1] - 1, y),
    #                         (100, 100, 100), cv2.FLOODFILL_FIXED_RANGE)
    K2 =FFJS.FloodFill_Jinshan(image,x[1] - 1, y)
    print('len(K2)=', len(K2))

    # output2 = cv2.floodFill(imgCopy, mask2, (x[2] - 1, y),
    #                         (255, 200, 0), cv2.FLOODFILL_FIXED_RANGE)
    K3 =FFJS.FloodFill_Jinshan(image,x[2] - 1, y)
    print('len(K3)=', len(K3))

    # output11 = cv2.floodFill(imgCopy, mask2, (x[3] - 1, y),
    #                          (255, 200, 0), cv2.FLOODFILL_FIXED_RANGE)
    K22 =FFJS.FloodFill_Jinshan(image,x[3] - 1, y)
    print('len(K22)=', len(K22))

    # output00 = cv2.floodFill(imgCopy, mask2, (x[4] - 1, y),
    #                          (255, 0, 0), cv2.FLOODFILL_FIXED_RANGE)
    K11 =FFJS.FloodFill_Jinshan(image,x[4] - 1, y)
    print('len(K11)=', len(K11))

    if (len(K1) == len(K11)) and (len(K2) == len(K22)) and (len(K3) > 0) :
        x = tuple(x)
        X.append(x)
        Y.append(y)
        print('111111111111111111111')
        
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
    # print('x0=',x0)
    # print('y0=',y0)
    # print('x1',x1)
    # print('y1',y1)
    # print('x2',x2)
    # print('y2',y2)
    # print('x3',x3)
    # print('y3',y3)

    wden = w * (x2*y3 - x3*y2 + (x3-x2)*y1 + x1*(y2-y3))
    hden = h * (x2*y3 + x1*(y2-y3) - x3*y2 + (x3-x2)*y1)

    c.append ((x1*(x2*y3-x3*y2) + x0*(-x2*y3+x3*y2+(x2-x3)*y1) + x1*(x3-x2)*y0) // wden)
    c.append(-(x0*(x2*y3+x1*(y2-y3)-x2*y1) - x1*x3*y2 + x2*x3*y1+ (x1*x3-x2*x3)*y0) // hden)
    c.append(x0)
    c.append((y0*(x1*(y3-y2)-x2*y3+x3*y2) + y1*(x2*y3-x3*y2) +x0*y1*(y2-y3)) // wden)
    c.append((x0*(y1*y3-y2*y3) + x1*y2*y3 - x2*y1*y3 +y0*(x3*y2-x1*y2+(x2-x3)*y1)) // hden)
    c.append (y0)
    c.append((x1*(y3-y2) + x0*(y2-y3) + (x2-x3)*y1 + (x3-x2)*y0) // wden)
    c.append((-x2*y3 + x1*y3 + x3*y2 + x0*(y1-y2) - x3*y1 + (x2-x1)*y0) //hden)
    c.append(1)

    return c



#*********************************************************#    
#************************水平扫描*************************#
#*********************************************************#  
def finder_scan(image):
    check = [1, 1, 3, 1, 1]
    ok = 0
    print('image.shape=', image.shape)
    for y in range(0, image.shape[0]):
        if y==50:
            # cv2.line(img, (0,y),(img.shape[1],y),(0,255,0),2)

            x_row, Xwidth = image_width(y, image[y, :])
            # print('image[y, :]=', image[y, :])
            # print('x_row',x_row)
            # print("****************")

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
                        # break
                    #   print(check[i]*avg-err-1,'...', Xwidth[xW+i],'..',check[i]*avg+err+1)
                    # print('ok',ok)

                if ok == 1:  # 水平扫描结束
                    # print("print circle")

                    Xcenter = x_row[xW + 1] + \
                        (x_row[xW + 2] - x_row[xW + 1]) // 2
                    # cv2.circle(img, (x_row[xW + 1], y), 1, (255, 0, 0), 1)
                    focus, XX = test_vertical(image, x_row[xW], y, pd[0], pd[1],
                                              pd[2], pd[3], pd[4], Xcenter, x)
                    # focus = tuple(focus)
                    # print('focus111', focus)
                    # print('len',len(focus))
                    # print('XX_origin', XX)

                    if XX != [] and y != [] :
                        # cv2.circle(img, (XX[0], y), 1, (255, 0, 0), 1)
                        # book = np.zeros((dst.shape[0], dst.shape[1]), np.uint8)
                        # book[XX[1]][y]=1#标记起始位置为1
                        # print('XX[3]', XX[3])
                        # print('111111111111')
                        # print('XX_origin', XX,y)
                        # k = FloodFill2(image,XX[3],y,book)
                        # KK.append(k)
                        # print('K',k)
                        # print('len',len(k))

                        # K,rect =FFJS.FloodFill_Jinshan(image,XX[0]-1,y)
                        Row, Col = CompareFloodFill(image,XX,y)
                        # print('sum',len(k))
                        # setup = perspectivate_setup(rect,7,7)
                        # print('setup=',setup)
                        # k=np.zeros((4,2),np.uint8)        
                        # for i in focus:
                        #   cv2.circle(img, i, 1, (0, 255, 0), -1)
                        #   print('len--i',i)
                        # Row, Col = FloodFill(image, XX, y)
                        # print('Row,Col', Row, Col)
                        # # print(Row[0][1])
                        # cv2.circle(
                        #     img, (Row[0][2] - 1, Col[0]), 1, (0, 0, 255), -1)
                        # if Row != () and Col != ():
                        #     print('Row,Col', Row, Col)
                            # cv2.circle(
                            #     imgCopy2, (Row[0][0] - 1, Col[0]), 1, (255, 0, 0), -1)
                            # cv2.circle(
                            #     imgCopy2, (Row[0][1] - 1, Col[0]), 1, (255, 0, 0), -1)
                            # cv2.circle(
                            #     imgCopy2, (Row[0][2] - 1, Col[0]), 1, (0, 0, 255), -1)
                            # cv2.circle(
                            #     imgCopy2, (Row[0][3] - 1, Col[0]), 1, (255, 0, 0), -1)
                            # cv2.circle(
                            #     imgCopy2, (Row[0][4] - 1, Col[0]), 1, (255, 0, 0), -1)


                        # cv2.floodFill(imgCopy2, mask3, (Row[0][2] - 10, Col[0]),
                        #               (255, 0, 0), cv2.FLOODFILL_FIXED_RANGE)
                        # pass

    return 1


img = cv2.imread('1.png')
# global kk
# kk=[]
# sum2 = 1
# imgCopy = img.copy()
# imgCopy2 = img.copy()
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# imgCopy = gray.copy()
# imgCopy2 = gray.copy()

# mask = np.zeros([img.shape[0] + 2, img.shape[1] + 2], np.uint8)

# mask2 = np.zeros([img.shape[0] + 2, img.shape[1] + 2], np.uint8)

# mask3 = np.zeros([img.shape[0] + 2, img.shape[1] + 2], np.uint8)
# focus = []
dst = cv2.adaptiveThreshold(gray ,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,25,5)

mask = np.zeros((dst.shape[0] + 2, dst.shape[1] + 2), np.uint8)

mask2 = np.zeros((dst.shape[0] + 2, dst.shape[1] + 2), np.uint8)

mask3 = np.zeros((dst.shape[0] + 2, dst.shape[1] + 2), np.uint8)

imgCopy = (cv2.cvtColor(dst,cv2.COLOR_GRAY2RGB)).copy()
imgCopy2 = (cv2.cvtColor(dst,cv2.COLOR_GRAY2RGB)).copy()



focus = finder_scan(dst)
# print('focus', focus)
# cv2.floodFill()
plt.imshow(img, 'gray'), plt.title('gray'), plt.show()
# cv2.imshow('img',img)

plt.imshow(imgCopy2, 'gray'), plt.title('imgCopy2'), plt.show()

# cv2.waitKey()
# cv2.destroyAllWindows()