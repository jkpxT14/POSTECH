package edu.postech.csed232.exercise6.view;

import edu.postech.csed232.exercise6.model.Cell;
import org.jetbrains.annotations.NotNull;

import javax.swing.JTextField;
import java.awt.Color;

import static edu.postech.csed232.exercise6.base.GameConstants.ACTIVE_BORDER;
import static edu.postech.csed232.exercise6.base.GameConstants.INACTIVE_BORDER;
import static edu.postech.csed232.exercise6.base.GameConstants.CELL_FONT;

/**
 * UI component for a single Sudoku cell.
 */
public class CellUI extends JTextField {
    @NotNull
    private final Cell cell;

    /**
     * Constructs a CellUI.
     *
     * @param cell         the associated model cell
     * @param diagonal     whether the cell is on the board diagonal
     */
    public CellUI(@NotNull Cell cell, boolean diagonal) {
        this.cell = cell;
        initializeUI(diagonal);
    }

    /**
     * Returns the associated cell model.
     *
     * @return the cell model linked to this UI component
     */
    @NotNull
    public Cell getCell() {
        return cell;
    }

    /**
     * Mark this cell UI as activated or deactivated
     *
     * @param activated true if this cell UI is activated, and false otherwise
     */
    public void setActivated(boolean activated) {
        setBorder(activated ? ACTIVE_BORDER : INACTIVE_BORDER);
        setEditable(true);
    }

    private void initializeUI(boolean diagonal) {
        setFont(CELL_FONT);
        setHorizontalAlignment(JTextField.CENTER);
        setBorder(ACTIVE_BORDER);
        setFocusTraversalKeysEnabled(false);

        if (diagonal)
            setBackground(Color.LIGHT_GRAY);

        cell.getNumber().ifPresent(number -> {
            setForeground(Color.BLUE);
            setText(number.toString());
            setEditable(false);
        });
    }

}
