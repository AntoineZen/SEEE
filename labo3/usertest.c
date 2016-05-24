#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

char buffer[80];

int main(int argc, char **argv)
{
	int ret;

	int f = open("/dev/sp6", O_RDWR);

	// Read initial value
	ret = read(f, buffer, sizeof(buffer));
	printf("Read \"%s\" (%d bytes)\n", buffer, ret);

	// modify the kernel value
	lseek(f, 0, SEEK_SET);
	sprintf(buffer, "Hello kernel!");
	ret = write(f, buffer, strlen(buffer));
	printf("Write \"%s\" (%d bytes)\n", buffer, ret);

	// Read modified value
	lseek(f, 0, SEEK_SET);
	ret = read(f, buffer, sizeof(buffer));
	printf("Read \"%s\" (%d bytes)\n", buffer, ret);

	close(f);
}
