#include <unistd.h>
#include <errno.h>
#include<string.h>
int main(int argc, char *argv[]){
  /* prefix is an array that will hold the given prefix, declare and clear prefix
  for(i = 0; i < 6; i++){
    prefix[i] = " ";
  }
  
  char words[] = "Hello there this is using write";

  write(1, words, sizeof(words));
  */
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
  int lenOfInput = 0;
  int i = 0;
  while(argv[1][i] != "\0"){
    lenOfInput++;
    i++;
  }
  
    /*Move from arguments into array*/
    if(argc == 2){
      /* prefix is stored in argv[1] */
      /*
      if(sizeof(argv[1]) != 6){
	char not6DigitsErrorMessage[] = "Input not 6 digts\n";
	write(1, not6DigitsErrorMessage, sizeof(not6DigitsErrorMessage));
	return 1;
      }
      */
      int isAllDigits = 1;
      int i;
      for(i = 0; i < lenOfInput; i++){
	if((argv[1][i] == 0) || (argv[1][i] == 1) || (argv[1][i] == 2) || (argv[1][i] == 3) || (argv[1][i] == 4) || (argv[1][i] == 5) || (argv[1][i] == 6) || (argv[1][i] == 7)  || (argv[1][i] == 8) || (argv[1][i] == 9)){
	  isAllDigits = 0;
	}else{
	  isAllDigits = 1;
	  break;
	}
	write(1, &i, sizeof(i));
      }
      if(isAllDigits == 0){
	char notAllDigitsErrorMessage[] = "Input not comprised of digits 0 - 9\n";
        write(1, notAllDigitsErrorMessage, sizeof(notAllDigitsErrorMessage));
        return 1;
      }
    }


  return 0;
}
