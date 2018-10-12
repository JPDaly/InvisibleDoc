#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INIT_FILE_SIZE 100
#define ASCII_CHARS  97
#define PASSWORD_MAX 20
#define ASCII_FILE_LOCATION "E:/Programming/c/InvisibleDoc/Resources/ascii.txt"
//this should probably just be "/Resources/ascii.txt" since not everyone will keep this in the same directory

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
void hide(FILE *file, char *file_vals, char *asciis, FILE *mapping);
void recover(FILE *file, char *file_vals, char *asciis, char *mapping);
char *get_ascii_arrray(FILE *ascii_file);
char *get_mapping_array(int password, char *asciis);
void re_write_file(FILE *file, char *file_vals);

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
	srand(get_password());
	
	//get chars from ascii file
	asciis = get_ascii_arrray(ascii_file);
	fclose(ascii_file);
	
	/*
		From here on I haven't compiled or checked.
	*/
	
	//create an array that represents the chars respectively.
	mapping = get_mapping_array(password, asciis);
	
	/*
		Testing:
			-Is mapping just a rearranged version of asciis?
			-Is the output printing as a string? (is the '\0' character there?).
			-Any warnings or errors?
	*/
	printf("%s\n", asciis);
	printf("%s\n", mapping);
	
	//do as the user asked
	if(argv[2][0] == 'r'){
		recover(file, file_vals, ascii_file, mapping);
	} else {
		hide(file, file_vals, ascii_file, mapping);
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
hide(FILE *file, char *file_vals, char *asciis, FILE *mapping){
	int i=0, j;
	char c;
	
	//iterate through each char in the file
	while((c=file_vals[i++])!='\0'){
		//iterate through the possible chars to find a match
		for(j=0; j<ASCII_CHARS; j++) {
			//If you didn't find one print error
			if(asciis[j] == '\0'){
				printf("\nError. Unknown character in file.\n");
				printf("Program is being aborted. \nFile will be returned to normal.");
				re_write_file(file, file_vals);
				return;
			//if found one change it to a new char from mapping
			} else if(c == asciis[j]){
				fprintf(file, "%c", mapping[j]);
				break;
			}
		}
	}
	
	return;
}

void 
recover(FILE *file, char *file_vals, char *asciis, char *mapping){
	int i=0, j;
	char output_location, c;

	//ask user if they want to view the content or recover it to the file.
	while(1) {
		printf("Would you like to view the content here (v) or save it in the existing file (s)?\n");
		printf("Input: ");
		scanf("%c", output_location);
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
				print("In other words... I can't recover the file content.\n");
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
	if(output_location == 's') {
		printf("\nFinished. You can now open the file.\n");
	}
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
	asciis[i] = '\0';
	return asciis;
}

 char 
 *get_mapping_array(int password, char *asciis){
 	int i, j, c, mapped;
 	char *mapping, map_to;
 	mapping = malloc(sizeof(char)*ASCII_CHARS);

 	for(i=0; i<ASCII_CHARS; i++){
		mapped = 0;
 		/*
			POTENTIAL ERROR!!!! I don't think you need the -32 in this.
		*/
		map_to = (rand()%ASCII_CHARS)-32;
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
 
 
//Hasn't been tested and may not work
void 
re_write_file(FILE *file, char *file_vals){
	fprintf(file, "%s", file_vals);
	return;
}