package q3.js1n16;

import org.aspectj.lang.Signature;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.*;

/**
 * Created by lambda-c on 3/22/17.
 */
public aspect executionTime {

    //  wantedMethod is the decription of the Methods meet requirment.
    // MENTION: here is execution and in  the other two aspect is call
    pointcut wantedMethod(): execution ( public int q3..*(int));
    //  mainFunction is for after it to write csv file.
    pointcut mainFunction(): execution(public static * q3..main(..));


    // define a class, store the times, calculatet the mean and standard derivation
    private class calClass {
        List<Long> numbers = new ArrayList<Long>();
        long sum = 0;
        double mean, sd;
        boolean meanOutdated = true, sdOutdated = true;

        calClass() {
        }

        public void addNum(long n) {
            numbers.add(n);
            sum += n;
            meanOutdated = true;
            sdOutdated = true;
        }

        private void calculateSD() {
            double mean = this.getMean();
            double allSquare = 0.0;
            for (long i : numbers) {
                allSquare += (i - mean) * (i - mean);
            }
            this.sd = Math.sqrt(allSquare / numbers.size());
        }

        private void calculateMean() {
            this.mean = 1.0 * sum / numbers.size();
            meanOutdated = false;
        }

        public double getMean() {
            if (meanOutdated) {
                this.calculateMean();
            }
            return this.mean;
        }

        public double getSD() {
            if (sdOutdated) {
                this.calculateSD();
            }
            return this.sd;

        }

        public boolean isEmpty(){
            return numbers.isEmpty()? true:false;
        }

        public String toString() {
            return numbers.toString();
        }


    }
    // one signature corresponds to an instance of the class defined above
    Map<Signature, calClass> runTimes = new HashMap<Signature, calClass>();


    int around(int i):wantedMethod() && args(i){

        // get the Signature's corresponding class, ready to store data
        Signature thisSignature = thisJoinPoint.getSignature();
        calClass timeList = runTimes.get(thisSignature);
        if (timeList == null) {
            timeList = new calClass();
            runTimes.put(thisSignature, timeList);
        }
        long startTime, endTime;


        // record the time defore proceed and after proceed
        // minus to get the difference, the duration
        // if the proceed is failed, the duration wont be added to the result
        // which means all failed functions are ignored to statistic
        startTime = System.nanoTime();
        int ans = proceed(i);
        endTime = System.nanoTime();

        long duration = endTime - startTime;
        timeList.addNum(duration);

        return ans;
    }


    after(): mainFunction(){
        try {
            PrintWriter fileOut
                    = new PrintWriter(new BufferedWriter(new FileWriter("runtimes.csv")));

            for (Signature s : runTimes.keySet()) {
                calClass timeList = runTimes.get(s);
                String methodName = s.toLongString();

                // if a function never succeed
                if (timeList.isEmpty()){
                    fileOut.println( methodName + ',' + "Never succeed" + ',' + "NA");
                    continue;
                }

                double mean = timeList.getMean();
                double sd = timeList.getSD();

                String meanStr = String.format("%.2f", mean);
                String sdStr = String.format("%.2f", sd);

                fileOut.println(methodName + ',' + meanStr + ',' + sdStr);
            }
            fileOut.close();
        }
        // Just in case the file write failed
        catch (java.io.IOException ex) {
            System.out.println("Failed to write CSV files!");
        }

    }

}
