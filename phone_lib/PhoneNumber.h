#ifndef HW_1_PHONE_LIB_PHONENUMBER_H_
#define HW_1_PHONE_LIB_PHONENUMBER_H_

#include <stdio.h>

typedef struct PhoneNumber {
  char* name;
  char* netCode;
  char* phone;
} PhoneNumber;

char* readName(FILE* fp);

char* readPhone(FILE* fp);

PhoneNumber contactParser(char* name, char* phone);

int contCmp(const void* l, const void* r);

void printAllContacts(PhoneNumber* contacts, size_t size, FILE* fp);

#endif  // HW_1_PHONE_LIB_PHONENUMBER_H_
