#include <gtest/gtest.h>

extern "C" {
    #include "PhoneNumber.h"
}

TEST(readName, testNormalName) {
    const char * name = "\nDean\n";
    const char * name_n = "Dean";
    FILE * stdin_f = fmemopen((char *)name, strlen(name), "r");
    char * name_r = readName(stdin_f);
    EXPECT_EQ(0, strcmp(name_n, name_r));
    if (name_r) {
        free(name_r); name_r = NULL;
    }
    fclose(stdin_f);
}

TEST(readName, testLongName) {
    const char * name = "\nQwertyuiopasdfghjklzxcvbnm\n";
    const char * name_n = "Qwertyuiopasdfghjklzxcvbnm";
    FILE * stdin_f = fmemopen((char *)name, strlen(name), "r");
    char * name_r = readName(stdin_f);
    EXPECT_EQ(0, strcmp(name_n, name_r));
    if (name_r) {
        free(name_r);
    }
    fclose(stdin_f);
}

TEST(readName, testExtendedName) {
    const char * name = "\nQwertyuiopasdf}ghjklz{xcvbnm:Qw/er?""tyuiopadd@!$sdfghj%klzxcvbnmQwertyuiopasdfghjklzxcvbnm\n";
    const char * name_n = "Qwertyuiopasdf}ghjklz{xcvbnm:Qw/er?""tyuiopadd@!$sdfghj%klzxcvbnmQwertyuiopasdfghjklzxcvbnm";
    FILE * stdin_f = fmemopen((char *)name, strlen(name), "r");
    char * name_r = readName(stdin_f);
    EXPECT_EQ(0, strcmp(name_n, name_r));
    if (name_r) {
        free(name_r);
    }
    fclose(stdin_f);
}

TEST(readName, testNoName) {
    const char * name = "\n\n";
    FILE * stdin_f = fmemopen((char *)name, strlen(name), "r");
    char * name_r = readName(stdin_f);
    EXPECT_EQ(NULL, name_r);
    if (name_r) {
        free(name_r);
    }
    fclose(stdin_f);
}

TEST(readPhone, testNormalPhone) {
    const char * phone = "89771320591\n";
    const char * phone_n = "89771320591";
    FILE * stdin_f = fmemopen((char *)phone, strlen(phone), "r");
    char * phone_r = readPhone(stdin_f);
    EXPECT_EQ(0, strcmp(phone_n, phone_r));
    if (phone_r) {
        free(phone_r); phone_r = NULL;
    }
    fclose(stdin_f);
}

TEST(readPhone, testInvalidLongPhone) {
    const char * phone = "897713205911\n";
    FILE * stdin_f = fmemopen((char *)phone, strlen(phone), "r");
    char * phone_r = readPhone(stdin_f);
    EXPECT_EQ(NULL, phone_r);
    if (phone_r) {
        free(phone_r);
    }
    fclose(stdin_f);
}

TEST(readPhone, testInvalidShortPhone) {
    const char * phone = "8977132059\n";
    FILE * stdin_f = fmemopen((char *)phone, strlen(phone), "r");
    char * phone_r = readPhone(stdin_f);
    EXPECT_EQ(NULL, phone_r);
    if (phone_r) {
        free(phone_r);
    }
    fclose(stdin_f);
}

TEST(readPhone, testNoPhone) {
    const char * phone = "\n";
    FILE * stdin_f = fmemopen((char *)phone, strlen(phone), "r");
    char * phone_r = readPhone(stdin_f);
    EXPECT_EQ(NULL, phone_r);
    if (phone_r) {
        free(phone_r);
    }
    fclose(stdin_f);
}

TEST(contactParser, testNormalContact) {
    const char * name = "Dean";
    const char * phone = "89771320591";
    PhoneNumber cont = contactParser((char *)name, (char *)phone);
    EXPECT_EQ(0, strcmp(name, cont.name));
    char netCode[4];
    strncpy(netCode, phone + 1, 3);
    netCode[3] = '\0';
    char number[8];
    strncpy(number, phone + 4, 7);
    number[7] = '\0';
    EXPECT_EQ(0, strcmp(netCode, cont.netCode));
    EXPECT_EQ(0, strcmp(number, cont.phone));
    free(cont.name);
    free(cont.netCode);
    free(cont.phone);
}

TEST(printAllContacts, testNormal) {
    size_t size = 5;
    const char * data = "\nDean\n\
89771320591\n\
\nDmitry\n\
89771320592\n\
\nIvannnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn\n\
81000000000\n\
\nSomeone\n\
81111111111\n\
\nIvan\n\
89773792899\n";
    FILE * stdin_f = fmemopen((char *)data, strlen(data), "r");
    PhoneNumber * conts = (PhoneNumber *)calloc(size, sizeof(PhoneNumber));
    for (size_t i = 0; i < size; ++i) {
        char * name = readName(stdin_f);
        char * phone = readPhone(stdin_f);
        //printf("\n%s %s\n", name, phone);
        conts[i] = contactParser(name, phone);
        free(name);
        free(phone);
    }

     fclose(stdin_f);
    const char output[][100] = {
        "Owner: Ivannnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn\nPhone: 8(100)000-00-00\n\0",
        "Owner: Someone\nPhone: 8(111)111-11-11\n\0",
        "Owner: Dean\nPhone: 8(977)132-05-91\n\0", 
        "Owner: Dmitry\nPhone: 8(977)132-05-92\n\0", 
        "Owner: Ivan\nPhone: 8(977)379-28-99\n\0"};
     FILE * stdout_f = fopen("output.txt", "w");
    printAllContacts(conts, size, stdout_f);
    fclose(stdout_f);
    stdout_f = fopen("output.txt", "r");
    char lines[5][100];
    int c, i = 0, line = 0;
    int isNew = 0;
    while((c = fgetc(stdout_f)) != EOF) {
        if (c == '\n') {
            isNew++;
            if (isNew % 2 == 0 && isNew) {
                lines[line][i++] = '\n';
                lines[line++][i] = '\0';
                i = 0;
                continue;
            }
        }
        lines[line][i++] = c;
    };
    fclose(stdout_f);
    remove("output.txt");
    for (size_t i = 0; i < size; i++) {
        //printf("\n%s\n\n%s\n", output[i], lines[i]);
        EXPECT_EQ(0, strcmp(output[i], lines[i]));
    }
    //printf("\n%s\n\n%s\n", lines, output);
    //free(lines);
    for  (size_t i = 0; i < size; ++i) {
        free(conts[i].name);
        free(conts[i].netCode);
        free(conts[i].phone);
    }
    free(conts);
}