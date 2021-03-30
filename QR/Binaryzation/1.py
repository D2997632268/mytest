# import numpy 

# import cv2

# import matplotlib.pyplot as plt 

# e = 1 + 2j

# print(type(e))

# print(int.__doc__)


# s1 = '(>^ω^<)喵\n'
# s2 = "Dogge's home"
# print(type(s1))    
# print("%s, %s" % (s1, s2))

# print('a'+ '.' + 'b')

# def f():
# 	"return 'hello world!!'"
# 	return 'Hello World!'

# print(f())

# print(f.__doc__)

# def ff(v, l='python'):
# 	return '{}, {}!'.format(v,l)

# print(ff('hello'))
# print(ff('bye', 'C/C++'))

# def fff(*arg, con='&'):
# 	print('hello', con.join(arg))

# fff('python', 'C', 'C++','java', con='/')

# pairs = [(1,'one'), (2, 'two'), (3, 'three'), (4, 'four')]
# pairs.sort(key=lambda pairs : pairs[1])
# print(pairs)
# pairs.sort(key=lambda pairs : pairs[0])
# print(pairs)

# for i in range(1,10,2):
# 	if i==2:
# 		continue

# 	print(i)

# a = [1,2,3,'a']
# a = True
# a = not a
# print(a)
# i=0
# a = True
# while  i<10:
# 	i += 1
# 	a = (not a)
# 	print(i,a)

# a=[1,2,3,4,5,6,7,8,9]
# for i in range(0,len(a),2):
# 	print(a[i])

# import cv2
# from PIL import Image
# img = cv2.imread('39-2.jpg')
# image = imrotate(img,30)
# cv2.imsave(image,'39-2-30.jpg')

# im = Image.open("39-4.jpg")
# im = im.rotate(10)
# im.save("39-4-10.jpg")

# a = False
# b = True
# i=0
# z=0
# while i<10:

# 	print('-----in-----',i)
# 	print('z',z)
# 	print(a)	
# 	if a:
# 		a = not(a)
# 		continue

# 	i += 1
# 	if i%2==0:
# 		a = (not a)
# 		z = 11111
		
# 	else:
# 		a = (not a)
# 		z=22222
	
# 		# z=33333
	
# a=20	
# if a == 6:
# 	print('正')
# else:
# 	print('反')

# a = -2
# if a:
# 	print('真',a)
# else:
# 	print('反',a)	
score = [5/10, 4/10, 6/10, 3/10,7/10,2/10,8/10,1/10,9/10,10/10]
for i in range(0,10):
		print(10*score[i])	