#include "ATM.h"
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <typeinfo>
#include <fstream>
#include <sstream>

using namespace std;

bool calculator(int n) {
    if (n%1000 < 1000 && n%1000> 0) {
        return true;
    }
   return false;
}

ATM::ATM(string pbank, int num, string tp, string lan, long long cash, long long ad_c, long long ad_pw) {
    this->primary_bank_name = pbank;
    this->serial_num = num;
    this->atm_type = tp;
    this->language = lan;
    this->atm_cash = cash;
    this->admin_card = ad_c;
    this->admin_pw = ad_pw;
    this->start_idx = 0;
    this->withdrawal_count = 0;

}
void ATM::atm_start() {
    int lan = 1;
    cout << "Our bank name is " << this->primary_bank_name << endl;
    cout << "ATM type is "<<this->atm_type<< " and ATM serial number is " << this->serial_num << endl;
    if (this->language == "Bilingual") {
        cout << "We support 2 languages, English and Korean. Please select the language you want to use" << endl;
        cout << "본 ATM기기는 영어와 한글 두 종류의 언어를 지원합니다. 사용하고자 하는 언어를 선택하세요" << endl;
        cout << "1. English(영어)" << endl;
        cout << "2. Korean(한글)" << endl;
        cin >> lan;
    }
    if (lan == 2) {
        insert_card_k();
    }
    else {
    insert_card();
    }
}

void ATM::transaction_menu() {
    ofstream newfile("transaction_history_file.txt");
    for (int i = 0; i < transaction_history.size(); i++) {
        newfile << transaction_history[i] << "\n";
    }
    cout << "----------End admin transaction history----------" << endl;
    newfile.close();
}

void ATM::insert_card() {

    cout << "----------Session start----------" << endl;

    int flag = 0;
    int i = 3;

    while (i > 0) {
        if (flag ==2) {
            cout << "You have " << i << " chance to enter the password" << endl;
            cout << "Enter the password" << endl;
            cin >> this->pw;
            while (cin.fail() == true) {
            cout << "Error! Please insert the password again" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cin >> this->pw;
        }

        }
        else {
             cout << "You have " << i << " chance to enter the account and password" << endl;
            cout << "Insert the card number" << endl;
            cin >> this->card_num;

            while (cin.fail() == true) {
                cout << "You you insert the correct 'number' to access the atm" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cin >> this->card_num;
            }

            cout << "Enter the password" << endl;
            cin >> this->pw;

            while (cin.fail() == true) {
            cout << "Error! Please insert the password again" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cin >> this->pw;
        }
        }
       

        if ((card_num == admin_card) && (pw == admin_pw)) {
            int t;
            cout << "Enter the number" << endl;
            cout << "1. Transaction history" << endl;
            cout << "2. Quit" << endl;
            cin >> t;
            while (cin.fail() == true || t < 0 || t>2) {
                cout << "You should enter correct number" << endl;
                cout << "Enter the number" << endl;
                cout << "1. Transaction history" << endl;
                cout << "2. Quit" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cin >> t;
            }
            if (t == 1) {
                transaction_menu();
            }
            break;


        };
        flag = account_authorization();
        if (flag == 1) {
            session_end();
            break;
        }
        if (flag == 2) {
            cout << "Enter the password again" << endl;
            
        }
        if (flag == 3) {
            cout << "It is incorrect account" << endl;
        }
        if (flag == 4) {
            cout << "Bank name is not corresponded" << endl;
        }
        i--;
    }

}

void ATM::session_end() {
    this->withdrawal_count = 0;
    cout << "----------Session end----------" << endl;
    for (int i = start_idx; i < transaction_history.size(); i++) {
        cout << transaction_history[i] << endl;
    }
    this->start_idx = transaction_history.size();
}

int ATM::account_authorization() {
    
    int check_end;

    int flag;
    int bank_id;
    int id;

    id = all_bank.size();
    
    for (int i = 0; i < id; i++) {
        flag = all_bank[i]->authorization(card_num, pw);
        
        if (flag == 1) {      
            bank_id = i;
            break;
        }
        else if (flag == 2) { 
            return 2;
        }
    }
    if (flag == 3) {

        return 3;
    }

    
    Bank* bank = all_bank[bank_id];
    


    if (flag == 1) {
        Account* a = bank->getaccount(card_num, pw);
        if (atm_type == "Single") {
            if (primary_bank_name != a->bankname) {
                return 4;
            }
        }
        string histo = "Session start with account ";
        histo +=  to_string(a->account_number) + " " + a->username;
        transaction_history.push_back(histo);
        a->history.push_back(histo);
        //cout << histo << endl;
        cout << "Select the number you need" << endl;
        int i;

        do {
            cout << "1. Deposit" << endl;
            cout << "2. Withdrawal" << endl;
            cout << "3. Transfer" << endl;
            cout << "4. Quit" << endl;
            cin >> i;

            while (cin.fail() == true || i <= 0 || i > 4) {
                cout << "You should choose between 1 to 4" << endl;
                cout << "1. Deposit" << endl;
                cout << "2. Withdrawal" << endl;
                cout << "3. Transfer" << endl;
                cout << "4. Quit" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cin >> i;
            }

            switch (i) {
            case 1:
                deposit(a);
                break;
            case 2:
                if (withdrawal_count == 3) {
                    cout << "You exceed the 3 time of withdrawal count" << endl;
                    break;
                }
                else {
                    check_end = withdrawl(a);
                    if (check_end == 2) {
                        return 1;
                    }
                }
                break;
            case 3:
                check_end = transfer(a);
                if (check_end == 2) {
                    return 1;
                }
                break;
            }
        } while (i != 4);

        return 1;

    }
    else {

        cout << "Enter the account and password again" << endl;
        return flag;
    }
}

