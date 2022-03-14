#include "PhoneNumber.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t MAX_SIZE = 20;
const size_t MAX_PHONE_SIZE = 11;

char *readName(FILE *fp) {
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

char *readPhone(FILE *fp) {
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

PhoneNumber contactParser(char *name, char *phone) {
  PhoneNumber contact;
  contact.name = (char *)calloc(strlen(name) + 1, sizeof(char));
  contact.netCode = (char *)calloc(4, sizeof(char));
  contact.phone = (char *)calloc(8, sizeof(char));
  if (contact.name && contact.netCode && contact.phone) {
    strncpy(contact.name, name, strlen(name));
    strncpy(contact.netCode, phone + 1, 3);
    strncpy(contact.phone, phone + 4, 7);
    contact.name[strlen(name)] = '\0';
    contact.netCode[3] = '\0';
    contact.phone[7] = '\0';
  }
  return contact;
}

int contCmp(const void *l, const void *r) {
  PhoneNumber *lhs = (PhoneNumber *)l;
  PhoneNumber *rhs = (PhoneNumber *)r;
  return strcmp(lhs->netCode, rhs->netCode);
}

void printAllContacts(PhoneNumber *contacts, size_t size, FILE *fp) {
  qsort(contacts, size, sizeof(PhoneNumber), contCmp);
  for (size_t i = 0; i < size; ++i) {
    char first[4] = {contacts[i].phone[0], contacts[i].phone[1],
                     contacts[i].phone[2], '\0'};
    char second[3] = {contacts[i].phone[3], contacts[i].phone[4], '\0'};
    char third[3] = {contacts[i].phone[5], contacts[i].phone[6], '\0'};
    fprintf(fp, "Owner: %s\n", contacts[i].name);
    fprintf(fp, "Phone: 8(%s)%s-%s-%s\n", contacts[i].netCode, first, second,
            third);
  }
}
