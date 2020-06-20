

/* This file implements file operations functions . Used to open , close and read the binary file .
 * This file is common across win/linux/mac
 */

#include "main.h"
FILE *file=NULL; //File pointer for our file related I/O
uint8_t file_is_opened = 0 ;
uint8_t user_app[300];
//This is the name of the .bin file stored in the below path .




//This function opens the file, calculates and returns total length in bytes
uint32_t calc_file_len(void)
{
    FILE *file;
    uint32_t fileLen;

    //OPen the file in binary format to read.
    printf ("\n\n   Enter full path of app's binary file: ");
    scanf("%s", user_app);
    file = fopen(user_app, "rb");

    if(! file){
        // fprintf(stderr, "Unable to open file %s", "user_app.bin");
        perror("\n   bin file not found");
        exit(0);
    }

    //Get file length
	fseek(file, 0, SEEK_END);
	fileLen=ftell(file);
	fseek(file, 0, SEEK_SET);

    fclose(file);

    return fileLen;

}

//This function opens the file , global file handle is used to store the file pointer
void open_the_file(void)
 {
    file = fopen(user_app, "rb");

    if(! file){
        // fprintf(stderr, "Unable to open file %s", "user_app.bin");
        perror("\n   bin file not found");
        exit(0);
    }

 }

 //This function reads the file for a given "len" bytes
uint32_t read_the_file(uint8_t *buffer, uint32_t len)
{
    uint32_t ret=0;

    //Read file contents into buffer
	ret=fread(buffer, 1, len, file);

	//return how much is really read
    return ret;

}

//close the global file handle
void close_the_file(void)
{
    fclose(file);
}
