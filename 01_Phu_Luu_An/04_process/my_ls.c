//program: my_ls
//if run "./my_ls", display list of files
//if run "./my_ls -l", display list of files with user name, file size
//keyword:
//		.how to print file size in C linux
//		.how to print modified time of file in C linux
//url: https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
//url: https://stackoverflow.com/questions/10446526/get-last-modified-time-of-file-in-linux

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>

int main(int argc, char *argv[])
{
	DIR *dir;
	struct dirent *ent;
	struct stat st;
	struct tm *utc_time;

	if ((dir = opendir(".")) == NULL) {
		perror("Failed to open current directory.\n");
		return EXIT_FAILURE;
	}

	while ((ent = readdir(dir)) != NULL) {
		stat(ent->d_name,&st);
		utc_time = gmtime((time_t *)&st.st_mtime);

		printf( "%04d:%02d:%02d %02d:%02d:%02d\t%s\t%ld\t%s\n",
				utc_time->tm_year + 1900,
				utc_time->tm_mon,
				utc_time->tm_mday,
				utc_time->tm_hour,
				utc_time->tm_min,
				utc_time->tm_sec,
				getpwuid(st.st_uid)->pw_name,
				st.st_size,
				ent->d_name
			);
	}

	closedir(dir);
	return EXIT_SUCCESS;
}