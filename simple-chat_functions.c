#include "simple-chat_functions.h"


//This function initializes a IPv4 Socket
SOCKET initialize_Socket_IPv4() {
	
	return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

//This function generates an IPv4 address allocated in the heap
struct sockaddr_in generate_IPv4_Address(char* ip, int port) {
	
	//Generating the address based on the inputed IP and the specified port
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);

	//Handling null IP's so that they become loopback
	if (strlen(ip) == 0)
		address.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY means any IP
	else
		inet_pton(AF_INET, ip, &address.sin_addr); //If whe actually have an IP, transform it to the network presentation form

	return address;
}

int checkPort5000() {
    FILE* fp;
    char buffer[MAX_MSG_SIZE];

    // Run netstat and open a pipe to read the output
    fp = popen("netstat -an", "r");
    if (fp == NULL) {
        perror("Can't check open ports");
        return 1;
    }

    printf("IPs with port 5000 open:\n");

    while (fgets(buffer, MAX_MSG_SIZE, fp) != NULL) {
        // Look for lines with ":5000" and either LISTENING or ESTABLISHED
        if (strstr(buffer, ":5000") && (strstr(buffer, "LISTENING") || strstr(buffer, "ESTABLISHED"))) {
            char* ipStart = strtok(buffer, " "); // Tokenize by spaces
            while (ipStart) {
                if (strchr(ipStart, '.')) { // Find an IPv4 address
                    printf("%s\n", ipStart);
                    break;
                }
                ipStart = strtok(NULL, " ");
            }
        }
    }

    pclose(fp);
    return 0;
}

int getWirelessIP() {
    FILE* fp;
    char buffer[MAX_MSG_SIZE];
    int inWirelessSection = 0;

    // Run ipconfig and open a pipe to read the output
    fp = popen("ipconfig", "r");
    if (fp == NULL) {
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

char *concat(char message[512], char username[30]){
	char *final_message;
	strcat(*final_message, username);
	strcat(*final_message, "> ");
	strcat(*final_message, message);

	return *final_message;
}


void encrypt(char text[512]) {
    char key[12] = "X̌₽æþ¤";
    for (int i = 0; i < strlen(text); i++) {
        text[i] = text[i] ^ key[i % strlen(key)];
    }
}
