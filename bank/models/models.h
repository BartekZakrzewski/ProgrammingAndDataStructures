#ifndef MODELS_H
#define MODELS_H

#define NAME_LEN 50
#define ADDR_LEN 100
#define PESEL_LEN 25

typedef struct {
    char accId[6];
    char name[NAME_LEN];
    char surname[NAME_LEN];
    char address[ADDR_LEN];
    char pesel[PESEL_LEN];
    double balance;
} Account;

#define REG_LEN 10

typedef struct {
    char insuraceId[6];
    char accId[6];
    char regNumber[REG_LEN];
    double price;
} Insurance;

#endif
