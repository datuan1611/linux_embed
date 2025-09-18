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

.để kết thúc 1 thread chủ động: call pthread_exit() hoặc return trong hàm start_routine của thread
void pthread_exit(void *retval)
.để kết thúc 1 thread bị động: call pthread_cancel() ở 1 thread khác để huỷ thread này
int pthread_cancel(pthread_t thread)
.để block cho đến khi thread cần đợi kết thúc, và return value nếu thread đã kết thúc từ trước
int pthread_join(pthread_t thread, void **retval)

#4. Đồng bộ dữ liệu giữa các thread
.CPU không truy cập trực tiếp vào RAM,
 khi muốn dùng 1 biến trong RAM, CPU nạp biến đấy vào cache của core CPU
 CPU thao tác tính toán trên cache, sau 1 thời gian sẽ sync data ở cache vào RAM
.do cơ chế cache, khi lập trình multithread sẽ bị xung đột data, CPU không load được giá trị mới nhất của biến
.khi khai báo "volatile" cho biến -> CPU không sử dụng cache, mà truy cập thẳng vào RAM
 tuy nhiên khi tính toán, CPU sẽ thực hiện các việc:
 (1)nạp giá trị vào thanh ghi
 (2)tăng giá trị trong thanh ghi
 (3)truyền giá trị vào RAM
 do đó vẫn bị cache giá trị trong thanh ghi
 "volatile" vẫn không khắc phụ được vấn đề trên
 note:"volatile" không dùng cache nên xử lý tính toán bị chậm hơn
.khi sử dụng std::queue
 (1)thread_1 lấy dữ liệu từ sensor, dùng push_back() để đẩy vào queue
 (2)thread_2 dùng empty() để check queue có dữ liệu hay không, rồi dùng pop_front() để lấy dữ liệu từ queue
 note:
 	có bug sau khi thread_1 đẩy dữ liệu vào queue rồi, nhưng thread_2 vẫn check ra empty
	nguyên nhân là do khi thread_1 đẩy dữ liệu vào queue, biến count trong hàm empty() đã tăng nhưng vẫn ở cache, chưa lưu vào RAM
	do đó khi thread_2 check empty() thì biến count vẫn là 0, và hiểu là ở queue vẫn chưa có data

.khắc phục xung đột data bằng mutex hoặc semaphore
.MUTEX:
 chỉ có 1 khoá duy nhất, trong 1 thời điểm chỉ có 1 thread có được khoá, các thread khác phải đợi
 coding:
	int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr)
	int pthread_mutex_destroy(pthread_mutex_t *mutex)
	int pthread_mutex_lock(pthread_mutex_t *mutex)
	int pthread_mutex_unlock(pthread_mutex_t *mutex)
	int pthread_mutex_trylock(pthread_mutex_t *mutex)
