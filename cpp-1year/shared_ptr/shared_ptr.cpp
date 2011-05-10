#include <stdio.h>
#include "object.h"
#include "shared_ptr.h"

/*************************************
     Methods of class 'shared_ptr'
*************************************/
 
shared_ptr::shared_ptr(Object *ptr) {
	myStoragePtr = new storage(ptr);
}

shared_ptr::~shared_ptr() {
	deleteCopy();
}

shared_ptr::shared_ptr(const shared_ptr& ptr) {
	myStoragePtr = ptr.myStoragePtr;
	createCopy();	
}


shared_ptr& shared_ptr::operator=(const shared_ptr& ptr) {
	if (myStoragePtr != ptr.myStoragePtr) {
		deleteCopy();
		myStoragePtr = ptr.myStoragePtr;
		createCopy();
	}
	return *this;
}

shared_ptr& shared_ptr::operator=(Object *ptr) {
	if (myStoragePtr->myPointer != ptr) {
		deleteCopy();
		myStoragePtr = new storage(ptr);
	}
	return *this;
}

Object* shared_ptr::operator->() const {
	return myStoragePtr->myPointer;
}

Object& shared_ptr::operator*() const {
	return *(myStoragePtr->myPointer);
}
    
bool shared_ptr::isNull() const {
	return (myStoragePtr->myPointer == 0);
}

void shared_ptr::createCopy() {
	myStoragePtr->myCounter++;
}

void shared_ptr::deleteCopy() {
	myStoragePtr->myCounter--;
	if (myStoragePtr->myCounter == 0) {
		delete myStoragePtr;
	} 
}

/*************************************
     Methods of class 'storage'
*************************************/

shared_ptr::storage::storage(Object* ptr) {
	myPointer = ptr;
	myCounter = 1;	
}

shared_ptr::storage::~storage() {	
	if (myPointer != 0) {
		delete myPointer;
	}
}





