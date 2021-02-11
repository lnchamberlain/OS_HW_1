#include <math.h>

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
		    res += pow(10, incCounter-1) * digit;
		}
		incCounter--;
		i++;
	}
	return res;
}