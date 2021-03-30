#1/bin/sh
a=0
b=0
if [ $1 -eq 1 ]
then
	while [ $b -lt 10000 ]
	do
		b=`expr 1 + $b`
		name="frame${b}.yuv"

		if [ -f ${name} ]
		then 
			a=`expr 1 + $a`
			name_move="frame${a}.yuv"
			mv   ${name} ${name_move}
			echo ${name} "to" ${name_move}
		fi

	done
	
else
	echo  @#delete_yuv_file@#  >/dev/ttyACM0
	echo delete_yuv_file
fi

