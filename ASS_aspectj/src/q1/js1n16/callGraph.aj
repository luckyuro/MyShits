package q1.js1n16;
//import java.util.Stack;

import org.aspectj.lang.Signature;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.HashSet;
import java.util.Set;

/**
 * Created by lambda-c on 3/18/17.
 */
public aspect callGraph {
    //wantedMethod is the decription of the Methods meet requirment.
    pointcut wantedMethod(): call ( public int q1..*(int));
    //mainFunction is for after it to write csv file.
    pointcut mainFunction(): execution(public static * q1..main(..));

    // using Set to store the nodes'Signature, no duplication.
    Set<String> nodeSet = new HashSet<String>();
    // define a class to store edges, and Methods needed to use in Set
    // and use set to store the edge
    class Edge{
        String from;
        String to;

        Edge(String from, String to){
            this.from = from;
            this.to = to;
        }

        public int hashCode(){
            return from.hashCode()*to.hashCode()*from.hashCode();
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


    //using built-in functions
    //find every point that satisfy the requirement
    before():wantedMethod(){
        //the Method that being called
        String thisSignature = thisJoinPointStaticPart.getSignature().toLongString();
        // add the signature to the set, (no duplication will exist
        nodeSet.add(thisSignature);
        //the caller Method
        String enclosingSignature = thisEnclosingJoinPointStaticPart.getSignature().toLongString();

        // if the caller is already in the node, so the an edge should be generated and stored.
        if (nodeSet.contains(enclosingSignature)){
            edgeSet.add(new Edge(enclosingSignature, thisSignature));
        }

    }


    after() :mainFunction() {
        System.out.println(nodeSet);
        System.out.println(edgeSet);
        // the file names
        String nodeFileName = "q1-nodes.csv";
        String edgeFileName = "q1-edges.csv";
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


