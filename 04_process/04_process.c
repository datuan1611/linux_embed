//unit4_process

#1. Hàm MAIN
.là entry-point của chương trình
.prototype: int main(int argc, char *argv[])
.trước khi vào hàm main, kernel sẽ chạy 1 số đoạn code start routine để khởi tạo bộ nhớ cho process
 khi hàm main start, nó nhé nhận được 2 tham số từ kernel
.2 cách kết thúc chương trình
	kết thúc chủ động: call "return", gặp lệnh "exit()"
	kết thúc bị động: chương trình đang chạy thì bị crash (ex: Segment Fault)
 dựa vào mã trả về khi kết thúc của chương trình -> có thể phán đoán được nguyên nhân chương trình vuh crash

#2. Các tham số môi trường xung quanh 1 process
.biến môi trường được lưu ở global
	extern char **environ
.có thể truy xuất qua hàm getenv
 char *getenv(const char *name)
.hiển thị danh sách biến môi trường bằng cmd "printenv"

.biến global errno mang giá trị lỗi hàm system call gần nhất
	#include <errno.h>
 khi return failed nên printf thêm mã lỗi "errno" để dễ debug lỗi sau này
 example: keyword "errno 13 linux"

#3. Cấu trúc bộ nhớ của process
  ///high address
	------------------------
	....						//cmd line arguments and environment variables
	------------------------
	stack
	  |
	  v
	heap
	------------------------
	uninitialized data (bss)	//initialized to zero by exec
	------------------------
	initialized data			//read from program file by exec
	------------------------
	text						//read from program file by exec
	------------------------
  ///low address

.trong linux có thể check được con trỏ của 1 process đang trỏ vào vùng nhớ nào
	ps -aux
	ls /proc/741
	cat /proc/741/maps		//show ra con trỏ đang trỏ vào địa chỉ vùng nhớ nào

.cấp phát động bộ nhớ cho process
	void *malloc(size_t size)
	void free(void *ptr)
 khi cấp phát bộ nhớ bằng malloc, OS sẽ có 1 quyển sổ ghi lại trước đó process đã xin cấp phát bao nhiêu byte, bắt đầu từ vùng nào đến vùng nào
 nếu process kết thúc mà không free những vùng nhớ đã xin cấp phát, OS sẽ tra lại quyển sổ kia, và tự động free hết những vùng nhớ đã xin cấp phát trước đó
 -> lỗi memory leak chỉ xảy ra khi chương trình đang chạy lâu dài
 -> tip: có thể fix work-around khi bị memory leak bằng cách định kỳ sau 1 thời gian thì reset lại process

#4. Shared Library
.là các file binary được build từ code C
.chúng chứa các hàm để gọi từ chương trình khác
.mỗi khi cần truy xuất hàm từ thư viện động, OS sẽ load 1 phần hoặc toàn bộ thư viện đó vào RAM, sau đó trả về địa chỉ hàm cho process
 Process sẽ gọi hàm thông qua địa chỉ hàm

#5. Process identifier (PID)
.a unique number
.used to identify process
.some numbers is reserved for special process
	swapd process
	init process
.các cmd line dùng PID để thao tác với process
	ps -aux			//hiển thị danh sách các process đang chạy
	lsof -p 3865	//hiển thị danh sách các file được mở bởi process PID 3865

#6. fork function
	pid_t fork(void)
.tạo ra 1 process mới bằng cách duplicate process hiện tại (process parent)
.tạo ra 1 bản sao cho bộ nhớ hiện tại của process
 PID thật sẽ được trả cho process con
 PID 0 sẽ được trả về cho process cha
 process con có thể chạy và hoàn thành công việc trước process cha
.sinh sản vô tính, các process được sinh ra đều giống nhau
 process mới có file table giống process cũ, nên sẽ gọi các function giống nhau

.zombie process đã chết rồi, không thể kill được, ko chiếm CPU, vẫn chiếm ít memory
 khi process con chết, sẽ gửi 1 signal chứa mã trả về, thông báo "đã chết" tới process cha,
 nếu process cha không wait để nhận mã trả về, thì process con sẽ chuyển sang status zombie (Z+)
 khi process cha chết trước process con, thì process con zombie chuyển sang status "mồ côi", tìm process cha init (pid=1)
 trong init process sẽ có xử lý wait với process con, ko bị zombie process nữa
 giải pháp: ở process cha phải call hàm wait() để nhận mã trả về của process con
	#include <sys/wait.h>
	pid_t wait(int *wstatus)
 url: https://stackoverflow.com/questions/16944886/how-to-kill-zombie-process


#7. exec function
	int execl(const char *pathname, const char *arg0,...,NULL)
.khởi tạo process mới từ chương trình nằm tại pathname
 sau khi gọi hàm exec, toàn bộ source code phía sau của chương trình sẽ  không được thực hiện nữa
.exec sẽ thay không gian bộ nhớ của process hiện tại bằng new process image
 sẽ ghi đè process mới vào process cũ, ko sinh thêm 1 process nữa
 những dòng code tiếp theo của process cũ đã bị ghi đè, ko thể chạy nữa
