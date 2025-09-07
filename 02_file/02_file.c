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

#2. Ứng dụng
.viết app đọc ghi file để test driver phần cứng
.viết app đọc ghi file quản lý process, device

#3. Phân Loại
.file thật:
   có vùng lưu trữ thật trên ổ cứng, tắt máy đi vẫn còn
.file ảo:
   được tạo ra khi OS running, lưu ở RAM, tắt máy đi sẽ mất

#4. Các Hàm Đọc/Ghi File
.int open(const char* pathname,int flags)
.int close(int fd)
.size_t read(int fd,void *buf, size_t count)
.size_t write(int fd,void *buf, size_t count)
.off_t lseek(int fd,off_t offset,int whence)
.void sync(void)

Example:
open()
    O_APPEND:   khi mở file, data được ghi vào cuối file, không ghi đè/xoá file cũ
    O_CREATE:   khi mở file đã có, xoá file cũ, tạo file mới để ghi data mới
fflush()
    đẩy dữ liệu vào bộ nhớ đệm (thường là RAM)
sync()
    sync data ở RAM và thiết bị ngoại vi