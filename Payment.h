/*
    ST_cardData : a struct that represents card data.
*/
typedef struct ST_cardData
{
    unsigned char cardHolderName[25];
    unsigned char primaryAccountNumber[19];
    unsigned char cardExpirationDate[6];
}ST_cardData;

/*
    ST_terminalData : a struct that represents terminal data.
*/
typedef struct ST_terminalData
{
    unsigned char transAmount[8];
    unsigned char maxTransAmount[8];
    unsigned char transactionDate[11];    
}ST_terminalData;


/*
    enumeration to represent a transaction state.
*/
typedef enum EN_transStat
{
    DECLINED,
    APPROVED
}EN_transStat;

/*
    ST_serverData : a struct that represents server data.
*/
typedef struct ST_serverData
{
    EN_transStat transactionStatus;
    unsigned int receiptReferenceNumber;
}ST_serverData;

/*
    ST_accountBalance : a struct that represents a user account's balance.    
*/
typedef struct ST_accountBalance
{
    unsigned char balance[8];
    unsigned char primaryAccountNumber[19];
}ST_accountBalance;

/*
    ST_transaction : a struct that represents transaction data
*/
typedef struct ST_transaction
{
    ST_cardData cardHolderData;
    ST_terminalData transData;
    ST_serverData transResponse;
}ST_transaction;

/*
    Saves user's transaction.
*/
void saveTransactionIntoServer(ST_transaction * transaction);

/*
    Checks the current balance.
*/
void checkBalance(ST_transaction * transaction);

/*
    Requests terminal data.
*/
void fillTerminalData(ST_transaction * transaction);

/*
    Checks expiration data.
*/
void checkExpiryData(ST_transaction * transaction);

/*
    Requests card data.
*/
void fillCardData(ST_transaction * transaction);

/*
    Checks transaction withdrawal amount versus the available balance
*/
void checkAmount(ST_transaction * transaction);


