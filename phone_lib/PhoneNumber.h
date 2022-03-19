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

void run_new(FILE* in, FILE* out, Phone_number** contacts,
             size_t* size, size_t* cont);

void get_execute_cmd(FILE* in, FILE* out);

int get_name_chars(FILE* fp, char** name, size_t* size);

int get_phone_chars(FILE* fp, char** phone);

#endif  // PHONE_LIB_PHONENUMBER_H_
