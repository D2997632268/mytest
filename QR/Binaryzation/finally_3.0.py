'''
笔记：
1、将一个数组a直接赋给另一个变量b eg:b = a
  b中的值变动 a中对应值会变动 
  正确应该是b = a.copy() a中的值不会随着b变动。


'''

import cv2
import numpy as np 
import matplotlib.pyplot as plt 
from time import *
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
vari = 0
BS = [0] * 9
head_39 = 0
bool_blance = False
trans_39 = ['0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','-','.',' ','$','/','+','%','*','0']
C32TAB = ['0','1','2','3','4','5','6','7','8','9','B','C','D','F','G','H','J','K','L','M','N','P','Q','R','S','T','U','V','W','X','Y','Z']
CIFANG32 = [1,32,1024,32768,1048576,33554432]
dig39 = [0x034,0x121,0x061,0x160,0x031,0x130,0x070,0x025,0x124,0x064,0x109,0x049,0x148,0x019,0x118,0x058,0x0d,0x10c,0x04c,0x01c,0x103,0x043,0x142,0x013,0x112,0x052,0x007,0x106,0x046,0x016,0x181,0x0c1,0x1c0,0x091,0x190,0x0d0,0x085,0x184,0x0c4,0x0a8,0x0a2,0x08a,0x02a,0x094]
score10 = [5/10, 4/10, 6/10, 3/10,7/10,2/10,8/10,1/10,9/10,10/10]
score20 = [11/20, 10/20, 12/20, 9/20, 13/20, 8/20, 14/20, 7/20, 15/20, 6/20, 16/20, 5/20, 17/20, 4/20, 18/20, 3/20, 19/20, 2/20, 20/20, 1/20]
decodeNUM = []
number = 0
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
		fx.append(int(y[i+1]-y[i]))
	for i in range(0,LEN-2):
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
	global bool_blance,vari
	c=1
	if bool_blance:
		c=0
		bool_blance = not bool_blance
	if c:
		if vari == 6 :
			bool_blance = not bool_blance
			vari = 20
		else:
			bool_blance = not bool_blance
			vari = 6
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
			blance = vari * dx
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
	for j in range(1,len(edgeWidth)-1):
		# print(len(edgeWidth))
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

def end(rightcp, pos, maxtimes):
	global d ,length
	i = rightcp*maxtimes
	tem = length-pos-1
	# print('baohubian=',rightcp*maxtimes)
	# print('tem=',tem)
	if tem<=2:
		return 1
	# print(i,d[pos+1])
	# if tem==2:
	# 	print(d[pos+1], d[pos+2])
	# 	if d[pos+1]>=i or d[pos+2]>=i:
	# 		return 1
	if tem>=3:
		if d[pos+1]>=i or d[pos+2]>=i or d[pos+3]>=i:
			return 1
	
	return 0
def decode39over(x, rightside, sigmanext, gapnext, head_39):
	global length, BS, d, decodeNUM, number
	# print('decode39--i',x)
	# print('length',length)
	decodeNUM=[]
	number = 0
	for i in range(x,length-10,10):
		sigma = 0
		if rightside:
			for j in range(0, 9):
				# print('------',i,j,i+j+1,d[i+j+1])
				# print(j,BS[j])
				BS[j] = d[i+j+1]
				sigma += BS[j]
		else:
			for j in range(0, 9):
				BS[j] = d[9-j+i]
				sigma += BS[j]
		if bili(sigma, sigmanext)>= 20:
			# print('----bili1-----',i, sigma, sigmanext)
			return 0
		gap = d[i]
		if bili(gap, gapnext)>= 20:
			# print('-----bili2-----',i, gap, gapnext)
			return 0
		gapnext = gap
		sigmanext = sigma
		value = decode39one(BS, sigma)
		if value:
			number +=1
			decodeNUM.append(value)
			if value==head_39:
				value=0
		if value==0:
			y = i+10
			i = length
			if number>=2:
				if decodeNUM[number-1] == head_39:
					if end(d[y-1],y-1,4):
						return 1
	return 0


def decode39():
	global d, length, BS, ERODE,decodeNUM, head_39
	# print('decode39-d=',d)
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
					# print('head_39',head_39)
					if head_39=='*' or head_39== '$':
						# decodeNUM=[]
						# decodeNUM.append(head_39)
						# print('--------------in----------------------')
						gapnext = d[a+9]
						num = decode39over(a+9, rightside, sigmanext, gapnext, head_39)
						# print('num=',num)
						# print(decodeNUM)
						# print('--------------out----------------------')
						return num
						# print(decodeNUM)
						
						# return 1
	return 0

