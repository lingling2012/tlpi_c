#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char const *argv[]) {
	int inputFd, outputFd, openFlags;
	mode_t filePerms;
	ssize_t numReads;
	char buf[BUF_SIZE];

	if (3 != argc || strcmp(argv[1], "--help") == 0) {
		printf("%s old-file new-file\n", argv[0]);
	}

	inputFd = open(argv[1], O_RDONLY);
	if (inputFd == -1) {
		printf("opening file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	filePerms = S_IRUSR | S_IWUSR  | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; // rw-rw-rw-
	outputFd = open(argv[2], openFlags, filePerms);
	if (outputFd == -1) {
		printf("opening file %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	while((numReads = read(inputFd, buf, BUF_SIZE)) > 0) {
		if (write(outputFd, buf, numReads) != numReads) {
			printf("couldn't write whole buffer\n");
			exit(EXIT_FAILURE);
		}
	}

	if (numReads == -1) {
		exit(EXIT_FAILURE);
	}

	if (close(inputFd) == -1) {
		exit(EXIT_FAILURE);
	}
	if (close(outputFd) == -1) {
		exit(EXIT_FAILURE);
	}

	return 0;
}