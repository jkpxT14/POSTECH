package edu.postech.csed232.exercise6.model;

import edu.postech.csed232.exercise6.base.Observer;
import edu.postech.csed232.exercise6.base.Subject;
import edu.postech.csed232.exercise6.event.Event;
import edu.postech.csed232.exercise6.event.NumberEvent;
import org.jetbrains.annotations.NotNull;

import java.util.HashSet;
import java.util.Set;

import static edu.postech.csed232.exercise6.base.GameConstants.MAX_CELL_NUMBER;
import static edu.postech.csed232.exercise6.base.GameConstants.MIN_CELL_NUMBER;

/**
 * A group that observes a set of cells, and maintains the invariant: if one
 * of the members has a particular value, none of its other members can have
 * that value as a possibility. Also reacts to cell updates by updating the
 * set of allowed possibilities.
 */
public class Group implements Observer, CellConstraint{
    @NotNull
    private final Set<Cell> cells;

    /**
     * Creates an empty group.
     */
    public Group(){
        cells=new HashSet<>();
    }

    /**
     * Adds a cell to this group and registers this group as a constraint and observer.
     * Assumes the cell has not yet been assigned a number.
     *
     * @param cell a cell to be added
     */
    public void addCell(@NotNull Cell cell){
        if(cells.add(cell)){
            cell.registerConstraint(this);
        }
    }

    /**
     * Returns true if the given number is not currently used in any member cell.
     *
     * @param number the number to check
     * @return true if the number is not used; false otherwise
     */
    @Override
    public boolean isAvailable(int number){
        if(!isValidNumber(number)){
            return false;
        }
        for(var cell: cells)
            if(cell.getNumber().isPresent() && cell.getNumber().orElseThrow()==number){
                return false;
            }
        return true;
    }

    /**
     * Responds to a number assignment or removal by updating possible values in the other
     * cells. If one of the cells has a particular value, none of its other members can have
     * the value as a possibility.
     *
     * @param caller the subject that triggered the event
     * @param arg    the event describing the number assignment or removal
     */
    @Override
    public void update(@NotNull Subject caller, @NotNull Event arg){
        if(!(caller instanceof Cell changedCell) || !cells.contains(changedCell)){
            return;
        }
        if(arg instanceof NumberEvent(int number, boolean set)){
            for(var cell: cells){
                if(cell==changedCell){
                    continue;
                }
                if(set){
                    cell.removePossibility(number);
                } else{
                    cell.addPossibility(number);
                }
            }
        }
    }

    /**
     * Checks if all class invariants and representation invariants hold for this object.
     *
     * @return true if the invariant holds
     */
    public boolean checkInv(){
        if(cells.contains(null)){
            return false;
        }
        var usedNumbers=new HashSet<Integer>();
        for(var cell: cells){
            if(!cell.checkInv()){
                return false;
            }
            var number=cell.getNumber();
            if(number.isPresent()){
                int value=number.orElseThrow();
                if(!isValidNumber(value) || !usedNumbers.add(value)){
                    return false;
                }
            }
        }
        for(var cell: cells){
            var number=cell.getNumber();
            if(number.isPresent() && hasOtherEmptyCellWithPossibility(cell, number.orElseThrow())){
                return false;
            }
        }
        return true;
    }
    private boolean hasOtherEmptyCellWithPossibility(@NotNull Cell cell, int number){
        for(var otherCell: cells)
            if(otherCell!=cell && otherCell.getNumber().isEmpty() && otherCell.containsPossibility(number)){
                return true;
            }
        return false;
    }
    private boolean isValidNumber(int number){
        return MIN_CELL_NUMBER<=number && number<=MAX_CELL_NUMBER;
    }
}