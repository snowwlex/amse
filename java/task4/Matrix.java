import java.io.*;
import java.util.Scanner;
import java.util.TreeMap;


public class Matrix {

    public Matrix(double[][] matrix) throws MatrixIOException {
        boolean isCorrect = checkMatrixEqualRowsAndColumns(matrix);
        if (isCorrect == false) {
            throw new MatrixIOException("incorrect format of matrix: too many numbers in a row");
        }
        this.data = matrix.clone();
        this.rowsNumber = this.data.length;
        this.columnsNumber= this.data[0].length;
    }

    public Matrix(File f) throws MatrixIOException {
        Reader reader;
        LineNumberReader lnReader;
        try {
            reader = new FileReader(f);
            lnReader = new LineNumberReader(reader);
        } catch (FileNotFoundException e) {
            throw new MatrixIOException("File not found.");
        }
        try {
            if (lineNumbers.containsKey(f.getAbsolutePath())) {
                for (int i = lineNumbers.get(f.getAbsolutePath()); i > 0; --i) {
                    if (!lnReader.ready()) {
                        throw new MatrixIOException("Incorrect input.");
                    }
                     lnReader.readLine();
                }
            }
            if (!lnReader.ready()) {
                throw new MatrixIOException("Incorrect input.");
            }

            Scanner scanner = new Scanner(lnReader.readLine());
            if (scanner.hasNextInt() == false) {
                throw new MatrixIOException("Incorrect input.");
            }

            this.rowsNumber = scanner.nextInt();
            if (scanner.hasNextInt() == false) {
                throw new MatrixIOException("Incorrect input.");
            }

            this.columnsNumber = scanner.nextInt();
            this.data = new double[this.rowsNumber][this.columnsNumber];
            for (int i = 0; i < this.rowsNumber; ++i) {
                if (!lnReader.ready()) {
                    throw new MatrixIOException("Incorrect input.");
                }
                scanner = new Scanner(lnReader.readLine());
                for (int j = 0; j < this.columnsNumber; ++j) {
                    if (!scanner.hasNextDouble()) {
                        throw new MatrixIOException("Incorrect input.");
                    }
                    this.data[i][j] = scanner.nextDouble();
                }
            }
            lineNumbers.put(f.getAbsolutePath(),lnReader.getLineNumber());
            lnReader.close();
        } catch (IOException e) {
            try {
                lnReader.close();
            } catch (IOException e1) {
            }
            throw new MatrixIOException("Incorrect input.");
        }
    }


    public void write(File f) throws MatrixIOException {
        FileWriter w;
        try {
            w = new FileWriter(f);
        } catch (IOException e) {
            throw new MatrixIOException("Writing matrix to file was failed");
        }
        try {
            w.write(Integer.toString(this.rowsNumber) + ' ' + Integer.toString(this.columnsNumber));
            for (int i = 0; i < this.rowsNumber; ++i) {
                w.write('\n');
                for (int j = 0; j < this.columnsNumber; ++j) {
                    w.write(Double.toString( this.data[i][j] ));
                    if (j + 1 < this.columnsNumber) {
                        w.write(' ');
                    }
                }
            }
            w.close();
        } catch (IOException e) {
            try {
                w.close();
            } catch (IOException e1) {  /* Ignoring */ }
            throw new MatrixIOException("Writing matrix to file was failed");
        }
    }
    public void add(Matrix m) {
        if ( checkMatrixHasEqualSize(m) == false ) {
            throw new MatrixArithmeticException("Can't add matrix, because of unequal size.");
        }
        for (int i=0; i < this.rowsNumber; i++) {
            for (int j=0; j < this.columnsNumber; j++) {
                this.data[i][j] += m.data[i][j];
            }
        }
    }
    public void scale(double scalar) {
        for (int i=0; i < this.rowsNumber; i++) {
            for (int j=0; j < this.columnsNumber; j++) {
                this.data[i][j] *= scalar;
            }
        }
    }
    public void subtract(Matrix m) {
        if ( checkMatrixHasEqualSize(m) == false ) {
            throw new MatrixArithmeticException("Can't subtract matrix, because of unequal size.");
        }
        for (int i=0; i < this.rowsNumber; i++) {
            for (int j=0; j < this.columnsNumber; j++) {
                this.data[i][j] -= m.data[i][j];
            }
        }
    }
    public void multiply(Matrix m) {
        if (this.columnsNumber != m.rowsNumber) {
            throw new MatrixArithmeticException("can't multiply this matrixes; number of columns at first matrix should be equal number of rows of second matrix");
        }
        int newColumnsNumber = m.columnsNumber;
        int newRowsNumber = this.rowsNumber;
        double[][] newMatrix = new double[newRowsNumber][newColumnsNumber];
        for (int i=0; i<newRowsNumber; ++i) {
           for (int j=0; j < newColumnsNumber; ++j) {
               double result = 0;
               for (int k=0; k < this.columnsNumber; ++k) {
                    result += this.data[i][k] * m.data[k][j];
               }
               newMatrix[i][j] = result;
          }
        }
        this.data = newMatrix;
        this.rowsNumber = newRowsNumber;
        this.columnsNumber = newColumnsNumber;
    }
    public void print() {
        for (int i=0; i < this.rowsNumber; i++) {
            for (int j=0; j < this.columnsNumber; j++) {
                System.out.print(this.data[i][j] + "\t");
            }
            System.out.println();
        }
    }
    private static boolean checkMatrixEqualRowsAndColumns(double[][] matrix) {
      int rows = matrix.length;
      int columns = matrix[0].length;
      for (int i=1; i<rows; i++) {
          if (matrix[i].length != columns) {
              return false;
          }
      }
      return true;
    }
    private boolean checkMatrixHasEqualSize(Matrix m) {
        boolean result =  this.rowsNumber == m.rowsNumber &&
                           this.columnsNumber == m.columnsNumber;
        return result;
    }

    private double[][] data;
    private int rowsNumber;
    private int columnsNumber;
    private static TreeMap<String, Integer> lineNumbers = new TreeMap<String, Integer>();
}
