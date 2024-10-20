#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

class Account {
private:
    static int nextAccNum;  
    int AccNo;
    string AccHolderName;
    double balance;
    double interest;
    double overdraftLimit;
    string password;
    string AccType;

public:
    Account(const string &holderName, double initBalance, const string &accType, const string &pwd)
        : AccHolderName(holderName), balance(initBalance), password(pwd), AccType(accType) {
        
        AccNo = nextAccNum++;
        interest = (accType == "Savings") ? 0.05 : 0.04;
        overdraftLimit = (accType == "Current") ? 5000.0 : 0.0; 
    }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Successfully deposited: Rs" << amount << endl;
        } else {
            cout << "Invalid deposit amount!" << endl;
        }
    }

    void withdraw(double amount) {
        const double withdrawalFee = 5.00;  
        const double minBalance = 100.00;   

        if (amount > 0 && (balance + overdraftLimit) >= (amount + withdrawalFee)) {
            if ((balance - amount) < minBalance) {
                cout << "Withdrawal denied! Minimum balance of Rs100.00 is required." << endl;
                return;
            }
            balance -= (amount + withdrawalFee);
            cout << "Successfully withdrew: Rs" << amount << " (with a fee of Rs" << withdrawalFee << ")" << endl;
        } else {
            cout << "Withdrawal amount exceeds balance and overdraft limit!" << endl;
        }
    }

    void calculateInterest() {
        double interest = balance * interest;
        balance += interest;
        cout << "Interest of Rs" << interest << " added to your account." << endl;
    }

    void displayAccDetails() const {
        cout << fixed << setprecision(2);
        cout << "\n---------------------------------------\n";
        cout << "            Account Details            \n";
        cout << "---------------------------------------\n";
        cout << setw(20) << "Account Number:" << setw(11) << AccNo << endl;
        cout << setw(20) << "Account Holder:" << setw(11) << AccHolderName << endl;
        cout << setw(20) << "Balance:" << setw(9) << "Rs " << balance << endl;
        cout << setw(20) << "Interest Rate:" << setw(10) << interest * 100 << "%" << endl;
        cout << setw(20) << "Overdraft Limit:" << setw(9) << "Rs " << overdraftLimit << endl;
        cout << setw(20) << "Account Type:" << setw(13) << AccType << endl;
        cout << "---------------------------------------\n";
    }

    bool authenticate(const string &enteredPassword) const {
        return enteredPassword == password;
    }

    int getAccNo() const {
        return AccNo;
    }
};

int Account::nextAccNum = 10001;  

class Bank {
private:
    vector<Account> accounts;

public:
    void createAcc() {
        string holderName, password, AccType;
        double initBalance;

        cout << "Enter Account Holder Name: ";
        cin.ignore();
        getline(cin, holderName);

        cout << "Enter Initial Balance: ";
        cin >> initBalance;

        cout << "Enter Account Type (Savings/Current): ";
        cin >> AccType;

        cout << "Set your account password: ";
        cin >> password;

        Account newAccount(holderName, initBalance, AccType, password);
        accounts.push_back(newAccount);
        cout << "Account created successfully!" << endl;
    }

    void deleteAccount(int accNum) {
        auto it = accounts.begin();
        while (it != accounts.end()) {
            if (it->getAccNo() == accNum) {
                string enteredPassword;
                cout << "Enter password to delete account: ";
                cin >> enteredPassword;

                if (it->authenticate(enteredPassword)) {
                    it = accounts.erase(it);
                    cout << "Account deleted successfully!" << endl;
                    return;
                } else {
                    cout << "Incorrect password! Account deletion denied." << endl;
                    return;
                }
            } else {
                ++it;
            }
        }
        cout << "Account not found!" << endl;
    }

    void transactionManagement() {
        int accNum;
        cout << "Enter Account Number: ";
        cin >> accNum;

        for (Account &account : accounts) {
            if (account.getAccNo() == accNum) {
                string enteredPassword;
                cout << "Enter password for transaction: ";
                cin >> enteredPassword;

                if (account.authenticate(enteredPassword)) {
                    int choice;
                    double amount;

                    cout << "1. Deposit\n2. Withdraw\nChoose an option: ";
                    cin >> choice;

                    switch (choice) {
                        case 1:
                            cout << "Enter amount to deposit: ";
                            cin >> amount;
                            account.deposit(amount);
                            break;
                        case 2:
                            cout << "Enter amount to withdraw: ";
                            cin >> amount;
                            account.withdraw(amount);
                            break;
                        default:
                            cout << "Invalid option!" << endl;
                    }
                } else {
                    cout << "Incorrect password! Transaction denied." << endl;
                }
                return;
            }
        }
        cout << "Account not found!" << endl;
    }

    void AccBalanceInquiry() {
        int accNum;
        cout << "Enter Account Number: ";
        cin >> accNum;

        for (const Account &account : accounts) {
            if (account.getAccNo() == accNum) {
                string enteredPassword;
                cout << "Enter password to view details: ";
                cin >> enteredPassword;

                if (account.authenticate(enteredPassword)) {
                    account.displayAccDetails();
                } else {
                    cout << "Incorrect password! Access denied." << endl;
                }
                return;
            }
        }
        cout << "Account not found!" << endl;
    }

    void interestCal() {
        int accNum;
        cout << "Enter Account Number: ";
        cin >> accNum;

        for (Account &account : accounts) {
            if (account.getAccNo() == accNum) {
                string enteredPassword;
                cout << "Enter password to calculate interest: ";
                cin >> enteredPassword;

                if (account.authenticate(enteredPassword)) {
                    account.calculateInterest();
                } else {
                    cout << "Incorrect password! Access denied." << endl;
                }
                return;
            }
        }
        cout << "Account not found!" << endl;
    }

    void displayAllAcc() const {
        cout << "\n---------------------------------------\n";
        cout << "          All Account Details          \n";
        cout << "---------------------------------------\n";
        for (const Account &account : accounts) {
            account.displayAccDetails();
        }
    }
};

int main() {
    Bank bank;
    int choice;

    while (true) {
        cout << "\nWelcome to Indian Bank" << endl;
        cout << "Your Trusted Partner in Financial Services." << endl;
        cout << "\n1. Create Account\n2. Delete Account\n3. Transaction Management\n4. Account Balance Inquiry\n5. Calculate Interest\n6. Display All Accounts\n7. Exit\nChoose an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                bank.createAcc();
                break;
            case 2: {
                int accNum;
                cout << "Enter Account Number to delete: ";
                cin >> accNum;
                bank.deleteAccount(accNum);
                break;
            }
            case 3:
                bank.transactionManagement();
                break;
            case 4:
                bank.AccBalanceInquiry();
                break;
            case 5:
                bank.interestCal();
                break;
            case 6:
                bank.displayAllAcc();
                break;
            case 7:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid option! Please try again." << endl;
        }
    }
}
