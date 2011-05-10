#include "stdio.h"
#include "object.h"
#include "shared_ptr.h"

/*************************************
     Methods of class 'Object'
*************************************/

void Object::setValue(unsigned int i, int value) {
	if (i < 1000) {
		myBigArray[i] = value;
	}
}
int Object::getValue(unsigned int i) {
	if (i >= 1000) {
		return -1;	
	}
	return myBigArray[i];
}




