#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

int calcExp(int b, int e);

int convertToInt(char *str) {
	int i;
	char c;
	int digit;
	int res = 0;
	int incCounter = 0;
	for (i = 0; str[i] != '\0'; i++) {
		incCounter++;
	}
	i = 0;
	while (incCounter > 0) {
		c = str[i];
		switch (c) {
			case '0':
				digit = 0;
				break;
			case '1':
				digit = 1;
				break;
			case '2':
				digit = 2;
				break;
			case '3':
				digit = 3;
				break;
			case '4':
				digit = 4;
				break;
			case '5':
				digit = 5;
				break;
			case '6':
				digit = 6;
				break;
			case '7':
				digit = 7;
				break;
			case '8':
				digit = 8;
				break;
			case '9':
				digit = 9;
				break;
			default:
				printf("An issue occurred while converting digits.");
		}
		if (incCounter == 1) {
		    res += digit;
		} else {
		    res += calcExp(10, incCounter-1) * digit;
		}
		incCounter--;
		i++;
	}
	return res;
}

int calcExp(int b, int e) {
	int x = 1;
	for (int i = 0; i < e; i++) {
		x *= b;
	}
	return x;
}

int main(int argc, char **argv) {
	int fd;
	char *buff;
	char *fileName;
	bool passedFile = false;
	int n = (int) pow(2, 64) - 1;
	int lines;
	int i;
	int linesRead;
	
	buff = (char *) malloc(n);
	
	if (argc == 4 && strcmp(argv[2], "-n") == 0) {
		lines = convertToInt(argv[3]);
		fileName = argv[1];
		passedFile = true;
	} else if (argc == 4 && strcmp(argv[1], "-n") == 0) {
		lines = convertToInt(argv[2]);
		fileName = argv[3];
		passedFile = true;
	} else if (argc == 2) {
		lines = 10;
		passedFile = true;
		fileName = argv[1];
	} else {
		lines = 10;
	}
	
	size_t endPoint;
	if (passedFile) {
		fd = open(fileName, O_RDONLY);
		if (fd == -1) {
			printf("Failed to open file.\n");
			exit(1);
		}
		endPoint = read(fd, buff, n);
		buff[endPoint] = '\0';
		close(fd);
	} else {
		endPoint = read(0, buff, n);
		buff[endPoint] = '\0';
	}
	
	linesRead = 0;
	char* output = (char *) malloc(endPoint);
	for (i = 0; i < endPoint; i++) {
		if (buff[i] == '\0') {
			break;
		}
		if (linesRead >= lines) {
			break;
		}
		if (buff[i] == '\n') {
			linesRead++;
			
		}
			output[i] = buff[i];
	}
	free(buff);
	printf("%s\n", output);
	free(output);
	
	return 0;
	
}