#include <gtest/gtest.h>

extern "C" {
    #include "PhoneNumber.h"
}

TEST(read_name, testNULLfp) {
    EXPECT_EQ(NULL, read_name(NULL));
}

TEST(read_name, testNormalName) {
    const char * name = "\nDean\n";
    const char * name_n = "Dean";
    FILE * stdin_f = fmemopen((char *)name, strlen(name), "r");
    char * name_r = read_name(stdin_f);
    EXPECT_EQ(0, strcmp(name_n, name_r));
    if (name_r) {
        free(name_r); name_r = NULL;
    }
    fclose(stdin_f);
}

TEST(read_name, testLongName) {
    const char * name = "\nQwertyuiopasdfghjklzxcvbnm\n";
    const char * name_n = "Qwertyuiopasdfghjklzxcvbnm";
    FILE * stdin_f = fmemopen((char *)name, strlen(name), "r");
    char * name_r = read_name(stdin_f);
    EXPECT_EQ(0, strcmp(name_n, name_r));
    if (name_r) {
        free(name_r);
    }
    fclose(stdin_f);
}

TEST(read_name, testExtendedName) {
    const char * name = "\nQwertyuiopasdf}ghjklz{xcvbnm:Qw/er?"\
"tyuiopadd@!$sdfghj%klzxcvbnmQwertyuiopasdfghjklzxcvbnm\n";
    const char * name_n = "Qwertyuiopasdf}ghjklz{xcvbnm:Qw/er?"\
"tyuiopadd@!$sdfghj%klzxcvbnmQwertyuiopasdfghjklzxcvbnm";
    FILE * stdin_f = fmemopen((char *)name, strlen(name), "r");
    char * name_r = read_name(stdin_f);
    EXPECT_EQ(0, strcmp(name_n, name_r));
    if (name_r) {
        free(name_r);
    }
    fclose(stdin_f);
}

TEST(read_name, testNoName) {
    const char * name = "\n\n";
    FILE * stdin_f = fmemopen((char *)name, strlen(name), "r");
    char * name_r = read_name(stdin_f);
    EXPECT_EQ(NULL, name_r);
    if (name_r) {
        free(name_r);
    }
    fclose(stdin_f);
}

TEST(read_phone, testNULLfp) {
    EXPECT_EQ(NULL, read_phone(NULL));
}

TEST(read_phone, testNormalPhone) {
    const char * phone = "89771320591\n";
    const char * phone_n = "89771320591";
    FILE * stdin_f = fmemopen((char *)phone, strlen(phone), "r");
    char * phone_r = read_phone(stdin_f);
    EXPECT_EQ(0, strcmp(phone_n, phone_r));
    if (phone_r) {
        free(phone_r); phone_r = NULL;
    }
    fclose(stdin_f);
}

TEST(read_phone, testInvalidLongPhone) {
    const char * phone = "897713205911\n";
    FILE * stdin_f = fmemopen((char *)phone, strlen(phone), "r");
    char * phone_r = read_phone(stdin_f);
    EXPECT_EQ(NULL, phone_r);
    if (phone_r) {
        free(phone_r);
    }
    fclose(stdin_f);
}

TEST(read_phone, testInvalidShortPhone) {
    const char * phone = "8977132059\n";
    FILE * stdin_f = fmemopen((char *)phone, strlen(phone), "r");
    char * phone_r = read_phone(stdin_f);
    EXPECT_EQ(NULL, phone_r);
    if (phone_r) {
        free(phone_r);
    }
    fclose(stdin_f);
}

TEST(read_phone, testNoPhone) {
    const char * phone = "\n";
    FILE * stdin_f = fmemopen((char *)phone, strlen(phone), "r");
    char * phone_r = read_phone(stdin_f);
    EXPECT_EQ(NULL, phone_r);
    if (phone_r) {
        free(phone_r);
    }
    fclose(stdin_f);
}

TEST(contact_parser, testNormalContact) {
    const char * name = "Dean";
    const char * phone = "89771320591";
    Phone_number cont = contact_parser((char *)name, (char *)phone);
    EXPECT_EQ(0, strcmp(name, cont.name_t));
    char net[4];
    strncpy(net, phone + 1, 3);
    net[3] = '\0';
    char number[8];
    strncpy(number, phone + 4, 7);
    number[7] = '\0';
    EXPECT_EQ(0, strcmp(net, cont.net_t));
    EXPECT_EQ(0, strcmp(number, cont.phone_t));
    free(cont.name_t);
    free(cont.net_t);
    free(cont.phone_t);
}

