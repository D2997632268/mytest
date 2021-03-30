# from turtle import *
# left(80)
# fd(100)
# right(30)
# fd(30)
# right(30)
# fd(40)
# input()


# for i in [1,2,3,4]:
# 	print(i)
# else:
# 	print(i, '我是else')
	
# a=[0,0,0,0]
# a[0]=-1
# print(a)

# def a(b):
# 	global num
# 	num+=1
# 	print('num1=',num)

# def func(e,f):
# 	global num
# 	num+=1
# 	print('num2=',num)

# # global num
# num=0

# func(1,2)
# a(3)

# a = {1,1,2,3,4,5}
# print(a)

# b = {(1,2),(3,4),(1,2),(5,4)}
# # b.add((3,8))
	# # print(b.type())
	# e = set((1,2),(3,4),(1,2),(5,4))
	# c = list(b)
	# print(c)
# print('e=',e)


# b = {(1,2),(2,3),(3,4), (5,6)}
# print(len(b))
# import numpy as np  
# a = [(1,2),(2,3),(3,4)]
# print(len(a))
# c = list(b)
# for i in c:
# 	if i not in a:
# 		a.append(i)
	
# print('c',c)
# # a.append((c-a))
# print(a)

# a[5] = (5,5)
# print(a)
# sum(a[1][1])
# print(sum(a[1]))
# print(type(a[1]))
# list(a[1])
# print(type(a[1]))
# print(np.diff(a[1]))
# a=[[1,2],[2,3],[3,4]]
# print(6%4)

# a = [[(1,2),(3,4),(5,6)],[(3,4),(5,5),(6,8)],[(4,5),(6,7),(8,8)]]
# print(a[1][1])

# j=True
# print(j)

'''
import cv2
import numpy as np 
image = cv2.imread('xy.png')
# print(image)
# col = image.shape[0]
# row = image.shape[1]
# print(col, row)
np.set_printoptions(threshold=np.inf)
image = cv2.imread('11.png',0)
# print(image)
col = image.shape[0]
row = image.shape[1]
M = cv2.getRotationMatrix2D((col/2, row/2), 65, 1)
rotate = cv2.warpAffine(image, M, (col, row))

cv2.imshow('image',image)
cv2.imshow('rotate',rotate)

cv2.imwrite('rotate65.png',rotate)

cv2.waitKey(0)
cv2.destroyAllWindows()

# a=[[-1 2 3] 
# [1 2 3]]
# print(a.shape)

'''


# for i in range(5,9):
# 	print(i)
# rect = 4
# if rect in (3,4,5):
# 	print(rect)
# else:
# 	print('不存在')
# for i in (1,1+1):
# 	print(i)

# temp_b = []

# for i in range(1,10):
# 	if i not in temp_b:
# 		temp_b.append(i)
# 	i += 1
	
# print(temp_b)\

# a = [1,2,3,4,5,6,7]
# for i in a:
# 	if i==2:
# 		a.remove(i)
# print(a)

# import numpy as np
# def coordinate_xy(a,book):
	
# 	for j in a:	
# 		print('-----------a-----------',a)

# 		next=[[0,1],[1,0],[0,-1],[-1,0]]
# 		# b=[]
# 		if j[0]>10 or j[0]<1 or j[1]>10 or j[1]<1 :
# 			continue
# 		for i in range(0,4):
# 			tx = j[0] + next[i][0]
# 			ty = j[1] + next[i][1]
# 			print(tx,ty)
# 			if book[tx,ty]==0:	
# 				book[tx,ty]=1
# 				# b.append([tx,ty])
# 				a.append([tx,ty])
# 		# print('b',b)
# 		# a.append(z for z in b)
# 		a.remove(j)
# 		print('a',a)

	# if a!=[]:
	# 	print('-------------------------')
	# 	coordinate_xy(a,book)
	# else:
	# 	return 0
		
	# return 0,0


# book = np.zeros((10, 10), np.uint8)
# a=[[5,5],]
# re = coordinate_xy(a,book)
# print('re',re)

# a = [0,0,0,0]

# a = 1
# print(a)

# a = [[1,2],[3,4],[4,5]]
# x,y = a[1]
# print(x)

# a=[1,2,3,4,5,6,7]
# print(a)

# for i in range(1,10,2):
# 	# i=i+1
# 	print(i)

# print((1>2)?1:2)
# i=1
# print(6337557873>>1)
# print(1^2)


# print(round(4.5))

# x=[4,5,6,7]
# y=[6,7,8,9]

# # for z in range(0,4):
# for i in x:
# 	print(i)

# if (299, 104) not in [[(299, 104), (344, 104), (342, 150), (296, 148)], [(418, 108), (464, 110), (461, 155), (415, 153)], [(292, 222), (338, 224), (336, 267), (290, 264)]]:
# 	print('111')
# else:
# 	print('2222')

