#include <stdio.h>
#include "Payment.h"

void main(void)
{
    unsigned char replay;
    do
    {
        ST_transaction transaction;
        ST_transaction * transaction_ptr = &transaction;       

        printf("Please Enter Card Data:\n");
        /* fills card data */
        fillCardData(transaction_ptr);       

        printf("Please Enter Terminal Data:\n");
        /* fills terminal data */
        fillTerminalData(transaction_ptr);

        /*check if the user he wants to continue or not */
        printf("Do you want to continue (y/n)?:\n");
        replay = getc(stdin);
    }while(replay == 'Y' || replay == 'y');
}