void ATM::deposit(Account* account) {
    int i = 0;
    int cash1000 = 0;
    int cash5000 = 0;
    int cash10000 = 0;
    int cash50000 = 0;
    int cashcount = 0;
    long long cashsum = 0;
    int checkcount = 0;
    int totalcheckcount = 30;
    long long checksum = 0;
    long long checkprice = 0;
    string histo2 = "";


    cout << "Select the type you want to depoist" << endl;
    cout << "1. Cash" << endl;
    cout << "2. Check" << endl;
    cout << "3. Quit" << endl;
    cin >> i;

    while (cin.fail() == true || i < 0 || i > 3) {
        cout << "Error! You should enter the correct number" << endl;
        cin.clear();
        cin.ignore(100, '\n');
        cout << "Select the type you want to depoist" << endl;
        cout << "1. Cash" << endl;
        cout << "2. Check" << endl;
        cout << "3. Quit" << endl;
        cin >> i;
    }


    switch (i) {
    case 1:
    {
        while (true) {
            cout << "Select the number of cash you want to deposit" << endl;
            cout << "cash 1000, cash 5000, cash 10000, cash 50000" << endl;
            cout << "1000 KRW: " << endl;
            cin >> cash1000;
            while (cin.fail() == true || cash1000 < 0) {
                cout << "Error! You should enter the  correct number" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cout << "Select the number of cash you want to deposit" << endl;
                cout << "1000 KRW: " << endl;
                cin >> cash1000;
            }
            cout << "5000 KRW: " << endl;
            cin >> cash5000;
            while (cin.fail() == true || cash5000 < 0) {
                cout << "Error! You should enter the correct number" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cout << "Select the number of cash you want to deposit" << endl;
                cout << "5000 KRW: " << endl;
                cin >> cash5000;
            }
            cout << "10000 KRW: " << endl;
            cin >> cash10000;
            while (cin.fail() == true || cash10000 < 0) {
                cout << "Error! You should enter the correct number" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cout << "Select the number of cash you want to deposit" << endl;
                cout << "10000 KRW: " << endl;
                cin >> cash10000;
            }
            cout << "50000 KRW: " << endl;
            cin >> cash50000;
            while (cin.fail() == true || cash50000 < 0) {
                cout << "Error! You should enter the correct number" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cout << "Select the number of cash you want to deposit" << endl;
                cout << "50000 KRW: " << endl;
                cin >> cash50000;
            }

            cashcount = cash1000 + cash5000 + cash10000 + cash50000;
            if (cashcount <= 50) {
                break;
            }
            else {
                cout << "Too many number of cash were inserted" << endl;
            }
        }
        cashsum = cash1000 * 1000 + cash5000 * 5000 + cash10000 * 10000 + cash50000 * 50000;
        cout << "Your account bank name is " << account->get_bank_name() << endl;
        cout << "Total cash sum is " << cashsum << endl;
        // cout << atm_bank<< endl;

        if (primary_bank_name != account->get_bank_name()) {

            int k;
            cout << "There will be deposit fee KRW 1000" << endl;
            cout << "Will you proceed the deposit?" << endl;
            cout << "1. Yes" << endl;
            cout << "2. No" << endl;
            cin >> k;
            while (cin.fail() == true || k < 0 || k > 2) {
                cout << "Error! You should enter the correct number" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cout << "There will be deposit fee KRW 1000" << endl;
                cout << "Will you proceed the deposit?" << endl;
                cout << "1. Yes" << endl;
                cout << "2. No" << endl;
                cin >> k;
            }
            if (k == 1) {
                this->atm_cash += cashsum;
                account->change_account_cash(cashsum - 1000);
                cout << "Deposit Success" << endl;
                histo2 = "Deposit: Cash deposit: " + to_string(cashsum - 1000) + " KRW into account";
                transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo2);
                account->history.push_back(histo2);

            }
            if (k == 2) {
                cout << "Take your money" << endl;
                cout << "----------End Deposit----------" << endl;
            }
            cash1000, cash5000, cash10000, cash50000, cashcount, cashsum = 0;

        }
        else {
            int k;
            cout << "Will you proceed the deposit?" << endl;
            cout << "1. Yes" << endl;
            cout << "2. No" << endl;
            cin >> k;
            while (cin.fail() == true || k < 0 || k > 2) {
                cout << "Error! You should enter the correct number" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cout << "Will you proceed the deposit?" << endl;
                cout << "1. Yes" << endl;
                cout << "2. No" << endl;
                cin >> k;
            }
            if (k == 1) {
                this->atm_cash += cashsum;
                account->change_account_cash(cashsum);
                histo2 = "Deposit: Cash deposit: " + to_string(cashsum) + " KRW into account";
                transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo2);
                account->history.push_back(histo2);
                cout << "----------Deposit Success----------" << endl;
            }
            if (k == 2) {
                cout << "take your money" << endl;
                cout << "----------End Deposit----------" << endl;
            }
            cash1000, cash5000, cash10000, cash50000, cashcount, cashsum = 0;
        }
       //cout << "Deposit Success" << endl;
        cout << "Your account has " << account->get_account_cash() << endl;
        cout << "ATM has " << this->atm_cash << endl;

        break;
    }

    case 2:
    {
            int j;
            while (true) {
                cout << "The total number of check you can deposit is " << totalcheckcount << endl;
                cout << "Hom many check(count of check) do you want to depoist" << endl;
                cin >> checkcount;
                while (totalcheckcount-checkcount<0) {
                    
                    cout << "The total number of check you can deposit is 30, but you exceed. Please enter the count of check under " << totalcheckcount << endl;
                    cin >> checkcount;
                    
                }
                totalcheckcount -=checkcount;
                cout << "Enter the price of check" << endl;
                cin >> checkprice;
                checksum += checkcount * checkprice;
                cout << "The total price you will depoist is " << checksum << "KRW" << endl;
                cout << "Select the number " << endl;
                cout << "1. Continue to deposit the check" << endl;
                cout << "2. Finish to deposit the check" << endl;
                cin >> j;
                if (j==2) 
                    break;
                
            }
        
        int k = 0;
    
        cout << "Will you proceed deposit?" << endl;
        cout << "1. Yes" << "\t" << "2. No" << endl;
        cin >> k;
        while (cin.fail() == true || k < 0 || k > 2) {
            cout << "Error! You should enter the correct number" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Will you proceed deposit?" << endl;
            cout << "1. Yes" << "\t" << "2. No" << endl;
            cin >> k;
        }
        if (k == 1) {
            account->change_account_cash(checksum);
            histo2 = "Deposit: Check deposit: " + to_string(checksum) + " KRW into account";
            transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo2);
            account->history.push_back(histo2);
            cout << "Deposit Success" << endl;
            cout << "Your account has " << account->get_account_cash()<<"KRW"<< endl;
            
            cout << "ATM has " << this->atm_cash<<"KRW"<< endl;
        }
        if (k == 2) {
            cout << "Take your check" << endl;
            cout << "End Deposit" << endl;
            checksum = 0;
            checkcount = 0;
        }
        

        break;
    }

    case 3:
    {
        cout << "Deposit End" << endl;
        // session_end();
        break;
    }

    }
}


