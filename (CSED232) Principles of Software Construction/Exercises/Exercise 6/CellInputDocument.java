package edu.postech.csed232.exercise6.control;

import edu.postech.csed232.exercise6.model.Cell;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.PlainDocument;

/**
 * A custom document for user input in a CellUI, enforcing Sudoku input constraints
 * and delegating message handling to MessageController.
 */
public class CellInputDocument extends PlainDocument {
    @NotNull
    private final Cell cell;
    @NotNull
    private final MessageService messageService;

    /**
     * Constructs a CellInputDocument with the associated cell and message controller.
     *
     * @param cell              the associated cell model
     * @param messageService the controller to update messages based on input
     */
    public CellInputDocument(@NotNull Cell cell, @NotNull MessageService messageService) {
        this.cell = cell;
        this.messageService = messageService;
    }

    /**
     * Attempts to insert a string into the document if it is a valid Sudoku number (1–9).
     * Triggers appropriate feedback via the message controller based on input validity.
     *
     * @param offset the position in the document to insert the content
     * @param str    the string to insert
     * @param attr   the attribute set for the inserted content
     * @throws BadLocationException if the insert position is invalid
     */
    @Override
    public void insertString(int offset, @Nullable String str, AttributeSet attr) throws BadLocationException {
        boolean isEmptyDocument = getLength() == 0;
        boolean isSingleDigit = str != null && str.matches("[1-9]");

        if (isEmptyDocument && isSingleDigit) {
            int number = Integer.parseInt(str);
            if (cell.setNumber(number)) {
                super.insertString(offset, str, attr);
                messageService.handleClearMessage();
            } else {
                messageService.handleInvalidConstraint();
            }
        } else {
            messageService.handleInvalidInput();
        }
    }

    /**
     * Removes content from the document and clears the associated number in the cell.
     * Triggers message controller to clear any previously displayed message.
     *
     * @param offs the starting offset to remove content
     * @param len  the length of content to remove
     * @throws BadLocationException if the removal parameters are invalid
     */
    @Override
    public void remove(int offs, int len) throws BadLocationException {
        super.remove(offs, len);
        cell.unsetNumber();
        messageService.handleClearMessage();
    }

    /**
     * Triggers a message update if the cell becomes stuck or reactivated based on its activation state.
     *
     * @param activated true if the cell is currently activated; false otherwise
     */
    public void notifyStuckIfNeeded(boolean activated) {
        boolean isEmptyCell = cell.getNumber().isEmpty();
        messageService.handleStuckStatus(activated, isEmptyCell);
    }

}
