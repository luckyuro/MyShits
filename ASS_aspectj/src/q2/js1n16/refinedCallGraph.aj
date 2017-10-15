package q2.js1n16;

/**
 * Created by lambda-c on 3/19/17.
 */


import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.lang.Exception;
import java.util.HashSet;
import java.util.Set;


public aspect refinedCallGraph {
    //wantedMethod is the decription of the Methods meet requirment.
    pointcut wantedMethod(): call ( public int q2..*(int));
    //mainFunction is for after it to write csv file.
    pointcut mainFunction(): execution(public static * q2..main(..));

    // using Set to store the nodes'Signature, no duplication.
    Set<String> nodeSet = new HashSet<String>();

    // define a class to store edges, and Methods needed to use in Set
    // and use set to store the edge
    class Edge {
        String from;
        String to;

        Edge(String from, String to) {
            this.from = from;
            this.to = to;
        }

        public int hashCode() {
            return from.hashCode() * to.hashCode() * from.hashCode();
        }

        public boolean equals(Object o) {
            Edge s = (Edge) o;
            return this.from.equals(s.from) && this.to.equals(s.to);
        }

        public String toString() {
            return this.from + "->" + this.to;
        }

    }
    Set<Edge> edgeSet = new HashSet<Edge>();

    // for some reason did not use the failure handling of aspectj
    int around(int i):wantedMethod() && args(i){
        int answer = 0;
//        System.out.println(thisJoinPoint.getSignature());

        // the record's staff is similar to the first question
        //the Method that being called
        String thisSignature = thisJoinPointStaticPart.getSignature().toLongString();
        // add the signature to the set, (no duplication will exist
        // no matter succeed or not, the node should exists
        nodeSet.add(thisSignature);

        // calculate the original result, if failed, it won't be recorded
        // the Exception could be caught by the origin java code
        answer = proceed(i);

        //if everything's ok
        //the caller Method
        String enclosingSignature = thisEnclosingJoinPointStaticPart.getSignature().toLongString();
        // if the caller is already in the node, so the an edge should be generated and stored.
        if (nodeSet.contains(enclosingSignature)) {
            edgeSet.add(new Edge(enclosingSignature, thisSignature));
        }

        return answer;

    }



    after(): mainFunction(){
        System.out.println(nodeSet);
        System.out.println(edgeSet);
        // the file names
        String nodeFileName = "q2-nodes.csv";
        String edgeFileName = "q2-edges.csv";
        try {
            // open two weiter for the two files
            PrintWriter nodeOut
                    = new PrintWriter(new BufferedWriter(new FileWriter(nodeFileName)));
            PrintWriter edgeOut
                    = new PrintWriter(new BufferedWriter(new FileWriter(edgeFileName)));

            // iterate the nodes set ,write the nodes, one a line
            for (String s : nodeSet){
                nodeOut.println(s);
            }
            // iterate the edges, the toString will generate <From> -> <To>
            for (Edge e : edgeSet) {
                edgeOut.println(e.toString());
            }
            nodeOut.close();
            edgeOut.close();
        }
        // Just in case the file write failed
        catch (java.io.IOException ex){
            System.out.println("Failed to write CSV files!");
        }
    }
}
