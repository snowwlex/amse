//package ru.amse.turkin;

public class Divide extends AbstractBinaryOperator  {

    public Divide(Expression expr1, Expression expr2)  {
        super(expr1,expr2);
    }
    
    public double evaluate() throws DivisionByZeroException  {
        double second = expr2.evaluate();
        if (second == 0) {
            throw new DivisionByZeroException();
        }
        double first = expr1.evaluate();
        return first / second;
    }
    
    
}
