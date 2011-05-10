package ru.amse.turkin;

import java.text.ParseException;

public class Tokenizer {


    public Tokenizer(String str) {
        tokensString = str;
    }

    public void restart() {
        curPos = 0;
        prevPos = 0;
    }

    public String seeNextToken() throws ParseException {
        int bufferPos = curPos;
        String string = nextToken();
        curPos = bufferPos;
        return string;
    }

    public boolean test(String string) throws ParseException {
        //int bufferPos = curPos;
        String token = nextToken();
        if  ( token.equals(string) ) {
            return true;
        }
        curPos = prevPos;
        return false;
    }

    public char getVarName() {
        return curVar;
    }

    public double getConst() {
        return curConst;
    }

    public int getPrevPos() {
        return prevPos;
    }

    public String getString() {
        return tokensString;
    }
    public String nextToken() throws ParseException {

        prevPos = curPos;
        
        while (curChar() == ' ') {
            curPos++;
        }

        char ch = curChar();

        if (ch == 0) {
            return "end";
        }

        String token = "";
        if (ch == '+' || ch == '-' || ch == '/' || ch == '*' || ch=='(' || ch==')') {
            token = Character.toString(ch);
            curPos++;

        } else if ( Character.isDigit(ch) == true ) {
            String constanta = "";
            while( curChar() != 0 && Character.isDigit( curChar() ) == true ) {
                constanta += Character.toString( curChar() );
                curPos++;
            }
            curConst = Integer.parseInt(constanta);
            token = "c";
        } else if (Character.isLetter(ch) == true ) {
           curVar = ch;
           curPos++;
           token = "var";
        }  else {
           throw new ParseException("Unexpected symbol! Available symbols are: +,-,*,/, (,), 0-9, A-Z, a-z", getPrevPos());
        }


        return token;
     }


//    private int nextChar() {
//        curPos += 1;
//        return curChar();
//    }

    private char curChar() {
        if (curPos >= tokensString.length()) {
            return 0;
        }
        return tokensString.charAt(curPos);
    }

    private int curPos;
    private int prevPos;
    private double curConst;
    private char curVar;
    private String tokensString;
}
