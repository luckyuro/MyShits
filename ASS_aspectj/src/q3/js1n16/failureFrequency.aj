package q3.js1n16;

import org.aspectj.lang.Signature;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by lambda-c on 3/22/17.
 */
public aspect failureFrequency {
    //  wantedMethod is the decription of the Methods meet requirment.
    pointcut wantedMethod(): call ( public int q3..*(int));
    //  mainFunction is for after it to write csv file.
    pointcut mainFunction(): execution(public static * q3..main(..));

    // the int array, 1st for all time, 2nd for succeed time
    Map<Signature, int[]> successInfo = new HashMap<Signature, int[]>();

    int around(int i): wantedMethod() && args(i){

        // get the methSignature, and get the corresponding int array
        // if not exist new one,
        Signature thisSignature = thisJoinPoint.getSignature();
        int[] methodFrequency = successInfo.get(thisSignature);
        if (methodFrequency != null) {
            methodFrequency[0]++;
        } else {
            successInfo.put(thisSignature, new int[]{1, 0});
        }


        int answer = proceed(i);

        // if the code goes here, the function didn't fail
        successInfo.get(thisSignature)[1]++;

        return answer;

    }


    after(): mainFunction(){
        try{
            PrintWriter fileOut
                    = new PrintWriter(new BufferedWriter(new FileWriter("failures.csv")));
            for (Signature s : successInfo.keySet()){
                int[] nums = successInfo.get(s);
                int total = nums[0];
                int success = nums[1];
                double failurePercent = 1.0*(total-success)/total;

                String frequencyStr = String.format("%.2f",failurePercent*100);
                fileOut.println(s.toLongString()+','+frequencyStr+'%');
            }

            fileOut.close();
        }
        // Just in case the file write failed
        catch (java.io.IOException ex) {
            System.out.println("Failed to write CSV files!");
        }

    }

}
