package edu.postech.csed232.exercise2;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.List;
import java.util.stream.IntStream;
import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.*;

@DisplayName("Hoping Rabbits Game Test Suite")
class HopingRabbitsGameTest {

    /**
     * Provides test scenarios for the Hoping Rabbits Game, including various board sizes,
     * move sequences, expected state transitions, and final game conditions (stuck/goal).
     * Each scenario tests different aspects: detailed intermediate checks vs. final state only.
     */
    static Stream<Arguments> testScenarios() {
        return Stream.of(
            Arguments.of(
                2,
                List.of("X", "O", "X", "X", "O"),
                List.of("xx_oo", "x_xoo", "xox_o", "xo_xo", "_oxxo", "o_xxo"),
                true,
                false
            ),
            Arguments.of(
                3,
                List.of("O", "X", "O", "X", "O", "O", "O", "O", "O", "O"),
                List.of("xo_xxoo"),
                false,
                false
            ),
            Arguments.of(
                5,
                List.of("X", "O", "X", "X", "O", "X", "O", "O", "O", "O"),
                List.of("xxo_xxxoooo"),
                false,
                false
            ),
            Arguments.of(
                10,
                List.of("O", "X", "O", "O", "X", "X", "X", "X", "X", "O", "O", "X", "X", "X", "O", "O", "X", "X", "O", "X", "O", "X", "X", "O", "X", "O", "O", "O", "O", "X"),
                List.of("xxxxxxxxxooooooooo_ox"),
                false,
                false
            ),
            Arguments.of(
                50,
                List.of("O", "X", "O", "X", "X", "O", "O", "O", "O", "O", "X", "X", "O", "X", "X", "X", "X", "X", "O", "X", "X", "O", "O", "O", "O", "O", "X", "O", "O", "X"),
                List.of("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx_xxxxxxxxoxxxxxooooooooooooooooooooooooooooooooooooooooooooooooo"),
                false,
                false
            ),
            Arguments.of(
                2,
                List.of("O", "X", "X", "X", "O", "O", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "X", "O"),
                List.of("oo_xx"),
                true,
                true
            ),
            Arguments.of(
                3,
                List.of("O", "X", "X", "O", "O", "O", "X", "X", "X", "O", "X", "X", "O", "O", "O", "X", "O", "X", "X", "X", "O"),
                List.of("ooo_xxx"),
                true,
                true
            ),
            Arguments.of(
                4,
                List.of("O", "X", "X", "O", "O", "O", "X", "X", "X", "X", "X", "X", "X", "O", "O", "O", "O", "X", "O", "O", "X", "X", "X", "X", "O", "X", "X", "O", "O", "X", "X", "X", "X", "X", "O"),
                List.of("oooo_xxxx"),
                true,
                true
            ),
            Arguments.of(
                5,
                List.of("X", "O", "O", "X", "X", "X", "O", "O", "O", "O", "X", "X", "X", "X", "X", "O", "O", "O", "O", "O", "X", "O", "O", "O", "X", "X", "X", "X", "O", "O", "O", "O", "X", "X", "X", "O", "X", "O", "O", "X"),
                List.of("ooooo_xxxxx"),
                true,
                true
            ),
            Arguments.of(
                5,
                List.of("X", "O", "O", "X", "X", "X", "O", "O", "O", "O", "X", "X", "X", "X", "X", "O", "O", "O", "O", "O", "X", "X", "X", "X", "X", "O", "X", "X", "O", "O", "O", "O", "O", "O", "X", "O", "X", "X", "O", "O", "X"),
                List.of("ooooo_xxxxx"),
                true,
                true
            )
        );
    }

    @ParameterizedTest(name = "n={0}, moves={1}, expectedStates={2}, stuck={3}, goal={4}")
    @DisplayName("Game Scenario: {0} rabbits, {3} stuck, {4} goal")
    @MethodSource("testScenarios")
    void testGame(int n, List<String> moves, List<String> expectedStates, boolean expectedStuck, boolean expectedGoal) {
        var game = new HopingRabbitsGame(n);
        
        boolean isDetailedTest = expectedStates.size() > 1; // Detailed tests have intermediate state checks
        if (isDetailedTest) {
            performDetailedTest(game, expectedStates, moves);
        } else {
            performSimpleTest(game, expectedStates.get(0), moves);
        }

        assertGameState(game, expectedStuck, expectedGoal);
    }

    private void performDetailedTest(HopingRabbitsGame game, List<String> expectedStateList, List<String> moveList) {
        // Check initial state
        assertEquals(expectedStateList.get(0), game.toString(), "Initial state mismatch");
        
        IntStream.range(0, moveList.size()).forEach(i -> {
            String moveStr = moveList.get(i);
            var move = Rabbit.valueOf(moveStr);
            boolean moved = game.move(move);
            assertTrue(moved, "Move " + (i + 1) + " failed");
            assertEquals(expectedStateList.get(i + 1), game.toString(), "State after move " + (i + 1) + " mismatch");
        });
    }

    /**
     * Performs moves without intermediate checks, only final state verification.
     */
    private void performSimpleTest(HopingRabbitsGame game, String expectedFinalState, List<String> moveList) {
        moveList.forEach(moveStr -> game.move(Rabbit.valueOf(moveStr)));
        assertEquals(expectedFinalState, game.toString());
    }

    /**
     * Asserts the final game state (stuck and goal).
     */
    private void assertGameState(HopingRabbitsGame game, boolean expectedStuck, boolean expectedGoal) {
        assertEquals(expectedStuck, game.isStuck(), "Stuck state mismatch");
        assertEquals(expectedGoal, game.isGoal(), "Goal state mismatch");
    }
}
