//06_IPC
//from Phú Lưu An
//url:	https://www.youtube.com/watch?v=EOEmic-870k		: IPC introduction
//		https://www.youtube.com/watch?v=ViWv0t8U_-8		: signal
//		https://www.youtube.com/watch?v=RP-PIdqnqiY		: shared mempry
//		https://www.youtube.com/watch?v=-Fs6wAV7tEw		: socket

//====================================================

#if 1	//0. INTRODUCTION

#0. INTRODUCTION
.IPC (Inter Process Communication): cơ chế trao đổi thông tin giữa các process với nhau
.Linux Embed: focus vào giao tiếp giữa App ở tầng User Space với driver ở tầng Kernel Space

//Common IPC
.Share memory
	2 process chia sẻ 1 vùng nhớ vật lý
	.ưu điểm: vùng nhớ chia sẻ lớn, có thể chia sẻ các struct phức tạp, tốc độ truy cập nhanh
	.nhược điểm: implement phức tạp, khó lock/unlock để sync data
	suggest: chỉ nên dùng để chia sẻ thông tin ở tầng user mode
	thường kết hợp giữa shared memory + signal để sync được data, tuy nhiên sẽ mất nhiều time hơn
	thay vào đó có thể dùng socket đã hỗ trợ lock/unlock data, chậm hơn shared mem, nhưng nhanh hơn shared mem + signal
.Pipe
	1 process cha sinh ra 1 process con -> process cha tạo ra 1 pipe (hoạt động như file) để giao tiếp với con
	.ưu điểm: implement đơn giản, giống như open file
	.nhược điểm: chỉ giao tiếp được cha-con, hoạt động 1 chiều
.Semaphore
	cơ chế gửi nhận tín hiệu, không phải trung chuyển 1 data message
.Signal
	cơ chế gửi nhận tín hiệu, có thể gửi tới bất kỳ process nào đó
	có thể xử lý signal bất thường từ hệ thống
	có 32 giá trị define, trong đó có 4 giá trị trống có thể viết hàm handle để xử lý riêng

	1 process có 1 không gian bộ nhớ VAS
	ở đầu VAS có 1 vùng memory PD (process descriptor) chứa các thông tin của process,
	trong PD chứa 1 bảng signal_handler(), có 32 hàng, mỗi hàng có index hàng + địa chỉ hàm handle() cho signal
	khi đăng ký 1 signal = đăng ký con trỏ hàm handler() vào bảng để xử lý cho signal đấy
	nên bảng signal_handler chỉ có tác dụng với process có PD đó thui
	ở bảng signal_handler có:
		4 ô trống -> có thể tạo mới, define 1 signal mới
		1 số ô có thể ghi đè -> ví dụ define riêng Ctrl+C để ko exit process
		(đa số các ô không cho phép ghi đè, ví dụ SIGKILL)

	hàm đăng ký signalHandler(index của signal, con trỏ hàm xử lý cho signal )

.Socket
	được dùng nhiều nhất
	hỗ trợ locking, sync dữ liệu
	có thể gửi dữ liệu phức, dữ liệu đơn
	có thể gửi ra ngoài máy tính (qua internet)
	nhược điểm: chậm hơn shared memory

.chỉ 1 vài cơ chế IPC có thể giao tiếp giữa user space và kernel space

//IPC for uer - kernel space
các IPC thường được dùng để giao tiếp giữa user space và kernel space
	.procfs
	.sysfs
	.system call
	.ioctl (là 1 loại system call, nhưng có thể specific riêng prototype cho driver)
	.device file
	.socket

độ phổ biến:
	device file > ioctl > proc > sysfs > system call > socket
	(socket chậm nên ít dùng giữa user-kernel space)

.procfs
	các file procfs được lưu ở folder "/proc"
	khi đọc/ghi file procfs có luồng xử lý riêng
	.ưu điểm: dễ dùng, có thể tự đọc/ghi bằng command line (cat,echo...hoặc python)
	.nhược điểm: chủ yếu giao tiếp dạng string (nếu là con trỏ thì ko lấy được data)
	suggest giao tiếp driver với con người, show thông tin dạng string
	lưu ý:
		user có thể gây lỗi crash với kernel (ghi ký tự lạ, tràn buffer),
		với cơ chế khác có thể được kernel chuẩn hoá data nên ko bị crash

