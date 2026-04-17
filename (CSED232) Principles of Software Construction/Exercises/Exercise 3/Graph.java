package edu.postech.csed232.exercise3;

import java.util.HashSet;
import java.util.Set;
import java.util.stream.Collectors;

import org.jetbrains.annotations.NotNull;

/**
 * A directed graph consisting of a set of vertices and edges. Self-loops are not allowed.
 *
 * @param <N> type of vertices, which must be immutable and comparable
 */
public interface Graph<N extends Comparable<N>>{
    /**
     * @param vertex a vertex to add
     * @return {@code true} if the graph is modified, {@code false} otherwise
     */
    boolean addVertex(@NotNull N vertex);

    /**
     * @param source a source vertex
     * @param target a target vertex
     * @return {@code true} if the graph is modified, {@code false} otherwise
     */
    boolean addEdge(@NotNull N source, @NotNull N target);

    /**
     * @param vertex a vertex to remove
     * @return {@code true} if the graph is modified, {@code false} otherwise
     */
    boolean removeVertex(@NotNull N vertex);

    /**
     * @param source a source vertex
     * @param target a target vertex
     * @return {@code true} if the graph is modified, {@code false} otherwise
     */
    boolean removeEdge(@NotNull N source, @NotNull N target);

    /**
     * @param vertex a vertex
     * @return {@code true} if the graph contains vertex
     */
    boolean containsVertex(@NotNull N vertex);

    /**
     * @param source a source vertex
     * @param target a target vertex
     * @return {@code true} if source and target is connected by an edge
     */
    boolean containsEdge(@NotNull N source, @NotNull N target);

    /**
     * @return the set of vertices in the graph
     */
    @NotNull Set<N> getVertices();

    /**
     * @return the set of edges in the graph
     */
    @NotNull Set<Edge<N>> getEdges();

    /**
     * @param vertex a vertex
     * @return the set of adjacent vertices of vertex
     */
    @NotNull Set<N> getNeighborhood(N vertex);

    /**
     * @param vertex a vertex
     * @return the set of reachable vertices from {@code vertex}
     */
    default @NotNull Set<N> findReachableVertices(@NotNull N vertex){
        var seen = new HashSet<N>();
        Set<N> frontier = new HashSet<>();

        if(containsVertex(vertex)){
            frontier.add(vertex);
        }

        while(!seen.containsAll(frontier)){
            seen.addAll(frontier);
            frontier=frontier.stream().flatMap(n -> getNeighborhood(n).stream()).filter(n -> !seen.contains(n)).collect(Collectors.toSet());
        }
        return Set.copyOf(seen);
    }
}