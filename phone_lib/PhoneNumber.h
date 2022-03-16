#ifndef PHONE_LIB_PHONENUMBER_H_
#define PHONE_LIB_PHONENUMBER_H_

#include <stdio.h>

typedef struct Phone_number {
  char* name_t;
  char* net_t;
  char* phone_t;
} Phone_number;

char* read_name(FILE* fp);

char* read_phone(FILE* fp);

Phone_number contact_parser(char* name, char* phone);

int cont_cmp(const void* l, const void* r);

int print_all_contacts(Phone_number* contacts, size_t size, FILE* fp);

int main_work(FILE* in, FILE* out);

void free_contacts(Phone_number* contacts, size_t size);

#endif  // PHONE_LIB_PHONENUMBER_H_
