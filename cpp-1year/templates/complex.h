#ifndef _COMPLEX_H
#define _COMPLEX_H

template < typename T > class Complex {
	private:
		T myReal;
		T myImg;
	public:

		Complex(T real=0, T img=0);
		Complex& operator+ (const Complex&  complex) {
			myReal += complex.myReal;
			myImg  += complex.myImg;
			return *this;
		}
		template <typename D>
		void printD<D>(D d);
		Complex& operator* (const Complex& complex);
		void print();



		
		
};
	template <typename T>
	Complex<T>::Complex(T real, T img): myReal(real), myImg(img) { }

	template <typename T>
	void Complex<T>::print() {
		printf("%d + %dj\n",myReal, myImg);
	}

	template <typename T>
	template <typename D>
	void Complex<T>::printD<D>(D d) {
		cout << d << endl;
	}


	template <typename T>
	Complex<T>& Complex<T>::operator* (const Complex& complex) {
		T tmp_real = myReal;
		myReal = myReal*complex.myReal - myImg*complex.myImg;
		myImg  = tmp_real*complex.myImg + myImg*complex.myReal;
		return *this;
	}








#endif
