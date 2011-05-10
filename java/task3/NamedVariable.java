package ru.amse.turkin;

public class NamedVariable extends Variable {
    
    public void setName(char name) {
        this.name = name;
    }
    public char getName() {
        return name;
    }

    private char name;

}
