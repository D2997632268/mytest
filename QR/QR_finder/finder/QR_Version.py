
def version_info(version):
	if version==1:
		data_bytes = 26
		apat = [0]
		ecc = [[26,16,1],
				[26,19,1],
				[26,9,1],
				[26,13,1]]
		return data_bytes, apat, ecc
	elif version==2:
		data_bytes = 44
		apat = [6,18,0]
		ecc = [[44,28,1],
				[44,34,1],
				[44,16,1],
				[44,22,1]]
		return data_bytes, apat, ecc
	elif version==3:
		data_bytes = 70
		apat = [6,22,0]
		ecc = [[70,44,1],
				[70,55,1],
				[35,13,2],
				[35,17,2]]
		return data_bytes, apat, ecc
	elif version==4:
		data_bytes = 100
		apat = [6,26,0]
		ecc = [[50,32,2],
				[100,80,1],
				[25,9,4],
				[50,24,2]]
		return data_bytes, apat, ecc
	elif version==5:
		data_bytes = 134
		apat = [6,30,0]
		ecc = [[67,43,2],
				[134,108,1],
				[33,11,2],
				[33,15,2]]
		return data_bytes, apat, ecc
	elif version==6:
		data_bytes = 172
		apat = [6,34,0]
		ecc = [[43,27,4],
				[86,68,2],
				[43,15,4],
				[43,19,4]]
		return data_bytes, apat, ecc
	elif version==7:
		data_bytes = 196
		apat = [6,22,38,0]
		ecc = [[49,31,4],
				[98,78,2],
				[39,13,4],
				[32,14,2]]
		return data_bytes, apat, ecc
	elif version==8:
		data_bytes = 242
		apat = [6,24,42,0]
		ecc = [[60,38,2],
				[121,97,2],
				[40,14,4],
				[40,18,4]]
		return data_bytes, apat, ecc
	elif version==9:
		data_bytes = 292
		apat = [6,26,46,0]
		ecc = [[58,36,3],
				[146,116,2],
				[36,12,4],
				[36,16,4]]
		return data_bytes, apat, ecc
	elif version==10:
		data_bytes = 346
		apat = [6,28,50,0]
		ecc = [[69,43,4],
				[86,68,2],
				[43,15,6],
				[43,19,6]]
		return data_bytes, apat, ecc
	elif version==11:
		data_bytes = 404
		apat = [6,30,54,0]
		ecc = [[80,50,1],
				[101,81,4],
				[36,12,3],
				[50,22,4]]
		return data_bytes, apat, ecc
	elif version==12:
		data_bytes = 466
		apat = [6,32,58,0]
		ecc = [[58,36,6],
				[116,92,2],
				[42,14,7],
				[46,20,4]]
		return data_bytes, apat, ecc
	elif version==13:
		data_bytes = 532
		apat = [6,34,62,0]
		ecc = [[59,37,8],
				[133,107,4],
				[33,11,12],
				[44,20,8]]
		return data_bytes, apat, ecc
	elif version==14:
		data_bytes = 581
		apat = [6,26,46,66,0]
		ecc = [[64,40,4],
				[145,115,3],
				[36,12,11],
				[36,16,11]]
		return data_bytes, apat, ecc
	elif version==15:
		data_bytes = 655
		apat = [6,26,48,70,0]
		ecc = [[65,41,5],
				[109,87,5],
				[36,12,11],
				[54,24,5]]
		return data_bytes, apat, ecc
	elif version==16:
		data_bytes = 733
		apat = [6,26,50,74,0]
		ecc = [[73,45,7],
				[122,98,5],
				[45,15,3],
				[45,15,3]]
		return data_bytes, apat, ecc
	elif version==17:
		data_bytes = 815
		apat = [6,30,54,78,0]
		ecc = [[74,46,10],
				[135,107,1],
				[42,14,2],
				[50,22,1]]
		return data_bytes, apat, ecc
	elif version==18:
		data_bytes = 532
		apat = [6,26,46,66,0]
		ecc = [[64,40,4],
				[145,115,3],
				[36,12,11],
				[36,16,11]]
		return data_bytes, apat, ecc
	elif version==19:
		data_bytes = 991
		apat = [6,30,58,86,0]
		ecc = [[70,44,3],
				[141,113,3],
				[39,13,9],
				[47,21,17]]
		return data_bytes, apat, ecc
	elif version==20:
		data_bytes = 1085
		apat = [6,34,62,90,0]
		ecc = [[67,41,3],
				[135,107,3],
				[43,15,15],
				[54,24,15]]
		return data_bytes, apat, ecc
	elif version==21:
		data_bytes = 1156
		apat = [6,28,50,72,92,0]
		ecc = [[68,42,17],
				[144,116,4],
				[46,16,19],
				[50,22,17]]
		return data_bytes, apat, ecc
	elif version==22:
		data_bytes = 1258
		apat = [6,26,50,74,98,0]
		ecc = [[74,46,17],
				[139,111,2],
				[37,13,34],
				[54,24,7]]
		return data_bytes, apat, ecc
	elif version==23:
		data_bytes = 1364
		apat = [6,30,54,78,102,0]
		ecc = [[75,47,4],
				[151,121,4],
				[45,15,16],
				[54,24,11]]
		return data_bytes, apat, ecc
	elif version==24:
		data_bytes = 1474
		apat = [6,28,54,80,106,0]
		ecc = [[73,45,6],
				[147,117,6],
				[46,16,30],
				[54,24,11]]
		return data_bytes, apat, ecc
	elif version==25:
		data_bytes = 1588
		apat = [6,32,58,84,110,0]
		ecc = [[75,47,8],
				[132,106,8],
				[45,15,22],
				[54,24,7]]
		return data_bytes, apat, ecc
	elif version==26:
		data_bytes = 1706
		apat = [6,30,58,86,114,0]
		ecc = [[74,46,19],
				[142,114,10],
				[46,16,33],
				[50,22,28]]
		return data_bytes, apat, ecc
	elif version==27:
		data_bytes = 1828
		apat = [6,34,62,90,118,0]
		ecc = [[73,45,22],
				[152,122,8],
				[45,15,12],
				[53,23,8]]
		return data_bytes, apat, ecc
	elif version==28:
		data_bytes = 1921
		apat = [6,26,50,74,98,122,0]
		ecc = [[73,45,3],
				[147,117,3],
				[45,15,11],
				[54,24,4]]
		return data_bytes, apat, ecc
	elif version==29:
		data_bytes = 2051
		apat = [6,30,54,78,102,126,0]
		ecc = [[73,45,21],
				[146,116,7],
				[45,15,19],
				[53,23,1]]
		return data_bytes, apat, ecc
	elif version==30:
		data_bytes = 2185
		apat = [6,26,52,78,104,130,0]
		ecc = [[75,47,19],
				[145,115,5],
				[45,15,23],
				[54,24,15]]
		return data_bytes, apat, ecc
	elif version==31:
		data_bytes = 2323
		apat = [6,30,56,82,108,134,0]
		ecc = [[74,46,2],
				[145,115,13],
				[45,15,23],
				[54,24,42]]
		return data_bytes, apat, ecc
	elif version==32:
		data_bytes = 2465
		apat = [6,34,60,86,112,138,0]
		ecc = [[74,46,10],
				[145,115,17],
				[45,15,19],
				[54,24,10]]
		return data_bytes, apat, ecc
	elif version==33:
		data_bytes = 2611
		apat = [6,30,58,86,114,142,0]
		ecc = [[74,46,14],
				[145,115,17],
				[45,15,11],
				[54,24,29]]
		return data_bytes, apat, ecc
	elif version==34:
		data_bytes = 2761
		apat = [6,34,62,90,118,146,0]
		ecc = [[74,46,14],
				[145,115,13],
				[46,16,59],
				[54,24,44]]
		return data_bytes, apat, ecc
	elif version==35:
		data_bytes = 2876
		apat = [6,30,54,78,102,126,150]
		ecc = [[75,47,12],
				[151,121,12],
				[45,15,22],
				[54,24,39]]
		return data_bytes, apat, ecc
	elif version==36:
		data_bytes = 3034
		apat = [6,24,50,76,102,128,154]
		ecc = [[75,47,6],
				[151,121,6],
				[45,15,2],
				[54,24,46]]
		return data_bytes, apat, ecc
	elif version==37:
		data_bytes = 3196
		apat = [6,28,54,80,106,132,158]
		ecc = [[75,46,29],
				[152,122,17],
				[45,15,24],
				[54,24,49]]
		return data_bytes, apat, ecc
	elif version==38:
		data_bytes = 3362
		apat = [6,32,58,84,110,136,162]
		ecc = [[74,46,13],
				[152,122,4],
				[45,15,42],
				[54,24,48]]
		return data_bytes, apat, ecc
	elif version==39:
		data_bytes = 3532
		apat = [6,26,54,82,110,138,166]
		ecc = [[75,47,40],
				[147,117,20],
				[45,15,10],
				[54,24,43]]
		return data_bytes, apat, ecc
	elif version==40:
		data_bytes = 3706
		apat = [6,30,58,86,114,142,170]
		ecc = [[75,47,18],
				[148,118,19],
				[45,15,20],
				[54,24,34]]
		return data_bytes, apat, ecc
