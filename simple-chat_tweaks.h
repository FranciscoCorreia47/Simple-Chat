// Encripts messages using XOR decryption
static void encrypt(char text[512]) {
    char key[12] = "X̌₽æþ¤";
    for (int i = 0; i < strlen(text); i++) {
        text[i] = text[i] ^ key[i % strlen(key)];
    }
}

// Concatenates the user's name and the message, to ease sending to server and printing on other clients/broadcasting
static char* concat(char message[512], char username[30]) {
    char* final_message;
    strcat(final_message, username);
    strcat(final_message, "> ");
    strcat(final_message, message);

    // Output should look like: "username> The message that the user inputed"
    return final_message;
}

// This function simply does what fflush(stdin) is supposed to do, but in a safe and consistent manner
static void flush_input(void) {
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
}
