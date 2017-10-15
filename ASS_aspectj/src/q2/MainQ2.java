package q2;

/**
 * Created by lambda-c on 3/18/17.
 */
public class MainQ2 {
    public static void main (String[] args){
        B myb = new B();
//        int ans = myb.foo(0);
//
//        int ans1 = myb.foo(0);
//        int ans2 = myb.fault(0);
     try{
        int ans3 = myb.foo2(0);}
        catch (Exception ex){
    System.out.println("Exception in main function");
        }
//        int a = 0/0;
//        int ans4 = myb.foo1(0);
        try{
            int ans4 = myb.foo1(0);}
        catch (Exception ex){
            System.out.println("Exception in main function");
        }

        //System.out.println(ans);
    }
}
