package ru.amse.turkin;

public abstract class AbstractBinaryOperator implements Expression {

    public AbstractBinaryOperator(Expression expr1, Expression expr2) {
        this.expr1 = expr1;
        this.expr2 = expr2;
    }
    
    public abstract int evaluate();
    
    protected Expression expr1;
    protected Expression expr2;
}
