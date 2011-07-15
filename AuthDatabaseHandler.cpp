/*
 ============================================================================
 Name        : AuthDatabaseHandler.c
 Author      : Ashley Pope 
 Version     : -1.0
 Copyright   : ...nobody would copy this (i hope)
 Description : Basic game server
 ============================================================================
 */

#include "AuthDatabaseHandler.h"

AuthDatabaseHandler::AuthDatabaseHandler(char *database_name) {
  strcpy(m_databaseName, database_name);
}

int AuthDatabaseHandler::database_connect() {
  m_errorCheck = sqlite3_open(m_databaseName, &m_databaseConn);
  if (m_errorCheck) {
      return 0;
  }
  return 1;
}

int AuthDatabaseHandler::auth_user(char *username, char *password) {
  if (database_connect() == 0) {
      return 0;
  }
  
  std::string auth_sql = "select * from auth_login where username=? and password=?";
  
  m_errorCheck = sqlite3_prepare_v2(m_databaseConn, auth_sql.c_str(), auth_sql.length(), &m_databaseStmt, &m_databaseTail);
  sqlite3_bind_text(m_databaseStmt, 1, username, -1, SQLITE_STATIC);
  sqlite3_bind_text(m_databaseStmt, 2, password, -1, SQLITE_STATIC);

  m_errorCheck = sqlite3_step(m_databaseStmt);
  
  sqlite3_finalize(m_databaseStmt);
  sqlite3_close(m_databaseConn);

  if (m_errorCheck == 100) {
      return 1;
  } else {
      return 0;
  }
}


