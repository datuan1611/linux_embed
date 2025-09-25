//unit2_file

Example:
.khi cài thư viện web-server trên linux thì không bị lệ thuộc phần cứng board là gì

trên mỗi board có phần cứng khác nhau
-> cần có driver giao tiếp với thanh ghi phần cứng (do hãng phát triển phần cứng viết)
-> tạo ra 1 file driver có tên do OS quyết định, đặt ở /dev
   driver được viết theo chuẩn file do OS quy định, xuất phát từ UNIX -> LINUX kế thừa
   với Windows và macOS thì khác, không theo chuẩn file

LINUX:
   Khi gửi lệnh in trên APP (Word,Excel...)
   Luồng dữ liệu:
     .ghi dữ liệu vào device file của máy in
     .truyền tiếp dữ liệu tới driver
     .đẩy vào port kết nối của máy in

WINDOWS:
   Dữ liệu không truyền theo chuẩn file, mà theo I/O Registry
   Ở Windows > Device Manager > truyền data vào node trên tree > đẩy vào Driver

Filesystem của Linux    (tree thư mục)
    /           //root
        bin         //cmd line + application
        boot        //file image dùng để boot OS (image kernel + eboot nạp OS + device-tree)
        dev         //device file
        etc         //file config của các app


#1. Định Nghĩa
.là instance của 1 đối tượng nào đó: process, device, data
.Linux quản lý mọi thứ thông qua file
.struct file include /linux/fs.h

#2. Ứng Dụng
.viết app đọc ghi file để test driver phần cứng
.viết app đọc ghi file quản lý process, device

#3. Phân Loại
.file thật:
   có vùng lưu trữ thật trên ổ cứng, tắt máy đi vẫn còn
.file ảo:
   được tạo ra khi OS running, lưu ở RAM, tắt máy đi sẽ mất
   vì lưu vào RAM nên xử lý đọc/ghi file sẽ nhanh hơn

#4. Các Hàm Đọc/Ghi File
.int open(const char* pathname,int flags)
.int close(int fd)
.size_t read(int fd,void *buf, size_t count)
.size_t write(int fd,void *buf, size_t count)
.off_t lseek(int fd,off_t offset,int whence)
.void sync(void)

3 step:
	open()
	read()	/ write()
	close()

Detail:
open()
	flag:
		O_APPEND	:khi mở file, data được ghi vào cuối file, không ghi đè/xoá file cũ
		O_CREAT		:khi mở file đã có, xoá file cũ, tạo file mới để ghi data mới
		O_DIRECT	:mở file thực hiện direct, không qua bộ nhớ đệm bằng fflush()
	pathname:
		thuyệt đối	:path từ thư mục /
		tương đối
	return:
		OK	:con trỏ file fd, giá trị int >= 3
		NG	:giá trị < 0
fflush()
    đẩy dữ liệu vào bộ nhớ đệm (thường là RAM), khu vực nhớ: DMA, cache phụ
	do device ngoại vi(implement) không xử lý nhanh bằng CPU(request)
sync()
    sync data ở RAM và thiết bị ngoại vi
read()
	chỉ định đọc count byte từ con trỏ file fd -> ghi vào vùng nhớ *buf
	return giá trị số byte đọc được
write()
	chỉ định ghi count byte từ vùng nhớ *buf -> ghi vào con trỏ file fd
	return giá trị số byte đọc được
close()
	xong khi đọc xong thì close file
	nếu mở quá nhiều file cùng lúc -> process sẽ bị treo

Reference:
	man7.org
	linux.die.net

