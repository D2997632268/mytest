import numpy as np 

import cv2

import  matplotlib.pyplot as plt 
from numpy import *

# import panads  as pd 
np.set_printoptions(threshold=np.inf)

'''
image = cv2.imread('1.png')

# image = np.zeros([200,200,3],np.uint8)
# image[79:150,79:150,:] = 255
# cv2.imshow('image',image)

# mask = np.zeros([202,202,1], np.uint8)
# mask[100:150,100:150] = 0
# cv2.imshow('mask',mask)
# cv2.floodFill(image, mask, (100,100), (0,0,255), cv2.FLOODFILL_MASK_ONLY)

# cv2.imshow('fill',image)

def fill_image(image):
    copyImage=image.copy() #复制图片
    h,w=image.shape[:2]
    mask=np.zeros([h+2,w+2],np.uint8)
    #(0,80) 起始点,(0,100,255) 蓝色 ,(100,100,50)棕色 ,(50,50,50) 浅黑
    output = cv2.floodFill(copyImage,mask,(40,47),(255,0,0),cv2.FLOODFILL_FIXED_RANGE)
    # cv2.imshow('floodfill',copyImage)
    # plt.imshow('floodfill',copyImage),
    plt.imshow(copyImage,'gray'),plt.title('copyImage'),plt.show()
    print(output[2])

fill_image(image)
# cv2.waitKey()
# cv2.destroyAllWindows()
'''
def image_width(y,pixel):
	# cv2.line(img,(0,y),(img.shape[1],y),(0,0,255),2)
	# print('pixel',pixel)
	width=[]
	i=1
	xNum = 1
	row=[]
	for x in range(2,len(pixel)):
		if pixel[x-1]==pixel[x]:
			# print('x',x)
			xNum +=1
			
		else:
			# print('Num',xNum)
			width.append(xNum)
			row.append(x)
			i += 1
			xNum = 1
			# print('******************',x)
		if x==len(pixel)-1:
			# print("111111111111")
			width.append(xNum)
			row.append(x)
	# print('width',width)
	# print('sum',sum(width))
	# # print(width.type)	
	# print('row',row)
	return row,width	

def finder_scan(image):
	check=[1,1,3,1,1]
	print('image.shape=',image.shape)
	for y in range(1,image.shape[0]):
		# if y == 50:
			# cv2.line(img, (0,y),(img.shape[1],y),(0,255,0),2)

			x_row,Xwidth = image_width(y,image[y,:])
			# print('x_row',x_row)
			# print("****************")

			for xW in range(0,len(Xwidth)-4):
				avg = (Xwidth[xW]+Xwidth[xW+1]+Xwidth[xW+3]+Xwidth[xW+4])/4
				err = avg*1/4
				# print('avg',avg)
				# print('err',err)
				ok = 1
				for i in range(0,5):
					if Xwidth[xW+i]<check[i]*avg-err or Xwidth[xW+i]>check[i]*avg+err+1:
						ok = 0
						# break
				# 	print(check[i]*avg-err-1,'...', Xwidth[xW+i],'..',check[i]*avg+err+1)
				# print('ok',ok)

				if ok==1:#水平扫描结束
					# print("print circle")
					cv2.circle(img,(x_row[xW-1],y),8,(255,0,0),1)

						# print('Xwidth',Xwidth)
				

	



img = cv2.imread('1.png')

gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)


# ret, dst = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 25,10)
dst = cv2.adaptiveThreshold(gray ,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,25,5)
# dst = cv2.adaptiveThreshold(gray,255,cv2.ADAPTIVE_THRESH_MEAN_C,cv2.THRESH_BINARY,25,5)
# plt.imshow(dst,'gray'),plt.title('img1'),plt.show()

# contours = cv2.findContours(dst, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)[1]
# cv2.drawContours(img, contours, -1, (255,0,0),2)

# print('binaryzation=',dst)

finder_scan(dst)

# line_1=[]
# print('dst.shape=',dst.shape)
# cv2.line(img,(0,220),(dst.shape[1],220),(0,0,255),2)
# line_1 = dst[:,220]
# print(line_1)
# plt.plot(line_1),plt.title('line'),plt.show()
# print(len(line_1))
plt.imshow(img ,'gray'),plt.title('img'),plt.show()


