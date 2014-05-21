#include <node.h>
#include "dtvcontroller.h"
#include "include/database.h"

using namespace v8;

CDatabase	*m_cDatabase = NULL;
CDtvCtrl	*m_cDtvCtrl = NULL;
void init();
void initDatabase();
void test(void* s);
// Returns the Nth number in the fibonacci sequence where N is the first
// argument passed.
Handle<Value> Fibonacci(const Arguments& args) {
    HandleScope scope;

    // Check that there are enough arguments. If we access an index that doesn't
    // exist, it'll be Undefined().
    if (args.Length() < 1) {
        // No argument was passed. Throw an exception to alert the user to
        // incorrect usage. Alternatively, we could just use 0.
        return ThrowException(
            Exception::TypeError(String::New("First argument must be a number"))
        );
    }

    // Cast a value to a specific type. See
    // http://izs.me/v8-docs/classv8_1_1Value.html for available To*() functions
    // and type checking functions. When converting to integer, make sure the
    // POD type you use is big enough!
    Local<Integer> integer = args[0]->ToInteger();
    int32_t seq = integer->Value();

    // Also possible in one call. (Don't forget HandleScope, otherwise the
    // intermediate Local handle won't be cleaned up!)
    // int32_t seq = args[0]->ToInteger()->Value();

    // Check for invalid parameter.
    if (seq < 0) {
        return ThrowException(Exception::TypeError(String::New(
            "Fibonacci sequence number must be positive")));
    }

    // The actual algorithm.
    int32_t current = 1;
    for (int32_t previous = -1, next = 0, i = 0; i <= seq; i++) {
        next = previous + current;
        previous = current;
        current = next;
    }

    return scope.Close(Integer::New(current));
}

Handle<Value> getTime(const Arguments& args) {
	HandleScope scope;
	printf("\n================Handle<Value> getTime===========================\n");
	char time[DATETIME_SLEN];
	strcpy(time, m_cDtvCtrl->GetUnixTime());
	/*if (!m_cDtvCtrl) {
		time = m_cDtvCtrl->GetTime();
	} else {
		strcpy(time, "undefined");
	}*/
	return scope.Close(String::New(time));
}

void RegisterModule(Handle<Object> target) {
	init();
	//initDatabase();
    target->Set(String::NewSymbol("fibonacci"),
        FunctionTemplate::New(Fibonacci)->GetFunction());
    target->Set(String::NewSymbol("getTime"),
        FunctionTemplate::New(getTime)->GetFunction());
}

NODE_MODULE(controller, RegisterModule);

// >>>>>>>>>>>>>>>>>>>>>>> CDtvCtrl <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void init() {
	if (!m_cDtvCtrl) {
		m_cDtvCtrl = new CDtvCtrl();
		m_cDtvCtrl->Init();
		//printf("\n TEST JSON >>> %s \n\n", m_cDtvCtrl->TestJson()==0?"SUCCESS":"FAIL");
		//m_cDtvCtrl->TestCurl();
	}
}

// >>>>>>>>>>>>>>>>>>>>>>> CDatabase <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void test(void* s) {
	printf("\n\n========== %s =============\n\n",(char*) s);
}
void initDatabase() {
	int rows, columns;
	char **table;
	char *err_msg;

	m_cDatabase = new CDatabase(&test);	
	m_cDatabase->OpenDatabase((char*)"../userdata/newdatabase.db", 0);	
	m_cDatabase->CreateNewTable((char*)"CREATE TABLE IF NOT EXISTS VOD(VodID TEXT PRIMARY KEY, VodType TEXT, FileName TEXT, FileSize NUMERIC, FileType TEXT, Date TEXT, Path TEXT)");
	char strSQL1[50] = "INSERT INTO VOD VALUES(\"VOD";
	char strSQL2[100] = "\", \"DSMCC-VOD\", \"Avatar\", 172000000, \"mp4\", \"2013-05-02\", \"/usb/VOD/mp4/Avatar.mp4\")";
	char *sqlstr = (char*)malloc(sizeof(char*)*255);
	for (int i = 0; i < 10; i++){
		sprintf(sqlstr, "%s%d%s", strSQL1, i, strSQL2);
		if (m_cDatabase->ExecuteQuery(sqlstr) == RM_ERROR){
			break;
		}		
	}
	if (m_cDatabase->ExecuteQuery((char*)"select * from VOD", &table, &rows, &columns, &err_msg) != RM_ERROR){
		if (rows > 0)
		for (int i = 0; i < rows; i++){
			for (int j = 0; j < columns; j++)
			{
				printf("%s\t", table[i*columns + j]);
			}
			printf("\n");
		}
		printf("\n");
	}else{
		printf("\nError: %s\n", err_msg);
	}
	//int primaryKey;
	//m_cDatabase->CheckPrimaryKey("VOD", "VodID", &primaryKey);
	//printf("\nm_cDatabase->CheckPrimaryKey : %d\n", primaryKey);
	//m_cDatabase->CheckPrimaryKey("VOD", "VodType", &primaryKey);
	//printf("\nm_cDatabase->CheckPrimaryKey : %d\n", primaryKey);
	//m_cDatabase->RenameColumn("VOD", "VodID", "VodID2");
	char *sqlStr = (char*)malloc(sizeof(char*)*255);
	char* nameTable = (char*)malloc(sizeof(char*)*255);
	strcpy(nameTable, "VOD");
	sprintf(sqlStr, "select * from %s where 0=1", nameTable);
	if (m_cDatabase->ExecuteQuery(sqlStr) == RM_ERROR){
		printf("\n====== ExecuteQuery(sqlStr) == RM_ERROR ======\n");
	} else {
		printf("\n====== ExecuteQuery(sqlStr) == RM_OK ======\n");
		for (int j = 0; j < columns; j++)
		{
			char *s = (char*)malloc(sizeof(char*)*255);
			memset(s, '\0', 255);
			m_cDatabase->CheckColumnType(j, s);
			//m_cDatabase->CheckColumnType((char*)"VOD", j, s);
			printf("\nColumn[%d] Type: %s\n", j, s);
		}
	}
	delete(m_cDatabase);
}