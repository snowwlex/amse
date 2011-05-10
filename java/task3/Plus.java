package ru.amse.turkin;

public class Plus extends AbstractBinaryOperator  {

    public Plus(Expression expr1, Expression expr2) {
        super(expr1,expr2);
    }
    
    public double evaluate() {
        return expr1.evaluate() + expr2.evaluate();
    }
    
   
}
