package q3;

/**
 * Created by lambda-c on 3/18/17.
 */
public class MainQ3 {
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

        int t_ans;

        for (int i=0; i <= 11;i++){
            t_ans = myb.bar(3);
        }

        //System.out.println(ans);
    }
}
