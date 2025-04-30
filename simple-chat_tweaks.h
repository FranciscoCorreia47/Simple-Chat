//Function that encrypts the message from the client before sending it, using XOR
void encrypt(char text[512]);

//Function that put together the username chosen by the client and the message that will be sent
void concat(char message[512], char username[30]);

// This Function does what fflush(stdin) should do, without any risks
static void flush_input(void){
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF);
}
