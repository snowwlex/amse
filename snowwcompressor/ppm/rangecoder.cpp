/*  RangeCoder
 * http://www.pilabs.org.ua/sh/aridemo6.zip
 */
#include <stdio.h>
#include "rangecoder.h"

void RangeCoder::StartEncode( FILE *out ) {
   low=FFNum=Cache=0; range=(unsigned int)-1;
   f = out;
}

void RangeCoder::StartDecode( FILE *in )
 {
   code=0;
   range=(unsigned int)-1;
   f = in;
   for (int i=0; i<5; i++) code=(code<<8) | getc(f);
 }

 void RangeCoder::FinishEncode( void )
 {
   low+=1;
   for (int i=0; i<5; i++) ShiftLow();
 }
 void RangeCoder::FinishDecode( void ) {}
 void RangeCoder::encode(unsigned int cumFreq, unsigned int freq, unsigned int totFreq)
 {
   low += cumFreq * (range/= totFreq);
   range*= freq;
   while( range<(1<<24) ) ShiftLow(), range<<=8;
 }
 void RangeCoder::ShiftLow( void )
 {

   if ( (low>>24)!=0xFF ) {
	  putc(Cache + (low>>32),f);
      int c = 0xFF+(low>>32);
      while( FFNum ) {
    	  putc(c, f);
    	  FFNum--;
      }
      Cache = (unsigned int)(low)>>24;
   } else FFNum++;
   low = (unsigned int)(low)<<8;
 }

 unsigned int RangeCoder::get_freq (unsigned int totFreq) {
   return code / (range/= totFreq);
 }
 void RangeCoder::decode_update (unsigned int cumFreq, unsigned int freq, unsigned int totFreq)
 {
   code -= cumFreq*range;
   range *= freq;
   while( range<(1<<24) ) code=(code<<8)|getc(f), range<<=8;
 }


