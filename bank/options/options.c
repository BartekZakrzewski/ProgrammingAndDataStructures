/* Options */
const char *menu_options[] = {
    "1. Quit",
    "2. Accounts",
    "3. Create Insurance",
    "4. Transfer"};

const char *account_options[] = {
    "1. Back",
    "2. Create Account",
    "3. List Accounts",
    "4. Search Accounts",
    "5. Widthdraw",
    "6. Deposit"};

const char **options[] = {menu_options, account_options};

/* Prompts */
const char *create_account[] = {
    "Name: ", "Surname: ", "Address: [City Street StreetNumber HomeNumber]: ",
    "PESEL: ", "Balance: "};

const char *create_insurance[] = {
    "User Id: ", "Car registration number: ", "Insurance price: "};

const char *search[] = {"Name", "Surname", "PESEL", "ID", "Address"};
const int n_search   = 5;

/* Table constants */
const char *headers[] = {"UUID", "Name", "Surname", "Address", "PESEL", "Balance", "Insurances"};
const int n_headers   = 7;
