//06_IPC
//from SWAT of FPT

#0. IPC (Inter Process Communication)
.trao đổi dữ liệu giữa các process trong cùng 1 machine
/*---------------------------------------------------
User space		App1 <--IPC--> App2			App3
App layer			\			|			/
						\		|		/
							\	|	/
Kernel						  Kernel
OS							/	|	\
						/		|		\
					/			|			\
Hardware		CPU			Memory			Driver
Layer
----------------------------------------------------*/

.ICP techniques
	(1) using Unix Domain Socket
	(2) Message Queues
	(3) Shared Memory
	(4) Signals

#1. Shared memory
.cơ chế trao đổi dữ liệu thông qua 1 vùng nhớ của kernel (Kernel Memory)
 không process nào sở hữu vùng nhớ này

.khi 1 process execute,
 nó được cấp phát 1 vùng nhớ của hệ thống, là Virtual Address Space (VAS)
 mỗi process chỉ có thể access 1 vùng nhớ VAS
 process ko được phép read/write vùng nhớ ngoài VAS
 nếu process truy cập vùng nhớ ngoài VAS -> OS sẽ kill process (Segment Fault)

.Kernel Memory
 là vùng nhớ của kernel, thực hiện các task như là Scheduler, Timer, Interupt, Process Management...
 process không sử hữu kernel memory, nhưng có thể request kernel tạo vùng nhớ ở kernel memory
 process không thể access directly vào kernel memory, mà phải thông qua cơ chế Shared Memory

.Cơ chế hoạt động
/*-------------------------------------------/
User space		process A			process B
					|					^
					|					|
OS/Kernel			|->	shared memory --|
/-------------------------------------------*/

/*---------------------------------------------------------------------------/
	<PROCESS A>		|									|	<PROCESS B>	
	[stack]			|									|	[stack]
---------------------									---------------------
Program Control Block									Program Control Block
	A		A*												B		B*
	| mmap |												| mmap |
			\												/
				\	<WRITE>						<READ>	/
					\								/
						\						/
							K				K*
	[heap]					| kernel memory |				[heap]
---------------------									---------------------
	[data]			|			OS/Kernel				|	[data]
	[code]			|									|	[code]
/----------------------------------------------------------------------------*/
kernel memory từ address [K] đến address [K*]
process request OS cấp phát 1 vùng nhớ mmap ở heap của VAS từ address [A] đến address [A*]
process sử dựng system call mmap() để ánh xạ vùng nhớ trên VAS với vùng nhớ trên kernel memory
/*---------Address Mapping----------*/
[A]		-> 		[K]		->		[B]
[A*]	-> 		[K*]	->		[B*]
size: A* - A = K* - K = B* - B (bytes)
/*----------------------------------*/
data thực sự nằm trên vùng nhớ kernel memory, không phải trên vùng nhớ mmap ở VAS
khi process access địa chỉ [Ai] nằm giữa [A] < [Ai] < [A*] -> thì kernel translate ra địa chỉ [Ki] nằm giữa [K] < [Ki] < [K*]

các process không biết khi nào shared memory được update
sau khi process A write data vào shared memory, process A cần notify cho process B biết (sử dụng IPC khác, như là socket,msgQ)
sau đó, process B có thể read data mới ở shared memory, rồi update thông tin nội bộ phía process B

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int shm_open(const char *name, int oflag, mode_t mode)	//create/open a POSIX shared memory object inside Kernel
		//oflag: O_RDONLY | O_RDRW | O_CREAT | O_TRUNC
int shm_unlink(const char *name)	//remove shared memory object
int truncate(const char *path, off_t length)	//re-size the shared memory object to new size
void *mmap(void addr[length], size_t length, int prot, int flags, int fd, off_t offset)	//mapping kernel memory segment into process's VAS
int munmap(void addr[length], size_t length)		//unmap shared memory

