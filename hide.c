#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INIT_FILE_SIZE 100
#define ASCII_CHARS  97
#define PASSWORD_MAX 20
#define ASCII_FILE_LOCATION "Resources/ascii.txt"


FILE *get_file(char *argv[]);
char *read_file(FILE *f);
int get_password();
void hide(char *argv, FILE *file, char *file_vals, char *asciis, char *mapping);
void recover(FILE *file, char *file_vals, char *asciis, char *mapping);
char *get_ascii_arrray(FILE *ascii_file);
char *get_mapping_array(int password, char *asciis);
void re_write_file(FILE *file, char *file_vals);
int string_to_int(char string[]);
int power(int base, int power);


int
main(int argc, char *argv[]){
	char *file_vals, *asciis, *mapping;
	FILE *ascii_file, *file;
	int password;

	//Open file and store it's content
	file = get_file(argv);
	if(file == NULL) exit(EXIT_FAILURE);
	file_vals = read_file(file);
	fclose(file);
	
	//reopen file so that it can now be written to
	//open ascii file so we can map the chars in the file to new files
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

	//use password to seed the random function
	password = get_password();
	srand(password);
	
	//get chars from ascii file
	asciis = get_ascii_arrray(ascii_file);
	fclose(ascii_file);
	
	//create an array that represents the chars respectively.
	mapping = get_mapping_array(password, asciis);

	//do as the user asked
	if(argv[2][0] == 'r'){
		recover(file, file_vals, asciis, mapping);
	} else {
		hide(argv[1], file, file_vals, asciis, mapping);
	}
	
	//can't put code here because I have an error check inside hide which doesn't exit but returns early from the function
	
	free(mapping);
	free(asciis);
	free(file_vals);
	fclose(file);
	return 0;
}

//**************************************************************************
// Functions

//Returns file string and checks for input errors
FILE 
*get_file(char *argv[]) {
	FILE *temp_file;
	//Error if inputs are missing
	if(argv[1] == NULL || argv[2] == NULL){
		printf("\nError. Missing inputs.\n");
		printf("Format is as follows:	");
		printf("\'hide *file location* *action*\'\n");
		printf("The \"action\" parameter can be \'h\'' for hiding file content or \'r\'' for recovering file content.\n");
		return NULL;
	}
	//Error if file can't be opened
	temp_file = fopen(argv[1], "r");
	if(temp_file == NULL){
		printf("\nError. Invalid file input.\n");
		printf("Make sure to include the entire file directory.\n");
		return NULL;
	//Error if second input doesn't match options
	} else if(argv[2][0] != 'h' && argv[2][0] != 'r'){
		printf("\nError. Invalid action.\n");
		printf("The \"action\" parameter can be \'h\'' for hiding file content or \'r\'' for recovering file content.\n");
		return NULL;
	}
	return temp_file;
}

//Returns array of characters found in the file
char 
*read_file(FILE *file) {
	int i=0, vals_size=INIT_FILE_SIZE;
	char c, *vals;
	
	vals = malloc(sizeof(char)*vals_size);
	if(vals == NULL){
		printf("Malloc Error.\n");
		exit(EXIT_FAILURE);
	}
	//iterate through file one at a time
	while((c = fgetc(file)) != EOF) {
		vals[i++] = c;
		//allocate more space for array if necassary
		if(i==vals_size) {
			vals_size *= 2;
			vals = realloc(vals, sizeof(char)*vals_size);
		}
	}
	vals[i] = '\0';
	return vals; 
}

//gets users password for transforming file
int
get_password(){
	int i;
	char c, pass[PASSWORD_MAX];
	//iteratively request the user to input a password until a valid password is given
	while(1) {
		printf("Input password number: ");
		scanf("%s", pass);
		i=0;
		//check that the password is an integer
		while((c=pass[i++])!='\0') {
			if(isalpha(c)) break;
		}
		if(c=='\0') break;
		printf("\nPassword must be an integer.\n");
	}
	//Return the password as an int
	return string_to_int(pass);
}

//Used in get_password to return the string password as an int
int 
string_to_int(char string[]) {
	int i, str_len, int_val=0;

	//convert string to int using ascii values of chars.
	str_len = strlen(string);
	for(i=0; i<str_len; i++) {
		int_val += (string[i]-48)*power(10,str_len-i);
	}
	return int_val;
}

