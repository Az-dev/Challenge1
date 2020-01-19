#include <stdio.h>
#include "Payment.h"
#include <string.h>

/*represents transaction reference number*/
static unsigned int trans_ref_num = 0;

/*a flag to indicate whether a primary card number exists in server array or not*/
static unsigned char card_exist = 0; /* `0` initial value represents non-existance state*/ 

/*
    server array that stores transactions
*/
ST_transaction server_transactions[255];

/*
    Server Given account/balance array of ST_accountBalance struct
*/
ST_accountBalance server_accounts[10] =
{
    {"0100.00","123456789"},
    {"6000.00","234567891"},
    {"3250.25","567891234"},
    {"1500.12","456789123"},
    {"0500.00","258649173"},
    {"2100.00","654823719"},
    {"0000.00","971362485"},
    {"0001.00","793148625"},
    {"0010.12","123123456"},
    {"0000.55","456789321"},    
};

/*
    compares two strings.
*/
unsigned char compareStrings(unsigned char *str1,unsigned char *str2,unsigned int size)
{
    int i =0 ;
    for(;i<(size-1);i++)
    {
        if(str1[i] > str2[i])
        {            
            return 1; 
        }        
        else if (str1[i] < str2[i])
        {            
            return -1; 
        }               
    }    
    return 0;
}

/*
    compare expire date against transaction date.
*/
unsigned char compareToTransDate(const unsigned char * trans_date,const unsigned char * expire_date)
{
    unsigned char trans_year[3] , expire_year[3] , trans_month[3] , expire_month[3];  
    /*extracting expiration year as a copy into expire_year[3]*/
    strncpy(expire_year, &expire_date[3],2);
    /*extracting transaction year as a copy into trans_year[3]*/
    strncpy(trans_year, &trans_date[8],2);
    /*extracting transaction month as a copy into trans_month[3]*/
    strncpy(trans_month, &trans_date[3],2);
    /*extracting expiration month as a copy into expire_month[3]*/
    strncpy(expire_month,&expire_date[0],2);    
    /*comparing years*/
    if(compareStrings(trans_year,expire_year,3) == 0)
    {
        /*years are equal then proceed to check months*/
        if(compareStrings(trans_month,expire_month,3) == 1)
        {
            /*trans_month > expire_month*/
            return 1;
        }else
        {
            /*trans_month <= expire_month*/
            return 0;
        }

    }else if(compareStrings(trans_year,expire_year,3) == 1)
    {
        /*transaction year > expire year*/
        return 1;
    }else
    {
        /*transaction year < expire year*/ 
        return 0;
    }
}


/*
    Saves the transaction data into the server array.
*/
void saveTransactionIntoServer(ST_transaction * transaction)
{
    /*save transaction to server_transactions*/
    server_transactions[trans_ref_num] = *(transaction);    
}

/*
    Checks if the transaction amount > account balance     
*/
void checkBalance(ST_transaction * transaction)
{
    /* Check existance of the primary account number in server_accounts array */
    int i = 0;
    for (;i<10;i++)
    {
        if(compareStrings(transaction->cardHolderData.primaryAccountNumber, server_accounts[i].primaryAccountNumber,10) == 0)
        {
            card_exist = 1; 
            printf("Verifying Data from the server...\n");
            /*checks if the transaction amount > account balance*/            
            if(compareStrings(transaction->transData.transAmount,server_accounts[i].balance,8) == 1)
            {     
                /*save status*/                 
                transaction->transResponse.transactionStatus = 0; /*DECLINED*/
                printf("The transaction is DECLINED\n");                              
            }else{
                /*save status*/
                transaction->transResponse.transactionStatus = 1; /*APPROVED*/ 
                printf("The transaction is APPROVED\n");
            }
        }        
    }
    if(!card_exist){
        /*save status*/                 
        transaction->transResponse.transactionStatus = 0; /*DECLINED*/
        printf("The transaction is DECLINED\n");  
    }
    /*store reference number*/ 
    transaction->transResponse.receiptReferenceNumber = trans_ref_num;
    /*save transaction refernce number*/ 
    saveTransactionIntoServer(transaction);
}

/*
    Checks transaction withdrawal amount versus the allowed max value
*/
void checkAmount(ST_transaction * transaction)
{   
    /*comparing entered amount against max value*/    
    if(compareStrings(transaction->transData.transAmount,"5000.00",8) == 1)
    { 
        /*saving the response*/
        transaction->transResponse.transactionStatus = 0; /*DECLINED*/
        printf("The transaction is DECLINED!\n");
    }else
    {
        /* Storing transaction date. */
        printf("Please Enter transaction Date:\n");
        fflush(stdin);
        fgets(transaction->transData.transactionDate,11,stdin);    

        /* check checkExpiryData */
        checkExpiryData(transaction);
    }
}

/*
    Checks expiration data.
*/
void checkExpiryData(ST_transaction * transaction)
{
    if(compareToTransDate(transaction->transData.transactionDate,transaction->cardHolderData.cardExpirationDate) == 1)
    { 
        /*saving the response*/        
        transaction->transResponse.transactionStatus = 0; /*DECLINED*/
        printf("The transaction is DECLINED!\n");
    }else{           
        /* check balance*/
        checkBalance(transaction);
    }
}

/*
    fills terminal data.
*/
void fillTerminalData(ST_transaction * transaction)
{
    /* Storing transaction amount*/
    printf("Please Enter The Amount:\n");
    fflush(stdin);
    fgets(transaction->transData.transAmount,8,stdin); 

    /* Storing max amount*/    
    strcpy(transaction->transData.maxTransAmount,"5000.00");

    /* check entered amount */
    checkAmount(transaction);            
}

/*
    Requesting card data and storing it.
*/
void fillCardData(ST_transaction * transaction)
{       
    printf("Please Enter CardHolder Name:\n");
    fflush(stdin);
    fgets(transaction->cardHolderData.cardHolderName,25,stdin);
    printf("Please Enter Primary Account Number:\n");
    fflush(stdin);
    fgets(transaction->cardHolderData.primaryAccountNumber,19,stdin);
    printf("Please Enter Card Expiry Date:\n");
    fflush(stdin);
    fgets(transaction->cardHolderData.cardExpirationDate,6,stdin);    
}

