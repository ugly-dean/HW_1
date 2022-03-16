#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PhoneNumber.h"

const size_t MAX_CONTACTS = 10;

int main() {
  size_t size = MAX_CONTACTS;
  PhoneNumber *contacts = (PhoneNumber *)calloc(size, sizeof(PhoneNumber));
  size_t cont = 0;
  char command[6];
  int status = 1;

  while (status > 0) {
    printf("Enter command (new, print, exit):\n");
    if (scanf("%5s", command) == 1) {
      if (strcmp(command, "new") == 0) {
        printf("Enter owner's name:\n");
        char *name = readName(stdin);
        if (name) {
          printf("Enter phone number:\n");
          char *phone = readPhone(stdin);
          if (phone) {
            if (cont == size) {
              size += MAX_CONTACTS;
              PhoneNumber *tmp =
                  (PhoneNumber *)realloc(contacts, sizeof(PhoneNumber) * size);
              contacts = tmp;
            }
            contacts[cont++] = contactParser(name, phone);
            free(phone);
          } else {
            printf("Invalid phone number! Try again...\n");
          }
          free(name);
        } else {
          printf("Invalid owner's name! Try again...\n");
        }
      } else if (strcmp(command, "print") == 0) {
        printAllContacts(contacts, cont, stdout);
      } else if (strcmp(command, "exit") == 0) {
        status = -1;
      } else {
        printf("Wrong command!\n");
      }
    }
  }
  for (size_t i = 0; i < size; ++i) {
    free(contacts[i].name);
    free(contacts[i].netCode);
    free(contacts[i].phone);
  }
  free(contacts);
  return 0;
}
