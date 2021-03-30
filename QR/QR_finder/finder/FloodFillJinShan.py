import numpy as np

import cv2


#*********************************************************#    
#**************************水漫金山***********************#
#*********************************************************#  
def FloodFill2(image,x,y,book,kk):
    next=[[0,1],[1,0],[0,-1],[-1,0]]#8连通区域    
    #枚举四个方向
    for i in range(0,4):
        tx = x + next[i][0]
        ty = y + next[i][1]
        # print(tx,ty)
        # cv2.circle(img, (tx,ty),1,(0,255,0),1)
        # cv2.circle(img, (x[0],y),1,(0,0,255),1)

        if tx <1 or tx >=image.shape[1] or ty<1 or ty>=image.shape[0]:
            continue
        if (image[tx][ty]==image[x][y]) and (book[tx][ty]==0):
            cv2.circle(image, (tx,ty),1,(0,255,0),1)
            book[tx][ty]=1
            kk.append([tx,ty])
            FloodFill2(image,tx,ty,book,kk)
    
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
def FloodFill_Jinshan(image,x,y):
	kk=[]
	book = np.zeros((image.shape[0], image.shape[1]), np.uint8)
	book[x][y]=1
	k=[] 
	print('kk=',len(kk))
	print('k=',len(k))
	# rect = np.zeros((4,2),np.uint8)   
	k = FloodFill2(image,x,y,book,kk)
	print('kk=',len(kk))
	print('k=',len(k))

	# add, diff = Add_Diff(k)

	# # print('add=',add)
	# a = np.argmin(add)
	# b = np.argmax(add)
	# c = np.argmin(diff)
	# d = np.argmax(diff)
	# # print('rect=',a,b,c,d)

	# # cv2.circle(image, (k[a][0], k[a][1]), 1, (255, 0, 0), 1)#rect[0]
	# # cv2.putText(image, '0', (k[a][0]-10, k[a][1]), cv2.FONT_HERSHEY_SIMPLEX, 0.25, (255,0,0), 1, cv2.LINE_AA)

	# rect[0] = [k[a][0], k[a][1]]

	# # cv2.circle(image, (k[d][0], k[d][1]), 1, (255, 0, 0), 1)#rect[1] 
	# # cv2.putText(image, '1', (k[d][0]+10, k[d][1]), cv2.FONT_HERSHEY_SIMPLEX, 0.25, (255,0,0), 1, cv2.LINE_AA)

	# rect[1] = [k[d][0], k[d][1]]

	# # cv2.circle(image, (k[b][0], k[b][1]), 1, (255, 0, 0), 1)#rect[2]
	# # cv2.putText(image, '2', (k[b][0]+10, k[b][1]), cv2.FONT_HERSHEY_SIMPLEX, 0.25, (255,0,0), 1, cv2.LINE_AA)

	# rect[2] = [k[b][0], k[b][1]]

	# # cv2.circle(image, (k[c][0], k[c][1]), 1, (255, 0, 0), 1)#rect[3]
	# # cv2.putText(image, '3', (k[c][0]-10, k[c][1]), cv2.FONT_HERSHEY_SIMPLEX, 0.25, (255,0,0), 1, cv2.LINE_AA)

	# rect[3] = [k[c][0], k[c][1]]

	# print('rect=',rect)
	return k#,rect
