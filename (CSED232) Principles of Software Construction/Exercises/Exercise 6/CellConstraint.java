package edu.postech.csed232.exercise6.model;

/**
 * An interface for cell constraints.
 * A cell constraint represents a condition that restricts what values can be assigned to a cell.
 */
@FunctionalInterface
public interface CellConstraint {
    /**
     * Checks if the given number is allowed according to the constraint.
     *
     * @param number the number to check
     * @return true if the number is allowed; false otherwise
     */
    boolean isAvailable(int number);
}