#include <stdio.h>
#include <stdlib.h>
#include "object.h"
#include "shared_ptr.h"
#include "main.h"

/*************************************
     Functions
*************************************/

// set - устанавливает значения от a до b в value
void set(shared_ptr ptrObject, unsigned int a, unsigned int b, int value) {
	unsigned int i;
	for (i = a; i <= b; ++i) {
		ptrObject->setValue(i, value);
	}
}

// show - выводит на экран значения от a до b
void show(shared_ptr ptrObject, unsigned int a, unsigned int b) {
	unsigned int i;
	int value;
	for (i = a; i <= b; ++i) {
		value = (*ptrObject).getValue(i);
		printf("array[%d] = %d\n", i, value);
	}
	printf("---------------------\n");
}
// make - создает объект, заполняет его значениями value
shared_ptr make(int value) {
	shared_ptr ptr = new Object();
	set(ptr, 0, 999, value);
	return ptr;
}



int main(void) {
	//инициализация
	shared_ptr p1 = new Object();

	//создание еще одного указателя на этот же объект;
	shared_ptr p2 = p1;	

	// изменение значения указателя на другой объект
	shared_ptr p3 = new Object();
	p3 = p2;

	//может принимать нулевое значение, проблем при вызове деструктора не будет
	shared_ptr pNull = 0;

	//использование оператора присваивания с обычным указателем на объект
	p3 = new Object();
	p3 = 0;

	//передаем умный указатель функции в качестве параметра
	// p1 и p2 ссылаются на один и тот же объект
	set(p2, 0, 7, 1);
	set(p2, 5, 10, 2);
	show(p1, 0, 10);
	
	//возвращаем из функции 
	p3 = make(13);
	set(p3, 0, 7, 12);
	show(p3, 0, 15);
 	return 0;	
}
