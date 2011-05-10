//package ru.amse.turkin;

import java.text.ParseException;
import java.util.ArrayList;
import java.util.ListIterator;

public class Main {

    private static final int START_BOUNDARY = 0;
    private static final int FINISH_BOUNDARY = 10;

    public static void main(String[] args) {

        
		if (args.length != 1) {
			System.out.println("Wrong use!\nUsage is: Main 'expr'");
			return;
		}

        calculate(args[0]);
    }

    private static void calculate(String string) {
        Calculator calculator = new Calculator( string );

        try {
            Expression expr = calculator.calculateExpr();
            ArrayList<NamedVariable> variables = calculator.getVariables();
            System.out.format("%s", string);
            if ( variables.size() > 0 ) {
                printListing(expr, variables);
            } else {
                System.out.format("  = %.2f\n",expr.evaluate() );
            }

        } catch (ParseException e) {
            System.out.format("%s at string:\n", e.getMessage());
            System.out.format("%s\n", string);
            String spaces = "";
            for (int i = 0; i < e.getErrorOffset() ; i++) {
                spaces = spaces.concat(" ");    
            }
            System.out.format("%s^\n", spaces);
        } catch (StackOverflowError e) {
            System.out.format("%s\n", e.getMessage());
        }

    }

    private static void printListing( Expression expr, ArrayList<NamedVariable> variables ) {


        if (variables.size() == 0 ) {
            return;
        }
        
        ListIterator<NamedVariable> iterator = variables.listIterator();

        System.out.println();
        while (iterator.hasNext()) {
            NamedVariable a = iterator.next();
            a.set(START_BOUNDARY);
            System.out.format("%4s ", a.getName());
        }
        System.out.format("%14s\n", "result" );

        mark: while ( true ) {
            iterator = variables.listIterator();
            while (iterator.hasNext()) {
                NamedVariable a = iterator.next();
                System.out.format(" %3.0f ", a.get());
            }

            try {
                double eval = expr.evaluate();
                System.out.format(" %13f", eval );
            }
            catch ( DivisionByZeroException e) {
               System.out.format(" %13s", "DBZ");
            }
            
            System.out.println();

            iterator = variables.listIterator();
            while (iterator.hasNext()) {
                NamedVariable a = iterator.next();
                if (a.get() == FINISH_BOUNDARY) {
                    a.set(START_BOUNDARY);
                    if (!iterator.hasNext()) {
                        break mark;
                    }
                } else {
                    a.set(a.get() + 1);
                    break;
                }
            }
        }

    }



       

}