def crosswise(col,line,num,success_line):
	global score10, score20, boolCOOL,COL2,COL,loop_number
	# print('num=',num)
	if line>col or line<0:
		return -1
	if loop_number>0 :
		# print('success_line=',success_line)
		if boolCOOL:
			COL += 2
		if boolCOOL:
		 	Line = success_line + COL
		 	boolCOOL = (not boolCOOL)
		 	if Line>=col:
		 		# result = 0
		 		print('decode failing!!!!')
		 		return -1
	 		return Line
		else:
		 	Line = success_line - COL
		 	boolCOOL = (not boolCOOL)
		 	if Line<0:
		 		# result = 0
		 		print('decode failing!!!!')
		 		return -1
	 		return Line
		
	if num<20:
		return int(col*score20[num]-col//40)
	if loop_number<=0 and num>=20 :
		# print('num<=10')
		# print('boolCOOL=',boolCOOL)
		if boolCOOL:
			COL2 += 2
		if boolCOOL:
		 	line = col//2 + COL2
		 	boolCOOL = (not boolCOOL)
		 	if line>=col:
		 		# result = 0
		 		print('decode failing!!!!')
		 		return -1
	 		return line
		else:
		 	line = col//2 - COL2
		 	boolCOOL = (not boolCOOL)
		 	if line<0:
		 		# result = 0
		 		print('decode failing!!!!')
		 		return -1
	 		return line
		# boolCOOL = (not boolCOOL)



def LoopDecode():
	row = 0
	lin_num = 0
	success_line = 0
	start = time()
	while 1:
		global head_39, data_test, fx, fxx, edgeWidth, edgeWidth_1, outer, maopaotab, BS, COL,COL2, boolCOOL,loop_number,yy,fx_size,wav,baoluoupData,baoluodownData,LEN,edge,z_result,length,d
		head_39 = 0
		data_test = []
		fx = [] #一阶差分
		fxx = [] #二阶差分
		edgeWidth = [] #条纹宽度
		edgeWidth_1 = [] #条纹宽度
		outer = []
		maopaotab = []
		# decodeNUM = []
		BS = [0] * 9

		row = crosswise(m-1, row, lin_num, success_line)
		# print('row',row)
		if row<0:
			break
		lin_num += 1
		# print('row',row)
		y = gray[row,:]
		yy = y.copy()
		LEN = len(y)
		cv2.line(image, (0,row), (n,row), (255,0,0),1)
		data_test=[0.2420,0.3989,0.2420]
		GaussianFilter(y, data_test, size, LEN)
		y = yy.copy().astype(np.float32)
		LEN = len(y)
		Differential(y,fx,fxx,LEN)
		baoluoupData = y.copy().astype(np.float32)
		baoluodownData = y.copy().astype(np.float32)
		wav = y.copy().astype(np.float32)
		edge = [0] * n
		fx_size = len(fx)
		BaoLuoXian()
		edgeNum = SearchEdge()
		z_result = []
		for i in range(0,edgeNum+1):
			if edge[i]!=0:
				z_result.append(round(edge[i],4))
		d,length = SearchCodebarWidth(len(z_result))
		d = d[:length+1]
		d.insert(0, 100)
		d.append(100)
		# print('d=',d)
		length = len(d)
		decodeNUM = []
		result = decode39()
		# print(loop_number,decodeNUM)
		# print('decode39---result=',result)
		if result != 0 :
			loop_number += 1
			success_line = row
			# print('loop_number=',loop_number)
		if loop_number>=2:
			# cv2.imwrite('success/'+str(success)+'.jpg',image)
			# success += 1
			print('decode success!!')
			break
		end_time = time()
		# print('start-end_time',end_time-start)
		if end_time-start>1:
			result = 0
			break
	return result


C = 1
success = 0
fail = 0
cap = cv2.VideoCapture(0)
ret, frame = cap.read()
# cv2.imshow('cap',frame)
timeF=1
while ret:
	ret, image = cap.read()
	cv2.line(image, (0,240), (640,240), (0,0,255),1)
	
	if cv2.waitKey(10) & 0xff == ord('q'):		
		break
	# print('C=',C)
	if C%timeF==0 :
		start1 = time()
		cv2.imwrite('images/'+str(C)+'.jpg',image)
		# image = cv2.imread('image/'+str(C)+'.jpg')
		# print('-----------------new image----------')
		gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
		m, n = gray.shape
		print(m,n)
		COL = 0
		COL2 = 0
		boolCOOL = True
		loop_number = 0
		fx_size = 0
		LEN = 0
		edge = [0] * n
		z_result = []
		length = 0
		d = []
		result = LoopDecode()
		if result:
			end_time1 = time()
			head = str(head_39)
			decodeNUM.insert(0,head)
			success += 1
			cv2.imwrite('success/'+str(success)+'.jpg',image)
			print('time=',end_time1-start1)
			print('DECODE',decodeNUM)
		else:
			fail += 1
			cv2.imwrite('failture/'+str(fail)+'.jpg',image)
	cv2.imshow('cap',image)
	# break
	C = C+1


	
# if cv2.waitKey(10) & 0xff == ord('q'):		
# 	cv2.waitKey(1)
# 	cap.release()
# 	cv2.destroyAllWindows()
	

