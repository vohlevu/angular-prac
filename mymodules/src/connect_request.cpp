#include "connect_request.h"

/**
 * Constructor: connect_request
 */
connect_request::connect_request() {
	strcpy(data, "");
};
/**
 * Deconstructor: connect_request
 */
connect_request::~connect_request() {
	free(data);
};

void connect_request::setData(char* d) {
	strcpy(data, d);
}

void connect_request::getData(char* d) {
	strcpy(d, data);
}