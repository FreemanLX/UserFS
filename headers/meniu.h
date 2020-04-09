#ifndef MENIU_H_INCLUDED
#define MENIU_H_INCLUDED
#include "login_sql.h"
#include <sys/stat.h>
#include <unistd.h>
#include "encrypt.h"
#include <sqlite3.h>
#include <termcap.h>
#include <bits/stdc++.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <experimental/filesystem>
#include <chrono>
using namespace std;




struct
{
    char id[2];
    char username[256];
    char password[30];
    char admin[2];
    char login[2];
} v[100];


int id_max_db = 0;

class meniu_start
{

public:
    int verify(const char *fname);
    void database(char *baza_de_date);
    void intrebare(char *e, auto intrebare1, int ok);
    void users(int ok, char *baza_de_date);
    void startup();
    void clearscreen();
    void meniu(char *s, int admin, char *db);
    int verificare(char *t1, char *t2);
};



static int id_db(void* data, int argc, char** argv, char** azColName)
{
    id_max_db++;
    return 0;
}

static int afisare(void* data, int argc, char** argv, char** azColName)
{
    for (int i = 0; i < argc; i++)
    {
        cout<<azColName[i]<<":  "<<argv[i]<<"    ";
        if (i % 2 == 1 && i!=0)
            cout<<endl;
    }
    return 0;
}


