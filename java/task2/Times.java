package ru.amse.turkin;

public class Times extends AbstractBinaryOperator {

    public Times(Expression expr1, Expression expr2) {
        super(expr1,expr2);
    }
    
    public int evaluate() {
        return expr1.evaluate() * expr2.evaluate();
    }
    
   
}
