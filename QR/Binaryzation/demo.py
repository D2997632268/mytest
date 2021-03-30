'''
笔记：
1、将一个数组a直接赋给另一个变量b eg:b = a
  b中的值变动 a中对应值会变动 
  正确应该是b = a.copy() a中的值不会随着b变动。


'''

import cv2
import numpy as np 
import matplotlib.pyplot as plt 
np.set_printoptions(threshold=np.inf)

sigma = 1
pi = 3.14159
size = 2
ERODE = None
data_test = []
fx = [] #一阶差分
fxx = [] #二阶差分
edgeWidth = [] #条纹宽度
edgeWidth_1 = [] #条纹宽度
outer = []
maopaotab = []
BS = [0] * 9
trans_39 = ['0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','-','.',' ','$','/','+','%','*','0']
C32TAB = ['0','1','2','3','4','5','6','7','8','9','B','C','D','F','G','H','J','K','L','M','N','P','Q','R','S','T','U','V','W','X','Y','Z']
CIFANG32 = [1,32,1024,32768,1048576,33554432]
dig39 = [0x034,0x121,0x061,0x160,0x031,0x130,0x070,0x025,0x124,0x064,0x109,0x049,0x148,0x019,0x118,0x058,0x0d,0x10c,0x04c,0x01c,0x103,0x043,0x142,0x013,0x112,0x052,0x007,0x106,0x046,0x016,0x181,0x0c1,0x1c0,0x091,0x190,0x0d0,0x085,0x184,0x0c4,0x0a8,0x0a2,0x08a,0x02a,0x094]

#************************卷积核************************#
def GaussianKernel(data_test, size, sigma):
	N = size
	for i in range(1,2*N):
		data_test.append(np.exp(-(i-N)*(i-N)/(2*sigma*sigma))/(sigma*np.sqrt(2*pi)))
	print(data_test)

#************************高斯滤波************************#
def GaussianFilter(y, data_test, size, LEN):
	N = size
	global yy
	for i in range(N-1,LEN-N+1):
		# print('i',i)
		data_1 = 0
		# print('data_1',data_1)
		for j in range(0,3):
			
			data_1 = data_1+y[i-1+j]*data_test[j]
			# print(i-1+j)
			# print(y[i-1+j])
		yy[i]=int(data_1)

#**************************差分************************#		
def Differential(y, fx, fxx, LEN):
	# print('y=',y)
	for i in range(0,LEN-1):
		# print('11111111111',y[i+1],y[i])
		# print('fx----',y[i+1]-y[i])
		# a = (y[i+1]-y[i])
		fx.append(int(y[i+1]-y[i]))
	for i in range(0,LEN-2):
		# print('22222222',fx[i+1],fx[i])
		# print('fxx----',fx[i+1]-fx[i])
		# b = fx[i+1]-fx[i]
		fxx.append(int(fx[i+1]-fx[i]))

#*************************包络线************************#
def BaoLuoXian():
	global baoluoupData, baoluodownData, wav, fx_size
	lastup = 0.0
	lastdown = 3000.0
	rate = 0.5
	for i in range(0,fx_size):
		if wav[i] >= lastup:
			baoluoupData[i] = wav[i]
		else:
			if wav[i] >= lastup-rate:
				baoluoupData[i] = wav[i]
			else:
				baoluoupData[i] = lastup - rate

		lastup = baoluoupData[i]
		if wav[i] <= lastdown:
			baoluodownData[i] = wav[i]
		else:
			if wav[i]<= lastdown + rate:
				baoluodownData[i] = wav[i]
			else:
				baoluodownData[i] = lastdown + rate
		lastdown = baoluodownData[i]
		rate = (baoluoupData[i] - baoluodownData[i])/64.0
	return 1

#************************上升沿触发************************#
def upTrigger(i,tri):
	# global upTri
	chazhi = baoluoupData[i+3] - baoluodownData[i+3]
	if chazhi >= (baoluoupData[i-1] - baoluodownData[i-1])*1.3 and chazhi >=10:
	 	tri += 1
	return tri

