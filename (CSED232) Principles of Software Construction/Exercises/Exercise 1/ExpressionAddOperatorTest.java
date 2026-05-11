package edu.postech.csed232.exercise1;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

public class ExpressionAddOperatorTest {

    final ExpressionAddOperator solver = new ExpressionAddOperator();
    @DisplayName("Parameterized cases for ExpressionAddOperator")
    @ParameterizedTest(name = "{index} => number={0}, target={1}, expected={2}")
    @CsvSource({
            "1234, 20, true",
            "1234, 22, false",
            "123, 0, true",
            "123, 7, false",
            "105, 5, true",
            "105, 7, false",
            "12345, 15, true",
            "12345, 100, false",
            "1111, 11, true",
            "1111, 5, false",
            "15, 17, false",
            "15, 15, true",
            "100200300, 100500, true",
            "100200300, 100000, false",
            "54321, 3, true", // 5+4-3-2-1
            "5678, 13, false",
            "987654, 13, true", // 98-76-5-4
            "1010, 10, false",
            "10101, 1, true", // 10-10+1
            "987, 2, false",
            "910, 19, true", // 9+10
            "1234, 50, false",
            "123456789, 100, true", // 123-45-67+89
            "123456789, 600, false",
            "555555555, 5055, true" // 5555-555+55
    })
    void testSolve(int number, int target, boolean expected) {
        assertEquals(expected, solver.solve(number, target));
    }

    @DisplayName("Exception when input number is negative")
    @ParameterizedTest(name = "exception => number={0}, target={1}")
    @CsvSource({"-123, 10"})
    void testSolveException(int number, int target) {
        assertThrows(IllegalArgumentException.class, () -> solver.solve(number, target));
    }

}