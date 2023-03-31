#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include "Account.h"

using namespace std;

Account::Account(string bankname_input, string username_input,
                int pw_input,long long account_number_input, long long fund) {
    this->bankname = bankname_input;
    this->username = username_input;
    this->account_number = account_number_input;
    this->account_pw = pw_input;
    this->account_cash = fund;
}


long long  Account::get_account_number() {
    return this->account_number;
}
int Account::get_account_pw() {
    return this->account_pw;
}

string Account::get_bank_name() {
    return this->bankname;
}

void Account::change_account_cash(int num) {
    account_cash+=num;
}

long long Account::get_account_cash() {
    return account_cash;
}


