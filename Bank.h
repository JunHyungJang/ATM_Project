#pragma once 
#include <iostream>
#include <string>
#include <vector>
#include "Account.h"



using namespace std;

class Bank{
    public:
        Bank(string);
        int authorization(long long card_num, int pw);
        Account* getaccount(long long card_nut, int pw);
        vector<Account*> account_info;
        int transfer_authorization(long long);
        Account* get_transfer_account(long long);
        string bankname;



};