package edu.postech.csed232.exercise6;

import javax.swing.SwingUtilities;

import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;
import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static edu.postech.csed232.exercise6.base.GameConstants.ACTIVE_BORDER;
import static edu.postech.csed232.exercise6.base.GameConstants.INACTIVE_BORDER;
import static edu.postech.csed232.exercise6.base.GameConstants.MAX_CELL_NUMBER;
import static edu.postech.csed232.exercise6.base.GameConstants.MIN_CELL_NUMBER;
import edu.postech.csed232.exercise6.base.Observer;
import edu.postech.csed232.exercise6.base.Subject;
import edu.postech.csed232.exercise6.control.CellActivationController;
import edu.postech.csed232.exercise6.control.CellInputDocument;
import edu.postech.csed232.exercise6.control.MessageService;
import edu.postech.csed232.exercise6.control.MessageType;
import edu.postech.csed232.exercise6.control.MessageUIController;
import edu.postech.csed232.exercise6.event.ActivationEvent;
import edu.postech.csed232.exercise6.event.Event;
import edu.postech.csed232.exercise6.event.MessageEvent;
import edu.postech.csed232.exercise6.event.NumberEvent;
import edu.postech.csed232.exercise6.model.Cell;
import edu.postech.csed232.exercise6.model.Group;
import edu.postech.csed232.exercise6.view.CellUI;
import edu.postech.csed232.exercise6.view.MessageUI;

public class ControllerTest{
    private Cell cell;
    private TestObserver observer;
    private CellInputDocument document;
    private MessageService messageService;
    private MessageUI messageUI;

    static class TestObserver implements Observer{
        @Nullable
        String lastMsg=null;
        int messageCount=0;

        @Override
        public void update(@NotNull Subject caller, @NotNull Event arg){
            if(arg instanceof MessageEvent(String message)){
                lastMsg=message;
                ++messageCount;
            }
        }
    }

    @BeforeEach
    void setup(){
        cell=new Cell();
        observer=new TestObserver();
        messageService=new MessageService();
        messageService.addObserver(observer);
        document=new CellInputDocument(cell, messageService);
        messageUI=new MessageUI();
        new MessageUIController(messageUI, messageService);
    }

    @Test
    @DisplayName("MessageType: formats messages with and without arguments")
    void messageType_formatsMessages(){
        assertEquals("", MessageType.CLEAR.text());
        assertEquals("Available numbers: 1, 2, 3", MessageType.HINT_AVAILABLE.text("1, 2, 3"));
    }

    @Test
    @DisplayName("MessageService: dispatches simple messages")
    void messageService_dispatchesSimpleMessages(){
        messageService.handleHintNoSelection();
        assertEquals(MessageType.HINT_NO_SELECTION.text(), observer.lastMsg);

        messageService.handleHintHasNumber();
        assertEquals(MessageType.HINT_HAS_NUMBER.text(), observer.lastMsg);

        messageService.handleInvalidInput();
        assertEquals(MessageType.INVALID_INPUT.text(), observer.lastMsg);

        messageService.handleInvalidConstraint();
        assertEquals(MessageType.INVALID_CONSTRAINT.text(), observer.lastMsg);

        messageService.handleClearMessage();
        assertEquals(MessageType.CLEAR.text(), observer.lastMsg);
    }

    @Test
    @DisplayName("MessageService: sends available hint message when possibilities exist")
    void messageService_sendsAvailableHintMessage(){
        cell.removePossibility(2);
        cell.removePossibility(4);
        cell.removePossibility(9);

        messageService.handleHintPossibilities(cell);

        assertEquals(MessageType.HINT_AVAILABLE.text("1, 3, 5, 6, 7, 8"), observer.lastMsg);
    }

    @Test
    @DisplayName("MessageService: sends no-available hint message when no possibility exists")
    void messageService_sendsNoAvailableHintMessage(){
        for(int number=MIN_CELL_NUMBER; number<=MAX_CELL_NUMBER; ++number){
            cell.removePossibility(number);
        }
        messageService.handleHintPossibilities(cell);

        assertEquals(MessageType.HINT_NO_AVAILABLE.text(), observer.lastMsg);
    }

