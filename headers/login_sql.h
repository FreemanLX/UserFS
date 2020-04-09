#include <bits/stdc++.h>
#include <sqlite3.h>
#include "decrypt.h"
#ifndef LOGIN_SQL_H_INCLUDED
#define LOGIN_SQL_H_INCLUDED
using namespace std;

ofstream fout("Root/program.proc", ios::trunc);

char ver_pass[30], ver_user[30];
char database_login[30];
int ok_first = 0;
int admin_cp;

void convertToString(string& s, char* a, int size) 
{ 
    int i; 
    s = ""; 
    for (i = 0; i < size; i++) { 
        s = s + a[i]; 
    } 
} 

class login_sql{
public:
    int all_id;
    int login(int &admin);
};


    static int user_pass(void* data, int argc, char** argv, char** azColName)
    {
        login_sql g;
        int j = 0; 
        for (int i = 0; i < argc; i+=3)
        {
            char password_decr[32];
            clock_t timer = clock();
            string password_enc;
            convertToString(password_enc, argv[i+1], strlen(argv[i+1]));
            timer = -timer + clock();
            fout<<"Convert to string             Time used   "<<(float)timer/CLOCKS_PER_SEC<<" in seconds                   User:  Root                          Login: 1"<<endl;
            clock_t decrypta = clock();
            thread(decryption, password_enc, password_decr).join();
            decrypta = -decrypta + clock();
            fout<<"AES128 Decryption             Time used   "<<(float)decrypta/CLOCKS_PER_SEC<<" in seconds              User:  Root                          Login: 1"<<endl;
            if (strcmp(argv[i],ver_user)==0 && strcmp(ver_pass, password_decr) == 0)
             {
                ok_first = 1;
                if (argv[i+2][0] == '1')
                     admin_cp = 1;
                else admin_cp = 0;
                break;
             }
        }
        return 0;
    }


    int login_sql::login(int &admin)
    {

        cout<<"Pentru inceput introdu userul si parola"<<endl;
        sqlite3 *DB;
        cout<<"Utilizator: ";
        cin>>ver_user;
        cout<<"Parola utilizatorului "<<ver_user<<" este: ";
        cin>>ver_pass;
        cout<<"In ce baza de date e salvata?"<<endl;
        cin>>database_login;
        ifstream verify(database_login);
        if(!verify.good())
             cout<<"Nu exista respectiva baza de date"<<endl;
        else{
        fout<<"Task log in login user"<<endl;
        clock_t timer1 = clock();
        int exit = sqlite3_open(database_login, &DB);
        string query = "SELECT USER, PASSWORD, ADMIN FROM PERSON;";
        sqlite3_exec(DB, query.c_str(), user_pass, NULL, NULL);
        timer1 = -timer1 + clock();
        fout<<"SQL Lite                            Time used "<<(float)timer1/CLOCKS_PER_SEC<<" in seconds              User: Root                                Login: 1"<<endl; 
        admin = admin_cp;
        if(ok_first==1)
        {
            string complete, db_str;
            convertToString(complete, ver_user, strlen(ver_user));
            convertToString(db_str, database_login, strlen(database_login));
            string toor = "Root/" + db_str + "/" + complete;
            string se = toor + "/" + complete + ".proc";
            ofstream g(se.c_str(), std::ios::in | std::ios::out | std::ios::trunc); 
            g<<"Login                            Time used "<<(float)timer1/CLOCKS_PER_SEC<<" in seconds              User: "<<ver_user<<"                                Login: 1"<<endl;   
            
            return 1;
        }
        else return 0;
        }
    }
    
#endif // LOGIN_SQL_H_INCLUDED    
