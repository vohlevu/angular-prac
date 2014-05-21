#include "database.h"

/******************************************************************************
* FUNCTION: 
*  CDatabase::CDatabase()
*  DESCRIPTION:
*			
* @PARAM: 
* @RETURN: 
* NOTE: 
******************************************************************************/
void (*MyFunc)(void*);
CDatabase::CDatabase(void (*func_pointer)(void*))
{
#if 0
	dataPath = (char*)malloc(sizeof(char*)*strlen(DATABASE_PATH));
	strcpy(dataPath, DATABASE_PATH);
	printf("Database >> Open databse default with path [%s]\n", dataPath);	
	int result = sqlite3_open(dataPath, &pdb);
	//if (result == SQLITE_CANTOPEN){
	if (result != SQLITE_OK){
		ShowErrorMessage(result);
		pdb = NULL;
	}
	selectStmt = NULL;	
	dataPath	= NULL;
#else
	pdb 		= NULL;
	selectStmt 	= NULL;
	dataPath	= NULL;
	func_pointer((void*)"init CDatabase...");
	MyFunc = func_pointer;
	MyFunc((void*)"Testing........");
#endif
}

/******************************************************************************
* FUNCTION: 
*  CDatabase::~CDatabase()
*  DESCRIPTION:
*			
* @PARAM: 
* @RETURN: 
* NOTE: 
******************************************************************************/
CDatabase::~CDatabase()
{
	printf("Database >> ~CDatabase\n");
	CloseDatabase();
}

/******************************************************************************
* FUNCTION: 
*  			OpenDatabase(char* _datapath)
*  DESCRIPTION:
*	open a database		
* @PARAM: 
*	_datapath: path database
*	flag:
*		0: open read and write  mode
*		1: open readonly mode
* @RETURN: 
* NOTE: 
******************************************************************************/
RMstatus CDatabase::OpenDatabase(char* _dataPath, int flag)
{
	//clear all old data
	//CloseDatabase();
	printf("Database >> OpenDatabase [name = %s][open mode = %d]\n", _dataPath, flag);
	dataPath = (char*)malloc(sizeof(char*)*strlen(_dataPath));
	strcpy(dataPath, _dataPath);
	int result = -1;
	if (flag ==0){
		result = sqlite3_open(dataPath, &pdb);
		printf("Open database with read and write  mode\n");
		//result = sqlite3_open_v2(dataPath, &pdb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL ); //file name with format utf8
	}else{
		result = sqlite3_open_v2(dataPath, &pdb, SQLITE_OPEN_READONLY, NULL );	
		printf("\n\t\033[31m==================================================\n");
		printf("\t==                                              ==\n");
		printf("\t==      OPEN DATABASE WITH READONLY MODE        ==\n");
		printf("\t==                                              ==\n");
		printf("\t==================================================\033[0m\n");
	}
	//if (result == SQLITE_CANTOPEN){
	if (result != SQLITE_OK){
		printf("Database >> Error for [%s]\n", dataPath);
		ShowErrorMessage(result);
		pdb = NULL;
		return RM_ERROR;
	}
	selectStmt = NULL;
	return RM_OK;
}

/******************************************************************************
* FUNCTION: 
*  			CloseDatabase()
*  DESCRIPTION:
*	create a new database			
* @PARAM: 
*	NONE
* @RETURN: 
* NOTE: 
******************************************************************************/
void CDatabase::CloseDatabase()
{
	if (pdb == NULL)
		return;
	sqlite3_close(pdb);		
	sqlite3_finalize(selectStmt);
	sqlite3_shutdown();
	printf("Database >> Close databse with path [%s]\n", dataPath);
	dataPath = NULL;
	pdb = NULL;
}

