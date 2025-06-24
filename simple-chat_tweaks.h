// Encripts messages using XOR decryption
static void encrypt(char text[512]) {
    char key[12] = "X̌₽æþ¤";
    for (int i = 0; i < strlen(text); i++) {
        text[i] = text[i] ^ key[i % strlen(key)];
    }
}

// Concatenates the user's name and the message, to ease sending to server and printing on other clients/broadcasting
static char* concat(const char message[512], const char username[30]) {
    // Allocate enough memory: username + "> " + message + null terminator
    size_t len = strlen(username) + 2 + strlen(message) + 1;
    char* final_message = malloc(len);

    if (!final_message) return NULL; // Allocation failed

    strcpy(final_message, username);
    strcat(final_message, "> ");
    strcat(final_message, message);

    final_message[len - 1] = '\0';

    return final_message;
}

// This function simply does what fflush(stdin) is supposed to do, but in a safe and consistent manner
static void flush_input(void) {
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
}
