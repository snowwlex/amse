/*
 * model.cpp
 *
 *  Created on: 04.12.2009
 *      Author: alex
 */

#include <stdio.h>
#include <map>
#include <queue>
#include <string>

#include "../bitfile.h"
#include "../filestructure.h"
#include "../compressor.h"

#include "ppm.h"
#include "model.h"


const unsigned int MAX_Total = 0x3fff;

ContextModel::ContextModel(int order) {
	myOrder = order;
}

void ContextModel::Print() const {
	CONTEXTS::const_iterator itContext;
	COUNTERS::const_iterator itCounter;
	for (itContext = myContexts.begin(); itContext != myContexts.end(); ++itContext) {
		printf("Контекст '%s' : ", itContext->first.c_str());
		for (itCounter = itContext->second.myCounters.begin(); itCounter !=  itContext->second.myCounters.end(); ++itCounter) {
				printf("'%c'=%u,", itCounter->first, itCounter->second);
			}
		printf(" символ ухода = %u ,",itContext->second.myEsc);
		printf("Total = %d\n", itContext->second.myTotal);
	}
}
int ContextModel::GetInterval(std::string context, unsigned char symbol, Intervals& curInterval, unsigned int* mask,bool codeNull) const {
	CONTEXTS::const_iterator itContext;
	COUNTERS::const_iterator itCounter;

	unsigned int cumFreq;
	bool found;

	curInterval.cumFreq = 0;
	curInterval.curFreq = 0;
	curInterval.totalFreq = 0;

	if (myOrder < (int)context.length() ) {
		context = cutString(context, myOrder);
	}
	else if (myOrder > (int)context.length() ) {
		return 2; //данного контекста не существует
	}



	itContext = myContexts.find(context);
	if (itContext == myContexts.end() ) {
		return 2; //данного контекста не существует
	}


	for (itCounter = itContext->second.myCounters.begin(), cumFreq = 0, found = false;
		 itCounter != itContext->second.myCounters.end();
		 ++itCounter) {

		if (mask[itCounter->first] == 0) {

			if (codeNull == false && itCounter->first == symbol) {
				curInterval.cumFreq = cumFreq;
				curInterval.curFreq = itCounter->second;
				found = true;
			}

			cumFreq += itCounter->second;

			mask[itCounter->first] = 1;
			//printf("Порядок %d, Исключаем из рассмотрения '%c'\n", myOrder,itCounter->first);

		}
	}

	curInterval.totalFreq = cumFreq + itContext->second.myEsc ;

	if (!found) { // такого символа в статистике нет, возвращаем вероятность ухода
		curInterval.cumFreq = cumFreq;
		curInterval.curFreq = itContext->second.myEsc;
		return 1; // 1 => символ ухода
	}

	return 0; //возвращается интервал для символа, который существует для запрошенного контекста
}

void ContextModel::SetOrder( int order) {
	myOrder = order;
}

void ContextModel::UpdateContextModel(std::string context, char symbol) {

	CONTEXTS::iterator itContext;
	COUNTERS::iterator itCounter;
	std::pair<CONTEXTS::iterator,bool> resultInsertContext;
	std::pair<COUNTERS::iterator,bool> resultInsertCounter;
	PAIR_CONTEXTS pairContext;

	if (myOrder < (int)context.length() ) {
		context = cutString(context, myOrder);
	}
	else if (myOrder > (int)context.length() ) {
		return; //данный контекст не подходит этой модели
	}

//printf("===Апдейт модели %d, контекст '%s', символ '%c' \n", myOrder, context.c_str(), symbol);
	itContext = myContexts.find( context );
	if (itContext == myContexts.end() ) {
		//printf("===Такого контекст в модели не было, создаем\n");
		//такого контекста еще не было
		//создаем
		pairContext.first = context;
		pairContext.second.myEsc = 1;
		pairContext.second.myTotal = 0;
		resultInsertContext = myContexts.insert( pairContext );
		itContext = resultInsertContext.first;
	}
	//printf("Апдейтим контекст '%s'\n",context.c_str());

	if ( (itContext->second.myTotal + itContext->second.myEsc) >= MAX_Total) {
		Rescale(itContext->second);
		//printf("Rescaling, контекст '%s' порядок %d\n",context.c_str(), myOrder);
	}

	itCounter = itContext->second.myCounters.find(symbol);
	if (itCounter == itContext->second.myCounters.end() ) {
		//printf("===Такого символа в статистике нет, создаем\n");
		//такого символа в статистике нет
		//создаем
		resultInsertCounter = itContext->second.myCounters.insert( std::pair<unsigned char, unsigned int>( symbol, 0 )  );
		itCounter = resultInsertCounter.first;
		itContext->second.myEsc += 1;
		//	printf("Вставили символ '%c', символов ухода теперь %d\n",symbol,myEsc);
	}
	itCounter->second += 1;
	itContext->second.myTotal += 1;
	//printf("Теперь всего символов в данной статистике %d\n",myTotal);
}

