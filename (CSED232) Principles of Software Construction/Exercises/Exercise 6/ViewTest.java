package edu.postech.csed232.exercise6;

import java.awt.Color;
import java.awt.Dimension;

import javax.swing.JTextField;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static edu.postech.csed232.exercise6.base.GameConstants.ACTIVE_BORDER;
import static edu.postech.csed232.exercise6.base.GameConstants.CELL_FONT;
import static edu.postech.csed232.exercise6.base.GameConstants.INACTIVE_BORDER;
import static edu.postech.csed232.exercise6.base.GameConstants.MESSAGE_FONT;
import edu.postech.csed232.exercise6.model.Cell;
import edu.postech.csed232.exercise6.view.CellUI;
import edu.postech.csed232.exercise6.view.MessageUI;

public class ViewTest{
    private Cell cell;

    @BeforeEach
    void setup(){
        cell=new Cell();
    }

    @Test
    @DisplayName("MessageUI: initial state is correctly configured")
    void messageUI_initialStateIsCorrectlyConfigured(){
        var messageUI=new MessageUI();

        assertEquals("", messageUI.getText());
        assertEquals(MESSAGE_FONT, messageUI.getFont());
        assertEquals(new Dimension(400, 40), messageUI.getPreferredSize());
        assertNotNull(messageUI.getBorder());
    }

    @Test
    @DisplayName("MessageUI: text can be updated")
    void messageUI_textCanBeUpdated(){
        var messageUI=new MessageUI();

        messageUI.setText("Invalid input");

        assertEquals("Invalid input", messageUI.getText());
    }

    @Test
    @DisplayName("CellUI: getCell returns the associated cell")
    void cellUI_getCellReturnsAssociatedCell(){
        var cellUI=new CellUI(cell, false);

        assertSame(cell, cellUI.getCell());
    }

    @Test
    @DisplayName("CellUI: empty non-diagonal cell is initialized as an editable blank cell")
    void cellUI_emptyNonDiagonalCellIsInitializedCorrectly(){
        var cellUI=new CellUI(cell, false);

        assertEquals("", cellUI.getText());
        assertTrue(cellUI.isEditable());
        assertEquals(CELL_FONT, cellUI.getFont());
        assertEquals(JTextField.CENTER, cellUI.getHorizontalAlignment());
        assertEquals(ACTIVE_BORDER, cellUI.getBorder());
        assertFalse(cellUI.getFocusTraversalKeysEnabled());
        assertNotEquals(Color.LIGHT_GRAY, cellUI.getBackground());
    }

    @Test
    @DisplayName("CellUI: diagonal cell has light gray background")
    void cellUI_diagonalCellHasLightGrayBackground(){
        var cellUI=new CellUI(cell, true);

        assertEquals(Color.LIGHT_GRAY, cellUI.getBackground());
    }

    @Test
    @DisplayName("CellUI: pre-filled cell shows its number in blue and is read-only")
    void cellUI_prefilledCellShowsNumberInBlueAndIsReadOnly(){
        assertTrue(cell.setNumber(7));

        var cellUI=new CellUI(cell, false);

        assertEquals("7", cellUI.getText());
        assertEquals(Color.BLUE, cellUI.getForeground());
        assertFalse(cellUI.isEditable());
        assertEquals(ACTIVE_BORDER, cellUI.getBorder());
    }

    @Test
    @DisplayName("CellUI: setActivated(true) sets active border and keeps the cell editable")
    void cellUI_setActivatedTrueSetsActiveBorderAndEditable(){
        var cellUI=new CellUI(cell, false);

        cellUI.setActivated(true);

        assertEquals(ACTIVE_BORDER, cellUI.getBorder());
        assertTrue(cellUI.isEditable());
    }

    @Test
    @DisplayName("CellUI: setActivated(false) sets inactive border and keeps the cell editable")
    void cellUI_setActivatedFalseSetsInactiveBorderAndEditable(){
        var cellUI=new CellUI(cell, false);

        cellUI.setActivated(false);

        assertEquals(INACTIVE_BORDER, cellUI.getBorder());
        assertTrue(cellUI.isEditable());
    }

    @Test
    @DisplayName("CellUI: setActivated changes even a pre-filled cell back to editable")
    void cellUI_setActivatedChangesPrefilledCellBackToEditable(){
        assertTrue(cell.setNumber(7));
        var cellUI=new CellUI(cell, false);

        assertFalse(cellUI.isEditable());

        cellUI.setActivated(false);

        assertEquals(INACTIVE_BORDER, cellUI.getBorder());
        assertTrue(cellUI.isEditable());

        cellUI.setActivated(true);

        assertEquals(ACTIVE_BORDER, cellUI.getBorder());
        assertTrue(cellUI.isEditable());
    }
}