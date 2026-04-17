package edu.postech.csed232.exercise3;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.jetbrains.annotations.NotNull;

/**
 * An implementation of Graph with an edge list representation.
 *
 * @param <N> type of vertices, which must be immutable and comparable
 */
public class EdgeListGraph<N extends Comparable<N>> implements Graph<N>{
    private final @NotNull Set<N> vertices;
    private final @NotNull List<Edge<N>> edges;

    public EdgeListGraph(){
        vertices = new HashSet<>();
        edges = new ArrayList<>();
    }

    @Override
    public boolean addVertex(@NotNull N vertex){
        return vertices.add(vertex);
    }

    @Override
    public boolean addEdge(@NotNull N source, @NotNull N target){
        if(source.equals(target)){
            return false;
        }

        Edge<N> edge = new Edge<>(source, target);
        if(edges.contains(edge)){
            return false;
        }

        vertices.add(source);
        vertices.add(target);
        edges.add(edge);
        return true;
    }

    @Override
    public boolean removeVertex(@NotNull N vertex){
        if(!vertices.remove(vertex)){
            return false;
        }
        edges.removeIf(edge -> edge.source().equals(vertex) || edge.target().equals(vertex));
        return true;
    }

    @Override
    public boolean removeEdge(@NotNull N source, @NotNull N target){
        return edges.remove(new Edge<>(source, target));
    }

    @Override
    public boolean containsVertex(@NotNull N vertex){
        return vertices.contains(vertex);
    }

    @Override
    public boolean containsEdge(@NotNull N source, @NotNull N target){
        return edges.contains(new Edge<>(source, target));
    }

    @Override
    public @NotNull Set<N> getVertices(){
        return Collections.unmodifiableSet(vertices);
    }

    @Override
    public @NotNull Set<Edge<N>> getEdges(){
        return Set.copyOf(edges);
    }

    @Override
    public @NotNull Set<N> getNeighborhood(N vertex){
        if(!vertices.contains(vertex)){
            return Set.of();
        }

        Set<N> neighbors = new HashSet<>();
        for (Edge<N> edge: edges) {
            if(edge.source().equals(vertex)){
                neighbors.add(edge.target());
            }
        }
        return Set.copyOf(neighbors);
    }
}