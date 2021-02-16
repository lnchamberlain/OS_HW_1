#include <unistd.h>
#include <errno.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <stdio.h>
/*Would like to use this but can't figure out how..*/
/*TODO change the file size to (size_t) lseek(SEEK_END)*/
/*TODO change error messages to assert strerror and fprintf*/
/*TODO Change num characters per line to be aquired using lseek as well*/
int returnDigit(char ch);
int checkValue(char mappedFile[], int toCompare, int offsetIndex);
int binarySearchOfNanpaArray(char mappedFile[], int startIndex, int endIndex, int numCharsPerLine, int target);
int getNumFromString(char str[]);


const size_t MAX_FILE_SIZE = 2147483648;
const char PATH_NAME[] = "nanpa";

int main(int argc, char *argv[]){
  int fd;
  int prefixValue;
    
  /*Handle case too many arguments passed*/
  if(argc > 2){
    char tooManyArgumentsErrorMessage[] = "Enter 1 prefix\n";
    write(1, tooManyArgumentsErrorMessage, sizeof(tooManyArgumentsErrorMessage));
    return 1;
  }
  /*Handle case no argument passed*/
  if( argc < 2){
    char tooFewArgumentsErrorMessage[] = "No prefix entered\n";
    write(1, tooFewArgumentsErrorMessage, sizeof(tooFewArgumentsErrorMessage));
    return 1;
  }
  
    /*Move from arguments into array*/
   if(argc == 2){
      /* prefix is stored in argv[1] */
      const char *prefix = argv[1];
      prefixValue = getNumFromString(prefix);
     
      /*Handle case: not 6 digit input*/
      if(prefixValue == 0){
	char not6DigitsErrorMessage[] = "Input not 6 digts\n";
	write(1, not6DigitsErrorMessage, sizeof(not6DigitsErrorMessage));
	return 1;
      }
      
      /*Handle case: not all digits*/
      if(prefixValue < 0){
	char notAllDigitsErrorMessage[] = "Input not comprised of digits 0 - 9\n";
        write(1, notAllDigitsErrorMessage, sizeof(notAllDigitsErrorMessage));
        return 1;
      }    

      /*Open Nanpa.txt */
     
      fd = open(PATH_NAME, O_RDONLY, 0);
      int offset = lseek(fd, 0, SEEK_CUR);
      if(fd == -1){
	char fileOpenFailedMessage[] = "File failed to open\n";
	write(1, fileOpenFailedMessage, sizeof(fileOpenFailedMessage));
	return 1;
      }
      if(fd >= 0){
	char fileOpenSuccess[] = "File opened successfully\n";
	write(1, fileOpenSuccess, sizeof(fileOpenSuccess));
      }
    }

    /*Map to memory using mmap*/
   /*TODO change the file size to (size_t) lseek(SEEK_END)*/
  size_t fileLength = lseek(fd, (size_t)0, SEEK_END);
  char *mappedFile = mmap(NULL, fileLength, PROT_READ, MAP_PRIVATE, fd, 0);
    if(mappedFile == MAP_FAILED){
      char mappingFailureMessage[] = "Mapping failed\n";
      write(1, mappingFailureMessage, sizeof(mappingFailureMessage));
      return 1;
    }
    else{
      char mappingSuccessMessage[] = "File Mapped Successfully\n";
      write(1, mappingSuccessMessage, sizeof(mappingSuccessMessage));

    }
    int numCharsPerLine;
    int i= 0;
    /*Find the number of lines in file by counting '\n' + 1 */
    while(mappedFile[i] != '\n'){
      i++;
    }
    
    numCharsPerLine = i + 1;
    int numEntries = (int)fileLength / numCharsPerLine;   
    int entryNumber;
 entryNumber = binarySearchOfNanpaArray(mappedFile,0, numEntries-1, numCharsPerLine, prefixValue);
 /*entryNumber is equal to the line number of the prefix, the first letter can be read using mappedFile[entryNumber * numCharsPerLine], entryNumber is set to -1 if the number is not found */
 if(entryNumber == -1){
   char notFoundInNanpa[] = "Prefix not found in Nanpa\n";
   write(1, notFoundInNanpa, sizeof(notFoundInNanpa));
   return 1;
 }
 else{
   i = 0;
   int toBreak = 0;
   int startIndexOfCity = (entryNumber * 32) + 6;
   printf("Char = %c \n", mappedFile[startIndexOfCity]);
   /*while(1){
     if(mappedFile[startIndexOfCity + i] == " "){
       if(mappedFile[startIndexOfCity+ i +1] == " "){
	 toBreak = -1;
       }
     }
     printf("char = %c \n", mappedFile[startIndexOfCity + i]);
     if(toBreak < 0){
       break;
     }
     write(1, mappedFile[i+startIndexOfCity], sizeof(mappedFile[i+startIndexOfCity]));
     i++;
     }
   */
   char currChar;
   int endIndexOfCity;
   for( i = 0; i < numCharsPerLine; i++){
     if(mappedFile[startIndexOfCity + i] == ' '){
       if(mappedFile[startIndexOfCity+ i +1] == ' '){
	 endIndexOfCity = i;
       }
     }
    }
   printf("endIndex = %d\n", endIndexOfCity);
   for(i = 0; i < endIndexOfCity; i++){
     currChar = mappedFile[i + startIndexOfCity];
     printf("%c", currChar);
     write(1, currChar, sizeof(currChar));
   }
 }
 /*unmap file from memory */
 if(munmap(mappedFile, fileLength) < 0){
   char unmapFailedMessage[] = "Unmap failed \n";
   write(1, unmapFailedMessage, sizeof(unmapFailedMessage));
 }

 /*Close the file descriptor*/
 if( close(fd) < 0){
   char closeError[] = "Error closing file \n";
   write(1, closeError, sizeof(closeError));
   return 1;
 }
 return 0;
}