int meniu_start::verify(const char *fname)
{
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void meniu_start::database(char *baza_de_date)
{
    int e;
    while(true)
    {
        cout<<"Ce nume vrei sa ai la baza de date?"<<endl;
        cin>>baza_de_date;
        if (strlen(baza_de_date) > 1) break;
        else cout<<"Nume neacceptat"<<endl;
    }
    string s2 = ".db";
    string con_baza;
    convertToString(con_baza, baza_de_date, strlen(baza_de_date));
    if (con_baza.find(s2) == std::string::npos)
        strcat(baza_de_date, ".db");
    sqlite3* DB;
    std::string sql = "CREATE TABLE PERSON("
                      "ID             INT                     NOT NULL, "
                      "USER           TEXT                    NOT NULL, "
                      "PASSWORD       TEXT                    NOT NULL, "
                      "ADMIN          INT                     NOT NULL, "
                      "LOGIN          INT                     NOT NULL );";
    int exit = 0;
    exit = sqlite3_open(baza_de_date, &DB);
    char* messaggeError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

    if (exit != SQLITE_OK)
    {
        std::cerr << "Eroare in crearea bazei de date" << std::endl;
        sqlite3_free(messaggeError);
        database(baza_de_date);
    }
    else
        std::cout << "Baza de date cu numele " << baza_de_date << " a fost creat cu succes" << std::endl;
}

void meniu_start::intrebare(char *e, auto intrebare1, int ok)
{
    if(ok==0) strcpy(e, "1");
    else
    {
        int gamma;
        do
        {
            while(true)
            {
                string str;
                cout<<intrebare1<<endl;
                char read[100];
                scanf("%s", read);
                convertToString(str, read, strlen(read));
                std::stringstream ss(str);
                if( ss >> gamma )
                {
                    if(gamma !=0 && gamma!=1) cout<<"Input only 1 / 0 (1 = TRUE) / (0 = FALSE)"<<endl;
                    if(gamma == 0)
                    {
                        strcpy(e, "0");
                        break;
                    }
                    else
                    {
                        strcpy(e, "1");
                        break;
                    }
                }
                else
                {
                    cout<<"Input only 1 / 0 (1 = TRUE) / (0 = FALSE)"<<endl;
                }
            }
        }
        while (!(gamma==1 || gamma==0));
    }


}


int meniu_start::verificare (char *t1, char *t2)
{
    string t1_str, t2_str;
    convertToString(t1_str, t1, strlen(t1));
    convertToString(t2_str, t2, strlen(t2));
    if(strlen(t1) < strlen(t2))
    {
        if (t2_str.find(t1_str) != std::string::npos)
        {
            return 0;
        }
    }
    if (strlen(t1) > strlen(t2))
    {
        if (t1_str.find(t2_str) != std::string::npos)
        {
            return 0;
        }
    }
    if (strlen(t1) == strlen(t2))
    {
        if(strcmp(t1,t2)==0)
        {
            return 0;
        }
    }
    return 1;
}


void meniu_start::users(int ok, char *baza_de_date)
{
    string database_str;
    convertToString(database_str, baza_de_date, strlen(baza_de_date));
    string partial = "Root/";
    auto check = mkdir(partial.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    partial = "Root/" + database_str;
    auto check1 = mkdir(partial.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    sqlite3* DB;
    char* messaggeError;
    string str;
    int e;
    while (true)
    {
        int ok1 = 0;
        std::getline(std::cin, str);
        std::stringstream ss(str);
        if( ss >> e )
        {
            int digits = static_cast<int> ( log10( static_cast<double>(e) ) ) + 1;
            break;
        }
        else
        {
            if(ok1 == 0)
            {
                cout<<"Cati utilizatori vrei sa creezi?"<<endl;
                ok1 = 1;
            }
            else
            {
                cout<<"Introduceti din nou:"<<endl;
            }
        }

    }


    int f = ok + e;
    while(ok < f)
    {
        do
        {
            cout<<"Utilizatorul "<<ok<<endl;
            v[ok].id[0] = ok + '0';
            cout<<"Introdu un nickname la user "<<endl;
            cin>>v[ok].username;
            do
            {
                cout<<"Introduceti o parola pentru userul "<<v[ok].username<<endl;
                cin>>v[ok].password;
                if (strlen(v[ok].password) < 5)
                {
                    cout<<"Parola trebuie sa aiba mai mult de 5 caractere"<<endl;
                    continue;
                }
            }
            while(strlen(v[ok].password) < 5);
            if(verificare(v[ok].username, v[ok].password)==0)
                cout<<"Parola si user-ul sunt asemnatoare, va rog sa reintroduceti"<<endl;
        }
        while (verificare(v[ok].username, v[ok].password) == 0);
        intrebare(v[ok].admin, "Doresti ca acest utilizator sa fie admin?", ok);
        intrebare(v[ok].login, "Doresti ca acest utilizator sa fie activ?", ok);
        char s[256];
        strcpy(s, "INSERT INTO PERSON VALUES(");
        strcat(s,v[ok].id);
        strcat(s,",");
        strcat(s,"'");
        strcat(s,v[ok].username);
        strcat(s,"'");
        strcat(s,",");
        strcat(s,"'");
        char password_encr[32];
        encryption(v[ok].password, password_encr);
        strcat(s,password_encr);
        strcat(s,"'");
        strcat(s,",");
        strcat(s,v[ok].admin);
        strcat(s,",");
        strcat(s,v[ok].login);
        strcat(s,");");
        int exit = sqlite3_open(baza_de_date, &DB);
        exit = sqlite3_exec(DB, s, NULL, NULL, NULL);
        if(exit == SQLITE_OK)
        {
            string complete;
            convertToString(complete, v[ok].username, strlen(v[ok].username));
            string toor = "Root/" + database_str + "/" + complete;
            string se = toor + "/" + complete + ".proc";
            auto check = mkdir(toor.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            if(!check)
            {
                ofstream g(se.c_str(), std::ios::in | std::ios::out | std::ios::trunc);
                g<<" ";
                if(g)
                {
                    cout<<"Fisierul de procese a fost creat cu succes si este localizat in "<<se<<endl;

                }
            }
            std::cout << "User-ul "<<v[ok].username<< " a fost creat cu succes!" << std::endl;
            ok++;
        }
    }
}

void meniu_start::startup()
{
    cout<<"Nu am detectat nicio baza de date valabila"<<endl;
    cout<<"Pentru inceput vei creea o baza de date"<<endl;
    char baza_de_date[20];
    database(baza_de_date);
    cout<<"Vei crea utilizatori"<<endl;
    users(1, baza_de_date);
    ofstream f("done");
    f<<" ";
    cout<<"Te rog sa redeschizi programul"<<endl;
}


void meniu_start::clearscreen()
{
    char buf[2048];
    char *str;
    tgetent(buf, getenv("TERM"));
    str = tgetstr("cl", NULL);
    fputs(str, stdout);
}


void initialize_clean(char *s)
{
    for(int i=0; i<strlen(s); i++)
        s[i] = '0';
}

void meniu_start::meniu(char *s, int admin, char *db)
{
    clearscreen();
    string come,db_str;
    convertToString(come, s, strlen(s));
    convertToString(db_str, db, strlen(db));
    string se = "Root/" + db_str + "/" + come + "/" + come + ".proc";
    fstream ha;
    ha.open(se.c_str(), std::ios::in | std::ios::out);
    cout<<"The process file "<<se<<endl;
    cout<<"Esti logat in userul "<< s <<endl;
    if (admin == 0)
        cout<<"Nu esti admin"<<endl;
    else cout<<"Esti logat ca si admin"<<endl;
    while (true)
    {
        if(admin == 1)
        {
            cout<<"Meniu:"<<endl;
            cout<<"1) Creaza tabel cu utilizatori (ADVANCED)"<<endl;
            cout<<"2) Insearea utilizatori in tabel "<<endl;
            cout<<"3) Afiseaza utilizatori dintr-un tabel anume"<<endl;
            cout<<"4) Logeaza-te intr-un utilizator"<<endl;
            cout<<"5) Simulare de creare de utilizatori"<<endl;
            cout<<"6) Afisarea de procese"<<endl;
            cout<<"7) Iesire din program"<<endl;
            cout<<"8) Reset the program. All the data will be removed"<<endl;
            cout<<s<<">";
            string alpha;
            int e;
            int ok_fix = 0;
            while(true)
            {
                std::getline(std::cin, alpha);
                std::stringstream ss(alpha);
                if( ss >> e )
                {
                    int digits = static_cast<int> ( log10( static_cast<double>(e) ) ) + 1;
                    break;
                }
                else if(ok_fix == 0) ok_fix = 1;
                else
                {
                    cout<<"Eroare introdu un numar"<<endl;
                    cout<<s<<">";
                }
            }

            if (e == 1)
            {
                clearscreen();
                char baza_de_date[25], loading[2];
                cout<<"The process Creating Database has been started  "<<endl;
                clock_t timer = clock();

                database(baza_de_date);
                timer = clock() - timer;
                ha<<"Database creator                            Time used "<<(float)timer/CLOCKS_PER_SEC<<" in seconds              User:"<< s << "                                Login: 1"<<endl;
                do
                {
                    cout<<"Doresti sa creezi utilizatori? Y / N?"<<endl;
                    cin>>loading;
                    if(loading[0] != 'Y' && loading[0]!= 'N')
                    {
                        cout<<"Nu ai introdus Y sau N, pe scurt o iau ca pe un nu"<<endl;
                    }
                }
                while (loading[0]!= 'Y' && loading[0]!= 'N');
                if(loading[0] == 'Y') users(1, baza_de_date);
                else if (loading[0] == 'y') users(1, baza_de_date);
                else if (loading[0] == 'N' || loading[0] == 'n') continue;
            }
            if (e == 2)
            {
                clearscreen();
                cout<<"The process Creating Users has been started  "<<endl;
                clock_t timer = clock();
                cout<<"Aici creeze utilzatori intr-o baza de date"<<endl;
                char database[20];
                cout<<"Introdu numele bazei de date"<<endl;
                cin>>database;
                ifstream verify(database);
                if(!verify.good())
                {
                    cout<<"Nu exista respectiva baza de date"<<endl;
                    continue;
                }
                sqlite3 *DB;
                int exit = sqlite3_open(database, &DB);
                string query = "SELECT USER FROM PERSON;";
                sqlite3_exec(DB, query.c_str(), id_db, NULL, NULL);
                users(id_max_db, database);
                timer = clock() - timer;
                ha<<"Creating users                            Time used "<<(float)timer/CLOCKS_PER_SEC<<" in seconds              User: "<<s<<"                                 Login: 1"<<endl;
            }
            if (e == 3)
            {
                clock_t timer = clock();
                clearscreen();
                cout<<"Introduce numele bazei de date"<<endl;
                char database[20];
                cin>>database;
                ifstream verify(database);
                if(!verify.good())
                {
                    cout<<"Nu exista respectiva baza de date"<<endl;
                    continue;
                }
                sqlite3 *DB;
                int exit = sqlite3_open(database, &DB);
                string query = "SELECT USER, ADMIN FROM PERSON;";
                sqlite3_exec(DB, query.c_str(), afisare, NULL, NULL);
                timer = clock() - timer;
                ha<<"SQL Lite Showing Users                          Time used "<<(float)timer/CLOCKS_PER_SEC<<" in seconds              User: "<<s<<"                                Login: 1"<<endl;
            }
            if (e == 4)
            {
                clearscreen();
                cout<<"Te-ai delogat din userul "<<s<<endl;
                login_sql g;
                int admin1 = admin;
                if(g.login(admin1)==1)
                {
                    if(admin1 == 1)
                    {
                        meniu(ver_user,admin, database_login);
                        break;

                    }
                    else
                    {
                        meniu(ver_user,0, database_login);
                        break;
                    }
                }
                else break;
            }
            if (e == 5)
            {

                clock_t timer = clock();
                clearscreen();
                int random;
                string beta;
                cout<<"Cati utilizatori doresti sa creezi random?"<<endl;
                while(true)
                {
                    std::getline(std::cin, beta);
                    std::stringstream ss(beta);
                    if( ss >> random )
                    {
                        int digits = static_cast<int> ( log10( static_cast<double>(random) ) ) + 1;
                        break;
                    }
                    else if(ok_fix == 0) ok_fix = 1;
                    else
                    {
                        cout<<"Cati utilizatori doresti sa creezi random?"<<endl;
                        cout<<"Eroare introdu un numar"<<endl;
                    }
                }
                int counter = 0;
                while (counter < random)
                {
                    char users[e];
                    char letters[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                                       'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                                       's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
                                     };

                    std::random_device r;
                    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
                    std::shuffle(std::begin(letters), std::end(letters),
                                 std::mt19937(seed));
                    char rand_username[32];
                    rand_username[0]=0;
                    unsigned v1;
                    do
                    {
                        v1 = rand() % 32;
                    }
                    while(v1<5);
                    int i = 0;
                    for (char c : letters)
                    {
                        if(i == v1)
                            break;
                        rand_username[i] = c;
                        i++;
                    }
                    rand_username[strlen(rand_username)]='\0';
                    char rand_password[32];
                    rand_password[0]=0;
                    i = 0;
                    do
                    {
                        v1 = rand() % 32;
                    }
                    while(v1 < 5);
                    for (char c : letters)
                    {
                        if(i == v1)
                            break;
                        rand_password[i] = c;
                        i++;
                    }
                    rand_password[strlen(rand_password)]='\0';
                    char login[2], admin[2];
                    do
                    {
                        v1 = rand() % 2;
                    }
                    while(v1 < 0 && v1 == 2);
                    login[0] = v1 + '0';
                    if (v1 == 1)
                    {
                        do
                        {
                            v1 = rand() % 2;
                        }
                        while(v1 < 0 && v1 == 2);
                        admin[0] = v1 + '0';
                    }
                    else
                        admin[0] = '0';
                    char complete[256];
                    cout<<"Introdus cu succes user-ul " << rand_username << " cu parola " << rand_password << endl;
                    counter++;
                    string username_t1;
                    convertToString(username_t1, s, strlen(s));
                    string folder = "Root/"+ db_str + "/" + username_t1 + "/Random_User";
                    auto fl = mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                    char *folder_char = new char[sizeof(folder)/sizeof(string)];
                    folder_char = (char*)folder.c_str();
                    strcat(folder_char, "/");
                    strcat(folder_char, rand_username);
                    auto check = mkdir(folder_char, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                    if (!check)
                        printf("Directory created\n");
                    rand_username[0]=0;
                    rand_password[0]=0;
                    timer = clock() - timer;
                    ha<<"Random user                           Time used "<<(float)timer/CLOCKS_PER_SEC<<" in seconds            User:"<<s<< "                                Login: 1\n";
                }
                continue;
            }
            if (e == 6)
            {
                clearscreen();
                cout<<"Afisarea de procese pentru userul "<<s<<endl;
                ifstream gamma(se.c_str());
                string s[100];
                int i=0;
                while(getline(gamma, s[i]))
                {
                    cout<<s[i]<<endl;
                    i++;
                }
                ifstream gamma1("Root/program.proc");
                i=0;
                while(getline(gamma1, s[i]))
                {
                    cout<<s[i]<<endl;
                    i++;
                }
            }
            if (e == 7)
            {
                ha<<"Exit program                             Time used 1 in seconds              User: "<<s <<"                             Login: 1"<<endl;
                clearscreen();
                cout<<"Ai iesit din program"<<endl;
                break;
            }
            if (e == 8)
            {
                clearscreen();
                cout<<"Resetting the program, this will take more than 2 seconds"<<endl;
                using namespace std::chrono;
                milliseconds mil(1000);
                mil = mil*10;
                mil.count();
                system("rm done");
                system("rm -r Root");
                system("find . -name \"*.db\" -type f -delete");
                cout<<"Reset complete. Reporneste programul"<<endl;
                break;
            }
        }
        else
        {
            cout<<"1) Logeaza-te intr-un utilizator"<<endl;
            cout<<"2) Iesire din program"<<endl;
            cout<<"3) Afiseaza utilizatori dintr-un tabel anume"<<endl;
            cout<<"4) Simulare de creare de utilizatori"<<endl;
            cout<<"5) Afisarea de procese"<<endl;
            cout<<s<<">";
            string alpha;
            int e;
            int ok_fix = 0;
            while(true)
            {
                std::getline(std::cin, alpha);
                std::stringstream ss(alpha);
                if( ss >> e )
                {
                    int digits = static_cast<int> ( log10( static_cast<double>(e) ) ) + 1;
                    break;
                }
                else if(ok_fix == 0) ok_fix = 1;
                else
                {
                    cout<<"Eroare introdu un numar"<<endl;
                    cout<<s<<">";
                }
            }

            if (e == 2)
            {
                clearscreen();
                ha<<"Exit program                             Time used 1 in seconds              User: "<<s <<"                             Login: 1"<<endl;
                cout<<"Ai iesit din program"<<endl;
                break;
            }
            if (e == 1)
            {
                clearscreen();
                cout<<"Te-ai delogat din userul "<<s<<endl;
                login_sql g;
                int admin1 = admin;
                if(g.login(admin1)==1)
                {
                    if(admin1 == 1)
                    {
                        admin = 1;
                        meniu(s, admin, database_login);
                        break;
                    }
                    else
                    {
                        admin = 0;
                        meniu(s, admin, database_login);
                        break;
                    }
                }
                else break;
            }
            if (e == 3)
            {
                clock_t timer = clock();
                clearscreen();
                cout<<"Introduce numele bazei de date"<<endl;
                char database[20];
                cin>>database;
                ifstream verify(database);
                if(!verify.good())
                {
                    cout<<"Nu exista respectiva baza de date"<<endl;
                    continue;
                }
                sqlite3 *DB;
                int exit = sqlite3_open(database, &DB);
                string query = "SELECT USER, ADMIN FROM PERSON;";
                sqlite3_exec(DB, query.c_str(), afisare, NULL, NULL);
                timer = clock() - timer;
                ha<<"SQL Lite Showing Users                          Time used "<<(float)timer/CLOCKS_PER_SEC<<" in seconds              User: "<<s<<"                                Login: 1"<<endl;
            }
            if (e == 4)
            {

                clock_t timer = clock();
                clearscreen();
                int random;
                string beta;
                cout<<"Cati utilizatori doresti sa creezi random?"<<endl;
                int ok_fix = 0;
                while(true)
                {
                    std::getline(std::cin, beta);
                    std::stringstream ss(beta);
                    if( ss >> random )
                    {
                        int digits = static_cast<int> ( log10( static_cast<double>(random) ) ) + 1;
                        break;
                    }
                    else if(ok_fix == 0) ok_fix = 1;
                    else
                    {
                        cout<<"Cati utilizatori doresti sa creezi random?"<<endl;
                        cout<<"Eroare introdu un numar"<<endl;
                    }
                }
                int counter = 0;
                while (counter < random)
                {
                    char users[e];
                    char letters[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                                       'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                                       's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
                                     };

                    std::random_device r;
                    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
                    std::shuffle(std::begin(letters), std::end(letters),
                                 std::mt19937(seed));
                    char rand_username[32];
                    rand_username[0]=0;
                    unsigned v1;
                    do
                    {
                        v1 = rand() % 32;
                    }
                    while(v1<5);
                    int i = 0;
                    for (char c : letters)
                    {
                        if(i == v1)
                            break;
                        rand_username[i] = c;
                        i++;
                    }
                    rand_username[strlen(rand_username)]='\0';
                    char rand_password[32];
                    rand_password[0]=0;
                    i = 0;
                    do
                    {
                        v1 = rand() % 32;
                    }
                    while(v1 < 5);
                    for (char c : letters)
                    {
                        if(i == v1)
                            break;
                        rand_password[i] = c;
                        i++;
                    }
                    rand_password[strlen(rand_password)]='\0';
                    char login[2], admin[2];
                    do
                    {
                        v1 = rand() % 2;
                    }
                    while(v1 < 0 && v1 == 2);
                    login[0] = v1 + '0';
                    if (v1 == 1)
                    {
                        do
                        {
                            v1 = rand() % 2;
                        }
                        while(v1 < 0 && v1 == 2);
                        admin[0] = v1 + '0';
                    }
                    else
                        admin[0] = '0';
                    char complete[256];
                    cout<<"Introdus cu succes user-ul " << rand_username << " cu parola " << rand_password << endl;
                    counter++;
                    string username_t1;
                    convertToString(username_t1, s, strlen(s));
                    string folder = "Root/"+ db_str + "/" + username_t1 + "/Random_User";
                    auto fl = mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                    char *folder_char = new char[sizeof(folder)/sizeof(string)];
                    folder_char = (char*)folder.c_str();
                    strcat(folder_char, "/");
                    strcat(folder_char, rand_username);
                    auto check = mkdir(folder_char, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                    if (!check)
                        printf("Directory created\n");
                    rand_username[0]=0;
                    rand_password[0]=0;
                    timer = clock() - timer;
                    ha<<"Random user                           Time used "<<(float)timer/CLOCKS_PER_SEC<<" in seconds            User:"<<s<< "                                Login: 1\n";
                }
                continue;
            }
             if (e == 5)
            {
                clearscreen();
                cout<<"Afisarea de procese pentru userul "<<s<<endl;
                ifstream gamma(se.c_str());
                string s[100];
                int i=0;
                while(getline(gamma, s[i]))
                {
                    cout<<s[i]<<endl;
                    i++;
                }
                gamma.close();
            }
        }
    }
}
#endif
