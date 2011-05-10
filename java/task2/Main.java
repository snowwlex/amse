package ru.amse.turkin;

public class Main {
    public static void main(String[] args) {
        Variable a = new Variable();
        Variable b = new Variable();
        Variable c = new Variable();
        Variable d = new Variable();
        
        // test expression #1
        a.set(5);
        b.set(12);
        c.set(19);
        Expression e = new Plus(a, new Minus(b,c) );        
        System.out.println("First test:");
        System.out.format("a = %d; b = %d; c = %d\n", a.get(), b.get(), c.get() );
        System.out.format("a + ( b - c ) = %d\n\n", e.evaluate() );
        
        // test expression #1 with different amounts
        a.set(12);
        b.set(17);
        System.out.println("The same expression with different 'a' and 'b':");
        System.out.format("a = %d; b = %d; c = %d\n", a.get(), b.get(), c.get() );
        System.out.format("a + ( b - c ) = %d\n\n", e.evaluate() );
        
        // test expression #2
        a.set(4);
        b.set(7);
        c.set(20);
        d.set(10);
        e = new Times( new Plus(a,a), new Minus(b, new Divide(c,d)) );     
        System.out.println("Test complex expression:");   
        System.out.format("a = %d; b = %d; c = %d; d = %d\n", a.get(), b.get(), c.get(), d.get() );
        System.out.format("(a + a)  * (b - c / d) = %d\n\n", e.evaluate() );
                
                      
    }
}