.sysfs
	các file sysfs được lưu ở folder "sys"
	
	file sysfs có thể đọc/ghi 1 cách thụ động (với procfs thì phải implement hàm đọc/ghi)
	user có thể đọc/ghi thẳng xuống sysfs mà driver (kernel) không biết
	driver phải compare giá trị trước/sau ở sysfs để check user có ghi giá trị mới ở sysfs hay không
	đánh giá độ ổn định: sysfs dễ gây crash hơn procfs, vì có thể bị ghi bị động bởi user

	sysfs: sau khi đăng ký thành công driver -> hệ thống sẽ tự tạo ra 1 số file sysfs mặc định
	procfs: muốn tạo file nào thì user phải tự tạo file đó

	sysfs: tự động clean-up directory/file khi device được remove khỏi hệ thống
	sysfs: có linking đến các file khác trong hệ thống (ví dụ /dev)

.system call
	parent of all user-kernel IPC

.ioctl
	được dùng phổ biến để giao tiếp user-kernel
	ưu điểm: linh hoạt, có thể handle với data structure phức tạp
	nhược điểm:
		phải parser dữ liệu người dùng truyền xuống (ép kiểu mới đọc được)
		user có thể gây lỗi hệ thống (tràn buffer)
		chỉ giao tiếp giữa process-process, ko hỗ trợ command line (cat,echo... hoặc python)

.device file
	1 file có struct file_operations để define các phương thức xử lý file
		open, close, read, write... ioctl... chmod, chown...
	khi kernel giao tiếp với hardware, chỉ dùng device file ở thư mục /dev với open, close, read, write
	mất nhiều effort để implement (phức tạp hơn nhiều với ioctl)

	khi viết driver cho màn hình
	chuyển mode màn hình, config màn hình -> dùng qua ioctl (ko phải read/write data struct phức tạp)
	đẩy data -> ép read/write qua device file
	nếu truyền raw data: ioctl chậm hơn với read/write của device file

#endif	//0. INTRODUCTION

//====================================================

#if 1	//#1. SIGNAL

#1. SIGNAL
.là cơ chế truyền tín hiệu giữa các process
.bản chất là software interupt
 OS cung cấp cho mỗi process 1 bảng signal table, process đăng ký các hàm handler xử lý signal vào đấy
 khi có 1 signal xảy ra với process, thì OS sẽ tự động call hàm handler xử lý cho signal đấy
 signal xảy ra là 1 event ngẫu nhiên từ bên ngoài tới process
 khi phát sinh signal, process xử lý signal như là xử lý interupt
	process sẽ dừng hết ở luồng xử lý chính, lưu thông tin process vào stack
	process nhảy vào vào luồng xử lý signal kia (hàm handler đã đăng ký)
	sau khi xử lý xong signal, process restore thông tin ở stack và quay lại chạy tiếp luồng xử lý chính
.bảng signal có 32 cell, để đăng ký signal handler

.các trường hợp xảy ra signal
	user sử dụng command line (ex: kill)
	process gửi signal đi
	chương trình gặp lỗi (ex: chạy con trỏ NULL, mở file ko tồn taị)
	user ấn 1 số tổ hợp phím đặc biệt (ex: Ctrl+C)

.có 4 loại signal
	signal có thể ignore (ex: tổ hợp phím Ctrl+C)
	signal không thể ignore (ex: chia cho "0", truy cập con troe NULL, ghi vào 1 file invalid)
	signal có thể chủ động điều khiển được (có thể viết 1 hàm handler cho nó, rồi đăng ký với OS)
	signal không thể chủ động điều khiển được (OS có 1 số hàm handler xử lý cho signal đó, phải dùng những hàm này để xử lý cho signal)
		//với signal ignore được thường cũng lập trình được hàm handler cho signal đó

