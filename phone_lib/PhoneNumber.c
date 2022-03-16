#include "PhoneNumber.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMPTY_FP "fp which is FILE * is NULL\n"
#define ENTER_STRING "Enter command (new, print, exit):\n"

const size_t MAX_SIZE = 20;
const size_t MAX_PHONE_SIZE = 11;
const size_t NET_CODE_T_SIZE = 4;
const size_t PHONE_T_SIZE = 8;

char *read_name(FILE *fp) {
  if (!fp) {
    printf("%s", EMPTY_FP);
    return NULL;
  }
  int c;
  size_t size = MAX_SIZE;
  char *name = (char *)calloc(size, sizeof(char));
  if (!name) return NULL;
  size_t i = 0;
  fgetc(fp);
  while (isprint(c = fgetc(fp))) {
    if (i == size - 1) {
      size += MAX_SIZE;
      char *tmp = (char *)realloc(name, size);
      if (tmp) {
        name = tmp;
      } else {
        if (name) free(name);
        return NULL;
      }
    }
    name[i++] = (char)c;
  }
  if (c == '\n' && i > 0) {
    name[i] = '\0';
    return name;
  }
  if (name) free(name);
  return NULL;
}

char *read_phone(FILE *fp) {
  if (!fp) {
    printf("%s", EMPTY_FP);
    return NULL;
  }
  int c;
  char *phone = (char *)calloc(MAX_PHONE_SIZE + 1, sizeof(char));
  if (!phone) return NULL;
  size_t i = 0;
  while (isdigit(c = fgetc(fp))) {
    if (i == MAX_PHONE_SIZE) {
      if (phone) free(phone);
      return NULL;
    }
    phone[i++] = (char)c;
  }
  if (c == '\n' && i == MAX_PHONE_SIZE) {
    phone[MAX_PHONE_SIZE] = '\0';
    return phone;
  }
  if (phone) free(phone);
  return NULL;
}

Phone_number contact_parser(char *name, char *phone) {
  Phone_number contact;
  contact.name_t = (char *)calloc(strlen(name) + 1, sizeof(char));
  contact.net_t = (char *)calloc(NET_CODE_T_SIZE, sizeof(char));
  contact.phone_t = (char *)calloc(PHONE_T_SIZE, sizeof(char));
  if (contact.name_t && contact.net_t && contact.phone_t) {
    strncpy(contact.name_t, name, strlen(name));
    strncpy(contact.net_t, phone + 1, 3);
    strncpy(contact.phone_t, phone + 4, 7);
    contact.name_t[strlen(name)] = '\0';
    contact.net_t[3] = '\0';
    contact.phone_t[7] = '\0';
  }
  return contact;
}

int cont_cmp(const void *l, const void *r) {
  Phone_number *lhs = (Phone_number *)l;
  Phone_number *rhs = (Phone_number *)r;
  return strcmp(lhs->net_t, rhs->net_t);
}

int print_all_contacts(Phone_number *contacts, size_t size, FILE *fp) {
  if (!fp) {
    printf("%s", EMPTY_FP);
    return EXIT_FAILURE;
  }
  qsort(contacts, size, sizeof(Phone_number), cont_cmp);
  for (size_t i = 0; i < size; ++i) {
    char first[4] = {contacts[i].phone_t[0], contacts[i].phone_t[1],
                     contacts[i].phone_t[2], '\0'};
    char second[3] = {contacts[i].phone_t[3], contacts[i].phone_t[4], '\0'};
    char third[3] = {contacts[i].phone_t[5], contacts[i].phone_t[6], '\0'};
    fprintf(fp, "Owner: %s\n", contacts[i].name_t);
    fprintf(fp, "Phone: 8(%s)%s-%s-%s\n", contacts[i].net_t, first, second,
            third);
  }
  return EXIT_SUCCESS;
}

void free_contacts(Phone_number *contacts, size_t size) {
  if (contacts) {
    for (size_t i = 0; i < size; ++i) {
      if (contacts[i].name_t) {
        free(contacts[i].name_t);
        contacts[i].name_t = NULL;
      }
      if (contacts[i].net_t) {
        free(contacts[i].net_t);
        contacts[i].net_t = NULL;
      }
      if (contacts[i].phone_t) {
        free(contacts[i].phone_t);
        contacts[i].phone_t = NULL;
      }
    }
    free(contacts);
  }
}

#define ENTER_NAME "Enter owner's name:\n"
#define ENTER_PHONE "Enter phone number:\n"
#define INVALID_NAME "Invalid owner's name! Try again...\n"
#define INVALID_PHONE "Invalid phone number! Try again...\n"
#define WRONG_COMMAND "Wrong command!\n"
#define COMMAND_FORMAT "%5s"
#define EMPTY_IN "in which is FILE * is NULL\n"
#define EMPTY_OUT "out which is FILE * is NULL\n"
#define CMD_NEW "new"
#define CMD_PRINT "print"
#define CMD_EXIT "exit"

const size_t MAX_CONTACTS = 10;
const size_t COMMAND_SIZE = 6;

int main_work(FILE *in, FILE *out) {
  if (!in) {
    printf("%s", EMPTY_IN);
    return EXIT_FAILURE;
  }
  if (!out) {
    printf("%s", EMPTY_OUT);
    return EXIT_FAILURE;
  }

  size_t size = MAX_CONTACTS;
  Phone_number *contacts = (Phone_number *)calloc(size, sizeof(Phone_number));
  size_t cont = 0;
  char command[COMMAND_SIZE];
  int status = 1;

  while (status > 0) {
    fprintf(out, ENTER_STRING);
    if (fscanf(in, COMMAND_FORMAT, command) == 1) {
      if (strcmp(command, CMD_NEW) == 0) {
        fprintf(out, ENTER_NAME);
        char *name = read_name(in);
        if (name) {
          fprintf(out, ENTER_PHONE);
          char *phone = read_phone(in);
          if (phone) {
            if (cont == size) {
              size += MAX_CONTACTS;
              Phone_number *tmp = (Phone_number *)realloc(
                  contacts, sizeof(Phone_number) * size);
              contacts = tmp;
            }
            contacts[cont++] = contact_parser(name, phone);
            free(phone);
          } else {
            fprintf(out, INVALID_PHONE);
          }
          free(name);
        } else {
          fprintf(out, INVALID_NAME);
        }
      } else if (strcmp(command, CMD_PRINT) == 0) {
        print_all_contacts(contacts, cont, out);
      } else if (strcmp(command, CMD_EXIT) == 0) {
        status = -1;
      } else {
        fprintf(out, WRONG_COMMAND);
      }
    }
  }
  free_contacts(contacts, size);
  return EXIT_SUCCESS;
}
