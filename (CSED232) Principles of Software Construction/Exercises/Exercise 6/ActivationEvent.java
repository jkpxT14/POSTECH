package edu.postech.csed232.exercise6.event;

/**
 * An event indicating that a cell has become either activated or deactivated.
 * Activation means the cell has at least one remaining valid possibility.
 *
 * @param activated true if the cell is activated; false if deactivated
 */
public record ActivationEvent(boolean activated) implements Event {
}