.signal table
	khi khởi tạo process, OS đính kèm mỗi process với 1 bảng signal table
	là 1 mảng 1 chiều có 32 cell, chứa con trỏ hàm để trỏ đến hàm handler xử lý cho signal đó
	khi mới khởi tạo, tất cả các signal handler đều là default handler
	với cell có trạng thái Read Only -> ko thể thay đổi trạng thái, ko thể override hàm handler
	với cell có trạng thái Read Write -> có thể thay đổi trạng thái, có thể override hàm handler

	struct task_struct mô tả 1 process, có các trường thông số mô tả signal
		struct signal_struct	*signal;
		struct sighand_struct	*sighand;
		struct sigpending		pending;

		SIGABRT      P1990      Core    Abort signal from abort(3)
		SIGALRM      P1990      Term    Timer signal from alarm(2)
		SIGBUS       P2001      Core    Bus error (bad memory access)
		SIGCHLD      P2001      Ign     Child stopped, terminated, or continued
		SIGCLD         -        Ign     A synonym for SIGCHLD
		SIGCONT      P1990      Cont    Continue if stopped
		SIGEMT         -        Term    Emulator trap
		SIGFPE       P1990      Core    Erroneous arithmetic operation
		SIGHUP       P1990      Term    Hangup detected on controlling terminal, or death of controlling process
		SIGILL       P1990      Core    Illegal Instruction
		SIGINFO        -                A synonym for SIGPWR
		SIGINT       P1990      Term    Interrupt from keyboard
		SIGIO          -        Term    I/O now possible
		SIGIOT         -        Core    IOT trap. A synonym for SIGABRT
		SIGKILL      P1990      Term    Kill signal
		SIGLOST        -        Term    File lock lost (unused)
		SIGPIPE      P1990      Term    Broken pipe: write to pipe with no readers; see pipe(7)
		SIGPOLL      P2001      Term    Pollable event (Sys V); synonym for SIGIO
		SIGPROF      P2001      Term    Profiling timer expired
		SIGPWR         -        Term    Power failure (System V)
		SIGQUIT      P1990      Core    Quit from keyboard
		SIGSEGV      P1990      Core    Invalid memory reference
		SIGSTKFLT      -        Term    Stack fault on coprocessor (unused)
		SIGSTOP      P1990      Stop    Stop process
		SIGTSTP      P1990      Stop    Stop typed at terminal
		SIGSYS       P2001      Core    Bad system call (SVr4), see also seccomp(2)
		SIGTERM      P1990      Term    Termination signal
		SIGTRAP      P2001      Core    Trace/breakpoint trap
		SIGTTIN      P1990      Stop    Terminal input for background process
		SIGTTOU      P1990      Stop    Terminal output for background process
		SIGUNUSED      -        Core    Synonymous with SIGSYS
		SIGURG       P2001      Ign     Urgent condition on socket
		SIGUSR1      P1990      Term    User-defined signal 1
		SIGUSR2      P1990      Term    User-defined signal 2
		SIGVTALRM    P2001      Term    Virtual alarm clock
		SIGXCPU      P2001      Core    CPU time limit exceeded, see setrlimit(2)
		SIGXFSZ      P2001      Core    File size limit exceeded, see setrlimit(2)
		SIGWINCH       -        Ign     Window resize signal
		
		
		Signal        x86/ARM     Alpha/   MIPS   PARISC   Notes
		            most others   SPARC
		─────────────────────────────────────────────────────────────────
		SIGHUP           1           1       1       1
		SIGINT           2           2       2       2
		SIGQUIT          3           3       3       3
		SIGILL           4           4       4       4
		SIGTRAP          5           5       5       5
		SIGABRT          6           6       6       6
		SIGIOT           6           6       6       6
		SIGBUS           7          10      10      10
		SIGEMT           -           7       7      -
		SIGFPE           8           8       8       8
		SIGKILL          9           9       9       9
		SIGUSR1         10          30      16      16
		SIGSEGV         11          11      11      11
		SIGUSR2         12          31      17      17
		SIGPIPE         13          13      13      13
		SIGALRM         14          14      14      14
		SIGTERM         15          15      15      15
		SIGSTKFLT       16          -       -        7
		SIGCHLD         17          20      18      18
		SIGCLD           -          -       18      -
		SIGCONT         18          19      25      26
		SIGSTOP         19          17      23      24
		SIGTSTP         20          18      24      25
		SIGTTIN         21          21      26      27
		SIGTTOU         22          22      27      28
		SIGURG          23          16      21      29
		SIGXCPU         24          24      30      12
		SIGXFSZ         25          25      31      30
		SIGVTALRM       26          26      28      20
		SIGPROF         27          27      29      21
		SIGWINCH        28          28      20      23
		SIGIO           29          23      22      22
		SIGPOLL                                            Same as SIGIO
		SIGPWR          30         29/-     19      19
		SIGINFO          -         29/-     -       -
		SIGLOST          -         -/29     -       -
		SIGSYS          31          12      12      31
		SIGUNUSED       31          -       -       31

