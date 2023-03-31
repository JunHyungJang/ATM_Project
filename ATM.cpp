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
        cout << "�� ATM���� ����� �ѱ� �� ������ �� �����մϴ�. ����ϰ��� �ϴ� �� �����ϼ���" << endl;
        cout << "1. English(����)" << endl;
        cout << "2. Korean(�ѱ�)" << endl;
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
    cout << "----------���� ����----------" << endl;

    int flag = 0;
    int i = 3;

    while (i > 0) {
        if (flag ==2) {
            cout<< "��й�ȣ�� �Է��� " << i << "���� ��ȸ�� �ֽ��ϴ�" << endl;
            cout << "��й�ȣ�� �Է��ϼ���" << endl;
            cin >> this->pw;
            while (cin.fail() == true) {
            cout << "����! ��й�ȣ�� �ٽ� �Է��Ͻÿ�" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cin >> this->pw;
        }

        }
        else {
            cout<< "��й�ȣ�� ���¸� �Է��� " << i << "���� ��ȸ�� �ֽ��ϴ�" << endl;
            cout << "ī�� ��ȣ�� �Է��ϼ���" << endl;
            cin >> this->card_num;

            while (cin.fail() == true) {
                cout << "ATM�⿡�� �ùٸ� ���ڸ� �����ؾ� �մϴ�." << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cin >> this->card_num;
            }

            cout << "ī�� ��ȣ�� �Է��ϼ���" << endl;
            cin >> this->pw;

            while (cin.fail() == true) {
            cout << "����! ��й�ȣ�� �ٽ� �Է��ϼ���" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cin >> this->pw;
        }
        }
       

        if ((card_num == admin_card) && (pw == admin_pw)) {
            int t;
            cout << "���ϴ� �������� ��ȣ�� �Է��ϼ���" << endl;
            cout << "1. �ŷ��޴�" << endl;
            cout << "2. ������" << endl;
            cin >> t;
            while (cin.fail() == true || t < 0 || t>2) {
                cout << "�ùٸ� ���ڸ� �Է����ּ���" << endl;
                cout << "���ϴ� �������� ��ȣ�� �Է��ϼ���" << endl;
                cout << "1. �ŷ��޴�" << endl;
                cout << "2. ������" << endl;
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
            cout << "��й�ȣ�� �ٽ� �Է��ϼ���" << endl;
            
        }
        if (flag == 3) {
            cout << "���������� ��ġ���� �ʽ��ϴ�" << endl;
        }
        if (flag == 4) {
            cout << "�����̸��� ��ġ���� �ʽ��ϴ�" << endl;
        }
        i--;
    }

}

