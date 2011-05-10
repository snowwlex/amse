//package ru.amse.turkin;

public class Variable implements Expression {

    public void set(double var) {
        this.var = var;
    }

    public double get() {
        return var;
    }
    
    public double evaluate() {
        return get();
    }

    private double var;

}