.coding:
	đăng ký signal handler
		#include <signal.h>
		void sig_handler(int signum);	//define 1 hàm handler để xử lý signal
			//protype function: void func_name(int)
		typedef typeof(void (int))  *sighandler_t;
		sighandler_t signal(int signum, sighandler_t handler);	//đăng ký hàm handler để xử lý signal
			//signum: signal muốn đăng ký
			//handler: là con trỏ hàm sig_handler() để xử lý signal
			//		   khi có signal truyền đến trùng giá trị signum -> hàm sig_handler() được gọi ra

	send signal
		int kill(pid_t pid, int signum)
		int raise(int signum)		//process tự gửi signal cho chính nó
			//signum: ví dụ SIGKILL
		kill command
		permission to send signal
			//nếu process hiện tại ko có permission để gửi SIGKILL tới process kia -> return FALSE
			//thường các process có cùng UID (sinh ra từ cùng 1 user) có thể gửi signal cho nhau
			//nếu gửi signal tới process khác UID thì tuỳ từng signal (với signal thường, ko phải terminate/stop, có thể vẫn gửi được)
			//để đảm bảo SIGKILL có thể gửi tới mọi process -> nên run chương trình với quyền root "sudo"

.1 số signal cơ bản
	SIGCHILD	//process child kết thúc sẽ gửi signal này cho process parent
	SIGILL		//khi access vùng nhớ ko hợp lệ
	SIGINT		//khi user nhấn tổ hợp phím Ctrl+C
	SIGKILL		//khi gửi signal terminate
	SIGSSEGV	//khi phát sinh lỗi segment fault

.những function an toàn khi gọi trong signal handler
	signal handler có thể được gọi ở bất kỳ thời điểm nào,
	nó break khỏi luồng thực thi của chương trình chính, và có thể gây ra xung đột với chương trình chính
	nếu xung đột dữ liệu giữa signal với chương trình chính -> bug phát sinh random, khó debug và tái hiện
	ví dụ:
		main thread:  call printf("Hello World") -> đã printf được "Hello W"
		signal handler: call printf("abc") -> printf xong "abc" sẽ quay lại printf tiếp "orld" ở main thread
		=> trên màn hình hiện ra "Hello Wabcorld"

		main thread: malloc() 1 vùng nhớ và trả về con trỏ heap ptr1
		signal handler: malloc() 1 vùng nhớ khác và trả về con trỏ heap ptr2
		=> sau khi xong signal handler vẫn ở con trỏ heap ptr2, không quay lại được con trỏ heap ptr1 ở main thread

	signal handler chia sẻ thread với chương trình chính -> ko an toàn
	do đó trong signal handler chỉ nên sử dụng các hàm an toàn trong URL dưới
	https://man7.org/linux/man-pages/man7/signal-safety.7.html

.ignore 1 signal
	ví dụ: khi 1 chương trình đang chạy sẽ ignore Ctrl+C
	sau khi ignore 1 signum: mỗi khi signal đó gửi đến, OS sẽ throw nó đi và ko call hàm signal handler
		sighandler_t signal(int signum, sighandler_t handler);
			//handler:	SIG_IGN: signal sẽ bị ignore, đây là 1 handler NULL ko xử lý gì
			//			SIG_DFL: call handler default của signal

