package edu.postech.csed232.exercise3;

import java.util.Comparator;

import org.jetbrains.annotations.NotNull;

/**
 * @param <N> type of vertices, which must be immutable and comparable
 */
public record Edge<N extends Comparable<N>>(@NotNull N source, @NotNull N target) implements Comparable<Edge<N>>{
    /**
     * Compares this edge with another edge. Edges are first compared by their source vertices; if source vertices are equal, they are compared by their target vertices.
     * Uses the natural ordering of the vertex type N.
     *
     * @param o the other edge to compare with
     * @return a negative integer, zero, or a positive integer as this edge is less than, equal to, or greater than the specified edge
     */
    @Override
    public int compareTo(@NotNull Edge<N> o){
        Comparator<N> order=Comparator.naturalOrder();
        int bySource=order.compare(source(), o.source());
        return bySource!=0?bySource:order.compare(target(), o.target());
    }

    /**
     * Returns a string representation of this edge in the format "(source,target)".
     *
     * @return a string representation of this edge
     */
    @Override
    public String toString(){
        return "("+source+","+target+")";
    }
}