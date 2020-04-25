for file in OS_PJ1_Test/*
do
	sudo dmesg -c > /dev/null
	opt_f=${file/OS_PJ1_Test/output}
	dmesg_f=${opt_f/\.txt/_dmesg\.txt}
	stdout_f=${opt_f/\.txt/_stdout\.txt}
	echo $stdout_f
	sudo ./project1.exe < $file > $stdout_f
	dmesg | grep Project1 > $dmesg_f
done