.block 1 signal
	nếu block signal: khi 1 signal gửi đến, hàm signal handler được gửi đến 1 hàng chờ (queue), tạm thời ko xử lý signal đó
	nếu unblock signal: hàm handler signal ở hàng chờ được call ngay sau khi unlock, sẽ xử lý signal đó
		//ignore là vất đi
		//block là signal vẫn còn, chỉ tạm thời ko xử lý

	cơ chế:
		OS cung cấp 1 signal mask (dãy 32 bit 0~1 : 4 bytes) tương ứng với bảng signal table
		để cập nhật dữ liệu signal table:
			(1) khai báo signal mask
			(2) khai báo cách hành xử với signal mask
		ví dụ:
			đăng ký mask ...101101 kèm theo trạng thái block
			khi đó sẽ block với các các signal có index là 0,2,3,5 ...
	
	coding:
		(1)khởi tạo signal mask
		#include <signal.h>
		typedef struct {
			unsigned long sig[_NSIG_WORDS];
		} sigset_t;
		int sigfillset(sigset_t *set);	//initialize and fill a signal set
		int sigemptyset(sigset_t *set);	//initialize and empty a signal set
		int sigaddset(sigset_t *set, int signo);	//add a signal to a signal set
		int sigdelset(sigset_t *set, int signo);	//delete a signal from a signal set
		//sigset_t: struct mô tả signal mask
		//thường khi lập trình, sẽ fill tất cả bit của mask là 0 hoặc là 1
		//		sigemptyset	: fill tất cả bit về 0
		//		sigfillse	: fill tất cả bit về 1
		//		sigaddset	: bật 1 số bit lên 1
		//		sigdelset	: xoá 1 số bit về 0

		(2)update mask để thao tác với bảng signal table
		int sigprocmask(int how, const sigset_t *_Nullable restrict set, sigset_t *_Nullable restrict oldset);
		//examine and change blocked signals
		//		how: SIG_BLOCK | SIG_UNBLOCK | SIG_SETMASK
		//			 SIG_SETMASK: override sigset vào mask signal của OS
		//		set: dùng để lưu sigset mới (sau khi update) của OS, chính là sigset dùng để override mask signal của OS
		//		oldset: dùng để lưu lại sigset cũ (trước khi update) của OS

		(3)check signal đang bị pending
		int sigpending(sigset_t *set);	//examine pending signals
		//1 signal đang bị block thì kernel vẫn gửi signal tới process
		//tuy nhiên signal được bị pending trong 1 hàng đợi và ko được xử lý

	exercise:
		.viết 1 chương trình C để block Ctrl+C từ user
		.viết 1 chương trình C để in ra signal mask của process hiện tại
		.viết 1 chương trình C block SIGINT,
		 sau đó check trong số các signal bị pending có SIGINT hay không
		 nếu có thì thoát chương trình
		.viết 1 process parent A và fork() ra 1 process child B, cùng đọc/ghi 1 file
		 quá trình đọc khi vào file:
		 -đọc biết counter ở đầu file
		 -(+1) cho biến counter
		 -ghi lại vào file 2 giá trị: biến counter mới + PID của process hiện tại
		 -sau khi ghi xong: dùng signal để thông báo cho process còn lại
		 -process còn lại nhận được signal -> mở file lên -> đọc biến counter -> ghi giá trị mới..

#endif	//#1. SIGNAL

//====================================================

#if 1	//#2. SHARED MEMORY

#2. SHARED MEMORY

.OS không quản lý bộ nhớ theo từng byte, mà quản lý theo từng page (có kích thước cố định, ~ 400 kB)
.linux có define struct page, có các trường thông tin
	permission: có bit chứa thông tin quyền đọc/ghi (ví dụ chmod 755)
	flag: có được mở rộng ra không? có thuộc cache mem hay không? có map với 1 file nào không?

.để shared memory giữa 2 process
	shm: old API, support cho tất cả Unix system
	mmap: new API, có thể ghi thông tin (sync) từ shared memory vào file

.shm
	nếu chỉ shared memory thuần tuý, ko cần sync data vào file thì nhanh hơn mmap
	các bước cần làm:
		1-generate ID cho vùng nhớ shared memory
		2-cấp phát (malloc) cho shared memory
		3-đính kèm (mapping) vùng nhớ vừa malloc vào process address space của process của mình
			//thực ra lúc này chỉ malloc vùng nhớ ở VAS của process
			//sau đó mapping vùng nhớ malloc với vùng nhớ shared memory trên kernel
			//shared memory không thuộc sở hữu của 1 process nào, mà thuộc về kernel
			//process nào muốn dùng thì tự malloc 1 vùng nhớ VAS cùng size, rồi mapping với địa chỉ của shared memory của kernel
		4-process đọc/ghi data ở shared memory
		5-process unmap địa chỉ vùng nhớ shared memory khỏi process address space
		6-khi all process ko dùng shared memory, 1 process destroy, giải phóng vùng nhớ shared memory
	coding:
		#include <sys/types.h>
		#include <sys/ipc.h>
		key_t ftok(const char *path, int proj_id);	//generate unique key for shared memory
		#include <sys/shm.h>
		int shmget(key_t key, size_t size, int shmflg);	//allocate a shared memory segment
				//key: đID của shm, được generate từ hàm ftok()
				//size: nên truyền theo bộ số size của 1 page
				//shmflg: chỉ thị thuộc tính/ action với shared memory
				//	IPC_CREAT | IPC_EXCL | SHM_HUGETLB | SHM_HUGE_2MB | SHM_NORESERVE
		void *shmat(int shmid, const void *shmaddr, int shmflg);	//attach/mapping shared memory with address in VAS
				//shmid: ID của shm, được generate từ hàm ftok()
				//shmaddr: address bắt đầu tìm trong VAS, nên để là NULL để nó tự chọn tìm addr còn trống từ đầu
				//shmflg: set lại permision khi attach vào
				//	SHM_RDONLY
				//return: trả về giống con trỏ malloc()
		int shmdt(const void *shmaddr);	//detach/unmapping shared memory with address in VAS
				//shmaddr: con trỏ trả về từ hàm shmat()
		int shmctl(int shmid, int op, struct shmid_ds *buf);	//control shared memory >> destroy
				//shmid: ID của shm, được generate từ hàm ftok()
				//op: sử dụng IPC_RMID để huỷ shm
				//	IPC_STAT | ICP_SET | IPC_RMID | IPC_INFO | SHM_INFO | SHM_STAT | SHM_STAT_ANY | SHM_LOCK | SHM_UNLOCK
				//buf: sau khi huỷ shm, sẽ lưu thông tin vào buffer này