int ATM::withdrawl(Account* account) {
    this->withdrawal_count++;

    int withdrawal_cash = 0;
    string histo3 = "";
    // cout << "How much money you want to withdrawl?" << endl;
    // cin>> withdrawal_cash;

    int fee;

    if(primary_bank_name!=  account->get_bank_name()) {
        fee = 2000;
    }
    else{
        fee = 1000;
    }
    

    int i = 0;
    do {
        cout << "How much money do you want to withdraw?" << endl;
        cin >> withdrawal_cash;
        while (cin.fail() == true || withdrawal_cash < 0 || calculator(withdrawal_cash))  {
            cout << "Error! You should enter the correct number" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "How much money do you want to withdraw?" << endl;
            cin >> withdrawal_cash;
        }
        if (withdrawal_cash <= 500000 && atm_cash >= withdrawal_cash && account->get_account_cash() >= withdrawal_cash + fee) {
            break;
        }
        else if (withdrawal_cash > 500000) {
            cout << "Money can't exceed 500,000 KRW" << endl;
        }
        else if (atm_cash < withdrawal_cash) {
            cout << "Atm does not have enough money" << endl;
            return 2;
        }
        else if (account->get_account_cash() < withdrawal_cash+fee) {
            cout << "Your account does not have enough money" << endl;
            return 2;
        }
        cout << "Please enter number " << endl;
        cout << "1. Proceed the withdrawal session" << endl;
        cout << "2. End the withdrawal session" << endl;
        cin >> i;
        while (cin.fail() == true || i < 0 || i > 2) {
            cout << "Error! You should enter the correct number" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "1. Proceed the withdrawal session" << endl;
            cout << "2. End the withdrawal session" << endl;
            cin >> i;
        }
        if (i == 2) {
            cout << "----------End withdrawal----------" << endl;
            return 1;
        }
        withdrawal_cash = 0;
    } while (true);

    // primary fee 1000
    if (primary_bank_name != account->get_bank_name()) {

        int k;
        cout << "There will be withdrawal fee KRW 2000 " << endl;
        cout << "Will you proceed the withdrawal" << endl;
        cout << "1. Yes" << endl;
        cout << "2. No" << endl;
        cin >> k;
        while (cin.fail() == true || k < 0 || k > 2) {
            cout << "Error! You should enter the correct number" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "There will be withdrawal fee KRW 2000 " << endl;
            cout << "Will you proceed the withdrawal" << endl;
            cout << "1. Yes" << endl;
            cout << "2. No" << endl;
            cin >> k;
        }
        if (k == 1) {
            this->atm_cash -= withdrawal_cash;
            account->change_account_cash(-(withdrawal_cash + 2000));
            histo3 = "Withdrawl: Cash Withdrawl: " + to_string((withdrawal_cash + 2000)) + " KRW from account";
            transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo3);
            account->history.push_back(histo3);
            cout << "Withdrawl Success" << endl;
            cout << "ATM has " << this->atm_cash << "KRW" << endl;
            cout << "Your account has " << account->get_account_cash() << "KRW" << endl;
        }
        if (k == 2) {
            cout << "End Withdrawal" << endl;
            return 1;
        }
    }
    else {
        int k;
        cout << "There will be withdrawal fee KRW 1000 " << endl;
        cout << "Will you proceed the withdrawal" << endl;
        cout << "1. Yes" << endl;
        cout << "2. No" << endl;
        cin >> k;
        while (cin.fail() == true || k < 0 || k > 2) {
            cout << "Error! You should enter the correct number" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "There will be withdrawal fee KRW 1000 " << endl;
            cout << "Will you proceed the withdrawal" << endl;
            cout << "1. Yes" << endl;
            cout << "2. No" << endl;
            cin >> k;
        }

        if (k == 1) {
            this->atm_cash -= withdrawal_cash;
            account->change_account_cash(-(withdrawal_cash + 1000));
            histo3 = "Withdrawl: Cash Withdrawl: " + to_string((withdrawal_cash + 1000)) + " KRW from account";
            transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo3);
            account->history.push_back(histo3);
            cout << "Withdrawl Success" << endl;
            cout << "ATM has " << this->atm_cash << "KRW" << endl;
            cout << "Your account has " << account->get_account_cash() << "KRW" << endl;
        }
        if (k == 2) {
            cout << "----------End withdrawal----------" << endl;
            return 1;
        }
    }



}

