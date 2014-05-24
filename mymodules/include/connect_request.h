
#ifndef CONNECT_H
#define CONNECT_H

#include <node.h>
#include <string.h>
using namespace node;

class connect_request : public ObjectWrap {
	private:
		char data[20];
	public:    /**
		 * Constructor: connect_request
		 */
		connect_request();
		/**
		 * Deconstructor: connect_request
		 */
		~connect_request();
		
		void setData(char* data);
		
		void getData(char* data);
};
#endif