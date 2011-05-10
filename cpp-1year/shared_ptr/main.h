#ifndef _MAIN_H
#define _MAIN_H

/*************************************
     Functions
*************************************/

// set - устанавливает значения от a до b в value;
void set(shared_ptr ptrObject, unsigned int a, unsigned int b, int value);
// show - выводит на экран значения от a до b
void show(shared_ptr ptrObject, unsigned int a, unsigned int b);
// make - создает объект, заполняет его значениями value
shared_ptr make(int value);



#endif
