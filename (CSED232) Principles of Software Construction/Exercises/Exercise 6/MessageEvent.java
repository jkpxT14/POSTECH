package edu.postech.csed232.exercise6.event;

/**
 * An event representing a message to be displayed in the UI.
 *
 * @param message the content of the message
 */
public record MessageEvent(String message) implements Event {
}
