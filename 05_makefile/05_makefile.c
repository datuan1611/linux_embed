//05_MakeFile

#1. Makefile
.Makefile là 1 script, bên trong có chứa các thông tin
	.cấu trúc của 1 project (file,dependency)
	.các command line dùng để tạo/huỷ file
.chương trình make sẽ đọc nội dung trong Makefile và thực thi nó
	target: dependency
	[tab]	[action]
	/*------------------------------
	all:
		gcc -c hello hello.c main.c -I.
	clean:
		rm -rf hello
	------------------------------*/
.cmd
	make		//compile
	make clean	//remove execute file "hello"

.có thể đặt tên Makefile theo các cách sau
	Makefile	//ưu tiên chạy file này
	makefile
	abcd.mk		//thêm vào Makefile: include abcd.mk

.gán giá trị cho biến
	var1  = $(var)	//recursive assignment	-> if var changes, var1 changes
	var2 := $(var)	//direct assignment		-> if var changes, var2 not changes
	var3 ?= $(var)	//if var3 had value, not assign; if var3 not had value, assign recursively
.biến môi trường
	$@		giá trị target, bên trái ":"
	$<		giá trị dependance đầu tiên, bên phải ":"
	$^		tất cả giá trị dependance, bên phải ":"

.Error: "rule1" is up to date
	nguyên nhân: trong folder có file "rule1" trùng tên với rule "rule1" trong Makefile
	giải pháp: sử dụng ".PHONY:" và liệt kê các rule trong Makefile
		.PHONY: rule1 rule2 rule3

#2. Quá trình compile 1 chương trình C
(1)Pre-processing	//giai đoạn tiền xử lý
	.loại bỏ comment
	.mở rộng các macros
	.mở rộng các include file
	.biên dịch các câu lệnh điều kiện
	.kết quả thu được ở giai đoạn này là 1 file ".i"
(2)Compilation		//giai đoạn dịch ngôn ngữ bậc cao sang asm
	.biên dịch từ file ".i" thu được ở bước pre-processing thành 1 file ".S" (assembly)
(3)Assembly			//giai đoạn dịch asm sang mã máy
	.biên dịch từ file ".S" thu được ở bước compilation thành 1 file ".o"
	.".o" là file chứa các chỉ lệnh cấp độ ngôn ngữ máy (machine language)
(4)Linhking			//giai đoạn linker link các file object thành 1 file execute
	.link các file ".o" thu được ở bước assembly thành 1 file execute

/*------------------Makefile------------------
.PHONY: hello stage1 stage2 stage3 stage4 all clean
hello:
	gcc -c -o main.o main.c
stage1:
	gcc -E hello.c -o hello.i
stage2:
	gcc -S hello.i -o hello.S
stage3:
	gcc -c hello.S -o hello.o
stage4:
	gcc -o hello hello.o main.o
all:
	gcc -o hello hello.c
clean:
	rm -rf hello hello.i hello.s hello.o main.o
---------------------------------------------*/

#3. Static lib và Shared lib
.thư viện là 1 tập hợp các đoạn mã được biên dịch sẵn để có thể được sử dụng lại trong 1 chương trình
.được chia làm 2 loại:
	Static lib
	Shared lib

	source file		source file		source file
		|				|				|
		v				v				v
	object file		object file		object file
		\				|				/
			\			|			/
				v		v		v
	runtime 	----> Linker  ----------->	executable
	library									program

.Linking time:
	Static lib:
		tất cả module trong thư viện được copy vào file thực thi ở thời điểm compile time
		khi OS load chương trình chỉ load 1 file thực thi duy nhất vào bộ nhớ
		(file thực thi đã bao gồm file source + static lib)
	Shared lib:
		file thực thi + file thư viện (shared lib) đều được load vào bộ nhớ ở thời điểm runtime
		1 shared lib có thể được nhiều chương trình sử dụng (dynamic linking)
.size
	Static lib: tốn nhiều bộ nhớ hơn
	Shared lib: tốn ít bộ nhớ hơn
.external file changes
	Static lib: file thực thi phải re-compile lại khi có sự thay đổi trong static lib
	Shared lib: file thực thi KHÔNG phải re-compile lại khi có sự thay đổi trong shared lib
.time
	Static lib: mất nhiều thời gian thực thi hơn
	Shared lib: mất it thời gian thực thi hơn
		nguyên nhân là do static lib mất thời gian load lib từ ổ cứng lên RAM hơn
		với shared lib thì đã load sắn lên RAM rồi

.Cách tạo thư viện static_lib và shared_lib
	key: building and using static and shared C libraries
	url: https://docencia.ac.upc.edu/FIB/USO/Bibliografia/unix-c-libraries.html
	static_lib
		ar rcs libutil.a util_file.o util_net.o util_math.o	//create static_lib
		ranlib libutil.a	//create index for static_lib
		cc main.o -L. -lutil -o prog	//util là tên lib đã được lược bỏ lib*.a
	shared_lib
		cc -fPIC -c util_file.c		//compile source code with PIC obtion
		cc -fPIC -c util_net.c
		cc -fPIC -c util_math.c
		cc -shared libutil.so util_file.o util_net.o util_math.o	//create shared_lib
		cc main.o -L. -lutil -o prog	//util là tên lib đã được lược bỏ lib*.so

	key: createing a shared and static library with the gnu compiler (gcc)
	url: https://renenyffenegger/notes/development/languages/C-C-plus-plus/GCC/create-libraries/index
		gcc -c main.c -o obj/main.o -I./inc
	static_lib
		gcc -c src/hello_A.c -o obj/static/hello_A.o -I./inc
		gcc -c src/hello_B.c -o obj/static/hello_B.o -I./inc
		ar rcs lib/static/libhello.a obj/static/hello_A.o obj/static/hello_B.o	//create static_lib
		gcc obj/main.o -Llib/static -lhello -o bin/static_exec
			//-L: indicate directory of libraries
			//-l: indicate name of libraries (ommited lib*.a/lib*.so)
		./bin/static_exec		//can be distributed without *.a and *.o
	shared_lib
		gcc -c -fPIC src/hello_A.c -o obj/shared/hello_A.o -I./inc	//compile source code with -fPIC obtion
		gcc -c -fPIC src/hello_B.c -o obj/shared/hello_B.o -I./inc	//compile source code with -fPIC obtion
		gcc -shared  obj/shared/hello_A.o obj/shared/hello_B.o -o lib/shared/libhello.so	//create shared_lib
		gcc obj/main.o -Llib/shared -lhello -o bin/shared_exec //linker similar to static_lib
		//Using shared libraries
		//	Method 1: copy *.so into default location /usr/lib
		sudo cp ./lib/shared/libhello.so /usr/lib
		sudo chmod 755 /usr/lib/libhello.so
		./bin/shared_exec
		//	Method 2: use shared lib with LD_LIBRARY_PATH
		export LD_LIBRARY_PATH=$(pwd)/lib/shared
		./bin/shared_exec