/******************************************************************************
* FUNCTION: 
*  		CreateNewTable(char *sqlCmd)
*  DESCRIPTION:
*		create a new table
* @PARAM: 
*		sqlCmd: query create table commmand
* @RETURN: 
* 		RM_OK: query is Ok 
* 		RM_ERROR: query is error 
******************************************************************************/
RMstatus CDatabase::CreateNewTable(char *sqlStr)
{
	if (pdb == NULL){
		printf("\n\033[31mError for [%s]\n", sqlStr);
		printf("Database is not open, please open a database.\033[0m\n\n");
		return RM_ERROR;
	}
	char *sql = (char*)malloc(sizeof(char*)*strlen(sqlStr));
	sprintf(sql, "%s;", sqlStr);
	int result = sqlite3_prepare_v2(pdb, sql, strlen(sql)+1, &selectStmt, NULL);
	if (result != SQLITE_OK){
		printf("\nError for sqlCmd = [%s]\n", sql);
		ShowErrorMessage(result);
		return RM_ERROR;
	}
	if (sqlite3_step(selectStmt) != SQLITE_DONE){ 
		printf("Database >> Create Table is ERROR, opened database with readonly\n");
		return RM_ERROR;
	}	
	printf("CreateNewTable is OK\n");
	return RM_OK;
}

/******************************************************************************
* FUNCTION: 
*  		ExecuteQuery(char *sqlCmd)
*  DESCRIPTION:
*		execute a query such as insert, update, delete
* @PARAM: 
*		sqlCmd: query commmand
* @RETURN: 
* 		RM_OK: query is Ok 
* 		RM_ERROR: query is error 
******************************************************************************/
RMstatus CDatabase::ExecuteQuery(char *sqlCmd)
{
	if (pdb == NULL){
		printf("\n\033[31mError for [%s]\n", sqlCmd);
		printf("Database is not open, please open a database.\033[0m\n");
		return RM_ERROR;
	}
	char * sqlstr = (char*)malloc(sizeof(char*)*strlen(sqlCmd));
	sprintf(sqlstr, "%s;", sqlCmd);
	int result = sqlite3_prepare(pdb, sqlstr, strlen(sqlstr)+1, &selectStmt, NULL);	
	if (result != SQLITE_OK){
		printf("\nError for sqlCmd = [%s]\n", sqlCmd);
		ShowErrorMessage(result);
		return RM_ERROR;
	}
	if (sqlite3_step(selectStmt) != SQLITE_DONE){ 
		printf("\nError for sql = [%s]", sqlCmd);
		printf("\nError Can not commit, opend database with readonly mode or insert with primary is the same\n");
		return RM_ERROR;
	}
	//int rowchange = sqlite3_changes(pdb);
	//printf("\nsqlite3_changes = [%d]\n", rowchange);
	printf("\nExecuteQuery query = [%s]\n", sqlstr);
	return RM_OK;
}

/******************************************************************************
* FUNCTION: 
*  		ExecuteQuery(char *sqlCmd, char ***tableResult, int *row, int *column, char **errmsg)
*  DESCRIPTION:
*		execute a query have get data
* @PARAM: 
*		sqlCmd		: query commmand
*		tableResult : data of select command 
*		row			: row(record) of select command 
*		column		: column(field) of select command 
*		errmsg		: error message if error
* @RETURN: 
* 		RM_OK: query is Ok 
* 		RM_ERROR: query is error 
******************************************************************************/
RMstatus CDatabase::ExecuteQuery(char *sqlCmd, char ***tableResult, int *row, int *column, char **errmsg)
{
	if (pdb == NULL){
		printf("\n\033[31mError for [%s]\n", sqlCmd);
		printf("Database is not open, please open a database.\033[0m\n\n");
		return RM_ERROR;
	}

	char * sqlstr = (char*)malloc(sizeof(char*)*strlen(sqlCmd));
	sprintf(sqlstr, "%s;", (char*)sqlCmd);
	int result = sqlite3_get_table(pdb, sqlstr, tableResult, row, column, errmsg);
	if (result != SQLITE_OK){
		*row = 0;
		*column = 0;
		printf("\nError for sqlCmd = [%s]\n", sqlstr); 
		ShowErrorMessage(result);
		return RM_ERROR;
	}
	printf("\nExecuteQuery exec: [%s]", sqlstr);
	printf("\nExecuteQuery exec: (Row, Col) = (%d, %d), err_msg = %s\n", *row, *column,  *errmsg);
	return RM_OK;
}

