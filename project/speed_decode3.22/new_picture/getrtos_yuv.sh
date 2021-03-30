#1/bin/sh
b=1
if [ $1 -eq 1 ]
then
	while true 
	do
		#sleep 3  
		#echo  @#send_yuv_file@#  >/dev/ttyACM0
		#sleep 1
		#mv -f frame.yuv  decode1.yuv
	       	#chmod 777 frame.yuv
	if [ -e "frame.yuv" ]
	then 
		a=1
	else
		rz > /dev/ttyACM0 < /dev/ttyACM0
		chmod 777 "frame.yuv"
		#b=$b+1
		b=`expr 1 + $b`

		name="frame+${b}.yuv"
		cp frame.yuv  ${name}
		echo ${name}
	fi
	done
	

elif [ $1 -eq 2 ]
then
	rm -rf decode.yuv
	echo  Factory_setting_file.conf  >/dev/ttyACM0
	sleep 1
	rz > /dev/ttyACM0 < /dev/ttyACM0
else
	echo  @#delete_yuv_file@#  >/dev/ttyACM0
	echo delete_yuv_file
fi

