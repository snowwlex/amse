/*  RangeCoder
 * http://www.pilabs.org.ua/sh/aridemo6.zip
 */


class RangeCoder
{
 unsigned int code, range, FFNum, Cache;
 long long low; //64 bit type
 FILE *f;
public:
 void StartEncode( FILE *out );
 void StartDecode( FILE *in );
 void FinishEncode( void );
 void FinishDecode( void );
 void encode(unsigned int cumFreq, unsigned int freq, unsigned int totFreq);
 void ShiftLow( void );
 unsigned int get_freq (unsigned int totFreq);
 void decode_update (unsigned int cumFreq, unsigned int freq, unsigned int totFreq);
};

