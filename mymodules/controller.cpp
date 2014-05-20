#include <node.h>
#include "dtvcontroller.h"

using namespace v8;

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

CDtvCtrl	*m_cDtvCtrl = NULL;
void init() {
	if (!m_cDtvCtrl) {
		m_cDtvCtrl = new CDtvCtrl();
		m_cDtvCtrl->Init();
		//printf("\n TEST JSON >>> %s \n\n", m_cDtvCtrl->TestJson()==0?"SUCCESS":"FAIL");
	}
}

Handle<Value> getTime(const Arguments& args) {
    HandleScope scope;
	printf("\n================Handle<Value> getTime===========================\n");
	char* time = m_cDtvCtrl->GetTime();
	/*if (!m_cDtvCtrl) {
		time = m_cDtvCtrl->GetTime();
	} else {
		strcpy(time, "undefined");
	}*/
	return scope.Close(String::New(time));
}

void RegisterModule(Handle<Object> target) {
	init();
    target->Set(String::NewSymbol("fibonacci"),
        FunctionTemplate::New(Fibonacci)->GetFunction());
    target->Set(String::NewSymbol("getTime"),
        FunctionTemplate::New(getTime)->GetFunction());
}

NODE_MODULE(controller, RegisterModule);
