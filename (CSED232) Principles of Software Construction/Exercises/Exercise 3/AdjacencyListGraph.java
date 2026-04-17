package edu.postech.csed232.exercise3;

import java.util.Collections;
import java.util.Set;
import java.util.SortedMap;
import java.util.SortedSet;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.stream.Collectors;

import org.jetbrains.annotations.NotNull;

/**
 * An implementation of Graph with an adjacency list representation.
 *
 * @param <N> type of vertices, which must be immutable and comparable
 */
public class AdjacencyListGraph<N extends Comparable<N>> implements Graph<N>{
    private final @NotNull SortedMap<N, SortedSet<N>> adjMap;

    public AdjacencyListGraph(){
        adjMap = new TreeMap<>();
    }

    @Override
    public boolean addVertex(@NotNull N vertex){
        if(adjMap.containsKey(vertex)){
            return false;
        }

        adjMap.put(vertex, new TreeSet<>());
        return true;
    }

    @Override
    public boolean addEdge(@NotNull N source, @NotNull N target){
        if (source.equals(target)) {
            return false;
        }

        adjMap.putIfAbsent(source, new TreeSet<>());
        adjMap.putIfAbsent(target, new TreeSet<>());

        return adjMap.get(source).add(target);
    }

    @Override
    public boolean removeVertex(@NotNull N vertex){
        if(!adjMap.containsKey(vertex)){
            return false;
        }

        adjMap.remove(vertex);
        for(SortedSet<N> neighbors: adjMap.values()){
            neighbors.remove(vertex);
        }
        return true;
    }

    @Override
    public boolean removeEdge(@NotNull N source, @NotNull N target){
        if(!adjMap.containsKey(source)){
            return false;
        }

        return adjMap.get(source).remove(target);
    }

    @Override
    public boolean containsVertex(@NotNull N vertex){
        return adjMap.containsKey(vertex);
    }

    @Override
    public boolean containsEdge(@NotNull N source, @NotNull N target){
        return adjMap.containsKey(source) && adjMap.get(source).contains(target);
    }

    @Override
    public @NotNull Set<N> getVertices(){
        return Collections.unmodifiableSet(adjMap.keySet());
    }

    @Override
    public @NotNull Set<Edge<N>> getEdges(){
        return adjMap.entrySet().stream().flatMap(entry -> entry.getValue().stream().map(n -> new Edge<>(entry.getKey(), n))).collect(Collectors.toUnmodifiableSet());
    }

    @Override
    public @NotNull Set<N> getNeighborhood(N vertex){
        if(!adjMap.containsKey(vertex)){
            return Set.of();
        }

        return Set.copyOf(adjMap.get(vertex));
    }

    /**
     * Provides a human-readable string representation for the abstract value of the graph
     *
     * @return a string representation
     */
    @Override
    public String toString(){
        return String.format("[vertex: {%s}, edge: {%s}]", getVertices().stream().map(N::toString).collect(Collectors.joining(", ")), getEdges().stream().map(Edge::toString).collect(Collectors.joining(", ")));
    }
}