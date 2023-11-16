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
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

int sendOverRS232(const char *data, const char *rs232Interface) {

    printf("Sending data over RS232 interface: %s\n", rs232Interface);
    printf("Data sent: %s\n", data);

    const char *port = "/dev/ttyUSB0"; // needs to be changed
    int serial_port = open(port, O_RDWR | O_NOCTTY);

    if (serial_port < 0) {
        perror("Error opening serial port");
        return -1;
    }

    struct termios portSettings;

    // get current attributes
    if (tcgetattr(serial_port, &portSettings) != 0) {
        perror("Error from tcgetattr");
        close(serial_port);
        return -1;
    }

    // baud rate needs to be changed
    cfsetospeed(&portSettings, B9600);
    cfsetispeed(&portSettings, B9600);

    // set port settings
    portSettings.c_cflag |= (CLOCAL | CREAD); // Enable receiver and ignore modem control lines
    portSettings.c_cflag &= ~CSIZE;
    portSettings.c_cflag |= CS8;              // 8-bit data
    portSettings.c_cflag &= ~PARENB;          // No parity bit
    portSettings.c_cflag &= ~CSTOPB;          // 1 stop bit
    portSettings.c_cflag &= ~CRTSCTS;         // No hardware flow control

    // set input output mode to raw
    portSettings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    portSettings.c_oflag &= ~OPOST;

    // set new attributes
    if (tcsetattr(serial_port, TCSANOW, &portSettings) != 0) {
        perror("Error from tcsetattr");
        close(serial_port);
        return -1;
    }

    write(serial_port, data, strlen(data));

    /*
     Examples of how to read with a buffer:

     char buffer [100];
     int n = read(serial_port, buffer, sizeof buffer);
     */

    close(serial_port);

    return 0;

}

int main(int argc, char *argv[]) {

	printf("Running! \n");

    // check if correct number of command line arguments are provided
    if (argc != 3 && argc != 4) {
    	printf("Incorrect command line inputs! \n");
    	printf("Format: gcc XXX -o XXX ./XXX XXX rs232_interface XXX \n");
        return -1;
    }

    const char *filePath = argv[1];

    // use either RS232 interface from command line or use a default value
    const char *rs232Interface = (argc == 4) ? argv[3] : "default_RS232";

    /*
    alternative method:

    char *fileP[] = (char *)malloc(sizeof(char) * 20);;
    scanf("Enter the filePath: %s", fileP);

    remember to free it later
    */

    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // read file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *fileContent = (char *)malloc(fileSize + 1);
    if (fileContent == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return -1;
    }


    // read file
    fread(fileContent, 1, fileSize, file);
    fclose(file);

    // null terminate the file content might not be necessary
    // fileContent[fileSize] = '\0';

    printf("File Content:\n%s\n", fileContent);

    sendOverRS232(fileContent, rs232Interface);

    free(fileContent);

    return 0;
}
