package edu.postech.csed232.exercise6.event;

/**
 * An event representing a change to the number in a cell.
 *
 * @param number the number involved in the event
 * @param set    true if the number was set; false if it was unset
 */
public record NumberEvent(int number, boolean set) implements Event {
}