# a = 1
# b = 2
# a,b = b,a
# print(a,b)

# def mergeSort(alist):
# 	print("1splitting",alist)
# 	if len(alist)>1:
# 		mid = len(alist)//2
# 		letfhalf = alist[:mid]
# 		righthalf = alist[mid:]

# 		mergeSort(letfhalf)
# 		mergeSort(righthalf)

# 		i = 0
# 		j = 0
# 		k = 0
# 		print('2',len(letfhalf), len(righthalf))
# 		while i<len(letfhalf) and j <len(righthalf):
# 			print('3',letfhalf[i],righthalf[j])
# 			if letfhalf[i] < righthalf[j]:
# 				alist[k] = letfhalf[i]
# 				i += 1
# 			else:
# 				alist[k] = righthalf[j]
# 				j += 1
# 			k += 1
# 			print('4',alist[k])
# 		print('5',i, len(letfhalf))
# 		while i< len(letfhalf):
# 			print('left')
# 			alist[k] = letfhalf[i]
# 			print('6alist[k]',k,alist[k])
# 			i += 1
# 			k += 1

# 		print('7',j,len(righthalf))
# 		while j<len(righthalf):
# 			print('right')
# 			alist[k] = righthalf[j]
# 			j += 1
# 			k += 1
# 	print('8merging',alist)


# a = [4,2,3,6,9]
# mergeSort(a)
# import numpy as np
# a = 37/3
# print(a)
# print(np.arctan(a))

# import cv2
# import numpy as np
# import matplotlib.pyplot as plt 
# from skimage.color import rgb2gray

# image = cv2.imread('c39mini_1.bmp')

# aaa = rgb2gray(image)
# plt.imshow(aaa,'gray'),plt.show()
# gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
# f = []

# row = 250
# up = row-30
# down = row+30
# for i in range(0,image.shape[1]):
# 	sumf = 0
# 	for j in range(up,down):
# 		sumf += gray[j,i]
# 		# print(sumf)
# 	f.append(sumf/61)
# 	# print('-----------',sumf//61)


# cv2.line(image,(0,up),(image.shape[1],up),(0,0,255),1)
# cv2.line(image,(0,down),(image.shape[1],down),(0,0,255),1)
# cv2.imshow('image',image)


# plt.plot(f),plt.show()

# cv2.waitKey()
# cv2.destroyAllWindows()

#-------------------------------------------------------------------------#
#首先利用plot_surface分析某张图片的灰度图
# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from skimage import io
from skimage.color import rgb2gray
import numpy as np
from matplotlib import cm
import cv2

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

img = cv2.imread("barcode/2.jpg")#c39mini_1.bmp
# gray_img = rgb2gray(img)
gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

X = np.arange(0, gray_img.shape[1], 1)
Y = np.arange(0, gray_img.shape[0], 1)

X, Y = np.meshgrid(X, Y)
Z = gray_img * 255
# Plot the surface.
surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm,
                       linewidth=0, antialiased=False)

# Customize the z axis.
# Add a color bar which maps values to colors.
fig.colorbar(surf, shrink=0.5, aspect=5)

plt.show()





#然后利用plot_wireframe分析某张图片的灰度图
# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from skimage import io
from skimage.color import rgb2gray
import numpy as np
from matplotlib import cm

import cv2

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

img = cv2.imread("barcode/2.jpg")#c39mini_1.bmp
# gray_img = rgb2gray(img)
gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# gray_img = cv2.pyrDown(gray_img)
# gray_img = cv2.pyrDown(gray_img)
# gray_img = cv2.pyrDown(gray_img)

X = np.arange(0, gray_img.shape[1], 1)
Y = np.arange(0, gray_img.shape[0], 1)

X, Y = np.meshgrid(X, Y)

Z = gray_img * 255
# Plot the surface.
surf = ax.plot_wireframe(X, Y, Z, rstride=1, cstride=1)
plt.show()




#然后利用plot_trisurf分析某张图片的灰度图
# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from skimage import io
from skimage.color import rgb2gray

import cv2

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

img = cv2.imread("barcode/2.jpg")#c39mini_1.bmp 0_10.png
# gray_img = rgb2gray(img)
gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
print(gray_img.shape)
gray_img = cv2.pyrDown(gray_img)
gray_img = cv2.pyrDown(gray_img)
gray_img = cv2.pyrDown(gray_img)
print(gray_img.shape)

x = []
y = []
z = []
for yi in range(0, gray_img.shape[0]):
    for xi in range(0, gray_img.shape[1]):
        y.append(yi)
        x.append(xi)
        z.append(gray_img[yi][xi] * 255)

ax.plot_trisurf(x,y,z)
plt.show()














































































