.mmap
	ko generate ID (key) mà sử dụng file fd
	các bước cần làm:
		1-create file fd
		2-set memory size
		3-mapping shared memory with VAS of process
		4-process read/write shared memory
		5-unmapping shared memory with VAS of process
		6-close file fd
	coding:
		#include <sys/mman.h>
		#include <sys/stat.h>
		#include <fcntl.h>
		#include 
		int shm_open(const char *name, int oflag, mode_t mode);	//create/open shared memory object
				//name: tên của shm object, nên đặt theo form [/name]
				//		có thể truyền vào 1 tên file bất kỳ, ko nhất thiết 1 file existing
				//oflag: O_RDONLY | O_RDWR | O_CREAT | O_EXCL | O_TRUNC
				//mode: mask [rwxrwxrwx] giống chong chmod, thường đặt là 0666 (rw-rw-rw-)
		int ftruncate(int fildes, off_t length);
				//nếu shm_open() để mở 1 file ko tồn tại thì cần call ftruncate() để set SIZE cho file fd
				//nếu shm_open() để mở 1 file đã tồn tại thì ko cần call ftruncate()
		void *mmap(void addr[size_t length], size_t length, int prot, int flags, int fd, off_t offset); //mapping files or devices into memory
				//addr: địa chỉ bắt đầu mmap, nên truyền NULL
				//length: độ dài của của vùng mmap, nên là bội số size của page
				//prot: priority của vùng mmap = PROT_EXEC | PROT_READ | PROT_WRITE | PROT_NONE
				//flag: nếu để là MAP_ANONYMOUS, là shared mem thuần tuý, ko cần sync xuống file trong ổ cứng
				//		thì ko check file fd nữa mà tạo 1 vùng nhớ trong ổ cứng rồi mmap vào đó,
				//		có thể truyền file fd là 1 số bất kỳ 2,3,4,5 (tức là file fd ko exist)
				//		nếu file fd ko exist thì ko call hàm close(), nếu call close() sẽ crash process
				//		MAP_SHARED | MAP_SHARED_VALIDATE | MAP_PRIVATE | MAP_32BIT | MAP_ANON | MAP_ANONYMOUS | MAP_DENYWRITE | MAP_EXECUTABLE | MAP_FILE | MAP_FIXED ...
				//fd: con trỏ file đã tạo ở hàm shm_open()
				//offset: địa chỉ file fd muốn bắt đầu mmap
		int munmap(void addr[size_t length], size_t length);	//unmapping files or devices into memory
				//addr: địa chỉ mmap trả về từ hàm mmap()
		int shm_unlink(const char *name);	//remove shared memory object

		const int SIZE = 4096;	//size of one page
		const char* name = "OS";
		int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
		ftruncate(shm_fd, SIZE);
		void* ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
		munmap(ptr, SIZE);
		shm_unlink(name);

	Note:
		nếu shared memory với data lớn (nhiều Gigabytes) -> mmap API nhanh hơn shm API

.exercise
	viết ứng dụng chat client - server trong cùng 1 PC
	sử dụng mmap với file fd log thật (để lưu log chat sau khi tắt shm)
	gợi ý: nên define cố định độ dài mỗi lần ghi vào shared memory

