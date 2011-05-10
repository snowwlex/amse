#ifndef _SHARED_PTR_H
#define _SHARED_PTR_H


/*************************************
     Class shared_ptr
*************************************/

class shared_ptr {

private:

	class storage {

	private:

	public:

		Object* myPointer;
		int myCounter;

		storage(Object* ptr);
		~storage();

	};

	storage *myStoragePtr;

	void createCopy();
	void deleteCopy();
    
public:

	shared_ptr(Object *ptr);
	~shared_ptr();
	shared_ptr(const shared_ptr& ptr);

	shared_ptr& operator=(const shared_ptr& ptr);
	shared_ptr& operator=(Object *ptr);
	Object* operator->() const;
	Object& operator*() const;
	bool isNull() const;

};





#endif
