#include "headers/maximum.h"
#include "headers/login_sql.h"
#include <bits/stdc++.h>
using namespace std;

#include "headers/meniu.h"


void strcpy_safe(char *first, char *second)
{
  int i;
  for (i = 0; second[i] != '\0'; ++i) {
        first[i] = second[i];
    }

    first[i] = '\0';
}


int main()
{
    login_sql g;
    meniu_start h;
    int admin;
    cout<<"Bun venit in program"<<endl;
    if(h.verify("done")==0) h.startup();
    else
    {
        if(g.login(admin)==1)
            h.meniu(ver_user, admin, database_login);
        else cout<<"Userul si parola introdusa nu functioneaza, prin urmare se inchide programul"<<endl;
    }
    return 0;
}
