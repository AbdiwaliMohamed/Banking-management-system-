#include<iostream>
#include<fstream>
#include<cctype>
#include<iomanip>
using namespace std;
int tr,t=0;
class account
{
protected:
    int acno;
    char name[50];
    int deposit;
    char type;
    int pin;
public:
    void create_account();
    void show_account() const;
    void modify();
    void dep(int);
    void draw(int);
    void setpasswd(int);
    void report() const;
    int retacno() const;
    int retdeposit() const;
    char rettype() const;
    int retpasswd() const;
    int check(int);
};

class customer:public account
{
    int ano;
public:
    int checkaccount();
    int login();
    void changepasswd();
    void pay();
    void viewaccount();
    int depositwithdraw(int n,int option);
    void depositwithdraw(int n,int option,int amt);
};

int customer::login()
{
  account ac;
  int k;
  bool flag=false;
  ifstream inFile;
  inFile.open("account.txt",ios::binary);
  cout<<"\nenter account number:";
  cin>>ano;
  cout<<"enter password(first time pin is your ac/no make sure to change later):";
  cin>>k;
    while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(account)))
    {
        if(ac.retacno()==ano&&ac.retpasswd()==k)
        {
            flag=true;
            cout<<"\nlogged in successfully\n";
            return 1;
        }
    }
    inFile.close();
    if(flag==false)
    cout<<"\nAccount number does not exist or incorrect pin";
    else return 0;
}

int customer::checkaccount()
{
    account ac;
    int n;
    bool flag=false;
    ifstream inFile;
    inFile.open("account.txt",ios::binary);
    while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(account)))
    {
        if(ac.retacno()==ano)
        {
            ac.show_account();
            flag=true;
            return ano;
        }
    }
    inFile.close();
    if(flag==false)
    cout<<"\nAccount number does not exist";
    return 0;
}

void customer::viewaccount()
{
    account ac;
    int n;
    bool flag=false;
    ifstream inFile;
    inFile.open("account.txt",ios::binary);
    while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(account)))
    {
        if(ac.retacno()==ano)
        {
            ac.show_account();
            flag=true;
            return ;
        }
    }
    inFile.close();
    if(flag==false)
    cout<<"\nAccount number does not exist";
}

void customer::changepasswd()
{
    int o,p,r;
    bool found=false;
    account ac;
    fstream File;
    cout<<"NOTE:For security reasons if pin is entered incorrectly once then this activity stops\n\tBut don't worry you can get it reset at Bank\n";
    cout<<"\nEnter old pin:";
    cin>>o;
    File.open("account.txt", ios::binary|ios::in|ios::out);
    if(!File)
    {
        cout<<"File could not be open !! Press any Key...";
        return;
    }
    while(!File.eof() && found==false)
    {
        File.read(reinterpret_cast<char *> (&ac), sizeof(account));
        if(ac.retacno()==ano&&ac.retpasswd()==o)
        {
            ac.show_account();
            cout<<"\nEnter new pin(4 digit):";
            cin>>p;
            cout<<"\nRe-enter new pin:";
            cin>>r;
            while(r!=p)
            {
             cout<<"\nRe-enter nw pin:";
             cin>>r;
            }
            ac.setpasswd(p);
            int pos=(-1)*static_cast<int>(sizeof(ac));
            File.seekp(pos,ios::cur);
            File.write(reinterpret_cast<char *> (&ac),sizeof(account));
            cout<<"\n Password Updated\n";
            found=true;
           }
         }
    File.close();
    if(found==false)
        cout<<"\n Record Not Found or pin Incorrect \n";
}

void customer::pay()
{
    int des,amt,d;
    cout<<"enter destination account number:";
    cin>>des;
    cout<<"\nyou will be asked to enter amount twice to avoid errors\nenter now:";
    cin>>tr;
    d=depositwithdraw(ano,2);
    if(t==1)
    {depositwithdraw(des,1,tr);}
    ofstream receipt;
    receipt.open("receipt.txt");
    if(t==1)
    {receipt<<"amount of "<<tr<<" transferred to ac/no "<<des<<" from your ac/no "<<ano<<"\navailable balance:"<<d;}
    else
    {receipt<<"amount of "<<tr<<" couldn't be transferred to ac/no "<<des<<" due to insufficient balance in your account "<<ano<<"\navailable balance:"<<d;}
    receipt.close();
    cout<<"\nprint your generated receipt@receipt.txt";
}

