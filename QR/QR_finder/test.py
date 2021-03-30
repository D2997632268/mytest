import cv2

import numpy as np

import operator

from functools import reduce

import matplotlib.pyplot as plt 

np.set_printoptions(threshold=np.inf)
#(42,42)  (32,32)

#第50行 48列 （50,48）
def flood_fill_seed(q, x, y, pixel, num):
	k=1
	LEN=9999
	X=[]
	Y=[]
	LEFT=[]
	RIGHT = []
	p = q.copy()
	row =  p.reshape(-1)
	# print('1:',row[0])
	# print(row)
	print(row.shape)
	print('pixel=',pixel)
	h = img.shape[0]
	w = img.shape[1]
	# print('h=',h)
	# print('w=',w)
	while (1):
		print('----------------')
		left = y
		right = y
		pos = x*w
		
		while (left>0 and pos+left-1>0 and row[pos+left-1] == pixel):
			left = left-1						

		while (right<w-1 and pos+right+1 <w*h-1 and row[pos+right+1] == pixel):
			right = right+1

		# print('left,right',left,right)
		for i in range(left, right+1):
			# print('rowNUM',pos+i,row[pos+i])
			row[pos+i] = num
			# print('rowNUM',pos+i,row[pos+i])
			cv2.circle(image, (x,i), 1, (255, 0, 0), -1)

		# print('k, len',k, LEN)
		# print('x, y',x,y)

		print('x,y,left,right=',x,y,left,right)

		while (1):
			if y<=w and y>=0 and x<=h and x>=0 and k>=1 and k<=LEN+1:
				if (x>0):
					# print('****1***')
					pos = (x-1)*w
					recurse = False
					for i in range(left,right+1):
						if (row[pos+i] == pixel):
							k=1
							# print('***2****')
							Y.append(i)
							X.append(x)
							LEFT.append(left)
							RIGHT.append(right)
							y = i
							x = x - 1
							recurse = True
							LEN = len(Y)
							# print('X=',X)
							print('row',pos+i,row[pos+i])
							break
					if recurse:
						break
				if (x<h-1):
					# print('****3***')
					pos = (x+1)*w
					recurse = False
					# print('row[pos+i]=',row[pos+i])
					for i in range(left, right+1):
						if (row[pos+i] == pixel):
							# print('***4****')
							k=1
							Y.append(i)
							X.append(x)
							LEFT.append(left)
							RIGHT.append(right)
							LEN = len(Y)
							y = i
							x = x + 1
							recurse = True
							# print('row',pos+i,row[pos+i])
							# print('X=',X)
							break
					if recurse:
						break
				if y>w and y<0 and x>h and x<0 :#1:
					return 1
				print('len-k=',LEN-k)
				x = X[LEN-k]
				y = Y[LEN-k]
				left = LEFT[LEN-k]
				right = RIGHT[LEN-k]
				print('k---x,y,left,right=',x,y,left,right)
				k=k+1  #检测第几次未找到相同行
			else:
				return 0

image = cv2.imread('1.png')

gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

img = cv2.adaptiveThreshold(gray ,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,25,5)
# print(img.shape[0])
# print(img.shape[1])
# image[10][60]=0
cv2.circle(image, (166,50), 1, (255, 0, 0), -1)
plt.imshow(image,'gray'),plt.title('color'),plt.show()
# print(img[35][60])
result = flood_fill_seed(img, 166, 50, img[50][166], 1)

print('result=',result)
# plt.imshow(img,'gray'),plt.title('gray'),plt.show()
plt.imshow(image,'gray'),plt.title('color'),plt.show()
