#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include "Bank.h"
using namespace std;

Bank::Bank(string bname) {
    this->bankname = bname;
}

int Bank::authorization(long long card_num, int pw) {
    // cout << account_info.size() << endl;
    // cout << this->bankname << endl;
    // cout << account_info.size() << endl;
    for(int i = 0; i< account_info.size(); i++){
        // cout << account_info[i]->get_account_number() << endl;
        if(account_info[i]->get_account_number() == card_num){
            if(account_info[i]->get_account_pw() == pw){
                // cout << "account exist" << endl;
                return 1;
            }
            return 2;
        }
        
    }
    return 3;
    
};

Account* Bank::getaccount(long long card_num, int pw) {
    for(int i = 0; i< account_info.size(); i++){
        if(account_info[i]->get_account_number() == card_num){
            if(account_info[i]->get_account_pw() == pw){
                // cout << "authorization success" << endl;
                return account_info[i];
            }
        }
    }
};


Account* Bank::get_transfer_account(long long card_num) {
    for(int i = 0; i< account_info.size(); i++) {
        if(account_info[i]->get_account_number() == card_num) {
            return account_info[i];
        }
    }
}

int Bank::transfer_authorization(long long card_num) {
    // cout << card_num << endl;
    for (int i = 0; i< account_info.size(); i++) {
        if (account_info[i]->get_account_number() == card_num) {
            return 1; // 계좌 있는거임
        }
    }
    // cout << "Transfer account is not correct. There is no account" << endl;
    return 2;
};