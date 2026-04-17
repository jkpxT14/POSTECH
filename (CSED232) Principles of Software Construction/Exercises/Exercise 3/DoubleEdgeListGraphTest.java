package edu.postech.csed232.exercise3;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

public class DoubleEdgeListGraphTest extends AbstractGraphTest<Double, EdgeListGraph<Double>>{
    @BeforeEach
    void setUp(){
        graph = new EdgeListGraph<>();
        v1=1.1;
        v2=2.2;
        v3=3.3;
        v4=4.4;
        v5=5.5;
        v6=6.6;
        v7=7.7;
        v8=8.8;
    }

    @Test
    @DisplayName("EdgeListGraph rejects self-loops")
    void addEdge_whenSelfLoop_thenReturnsFalseAndDoesNotChangeGraph(){
        assertFalse(graph.addEdge(v1, v1));
        assertFalse(graph.containsVertex(v1));
        assertTrue(graph.getVertices().isEmpty());
        assertTrue(graph.getEdges().isEmpty());
    }
}