    @Test
    @DisplayName("MessageService: handles stuck and recovered states")
    void messageService_handlesStuckAndRecoveredStates(){
        messageService.handleStuckStatus(false, true);
        assertEquals(MessageType.STUCK.text(), observer.lastMsg);

        messageService.handleStuckStatus(true, true);
        assertEquals(MessageType.CLEAR.text(), observer.lastMsg);

        messageService.handleInvalidInput();
        int messageCount=observer.messageCount;

        messageService.handleStuckStatus(false, false);
        assertEquals(MessageType.INVALID_INPUT.text(), observer.lastMsg);
        assertEquals(messageCount, observer.messageCount);

        messageService.handleStuckStatus(true, false);
        assertEquals(MessageType.CLEAR.text(), observer.lastMsg);
        assertEquals(messageCount+1, observer.messageCount);
    }

    @Test
    @DisplayName("MessageUIController: updates message UI on MessageEvent")
    void messageUIController_updatesMessageUIOnMessageEvent(){
        messageService.handleInvalidInput();

        assertEquals(MessageType.INVALID_INPUT.text(), messageUI.getText());
    }

    @Test
    @DisplayName("MessageUIController: ignores non-message events")
    void messageUIController_ignoresNonMessageEvents(){
        var controller=new MessageUIController(messageUI, messageService);
        messageUI.setText("previous");

        controller.update(new Subject(), new NumberEvent(3, true));

        assertEquals("previous", messageUI.getText());
    }

    @Test
    @DisplayName("CellInputDocument: valid input sets number, inserts text, and clears message")
    void cellInputDocument_validInputSetsNumberAndClearsMessage(){
        assertDoesNotThrow(()-> document.insertString(0, "5", null));

        assertEquals("5", getDocumentText(document));
        assertEquals(5, cell.getNumber().orElseThrow());
        assertEquals(MessageType.CLEAR.text(), observer.lastMsg);
    }

    @Test
    @DisplayName("CellInputDocument: invalid inputs are rejected with invalid-input message")
    void cellInputDocument_rejectsInvalidInputs(){
        assertDoesNotThrow(()-> document.insertString(0, "0", null));
        assertEquals("", getDocumentText(document));
        assertTrue(cell.getNumber().isEmpty());
        assertEquals(MessageType.INVALID_INPUT.text(), observer.lastMsg);

        assertDoesNotThrow(()-> document.insertString(0, "a", null));
        assertEquals("", getDocumentText(document));
        assertTrue(cell.getNumber().isEmpty());
        assertEquals(MessageType.INVALID_INPUT.text(), observer.lastMsg);

        assertDoesNotThrow(()-> document.insertString(0, "12", null));
        assertEquals("", getDocumentText(document));
        assertTrue(cell.getNumber().isEmpty());
        assertEquals(MessageType.INVALID_INPUT.text(), observer.lastMsg);

        assertDoesNotThrow(()-> document.insertString(0, null, null));
        assertEquals("", getDocumentText(document));
        assertTrue(cell.getNumber().isEmpty());
        assertEquals(MessageType.INVALID_INPUT.text(), observer.lastMsg);
    }

    @Test
    @DisplayName("CellInputDocument: input into nonempty document is rejected")
    void cellInputDocument_rejectsInputIntoNonemptyDocument(){
        assertDoesNotThrow(()-> document.insertString(0, "5", null));
        assertEquals("5", getDocumentText(document));

        assertDoesNotThrow(()-> document.insertString(1, "6", null));

        assertEquals("5", getDocumentText(document));
        assertEquals(5, cell.getNumber().orElseThrow());
        assertEquals(MessageType.INVALID_INPUT.text(), observer.lastMsg);
    }

    @Test
    @DisplayName("CellInputDocument: constraint violation is rejected without inserting text")
    void cellInputDocument_rejectsConstraintViolation(){
        var group=new Group();
        var fixedCell=new Cell();
        var targetCell=new Cell();

        group.addCell(fixedCell);
        group.addCell(targetCell);

        assertTrue(fixedCell.setNumber(5));
        assertFalse(targetCell.containsPossibility(5));

        var targetDocument=new CellInputDocument(targetCell, messageService);
        assertDoesNotThrow(()-> targetDocument.insertString(0, "5", null));

        assertEquals("", getDocumentText(targetDocument));
        assertTrue(targetCell.getNumber().isEmpty());
        assertEquals(MessageType.INVALID_CONSTRAINT.text(), observer.lastMsg);
    }

