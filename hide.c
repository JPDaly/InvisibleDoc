#include <stdio.h>
#include <stdlib.h>

#define INIT_FILE_SIZE 100
//Note sure if all of these are going to work in a .txt file
#define HORI_TAB '\t'
#define SPACE ' '
#define NEWLINE '\n'
#define VERTI_TAB '\v'
#define FEED '\f'
#define CARRIAGE_RETURN '\r'

/*
I think it would be beneficial to include a struct for the file variables eg:
	-FILE *file
	-file_vals
	-invis_vals
	-chars_included (array of the characters included in the file)
	-invis_codes (array of the combinations of whitespace characters that we will map the chars_included to)
*/	


FILE *get_file(char *argv[]);
char *read_file(FILE *f);
void hide(FILE *file, char *invis_vals);
void recover(FILE *file);
int get_password();


int
main(int argc, char *argv[]){
	char *file_vals;
	FILE *file = get_file(argv);
	//only added these becuase i'm pretty sure they will work
	char invis_vals = {HORI_TAB, NEWLINE, SPACE, VERTI_TAB};
	
	if(file == NULL) exit(EXIT_FAILURE);
	file_vals = read_file(file);
	
	srand(get_password());
	
	if(argv[2][0] == 'r'){
		recover(file);
	} else {
		hide(file, invis_vals);
	}
	
	free(file_vals);
	fclose(file);
	return 0;
}

// Functions

FILE 
*get_file(char *argv[]) {
	FILE *temp_file;
	if(argv[1] == NULL || argv[2] == NULL){
		printf("\nError. Missing inputs.\n");
		printf("Format is as follows:	");
		printf("\'secret *file location* *action*\'\n");
		printf("The \"action\" parameter can be \'h\'' for hiding file content or \'r\'' for recovering file content.\n");
		return NULL;
	}
	temp_file = fopen(argv[1], "r");
	if(temp_file == NULL){
		printf("\nError. Invalid file input.\n");
		printf("Make sure to include the entire file directory.\n");
		return NULL;
	} else if(argv[2][0] != 'h' && argv[2][0] != 'r'){
		printf("\nError. Invalid action.\n");
		printf("The \"action\" parameter can be \'h\'' for hiding file content or \'r\'' for recovering file content.\n");
		return NULL;
	}
	return temp_file;
}

char 
*read_file(FILE *file) {
	int i=0, vals_size=INIT_FILE_SIZE;
	char c, *vals;
	vals = malloc(sizeof(char)*vals_size);
	if(vals == NULL){
		printf("Malloc Error.\n");
		exit(EXIT_FAILURE);
	}
	while((c = fgetc(file)) != EOF) {
		vals[i++] = c;
		if(i==vals_size) {
			vals_size *= 2;
			vals = realloc(vals, sizeof(char)*vals_size);
		}
	}
	vals[i] = '\0';
	return vals; 
}

int
get_password(){
	int pass = -1;
	while(pass == -1) {
		//I'm not sure what this does if there is a non int input
		//Needs to include some incorrect input checking
		//Not even sure scanf is the right function
		printf("Input password: ");
		scanf("%d\n", &pass);
	}
	return pass;
}

void 
hide(FILE *file, char *invis_vals){
	printf("%d\n", rand());
	return;
}