/******************************************************************************
* FUNCTION: 
*  		DeleteDatabase()
*  DESCRIPTION:
*		delete database
* @PARAM: 
*		NONE
* @RETURN: 
* 		RM_OK: delete success
* 		RM_ERROR: delete error
******************************************************************************/
RMstatus CDatabase::DeleteDatabase(char *datapath)
{
		
	if (dataPath == NULL || strcmp(dataPath, "") == 0){
		printf("\nCan't delete: database is not opened\n");
		return RM_ERROR;
	}	
	char *cmd  = new char[10 + strlen(datapath)];
	sprintf(cmd, "rm -rf %s", datapath);
	int status = system(cmd);
	if (status == 0){
		printf("\nDeleteCurrentDatabase: [%s] is deleted\n", datapath);
		if (strcmp(datapath, dataPath) == 0)//delete database current
			CloseDatabase();
		return RM_OK;
	}
	printf("\nDeleteCurrentDatabase: ERROR >> [%s][error code = %d] can't delete\n", datapath, status);
	return RM_ERROR;
}

/******************************************************************************
* FUNCTION: 
*  		DropColumns(char *nameTable, char *column_need_del)
*  DESCRIPTION:
*		drop colums(s) in table
* @PARAM: 
*		char *nameTable
*		char *column_need_del
*
* @RETURN: 
* 		RM_OK: delete success
* 		RM_ERROR: delete error
******************************************************************************/
RMstatus CDatabase::DropColumn(char *nameTable, char *column_need_del)
{
	//get all column name in table nameTable
	char *sqlStr = (char*)malloc(sizeof(char*)*255);
	sprintf(sqlStr, "select * from %s where 0=1", nameTable);
	if (ExecuteQuery(sqlStr) == RM_ERROR){	
		return RM_ERROR;
	}
	int cols = sqlite3_column_count(selectStmt);
	printf("columns count: %d\n", cols);
	if (cols <= 0){
		printf("\nERROR: table %s do not exist columns, column count = 0\n", nameTable);
		return RM_ERROR;
	}
	printf("\nColumns need delete: [%s]\n", column_need_del);
	char columns[/*cols*/10][100];
	//std::vector<char[100]> columns(cols); 
	int idx_col_del = -1;
	printf("\nAll Columns in table: ");
	for (int i = 0; i < cols; i++){
		strcpy(columns[i], sqlite3_column_name(selectStmt, i));
		if (strcmp(columns[i], column_need_del) == 0)
			idx_col_del = i;
	}
	if (idx_col_del == -1 || idx_col_del > cols){
		printf("\nERROR: Can not find column [%s] in table [%s]\n", column_need_del, nameTable);
		return RM_ERROR;
	}else{
		printf("\nWill delete column [%s] in table [%s]\n", columns[idx_col_del], nameTable);
	}
	
	//1. rename
	sprintf(sqlStr, "ALTER TABLE %s RENAME TO %s_old", nameTable, nameTable);
	if (ExecuteQuery(sqlStr) == RM_ERROR){
		printf("\nError: can not rename table when drop column\n");
		return RM_ERROR;
	}
	//2. create new table without 'column_need_del'	
	char *listField = (char*)malloc(sizeof(char*)*255);
	strcpy(listField, "");
	for (int i = 0; i < cols; i ++){
		if (i != idx_col_del){
			strcat(listField, columns[i]);
			if (idx_col_del == (cols - 1)){ //idx_col_del is end of
				if (i != (idx_col_del - 1))
					strcat(listField, ",");
			}else{
				if (i != (cols - 1))
						strcat(listField, ",");
			}
		}
	}
	
	sprintf(sqlStr, "CREATE TABLE IF NOT EXISTS  %s(%s)", nameTable, listField);
	printf("\nList file: %s\n", listField);
	if (ExecuteQuery(sqlStr) == RM_ERROR){
		sprintf(sqlStr, "ALTER TABLE %s_old RENAME TO %s", nameTable, nameTable);
		ExecuteQuery(sqlStr);
		return RM_ERROR;
	}	
	
	//3. copy all data from table temple to table without column_need_del
	sprintf(sqlStr, "INSERT INTO %s(%s)  SELECT %s from %s_old", nameTable, listField, listField, nameTable);
	if (ExecuteQuery(sqlStr) == RM_ERROR){
		printf("\ncan not fill data from temple table to new table-> delete new table and rename table\n");
		sprintf(sqlStr, "DROP TABLE %s", nameTable);
		ExecuteQuery(sqlStr);
		
		sprintf(sqlStr, "ALTER TABLE %s_old RENAME TO %s", nameTable, nameTable);
		ExecuteQuery(sqlStr);
		return RM_ERROR;
	}
	
	//4. drop table temple
	sprintf(sqlStr, "drop table if exists %s_old", nameTable);
	printf("\nDROP TABLE CMD: %s", sqlStr);
	ExecuteQuery(sqlStr);
	return RM_OK;
}