unsigned int ContextModel::GetTotalFreq(std::string context, unsigned int *mask) {

	CONTEXTS::const_iterator itContext;
	COUNTERS::const_iterator itCounter;
	unsigned int CumFreqUnder;
	if (myOrder != (int)context.length() ) {
		//printf("ОШИБКА! невозможный контекст для данной модели.\n");
		//printf("порядок=%d, длина = %d\n",myOrder,(int)context.length());
		return 0;
	}
//printf("ищем контекст '%s'", context.c_str());
	itContext = myContexts.find(context);
	if (itContext == myContexts.end() ) {
		//printf("ОШИБКА! данного контекста '%s' не существует!\n",context.c_str());
		//printf("порядок=%d, длина = %d\n",myOrder,(int)context.length());
		return 0; //данного контекста не существует
	}

	for (itCounter = itContext->second.myCounters.begin(), CumFreqUnder = 0;
		 itCounter != itContext->second.myCounters.end();
		 ++itCounter) {

			if (mask == NULL || mask[itCounter->first] == 0) {
				CumFreqUnder += itCounter->second;
			}
	}


return CumFreqUnder + itContext->second.myEsc;
}
void ContextModel::Rescale(Statistic &stat) {

	COUNTERS::iterator itCounter;
	stat.myTotal = 0;
	for (itCounter = stat.myCounters.begin(); itCounter != stat.myCounters.end(); ++itCounter){
	      itCounter->second -= itCounter->second >> 1;
	      stat.myTotal += itCounter->second;
	}
}
int ContextModel::GetSymbol(std::string context, unsigned int cum_freq, unsigned char & curSymbol, Intervals & curInterval, unsigned int* mask) {
	CONTEXTS::iterator itContext;
	COUNTERS::iterator itCounter;
	unsigned int CumFreqUnder;
	bool found;


	itContext = myContexts.find(context);
	if ( itContext == myContexts.end() ) {
		printf("ОШИБКА! Такого контекста в модели нет\n");
		return -1;
	}



	for (itCounter = itContext->second.myCounters.begin(), CumFreqUnder = 0, found = false;
		 itCounter != itContext->second.myCounters.end();
		 ++itCounter) {

		if (mask[itCounter->first] == 0) {

			if  ( found == false && (CumFreqUnder + itCounter->second) > cum_freq ) {
				curSymbol = itCounter->first;
				curInterval.cumFreq = CumFreqUnder;
				curInterval.curFreq = itCounter->second;
				found = true;
			}

			CumFreqUnder += itCounter->second;
			mask[itCounter->first] = 1;
		}
	}



		curInterval.totalFreq = CumFreqUnder + itContext->second.myEsc ;

		if (found == false) { // такого символа в статистике нет, возвращаем вероятность ухода
			curInterval.cumFreq = CumFreqUnder;
			curInterval.curFreq = itContext->second.myEsc;
			return 1; // 1 => символ ухода
		}

		return 0; //обычный символ



}