int ATM::transfer(Account* account) {
    int id = 0;
    long long transfer_number = 0;
    int bank_id = 0;
    int flag = 0;
    int i = 0;
    int j = 0;
    Account* transfer_account = new Account("Daegu", "jinwoo", 1234, 444444444444, 50000);
    int transfer_fee = 0;
    int transfer_cash = 0;

    int cash1000 = 0;
    int cash5000 = 0;
    int cash10000 = 0;
    int cash50000 = 0;
    int cashcount = 0;
    int cashsum = 0;

    int checksum = 0;
    string histo4 = "";

    cout << "Please select the type of transfer " << endl;
    cout << "1. Cash transfer" << endl;
    cout << "2. Account transfer" << endl;
    cin >> i;
    while (cin.fail() == true || i < 0 || i > 2) {
        cout << "Error! You should enter the correct number" << endl;
        cin.clear();
        cin.ignore(100, '\n');
        cout << "Please select the type of transfer " << endl;
        cout << "1. Cash transfer" << endl;
        cout << "2. Account transfer" << endl;
        cin >> i;
    }
    int k = 0;

    do {
        cout << "Plase enter the account number to transfer" << endl;
        cin >> transfer_number;
        while (cin.fail() == true || transfer_number < 0 || transfer_number == account->account_number)  {
            cout << "Error! You should enter the correct number" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Please enter the account number to transfer" << endl;
            cin >> transfer_number;
        }

     
        for (int i = 0; i < all_bank.size(); i++) {
            flag = all_bank[i]->transfer_authorization(transfer_number);
            if (flag == 1) {
                Bank* bank = all_bank[i];               
                transfer_account = bank->get_transfer_account(transfer_number);
                k = 1;
                break;
            }

            if (i == (all_bank.size() - 1) && flag == 2) {
                
                
                cout << "Transfer account is not correct " << endl;
                continue;
            }

        }
        if (k == 1) {
            break;
        }
    } while (true);



    // calculating transfer fee
    if (primary_bank_name == account->get_bank_name()) {
        if (account->get_bank_name() == transfer_account->get_bank_name()) {
            cout << "Transfer fee between primary banks is 2000 KRW " << endl;
            transfer_fee = 2000;
        }
        else {
            transfer_fee = 3000;
            cout << "Transfer fee between primary and non-primary banks is 3000 KRW " << endl;
        }

    }
    else {
        if (primary_bank_name == transfer_account->get_bank_name()) {
            transfer_fee = 3000;
            cout << "Transfer fee between primary and non-primary banks is 3000 KRW " << endl;
        }
        else {
            transfer_fee = 4000;
            cout << "Transfer fee between non-primary banks is 4000 KRW " << endl;
        }
    }

    // cash transfer 
    if (i == 1) {

        cout << "Enter the amount of money you want to transfer" << endl;
        cin >> transfer_cash;
        while (cin.fail() == true || transfer_cash < 0 || calculator(transfer_cash)) {
            cout << "Error! You should enter the correct number" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Enter the amount of money you want to transfer" << endl;
            cin >> transfer_cash;
        }

        // cout << "Insert the number of money you want to transfer including transfer fee" << endl;
       
        cout << "Your total transfer money will be " << transfer_cash << "KRW and transfer fee will be " << transfer_fee << " KRW" << endl;
        cout << "Will you proceed the transfer" << endl;
        cout << "1. Yes" << endl;
        cout << "2. No" << endl;
        cin >> j;

        while (cin.fail() == true || j < 0 || j > 2) {
            cout << "Error! You should type correct number" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "Your total transfer money will be " << transfer_cash << "KRW and transfer fee will be " << transfer_fee << " KRW" << endl;
            cout << "Will you proceed the transfer" << endl;
            cout << "1. Yes" << endl;
            cout << "2. No" << endl;
            cin >> j;
        }

        if (j == 1) {
            while (true) {
                cout << "Total transfer money is " << transfer_cash + transfer_fee << endl;
                cout << "Select number of cash you will insert" << endl;
                cout << "cash1000, cash5000, cash10000, cash50000" << endl;
                cout << "1000 KRW: " << endl;
                cin >> cash1000;
                while (cin.fail() == true || cash1000 < 0) {
                    cout << "Error! You should type correct number" << endl;
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout << "Select the number of cash you want to transfer" << endl;
                    cout << "1000 KRW: " << endl;
                    cin >> cash1000;
                }
                cout << "5000 KRW: " << endl;
                cin >> cash5000;
                while (cin.fail() == true || cash5000 < 0) {
                    cout << "Error! You should type correct number" << endl;
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout << "Select the number of cash you want to transfer" << endl;
                    cout << "5000 KRW: " << endl;
                    cin >> cash5000;
                }
                cout << "10000 KRW: " << endl;
                cin >> cash10000;
                while (cin.fail() == true || cash10000 < 0) {
                    cout << "Error! You should type correct number" << endl;
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout << "Select the number of cash you want to transfer" << endl;
                    cout << "10000 KRW: " << endl;
                    cin >> cash10000;
                }
                cout << "50000 KRW: " << endl;
                cin >> cash50000;
                while (cin.fail() == true || cash50000 < 0) {
                    cout << "Error! You should type correct number" << endl;
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout << "Select the number of cash you want to transfer" << endl;
                    cout << "50000 KRW: " << endl;
                    cin >> cash50000;
                }

                cashcount = cash1000 + cash5000 + cash10000 + cash50000;
                if (cashcount <= 50) {
                    cashsum = cash1000 * 1000 + cash5000 * 5000 + cash10000 * 10000 + cash50000 * 50000;

                    if (cashsum == transfer_cash + transfer_fee) {
                        transfer_account->change_account_cash(transfer_cash);
                        this->atm_cash += cashsum;
                        transfer_account->change_account_cash(transfer_cash);
                       histo4 = "Transfer: Cash Transfer: " + to_string((transfer_fee + transfer_cash)) + " KRW was inserted and Cash: " 
                            + to_string(transfer_cash) + " KRW was send from " + to_string(account->account_number)
                            + " " + account->username + " " + account->bankname + " to "  
                            + to_string(transfer_account->account_number) + " " + transfer_account->username
                            + " " + transfer_account->bankname;
                        transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo4);
                        account->history.push_back(histo4);
                        transfer_account->history.push_back(histo4);
                        cout << "----------Transfer Success----------" << endl;
                        // delete transfer_account; // memory deallocation
                        break;
                    }
                    else {
                        cout << "Inserted cash is incorrect please put cash again" << endl;
                    }
                }
                else {
                    cout << "Too many cash in slot" << endl;
                }
            }

        }


        else if (j == 2) {
            cout << "----------End Transfer----------" << endl;
            // delete transfer_account;
            return 1;
        }

    }
    // account transfer
    if (i == 2) {
        while (true) {
            cout << "Enter the amount of cash you want to transfer" << endl;
            cin >> transfer_cash;
            while (cin.fail() == true || transfer_cash < 0) {
                cout << "Error! You should type correct number" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cout << "Enter the amount of cash you want to transfer" << endl;
                cin >> transfer_cash;
            }
            cout << "your total transfer money will be " << transfer_cash << " KRW and transfer fee will be " << transfer_fee << " KRW" << endl;
            cout << "WIll you proceed the transfer" << endl;
            cout << "1. yes" << endl;
            cout << "2. no" << endl;
            cin >> j;
            while (cin.fail() == true || j < 0 || j > 2) {
                cout << "Error! You should type correct number" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cout << "your total transfer money will be " << transfer_cash << " KRW and transfer fee will be " << transfer_fee << " KRW" << endl;
                cout << "WIll you proceed the transfer" << endl;
                cout << "1. yes" << endl;
                cout << "2. no" << endl;
                cin >> j;
            }

            if (j == 1) {
                if (account->get_account_cash() < transfer_cash + transfer_fee) {
                    cout << "No money in your account" << endl;
                    cout << "your account has " << account->get_account_cash() << " KRW but total moeny you need want to transfer is " << transfer_cash + transfer_fee << " KRW" << endl;
                    return 2;
                }
                else {
                    account->change_account_cash(-(transfer_fee + transfer_cash));
                    transfer_account->change_account_cash(transfer_cash);
                    histo4 = "Transfer: Account Transfer: " + to_string((transfer_fee + transfer_cash)) + " KRW was withdrawn from account " 
                        + to_string(account->account_number)
                        + " " + account->username + " " + account->bankname + " and " + to_string(transfer_cash) + " KRW was deposited into "
                        + to_string(transfer_account->account_number) + " " + transfer_account->username
                        + " " + transfer_account->bankname;
                    transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo4);
                    account->history.push_back(histo4);
                    transfer_account->history.push_back(histo4);
                    cout << "Transfer Success" << endl;
                    // delete transfer_account;
                    break;
                }
            }
            else if (j == 2) {
                cout << "End the transfer transaction" << endl;
                // delete transfer_account;
                return 1;
            }
        }
    }
}

