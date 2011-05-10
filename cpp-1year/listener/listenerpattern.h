#ifndef LISTENER_H_
#define LISTENER_H_

#include <list>


template <typename T>
class Listener {

public:
	void setModel(T* model) {
		unsubscribe();
		myModel = model;
	}

	virtual void update() = 0;
	virtual void meDeleted() {
		myModel = 0;
	}

protected:
	Listener(): myModel(0) { }
	virtual ~Listener() {
		unsubscribe();
	}

private:
	void unsubscribe() {
		if (myModel != 0) {
			myModel->removeListener(this);
		}
	}

protected:
	T* myModel;

};


template <typename T>
class Subject {

public:
	void addListener(Listener<T>* listener) {
		myListeners.push_back(listener);
	}
	void removeListener(Listener<T>* listener) {
		myListeners.remove(listener);
	}

protected:
	typedef std::list< Listener<T>* > LISTENERS;
	Subject() { }
	Subject(const Subject& subject) { } //конструктор с пустой реализацей, для того чтобы не копировался list listenerов
	Subject& operator=(const Subject& subject) { } // оператор присваивания пуст по той же причине
	virtual ~Subject() {
		typename LISTENERS::iterator itListener;
		for (itListener = myListeners.begin(); itListener != myListeners.end(); ++itListener) {
			(*itListener)->meDeleted();
		}
	}

protected:
	void notify() {
		typename LISTENERS::iterator itListener;
		for (itListener = myListeners.begin(); itListener != myListeners.end(); ++itListener) {
			(*itListener)->update();
		}
	}

private:
	LISTENERS myListeners;

};

#endif /* LISTENER_H_ */
