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

	FILE* f = fopen("/dev/sp6", "rw");

	// Read initial value
	ret = fread(buffer, sizeof(buffer), 1, f);
	printf("Read \"%s\" (%d bytes)\n", buffer, ret);

	// modify the kernel value
	fseek(f, 0, SEEK_SET);
	sprintf(buffer, "Hello kernel!");
	ret = fwrite(buffer, strlen(buffer), 1, f);
	printf("Write \"%s\" (%d bytes)\n", buffer, ret);

	// Read modified value
	fseek(f, 0, SEEK_SET);
	ret = fread(buffer, 1, sizeof(buffer), f);
	printf("Read \"%s\" (%d bytes)\n", buffer, ret);
}