//Used in string_to_int to return powers of 10 (this function works for other bases though)
int 
power(int base, int power) {
	int i, squared=1;
	for(i=0; i<power; i++){
		squared *= base;
	}
	return squared;
}

void 
hide(char *argv1, FILE *file, char *file_vals, char *asciis, char *mapping){
	int i=0, j;
	char c;
	
	//iterate through each char in the file
	while((c=file_vals[i++])!='\0'){
		//iterate through the possible chars to find a match
		for(j=0; j<ASCII_CHARS; j++) {
			//if found one change it to a new char from mapping
			if(c == asciis[j]){
				fprintf(file, "%c", mapping[j]);
				break;
			}
		}
		//If you didn't find one print error
		if(j == ASCII_CHARS){
			printf("\nError. Unknown character, %c in file.\n", c);
			printf("Program is being aborted. \nFile will be returned to normal.");
			//Reopened so that any changes that have already been made are cleared
			file = freopen(argv1, "w", stdout);
			re_write_file(file, file_vals);
			return;
		}
	}
	return;
}

void 
recover(FILE *file, char *file_vals, char *asciis, char *mapping){
	int i=0, j;
	char output_location, c;

	//ask user if they want to view the content or recover it to the file.
	getchar();
	while(1) {
		printf("Would you like to view the content here (v) or save it in the existing file (s)?\n");
		printf("Input: ");
		scanf("%c", &output_location);
		if(output_location == 'v' || output_location == 'f'){
			break;
		}
		printf("\nInvalid input. Try again.\n");
	}
	
	//same process as the hide function except mapping is replaced with asciis and vice versa
	while((c=file_vals[i++])!='\0'){
		for(j=0; j<ASCII_CHARS; j++) {
			if(mapping[j] == '\0'){
				printf("\nError. Unknown character in file.\n");
				printf("Program is being aborted. \nFile will be returned to normal.\n");
				printf("In other words... I can't recover the file content.\n");
				re_write_file(file, file_vals);
				return;
			} else if(c == mapping[j]){
				if(output_location == 'v'){
					printf("%c", asciis[j]);
				} else {
					fprintf(file, "%c", asciis[j]);
				}
				break;
			}
		}
	}
	if(output_location == 'f') {
		printf("\nFinished. You can now open the file.\n");
	} else {
		re_write_file(file, file_vals);
	}
	return;
}

//Reads through Resources/ascii.txt to get all possible chars
char 
*get_ascii_arrray(FILE *ascii_file){
	int i=0;
	char *asciis, c;
	
	//create array
	asciis = malloc(sizeof(char)*ASCII_CHARS);
	if(asciis == NULL){
		printf("Error while trying to allocate space.\n");
		exit(EXIT_FAILURE);
	}
	
	//iterate through the file and fill the array
	while((c=fgetc(ascii_file)) != EOF){
		asciis[i++] = c;
	}
	asciis[i] = '\0';
	return asciis;
}

//Uses password to return an array of rearranged ascii characters
 char 
 *get_mapping_array(int password, char *asciis){
 	int i, j, mapped, map_to;
 	char *mapping;
 	mapping = malloc(sizeof(char)*ASCII_CHARS);

 	for(i=0; i<ASCII_CHARS; i++){
		mapped = 0;
		map_to = (rand()%ASCII_CHARS);
		//iterate until we have found anoter char to map to
		while(!mapped) {
			mapped = 1;
			//iterate through mapping to see if this char is already used
			for(j=0; j<i; j++) {
				//if it is update map_to and repeat while loop
				if(mapping[j] == asciis[map_to]) {
					mapped = 0;
					j=-1;
					//If at end of array set to 0
					if(map_to == ASCII_CHARS-1){
						map_to = 0;
					} else {
						map_to++;
					}
					break;
				}
			}
		}
		mapping[i] = asciis[map_to];
 	}
	mapping[i] = '\0';
 	return mapping;
 }
  
 //Uses content of the file when first opened to restore it to how it was before program started
void 
re_write_file(FILE *file, char *file_vals){
	fprintf(file, "%s", file_vals);
	return;
}