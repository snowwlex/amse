import java.util.ArrayList;

public class Main {

    public static void main(String[] args) {
        test();
    }

    public static void test() {

        ArrayList al = new ArrayList();
        al.add(5);al.add(6);al.add(7);al.add(8);
        Bag bag = new Bag(al);
        assert(bag.size() == 4);
        //ass (bag.size() == 0);
        //bag.add(new Integer(5));
        //ass(bag.size() == 1);
        //bag.add(new Integer(5));
        //ass(bag.size() == 2);



    }

    public static void ass(boolean cond) {
        if (cond == false) {
            throw new AssertionError();
        }
    }
}
