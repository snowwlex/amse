/*
 * stl.cpp
 *
 *  Created on: 28.03.2010
 *      Author: Turkin Alexander
 */

#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include "predicates.h"

#include <functional>

typedef std::map<int,unsigned int> STATISTIC;
typedef std::multiset<int,byHundredsCompare> BY_HUNDREDS;

int main () {

	const int lBound = 100;

	long number;
	STATISTIC statData;
	STATISTIC::iterator itStat;
	BY_HUNDREDS byHundreds;
	BY_HUNDREDS::iterator itByHundreds;

	while (std::cin >> number) {
		++statData[number];
		byHundreds.insert(number);
	}

	std::cout << "Count of different numbers is " << statData.size() << std::endl;
	if (statData.empty() == false ) {
		itStat = std::max_element(statData.begin(),statData.end(), byQuantityCompare);
		std::cout << "Most frequent number is " <<  itStat->first << ", appeared " << itStat->second << " times" <<std::endl;
		itStat = std::min_element(statData.begin(),statData.end(), byQuantityCompare);
		std::cout << "Rarest number is " <<  itStat->first << ", appeared " << itStat->second << " times" <<std::endl;
	}

	std::pair<BY_HUNDREDS::iterator,BY_HUNDREDS::iterator> bounds;
	bounds = std::equal_range(byHundreds.begin(), byHundreds.end(), lBound, byHundredsCompare());
	BY_HUNDREDS::iterator 	itHSortedStart = bounds.first,
							itHSortedEnd   = bounds.second;

	std::cout << "Range: [" << (lBound/100*100) << ";" << (lBound/100*100+100) << ")";
	std::cout << "; quantity of numbers is " << std::distance(itHSortedStart,itHSortedEnd) << std::endl;

	if (itHSortedStart == itHSortedEnd) {
		std::cout << "There aren't numbers in this range" << std::endl;
	} else {
		for (itByHundreds = itHSortedStart; itByHundreds != itHSortedEnd; ++itByHundreds) {
			std::cout << *itByHundreds << std::endl;
		}
	}

	return 0;
}