#************************边缘搜索************************#
def SearchEdge():
	upTri = 0 #上升沿触发
	point = 0
	if fx[0] > 0:
		risestate = 1
		risevalue = 0
		j = 1
	else:
		risestate = 0
		falsevalue = 0
		j = 2
	for i in range(1,LEN-3):
		up = wav[i-1] + wav[i+1] - 2*wav[i]
		down = wav[i] + wav[i+2] - 2*wav[i+1]
		if (up >= 0 and down < 0) or (up <= 0 and down > 0):
			dx = abs((baoluoupData[i]-wav[i])-(wav[i]-baoluodownData[i]))
			dx = dx/(baoluoupData[i] - baoluodownData[i])
			blance = 6 * dx
			if risestate:
				if fx[i] > risevalue + blance:
					risevalue = fx[i]
					point = 1
				if fx[i] < risevalue - blance:
					risestate = 0
					falsevalue = fx[i]
					point = 1
					j += 1
			else:
				if fx[i] > falsevalue + blance:
					risevalue = fx[i]
					risestate = 1
					point = 1
					j += 1
				if fx[i] < falsevalue - blance:
					falsevalue = fx[i]
					point = 1
			if point:
				point = 0
				upTri = upTrigger(i,upTri)
				if upTri>=1:
					accuracy = abs(fxx[i-1])/abs(fxx[i-1] - fxx[i])
					edge[j] = i + accuracy
					# print(j, edge[j])
	return j

#************************宽度计算筛选************************#
def SearchCodebarWidth(num_edge):
	# print('edgeWidth_1',edgeWidth_1)
	m=0
	p = False
	global edgeWidth_1
	for i in range(1,num_edge):
		chaRE = z_result[i]-z_result[i-1]
		if chaRE > 0:
			edgeWidth.append(z_result[i]-z_result[i-1])
			# print('edgeWidth',edgeWidth)
	# edgeWidth_1 = [0] * len(edgeWidth) #条纹宽度
	edgeWidth_1 = edgeWidth.copy()
	for j in range(1,len(edgeWidth)):
		# print(j,edgeWidth[j])
		if p:
			p = ~p
			continue
		if edgeWidth[j]<1.0:
			# print('a',j, edgeWidth_1[m])
			edgeWidth_1[m] = edgeWidth_1[m]+edgeWidth[j]+edgeWidth[j+1]
			# print('a',j, edgeWidth_1[m])
			p = ~p
		else:
			m += 1
			edgeWidth_1[m] = round(edgeWidth[j],4)
			# print('b',j, edgeWidth_1[m])
	return edgeWidth_1,m


def bili(x,y):
	if x>=y:
		return (x * 10/y)
	else:
		return (y * 10/x)

#*************************起始符************************#
def start(leftcp, pos, maxtimes):
	global d 
	i = leftcp*maxtimes
	if pos<1:
		return 1
	if d[pos-1]>=i or d[pos-2]>=i or d[pos-3]>=i:
		return 1
	return 0

#************************解码模块************************#
def decode39one(BS, sigma):
	# print('模块解码！！！！')
	global ERODE
	maopaotab = []
	# print('ERODE=',ERODE)
	for i in range(0,9):
		if i and 1:
			tmp = BS[i] + ERODE
			BS[i] = round(tmp, 4)
		else:
			tmp = BS[i] - ERODE
			BS[i] = round(tmp, 4)
		maopaotab.append(BS[i])
	# print('--------------maopaotab---------------',maopaotab)
	for k in range(8):
		for i in range(8-k):
			# print(maopaotab[i])
			if maopaotab[i] > maopaotab[i+1]:
				maopaotab[i], maopaotab[i+1] = maopaotab[i+1], maopaotab[i]
	
	# print('---maopaotab--',maopaotab)
	ramer = (maopaotab[6] - maopaotab[5])/2 + maopaotab[5]
	if bili(ramer, sigma*22/180) >= 13:
		return 0
	k = 0
	for i in range(0,9):
		if BS[i] >= ramer:
			k += 1
		k = k*2
	k = k/2
	for i in range(0, 44):
		if k == dig39[i]:
			# outer.append[i]
			return trans_39[i]
	return 0

def decode39over(x, rightside, sigmanext, gapnext):
	global length, BS, d
	# print('decode39--i',x)
	for i in range(x,length,10):
		# print('i=',i,length)
		sigma = 0
		if rightside:
			for j in range(0, 9):
				BS[j] = d[i+j+1]
				sigma += BS[j]
		else:
			for j in range(0, 9):
				BS[j] = d[9-j+i]
				sigma += BS[j]
		if bili(sigma, sigmanext)>= 20:
			# print('---------',i, sigma, sigmanext)
			return 0
		gap = d[i]
		if bili(gap, gapnext)>= 20:
			# print('----------',i, gap, gapnext)
			return 0
		gapnext = gap
		sigmanext = sigma
		value = decode39one(BS, sigma)
		# return value
		print('value=',value)


