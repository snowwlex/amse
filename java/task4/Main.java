import java.io.*;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        if (args.length != 2) {
			System.out.println("Wrong use!");
            System.out.println("Usage is: Main file_from file_to");
			return;
		}
        File fileFrom = new File(args[0]);
        File fileTo = new File(args[1]);
        try {
            Matrix m1 = new Matrix(fileFrom);
            Matrix m2 = new Matrix(fileFrom);
            m1.multiply(m2);
            m1.write(fileTo);
        } catch (MatrixIOException e) {
            System.out.print(e.getMessage());
            System.out.println();
            return;
        } catch (MatrixArithmeticException e) {
            System.out.print(e.getMessage());
            System.out.println();
            return;
        }
        System.out.print("Done!");
    }


}