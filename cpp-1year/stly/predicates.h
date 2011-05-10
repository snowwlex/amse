/*
 * predicates.h
 *
 *  Created on: 29.03.2010
 *      Author: Turkin Alexander
 */

#ifndef PREDICATES_H_
#define PREDICATES_H_


struct byHundredsCompare {
	bool operator() (int one, int two) {
		return one/100 < two/100;
	}
};

inline bool byQuantityCompare (const std::pair<int,unsigned int>& one,const std::pair<int,unsigned int>&  two) {
		return one.second < two.second;
}

#endif /* PREDICATES_H_ */
