//package ru.amse.turkin;

public class DivisionByZeroException extends ArithmeticException {

    public DivisionByZeroException() {
        super();
    }

    public DivisionByZeroException(String string) {
        super(string);        
    }
}
