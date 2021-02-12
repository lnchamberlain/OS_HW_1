#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

int calcExp(int a, int b);

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

long getGap(char* cstr, int start) {
	int i;
	long sizeGap = 0;
	for (i = start; cstr[i] != '\0'; i++) {
		sizeGap++;
	}
	return sizeGap;
}

int* shiftArray(int *arr, int x, int len) {
	int i;
	for (i = 1; i < len; i++) {
		arr[i-1] = arr[i];
	}
	arr[len-1] = x;
	return arr;
}

char* copyChunk(char *source, char *dest, int start) {
	int i;
	int destIndex = 0;
	for (i = start; source[i] != '\0'; i++) {
		dest[destIndex] = source[i];
		destIndex++;
	}
	return dest;
}

int main(int argc, char **argv) {
	int fd;
	char *fileName;
	bool passedFile = false;
	int n = 10000;
	int lines;
	int i;
	int linesRead;
	
	char buff[n];
	
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
	
	if (passedFile) {
		fd = open(fileName, O_RDONLY);
		if (fd == -1) {
			printf("An error occured while trying to open the file.\n");
			exit(1);
		}
		linesRead = 0;
		int charsRead = 0;
		read(fd, buff, n);
		int ex = 0;
		int linePoint = 0;
		int lastLines[lines];
		int lineIndex = 0;
		for (i = 0; buff[i] != '\0'; i++) {
			if (buff[i] == '\n') {
				linesRead++;
				if (lineIndex != lines) {
					lastLines[lineIndex] = i;
					lineIndex++;
				}
				else {
					shiftArray(lastLines, i, lines);
				}
			}
		}
		if (linesRead > lines) {
			long outSize = getGap(buff, lastLines[0]);
			char *output = (char *) malloc(outSize);
			copyChunk(buff, output, lastLines[0]);
			printf("%s\n", output);
			free(output);
		}
		else {
			printf("%s\n", buff);
		}
	}
}