#pragma once 
#include <iostream>
#include <string>
#include <vector>
#include "Bank.h"
#include "Account.h"
using namespace std;

class ATM {

    public: 
          // req1 random함수나 그런걸로 생성
        string primary_bank_name;
        int serial_num;
        string atm_type;
        string language;
        long long atm_cash; 
        long long admin_card;
        long long admin_pw;


        int start_idx;
        long long card_num;
        int pw;
        Account* account;
    
        int withdrawal_count;

        ATM(string,int,string,string,long long,long long,long long);
        void atm_start();
        vector<Bank*> all_bank;
        vector<string> transaction_history;
        
        //English
        void insert_card();
        int account_authorization();

        void deposit(Account*);
        int withdrawl(Account*);
        int transfer(Account*);
        void transaction_menu();
        void session_end();



        //korean
        void insert_card_k();
        int account_authorization_k();
        void deposit_k(Account*);
        int withdrawal_k(Account*);
        int transfer_k(Account*);
        void transaction_menu_k();
        void session_end_k();


        


};