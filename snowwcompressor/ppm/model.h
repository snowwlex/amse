/*
 * model.h
 *
 *  Created on: 04.12.2009
 *      Author: alex
 */

#ifndef MODEL_H_
#define MODEL_H_


extern const unsigned int MAX_Total;

typedef std::queue<Intervals> modelResult;
typedef std::map<unsigned char , unsigned int > COUNTERS ;

struct Statistic {
	COUNTERS myCounters;    //счетчики
	unsigned int myTotal;   //сумма счетчиков
	unsigned int myEsc;     //счетчик символов ухода
};

typedef std::pair <std::string, Statistic> PAIR_CONTEXTS;
typedef std::map< std::string, Statistic > CONTEXTS;

class ContextModel {

private:
	int myOrder; //порядок модели
	CONTEXTS myContexts;  // структура для хранения статистик для контекстов

	void Rescale(Statistic &stat);
public:
	ContextModel(int order=0);
	void UpdateContextModel(std::string context, char symbol);
	void Print() const;
	int GetInterval(std::string context, unsigned char symbol, Intervals& curInterval, unsigned int *mask, bool codeNull = false) const;
	void SetOrder( int order);

	unsigned int GetTotalFreq(std::string context, unsigned int*mask = NULL);
	int GetSymbol(std::string context, unsigned int cum_freq, unsigned char &curSymbol,Intervals &curInterval, unsigned int *mask);



};



#endif /* MODEL_H_ */