/******************************************************************************
* FUNCTION: 
*  		AddColumn(char *nameTable, char *columnname, char *valueType)
*  DESCRIPTION:
*		add a column "columnname" with type value is 'valueType' in table nameTable
* @PARAM: 
*		char *nameTable
*		char *columnname
*		char *valueType
*
* @RETURN: 
* 		RM_OK: delete success
* 		RM_ERROR: delete error
******************************************************************************/
RMstatus CDatabase::AddColumn(char *nameTable, char *columnname, char *data_type)
{
	char *addColumn = (char*)malloc(sizeof(char*)*255);
	sprintf(addColumn, "ALTER TABLE %s ADD COLUMN %s %s", nameTable, columnname, data_type);
	return ExecuteQuery(addColumn);
}

/******************************************************************************
* FUNCTION: 
*  		RenameColumn(char *nameTable, char *columnname, char *columnnamenew)
*  DESCRIPTION:
*		rename column "columnname" in table "columnname" to "columnnamenew"
* @PARAM: 
*		char *nameTable
*		char *columnname
*		char *columnnamenew
*
* @RETURN: 
* 		RM_OK: delete success
* 		RM_ERROR: delete error
******************************************************************************/
RMstatus CDatabase::RenameColumn(char *nameTable, char *columnname, char *new_column_name)
{
	char *sqlStr = (char*)malloc(sizeof(char*)*255);
	sprintf(sqlStr, "select * from %s where 0=1", nameTable);
	if (ExecuteQuery(sqlStr) == RM_ERROR){	
		return RM_ERROR;
	}
	int cols = sqlite3_column_count(selectStmt);
	printf("columns count: %d\n", cols);
	if (cols <= 0){
		printf("\nERROR: table %s do not exist columns, column count = 0\n", nameTable);
		return RM_ERROR;
	}
	char *listField_old = (char*)malloc(sizeof(char*)*255);
	char *listField_new = (char*)malloc(sizeof(char*)*255);
	int idx_col_del = -1;	
	strcpy(listField_old, "");
	strcpy(listField_new, "");
	for (int i = 0; i < cols; i++){
		strcat(listField_old, sqlite3_column_name(selectStmt, i));
		if (strcmp(sqlite3_column_name(selectStmt, i), columnname) == 0){
			strcat(listField_new, new_column_name);
			idx_col_del = i;
		}else{
			strcat(listField_new, sqlite3_column_name(selectStmt, i));
		}
		if (i != (cols - 1)){
			strcat(listField_old, ",");
			strcat(listField_new, ",");
		}
	}
	
	//test
	//CheckPrimaryKey(nameTable, new_column_name);
	//end test
	
	if (idx_col_del == -1 || idx_col_del > cols){
		printf("\nERROR: Can not find column [%s] in table [%s]\n", columnname, nameTable);
		return RM_ERROR;
	}else{
		printf("\nWill rename column [%s] in table [%s] to [%s]\n",columnname, nameTable, new_column_name);
		printf("\nList field old [%s]\n", listField_old);
		printf("\nList field new [%s]\n", listField_new);
	}
	/*
	sprintf(sqlStr, "ALTER TABLE %s RENAME TO %s_old", nameTable, nameTable);
	if (ExecuteQuery(sqlStr) == RM_ERROR){
		printf("\nError: can not rename table when drop column\n");
		return RM_ERROR;
	}

	sprintf(sqlStr, "CREATE TABLE IF NOT EXISTS  %s(%s)", nameTable, listField_new);
	if (ExecuteQuery(sqlStr) == RM_ERROR){
		sprintf(sqlStr, "ALTER TABLE %s_old RENAME TO %s", nameTable, nameTable);
		ExecuteQuery(sqlStr);
		return RM_ERROR;
	}	
	
	sprintf(sqlStr, "INSERT INTO %s(%s)  SELECT %s from %s_old", nameTable,listField_new, listField_old, nameTable);
	if (ExecuteQuery(sqlStr) == RM_ERROR){
		printf("\ncan not fill data from temple table to new table-> delete new table and rename table\n");
		sprintf(sqlStr, "DROP TABLE %s", nameTable);
		ExecuteQuery(sqlStr);
		sprintf(sqlStr, "ALTER TABLE %s_old RENAME TO %s", nameTable, nameTable);
		ExecuteQuery(sqlStr);
		return RM_ERROR;
	}
	//4. drop table temple
	sprintf(sqlStr, "drop table if exists %s_old", nameTable);
	printf("\nDROP TABLE CMD: %s", sqlStr);
	ExecuteQuery(sqlStr);*/
	return RM_OK;
}

