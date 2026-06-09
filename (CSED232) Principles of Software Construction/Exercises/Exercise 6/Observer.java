package edu.postech.csed232.exercise6.base;

import edu.postech.csed232.exercise6.event.Event;
import org.jetbrains.annotations.NotNull;

/**
 * An observer that can observe subjects.
 */
@FunctionalInterface
public interface Observer {
    /**
     * This method is called whenever the observed object is changed.
     *
     * @param caller the subject
     * @param arg    an argument passed to caller
     */
    void update(@NotNull Subject caller, @NotNull Event arg);
}