#include "constants.h"
#include <stdio.h>
#include <string.h>

const int BASE_OPERATIONS = 4;
const int ACCOUNT_OPERATIONS = 4;
const int INSURANCE_OPERATIONS = 4;

const int MAIN_MENU = 1;
const int ACCOUNTS = 2;
const int INSURANCES = 3;
const int TRANSFERS = 4;

const int QUIT_OR_BACK = 1;
const int CREATE = 2;
const int SEARCH = 3;
const int LIST = 4;

const char *commands[] = {
    "quit",          "create_account",   "delete_account",
    "view_account",  "create_insurance", "delete_insurance",
    "view_insurance"};

const char *base_operations[] = {"1. Quit\n", "2. Accounts\n",
                                 "3. Insurances\n", "4. Transfer\n"};
const char *account_operations[] = {"1. Back\n", "2. Create account\n",
                                    "3. Search account\n",
                                    "4. List accounts\n"};
const char *insurance_operations[] = {"1. Back\n", "2. Create insurance\n",
                                      "3. Search insurance\n",
                                      "4. List insurances\n"};

const char **operations[] = {base_operations, account_operations,
                             insurance_operations};
const int operations_count[] = {BASE_OPERATIONS, ACCOUNT_OPERATIONS,
                                INSURANCE_OPERATIONS};

const char *create_account[] = {
    "Name: ", "Surname: ", "Address: [City Street StreetNumber HomeNumber]: ",
    "PESEL: ", "Balance: "};

const char *create_insurance[] = {
    "User Id: ", "Car registration number: ", "Insurance price: "};

const char **create_options[] = {create_account, create_insurance};
const int create_count[] = {5, 3};

const char *account_headers[] = {"ID",      "Name",  "Surname",
                                 "Address", "PESEL", "Balance"};
const char *insurance_headers[] = {"ID", "Account ID", "Car reg. number",
                                   "Price"};

const char **headers[] = {account_headers, insurance_headers};
const int headers_count[] = {6, 4};
const int cell_width[] = {20, 20};
