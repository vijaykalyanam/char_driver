#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argp)
{
	int fd;

	if (argc != 2) {
		printf("Usage : send_cmd cmd < possitive integer >\n");
		return -1;
	}
	printf("User Command :%d\n", atoi(argp[1]));
	fd = open("/dev/saanvika", O_RDWR);
	printf("fd : %d\n", fd);

	if (fd != -1) {
	printf("Ioctl :%d\n", ioctl(fd, atoi(argp[1]), NULL));
	}
	close(fd);
	return 0;
}
