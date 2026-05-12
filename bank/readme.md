# Banking system

## Main

Opens accounts and insurances files, if they do not exist it creates these files

## Helpers

Helper functions used across the project

### **Local functions**

    Functions used only insude helpers.c

    1. validate_letters
        **Parameters: char *str**
        Goes over the string and checks if every letter is between 'a' and 'z'
        or between 'A' and 'Z'

        if not, returns 1
        else return 0

    2. generateId
        **Parameters: char *id**
        Creates 6 chars long uuid from charset A-Z and 0-9

    3. idExsists
        **Parameters: FILE *fp, char *id**
        Reads every uuid in file fp

        returns 1 if passed id already exists
        returns 0 otherwise

### Public functions

    Helper functions

    1. read_string
        **Parameters: const char *prompt, char *buffer, int max_len**
        Prints the prompt and reads to buffer first max_len chars from stdin
        then replaces '\n' at the end of the string with '\0'

    2. generateUniqueId
        **Parameters: FILE *fp, char *id**
        Generates id using generateId function until idExists is 1

    3. __refresh
        **Parameters: void**
        if helper variable refresh is 1
        clears the terminal with system appropriate command
        "cls" or "clear"

    4. print_banner
        **Parameters: int show_welcome**
        Prints "Bank.c" banner
        if show_welcome is 1 prints a welcome message

    5. int action
        **Parameters: int option, int options_len**
        Prints avaliable actions
        Waits for user input
        Makes sure user input is valid based on the number of options

    6. validate_account
        **Parameters: Account account**
        Checks if name and surname are only made of letters (validate_letters func)
        Checks if pesel has length 11 and is only numbers ('0' - '9')
        Checks if balance is more or equal to 0

        returns -1 if name or surname is invalid
        returns -2 if pesel is invalid
        returns -3 if balance is invalid
        returns 0 otherwise

    7. validate_insurance
        **Parameters: Insurance insurance, FILE *accounts**
        Checks if provided Account Id exists
        Checks if Registration Number is made of only capital letters and numbers
        Checks if insurance's price is more or equal to 0

        returns -1 if Account Id is invalid
        returns -2 if Registration Number is invalid
        returns -3 if price is invalid
        returns 0 otherwise

    8. _create_insurance && _create_account
        **Parameters: FILE *fp**
        Prints appropriate prompts, generates uuid and reads user input

        returns Account/ Insurance instance

    9. __exit()
        **Parameters: void**
        For listing, searching and other places in the program
        you don't want the screen to refresh immediately,
        it waits until user enters 'y' or 'Y'

## Actions

Database actions

### Insurance actions

    Database actions for insurances

    1. try_ins
        **Parameters: FILE *accounts, FILE *insurances**
        Tries to create insurance using _create_insurance function
        Validates insurance using validate_insurance
        Creates temporary table with corresponding user data and new insurance
        Waits for confirmation, if confirmed, creates insurance

        returns the same as validate_insurance and prints appropriate error messages
        returns 1 if cancelled

### Account actions

    Database actions for accounts

    1. try_acc
        **Parameters: FILE *accounts, FILE *insurances**
        Tries to create account using _create_account function
        Validates account using validate_account
        Creates temporary table with new user data
        Waits for confirmation, if confirmed, creates account

        returns the same as validate_account and prints appropriate error messages
        returns 1 if cancelled

    2. list_acc
        **Parameters: FILE *accounts, FILE *insurance**
        Calls print_table with account, insurance and empty type and query
        Calls __exit

    3. search_acc
        **Parameters: FILE *accounts, FILE *insurances**
        Reads and validates search type
        Reads query using read_string function
        Prints table using print_table with accounts, insurances, type and query
        Calls __exit

### Transfers

    Database actions for balance

    1. transfer
        **Parameters: FILE *accounts, FILE *insurances**
        Using read_string, gets from id, to id and amount
        Validates the Ids
        Validates if user has sufficient balance
        Makes the transfer between accounts
        Prints termporary tables using print_table for users with new balance
        Waits for confirmation and modifies users' balance in accounts file

    2. widthdraw
        **Parameters: FILE *accounts, FILE *insurances**
        Using read_string, gets from id and amount
        Validates the Id
        Validates if user has sufficient balance
        Makes the widthdrawal
        Prints termporary table using print_table for user with new balance
        Waits for confirmation and modifies user's balance in accounts file

    3. Deposit
        **Parameters: FILE *accounts, FILE *insurances**
        Using read_string, gets from id and amount
        Validates the Id
        Makes the widthdrawal
        Prints termporary table using print_table for user with new balance
        Waits for confirmation and modifies user's balance in accounts file

## Options

Options and prompts

## Models

Account and Insurance structure type definitions
Input lengths definitions

## Table

Dynamic table printing

### Private functions

Functions accessed only in table.c

    1. static max               -> returns max between two ints
    2. static print_separator   -> prints horizontal separator "+-----+"
                                   based on number of columns and width of cells
    3. static print_row         -> prints row with centered text based
                                   on width and content of the cell
                                   "|  content  |"
    4. satic *get_insurance_str -> trims content
    5. static validate_search   -> validates if row matches type and query

### Misc

Miscellaneous functions for table.c

    1. Count records            -> Counts number of records
                                   based on type and query
    2. free_table               -> frees table
    3. compute_col_widths       -> calculates columns' width

### Builders

Builder functions

    1. _build_table             -> Builds Table instance using
                                   private and misc functions

### Public funcs

Functions accessed outside table.c

    1. print_table              -> Takes accounts, insurances files,
                                   type and query
                                   Executes build, misc and private
                                   functions to print a table
