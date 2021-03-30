import cv2

import numpy as np

import operator

from functools import reduce

import matplotlib.pyplot as plt 

np.set_printoptions(threshold=np.inf)
#(42,42)  (32,32)


def flood_fill_seed(q, x, y, num):
	pixel = q[y][x]
	area_count = 0
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
		left = x
		right = x
		pos = y*w
		
		while (left>0 and pos+left-1>0 and row[pos+left-1] == pixel):
			left = left-1						

		while (right<w-1 and pos+right+1 <w*h-1 and row[pos+right+1] == pixel):
			right = right+1

		# print('left,right',left,right)
		for i in range(left, right+1):
			# print('rowNUM',pos+i,row[pos+i])
			row[pos+i] = num
			# print('rowNUM',pos+i,row[pos+i])
			cv2.circle(image, (i,y), 1, (255, 0, 0), -1)

		if 1:
			area_count += (right-left+1)
			# if area_count>600:
			# 	return 600

		# print('k, len',k, LEN)
		# print('x, y',x,y)

		print('x,y,left,right=',x,y,left,right)

		while (1):
			if y<=h and y>=0 and x<=w and x>=0 and k>=1 and k<=LEN:
				if (y>0):
					# print('****1***')
					pos = (y-1)*w
					recurse = False
					for i in range(left,right+1):
						if (row[pos+i] == pixel):
							k=1
							# print('***2****')
							X.append(i)
							Y.append(y)
							LEFT.append(left)
							RIGHT.append(right)
							x = i
							y = y - 1
							recurse = True
							LEN = len(y)
							print('Y=',Y)
							# print('X=',X)
							print('row',pos+i,row[pos+i])
							break
					if recurse:
						break
				if (y<h-1):
					# print('****3***')
					pos = (y+1)*w
					recurse = False
					# print('row[pos+i]=',row[pos+i])
					for i in range(left, right+1):
						if (row[pos+i] == pixel):
							# print('***4****')
							k=1

							X.append(i)
							Y.append(y)
							LEFT.append(left)
							RIGHT.append(right)
							LEN = len(X)
							x = i
							y = y + 1
							recurse = True
							# print('row',pos+i,row[pos+i])
							print('y',y)
							print('Y=',Y)
							break
					if recurse:
						break
				if y>h or y<0 or x>w or x<0 or (LEN-k)==9998 or (LEN-k)<0 :#1:
					return 1
				print('len=',LEN)
				print('len-k=',LEN-k)
				x = X[LEN-k-1]
				y = Y[LEN-k-1]
				left = LEFT[LEN-k-1]
				right = RIGHT[LEN-k-1]
				print('k---x,y,left,right=',x,y,left,right)
				k=k+1  #检测第几次未找到相同行
			else:
				return area_count

image = cv2.imread('1.png')

gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

img = cv2.adaptiveThreshold(gray ,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,25,5)
# print(img.shape[0])
# print(img.shape[1])
cv2.circle(image, (166,50), 1, (255, 0, 0), -1)
plt.imshow(image,'gray'),plt.title('color'),plt.show()
print(img[50][166])
result = flood_fill_seed(img, 166, 50, 1)

print('result=',result)
# plt.imshow(img,'gray'),plt.title('gray'),plt.show()
plt.imshow(image,'gray'),plt.title('color'),plt.show()
