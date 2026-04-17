package edu.postech.csed232.exercise3;

import static org.junit.jupiter.api.Assertions.assertFalse;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

public class StringAdjacencyListGraphTest extends AbstractGraphTest<String, AdjacencyListGraph<String>>{
    @BeforeEach
    void setUp(){
        graph = new AdjacencyListGraph<>();
        v1="1";
        v2="2";
        v3="3";
        v4="4";
        v5="5";
        v6="6";
        v7="7";
        v8="8";
    }

    @Test
    @DisplayName("AdjacencyListGraph removeEdge returns false when source is absent")
    void removeEdge_whenSourceVertexIsAbsent_thenReturnsFalse(){
        assertFalse(graph.removeEdge(v1, v2));
    }
}