void account::create_account()
{
    int c=1;
    cout<<"\nEnter The account No. :";
    cin>>acno;
    for(;;)
    {
    if(c>1)
    {cout<<"\nIs your number 4 digit?\nThen the number may be linked with a different account\nEnter again :";
    cin>>acno;}
    if(acno>999&&acno<10000&&check(acno)==0)
    {
    break;
    }
    else if(check(acno)==1&&acno>999&&acno<10000)
    {
     c++;
    cout<<"\nAccount number already exists!!!!Try another number\n";
    }
    else if(acno<999||acno>10000)
    {
    c++;
    cout<<"\n Account number must be 4 digits\n";
    }
    }
    cout<<"\nEnter The Name of The account Holder : ";
    cin.ignore();
    cin.getline(name,50);
    cout<<"\nEnter Type of The account (C/S) : ";
    cin>>type;
    type=toupper(type);
    cout<<"\nEnter The Initial amount(>=500 for Saving and >=1000 for current ) : ";
    cin>>deposit;
    while(type=='S'&&deposit<500)
    {
     cout<<"\nMinimum Balance:500\nEnter your amount:";
     cin>>deposit;
    }
    while(type=='C'&&deposit<1000)
    {
     cout<<"\nMinimum Balance:1000\nEnter your amount:";
     cin>>deposit;
    }
    pin=acno;
    cout<<"\nAccount Created..";
}

void account::show_account() const
{
    cout<<"\nAccount No. : "<<acno;
    cout<<"\nAccount Holder Name : ";
    cout<<name;
    cout<<"\nType of Account : "<<type;
    cout<<"\nBalance amount : "<<deposit;
}


void account::modify()
{
    cout<<"\nAccount No. : "<<acno;
    cout<<"\nModify Account Holder Name : ";
    cin.ignore();
    cin.getline(name,50);
    cout<<"\nModify Type of Account : ";
    cin>>type;
    type=toupper(type);
    cout<<"\nModify Balance amount : ";
    cin>>deposit;
}


void account::dep(int x)
{
    deposit+=x;
}

void account::draw(int x)
{
    deposit-=x;
}

void account::report() const
{
    cout<<acno<<setw(10)<<" "<<name<<setw(10)<<" "<<type<<setw(6)<<deposit<<endl;
}


int account::retacno() const
{
    return acno;
}

int account::retpasswd() const
{
    return pin;
}

void account::setpasswd(int p)
{
    pin=p;
}

int account::retdeposit() const
{
    return deposit;
}

char account::rettype() const
{
    return type;
}

void write_account();
void display_sp(int);
void modify_account(int);
void delete_account(int);
void display_all();
void deposit_withdraw(int, int);
void intro();
int choice();

int main()
{
    char ch;
    int num,c;
    intro();
    c=choice();
    if(c==1)
    {do
    {
        system("cls");
        cout<<"\nMAIN MENU";
        cout<<"\n01. NEW ACCOUNT";
        cout<<"\n02. DEPOSIT AMOUNT";
        cout<<"\n03. WITHDRAW AMOUNT";
        cout<<"\n04. BALANCE ENQUIRY";
        cout<<"\n05. ALL ACCOUNT HOLDER LIST";
        cout<<"\n06. CLOSE AN ACCOUNT";
        cout<<"\n07. MODIFY AN ACCOUNT";
        cout<<"\n08. EXIT";
        cout<<"\nSelect Your Option (1-8) ";
        cin>>ch;
        system("cls");
        switch(ch)
        {
        case '1':
            write_account();
            break;
        case '2':
            cout<<"\nEnter The account No. : "; cin>>num;
            deposit_withdraw(num, 1);
            break;
        case '3':
            cout<<"\nEnter The account No. : "; cin>>num;
            deposit_withdraw(num, 2);
            break;
        case '4':
            cout<<"\nEnter The account No. : "; cin>>num;
            display_sp(num);
            break;
        case '5':
            display_all();
            break;
        case '6':
            cout<<"\nEnter The account No. : "; cin>>num;
            delete_account(num);
            break;
         case '7':
            cout<<"\nEnter The account No. : "; cin>>num;
            modify_account(num);
            break;
         case '8':
            cout<<"\nThank you for using bank management system";
            break;
         default :cout<<"choose the valid option";
        }
        cin.ignore();
        cin.get();
    }while(ch!='8');
    }
    else if(c==2)
    {
        customer b;
        int k=b.login();
        while(k!=1)
        {
        k=b.login();
        }
        if(k==1)
        {do
        {
        system("cls");
        cout<<"\nMAIN MENU";
        cout<<"\n01. CHANGE PIN";
        cout<<"\n02. TRANSFER/PAY";
        cout<<"\n03. VIEW ACCOUNT";
        cout<<"\n04. LOGOUT and EXIT";
        cout<<"\nSelect Your Option (1-4) ";
        cin>>ch;
        system("cls");
        switch(ch)
        {
        case '1':
            b.changepasswd();
            break;
        case '2':
            b.pay();
            break;
        case '3':
            b.viewaccount();
            break;
        case '4':
            cout<<"\nThank you for using bank management system";
            break;
         default :cout<<"choose the valid option";
        }
        cin.ignore();
        cin.get();
    }while(ch!='4');
        }}
    return 0;
}

