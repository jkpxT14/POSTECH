package edu.postech.csed232.exercise6.base;

import edu.postech.csed232.exercise6.event.Event;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.List;

/**
 * A subject that maintains a set of observers
 */
public class Subject {

    @NotNull
    private final List<Observer> observers = new ArrayList<>();

    /**
     * Adds an observer to the set of observers, provided that it is not already added.
     *
     * @param o an observer
     */
    public void addObserver(@NotNull Observer o) {
        if (!observers.contains(o))
            observers.add(o);
    }

    /**
     * Notify all the observers to indicate that this object has changed.
     *
     * @param arg an argument passed to the notify method of the observers
     */
    public void notifyObservers(@NotNull Event arg) {
        for (Observer o : observers)
            o.update(this, arg);
    }
}
