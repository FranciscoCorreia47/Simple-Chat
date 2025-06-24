#include "simple-chat_functions.h"

// This function initializes a IPv4 Socket on the operating system, to allow the app to use network resources
SOCKET initialize_Socket_IPv4() {

    // Returns the socket structure 
    return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    /*		          ^	        ^		    ^
                      |         |		    |
        IPv4 Address mode       |		    |
                  Tells the OS the message type is TCP
    */
}

// This function generates an IPv4 address in network notation
struct sockaddr_in generate_IPv4_Address(char* ip, int port) {

    // Generating the address based on the inputed IP and the specified port
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    // Handling null IP's
    if (strlen(ip) == 0)
        // INADDR_ANY is a wildcard that means that we will bind to all/any IP address that the local computer currently has
        address.sin_addr.s_addr = INADDR_ANY;
    else
        // If an IP was parsed, transform it to the network presentation form
        inet_pton(AF_INET, ip, &address.sin_addr);

    // Return the created network IPv4 address structure
    return address;
}

// Tells the server what is it's current IP
int getWirelessIP() {
    FILE* fp;
    char buffer[MAX_MSG_SIZE];
    int inWirelessSection = 0;

    // Run ipconfig and open a pipe to read the output
    fp = popen("ipconfig", "r");
    if (fp == NULL) {
        // Here we print error and return 1 because if the server does not know it's own IP, it can't tell clients so that they connect
        perror("Can't check IP");
        return 1;
    }

    while (fgets(buffer, MAX_MSG_SIZE, fp) != NULL) {
        // Check if we are in the wireless section
        if (strstr(buffer, "Wireless LAN adapter") != NULL) {
            inWirelessSection = 1; // Found the Wireless NIC section
        }

        // If inside the wireless section, look for IPv4 Address
        if (inWirelessSection && strstr(buffer, "IPv4 Address") != NULL) {
            char* ipStart = strstr(buffer, ": "); // Find colon and space
            if (ipStart) {
                ipStart += 2; // Move past ": "
                printf("IPv4 Address: %s", ipStart);
            }
            break; // Stop searching after finding the IP
        }
    }
    pclose(fp);
}