void write_account()
{
    account ac;
    ofstream outFile;
    outFile.open("account.txt",ios::binary|ios::app);
    ac.create_account();
    outFile.write(reinterpret_cast<char *> (&ac), sizeof(account));
    outFile.close();
}

void display_sp(int n)
{
    account ac;
    bool flag=false;
    ifstream inFile;
    inFile.open("account.txt",ios::binary);
    if(!inFile)
    {
        cout<<"File could not be open !! Press any Key...";
        return;
    }
    cout<<"\nBALANCE DETAILS\n";

    while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(account)))
    {
        if(ac.retacno()==n)
        {
            ac.show_account();
            flag=true;
        }
    }
    inFile.close();
    if(flag==false)
        cout<<"\nAccount number does not exist";
}

void modify_account(int n)
{
    bool found=false;
    account ac;
    fstream File;
    File.open("account.txt",ios::binary|ios::in|ios::out);
    if(!File)
    {
        cout<<"File could not be open !! Press any Key...";
        return;
    }
    while(!File.eof() && found==false)
    {
        File.read(reinterpret_cast<char *> (&ac), sizeof(account));
        if(ac.retacno()==n)
        {
            ac.show_account();
            cout<<"\nEnter The New Details of account"<<endl;
            ac.modify();
            int pos=(-1)*static_cast<int>(sizeof(account));
            File.seekp(pos,ios::cur);
            File.write(reinterpret_cast<char *> (&ac), sizeof(account));
            cout<<"\n Record Updated";
            found=true;
          }
    }
    File.close();
    if(found==false)
        cout<<"\n Record Not Found ";
}

void delete_account(int n)
{
    account ac;
    ifstream inFile;
    ofstream outFile;
    inFile.open("account.txt",ios::binary);
    if(!inFile)
    {
        cout<<"File could not be open !! Press any Key...";
        return;
    }
    outFile.open("Temp.txt",ios::binary);
    inFile.seekg(0,ios::beg);
    while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(account)))
    {
        if(ac.retacno()!=n)
        {
            outFile.write(reinterpret_cast<char *> (&ac), sizeof(account));
        }
    }
    inFile.close();
    outFile.close();
    remove("account.txt");
    rename("Temp.txt","account.txt");
    cout<<"\nRecord Deleted ..";
}
void display_all()
{
    account ac;
    ifstream inFile;
    inFile.open("account.txt",ios::binary);
    if(!inFile)
    {
        cout<<"File could not be open !! Press any Key...";
        return;
    }
    cout<<"\nACCOUNT HOLDER LIST";
    cout<<"A/c no.\tNAME\tType\tBalance\n";
    while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(account)))
    {
        ac.report();
    }
    inFile.close();
}