void ATM::session_end_k() {
    this->withdrawal_count = 0;
    cout << "----------���� ����----------" << endl;
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

        string histo = a->username + "�� ���� " + to_string(a->account_number) + " ���� ������ ���۵˴ϴ�" ;
        transaction_history.push_back(histo);
        a->history.push_back(histo);
        //cout << histo << endl;
        cout << "���Ͻô� �������� ��ȣ�� �Է��ϼ���" << endl;
        int i;

        do {
            cout << "1. ����" << endl;
            cout << "2. ���" << endl;
            cout << "3. �۱�" << endl;
            cout << "4. ������" << endl;
            cin >> i;

            while (cin.fail() == true || i <= 0 || i > 4) {
                cout << "������ 1���� 4 ������ ���ڸ� ���ÿ�" << endl;
                cout << "1. ����" << endl;
                cout << "2. ���" << endl;
                cout << "3. �۱�" << endl;
                cout << "4. ������" << endl;
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
                    cout << "3ȸ �̻� ����� �ϼ̱� ������ �̿��Ͻ� �� �����ϴ�." << endl;
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

        cout << "���¿� ��й�ȣ�� �ٽ� �Է����ּ���" << endl;
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


    cout << "������ ������ �����ϼ���" << endl;
    cout << "1. ����" << endl;
    cout << "2. ��ǥ" << endl;
    cout << "3. ������" << endl;
    cin >> i;

    while (cin.fail() == true || i < 0 || i > 3) {
        cout << "����! �ùٸ� ��ȣ�� �Է��ϼ���" << endl;
        cin.clear();
        cin.ignore(100, '\n');
        cout << "������ ������ �����ϼ���" << endl;
        cout << "1. ����" << endl;
        cout << "2. ��ǥ" << endl;
        cout << "3. ������" << endl;
        cin >> i;
    }


    switch (i) {
    case 1:
    {
        while (true) {
            cout << "�����Ͻ� ������ ���� �����ϼ���" << endl;
            cout << "1000��, 5000��, 10000��, 50000��" << endl;
            cout << "1000 KRW: " << endl;
            cin >> cash1000;
            while (cin.fail() == true || cash1000 < 0) {
                cout << "����! �ùٸ� ��ȣ�� �Է��ϼ���" << endl;

                cin.clear();
                cin.ignore(100, '\n');
            cout << "�����Ͻ� ������ ���� �����ϼ���" << endl;
                cout << "1000 KRW: " << endl;
                cin >> cash1000;
            }
            cout << "5000 KRW: " << endl;
            cin>> cash5000;
             while (cin.fail() == true || cash5000 < 0) {
                cout << "����! �ùٸ� ��ȣ�� �Է��ϼ���" << endl;
                cin.clear();
                cin.ignore(100, '\n');
            cout << "�����Ͻ� ������ ���� �����ϼ���" << endl;
                cout << "5000 KRW: " << endl;
                cin >> cash5000;
            }
            cout << "10000 KRW: " << endl;
            cin >> cash10000;
            while (cin.fail() == true || cash10000 < 0) {
                cout << "����! �ùٸ� ��ȣ�� �Է��ϼ���" << endl;
                cin.clear();
                cin.ignore(100, '\n');
            cout << "�����Ͻ� ������ ���� �����ϼ���" << endl;
                cout << "10000 KRW: " << endl;
                cin >> cash10000;
            }
            cout << "50000 KRW: " << endl;
            cin >> cash50000;
            while (cin.fail() == true || cash50000 < 0) {
                cout << "����! �ùٸ� ��ȣ�� �Է��ϼ���" << endl;
                cin.clear();
                cin.ignore(100, '\n');
            cout << "�����Ͻ� ������ ���� �����ϼ���" << endl;
                cout << "50000 KRW: " << endl;
                cin >> cash50000;
            }
       
            cashcount = cash1000 + cash5000 + cash10000 + cash50000;
            if (cashcount <= 50) {

                
                break;
            }
            else {
                cout << "�ʹ� ���� ���� ���ԵǾ����ϴ�" << endl;
            }
        }




        

        cashsum = cash1000 * 1000 + cash5000 * 5000 + cash10000 * 10000 + cash50000 * 50000;
        
        cout << "��� ������ �����̸��� " << account->get_bank_name() << " �Դϴ�" << endl;
        cout << "��ü �ݾ��� " << cashsum << " �Դϴ�"<< endl;
        // cout << atm_bank<< endl;

        if (primary_bank_name != account->get_bank_name()) {

            int k;
            cout << "���� ������KRW 1000�� �ֽ��ϴ�" << endl;
            cout << "������ �����Ͻðڽ��ϱ�?" << endl;
            cout << "1. ��" << endl;
            cout << "2. �ƴϿ�" << endl;
            cin >> k;
            while (cin.fail() == true || k < 0 || k > 2) {
                cout << "����! �ùٸ� ��ȣ�� �Է��ϼ���" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cout << "���� ������KRW 1000�� �ֽ��ϴ�" << endl;
                cout << "������ �����Ͻðڽ��ϱ�?" << endl;
                cout << "1. ��" << endl;
                cout << "2. �ƴϿ�" << endl;
                cin >> k;
            }
            if (k == 1) {
                this->atm_cash += cashsum;
                account->change_account_cash(cashsum - 1000);
                cout << "���ݼ���" << endl;
                histo2 = "����: ���� ����: " + to_string(cashsum - 1000) + " KRW";
                transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo2);
                account->history.push_back(histo2);

            }
            if (k == 2) {
                cout << "���� �ٽ� ����������" << endl;
                cout << "----------���� ����----------" << endl;
            }
            cash1000, cash5000, cash10000, cash50000, cashcount, cashsum = 0;

        }
        else {
            int k;
            cout << "������ ��� �����Ͻðڽ��ϱ�?" << endl;
            cout << "1. ��" << endl;
            cout << "2. �ƴϿ�" << endl;
            cin >> k;
            while (cin.fail() == true || k < 0 || k > 2) {
                cout << "����! �ùٸ� ��ȣ�� �Է��ϼ���" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cout << "������ ��� �����Ͻðڽ��ϱ�??" << endl;
                cout << "1. ��" << endl;
                cout << "2. �ƴϿ�" << endl;
                cin >> k;
            }
            if (k == 1) {
                this->atm_cash += cashsum;
                account->change_account_cash(cashsum);
                histo2 = "����: ���� ����: " + to_string(cashsum) + " KRW ";
                transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo2);
                account->history.push_back(histo2);
                cout << "----------���� ����----------" << endl;
            }
            if (k == 2) {
                cout << "���� ����������" << endl;
                cout << "----------���� ����----------" << endl;
            }
            cash1000, cash5000, cash10000, cash50000, cashcount, cashsum = 0;
        }
       //cout << "Deposit Success" << endl;
       
        cout << "����� ���¿���" << account->get_account_cash() <<"KRW �� ����ֽ��ϴ�."<< endl;
        cout << "ATM��� ����" << this->atm_cash << "KRW�� ������ �ֽ��ϴ�." << endl;

        break;
    }

    case 2:
    {
            int j;
            while (true) {
                cout << "����� ������ �� �ִ� ��ǥ�� ���� " << totalcheckcount << " �Դϴ�" <<endl;
                cout << "���� �ϰ���� ��ǥ�� ���� �Է��ϼ���" << endl;
                cin >> checkcount;
                while (totalcheckcount-checkcount<0) {
                    
                    cout << "������ �� �ִ� �ִ� ������ ���� 30��������, �ʰ��Ͽ����ϴ�." << totalcheckcount << " �Ʒ��� ���� �Է����ּ���" << endl;
                    cin >> checkcount;
                    
                }
                totalcheckcount -=checkcount;
                cout << "��ǥ�� ������ �Է��ϼ���" << endl;
                cin >> checkprice;
                checksum += checkcount * checkprice;
                cout << "�����Ͻ� ��ü �ݾ���" << checksum << "KRW �Դϴ�" << endl;
                 cout << "���Ͻô� �������� ��ȣ�� �Է��ϼ���" << endl;

                cout << "1. ��ǥ ������ ��� �����ϱ�" << endl;
                cout << "2. ��ǥ ���� �����ϱ�" << endl;
                cin >> j;
                if (j==2) 
                    break;
                
            }
        
        int k = 0;
    
        cout << "������ ����Ͻðڽ��ϱ�?" << endl;
        cout << "1. ��" << "\t" << "2. �ƴϿ�" << endl;
        cin >> k;
        while (cin.fail() == true || k < 0 || k > 2) {
            cout << "����! �ùٸ� ��ȣ�� �Է��ϼ���" << endl;

            cin.clear();
            cin.ignore(100, '\n');
            cout << "������ ����Ͻðڽ��ϱ�?" << endl;
            cout << "1. ��" << "\t" << "2. �ƴϿ�" << endl;
            cin >> k;
        }
        if (k == 1) {
            account->change_account_cash(checksum);
            histo2 = "����: ��ǥ ����: " + to_string(checksum) + " KRW";
            transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo2);
            account->history.push_back(histo2);
              cout << "���� ����" << endl;
            cout << "���¿� �ܾ��� " << account->get_account_cash()<<"KRW �ֽ��ϴ�"<< endl;
            
            cout << "ATM�� �ܾ���" << this->atm_cash<<"KRW �ֽ��ϴ�"<< endl;
        }
        if (k == 2) {
            cout << "���� �������ÿ�" << endl;
            cout << "���� ����" << endl;
            checksum = 0;
            checkcount = 0;
        }
      

        break;
    }

    case 3:
    {
        cout << "���� ����" << endl;
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
        cout << "�󸶳� ���� ���� �����Ͻðڽ��ϱ�?" << endl;
        cin >> withdrawal_cash;
        while (cin.fail() == true || withdrawal_cash < 0 || calculator(withdrawal_cash))  {
            cout << "����! �ùٸ� ��ȣ�� �Է��Ͻÿ�" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "�󸶳� ���� ���� �����Ͻðڽ��ϱ�?" << endl;
            cin >> withdrawal_cash;
        }
        //cout << "helloworld" << endl;
        cout << withdrawal_cash << endl;
        cout << fee <<endl;
        if (withdrawal_cash <= 500000 && atm_cash >= withdrawal_cash && account->get_account_cash() >= withdrawal_cash + fee) {
            break;
        }
        else if (withdrawal_cash > 500000) {
            cout << "500,000 KRW �̻� �����Ͻ� �� �����ϴ�." << endl;
        }
        else if (atm_cash < withdrawal_cash) {
            cout << "ATM�� ����� ���� ���� �ʽ��ϴ�." << endl;
            return 2;
        }
        else if (account->get_account_cash() < withdrawal_cash+fee) {
            cout << "���¿� ����� ���� ���� �ʽ��ϴ�." << endl;
            return 2;
        }
        cout << "�ش��ϴ� ��ȣ�� ���ÿ� " << endl;
        cout << "1. ������ ����Ѵ�." << endl;
        cout << "2. ������ ������." << endl;
        cin >> i;
        while (cin.fail() == true || i < 0 || i > 2) {
            cout << "����! �ùٸ� ��ȣ�� �Է��ϼ���" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "1. ������ ����Ѵ�" << endl;
            cout << "2. ������ ������" << endl;
            cin >> i;
        }
        if (i == 2) {
            cout << "----------���� ����----------" << endl;
            return 1;
        }
        withdrawal_cash = 0;
    } while (true);

    // primary fee 1000
    if (primary_bank_name != account->get_bank_name()) {

        int k;
        cout << "KRW 2000�� ���� �����ᰡ �ֽ��ϴ�" << endl;
        cout << "������ ����Ͻðڽ��ϱ�?" << endl;
        cout << "1. ��" << endl;
        cout << "2. �ƴϿ�" << endl;
        cin >> k;
        while (cin.fail() == true || k < 0 || k > 2) {
            cout << "����! �ùٸ� ��ȣ�� �Է��ϼ���" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "KRW 2000�� ���� �����ᰡ �ֽ��ϴ�" << endl;
            cout << "������ ����Ͻðڽ��ϱ�?" << endl;
            cout << "1. ��" << endl;
            cout << "2. �ƴϿ�" << endl;
            cin >> k;
        }
        if (k == 1) {
            this->atm_cash -= withdrawal_cash;
            account->change_account_cash(-(withdrawal_cash + 2000));
            histo3 = "����: ��������: " + to_string((withdrawal_cash + 2000)) + " KRW";
            transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo3);
            account->history.push_back(histo3);
            cout << "���⼺��" << endl;
            cout << "ATM ����" << this->atm_cash << "KRW �� �ֽ��ϴ�" << endl;
            cout << "���¿��� " << account->get_account_cash() << "KRW �� �ֽ��ϴ�" << endl;
        }
        if (k == 2) {
            cout << "���� ����" << endl;
            return 1;
        }
    }
    else {
        int k;
        cout << "KRW 1000�� ���� �����ᰡ �ֽ��ϴ�" << endl;
        cout << "������ ��� �Ͻðڽ��ϱ�?" << endl;
        cout << "1. ��" << endl;
        cout << "2. �ƴϿ�" << endl;
        cin >> k;
        while (cin.fail() == true || k < 0 || k > 2) {
            cout << "����! �ùٸ� ��ȣ�� �Է��Ͻÿ�" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "KRW 1000dml ���� �����ᰡ �ֽ��ϴ�" << endl;
            cout << "������ ��� �Ͻðڽ��ϱ�?" << endl;
            cout << "1. ��" << endl;
            cout << "2. �ƴϿ�" << endl;
            cin >> k;
        }

        if (k == 1) {
            this->atm_cash -= withdrawal_cash;
            account->change_account_cash(-(withdrawal_cash + 1000));
            histo3 = "����: ��������" + to_string(-(withdrawal_cash + 1000)) + " KRW";
            transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo3);
            account->history.push_back(histo3);
            cout << "���� ����" << endl;
            cout << "ATM ���� " << this->atm_cash << "KRW �� �ֽ��ϴ�" << endl;
            cout << "���¿��� " << account->get_account_cash() << "KRW �� �ֽ��ϴ�." << endl;
        }
        if (k == 2) {
            cout << "----------���� ����----------" << endl;
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

    cout << "�۱��� ������ �����Ͻÿ� " << endl;
    cout << "1. ���� �۱�" << endl;
    cout << "2. ���� �۱�" << endl;
    cin >> i;
    while (cin.fail() == true || i < 0 || i > 2) {
        cout << "����! �ùٸ� ���ڸ� �Է��Ͻÿ�" << endl;
        cin.clear();
        cin.ignore(100, '\n');
        cout << "�۱��� ������ �����Ͻÿ� " << endl;
        cout << "1. ���� �۱�" << endl;
        cout << "2. ���� �۱�" << endl;
        cin >> i;
    }
    int k = 0;

    do {
        cout << "�۱��Ͻ� ������ ��ȣ�� �Է��Ͻÿ�" << endl;
        cin >> transfer_number;
        while (cin.fail() == true || transfer_number < 0 || transfer_number == account->account_number)  {
        cout << "����! �ùٸ� ���ڸ� �Է��Ͻÿ�" << endl;
            cin.clear();
            cin.ignore(100, '\n');
        cout << "�۱��Ͻ� ������ ��ȣ�� �Է��Ͻÿ�" << endl;
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
                
                
                cout << "�۱��Ͻ� ���¹�ȣ�� ��ġ���� �ʽ��ϴ� " << endl;
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
            cout << "�� ���ణ�� �۱� ������� 2000 KRW �Դϴ� " << endl;
            transfer_fee = 2000;
        }
        else {
            transfer_fee = 3000;
            cout << "�� ����� Ÿ ������� �۱� ������� 3000 KRW �Դϴ�" << endl;
        }

    }
    else {
        if (primary_bank_name == transfer_account->get_bank_name()) {
            transfer_fee = 3000;
            cout << "�� ����� Ÿ ������� �۱� ������� 3000 KRW �Դϴ�" << endl;

        }
        else {
            transfer_fee = 4000;
            cout << "Ÿ ���ణ�� �۱� ������� 4000 KRW �Դϴ� " << endl;
        }
    }

    // cash transfer 
    if (i == 1) {

        cout << "�۱��Ͻ� ������ �Է��Ͻÿ�" << endl;
        cin >> transfer_cash;
        while (cin.fail() == true || transfer_cash < 0 || calculator(transfer_cash)) {
            cout << "����! �ùٸ� ���ڸ� �Է��Ͻÿ�" << endl;
            cin.clear();
            cin.ignore(100, '\n');
             cout << "�۱��Ͻ� ������ �Է��Ͻÿ�" << endl;

            cin >> transfer_cash;
        }

        // cout << "Insert the number of money you want to transfer including transfer fee" << endl;
       
        cout << "�۱��Ͻ� �ݾ���" << transfer_cash << "KRW �̰� �۱ݼ������ " << transfer_fee << " KRW �Դϴ�" << endl;
        cout << "�۱��� �����ϰڽ��ϱ�?" << endl;
        cout << "1. ��" << endl;
        cout << "2. �ƴϿ�" << endl;
        cin >> j;

        while (cin.fail() == true || j < 0 || j > 2) {
            cout << "����! �ùٸ� ���ڸ� �Է��Ͻÿ�" << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "�۱��Ͻ� �ݾ��� " << transfer_cash << "KRW �̰� �۱ݼ������ " << transfer_fee << " KRW �Դϴ�" << endl;
            cout << "�۱��� �����ϰڽ��ϱ�?" << endl;
            cout << "1. ��" << endl;
            cout << "2. �ƴϿ�" << endl;
            cin >> j;
        }

        if (j == 1) {
            while (true) {
                cout << "�۱ݿ� �ʿ��� �ݾ��� " << transfer_cash + transfer_fee <<" �Դϴ�" << endl;
                cout << "�Ա��Ͻ� ������ ������ �����Ͻÿ�" << endl;
                cout << "cash1000, cash5000, cash10000, cash50000" << endl;
                cout << "1000 KRW: " << endl;
                cin >> cash1000;
                while (cin.fail() == true || cash1000 < 0) {
                     cout << "����! �ùٸ� ���ڸ� �Է��Ͻÿ�" << endl;

                    cin.clear();
                    cin.ignore(100, '\n');
                      cout << "�Ա��Ͻ� ������ ������ �����Ͻÿ�" << endl;
                    cout << "1000 KRW: " << endl;
                    cin >> cash1000;
                }
                cout << "5000 KRW: " << endl;
                cin >> cash5000;
                while (cin.fail() == true || cash5000 < 0) {
                     cout << "����! �ùٸ� ���ڸ� �Է��Ͻÿ�" << endl;

                    cin.clear();
                    cin.ignore(100, '\n');
                      cout << "�Ա��Ͻ� ������ ������ �����Ͻÿ�" << endl;
                    cout << "5000 KRW: " << endl;
                    cin >> cash5000;
                }
                cout << "10000 KRW: " << endl;
                cin >> cash10000;
                while (cin.fail() == true || cash10000 < 0) {
                     cout << "����! �ùٸ� ���ڸ� �Է��Ͻÿ�" << endl;
                    cin.clear();
                    cin.ignore(100, '\n');
                      cout << "�Ա��Ͻ� ������ ������ �����Ͻÿ�" << endl;
                    cout << "10000 KRW: " << endl;
                    cin >> cash10000;
                }
                cout << "50000 KRW: " << endl;
                cin >> cash50000;
                while (cin.fail() == true || cash50000 < 0) {
                     cout << "����! �ùٸ� ���ڸ� �Է��Ͻÿ�" << endl;
                    cin.clear();
                    cin.ignore(100, '\n');
                      cout << "�Ա��Ͻ� ������ ������ �����Ͻÿ�" << endl;
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
                       histo4 = "�۱�: ���� �۱�: " + to_string((transfer_fee + transfer_cash)) + " KRW ���ԵǾ��� ����: " 
                            + to_string(transfer_cash) + " KRW �� " + to_string(account->account_number)
                            + " " + account->username + " " + account->bankname + " ���� "  
                            + to_string(transfer_account->account_number) + " " + transfer_account->username
                            + " " + transfer_account->bankname + " ���� ��������.";
                        transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo4);
                        account->history.push_back(histo4);
                        transfer_account->history.push_back(histo4);
                        cout << "----------�۱� ����----------" << endl;
                        // delete transfer_account; // memory deallocation
                        break;
                    }
                    else {
                        cout << "���� ������ ��ġ���� �ʽ��ϴ�." << endl;
                    }
                }
                else {
                    cout << "�ʹ� ���� ������ ���ԵǾ����ϴ�." << endl;
                }
            }

        }


        else if (j == 2) {
            cout << "----------�۱� ����----------" << endl;
            // delete transfer_account;
            return 1;
        }

    }
    // account transfer
    if (i == 2) {
        while (true) {
        cout << "�۱��Ͻ� ������ �Է��Ͻÿ�" << endl;

            cin >> transfer_cash;
            while (cin.fail() == true || transfer_cash < 0) {
                cout << "����! �ùٸ� ���ڸ� �Է��ϼ���" << endl;
                cin.clear();
                cin.ignore(100, '\n');
                cout << "�۱��Ͻ� ������ �Է��Ͻÿ�" << endl;

                cin >> transfer_cash;
            }
            cout << "�۱��Ͻ� �ݾ���" << transfer_cash << " KRW�̰� �۱� ������� " << transfer_fee << " KRW �Դϴ�." << endl;
            cout << "�۱��� ����Ͻðڽ��ϱ�?" << endl;
            cout << "1. ��" << endl;
            cout << "2. �ƴϿ�" << endl;
            cin >> j;
            while (cin.fail() == true || j < 0 || j > 2) {
                cout << "����! �ùٸ� ���ڸ� �Է��ϼ���" << endl;

                cin.clear();
                cin.ignore(100, '\n');
                cout << "�۱��Ͻ� �ݾ��� " << transfer_cash << " KRW �̰� �۱ݼ������ " << transfer_fee << " KRW �Դϴ�." << endl;
                cout << "�۱��� ����Ͻðڽ��ϱ�?" << endl;
                cout << "1. ��" << endl;
                cout << "2. �ƴϿ�" << endl;
                cin >> j;
            }

            if (j == 1) {
                if (account->get_account_cash() < transfer_cash + transfer_fee) {
                    cout << "���¿� �ܾ��� �����մϴ�." << endl;
                    cout << "���� ���¿��� " << account->get_account_cash() << " KRW �� ������ �۱ݿ� �ʿ��� �ݾ��� " << transfer_cash + transfer_fee << " KRW �Դϴ�" << endl;
                    return 2;
                }
                else {
                    account->change_account_cash(-(transfer_fee + transfer_cash));
                    transfer_account->change_account_cash(transfer_cash);
                    histo4 = "�۱�: ���¼۱�: " + to_string((transfer_fee + transfer_cash)) + " KRW ����  " 
                        + to_string(account->account_number)
                        + " " + account->username + " " + account->bankname + " �� ���� ���������� " + to_string(transfer_cash) + " KRW ����  "
                        + to_string(transfer_account->account_number) + " " + transfer_account->username
                        + " " + transfer_account->bankname + " �� �ԱݵǾ����ϴ�.";
                    transaction_history.push_back("ID" + to_string(transaction_history.size()) + ": " + histo4);
                    account->history.push_back(histo4);
                    transfer_account->history.push_back(histo4);
                    cout << "�۱� ����" << endl;
                    // delete transfer_account;
                    break;
                }
            }
            else if (j == 2) {
                cout << "�۱� ����" << endl;
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
    cout << "----------������ Transaction �޴� ����----------" << endl;
    newfile.close();
};
