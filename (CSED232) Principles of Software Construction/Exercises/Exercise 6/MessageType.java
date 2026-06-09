package edu.postech.csed232.exercise6.control;

import org.jetbrains.annotations.NotNull;

/**
 * Enumerates all user-facing message templates. Messages may optionally
 * be formatted using parameters.
 */
public enum MessageType {
    HINT_NO_SELECTION("Please select a cell to get a hint."),
    HINT_HAS_NUMBER("This cell already has a number."),
    HINT_NO_AVAILABLE("This cell has no available numbers."),
    HINT_AVAILABLE("Available numbers: %s"),

    INVALID_INPUT("Invalid input. Please enter a number between 1 and 9."),
    INVALID_CONSTRAINT("Invalid number due to Sudoku constraints"),

    STUCK("The puzzle is stuck: a cell has no remaining possibilities."),
    CLEAR("");

    private final String template;

    MessageType(String template) {
        this.template = template;
    }

    public @NotNull String text(Object... args) {
        return args.length > 0 ? template.formatted(args) : template;
    }
}
