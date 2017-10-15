package q3;
public class B {
    public int foo(int a) {
        bar(1);
        return 0;
    }
    public int bar(int b) {
        return baz(b);
    }
    public int baz(int a) {
        return a + a;
    }

    public int foo2(int a) throws Exception{
        bar(1);
        if (a==0) {
            throw new Exception("Have a try");
        }
        return 0;
    }

    public int fault(int a){
        return 0/0;
    }

    public int foo1(int a){
        return fault(0);
    }
}