#include "insurances.h"

Insurance read_insurance(FILE *fp) {
    Insurance insurance;

    generateUniqueId(fp, insurance.insuraceId);
    read_string(create_insurance[0], insurance.accId, 7);
    read_string(create_insurance[1], insurance.regNumber, REG_LEN);

    char price[32];
    read_string(create_insurance[2], price, sizeof(price));
    insurance.price = atof(price);

    return insurance;
}
