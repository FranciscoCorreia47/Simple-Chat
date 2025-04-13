#include "simple-chat_functions.h"

void encrypt(char text[512]){
  char key[6] = "X̌₽æþ¤";
  for(int i = 0; i < strlen(text); i++){
        text[i] = text[i] ^ key[i % strlen(key)];
  }
}

