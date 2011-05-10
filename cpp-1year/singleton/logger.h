#ifndef _LOGGER_H
#define _LOGGER_H

class Logger : public Singleton<Logger> {
	private:
		Logger() { }
		friend class Singleton<Logger>; // для того чтобы можно было в функции Instance запустить Logger();
	public:
		bool openLogFile();
		bool writeLogFile();
		bool closeLogFile();
};
#endif
