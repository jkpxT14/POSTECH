package edu.postech.csed232.exercise6.control;

import javax.swing.SwingUtilities;

import org.jetbrains.annotations.NotNull;

import edu.postech.csed232.exercise6.base.Observer;
import edu.postech.csed232.exercise6.base.Subject;
import edu.postech.csed232.exercise6.event.ActivationEvent;
import edu.postech.csed232.exercise6.event.Event;
import edu.postech.csed232.exercise6.view.CellUI;

/**
 * Controller that observes a Cell model and updates the associated CellUI accordingly.
 */
public class CellActivationController implements Observer{
    @NotNull
    private final CellUI cellUI;

    /**
     * Constructs the controller and registers it to the observed Cell.
     *
     * @param cellUI the associated view component
     */
    public CellActivationController(@NotNull CellUI cellUI){
        this.cellUI=cellUI;
        cellUI.getCell().addObserver(this);
    }

    /**
     * Reacts to changes in the Cell model and updates the view accordingly.
     *
     * @param caller the observed subject
     * @param arg    the event describing the change
     */
    @Override
    public void update(@NotNull Subject caller, @NotNull Event arg){
        if(!(arg instanceof ActivationEvent event)){
            return;
        }
        SwingUtilities.invokeLater(()-> cellUI.setActivated(event.activated()));
    }
}