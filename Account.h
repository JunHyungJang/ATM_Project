#pragma once 
#include <iostream>
#include <string>
#include <vector>
// #include <Bank.h>
using namespace std;

class Account {
    public:
        Account(string,string,int,long long, long long);
        long long get_account_number();
        int get_account_pw();
        // int get_bank_id();
        void change_account_cash(int);
        long long get_account_cash();
        string get_bank_name();
        string username;
        string bankname;
        long long account_number;
        int account_pw;
        long long account_cash;
        vector<string> history;
        };