void ATM::insert_card_k() {
    cout << "----------세션 시작----------" << endl;

    int flag = 0;
    int i = 3;

    while (i > 0) {
        if (flag ==2) {
            cout<< "비밀번호를 입력할 " << i << "번의 기회가 있습니다" << endl;
            cout << "비밀번호를 입력하세요" << endl;
            cin >> this->pw;
            while (cin.fail() == true) {
            cout << "오류! 비밀번호를 다시 입력하시오" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cin >> this->pw;
        }

        }
        else {
            cout<< "비밀번호와 계좌를 입력할 " << i << "번의 기회가 있습니다" << endl;
            cout << "카드 번호를 입력하세요" << endl;
            cin >> this->card_num;

            while (cin.fail() == true) {
                cout << "ATM기에는 올바른 숫자를 기입해야 합니다." << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cin >> this->card_num;
            }

            cout << "카드 번호를 입력하세요" << endl;
            cin >> this->pw;

            while (cin.fail() == true) {
            cout << "에러! 비밀번호를 다시 입력하세요" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cin >> this->pw;
        }
        }
       

        if ((card_num == admin_card) && (pw == admin_pw)) {
            int t;
            cout << "원하는 선택지의 번호를 입력하세요" << endl;
            cout << "1. 거래메뉴" << endl;
            cout << "2. 나가기" << endl;
            cin >> t;
            while (cin.fail() == true || t < 0 || t>2) {
                cout << "올바른 숫자를 입력해주세요" << endl;
                cout << "원하는 선택지의 번호를 입력하세요" << endl;
                cout << "1. 거래메뉴" << endl;
                cout << "2. 나가기" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cin >> t;
            }
            if (t == 1) {
                transaction_menu_k();
            }
            break;


        };
        flag = account_authorization_k();
        if (flag == 1) {
            session_end();
            break;
        }
        if (flag == 2) {
            cout << "비밀번호를 다시 입력하세요" << endl;
            
        }
        if (flag == 3) {
            cout << "계좌정보가 일치하지 않습니다" << endl;
        }
        if (flag == 4) {
            cout << "은행이름이 일치하지 않습니다" << endl;
        }
        i--;
    }

}

void ATM::session_end_k() {
    this->withdrawal_count = 0;
    cout << "----------세션 종료----------" << endl;
    for (int i = start_idx; i < transaction_history.size(); i++) {
        cout << transaction_history[i] << endl;
    }
    this->start_idx = transaction_history.size();
};

int ATM::account_authorization_k() {
int check_end;

    int flag;
    int bank_id;
    int id;

    id = all_bank.size();
    
    for (int i = 0; i < id; i++) {
        flag = all_bank[i]->authorization(card_num, pw);
        
        if (flag == 1) {      
            bank_id = i;
            break;
        }
        else if (flag == 2) { 
            return 2;
        }
    }
    if (flag == 3) {

        return 3;
    }

    
    Bank* bank = all_bank[bank_id];
    


    if (flag == 1) {
        Account* a = bank->getaccount(card_num, pw);
        if (atm_type == "Single") {
            if (primary_bank_name != a->bankname) {
                return 4;
            }
        }

        string histo = a->username + "의 계좌 " + to_string(a->account_number) + " 에서 세션이 시작됩니다" ;
        transaction_history.push_back(histo);
        a->history.push_back(histo);
        //cout << histo << endl;
        cout << "원하시는 선택지의 번호를 입력하세요" << endl;
        int i;

        do {
            cout << "1. 예금" << endl;
            cout << "2. 출금" << endl;
            cout << "3. 송금" << endl;
            cout << "4. 나가기" << endl;
            cin >> i;

            while (cin.fail() == true || i <= 0 || i > 4) {
                cout << "선택지 1에서 4 사이의 숫자를 고르시오" << endl;
                cout << "1. 예금" << endl;
                cout << "2. 출금" << endl;
                cout << "3. 송금" << endl;
                cout << "4. 나가기" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cin >> i;
            }

            switch (i) {
            case 1:
                deposit_k(a);
                break;
            case 2:
                if (withdrawal_count == 3) {
                    cout << "3회 이상 출금을 하셨기 떄문에 이용하실 수 없습니다." << endl;
                    break;
                }
                else {
                    check_end = withdrawal_k(a);
                    if (check_end == 2) {
                        return 1;
                    }
                }
                break;
            case 3:
                check_end = transfer_k(a);
                if (check_end == 2) {
                    return 1;
                }
                break;
            }
        } while (i != 4);

        return 1;

    }
    else {

        cout << "계좌와 비밀번호를 다시 입력해주세요" << endl;
        return flag;
    }
}


 void ATM::deposit_k(Account* account) {
    int i = 0;
    int cash1000 = 0;
    int cash5000 = 0;
    int cash10000 = 0;
    int cash50000 = 0;
    int cashcount = 0;
    long long cashsum = 0;
    int checkcount = 0;
    int totalcheckcount = 30;
    long long checksum = 0;
    long long checkprice = 0;
    string histo2 = "";


    cout << "예금의 종류를 선택하세요" << endl;
    cout << "1. 현금" << endl;
    cout << "2. 수표" << endl;
    cout << "3. 나가기" << endl;
    cin >> i;

    while (cin.fail() == true || i < 0 || i > 3) {
        cout << "오류! 올바른 번호를 입력하세요" << endl;
        cin.clear();
        cin.ignore(100, '\n');
        cout << "예금의 종류를 선택하세요" << endl;
        cout << "1. 현금" << endl;
        cout << "2. 수표" << endl;
        cout << "3. 나가기" << endl;
        cin >> i;
    }


    switch (i) {
    case 1:
    {
        while (true) {
            cout << "예금하실 지폐의 수를 선택하세요" << endl;
            cout << "1000원, 5000원, 10000원, 50000원" << endl;
            cout << "1000 KRW: " << endl;
            cin >> cash1000;
            while (cin.fail() == true || cash1000 < 0) {
                cout << "오류! 올바른 번호를 입력하세요" << endl;

                cin.clear();
                cin.ignore(100, '\n');
            cout << "예금하실 지폐의 수를 선택하세요" << endl;
                cout << "1000 KRW: " << endl;
                cin >> cash1000;
            }
            cout << "5000 KRW: " << endl;
            cin>> cash5000;
             while (cin.fail() == true || cash5000 < 0) {
                cout << "오류! 올바른 번호를 입력하세요" << endl;
                cin.clear();
                cin.ignore(100, '\n');
            cout << "예금하실 지폐의 수를 선택하세요" << endl;
                cout << "5000 KRW: " << endl;
                cin >> cash5000;
            }
            cout << "10000 KRW: " << endl;
            cin >> cash10000;
            while (cin.fail() == true || cash10000 < 0) {
                cout << "오류! 올바른 번호를 입력하세요" << endl;
                cin.clear();
                cin.ignore(100, '\n');
            cout << "예금하실 지폐의 수를 선택하세요" << endl;
                cout << "10000 KRW: " << endl;
                cin >> cash10000;
            }
            cout << "50000 KRW: " << endl;
            cin >> cash50000;
            while (cin.fail() == true || cash50000 < 0) {
                cout << "오류! 올바른 번호를 입력하세요" << endl;
                cin.clear();
                cin.ignore(100, '\n');
            cout << "예금하실 지폐의 수를 선택하세요" << endl;
                cout << "50000 KRW: " << endl;
                cin >> cash50000;
            }
       
            cashcount = cash1000 + cash5000 + cash10000 + cash50000;
            if (cashcount <= 50) {

                
                break;
            }
            else {
                cout << "너무 많은 지폐가 삽입되었습니다" << endl;
            }
        }




        

        cashsum = cash1000 * 1000 + cash5000 * 5000 + cash10000 * 10000 + cash50000 * 50000;
        
        cout << "당신 계좌의 은행이름은 " << account->get_bank_name() << " 입니다" << endl;
        cout << "전체 금액은 " << cashsum << " 입니다"<< endl;
        // cout << atm_bank<< endl;

        if (primary_bank_name != account->get_bank_name()) {

            int k;
            cout << "예금 수수료KRW 1000가 있습니다" << endl;
            cout << "예금을 진행하시겠습니까?" << endl;
            cout << "1. 네" << endl;
            cout << "2. 아니오" << endl;
            cin >> k;
            while (cin.fail() == true || k < 0 || k > 2) {
                cout << "오류! 올바른 번호를 입력하세요" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cout << "예금 수수료KRW 1000가 있습니다" << endl;
                cout << "예금을 진행하시겠습니까?" << endl;
                cout << "1. 네" << endl;
                cout << "2. 아니오" << endl;
                cin >> k;
            }
            if (k == 1) {
                this->atm_cash += cashsum;
                account->change_account_cash(cashsum - 1000);
                cout << "예금성공" << endl;
                histo2 = "예금: 현금 예금: " + to_string(cashsum - 1000) + " KRW";
                transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo2);
                account->history.push_back(histo2);

            }
            if (k == 2) {
                cout << "돈을 다시 가져가세요" << endl;
                cout << "----------예금 종료----------" << endl;
            }
            cash1000, cash5000, cash10000, cash50000, cashcount, cashsum = 0;

        }
        else {
            int k;
            cout << "예금을 계속 진행하시겠습니까?" << endl;
            cout << "1. 네" << endl;
            cout << "2. 아니오" << endl;
            cin >> k;
            while (cin.fail() == true || k < 0 || k > 2) {
                cout << "오류! 올바른 번호를 입력하세요" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cout << "예금을 계속 진행하시겠습니까??" << endl;
                cout << "1. 네" << endl;
                cout << "2. 아니오" << endl;
                cin >> k;
            }
            if (k == 1) {
                this->atm_cash += cashsum;
                account->change_account_cash(cashsum);
                histo2 = "예금: 현금 예금: " + to_string(cashsum) + " KRW ";
                transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo2);
                account->history.push_back(histo2);
                cout << "----------예금 성공----------" << endl;
            }
            if (k == 2) {
                cout << "돈을 가져가세요" << endl;
                cout << "----------예금 종료----------" << endl;
            }
            cash1000, cash5000, cash10000, cash50000, cashcount, cashsum = 0;
        }
       //cout << "Deposit Success" << endl;
       
        cout << "당신의 계좌에는" << account->get_account_cash() <<"KRW 가 들어있습니다."<< endl;
        cout << "ATM기는 현재" << this->atm_cash << "KRW를 가지고 있습니다." << endl;

        break;
    }

    case 2:
    {
            int j;
            while (true) {
                cout << "당신이 예금할 수 있는 수표의 수는 " << totalcheckcount << " 입니다" <<endl;
                cout << "예금 하고싶은 수표의 수를 입력하세요" << endl;
                cin >> checkcount;
                while (totalcheckcount-checkcount<0) {
                    
                    cout << "예금할 수 있는 최대 지폐의 수는 30장이지만, 초과하였습니다." << totalcheckcount << " 아래의 수를 입력해주세요" << endl;
                    cin >> checkcount;
                    
                }
                totalcheckcount -=checkcount;
                cout << "수표의 가격을 입력하세요" << endl;
                cin >> checkprice;
                checksum += checkcount * checkprice;
                cout << "예금하실 전체 금액은" << checksum << "KRW 입니다" << endl;
                 cout << "원하시는 선택지의 번호를 입력하세요" << endl;

                cout << "1. 수표 예금을 계속 진행하기" << endl;
                cout << "2. 수표 예금 종료하기" << endl;
                cin >> j;
                if (j==2) 
                    break;
                
            }
        
        int k = 0;
    
        cout << "예금을 계속하시겠습니까?" << endl;
        cout << "1. 네" << "\t" << "2. 아니오" << endl;
        cin >> k;
        while (cin.fail() == true || k < 0 || k > 2) {
            cout << "오류! 올바른 번호를 입력하세요" << endl;

            cin.clear();
            cin.ignore(100, '\n');
            cout << "예금을 계속하시겠습니까?" << endl;
            cout << "1. 네" << "\t" << "2. 아니오" << endl;
            cin >> k;
        }
        if (k == 1) {
            account->change_account_cash(checksum);
            histo2 = "예금: 수표 예금: " + to_string(checksum) + " KRW";
            transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo2);
            account->history.push_back(histo2);
              cout << "예금 성공" << endl;
            cout << "계좌에 잔액은 " << account->get_account_cash()<<"KRW 있습니다"<< endl;
            
            cout << "ATM의 잔액은" << this->atm_cash<<"KRW 있습니다"<< endl;
        }
        if (k == 2) {
            cout << "돈을 가져가시오" << endl;
            cout << "예금 종료" << endl;
            checksum = 0;
            checkcount = 0;
        }
      

        break;
    }

    case 3:
    {
        cout << "예금 종료" << endl;
        // session_end_k();
        break;
    }

    }
 };