//Sender
unsigned int size;
int shm_fd = shm_open(mmap_key, O_CREAT | O_RDWR | O_TRUNC, 0660);
ftruncate(shm_fd, size);
void* shm_req = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
//TODO
munmap(shm_req, size);
close(shm_fd);

//Receiver
unsigned int byte_to_read;
int shm_fd = shm_open(mmap_key, O_CREAT | O_RDONLY, 0660);
void* shm_req = mmap(NULL, byte_to_read, PROT_READ, MAP_SHARED, shm_fd, 0);
//TODO
munmap(shm_req, byte_to_read);
close(shm_fd);

#2. Message Queue
.cơ chế trao đổi dữ liệu giữa các process trong cùng 1 machine
.1 process có thể tạo 1 msgQ mới, hoặc sử dụng lại msgQ đã tạo trước đó

.mỗi msgQ được identify bằng ID duy nhất, và được quản lý bởi OS/Kernel
.process sender post data to msgQ -> process receiver read data from msgQ
.process create msgQ -> owner/creator của msgQ
.msgQ là cơ chế nhiều sender - 1 receiver
/*----------------------------------------------------------------------------/
User space		process A		process B					process C
					|		(n)		|							^	(1)
					|				|							|
System 				|				|		mq_open()			|
call				|				|		mq_send()			|
interface			|				|		mq_receive()		|
					|				|							|
		(enqueuer)	|				|							|	(dequeuer)
					v				v							|
OS/Kernel			-------------------->	Message Queue	-----
/----------------------------------------------------------------------------*/

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

mqd_t mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr);		//create a new POSIX msgQ / open existing msgQ
		//oflag: O_RDONLY | O_WRONLY | O_RDRW | O_CLOEXEC | O_CREAT | O_EXCL | O_NONBLOCK
int mq_close(mqd_t mqdes);			//close msgQ
int mq_send(mqd_t mqdes, const char msg_ptr[msg_len], size_t msg_len, unsigned int msg_prio);	//sending process place message in msgQ
ssize_t mq_receive(mqd_t mqdes, char msg_ptr[msg_len], size_t msg_len, unsigned int msg_prio);	//receiving process dequeuer a msg from msgQ
int mq_unlink(const char *name);	//owner process destroy msgQ

//Sender
char buffer[MSG_BUFFER_SIZE];
mqd_t msgQ_id;
int msgQ_fd = mq_open(msgQ_id, O_WRONLY | O_CREAT, 0, 0);
//TODO
mq_send(msgQ_fd, buffer, strlen(buffer)+1, 0);
mq_close(msgQ_fd);

//Receiver
char buffer[MSG_BUFFER_SIZE];
mqd_t msgQ_id;
int msgQ_fd = mq_open(msgQ_id, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr);
while (TRUE)
{
	mq_receive(msgQ_fd, buffer, MSG_BUFFER_SIZE, NULL);
}
//TODO

/*NOTE*/
	.khi build source msgQ cần thêm flag [-lrt]
		gcc sender.c	-o sender	-lrt
		gcc receiver.c	-o receiver	-lrt
	.tên msgQ_id cần bắt đầu bằng "/"
		./sender /testqueue
		./receiver /testqueue
	.blocking/nonblocking
		ở hàm mq_open() có tham số oflag [O_NONBLOCK] (default là blocking)
		ở mode blocking:
			-sender: nếu queue full khi muốn gửi msg đi -> chờ đến khi có slot thì gửi msg vào queue
			-receiver: nếu queue empty khi đang đợi msg đến -> chờ đến khi có slot trong queue thì nhận msg
		ở mode nonblocking:
			-sender: nếu queue full khi muốn gửi msg đi -> thoát msgQ luôn -> send failed
			-receiver: nếu queue empty khi đang đợi msg đến -> thoát msgQ luôn -> receive failed
	.msgQ trong linux theo cơ chế nhiều sender - 1 receiver
		nếu có nhiều receiver, khi có msgQ đến, receiver nào nhận trước thì lấy trước, rồi xoá msg khỏi queue
