#pragma once
#pragma once 
#include <iostream>
#include <string>
#include <vector>
#include "Bank.h"
#include "Account.h"
#include "ATM.h"

using namespace std;

class ATM_single : public ATM {
public:
    ATM_single(string, int, string, string, long long, long long, long long);
    void insert_card();
    void atm_start();
    int account_authorization();

    void insert_card_k();
    int account_authorization_k();
};