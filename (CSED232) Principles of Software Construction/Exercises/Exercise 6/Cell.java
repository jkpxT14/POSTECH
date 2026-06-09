package edu.postech.csed232.exercise6.model;

import java.util.HashSet;
import java.util.Optional;
import java.util.Set;

import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import static edu.postech.csed232.exercise6.base.GameConstants.MAX_CELL_NUMBER;
import static edu.postech.csed232.exercise6.base.GameConstants.MIN_CELL_NUMBER;
import edu.postech.csed232.exercise6.base.Observer;
import edu.postech.csed232.exercise6.base.Subject;
import edu.postech.csed232.exercise6.event.ActivationEvent;
import edu.postech.csed232.exercise6.event.NumberEvent;

/**
 * A cell that has a number and a set of possibilities. A cell may have a number of observers,
 * and notifies events to the observers.
 */
public class Cell extends Subject{
    @Nullable
    private Integer number;
    @NotNull
    private final Set<Integer> possibilities;
    @NotNull
    private final Set<CellConstraint> constraints;

    /**
     * Creates an empty cell. Initially, all numbers are possible.
     */
    public Cell(){
        number=null;
        possibilities=new HashSet<>();
        constraints=new HashSet<>();
        for(int i=MIN_CELL_NUMBER; i<=MAX_CELL_NUMBER; ++i){
            possibilities.add(i);
        }
    }

    /**
     * Returns the number assigned to this cell.
     *
     * @return the number, or Optional.empty() if the cell is empty
     */
    @NotNull
    public Optional<Integer> getNumber(){
        return Optional.ofNullable(number);
    }

    /**
     * Assigns a number to this cell if it is empty and the number is allowed.
     * Notifies observers if the number is set (NumberEvent).
     *
     * @param number the number to set
     * @return true if the number is successfully set
     */
    public boolean setNumber(int number){
        if(this.number!=null || !containsPossibility(number)){
            return false;
        }
        this.number=number;
        notifyObservers(new NumberEvent(number, true));
        return true;
    }

    /**
     * Unsets the assigned number if present.
     * Notifies observers if the number is removed (NumberEvent).
     *
     * @return true if the number was removed
     */
    public boolean unsetNumber(){
        if(number==null){
            return false;
        }
        int oldNumber=number;
        number=null;
        notifyObservers(new NumberEvent(oldNumber, false));
        return true;
    }

    /**
     * Registers a constraint for this cell and adds it as an observer if applicable.
     *
     * @param constraint the constraint (group) to register
     */
    public void registerConstraint(@NotNull CellConstraint constraint){
        if(constraints.add(constraint)){
            if(constraint instanceof Observer observer){
                addObserver(observer);
            }
            for(int i=MIN_CELL_NUMBER; i<=MAX_CELL_NUMBER; ++i)
                if(!constraint.isAvailable(i)){
                    removePossibility(i);
                }
        }
    }

    /**
     * Returns whether a number is currently considered possible.
     *
     * @param number the number to check
     * @return true if the number is a valid possibility
     */
    public boolean containsPossibility(int number){
        return isValidNumber(number) && possibilities.contains(number);
    }

    /**
     * Returns true if the cell has no possible values remaining.
     *
     * @return true if the set of possibilities is empty
     */
    public boolean hasNoPossibility(){
        return possibilities.isEmpty();
    }

    /**
     * Adds a possibility to this cell, if valid under all constraints. If the cell was previously
     * deactivated (no possibilities) and now becomes active, notifies with ActivationEvent(true).
     *
     * @param number the number to add as a possibility
     */
    public void addPossibility(int number){
        if(!isValidNumber(number) || !isAvailable(number)){
            return;
        }
        boolean wasDeactivated=hasNoPossibility();
        possibilities.add(number);
        if(wasDeactivated && !hasNoPossibility()){
            notifyObservers(new ActivationEvent(true));
        }
    }

    /**
     * Removes a possibility from this cell. If the cell becomes deactivated (no possibilities left),
     * notifies with ActivationEvent(false).
     *
     * @param number the number to remove
     */
    public void removePossibility(int number){
        if(!isValidNumber(number)){
            return;
        }
        boolean wasActivated=!hasNoPossibility();
        possibilities.remove(number);
        if(wasActivated && hasNoPossibility()){
            notifyObservers(new ActivationEvent(false));
        }
    }

    /**
     * Checks if all class invariants and representation invariants hold for this object.
     *
     * @return true if the invariant holds
     */
    public boolean checkInv(){
        if(number!=null && !isValidNumber(number)){
            return false;
        }
        if(possibilities.contains(null) || constraints.contains(null)){
            return false;
        }
        for(int possibleNumber: possibilities){
            if(!isValidNumber(possibleNumber)){
                return false;
            }
            if(number==null && !isAvailable(possibleNumber)){
                return false;
            }
        }
        return true;
    }
    private boolean isValidNumber(int number){
        return MIN_CELL_NUMBER<=number && number<=MAX_CELL_NUMBER;
    }
    private boolean isAvailable(int number){
        for(var constraint: constraints)
            if(!constraint.isAvailable(number)){
                return false;
            }
        return true;
    }
}