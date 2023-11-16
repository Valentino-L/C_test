/*
 ============================================================================
 Name        : Test1.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sendOverRS232(const char *data, const char *rs232Interface) {

    printf("Sending data over RS232 interface: %s\n", rs232Interface);

    // Do something

    printf("Data sent: %s\n", data);
}

int main(int argc, char *argv[]) {

	printf("Running! \n");

    // Check if correct number of command line arguments are provided
    if (argc != 3 && argc != 4) {
    	printf("Incorrect command line inputs! \n");
    	printf("gcc XXX -o XXX ./XXX XXX rs232_interface XXX \n");
        return EXIT_FAILURE;
    }

    const char *filePath = argv[1];

    // use either RS232 interface from command line or use a default value
    const char *rs232Interface = (argc == 4) ? argv[3] : "default_RS232";

    // alternatively
    /*
    char *fileP[] = (char *)malloc(sizeof(char) * 20);;
    scanf("Enter the filePath: %s", fileP);
    // remember to free it later
    */

    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // read file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *fileContent = (char *)malloc(fileSize + 1);
    if (fileContent == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return EXIT_FAILURE;
    }


    // read file
    fread(fileContent, 1, fileSize, file);
    fclose(file);

    // null terminate the file content might not be necessary
    // fileContent[fileSize] = '\0';

    printf("File Content:\n%s\n", fileContent);

    sendOverRS232(fileContent, rs232Interface);

    free(fileContent);

    return EXIT_SUCCESS;
}
