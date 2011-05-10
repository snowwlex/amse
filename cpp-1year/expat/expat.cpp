/*
 * expat.cpp
 *
 *      Author: Alex Turkin
 */
#include <iostream>
#include <cstdio>
#include <expat.h>
#include "data.h"

static void startTagHandler(void *userData, const char *tag, const char **atts) {
	Data* dataPointer = (Data*)userData;
 	++dataPointer->myTagsCounter;
 	 for (int i = 0; atts[i]; i += 2)  {
		++dataPointer->myAttsCounter;
	 }

 	if (dataPointer->curTextLength > 0) {
 		if (dataPointer->curTextLength > dataPointer->maxTextLength) {
			dataPointer->maxTextLength = dataPointer->curTextLength;
		}
		dataPointer->curTextLength = 0;
	}
}
static void endTagHandler(void *userData,const char *name) {
	Data* dataPointer = (Data*)userData;
	if (dataPointer->curTextLength > 0) {
		if (dataPointer->curTextLength > dataPointer->maxTextLength) {
			dataPointer->maxTextLength = dataPointer->curTextLength;
		}
		dataPointer->curTextLength = 0;
	}
}
static void characterDataHandler(void *userData,const char *s, int len) {
	Data* dataPointer = (Data*)userData;
	dataPointer->curTextLength += len;
}

int main() {
    int done, length;
	Data data;
	size_t bufferSize = 4*1024; //4 kb
	char* buffer = new char[bufferSize];

	XML_Parser parser = XML_ParserCreate(NULL);
    XML_SetStartElementHandler(parser, startTagHandler);
	XML_SetEndElementHandler(parser, endTagHandler);
	XML_SetCharacterDataHandler(parser, characterDataHandler);

	data.maxTextLength = data.curTextLength = 0;
	data.myAttsCounter = 0;
	data.myTagsCounter = 0;
	XML_SetUserData(parser, &data);

	do  {
		length = fread(buffer, sizeof(char), bufferSize, stdin);
		done = feof(stdin);
		if (XML_Parse(parser, buffer, length, done) == XML_STATUS_ERROR) {
			fprintf(stderr,"Error: %s at line %d\n", XML_ErrorString(XML_GetErrorCode(parser)),  (int)XML_GetCurrentLineNumber(parser));
      		done = 1;
    	}
	} while (!done);
	
	printf("Counter of tags       is: %d\n", data.myTagsCounter);
	printf("Counter of attributes is: %d\n", data.myAttsCounter);
	if (data.maxTextLength == 0) {
		printf("Character data was not found at this text!\n");
	} else {
		printf("Max length of character data is: %d bytes\n", data.maxTextLength);
	}

    XML_ParserFree(parser);
    delete buffer;
	
	return 0;
}