    @Test
    @DisplayName("CellInputDocument: remove clears document, unsets number, and clears message")
    void cellInputDocument_removeClearsNumberAndMessage(){
        assertDoesNotThrow(()-> document.insertString(0, "5", null));
        assertEquals(5, cell.getNumber().orElseThrow());

        assertDoesNotThrow(()-> document.remove(0, 1));

        assertEquals("", getDocumentText(document));
        assertTrue(cell.getNumber().isEmpty());
        assertEquals(MessageType.CLEAR.text(), observer.lastMsg);
    }

    @Test
    @DisplayName("CellInputDocument: remove on empty cell still clears message")
    void cellInputDocument_removeOnEmptyCellClearsMessage(){
        messageService.handleInvalidInput();
        assertEquals(MessageType.INVALID_INPUT.text(), observer.lastMsg);

        assertDoesNotThrow(()-> document.remove(0, 0));

        assertTrue(cell.getNumber().isEmpty());
        assertEquals(MessageType.CLEAR.text(), observer.lastMsg);
    }

    @Test
    @DisplayName("CellInputDocument: notifyStuckIfNeeded sends stuck only for empty deactivated cells")
    void cellInputDocument_notifyStuckIfNeededForEmptyCell(){
        document.notifyStuckIfNeeded(false);
        assertEquals(MessageType.STUCK.text(), observer.lastMsg);

        document.notifyStuckIfNeeded(true);
        assertEquals(MessageType.CLEAR.text(), observer.lastMsg);
    }

    @Test
    @DisplayName("CellInputDocument: notifyStuckIfNeeded does not send stuck for assigned deactivated cells")
    void cellInputDocument_notifyStuckIfNeededForAssignedCell(){
        assertTrue(cell.setNumber(7));
        messageService.handleInvalidInput();
        int messageCount=observer.messageCount;

        document.notifyStuckIfNeeded(false);

        assertEquals(MessageType.INVALID_INPUT.text(), observer.lastMsg);
        assertEquals(messageCount, observer.messageCount);

        document.notifyStuckIfNeeded(true);

        assertEquals(MessageType.CLEAR.text(), observer.lastMsg);
        assertEquals(messageCount+1, observer.messageCount);
    }

    @Test
    @DisplayName("CellActivationController: deactivates and reactivates CellUI through ActivationEvent")
    void cellActivationController_updatesCellUIOnActivationEvents(){
        var cellUI=new CellUI(cell, false);
        new CellActivationController(cellUI);

        assertEquals(ACTIVE_BORDER, cellUI.getBorder());

        for(int number=MIN_CELL_NUMBER; number<=MAX_CELL_NUMBER; ++number){
            cell.removePossibility(number);
        }
        flushSwingEvents();

        assertEquals(INACTIVE_BORDER, cellUI.getBorder());

        cell.addPossibility(3);
        flushSwingEvents();

        assertEquals(ACTIVE_BORDER, cellUI.getBorder());
    }

    @Test
    @DisplayName("CellActivationController: directly handles both activation values")
    void cellActivationController_directlyHandlesBothActivationValues(){
        var cellUI=new CellUI(cell, false);
        var controller=new CellActivationController(cellUI);

        controller.update(cell, new ActivationEvent(false));
        flushSwingEvents();

        assertEquals(INACTIVE_BORDER, cellUI.getBorder());

        controller.update(cell, new ActivationEvent(true));
        flushSwingEvents();

        assertEquals(ACTIVE_BORDER, cellUI.getBorder());
    }

    @Test
    @DisplayName("CellActivationController: ignores non-activation and null events")
    void cellActivationController_ignoresNonActivationAndNullEvents(){
        var cellUI=new CellUI(cell, false);
        var controller=new CellActivationController(cellUI);

        assertEquals(ACTIVE_BORDER, cellUI.getBorder());

        controller.update(cell, new MessageEvent("ignored"));
        flushSwingEvents();

        assertEquals(ACTIVE_BORDER, cellUI.getBorder());

        controller.update(cell, null);
        flushSwingEvents();

        assertEquals(ACTIVE_BORDER, cellUI.getBorder());
    }
    private static String getDocumentText(@NotNull CellInputDocument document){
        try{
            return document.getText(0, document.getLength());
        } catch(Exception e){
            throw new AssertionError(e);
        }
    }
    private static void flushSwingEvents(){
        try{
            SwingUtilities.invokeAndWait(()-> {
            });
        } catch(Exception e){
            throw new AssertionError(e);
        }
    }
}