/*checkValue takes in a pointer to a line in the mappedByLine array,
slices out the phone code at indicies 1-7 (ea ch entry begins with a 0
due to the null terminator), then casts to integers and compares the
inputted prefix. If\ the inputted prefix (denoted toCompare) is larger
than the area code, 1 is returned. If in the prefix matches the area
code, 0 is returned. If prefix is smaller than area code, -1 is
returned. */
int checkValue(char mappedFile[], int toCompare, int offsetIndex){
  char slice[6];
  for(int i = 0; i < 6; i++){
    slice[i] = "0";
   }
 
  slice[6] = NULL;
 
   for(int i = 0; i < 6; i++){
    slice[i] = mappedFile[offsetIndex + i];
  }
  int num;
 
  num = getNumFromString(slice);
   if(num > toCompare){
    return -1;
  }
  if(num < toCompare){
    return 1;
  }
  if(num == toCompare){
    return 0;
  }

}
/*Implemenation of binary search on nanpa array, accepts the nanpa file stored in an array by line and uses helpe\
 function checkVaule to compare midpoint area code values until the target is found. Returns the line index of the
 target, -1 if not in the array. */
int binarySearchOfNanpaArray(char mappedFile[], int startIndex, int endIndex, int numCharsPerLine, int target){
  int midpoint, result;
  while(startIndex <= endIndex){
    midpoint = (startIndex + (endIndex - startIndex) / 2);
     result = checkValue(mappedFile, target, midpoint * numCharsPerLine);
    if(result == 0){
      return midpoint;
    }
    if(result == -1){
      endIndex = midpoint - 1;
    }
    if(result == 1){
      startIndex = midpoint + 1;
    }
      }
    return -1;
}

/*getNumFromString accepts a string and then returns the integer value. Will return 0 if input is not of size 6 and will return a negative number if the input was not entirely digits 0-9.*/
int getNumFromString(char str[]){
  int integerValue = 0;

  /*Get the length of the string as sizeof(char*) will return the size of the p\
ointer, not the array*/
  int strLength = 0;
  while(str[strLength] != NULL){
    strLength++;
      }
    /*Handle case where input is not of the correct size*/
  if(strLength != 6){
     return 0;
  }
  else{
    integerValue += returnDigit(str[0]) * 100000;
    integerValue += returnDigit(str[1]) * 10000;
    integerValue += returnDigit(str[2]) * 1000;
    integerValue += returnDigit(str[3]) * 100;
    integerValue += returnDigit(str[4]) * 10;
    integerValue += returnDigit(str[5]);

  }
     return integerValue;
}

/*returnDigit is a helper function to getNumFromStr that accepts a character and returns the digit value. In the case where the character is not a digit 0-9, a negative number greater than the prefix is returned so that if the integerValue is negative, the input was not exclusively digits*/
int returnDigit(char ch){
  int digitVal;
  int chVal = (int) ch;
  switch(chVal){
  case 48:
    digitVal = 0;
    break;
  case 49:
    digitVal = 1;
    break;
  case 50:
    digitVal = 2;
    break;
  case 51:
    digitVal = 3;
    break;
  case 52:
    digitVal = 4;
    break;
  case 53:
    digitVal = 5;
    break;
  case 54:
    digitVal = 6;
    break;
  case 55:
    digitVal = 7;
    break;
  case 56:
    digitVal = 8;
    break;
  case 57:
    digitVal = 9;
    break;
  default:
    /*If any characters are not digits, returning a negative value larger than the most significant digit will ensure than the return value of get numFromStr is negative if the input is not exclusively*/
    digitVal = -1000000;

  }
  return  digitVal;

}