.homework
	viết ứng dụng giả lập mô hình client - server
	giữa 2 chương trình chia sẻ chung 1 vùng nhớ (vùng nhớ shm được tạo ra bởi server)
	sau khi server init sẽ khởi tạo shm, rồi sleep luôn (đa số thời gian là sleep)
	vùng nhớ shm lưu data được define theo struct sau:
		struct data {
			char name[25];
			int age;
		}
	client chạy lên, yêu cầu user nhập tên + tuổi, rồi lưu vào struct data trên shm
	client request server wake-up, và đọc data trong shm, rồi ghi vào 1 file log
	(cách client request server wake-up là cơ chế signal)
	client và server được viết trên 2 file source C riêng, ko sử dụng fork()

.Bổ sung:
	tìm hiểu thêm về cơ chế lock/unlock để sync data với shared memory

#endif	//#2. SHARED MEMORY

//====================================================

#if 1	//#3. SOCKET

#3. SOCKET
.socket: cửa ngõ của hệ thống có thể giao tiếp với bên ngoài ở internet

.mạng máy tính
địa chỉ IP
	IPv4
		dãy số 32 bit, dùng 4 byte để đánh địa chỉ, tối đa 4 tỷ địa chỉ IP
	dùng để định danh các máy tính trong mạng internet
	ở đầu mỗi bản tin được truyền trong internet đều gắn địa chỉ IP
Global IP và Local IP
	vì tổng thiết bị PC trên internet đã nhiều hơn 4 tỷ >> cần gom nhóm IP
	với mỗi mạng LAN cục bộ:
		các thiết bị có chung 1 global IP với IP của wifi modern
		cục rounter sẽ chia tiếp local IP cho các thiết bị trong mạng LAN
	(IPv6, dùng 6 byte để đánh IP, hiện nay vẫn chưa dùng phổ biển)
Port
	trên 1 PC có nhiều ứng dụng cùng lúc connect ra internet
	mỗi 1 ứng dụng đăng ký với OS 1 port là 1 số nguyên ko âm để định danh
	trong bản tin gửi tới PC, ngoài IP còn có số Port Number >> biết được gói tin gửi tới ứng dụng nào

Client & Server
	là cơ chế thiết lập kênh truyền giữa các thiết bị trong internet
	Client:
		phải biết IP của server
		phải chủ động connect đến server
		phải gửi bản tin yêu cầu kết nối tới server
	Server:
		ko quan tâm IP của Client
		chỉ chủ động lắng ngăn
		khi nhận bản tin yêu cầu kết nối từ client, có thể chấp nhận hoặc từ chối
		- chấp nhận: sẽ thiết lập kênh truyền, bản tin được gửi trong kênh truyền đó
		- từ chối: ko thiết lập kênh truyền

.Socket overview
	socket là các file ở dạng endpoint, khi ghi data vào 1 đầu thì data sẽ được gửi sang 1 hoặc nhiều đầu khác
	khác với các file thông thường:
		- socket cho phép data được gửi ra khỏi máy đi vào mạng internet
		- là file point-to-point, data khi đi vào 1 socket ở đầu đường ống >> data sẽ đi ra ở 1 socket ở cuối đường ống

