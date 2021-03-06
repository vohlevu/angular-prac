#include <node.h>
#include "dtv/defs_dtv_sysinfo.h"
#include "dtvcontroller.h"
#include "database.h"
#include "sysinfo.h"
#include "connect_request.h"

using namespace node;
using namespace v8;

CDatabase	*m_cDatabase = NULL;
CDtvCtrl	*m_cDtvCtrl = NULL;
CSysInfo	*m_CSysInfo = NULL;
Persistent<Function> callback;
void initDTV();
void ReadInfoDtvToNand();
void initDatabase();
void socketCallbackFunc(char* data);
void UV_NOP(uv_work_t* req);
void callbackToJavaScript(uv_work_t *req, int status);
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
	
	return scope.Close(String::New(time));
}

Handle<Value> getChannelNameById(const Arguments& args) {
	HandleScope scope;
	if (args.Length() < 1) {
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close(Undefined());
	}
    Local<Integer> integer = args[0]->ToInteger();
    int32_t seq = integer->Value();
	printf("\n================Handle<Value> getChannelNameById===========================%d\n", seq);
	
	char result[SHORT_NAME_LENGTH];
	strcpy(result, m_cDtvCtrl->GetChannelNameById(seq));

	return scope.Close(String::New(result));
}

Handle<Value> getTotalChannel(const Arguments& args) {
	HandleScope scope;
	printf("\n================Handle<Value> getTotalChannel===========================\n");
	int32_t result = m_cDtvCtrl->GetTotalChannel();

	return scope.Close(Integer::New(result));
}

Handle<Value> getEpgInfoString(const Arguments& args) {
	HandleScope scope;
	if (args.Length() < 1) {
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close(Undefined());
	}
	char *c_arg;
	String::Utf8Value str(args[0]->ToString());
	c_arg = *str;
	printf("\n================Handle<Value> getEpgInfoString===========================%s\n", c_arg);
	string data = m_cDtvCtrl->CreateEPGInfoString(c_arg);
	//const char *ret = data.c_str();
	char *ret = new char[data.length() + 1];
	strcpy(ret, data.c_str());
	//delete [] ret;
	return scope.Close(String::New(ret));
}

Handle<Value> registerSocketCallbackFunc(const Arguments& args) {
	HandleScope scope;
	// Ensure that we got a callback. Generally, your functions should have
	// optional callbacks. In this case, you can declare an empty
	// Local<Function> handle and check for content before calling.
	if (!args[0]->IsFunction()) {
		return ThrowException(Exception::TypeError(
			String::New("First argument must be a callback function")));
	}
    // There's no ToFunction(), use a Cast instead.
	Local<Function> callbackLocal = Local<Function>::Cast(args[0]);
    callback = Persistent<Function>::New(callbackLocal);

    return Undefined();
}
// >>>>>>>>>>>>>>>>>>>>>>> Callback <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void socketCallbackFunc(char* data) {
	printf("\n\n========== socketCallbackFunc =============%s\n\n", data);
	// We need to pass this over to the nodejs thread, so it can create V8 objects
	connect_request* ar = new connect_request();
	ar->setData(data);
	//ar->Ref();
	
	uv_work_t* uv = new uv_work_t;
	uv->data = ar;
	int r = uv_queue_work(uv_default_loop(), uv, UV_NOP, callbackToJavaScript);
	if (r != 0) {
		printf("err: %d while queuing message", r);
		//ar->Unref();
		delete ar;
		delete uv;
	}
}

void UV_NOP(uv_work_t* req) { /* No operation */ }

// Use it because we call from other thread
void callbackToJavaScript(uv_work_t *req, int status) {
	HandleScope scope;
	
	connect_request* r = static_cast<connect_request* >(req->data);
	char data[20];
	r->getData(data);
	printf("\n\n========== callbackToJavaScript =============%s\n\n", data);
	
	TryCatch try_catch;
	// In case the operation succeeded, convention is to pass null as the
	// first argument before the result arguments.
	const unsigned argc = 1;
	Local<Value> argv[argc] = {
		Local<Value>::New(String::New(data))
	};

	// Note: When calling this in an asynchronous function that just returned
	// from the threadpool, you have to wrap this in a v8::TryCatch.
	// You can also pass another handle as the "this" parameter.
	callback->Call(Context::GetCurrent()->Global(), argc, argv);
	
	if (try_catch.HasCaught()) {
		FatalException(try_catch);
	}
}
// >>>>>>>>>>>>>>>>>>>>>>> RegisterModule <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void RegisterModule(Handle<Object> target) {
	initDTV();
	//initDatabase();
    target->Set(String::NewSymbol("fibonacci"), FunctionTemplate::New(Fibonacci)->GetFunction());
    target->Set(String::NewSymbol("getTime"), FunctionTemplate::New(getTime)->GetFunction());
    target->Set(String::NewSymbol("getEpgInfoString"), FunctionTemplate::New(getEpgInfoString)->GetFunction());
    target->Set(String::NewSymbol("getChannelNameById"), FunctionTemplate::New(getChannelNameById)->GetFunction());
    target->Set(String::NewSymbol("getTotalChannel"), FunctionTemplate::New(getTotalChannel)->GetFunction());
    target->Set(String::NewSymbol("registerSocketCallbackFunc"), FunctionTemplate::New(registerSocketCallbackFunc)->GetFunction());
}

NODE_MODULE(controller, RegisterModule);

// >>>>>>>>>>>>>>>>>>>>>>> CDtvCtrl <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
SDtvParams dtv_sysInfoParams;
void initDTV() {
	if (!m_cDtvCtrl) {
		m_cDtvCtrl = new CDtvCtrl();
	}
	m_cDtvCtrl->Init();
	m_cDtvCtrl->SetSocketCallbackFunc(&socketCallbackFunc);
	memset(&dtv_sysInfoParams, 0, sizeof(SDtvParams));
	m_cDtvCtrl->SetDtvSysInfoParams(&dtv_sysInfoParams);
	if (!m_CSysInfo) {
		m_CSysInfo = new CSysInfo();
	}
	ReadInfoDtvToNand();
}

void ReadInfoDtvToNand() {
	RMuint8 tunerID = 0;
	m_cDtvCtrl->SwitchToDtvPort(tunerID);
	m_cDtvCtrl->ResetDtvSystemInfo(tunerID);
	if(m_CSysInfo->Readdtv(&dtv_sysInfoParams)== RM_OK)
	{
		if(m_cDtvCtrl->RestoreChannelInfo(tunerID, TRUE)==RM_OK){
			return;
		}
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