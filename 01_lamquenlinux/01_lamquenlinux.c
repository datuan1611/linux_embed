///unit1_lamquenlinux

#1. Cài app trên linux board

・Cách 1: Copy thủ công app vào cây thư mục
 dùng yocto hoặc build root
	tải code của app về
	build code ra file binary
	copy file binary vào /boot
 build app tạo ra image
 cách cài khó hơn, cài được cho nhiều board cùng lúc

・Cách 2: Dùng apt cài thủ công vào board
 tạo file image cho sdcard
 cách này đơn giản hơn, nhưng chỉ cài cho từng board

#2. Công cụ lập trình
IDE
	text editer
	compiler
	debugger

vi
	mode
		command		//press ESC
			:set number			//show line number
			:set autoindent		//tự động tab khi xuống dòng
			:syntax on			//hiển thị color trong vim
			:x					//nhảy đến dòng x
			/string				//tìm kiếm chuỗi string
			:wq					//save và quit
		insert		//press i

	//helloworld.c
	1	#include <stdio.h>
	2	void main()
	3	{
	4		int birth_year = 1989;
	5		int current_year = 2021;
	6		printf("I am %d years old.\n",current_year - birth_year);
	7	}

compile
	gcc -o hello_exe helloworld.c		//build ra file binary
	./hello_exe

debug
	gcc -g -o hello_exe helloworld.c	//nhúng code vào file binary
	objdump -D -s hello_exe > dump.txt	//dịch ngược file binary ra code
	gdb hello_exe
	br 6				//đặt break-point ở dòng 6 ở file source helloworld.c
	run
	print birth_year	//in ra giá trị biến birth_year
	print current_year	//in ra giá trị biến current_year
	quit
	

#TIP:
・cài đặt gcc, gdb
	sudo apt install build-essential
	sudo apt install libmpfr-dev libgmp3-dev libmpc-dev -yđặt
・cài đặt tools cần thiết
	sudo apt install net-tools
	sudo apt install tree
・tìm kiếm trong linux
	grep -nrwI <keyword>
		-n	show line number
		-r	tìm đệ quy, tìm trong cả các folder con
		-w	tìm chính xác keyword
		-I	ignore file binary
	※Nhược điểm: ko phân biệt được code và text, nên tìm ra rất nhiều vị trí

	cscope -R -p4		//parser database về code
		-R	parser cả folder con
	Find this C symbol:				//tìm ký tự, biến, hàm
	Find this global definition:	//tìm các biến global
	...
	Ctrl + D						//exit
・cài đặt VSCode để debug code trên board
	board phải kết nối internet
	phải có APT
		để cài đặt APT trên board
		(1) tải code APT
		(2) build ra file binary
		(3) copy vào thư mục /bin/apt
	cài ssh-server lên linux
		sudo apt update
		sudo apt upgrade -y
		sudo apt install openssh-server
		sudo systemctl enable --now ssh
		sudo systemctl status ssh
		sudo ufw status
		sudo ufw allow ssh
		ssh username@IP_address
		ssh username@domain
		exit
		---
		sudo systemctl start ssh
		sudo systemctl restart ssh
		sudo systemctl stop ssh
		sudo systemctl disable ssh
		sudo apt remove openssh-server
		sudo ufw delete allow ssh
	cài extension trên VSCode
		C/C++
		Remote Development
	kết nối VSCode với SSH
		.Ctrl+P
		>ssh username@IP_Address
		.enter password
		.File > Save Workspace As...