bool CDatabase::CheckPrimaryKey(char * tableName, char* columnName, int* primaryKey)
{
    const char *datatype;
    const char *colseq;
    int    NotNull, Autoinc;
	int rc  = sqlite3_table_column_metadata(pdb, NULL, tableName, columnName, &datatype, &colseq, &NotNull, primaryKey, &Autoinc);
	if (rc==SQLITE_OK){
		printf("\nTable info and column info:");
		printf("\ndatatype  : %s", datatype);
		printf("\ncolseq    : %s", colseq);
		printf("\nNotNull   : %d", NotNull);
		printf("\nPrimaryKey: %d", *primaryKey);
		printf("\nAutoinc   : %d", Autoinc);
		return true;
	}else{
		printf("\nError sqlite3_table_column_metadata: %s", sqlite3_errmsg(pdb));
		return false;
	}
}
bool CDatabase::CheckColumnType(int iCol, char* datatype)
{
	if (selectStmt == NULL) {
		printf("\nCheckColumnType : selectStmt == NULL\n");
		return false;
	}
	int cols = sqlite3_column_count(selectStmt);
	//printf("\nCheckColumnType : sqlite3_column_count = %d\n", cols);
	if (iCol >= cols || iCol < 0) {
		printf("\nCheckColumnType : iCol out of index\n");
		return false;
	}
	/*printf("\nCheckColumnType : sqlite3_column_name = %s\n", sqlite3_column_name(selectStmt, iCol));
	switch (sqlite3_column_type(selectStmt, iCol)) {
		case SQLITE_INTEGER:
			strcpy(datatype, "INTEGER");
		break;
		case SQLITE_FLOAT:
			strcpy(datatype, "FLOAT");
		break;
		case SQLITE_TEXT:
			strcpy(datatype, "TEXT");
		break;
		case SQLITE_BLOB:
			strcpy(datatype, "BLOB");
		break;
		case SQLITE_NULL:
			//strcpy(datatype, "NULL");
			printf("\nCheckColumnType : SQLITE_NULL\n");
			return false;
	} */ /*switch*/
	//printf("\nCheckColumnType : %s\n", datatype);
	strcpy(datatype, sqlite3_column_decltype(selectStmt, iCol));
	//printf("\nCheckColumnType : %s\n", sqlite3_column_decltype(selectStmt, iCol));
	return true;
}
bool CDatabase::CheckColumnType(char * tableName, int iCol, char* datatype)
{
    const char *colseq, *type;
	char* columnName = (char*)malloc(sizeof(char*)*255);;
    int    NotNull, Autoinc, PrimaryKey;
	strcat(columnName, sqlite3_column_name(selectStmt, iCol));
	int rc  = sqlite3_table_column_metadata(pdb, NULL, tableName, columnName, &type, &colseq, &NotNull, &PrimaryKey, &Autoinc);
	if (rc==SQLITE_OK){
		//printf("\nTable info and column info:");
		printf("\ndatatype  : %s", type);
		strcpy(datatype, type);
		//printf("\ncolseq    : %s", colseq);
		//printf("\nNotNull   : %d", NotNull);
		//printf("\nPrimaryKey: %d", PrimaryKey);
		//printf("\nAutoinc   : %d", Autoinc);
		return true;
	}else{
		printf("\nError sqlite3_table_column_metadata: %s", sqlite3_errmsg(pdb));
		return false;
	}
}
/******************************************************************************
* FUNCTION: 
*  		ShowErrorMessage(int error_code)
*  DESCRIPTION:
*		show error message
* @PARAM: 
*		error_code: error code
* @RETURN: 
* 		NONE
******************************************************************************/
void CDatabase::ShowErrorMessage(int error_code)
{
	printf("\033[31m\t-Error_code: %d\n\t-\033[0m",error_code);
	if (error_code == SQLITE_OK)
		return;
	switch(error_code){
		//case SQLITE_OK:
		//	printf("Successful result\n");
		//	break;
		case  SQLITE_ERROR:
			printf("\033[31mError message: SQL error or missing database\033[0m\n");
			break;
		case  SQLITE_INTERNAL: 
			printf("\033[31mError message: Internal logic error in SQLite\033[0m\n");
			break;
		case  SQLITE_PERM:
			printf("\033[31mError message: Access permission denied\033[0m\n");
			break;
		case  SQLITE_ABORT:
			printf("\033[31mError message: Callback routine requested an abort\033[0m\n");
			break;
		case  SQLITE_BUSY:
			printf("\033[31mError message: The database file is locked\033[0m\n");
			break;
		case  SQLITE_LOCKED:
			printf("\033[31mError message: A table in the database is locked\033[0m\n");
			break;
		case  SQLITE_NOMEM:
			printf("\033[31mError message: A malloc() failed\033[0m\n");
			break;
		case  SQLITE_READONLY:
			printf("\033[31mError message: Attempt to write a readonly database\033[0m\n");
			break;
		case  SQLITE_INTERRUPT:
			printf("\033[31mError message: Operation terminated by sqlite3_interrupt()\033[0m\n");
			break;
		case  SQLITE_IOERR:
			printf("\033[31mError message: Some kind of disk I/O error occurred\033[0m\n");
			break;
		case  SQLITE_CORRUPT:
			printf("\033[31mError message: The database disk image is malformed\033[0m\n");
			break;
		case  SQLITE_NOTFOUND:
			printf("\033[31mError message: Unknown opcode in sqlite3_file_control()\033[0m\n");
			break;
		case  SQLITE_FULL:
			printf("\033[31mError message: Insertion failed because database is full\033[0m\n");
			break;
		case  SQLITE_CANTOPEN:
			printf("\033[31mError message: Unable to open the database file\033[0m\n");
			break;
		case  SQLITE_PROTOCOL:
			printf("\033[31mError message: Database lock protocol error\033[0m\n");
			break;
		case  SQLITE_EMPTY:
			printf("\033[31mError message: Database is empty\033[0m\n");
			break;
		case  SQLITE_SCHEMA:
			printf("\033[31mError message: The database schema changed\033[0m\n");
			break;
		case  SQLITE_TOOBIG:
			printf("\033[31mError message: String or BLOB exceeds size limit\033[0m\n");
			break;
		case  SQLITE_CONSTRAINT:
			printf("\033[31mError message: Abort due to constraint violation\033[0m\n");
			break;
		case  SQLITE_MISMATCH:
			printf("\033[31mError message: Data type mismatch\033[0m\n");
			break;
		case  SQLITE_MISUSE:
			printf("\033[31mError message: Library used incorrectly\033[0m\n");
			break;
		case  SQLITE_NOLFS:
			printf("\033[31mError message: Uses OS features not supported on host\033[0m\n");
			break;
		case  SQLITE_AUTH:
			printf("\033[31mError message: Authorization denied\033[0m\n");
			break;
		case  SQLITE_FORMAT:
			printf("\033[31mError message: Auxiliary database format error\033[0m\n");
			break;
		case  SQLITE_RANGE:
			printf("\033[31mError message: 2nd parameter to sqlite3_bind out of range\033[0m\n");
			break;
		case  SQLITE_NOTADB:
			printf("\033[31mError message: File opened that is not a database file\033[0m\n");
			break;
		case  SQLITE_ROW:
			printf("\033[31mError message: sqlite3_step() has another row ready\033[0m\n");
			break;
		case  SQLITE_DONE:
			printf("\033[31mError message: sqlite3_step() has finished executing\033[0m\n");
			break;
	}
}
