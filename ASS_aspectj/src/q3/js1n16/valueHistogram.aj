package q3.js1n16;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.aspectj.lang.Signature;

/**
 * Created by lambda-c on 3/19/17.
 */
public aspect valueHistogram {
//  wantedMethod is the decription of the Methods meet requirment.
    pointcut wantedMethod(): call ( public int q3..*(int));
    //  mainFunction is for after it to write csv file.
    pointcut mainFunction(): execution(public static * q3..main(..));

    //  for every parameter value has three values to describe it {int0:[int00,int01],int1:[int10,int11]...}
    // the int array, 1st for parameter, 2nd for return
    private class histInfo extends HashMap<Integer, int[]> {}

    //  for every signature has there own data {Method0:Info0, Method1:Info1....}
    Map<Signature, histInfo> methodData = new HashMap<Signature, histInfo>();


    int around(int i):wantedMethod() && args(i){
        int answer = 0;

        // get the Signature and the corresponding histInfo if exist
        Signature thisSignature = thisJoinPointStaticPart.getSignature();
        histInfo thisSignatureInfo = methodData.get(thisSignature);

        // update by the methodSignature, and then update by value(as parameter)
        // if Signature exists in the set, update it
        // if not make a new histInfo object and add into it
        if (thisSignatureInfo != null) {
            int[] update0 = thisSignatureInfo.get(i);
            // if the value already in the set, +1 to the pos of para, if not, put it into the set
            if (update0 != null) {
                update0[0]++;
            } else {
                thisSignatureInfo.put(i, new int[]{1, 0});
            }
        } else {
            histInfo newSignatureInfo = new histInfo();
            newSignatureInfo.put(i, new int[]{1, 0});
            methodData.put(thisSignature, newSignatureInfo);
            thisSignatureInfo = newSignatureInfo;
        }


        // no matter the function succeed or not, the input will be recorded
        answer = proceed(i);

        // update the value as return
        // if failed, no return value will be updated
        // with the code former, the needed object should exist
        int[] update1 = thisSignatureInfo.get(answer);
        if (update1 != null) {
            update1[1]++;
        } else {
            thisSignatureInfo.put(answer, new int[]{0, 1});
        }

        return answer;
    }


    after():mainFunction() {
        for (Signature s : methodData.keySet()) {
            // key and the corresponding data use the Signature as filename
            String fileName = s.toString().concat("-hist.csv");
            histInfo info = methodData.get(s);

            try {
                PrintWriter fileOut
                        = new PrintWriter(new BufferedWriter(new FileWriter(fileName)));

                // get evey pair of values by the key, and write to csv file.
                for (int v : info.keySet()) {
                    int[] d = info.get(v);
                    int para = d[0];
                    int ret = d[1];

                    String vStr = String.format ("%d", v);
                    String paraStr = String.format ("%d", para);
                    String retStr = String.format ("%d", ret);
                    fileOut.println(vStr + ',' + paraStr + ',' + retStr);
                }

                fileOut.close();

            }
            // Just in case the file write failed
            catch (java.io.IOException ex) {
                System.out.println("Failed to write CSV files!");
            }
        }
    }
}
