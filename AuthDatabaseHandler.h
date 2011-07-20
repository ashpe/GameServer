#include <sqlite3.h> 
#include <string.h>
#include <string>
#include <iostream>

class AuthDatabaseHandler {

    public: //naming for static
       AuthDatabaseHandler(char *database_name="auth_database");
       int add_unique_code(char *username, char *password, char *code);
       int auth_user(char *username, char *password);
       int auth_code(char *code);
       int getErrorCheck();

    private: //naming for members
       char m_databaseName[50];
       const char *m_databaseTail;
       sqlite3 *m_databaseConn;
       sqlite3_stmt *m_databaseStmt;
       int m_errorCheck;
       int database_connect();

};
