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
}