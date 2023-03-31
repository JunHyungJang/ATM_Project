#include "ATM2.h"
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <typeinfo>
#include <fstream>
#include <sstream>
using namespace std;



ATM_single::ATM_single(string pbank, int num, string tp, string lan, long long cash, long long ad_c, long long ad_pw) :ATM
(pbank, num, "Single", lan, cash, ad_c, ad_pw) {
    this->primary_bank_name = pbank;
    this->serial_num = num;
    this->atm_type = tp;
    this->language = lan;
    this->atm_cash = cash;
    this->admin_card = ad_c;
    this->admin_pw = ad_pw;
    this->start_idx = 0;
    this->withdrawal_count = 0;
    cout << "ATM_single Constructor" << endl;
}

void ATM_single::atm_start() {
    int lan;
    cout << "Our bank name is " << this->primary_bank_name << endl;
    cout << "ATM type is " << this->atm_type << " and ATM serial number is " << this->serial_num << endl;
    if (this->language == "Bilingual") {
        cout << "We support 2 languages, English and Korean. Please select the language you want to use" << endl;
        cout << "�� ATM���� ����� �ѱ� �� ������ �� �����մϴ�. ����ϰ��� �ϴ� �� �����ϼ���" << endl;
        cout << "1. English(����)" << endl;
        cout << "2. Korean(�ѱ�)" << endl;
        cin >> lan;
    }
    if (lan == 2) {
        ATM_single::insert_card_k();
    }
    else {
        ATM_single::insert_card();
    }
    // ATM_single:: insert_card();

}

void ATM_single::insert_card() {

    cout << "----------Session start----------" << endl;

    int flag = 0;
    int i = 3;
    cout << "Single ATM insert card" << endl;
    while (i > 0) {
        if (flag == 2) {
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
            cout << "1. Transaction menu" << endl;
            cout << "2. Quit" << endl;
            cin >> t;
            while (cin.fail() == true || t < 0 || t>2) {
                cout << "You should enter correct number" << endl;
                cout << "Enter the number" << endl;
                cout << "1. Transaction menu" << endl;
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

int ATM_single::account_authorization() {

    int check_end;

    int flag;
    int bank_id;
    int id;

    id = all_bank.size();

    cout << "ATM_Single Authorization" << endl;

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
        if (primary_bank_name != a->bankname) {
            return 4;
        }
        string histo = "Session start with account ";
        histo += to_string(a->account_number) + " " + a->username;
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

void ATM_single::insert_card_k() {
    cout << "----------���� ����----------" << endl;

    int flag = 0;
    int i = 3;

    while (i > 0) {
        if (flag == 2) {
            cout << "��й�ȣ�� �Է��� " << i << "���� ��ȸ�� �ֽ��ϴ�" << endl;
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
            cout << "��й�ȣ�� ���¸� �Է��� " << i << "���� ��ȸ�� �ֽ��ϴ�" << endl;
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

int ATM_single::account_authorization_k() {
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


        string histo = a->username + "�� ���� " + to_string(a->account_number) + " ���� ������ ���۵˴ϴ�";
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