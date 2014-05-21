#ifndef DATABASE_H
#define DATABASE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "sqlite/sqlite3.h"

#define DATABASE_PATH 		"../userdata/data_sample_utf8.db"
#define RMstatus 		int
#define RM_OK 			1
#define RM_ERROR 		0

using namespace std;

class CDatabase
{
private:
	char 			*dataPath; 		//name of database
	sqlite3 		*pdb;			//pointer database
	sqlite3_stmt 	*selectStmt;  	//contain output of select query
public:
	CDatabase(void (*func_pointer)(void*));
	~CDatabase();
	RMstatus		OpenDatabase(char* _dataPath, int flag);
	void			CloseDatabase();
	RMstatus 		CreateNewTable(char *sqlStr);
	RMstatus 		ExecuteQuery(char *sqlCmd); //for insert, update, delete
	RMstatus 		ExecuteQuery(char *sqlCmd, char ***tableResult, int *row, int *column, char **errmsg);
	RMstatus 		DeleteDatabase(char *datapath);
	RMstatus 		DropColumn(char *nameTable, char *column_need_del);
	RMstatus 		AddColumn(char *nameTable, char *columnname, char *data_type);
	RMstatus 		RenameColumn(char *nameTable, char *columnname, char *new_column_name);
	void	 		ShowErrorMessage(int error_code);
	bool 			CheckPrimaryKey(char * tableName, char* columnName, int* primaryKey);
	bool 			CheckColumnType(char * tableName, int iCol, char* datatype);
	bool			CheckColumnType(int iCol, char* datatype);

};
#endif //DATABASE_H