// TEST(free_contacts, testNormal) {
//     size_t size = 5;
//     const char * data = "\nDean\n\
// 89771320591\n\
// \nDmitry\n\
// 89771320592\n\
// \nIvannnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn\n\
// 81000000000\n\
// \nSomeone\n\
// 81111111111\n\
// \nIvan\n\
// 89773792899\n";
//     FILE * stdin_f = fmemopen((char *)data, strlen(data), "r");
//     Phone_number * conts = (Phone_number *)calloc(size, sizeof(Phone_number));
//     for (size_t i = 0; i < size; ++i) {
//         char * name = read_name(stdin_f);
//         char * phone = read_phone(stdin_f);
//         //printf("\n%s %s\n", name, phone);
//         conts[i] = contact_parser(name, phone);
//         free(name);
//         free(phone);
//     }
//     fclose(stdin_f);
//     free_contacts(conts, size);
//     EXPECT_EQ(NULL, conts);
// }

TEST(print_all_contacts, testNULLfp) {
    size_t size = 1;
    Phone_number cont = {"Dean", "977", "1320591"};
    EXPECT_EQ(EXIT_FAILURE, print_all_contacts(&cont, size, NULL));
}

TEST(print_all_contacts, testNormal) {
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
    Phone_number * conts = (Phone_number *)calloc(size, sizeof(Phone_number));
    for (size_t i = 0; i < size; ++i) {
        char * name = read_name(stdin_f);
        char * phone = read_phone(stdin_f);
        //printf("\n%s %s\n", name, phone);
        conts[i] = contact_parser(name, phone);
        free(name);
        free(phone);
    }

    fclose(stdin_f);
    const char output[][100] = {
        "Owner: Ivannnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn\n\
Phone: 8(100)000-00-00\n\0",
        "Owner: Someone\nPhone: 8(111)111-11-11\n\0",
        "Owner: Dean\nPhone: 8(977)132-05-91\n\0", 
        "Owner: Dmitry\nPhone: 8(977)132-05-92\n\0", 
        "Owner: Ivan\nPhone: 8(977)379-28-99\n\0"};
     FILE * stdout_f = fopen("output.txt", "w");
    int res = print_all_contacts(conts, size, stdout_f);
    EXPECT_EQ(EXIT_SUCCESS, res);
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
    free_contacts(conts, size);
}

TEST(main_work, testNULLio) {
    EXPECT_EQ(EXIT_FAILURE, main_work(NULL, NULL));
    EXPECT_EQ(EXIT_FAILURE, main_work(stdin, NULL));
    EXPECT_EQ(EXIT_FAILURE, main_work(NULL, stdout));
}

TEST(main_work, testNormal) {
    char * input = "\
paosko\n\
\nnew\nDean\n89771320591\n\
\nnew\nDmitry\n89771320592\n\nnew\n\
Ivannnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn\n\
81000000000\n\nnew\n\n88888888888\n\
\nnew\nSomeone\n81111111111\n\
\nnew\nDean\n888\n\nnew\nIvan\n89773792899\n\
\nprint\n\nexit\n";
    FILE * in = fmemopen(input, strlen(input), "r");
    const size_t STR = 2000;
    char output[STR];
    FILE * out = fmemopen(output, STR, "w");
    main_work(in, out);
    char * output_ex = "\
Enter command (new, print, exit):\nWrong command!\n\
Enter command (new, print, exit):\nWrong command!\n\
Enter command (new, print, exit):\nEnter owner's name:\n\
Enter phone number:\nEnter command (new, print, exit):\n\
Enter owner's name:\nEnter phone number:\n\
Enter command (new, print, exit):\nEnter owner's name:\n\
Enter phone number:\nEnter command (new, print, exit):\n\
Enter owner's name:\nInvalid owner's name! Try again...\n\
Enter command (new, print, exit):\nWrong command!\n\
Enter command (new, print, exit):\nWrong command!\n\
Enter command (new, print, exit):\nWrong command!\n\
Enter command (new, print, exit):\nEnter owner's name:\n\
Enter phone number:\nEnter command (new, print, exit):\n\
Enter owner's name:\nEnter phone number:\n\
Invalid phone number! Try again...\n\
Enter command (new, print, exit):\nEnter owner's name:\n\
Enter phone number:\nEnter command (new, print, exit):\n\
Owner: Ivannnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn\n\
Phone: 8(100)000-00-00\nOwner: Someone\n\
Phone: 8(111)111-11-11\nOwner: Dean\n\
Phone: 8(977)132-05-91\nOwner: Dmitry\n\
Phone: 8(977)132-05-92\nOwner: Ivan\n\
Phone: 8(977)379-28-99\nEnter command (new, print, exit):\n";
    fclose(in);
    fclose(out);
    //printf("\noutput_ex = %s\n\noutput = %s\n", output_ex, output);
    EXPECT_EQ(0, strcmp(output_ex, output));
}