int ATM::withdrawal_k(Account* account) {
    this->withdrawal_count++;

    int withdrawal_cash = 0;
    string histo3 = "";
    // cout << "How much money you want to withdrawl?" << endl;
    // cin>> withdrawal_cash;

    int fee;

    if(primary_bank_name!=  account->get_bank_name()) {
        fee = 2000;
    }
    else{
        fee = 1000;
    }
    

    int i = 0;
    do {
        cout << "얼마나 많은 돈을 인출하시겠습니까?" << endl;
        cin >> withdrawal_cash;
        while (cin.fail() == true || withdrawal_cash < 0 || calculator(withdrawal_cash))  {
            cout << "오류! 올바른 번호를 입력하시오" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "얼마나 많은 돈을 인출하시겠습니까?" << endl;
            cin >> withdrawal_cash;
        }
        //cout << "helloworld" << endl;
        cout << withdrawal_cash << endl;
        cout << fee <<endl;
        if (withdrawal_cash <= 500000 && atm_cash >= withdrawal_cash && account->get_account_cash() >= withdrawal_cash + fee) {
            break;
        }
        else if (withdrawal_cash > 500000) {
            cout << "500,000 KRW 이상 인출하실 수 없습니다." << endl;
        }
        else if (atm_cash < withdrawal_cash) {
            cout << "ATM에 충분한 돈이 있지 않습니다." << endl;
            return 2;
        }
        else if (account->get_account_cash() < withdrawal_cash+fee) {
            cout << "계좌에 충분한 돈이 있지 않습니다." << endl;
            return 2;
        }
        cout << "해당하는 번호를 고르시오 " << endl;
        cout << "1. 인출을 계속한다." << endl;
        cout << "2. 인출을 끝낸다." << endl;
        cin >> i;
        while (cin.fail() == true || i < 0 || i > 2) {
            cout << "오류! 올바른 번호를 입력하세요" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "1. 인출을 계속한다" << endl;
            cout << "2. 인출을 끝낸다" << endl;
            cin >> i;
        }
        if (i == 2) {
            cout << "----------인출 종료----------" << endl;
            return 1;
        }
        withdrawal_cash = 0;
    } while (true);

    // primary fee 1000
    if (primary_bank_name != account->get_bank_name()) {

        int k;
        cout << "KRW 2000의 인출 수수료가 있습니다" << endl;
        cout << "인출을 계속하시겠습니까?" << endl;
        cout << "1. 네" << endl;
        cout << "2. 아니오" << endl;
        cin >> k;
        while (cin.fail() == true || k < 0 || k > 2) {
            cout << "오류! 올바른 번호를 입력하세요" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "KRW 2000의 인출 수수료가 있습니다" << endl;
            cout << "인출을 계속하시겠습니까?" << endl;
            cout << "1. 네" << endl;
            cout << "2. 아니오" << endl;
            cin >> k;
        }
        if (k == 1) {
            this->atm_cash -= withdrawal_cash;
            account->change_account_cash(-(withdrawal_cash + 2000));
            histo3 = "인출: 현금인출: " + to_string((withdrawal_cash + 2000)) + " KRW";
            transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo3);
            account->history.push_back(histo3);
            cout << "인출성공" << endl;
            cout << "ATM 에는" << this->atm_cash << "KRW 이 있습니다" << endl;
            cout << "계좌에는 " << account->get_account_cash() << "KRW 이 있습니다" << endl;
        }
        if (k == 2) {
            cout << "인출 종료" << endl;
            return 1;
        }
    }
    else {
        int k;
        cout << "KRW 1000의 인출 수수료가 있습니다" << endl;
        cout << "인출을 계속 하시겠습니까?" << endl;
        cout << "1. 네" << endl;
        cout << "2. 아니오" << endl;
        cin >> k;
        while (cin.fail() == true || k < 0 || k > 2) {
            cout << "오류! 올바른 번호를 입력하시오" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "KRW 1000dml 인출 수수료가 있습니다" << endl;
            cout << "인출을 계속 하시겠습니까?" << endl;
            cout << "1. 네" << endl;
            cout << "2. 아니오" << endl;
            cin >> k;
        }

        if (k == 1) {
            this->atm_cash -= withdrawal_cash;
            account->change_account_cash(-(withdrawal_cash + 1000));
            histo3 = "인출: 현금인출" + to_string(-(withdrawal_cash + 1000)) + " KRW";
            transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo3);
            account->history.push_back(histo3);
            cout << "인출 성공" << endl;
            cout << "ATM 에는 " << this->atm_cash << "KRW 이 있습니다" << endl;
            cout << "계좌에는 " << account->get_account_cash() << "KRW 이 있습니다." << endl;
        }
        if (k == 2) {
            cout << "----------인출 종료----------" << endl;
            return 1;
        }
    }

};
int ATM::transfer_k(Account* a) {
int id = 0;
    long long transfer_number = 0;
    int bank_id = 0;
    int flag = 0;
    int i = 0;
    int j = 0;
    Account* transfer_account = new Account("Daegu", "jinwoo", 1234, 444444444444, 50000);
    int transfer_fee = 0;
    int transfer_cash = 0;

    int cash1000 = 0;
    int cash5000 = 0;
    int cash10000 = 0;
    int cash50000 = 0;
    int cashcount = 0;
    int cashsum = 0;

    int checksum = 0;
    string histo4 = "";

    cout << "송금의 종류를 선택하시오 " << endl;
    cout << "1. 현금 송금" << endl;
    cout << "2. 계좌 송금" << endl;
    cin >> i;
    while (cin.fail() == true || i < 0 || i > 2) {
        cout << "오류! 올바른 숫자를 입력하시오" << endl;
        cin.clear();
        cin.ignore(100, '\n');
        cout << "송금의 종류를 선택하시오 " << endl;
        cout << "1. 현금 송금" << endl;
        cout << "2. 계좌 송금" << endl;
        cin >> i;
    }
    int k = 0;

    do {
        cout << "송금하실 계좌의 번호를 입력하시오" << endl;
        cin >> transfer_number;
        while (cin.fail() == true || transfer_number < 0 || transfer_number == account->account_number)  {
        cout << "오류! 올바른 숫자를 입력하시오" << endl;
            cin.clear();
            cin.ignore(100, '\n');
        cout << "송금하실 계좌의 번호를 입력하시오" << endl;
            cin >> transfer_number;
        }

     
        for (int i = 0; i < all_bank.size(); i++) {
            flag = all_bank[i]->transfer_authorization(transfer_number);
            if (flag == 1) {
                Bank* bank = all_bank[i];               
                transfer_account = bank->get_transfer_account(transfer_number);
                k = 1;
                break;
            }

            if (i == (all_bank.size() - 1) && flag == 2) {
                
                
                cout << "송금하실 계좌번호가 일치하지 않습니다 " << endl;
                continue;
            }

        }
        if (k == 1) {
            break;
        }
    } while (true);



    // calculating transfer fee
    if (primary_bank_name == account->get_bank_name()) {
        if (account->get_bank_name() == transfer_account->get_bank_name()) {
            cout << "주 은행간의 송금 수수료는 2000 KRW 입니다 " << endl;
            transfer_fee = 2000;
        }
        else {
            transfer_fee = 3000;
            cout << "주 은행과 타 은행과의 송금 수수료는 3000 KRW 입니다" << endl;
        }

    }
    else {
        if (primary_bank_name == transfer_account->get_bank_name()) {
            transfer_fee = 3000;
            cout << "주 은행과 타 은행과의 송금 수수료는 3000 KRW 입니다" << endl;

        }
        else {
            transfer_fee = 4000;
            cout << "타 은행간의 송금 수수료는 4000 KRW 입니다 " << endl;
        }
    }

    // cash transfer 
    if (i == 1) {

        cout << "송금하실 현금을 입력하시오" << endl;
        cin >> transfer_cash;
        while (cin.fail() == true || transfer_cash < 0 || calculator(transfer_cash)) {
            cout << "오류! 올바른 숫자를 입력하시오" << endl;
            cin.clear();
            cin.ignore(100, '\n');
             cout << "송금하실 현금을 입력하시오" << endl;

            cin >> transfer_cash;
        }

        // cout << "Insert the number of money you want to transfer including transfer fee" << endl;
       
        cout << "송금하실 금액은" << transfer_cash << "KRW 이고 송금수수료는 " << transfer_fee << " KRW 입니다" << endl;
        cout << "송금을 진행하겠습니까?" << endl;
        cout << "1. 네" << endl;
        cout << "2. 아니오" << endl;
        cin >> j;

        while (cin.fail() == true || j < 0 || j > 2) {
            cout << "오류! 올바른 숫자를 입력하시오" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "송금하실 금액은 " << transfer_cash << "KRW 이고 송금수수료는 " << transfer_fee << " KRW 입니다" << endl;
            cout << "송금을 진행하겠습니까?" << endl;
            cout << "1. 네" << endl;
            cout << "2. 아니오" << endl;
            cin >> j;
        }

        if (j == 1) {
            while (true) {
                cout << "송금에 필요한 금액은 " << transfer_cash + transfer_fee <<" 입니다" << endl;
                cout << "입금하실 현금의 개수를 선택하시오" << endl;
                cout << "cash1000, cash5000, cash10000, cash50000" << endl;
                cout << "1000 KRW: " << endl;
                cin >> cash1000;
                while (cin.fail() == true || cash1000 < 0) {
                     cout << "오류! 올바른 숫자를 입력하시오" << endl;

                    cin.clear();
                    cin.ignore(100, '\n');
                      cout << "입금하실 현금의 개수를 선택하시오" << endl;
                    cout << "1000 KRW: " << endl;
                    cin >> cash1000;
                }
                cout << "5000 KRW: " << endl;
                cin >> cash5000;
                while (cin.fail() == true || cash5000 < 0) {
                     cout << "오류! 올바른 숫자를 입력하시오" << endl;

                    cin.clear();
                    cin.ignore(100, '\n');
                      cout << "입금하실 현금의 개수를 선택하시오" << endl;
                    cout << "5000 KRW: " << endl;
                    cin >> cash5000;
                }
                cout << "10000 KRW: " << endl;
                cin >> cash10000;
                while (cin.fail() == true || cash10000 < 0) {
                     cout << "오류! 올바른 숫자를 입력하시오" << endl;
                    cin.clear();
                    cin.ignore(100, '\n');
                      cout << "입금하실 현금의 개수를 선택하시오" << endl;
                    cout << "10000 KRW: " << endl;
                    cin >> cash10000;
                }
                cout << "50000 KRW: " << endl;
                cin >> cash50000;
                while (cin.fail() == true || cash50000 < 0) {
                     cout << "오류! 올바른 숫자를 입력하시오" << endl;
                    cin.clear();
                    cin.ignore(100, '\n');
                      cout << "입금하실 현금의 개수를 선택하시오" << endl;
                    cout << "50000 KRW: " << endl;
                    cin >> cash50000;
                }

                cashcount = cash1000 + cash5000 + cash10000 + cash50000;
                if (cashcount <= 50) {
                    cashsum = cash1000 * 1000 + cash5000 * 5000 + cash10000 * 10000 + cash50000 * 50000;

                    if (cashsum == transfer_cash + transfer_fee) {
                        transfer_account->change_account_cash(transfer_cash);
                        this->atm_cash += cashsum;
                        transfer_account->change_account_cash(transfer_cash);
                       histo4 = "송금: 현금 송금: " + to_string((transfer_fee + transfer_cash)) + " KRW 삽입되었고 현금: " 
                            + to_string(transfer_cash) + " KRW 이 " + to_string(account->account_number)
                            + " " + account->username + " " + account->bankname + " 에서 "  
                            + to_string(transfer_account->account_number) + " " + transfer_account->username
                            + " " + transfer_account->bankname + " 에게 보내졌다.";
                        transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo4);
                        account->history.push_back(histo4);
                        transfer_account->history.push_back(histo4);
                        cout << "----------송금 성공----------" << endl;
                        // delete transfer_account; // memory deallocation
                        break;
                    }
                    else {
                        cout << "넣은 현금이 일치하지 않습니다." << endl;
                    }
                }
                else {
                    cout << "너무 많은 현금이 삽입되었습니다." << endl;
                }
            }

        }


        else if (j == 2) {
            cout << "----------송금 종료----------" << endl;
            // delete transfer_account;
            return 1;
        }

    }
    // account transfer
    if (i == 2) {
        while (true) {
        cout << "송금하실 현금을 입력하시오" << endl;

            cin >> transfer_cash;
            while (cin.fail() == true || transfer_cash < 0) {
                cout << "오류! 올바른 숫자를 입력하세요" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cout << "송금하실 현금을 입력하시오" << endl;

                cin >> transfer_cash;
            }
            cout << "송금하실 금액은" << transfer_cash << " KRW이고 송금 수수료는 " << transfer_fee << " KRW 입니다." << endl;
            cout << "송금을 계속하시겠습니까?" << endl;
            cout << "1. 네" << endl;
            cout << "2. 아니오" << endl;
            cin >> j;
            while (cin.fail() == true || j < 0 || j > 2) {
                cout << "오류! 올바른 숫자를 입력하세요" << endl;

                cin.clear();
                cin.ignore(100, '\n');
                cout << "송금하실 금액은 " << transfer_cash << " KRW 이고 송금수수료는 " << transfer_fee << " KRW 입니다." << endl;
                cout << "송금을 계속하시겠습니까?" << endl;
                cout << "1. 네" << endl;
                cout << "2. 아니오" << endl;
                cin >> j;
            }

            if (j == 1) {
                if (account->get_account_cash() < transfer_cash + transfer_fee) {
                    cout << "계좌에 잔액이 부족합니다." << endl;
                    cout << "현재 계좌에는 " << account->get_account_cash() << " KRW 이 있지만 송금에 필요한 금액은 " << transfer_cash + transfer_fee << " KRW 입니다" << endl;
                    return 2;
                }
                else {
                    account->change_account_cash(-(transfer_fee + transfer_cash));
                    transfer_account->change_account_cash(transfer_cash);
                    histo4 = "송금: 계좌송금: " + to_string((transfer_fee + transfer_cash)) + " KRW 계좌  " 
                        + to_string(account->account_number)
                        + " " + account->username + " " + account->bankname + " 로 부터 빠져나갔고 " + to_string(transfer_cash) + " KRW 원이  "
                        + to_string(transfer_account->account_number) + " " + transfer_account->username
                        + " " + transfer_account->bankname + " 로 입금되었습니다.";
                    transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo4);
                    account->history.push_back(histo4);
                    transfer_account->history.push_back(histo4);
                    cout << "송금 성공" << endl;
                    // delete transfer_account;
                    break;
                }
            }
            else if (j == 2) {
                cout << "송금 종료" << endl;
                // delete transfer_account;
                return 1;
            }
        }
    }

}
void ATM::transaction_menu_k() {
    ofstream newfile("transaction_history_file.txt");
    for (int i = 0; i < transaction_history.size(); i++) {
        newfile << transaction_history[i] << "\n";
    }
    cout << "----------관리자 Transaction 메뉴 종료----------" << endl;
    newfile.close();
};