def decode39():
	global d, length, BS, ERODE
	print('decode39-d=',d)
	if length>=32:
		find_length = length - 30
		gaptype = 0
		for a in range(0,find_length):	
			# print('---1----d[a+3], d[a+5]',a, d[a+3], d[a+5])
			if bili(d[a+3], d[a+5])<=18:
				# print('---2----d[a+3], d[a+5]',a, d[a+3], d[a+5])
				if start(d[a], a, 4):
					# print('----3----d[a+3], d[a+5]',a, d[a+3], d[a+5])
					if d[a+1]<d[a+7]:
						minn = d[a+1]
					else:
						minn = d[a+7]
					ERODE = round((d[a+8]-minn)/2, 4)
					sigma = 0
					if d[a+1] > d[a+7]:
						rightside = 1
						for i in range(0,9):
							BS[i] = d[i+a]
							sigma += BS[i]
					else:
						rightside = 0
						for i in range(0,9):
							BS[i] = d[8-i+a]
							sigma += BS[i]
					sigmanext = sigma
					# print('sigma, rightside',sigma, rightside)
					# print('BS=',BS)
					head_39 = decode39one(BS, sigma)
					print('head_39',head_39)
					if head_39=='*' or head_39== '$':
						print('--------------in----------------------')
						gapnext = d[a+9]
						# if decode39over(a+9, rightside, sigmanext, gapnext):
						# 	pass
						decode39over(a+9, rightside, sigmanext, gapnext)
						print('--------------out----------------------')


image = cv2.imread('39-2.jpg')
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
m, n = gray.shape
# print(m,n)
y = gray[m//2+4,:]
yy = y.copy()
LEN = len(y)
cv2.line(image, (0,m//2), (n,m//2), (255,0,0),1)
print('orignal line',y)

plt.plot(y),plt.title('orignal line'),plt.show()
plt.imshow(image,'gray'),plt.show()

# GaussianKernel(data_test, size,sigma)
data_test=[0.2420,0.3989,0.2420]
GaussianFilter(y, data_test, size, LEN)
# plt.plot(yy),plt.title('filter line'),plt.show()
# print('yy-filter',yy)
y = yy.copy().astype(np.float32)
LEN = len(y)

Differential(y,fx,fxx,LEN)
# print('fx=',fx)
# print('fxx=',fxx)

baoluoupData = y.copy().astype(np.float32)
baoluodownData = y.copy().astype(np.float32)
wav = y.copy().astype(np.float32)
edge = [0] * n
fx_size = len(fx)
BaoLuoXian()
# print('baoluoupData=', baoluoupData)
# print('baoluodownData=',baoluodownData)

edgeNum = SearchEdge()
z_result = []
# print('edgeNum=',edgeNum)
for i in range(0,edgeNum+1):
	if edge[i]!=0:
		z_result.append(round(edge[i],4))
# print('len, z_result=',len(z_result),z_result)


# x = np.linspace(0,1,n)
# plt.plot(x,y),plt.title('orignal line')
# plt.plot(x,baoluoupData, color='r', label='baoluoUP',linewidth=1)
# plt.plot(x,baoluodownData, color='g', label='baoluoDOWN',linewidth=1),plt.show()


d,length = SearchCodebarWidth(len(z_result))
# print('length',length)
# print('d',d)
d = d[:length+1]
# print('前len-d',len(d))
d.insert(0, 100)
# print('后len-d',len(d))
# print(d)

# print('baoluoupData',baoluoupData)
# plt.plot(y,color='b', label='filter line'),plt.show()
# plt.legend(loc='best')
# plt.show()
# plt.plot(fx),plt.title('fx line'),plt.show()
# plt.plot(fxx),plt.title('fxx line'),plt.show()

#---------------------------------decode-----------------------------------#

length = len(d)
decode39()

# plt.imshow(image,'gray'),plt.show()
# cv2.imshow('gray',gray)
# cv2.waitKey()
# cv2.destroyAllWindows()
