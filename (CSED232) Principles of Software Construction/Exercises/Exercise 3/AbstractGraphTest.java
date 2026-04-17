package edu.postech.csed232.exercise3;

import java.util.Set;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import org.junit.jupiter.api.Disabled;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

/**
 * Shared black-box contract tests for Graph implementations.
 *
 * @param <N> type of vertices
 * @param <G> type of Graph
 */
@SuppressWarnings({"null", "nullness"})
@Disabled
abstract public class AbstractGraphTest<N extends Comparable<N>, G extends Graph<N>>{
    G graph;
    N v1, v2, v3, v4, v5, v6, v7, v8;

    @Test
    @DisplayName("addVertex returns true for absent vertex")
    void addVertex_whenAbsent_thenReturnsTrue(){
        assertTrue(graph.addVertex(v1));
    }

    @Test
    @DisplayName("addVertex rejects duplicate vertex")
    void addVertex_whenDuplicate_thenReturnsFalse(){
        assertTrue(graph.addVertex(v6));
        assertTrue(graph.addVertex(v7));
        assertFalse(graph.addVertex(v6));
        assertTrue(graph.containsVertex(v6));
        assertTrue(graph.containsVertex(v7));
    }

    @Test
    @DisplayName("addEdge returns true for new edge with existing vertices")
    void addEdge_whenVerticesPresentAndEdgeAbsent_thenReturnsTrue(){
        graph.addVertex(v1);
        graph.addVertex(v2);
        assertTrue(graph.addEdge(v1, v2));
        assertTrue(graph.containsEdge(v1, v2));
    }

    @Test
    @DisplayName("addEdge adds missing vertices automatically")
    void addEdge_whenVerticesAreMissing_thenAddsMissingVerticesAndReturnsTrue(){
        assertTrue(graph.addEdge(v3, v4));
        assertTrue(graph.containsVertex(v3));
        assertTrue(graph.containsVertex(v4));
        assertTrue(graph.containsEdge(v3, v4));
        assertEquals(Set.of(v3, v4), graph.getVertices());
    }

    @Test
    @DisplayName("addEdge returns false for duplicate edge")
    void addEdge_whenDuplicate_thenReturnsFalse(){
        assertTrue(graph.addEdge(v1, v2));
        assertFalse(graph.addEdge(v1, v2));
        assertEquals(Set.of(new Edge<>(v1, v2)), graph.getEdges());
    }

    @Test
    @DisplayName("removeVertex returns false for absent vertex")
    void removeVertex_whenAbsent_thenReturnsFalse(){
        graph.addEdge(v1, v2);
        graph.addEdge(v3, v4);
        assertFalse(graph.removeVertex(v8));
        assertEquals(Set.of(v1, v2, v3, v4), graph.getVertices());
        assertEquals(
            Set.of(
                new Edge<>(v1, v2),
                new Edge<>(v3, v4)
            ),
            graph.getEdges()
        );
    }

    @Test
    @DisplayName("removeVertex removes vertex and all incident edges")
    void removeVertex_whenPresent_thenRemovesVertexAndIncidentEdges(){
        graph.addEdge(v1, v2);
        graph.addEdge(v2, v3);
        graph.addEdge(v4, v2);
        graph.addEdge(v5, v6);
        assertTrue(graph.removeVertex(v2));
        assertFalse(graph.containsVertex(v2));
        assertFalse(graph.containsEdge(v1, v2));
        assertFalse(graph.containsEdge(v2, v3));
        assertFalse(graph.containsEdge(v4, v2));
        assertTrue(graph.containsEdge(v5, v6));
        assertEquals(Set.of(v1, v3, v4, v5, v6), graph.getVertices());
        assertEquals(Set.of(new Edge<>(v5, v6)), graph.getEdges());
    }

    @Test
    @DisplayName("removeEdge returns false for absent edge")
    void removeEdge_whenAbsent_thenReturnsFalse(){
        graph.addEdge(v2, v1);
        assertFalse(graph.removeEdge(v1, v2));
        assertTrue(graph.containsEdge(v2, v1));
        assertEquals(Set.of(new Edge<>(v2, v1)), graph.getEdges());
    }

    @Test
    @DisplayName("removeEdge removes only the specified edge")
    void removeEdge_whenPresent_thenRemovesOnlyThatEdge(){
        graph.addEdge(v1, v2);
        graph.addEdge(v2, v1);
        assertTrue(graph.removeEdge(v1, v2));
        assertFalse(graph.containsEdge(v1, v2));
        assertTrue(graph.containsEdge(v2, v1));
        assertEquals(Set.of(new Edge<>(v2, v1)), graph.getEdges());
        assertEquals(Set.of(v1, v2), graph.getVertices());
    }

