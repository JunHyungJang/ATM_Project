#include <iostream>
#include <string>
#include <vector>
#include "Account.h"
#include "ATM.h"
#include "Bank.h"
#include "ATM2.h"

#include <typeinfo>


using namespace std;


int main () {
   
    Bank* bank1 = new Bank("Kakao");
    Bank* bank2 = new Bank("Shinhan");
 

    Account* account_a1 = new Account("Kakao","David",1234,111111111111,5000);
    Account* account_a2 = new Account("Shinhan","Jane",1234,222222222222,5000);
    Account* account_a3 = new Account("Kakao","Kate",1234,333333333333,5000);
  
    
    bank1->account_info.push_back(account_a1);
    bank2->account_info.push_back(account_a2);
    bank1->account_info.push_back(account_a3);

    ATM_single* atm1 = new ATM_single("Kakao",111111,"Single","Unilingual",5000,999999999999,5678);
    ATM* atm2 = new ATM("Shinhan",222222,"Multi","Bilingual",5000,999999999999,5678);
    ATM* atm3 = new ATM("Kakao",111111,"Multi","Bilingual",5000,999999999999,5678);


    atm1->all_bank.push_back(bank1);
    atm1->all_bank.push_back(bank2);
    atm2->all_bank.push_back(bank1);
    atm2->all_bank.push_back(bank2);
    atm3->all_bank.push_back(bank1);
    atm3->all_bank.push_back(bank2);
    

    cout << "----------------------------------------" << endl;
        cout << "[Account 1] Balance: " << account_a1->account_cash << endl;
        cout << "[Account 2] Balance: " << account_a2->account_cash << endl; 
        cout << "[Account 3] Balance: " << account_a3->account_cash << endl;
        cout << "[ATM 1] Remaining Cash: " << atm1->atm_cash << endl;
        cout << "[ATM 2] Remaining Cash: " << atm2->atm_cash << endl;  
        cout << "[ATM 3] Remaining Cash: " << atm3->atm_cash << endl;
        cout << "----------------------------------------" << endl;
    int k = 0;
    while(true){
        cout << "We have 3 atms(����� 3���� atm�Ⱑ �ֽ��ϴ�)." << endl;
        cout << "Enter the atm number you want to use(����ϰ��� �ϴ� atm�� ��ȣ�� �Է��ϼ���)" << endl;
        cout << "1. Atm1 is kakao bank, single, and unilingual(ATM 1���� īī�� ����, Ÿ ���ణ �ŷ� �Ұ���, ��� �����մϴ�)" << endl;
        cout << "2. Atm2 is shinhan bank, multi, and bilingual(ATM 2���� ��������, Ÿ ���ణ �ŷ� ����, ���� �� �ѱ�� �����մϴ�)" << endl;
        cout << "3. Atm3 is kakao bank, multi, and bilingual(ATM 3���� īī�� ����, Ÿ ���ణ �ŷ� ����, ���� �� �ѱ�� �����մϴ�)" << endl;
        cout << "4. if you want to quit the program(���α׷��� �����ϰ� ������ 4���� ��������)" << endl;
        cin >> k;


        switch(k){
            case 1 :
                atm1->atm_start();
                break;
            case 2:
                atm2->atm_start();
                break;
            case 3 :
                atm3->atm_start();
                break;

        }

        if (k==4){
            break;
        }
        cout << "----------------------------------------" << endl;
        cout << "[Account 1] Balance: " << account_a1->account_cash << endl;
        cout << "[Account 2] Balance: " << account_a2->account_cash << endl; 
        cout << "[Account 3] Balance: " << account_a3->account_cash << endl;
        cout << "[ATM 1] Remaining Cash: " << atm1->atm_cash << endl;
        cout << "[ATM 2] Remaining Cash: " << atm2->atm_cash << endl;  
        cout << "[ATM 3] Remaining Cash: " << atm3->atm_cash << endl;
        cout << "----------------------------------------" << endl;



    }

    cout << "----------����----------" << endl;
}
