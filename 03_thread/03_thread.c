//unit3_thread

#1. Ứng Dụng
lập trình đa luồng giúp tối ưu hoá tài nguyên hardware trong hệ thống
giúp hệ thống chạy nhanh nhất, mượt mà nhất có thể

#2. Định Nghĩa
Thread: là luồng các lệnh mà CPU coi là 1 đối tượng khi sử dụng bộ lập lịch

.Ưu điểm: tối ứng được phần cứng
.Nhược điểm: có nguy cơ xung đột dữ liệu dùng chung, khó debug

#3. Triển khai source code
//library
#include <pthread.h>
//tạo thêm thread mới
int pthread_create(	pthread_t *restrict thread,				//thread ID
					const pthread_attr_t *restrict attr,	//thuộc tích của thread được tạo ra (NULL nếu để mặc định)
					void *(*start_routine)(void *),			//khi thread chạy thì call con trỏ hàm start_routine
					void *restrict arg );					//tham số truyền vào con trỏ hàm hàm start_routine (NULL nếu không truyền gì cả)
return 0; //if OK
return error number; //on failure

//đợi hàm start_routine kết thúc, cũng là lúc thread kết thúc
//nếu hàm start_routine của thread kết thúc rồi, thì pthread_join không đợi nữa
//pthread_join() đảm bảo (1) hàm start_routine đã kết thúc và (2) giải phóng tài nguyên thread
int pthread_join(	pthread_t thread,	//thread ID
					void **retval);		//giá trị trả về của hàm start_routine()

.khi compile source code thì phải thêm flag "-pthread"
gcc -g -o hellothread_exe -pthread hello_thread.c

.dùng cmd "time" để đo performance của chương trình
time hellothread_exe