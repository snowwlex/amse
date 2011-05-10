#ifndef _SINGLETON_H
#define _SINGLETON_H

template <class T>
class Singleton {
	
    public:
	static T* Instance() {
		if (!m_pInstance) {
			m_pInstance = new T;
		}
		return m_pInstance;
	}
    protected:
	Singleton() { }
    private:
	static T* m_pInstance;
	Singleton& operator=(const Singleton&) { return *this; }
	Singleton(const Singleton&) { }	
};


template <class T>
T* Singleton<T>::m_pInstance = 0;


#endif