Example:
	touch data
		Do Anh Tuan
	touch hellofile.c
	/*----------------------------------------------------------
		#include <stdio.h>
		#include <fcntl.h>
		
		void main()
		{
			int fd = -1;
			const char path[] = "/home/jk/Desktop/02_file/data";
			char c = 0;
		
			fd = open(path, O_RDONLY);
			if( fd < 0 ) {
				printf("Can not open file\n");
				return;
			}
		
			while( read(fd, &c, 1) == 1 )
				printf("%c", c);
			printf("\n");
		
			close(fd);
		}
	----------------------------------------------------------*/
	touch print_mem_size.c
	/*----------------------------------------------------------
		//open file /proc/meminfo
		//printf capacity free of RAM

		#include <stdio.h>
		#include <fcntl.h>
		#include <unistd.h>
		#include <string.h>

		#define BUFFER_SIZE 1024

		int main()
		{
			const char path[] = "/proc/meminfo";
			const char keyword[] = "MemFree:";
			char buffer[BUFFER_SIZE];

			int fd = open(path, O_RDONLY);
			if( fd == -1 ) {
				perror("Can not open file.\n");
				return 1;
			}

			off_t position = 0;
			ssize_t bytesRead;
			int found = 0;

			while( (bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0 ) {
				for( int i = 0; i <= bytesRead - strlen(keyword); i++ ) {
					if( memcmp(&buffer[i], keyword, strlen(keyword)) == 0 ) {
						position += i;
						found = 1;
						break;
					}
				}
				if( found ) break;
				position += bytesRead;
			}

			char c = 0;
			if( found ) {
				lseek(fd, position, SEEK_SET);
				while( read(fd, &c, 1) == 1 && c != 10 )
					printf("%c", c);
				printf("\n");
			}

			close( fd );
			return 0;
		}
	----------------------------------------------------------*/

#TIP:
.khi chạy 1 process sẽ tạo 1 bảng file_table để quản lý các file được mở
	ps -aux | grep print_mem_size	//list và tìm process print_mem_size đang chạy
	lsof -p 3889					//scan bảng file_table, và list danh sách các file đang mở bởi process có PID:3889
	ls /proc/3889/fd				//bảng file_table khi chạy process 3889
.xem open-source kernel implement file_table
	keyword: task_struct linux kernel
	reference: elixir.bootlin.com/linux/lastest/source/linux/sched.h
	mỗi process sẽ tạo 1 task_struct để quản lý data
	struct file_struct	*files;		//con trỏ array 1 chiều
		struct file __rcu * fd_array[NR_OPEN_DEFAULT];	//bảng file table, chứa index các file đang mở
		struct file	{};				//chứa thông tin file đang mở
.các hàm fopen(), fclose() dễ coding hơn, tuy nhiên ít tham số và ít control phần cứng hơn so với hàm gốc open(), close()
.hàm sync() sẽ đồng bộ data từ cache ra file, tuy nhiên sẽ sync tất cả các file trong hệ thống
 có thể dùng O_DIRECT để tự động sync từ cache ra file, tuy nhiên tốc độ đọc/ghi file sẽ chậm
 
 #Homework
 1. Viết app key-logger
	.tìm file đại diện driver của bàn phím trong thư mục /dev
	 keyword: how to read keyboard buffer in linux
	.open file driver, đọc dữ liệu
	.detect khi user ấn phím "P" thì hiển thị ra màn hình
		#include <linux/input.h>
		struct input_event{};
		/dev/input/event2

#Bổ Sung:
.có các loại file sau:\
	regular file: file thông thường, như text file, executable file
	directory file: file chứa danh sách các file khác
	character device file: file đại diện cho các thiết bị không có địa chỉ vùng nhớ
	block device file: file đại diện cho các thiết bị có địa chỉ vùng nhớ
	link file: file đại diện cho 1 file khác
	socket file: file đại diện cho 1 socket
	pipe file: file đại diện cho 1 pipe
.khi process sử dụng read()/write() để đọc/ghi file
	#read()
	1-kernel xác định page cần đọc
	2-kernel đọc từ Page Cache
	3-nếu page có trong Page Cache, thì thông tin sẽ được đọc ra
	4-nếu page không có trong Page Cache, thì đọc từ vùng nhớ vật lý vào Page Cache, sau đó đọc ra cho userspace
	#write()
	1-kernel ghi nội dung page vào Page Cache
	2-Page Cache sẽ được ghi vào vùng nhớ vật lý định kỳ, hoặc khi dùng các lệnh sync(), fsync()
