#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[]){
	printf("This is C, i have received : %ld", strlen(argv[1]));
	FILE *fp;
	fp = (argv[1],"r");
	
	
	
	return 0;
}