.Coding
	1-Create a soket
		#include <sys/socket.h>
		int socket(int domain, int type, int protocol)	//create an endpoint for communication
				//domain: chỉ định phương thức giao tiếp là ghi data ra ngoài, hoặc chỉ trung chuyển trong hệ thống?
				//		AF_UNIX		Local communication
				//		AF_INET		IPv4 Internet protocols
				//		AF_CAN		Controller Area Network automotive bus protocol
				//type: định nghĩa cách truyền dữ liệu
				//		SOCK_STREAM	| SOCK_DGRAM | SOCK_SEQPACKET | SOCK_RAW | SOCK_RDM | SOCK_PACKET | SOCK_NONBLOCK | SOCK_CLOEXEC
				//protocol: định nghĩa chuẩn giao tiếp
				//return: trả về file fd đại diện cho socket

	2-Address bytes ordering
		big endian và little endian: chỉ định cách sắp xếp byte trong bộ nhớ
			1 số 4 byte:
				0x12345678
			big endian: byte lớn ưu tiên sắp xếp trước, thứ tự byte trong memory giống như khi ta viết
				0x00: 12
				0x01: 34
				0x02: 56
				0x03: 78
				//[12][34][56][78]
			little endian: byte nhỏ ưu tiên sắp xếp trước, thứ tự byte trong memory ngược với khi ta viết
				0x00: 78
				0x01: 56
				0x02: 34
				0x03: 12
				//[78][56][34][12]
			có khác biệt 1 số dòng CHIP, big endian (network, PowerPC) và little endian (intel, amd, arm)

		cần 1 chuẩn chung trong network để tất cả có thể hiểu được
			trong network chỉ dùng chuẩn big endian,
			mọi bản tin phải convert theo chuẩn big endian khi truyền vào network
		
		các hàm convert order của data từ máy tính sang network
			#include <arpa/inet.h>
			uint32_t htonl(uint32_t hostlong);
			uint16_t htons(uint16_t hostshort);
			uint32_t ntohl(uint32_t netlong);
			uint16_t ntohs(uint16_t netshort);

	3-Address format
			#include <sys/socket.h>
			struct sockaddr {
				sa_family_t		sa_family;		/* Address family */
				char			sa_data[];		/* Socket address */
			};
			#include <netinet/in.h>
			struct sockaddr_in {
				sa_family_t		sin_family;		/* AF_INET */
				in_port_t		sin_port;		/* Port number */
				struct in_addr	sin_addr;		/* IPv4 address */
			};

	4-Gán địa chỉ cho socket
		sau khi server gán địa chỉ cho socket, mỗi khi ghi data xuống socket file,
		data sẽ đóng gói kèm địa chỉ và được phân luồng để gửi đến máy đích
			#include <sys/socket.h>
			int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);	//bind a name to a socket

	5-Thiết lập kết nối
			#include <sys/socket.h>
			//CLIENT
			int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);	//initiate a connection on a socket
					//addr: địa chỉ server
					//gửi bản tin xin được kết nối tới server
			#include <unistd.h>
			size_t read(int fd, void buf[size_t count], size_t count);	//ead from a file descriptor
			size_t write(int fd, const void buf[size_t count], size_t count);	//write to a file descriptor
			int close(int fd);		//close a file descriptor
					//sau khi kết nối, client gửi nhận data tới server bằng hàm read/write
					//client gửi nhận trực tiếp vào socket yêu cầu kết nối tới server
					//client ko cần tạo thêm socket kết nối point-to-point như server
					//nếu ko muốn kết nối với server nữa, client call hàm close()
			//SERVER
			int listen(int sockfd, int backlog);	//listen for connections on a socket
					//tạo ra 1 hàng đợi, số ô trong hàng đợi bằng số backlog
					//mỗi bản tin từ client được đẩy vào 1 ô trong hàng đợi
			int accept(int sockfd, struct sockaddr *_Nullable restrict addr, socklen_t *_Nullable restrict addrlen);	//accept a connection on a socket
					//server sẽ quyết định đồng ý kết nối với client trong hàng đợi hay ko
					//nếu đồng ý sẽ tạo socket mới để point-to-point với client
					//(sockfd chỉ là socket được tao ra ban đầu để server lắng nghe các bản tin kết nối)
			#include <unistd.h>
			size_t read(int fd, void buf[size_t count], size_t count);	//ead from a file descriptor
			size_t write(int fd, const void buf[size_t count], size_t count);	//write to a file descriptor
			int close(int fd);		//close a file descriptor
					//sau khi accept, server có thể gửi nhận data vào client qua hàm read/write
					//nếu ko muốn connect với client nữa >> call hàm close()
					//server có 2 loại socket
					//(1) listenning socket: chỉ cần 1 socket
					//(2) connection socket: cần [backlog] socket để kết nối point-to-point tới [backlog] client
	
			<< SERVER >>		|		<< CLIENT >>
			socket()			|		socket()
			init sockaddr_in	|		init sockaddr_in
			bind()				|		connect()
			listen()			|		-
			accept()			|		-
			-	-	-	-	<< COMMON >>	-	-	-	-
								send()
								read()
								write()
								close()

.Exercise
	ở linux có card mạng ảo, có look-back interface:
		inet addr: 127.0.0.1
		Mask: 255.0.0.0
	tất cả bản tin gửi tới look-back interface >> bản tin gửi ngược lại máy tính
	có thể viết 2 ứng dụng client-server chat bản tin cho nhau qua card mạng look-back này
	(khuyến cáo: nên viết 2 ứng dựng client-server chat với nhau từ 2 máy ảo ở 2 PC khác nhau)

.Homework
	1-Viết chương trình chat theo mô hình client-server, cả 2 chạy trên cùng 1 PC
	2-Viết chương trình chat client-server cho phép chat trong mạng LAN
	3-Viết chương trình cho phép gửi nhận file từ nhà và công ty

#endif	//#2. SHARED MEMORY

//====================================================