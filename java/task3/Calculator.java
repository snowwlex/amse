//package ru.amse.turkin;

import java.text.ParseException;
import java.util.ArrayList;
import java.util.ListIterator;

public class Calculator {
    public Calculator(String str) {
        tokenizer = new Tokenizer(str);
        variables = new ArrayList<NamedVariable>();
    }

    public ArrayList<NamedVariable> getVariables() {
        return variables;
    }
    
    public Expression calculateExpr() throws ParseException {
        tokenizer.restart();
        variables = new ArrayList<NamedVariable>();
        Expression e = parseExpr();
        if ( tokenizer.nextToken().equals("end") == false) {
            throw new ParseException("Unexpected end of expression", tokenizer.getPrevPos());
        }
        return e;
    }

    public String getCalcString() {
        return tokenizer.getString();
    }

    public Expression parseExpr() throws ParseException {
        Expression result = parseTerm1();
        while(true) {
            String token = tokenizer.seeNextToken();
            if ( token.equals("+") ) {
                tokenizer.nextToken();
                result = new Plus( result, parseTerm1());
            }
            else if ( token.equals("-") ) {
                tokenizer.nextToken();
                result = new Minus( result, parseTerm1());
            } else {
                break;
            }
        }

        return result;

    }

    private Expression parseTerm1() throws ParseException {
        Expression result = parseTerm2();
        while(true) {
            String token = tokenizer.seeNextToken();
            if ( token.equals("*") ) {
                tokenizer.nextToken();
                result = new Times( result, parseTerm2());
            }
            else if ( token.equals("/") ) {
                tokenizer.nextToken();
                result = new Divide( result, parseTerm2());;
            } else {
                break;
            }
        }
        return result;              
    }

    private Expression parseTerm2() throws ParseException {
        Expression result;
        
        String token = tokenizer.nextToken();
        if ( token.equals("c") ) {
            Variable c = new Variable();
            c.set(tokenizer.getConst());
            result = c;
        } else if ( token.equals("var") ) {

            NamedVariable variable = new NamedVariable();

            boolean  isVarNew = true;
            ListIterator<NamedVariable> iterator = variables.listIterator();
            while ( isVarNew == true && iterator.hasNext() ) {
                NamedVariable curVariable = iterator.next();
                if (curVariable.getName() == tokenizer.getVarName()) {
                    variable = curVariable;
                    isVarNew = false;
                }
            }

            if (  isVarNew )  {
                variable.setName(tokenizer.getVarName());
                variables.add(variable);
            }

            result = variable;

        } else if (token.equals("(") ) {
            result = parseExpr();
            token = tokenizer.nextToken();
            if ( token.equals(")") == false ) {
                throw new ParseException("Closing bracket ')' is missing", tokenizer.getPrevPos());
            }
        } else if (token.equals("-") ) {
            Variable c = new Variable();
            c.set(-1);
            result = new Times(c,parseTerm2());
        } else {
            throw new ParseException("Unexpected symbol", tokenizer.getPrevPos());
        }
        return result;
    }

    private ArrayList<NamedVariable> variables;
    private Tokenizer tokenizer;

}