void deposit_withdraw(int n, int option)
{
    int amt;
    bool found=false;
    account ac;
    fstream File;
    File.open("account.txt", ios::binary|ios::in|ios::out);
    if(!File)
    {
        cout<<"File could not be open !! Press any Key...";
        return;
    }
    while(!File.eof() && found==false)
    {
        File.read(reinterpret_cast<char *> (&ac), sizeof(account));
        if(ac.retacno()==n)
        {
            ac.show_account();
            if(option==1)
            {
                cout<<"\nTO DEPOSITE AMOUNT ";
                cout<<"\nEnter The amount to be deposited";
                cin>>amt;
                ac.dep(amt);
            }
            if(option==2)
            {
                cout<<"\nTO WITHDRAW AMOUNT ";
                cout<<"\nEnter The amount to be withdraw";
                cin>>amt;
                int bal=ac.retdeposit()-amt;
                if((bal<500 && ac.rettype()=='S') || (bal<1000 && ac.rettype()=='C'))
                    cout<<"Insufficience balance";
                else
                    ac.draw(amt);
            }
            int pos=(-1)*static_cast<int>(sizeof(ac));
            File.seekp(pos,ios::cur);
            File.write(reinterpret_cast<char *> (&ac), sizeof(account));
            cout<<"\n Record Updated";
            found=true;
           }
         }
    File.close();
    if(found==false)
        cout<<"\n Record Not Found ";
}

int customer::depositwithdraw(int n,int option)
{
    int amt;
    bool found=false;
    account ac;
    fstream File;
    File.open("account.txt", ios::binary|ios::in|ios::out);
    if(!File)
    {
        cout<<"File could not be open !! Press any Key...";
        return 0;
    }
    while(!File.eof() && found==false)
    {
        File.read(reinterpret_cast<char *> (&ac), sizeof(account));
        if(ac.retacno()==n)
        {
            cout<<"\nThis is your Account record:\n";
            ac.show_account();
            if(option==1)
            {
                cout<<"\nTO DEPOSIT AMOUNT ";
                cout<<"\nEnter The amount to be deposited";
                cin>>amt;
                ac.dep(amt);
            }
            if(option==2)
            {
                cout<<"\nEnter The amount to be pay/transfer:";
                cin>>amt;
                while(amt!=tr)
                {
                 cout<<"\nRe-Enter The amount to be pay/transfer:";
                 cin>>amt;
                }
                int bal=ac.retdeposit()-amt;
                if((bal<500 && ac.rettype()=='S') || (bal<1000 && ac.rettype()=='C'))
                    {cout<<"\nInsufficient balance\n";
                      t=0;}
                else
                    {ac.draw(amt);
                     t=1;
                    }
            }
            int pos=(-1)*static_cast<int>(sizeof(ac));
            File.seekp(pos,ios::cur);
            File.write(reinterpret_cast<char *> (&ac), sizeof(account));
            cout<<"\n Your account Updated\n";
            found=true;
           }
         }
    File.close();
    if(found==false)
        cout<<"\n Record Not Found \n";
    return(ac.retdeposit());
}

void customer::depositwithdraw(int n, int option,int amt)
{
    bool found=false;
    account ac;
    fstream File;
    File.open("account.txt", ios::binary|ios::in|ios::out);
    if(!File)
    {
        cout<<"File could not be open !! Press any Key...";
        return ;
    }
    while(!File.eof() && found==false)
    {
        File.read(reinterpret_cast<char *> (&ac), sizeof(account));
        if(ac.retacno()==n)
        {
            //ac.show_account();
            if(option==1)
            {
                ac.dep(amt);
            }
            if(option==2)
            {
                int bal=ac.retdeposit()-amt;
                if((bal<500 && ac.rettype()=='S') || (bal<1000 && ac.rettype()=='C'))
                    cout<<"Insufficience balance";
                else
                    ac.draw(amt);
            }
            int pos=(-1)*static_cast<int>(sizeof(ac));
            File.seekp(pos,ios::cur);
            File.write(reinterpret_cast<char *> (&ac), sizeof(account));
            cout<<"\n Destination Record Updated \n";
            found=true;
           }
         }
    File.close();
    if(found==false)
        cout<<"\n Destination Account Not Found in our Bank";
//    return(ac.retdeposit());
}

int account::check(int n)
{
    account ac;
    ifstream inFile;
    inFile.open("account.txt",ios::binary);
    while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(account)))
    {
        if(ac.retacno()==n)
        {

            return 1;
        }
    }
    inFile.close();
    return 0;
}

void intro()
{
    cout<<"\n\t\t BANK MANAGEMENT SYSTEM";
    cout<<"\nMADE BY :) Abdiwali Mohamed Mursal";
    cin.get();
}

int choice()
{
    int c;
    cout<<"WELCOME Please Choose type ? \n1.BANK\n2.CUSTOMER\n";
    cin>>c;
    return c;
}

