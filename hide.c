#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INIT_FILE_SIZE 100
#define ASCII_CHARS  97
#define PASSWORD_MAX 20
#define ASCII_FILE_LOCATION "E:/Programming/c/InvisibleDoc/Resources/ascii.txt"

/*
typedef struct {
	FILE *file;
	char *file_vals;
	char char_selection[ASCII_CHARS]; //this is a fixed size variable
	char possible_chars[]; //has to be this because there is no way of knowing what chars where in the file once it is hidden
} input_file_t


I think it would be beneficial to include a struct for the file variables:
	-FILE *file
	-*file_vals
	-invis_vals
	-chars_included (array of the characters included in the file)
	-invis_codes (array of the combinations of whitespace characters that we will map the chars_included to)
*/	


FILE *get_file(char *argv[]);
char *read_file(FILE *f);
int get_password();
void hide(FILE *file, FILE *ascii_file);
void recover(FILE *file, FILE *ascii_file);
char *get_ascii_arrray(FILE *ascii_file);
char *get_mapping_array(int password, char *asciis);

int string_to_int(char string[]);
int power(int base, int power);



int
main(int argc, char *argv[]){
	char *file_vals, *asciis, mapping;
	FILE *ascii_file, *file;

	file = get_file(argv);
	if(file == NULL) exit(EXIT_FAILURE);
	file_vals = read_file(file);
	fclose(file);
	
	file = fopen(argv[1], "w");
	if(file == NULL){
		printf("\nError. Couldn't modify file.\n");
		exit(EXIT_FAILURE);
	}
	ascii_file = fopen(ASCII_FILE_LOCATION, "r");
	if(file == NULL){
		printf("\nError. Couldn't open resource file, ascii.txt.\n");
		exit(EXIT_FAILURE);
	}

	asciis = get_ascii_arrray(ascii_file);
	// mapping = get_mapping_array(password, asciis);

	printf("%s\n", file_vals);
	srand(get_password());


	
	if(argv[2][0] == 'r'){
		//recover(file, ascii_file);
	} else {
		hide(file, ascii_file);
	}
	
	free(file_vals);
	fclose(file);
	fclose(ascii_file);
	return 0;
}

//**************************************************************************
// Functions

FILE 
*get_file(char *argv[]) {
	FILE *temp_file;
	if(argv[1] == NULL || argv[2] == NULL){
		printf("\nError. Missing inputs.\n");
		printf("Format is as follows:	");
		printf("\'hide *file location* *action*\'\n");
		printf("The \"action\" parameter can be \'h\'' for hiding file content or \'r\'' for recovering file content.\n");
		return NULL;
	}
	temp_file = fopen(argv[1], "w");
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
	int i;
	char c, pass[PASSWORD_MAX];
	while(1) {
		printf("Input password number: ");
		scanf("%s", pass);
		i=0;
		while((c=pass[i++])!='\0') {
			if(isalpha(c)) break;
		}
		if(c=='\0') break;
		printf("\nPassword must be an integer.\n");
	}
	return string_to_int(pass);
}

int 
string_to_int(char string[]) {
	int i, str_len, int_val=0;

	str_len = strlen(string)-1;
	for(i=0; i<=str_len; i++) {
		int_val += (string[i]-48)*power(10,str_len-i);
	}
	return int_val;
}

int 
power(int base, int power) {
	int i, squared=1;
	for(i=0; i<power; i++){
		squared *= base;
	}
	return squared;
}

void 
hide(FILE *file, FILE *ascii_file){

	return;
}

void 
recover(FILE *file, FILE *ascii_file){
	
	return;
}

char 
*get_ascii_arrray(FILE *ascii_file){
	int i=0;
	char *asciis, c;
	asciis = malloc(sizeof(char)*ASCII_CHARS);
	
	while((c=fgetc(ascii_file)) != EOF){
		asciis[i++] = c;
	}
	return asciis;
}

// char 
// *get_mapping_array(int password, char *asciis){
// 	int i;
// 	char *mapping, map_to;
// 	mapping = malloc(sizeof(char)*ASCII_CHARS);

// 	for(i=0; i<ASCII_CHARS; i++){
// 		map_to = asciis[(rand()%ASCII_CHARS)-32];
// 	}
// 	return mapping;
// }