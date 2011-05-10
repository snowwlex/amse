package ru.amse.turkin;

public class Variable implements Expression {

    public void set(int var) {
        this.var = var;
    }
    
    public int get() {
        return var;
    }
    
    public int evaluate() {
        return get();
    }

    private int var;
}
