'''
笔记：
1、image[第几行][第几列]
2、总行数, 总列数=image.shape
3、a=[[1,2,3],[4,6,7]]------>a[1][0]=4

'''
import numpy as np 
import matplotlib.pyplot as plt 
import cv2
np.set_printoptions(threshold=np.inf)
#彩色图像转换成灰度图像
def rgb2gray(A):
	print(A.shape[0],A.shape[1])#702 405
	#定义一个空列表
	x = []
	for i in range(A.shape[0]):
		for j in range(A.shape[1]):
			#使用转换公式进行彩色灰度转化
			# print(i,j)
			x.append(np.int(A[i,j,0]*0.3 + A[i,j,1]*0.59 + A[i,j,2]*0.11))
	#将列表转换成矩阵
	x = np.array(x)
	#将矩阵转变成原尺寸大小
	x = x.reshape(A.shape[0], A.shape[1])
	return x				

#magnitude和phase
def Get_magnitude_and_phase(gray,sobelX,sobelY):
	global phase,status
	for i in range(gray.shape[0]):
		for j in range(gray.shape[1]):
			status[i,j] = np.sqrt(sobelX[i,j]**2 + sobelY[i,j]**2)
			# print('--',sobelX[i,j], sobelY[i,j])
			if sobelY[i,j]==0:
				phase.append(0,)
			else:
				phase.append(np.arctan(sobelX[i,j]/sobelY[i,j]))
				# print(sobelX[i,j], sobelY[i,j])

			if status[i,j]<=17:
				status[i,j]=1
				mag.append(1)
			else:
				status[i,j]=0
				mag.append(0)
	phase = np.array(phase)
	phase = phase.reshape(gray.shape[0],gray.shape[1])
	# print(phase)
	return 1
def countWordFlip(m, edge, prev_byte):
	# print('m',m)
	compare = m^(m>>8)
	if compare & 0xFF:
		edge += 1
	if compare & 0xFF00:
		edge += 1
	if compare & 0xFF0000:
		edge += 1
	if prev_byte^(m & 0xFF):
		edge += 1
	prev_byte = compare>>24


def countLineFlip(index, words_count, edge, prev_byte):
	global mag
	for i in range(0,words_count):
		index += 1
		countWordFlip(mag[index], edge, prev_byte)


def countThreeLinesFlip(index, offset_v1, offset_v2):
	index = (index+3) & 0xFFFFFFFC
	global mag
	index_32 = [index>>2, (index-offset_v1)>>2, (index+offset_v2)>>2]
	Row_edge=[0]*3
	prev_byte=[mag[index], mag[index-offset_v1], mag[index+offset_v2]]
	passs = [False, offset_v1==0, offset_v2==0]
	words_count = 8
	for i in range(0,3):
		if passs[i]:
			Row_edge[i] = Row_edge[0]
			continue
		edge = 0
		index = index_32[i]
		countLineFlip(index, words_count, edge, prev_byte)

		if prev_byte[i] ^ (mag[index + words_count] & 0xFF):
			edge += 1
		Row_edge[i] = edge

	return Row_edge[0] + Row_edge[1] + Row_edge[2]



image = plt.imread('hui.jpg')#hui2.jpg
# img = rgb2gray(image)
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
status = gray.copy()
phase = []
mag = []
H = gray.shape[0]
W = gray.shape[1]
print(H,W)
# phase = gray.copy()
# plt.subplot(121),plt.imshow(image),plt.title('original image')
# plt.subplot(122),plt.imshow(gray, cmap='gray'),plt.title('gray image')
# plt.show()

sobelx = cv2.Sobel(gray, cv2.CV_64F, 1, 0, ksize=5)
sobelx = cv2.convertScaleAbs(sobelx)
sobely = cv2.Sobel(gray, cv2.CV_64F, 0, 1, ksize=5)
sobely = cv2.convertScaleAbs(sobely)

Get_magnitude_and_phase(gray, sobelx, sobely)

# print(status)
# plt.subplot(132),plt.imshow(status, cmap='gray'),plt.title('status image')
# plt.subplot(131),plt.imshow(image, cmap='gray'),plt.title('original image')
# plt.subplot(133),plt.imshow(phase, cmap='gray'),plt.title('phase image')
# plt.show() 
sobelxy = cv2.addWeighted(sobelx, 0.5, sobely, 0.5, 0)
# print(sobelxy)

# mag = status.tolist()
# # mag = mag.tolist()
# print(status)

Step_j=8
Step_i=8
Max_1=0
Max_2=0
Max_3=0

barcodeTypeCount = 0
FB_Barcode_S = 2
FB_Barcode_EdgeTh =	12
index = 0
for j in range(0,H,2):
	Row_edge = 0
	prev_byte = mag[index]
	# print('prev_byte',prev_byte)
	print(index>>2, W>>2, Row_edge, prev_byte)
	countLineFlip(index>>2, W>>2, Row_edge, prev_byte)
	print(index>>2, W>>2, Row_edge, prev_byte)
	if Row_edge>FB_Barcode_EdgeTh:
		break
	index += W*FB_Barcode_S
	print('Row_edge',Row_edge)





# plt.subplot(131),plt.imshow(sobelx, cmap='gray'),plt.title('sobelx image')
# plt.subplot(132),plt.imshow(sobely, cmap='gray'),plt.title('sobely image')
# plt.subplot(133),plt.imshow(sobelxy, cmap='gray'),plt.title('sobelxy image')
# plt.show() 