    @Test
    @DisplayName("containsVertex returns false for absent vertex")
    void containsVertex_whenAbsent_thenReturnsFalse(){
        assertFalse(graph.containsVertex(v1));
    }

    @Test
    @DisplayName("containsVertex returns true for present vertex")
    void containsVertex_whenPresent_thenReturnsTrue(){
        graph.addVertex(v1);
        assertTrue(graph.containsVertex(v1));
    }

    @Test
    @DisplayName("containsEdge returns false for absent edge")
    void containsEdge_whenAbsent_thenReturnsFalse(){
        graph.addVertex(v1);
        graph.addVertex(v2);
        assertFalse(graph.containsEdge(v1, v2));
    }

    @Test
    @DisplayName("containsEdge returns true for present edge")
    void containsEdge_whenPresent_thenReturnsTrue(){
        graph.addEdge(v1, v2);
        assertTrue(graph.containsEdge(v1, v2));
    }

    @Test
    @DisplayName("containsEdge respects edge direction")
    void containsEdge_whenOnlyReverseEdgeExists_thenReturnsFalse(){
        graph.addEdge(v1, v2);
        assertFalse(graph.containsEdge(v2, v1));
    }

    @Test
    @DisplayName("getVertices returns empty set for empty graph")
    void getVertices_whenGraphIsEmpty_thenReturnsEmptySet(){
        assertEquals(Set.of(), graph.getVertices());
    }

    @Test
    @DisplayName("getVertices returns all vertices in graph")
    void getVertices_whenGraphHasVertices_thenReturnsAllVertices(){
        graph.addVertex(v1);
        graph.addVertex(v3);
        graph.addVertex(v5);
        assertEquals(Set.of(v1, v3, v5), graph.getVertices());
    }

    @Test
    @DisplayName("getEdges returns empty set for empty graph")
    void getEdges_whenGraphIsEmpty_thenReturnsEmptySet(){
        assertEquals(Set.of(), graph.getEdges());
    }

    @Test
    @DisplayName("getEdges returns all edges in graph")
    void getEdges_whenGraphHasEdges_thenReturnsAllEdges(){
        graph.addEdge(v1, v2);
        graph.addEdge(v1, v4);
        graph.addEdge(v3, v6);
        assertEquals(
            Set.of(
                new Edge<>(v1, v2),
                new Edge<>(v1, v4),
                new Edge<>(v3, v6)
            ),
            graph.getEdges()
        );
    }

    @Test
    @DisplayName("getNeighborhood returns empty set for absent vertex")
    void getNeighborhood_whenVertexAbsent_thenReturnsEmptySet(){
        assertEquals(Set.of(), graph.getNeighborhood(v1));
    }

    @Test
    @DisplayName("getNeighborhood returns all adjacent vertices")
    void getNeighborhood_whenVertexHasOutgoingEdges_thenReturnsAdjacentVertices(){
        graph.addEdge(v1, v2);
        graph.addEdge(v1, v4);
        graph.addEdge(v3, v1);
        assertEquals(Set.of(v2, v4), graph.getNeighborhood(v1));
    }

    @Test
    @DisplayName("getNeighborhood returns empty set for vertex with no outgoing edges")
    void getNeighborhood_whenVertexHasNoOutgoingEdges_thenReturnsEmptySet(){
        graph.addEdge(v1, v2);
        graph.addVertex(v3);
        assertEquals(Set.of(), graph.getNeighborhood(v2));
        assertEquals(Set.of(), graph.getNeighborhood(v3));
    }

    @Test
    @DisplayName("findReachableVertices returns all reachable vertices")
    void findReachableVertices_whenGraphHasMultipleComponents_thenReturnsReachableSet(){
        graph.addEdge(v1, v2);
        graph.addEdge(v1, v4);
        graph.addEdge(v2, v4);
        graph.addEdge(v3, v6);
        graph.addEdge(v6, v3);
        assertEquals(Set.of(v1, v2, v4), graph.findReachableVertices(v1));
        assertEquals(Set.of(v2, v4), graph.findReachableVertices(v2));
        assertEquals(Set.of(v3, v6), graph.findReachableVertices(v3));
        assertEquals(Set.of(v4), graph.findReachableVertices(v4));
        assertTrue(graph.findReachableVertices(v5).isEmpty());
    }
}