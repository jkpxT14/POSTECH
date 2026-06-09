package edu.postech.csed232.exercise6;

import java.lang.reflect.Field;
import java.util.Optional;
import java.util.Set;

import org.jetbrains.annotations.NotNull;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertNotSame;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static edu.postech.csed232.exercise6.base.GameConstants.BOARD_SIZE;
import static edu.postech.csed232.exercise6.base.GameConstants.MAX_CELL_NUMBER;
import static edu.postech.csed232.exercise6.base.GameConstants.MIN_CELL_NUMBER;
import edu.postech.csed232.exercise6.base.GameInstanceFactory;
import edu.postech.csed232.exercise6.base.Observer;
import edu.postech.csed232.exercise6.base.Subject;
import edu.postech.csed232.exercise6.event.ActivationEvent;
import edu.postech.csed232.exercise6.event.Event;
import edu.postech.csed232.exercise6.event.NumberEvent;
import edu.postech.csed232.exercise6.model.Board;
import edu.postech.csed232.exercise6.model.Cell;
import edu.postech.csed232.exercise6.model.CellConstraint;
import edu.postech.csed232.exercise6.model.Group;

public class ModelTest{
    private Cell cell;

    private static class TestObserver implements Observer{
        int numberEventCount=0;
        int activationEventCount=0;
        Integer lastNumber=null;
        Boolean lastSet=null;
        Boolean lastActivated=null;

        @Override
        public void update(@NotNull Subject caller, @NotNull Event arg){
            if(arg instanceof NumberEvent event){
                ++numberEventCount;
                lastNumber=event.number();
                lastSet=event.set();
            }
            if(arg instanceof ActivationEvent event){
                ++activationEventCount;
                lastActivated=event.activated();
            }
        }
    }

    @BeforeEach
    void setup(){
        cell=new Cell();
    }

    @Test
    @DisplayName("cell: initially has no number and all possibilities 1-9")
    void cell_initiallyHasNoNumberAndAllPossibilities(){
        assertTrue(cell.getNumber().isEmpty());
        assertFalse(cell.containsPossibility(0));
        assertFalse(cell.containsPossibility(10));

        for(int n=MIN_CELL_NUMBER; n<=MAX_CELL_NUMBER; ++n){
            assertTrue(cell.containsPossibility(n));
        }
        assertFalse(cell.hasNoPossibility());
        assertTrue(cell.checkInv());
    }

    @Test
    @DisplayName("cell: setNumber succeeds only for an available number in an empty cell")
    void cell_setNumberSucceedsOnlyForAvailableNumberInEmptyCell(){
        var observer=new TestObserver();
        cell.addObserver(observer);

        assertFalse(cell.setNumber(0));
        assertFalse(cell.setNumber(10));
        assertEquals(0, observer.numberEventCount);

        cell.removePossibility(4);
        assertFalse(cell.setNumber(4));
        assertEquals(0, observer.numberEventCount);

        assertTrue(cell.setNumber(3));
        assertEquals(Optional.of(3), cell.getNumber());
        assertEquals(1, observer.numberEventCount);
        assertEquals(3, observer.lastNumber);
        assertEquals(true, observer.lastSet);

        assertFalse(cell.setNumber(5));
        assertEquals(Optional.of(3), cell.getNumber());
        assertEquals(1, observer.numberEventCount);
        assertTrue(cell.checkInv());
    }

    @Test
    @DisplayName("cell: unsetNumber succeeds only when the cell has a number")
    void cell_unsetNumberSucceedsOnlyWhenNumberExists(){
        var observer=new TestObserver();
        cell.addObserver(observer);

        assertFalse(cell.unsetNumber());
        assertEquals(0, observer.numberEventCount);

        assertTrue(cell.setNumber(7));
        assertTrue(cell.unsetNumber());
        assertTrue(cell.getNumber().isEmpty());

        assertEquals(2, observer.numberEventCount);
        assertEquals(7, observer.lastNumber);
        assertEquals(false, observer.lastSet);

        assertFalse(cell.unsetNumber());
        assertEquals(2, observer.numberEventCount);
        assertTrue(cell.checkInv());
    }

    @Test
    @DisplayName("cell: removePossibility deactivates the cell exactly when the last possibility is removed")
    void cell_removePossibilityDeactivatesOnlyWhenLastPossibilityIsRemoved(){
        var observer=new TestObserver();
        cell.addObserver(observer);

        cell.removePossibility(0);
        cell.removePossibility(1);
        cell.removePossibility(1);

        assertFalse(cell.containsPossibility(1));
        assertFalse(cell.hasNoPossibility());
        assertEquals(0, observer.activationEventCount);

        for(int n=2; n<=MAX_CELL_NUMBER; ++n){
            cell.removePossibility(n);
        }
        assertTrue(cell.hasNoPossibility());
        assertEquals(1, observer.activationEventCount);
        assertEquals(false, observer.lastActivated);

        cell.removePossibility(MAX_CELL_NUMBER);
        assertEquals(1, observer.activationEventCount);
        assertTrue(cell.checkInv());
    }

    @Test
    @DisplayName("cell: addPossibility activates the cell only when a valid possibility is restored")
    void cell_addPossibilityActivatesOnlyWhenValidPossibilityIsRestored(){
        CellConstraint noFive=number-> number!=5;
        cell.registerConstraint(noFive);
        cell.registerConstraint(noFive);

        assertFalse(cell.containsPossibility(5));

        for(int n=MIN_CELL_NUMBER; n<=MAX_CELL_NUMBER; ++n){
            cell.removePossibility(n);
        }
        assertTrue(cell.hasNoPossibility());

        var observer=new TestObserver();
        cell.addObserver(observer);

        cell.addPossibility(0);
        cell.addPossibility(5);

        assertTrue(cell.hasNoPossibility());
        assertEquals(0, observer.activationEventCount);

        cell.addPossibility(7);

        assertFalse(cell.hasNoPossibility());
        assertTrue(cell.containsPossibility(7));
        assertEquals(1, observer.activationEventCount);
        assertEquals(true, observer.lastActivated);

        cell.addPossibility(8);
        assertEquals(1, observer.activationEventCount);
        assertTrue(cell.checkInv());
    }

    @Test
    @DisplayName("cell: checkInv detects broken concrete representations")
    void cell_checkInvDetectsBrokenConcreteRepresentations(){
        var invalidNumberCell=new Cell();
        setPrivateField(invalidNumberCell, "number", 0);
        assertFalse(invalidNumberCell.checkInv());

        var nullPossibilityCell=new Cell();
        Set<Integer> nullPossibilities=getPrivateField(nullPossibilityCell, "possibilities");
        nullPossibilities.add(null);
        assertFalse(nullPossibilityCell.checkInv());

        var invalidPossibilityCell=new Cell();
        Set<Integer> invalidPossibilities=getPrivateField(invalidPossibilityCell, "possibilities");
        invalidPossibilities.add(10);
        assertFalse(invalidPossibilityCell.checkInv());

        var nullConstraintCell=new Cell();
        Set<CellConstraint> nullConstraints=getPrivateField(nullConstraintCell, "constraints");
        nullConstraints.add(null);
        assertFalse(nullConstraintCell.checkInv());

        var unavailablePossibilityCell=new Cell();
        Set<CellConstraint> constraints=getPrivateField(unavailablePossibilityCell, "constraints");
        constraints.add(number-> number!=5);
        assertFalse(unavailablePossibilityCell.checkInv());
    }

    @Test
    @DisplayName("group: isAvailable rejects invalid numbers and detects used numbers")
    void group_isAvailableRejectsInvalidNumbersAndDetectsUsedNumbers(){
        var group=new Group();
        var first=new Cell();
        var second=new Cell();

        group.addCell(first);
        group.addCell(second);

        assertFalse(group.isAvailable(0));
        assertFalse(group.isAvailable(10));
        assertTrue(group.isAvailable(1));
        assertTrue(group.isAvailable(9));

        assertTrue(first.setNumber(4));

        assertFalse(group.isAvailable(4));
        assertTrue(group.isAvailable(5));
    }

    @Test
    @DisplayName("group: update maintains possibilities when a number is set and unset")
    void group_updatesPossibilitiesWhenNumberIsSetAndUnset(){
        var group=new Group();
        var first=new Cell();
        var second=new Cell();

        group.addCell(first);
        group.addCell(second);
        group.addCell(second);

        assertTrue(first.setNumber(4));

        assertFalse(second.containsPossibility(4));
        assertTrue(group.checkInv());

        assertTrue(first.unsetNumber());

        assertTrue(second.containsPossibility(4));
        assertTrue(group.checkInv());
    }

    @Test
    @DisplayName("group: restoring a possibility respects other constraints of the cell")
    void group_restorePossibilityRespectsOtherConstraints(){
        var row=new Group();
        var column=new Group();

        var rowCell=new Cell();
        var sharedCell=new Cell();
        var columnCell=new Cell();

        row.addCell(rowCell);
        row.addCell(sharedCell);
        column.addCell(sharedCell);
        column.addCell(columnCell);

        assertTrue(columnCell.setNumber(5));
        assertFalse(sharedCell.containsPossibility(5));

        assertTrue(rowCell.setNumber(5));
        assertFalse(sharedCell.containsPossibility(5));

        assertTrue(rowCell.unsetNumber());
        assertFalse(sharedCell.containsPossibility(5));
        assertTrue(row.checkInv());
        assertTrue(column.checkInv());
    }

    @Test
    @DisplayName("group: update ignores non-cell callers, non-member cells, and unrelated events")
    void group_updateIgnoresIrrelevantEvents(){
        var group=new Group();
        var first=new Cell();
        var second=new Cell();

        group.addCell(first);
        group.addCell(second);

        group.update(new Subject(), new NumberEvent(3, true));
        assertTrue(second.containsPossibility(3));

        group.update(new Cell(), new NumberEvent(3, true));
        assertTrue(second.containsPossibility(3));

        group.update(first, new ActivationEvent(false));
        assertTrue(second.containsPossibility(3));
        assertTrue(group.checkInv());
    }

    @Test
    @DisplayName("group: checkInv accepts assigned cells with different numbers")
    void group_checkInvAcceptsDifferentAssignedNumbers(){
        var group=new Group();
        var first=new Cell();
        var second=new Cell();

        group.addCell(first);
        group.addCell(second);

        assertTrue(first.setNumber(3));
        assertTrue(second.setNumber(5));

        assertTrue(group.checkInv());
    }

    @Test
    @DisplayName("group: checkInv detects duplicate assigned numbers")
    void group_checkInvDetectsDuplicateAssignedNumbers(){
        var group=new Group();
        var first=new Cell();
        var second=new Cell();

        assertTrue(first.setNumber(3));
        assertTrue(second.setNumber(3));

        group.addCell(first);
        group.addCell(second);

        assertFalse(group.checkInv());
    }

    @Test
    @DisplayName("group: checkInv detects null members and invalid member cells")
    void group_checkInvDetectsBrokenMembers(){
        var groupWithNull=new Group();
        Set<Cell> cellsWithNull=getPrivateField(groupWithNull, "cells");
        cellsWithNull.add(null);
        assertFalse(groupWithNull.checkInv());

        var groupWithInvalidCell=new Group();
        var invalidCell=new Cell();
        setPrivateField(invalidCell, "number", 0);
        groupWithInvalidCell.addCell(invalidCell);
        assertFalse(groupWithInvalidCell.checkInv());
    }

    @Test
    @DisplayName("group: checkInv detects a possibility that should be blocked by another member's number")
    void group_checkInvDetectsPossibilityBlockedByMemberNumber(){
        var group=new Group();
        var first=new Cell();
        var second=new Cell();

        group.addCell(first);
        group.addCell(second);

        assertTrue(first.setNumber(4));
        assertFalse(second.containsPossibility(4));

        Set<Integer> possibilities=getPrivateField(second, "possibilities");
        possibilities.add(4);

        assertFalse(group.checkInv());
    }

    @Test
    @DisplayName("board: default puzzle is initialized correctly and preserves invariants")
    void board_defaultPuzzleIsInitializedCorrectly(){
        var board=new Board(GameInstanceFactory.createGameInstance());

        assertEquals(Optional.of(9), board.getCell(1, 3).getNumber());
        assertEquals(Optional.of(6), board.getCell(2, 2).getNumber());
        assertEquals(Optional.of(7), board.getCell(9, 9).getNumber());

        var firstCell=board.getCell(1, 1);
        assertTrue(firstCell.getNumber().isEmpty());
        assertTrue(firstCell.containsPossibility(5));
        assertTrue(firstCell.containsPossibility(8));

        for(int n: new int[]{1, 2, 3, 4, 6, 7, 9}){
            assertFalse(firstCell.containsPossibility(n));
        }
        assertTrue(board.checkInv());
    }

    @Test
    @DisplayName("board: getCell uses 1-based indices and returns the same cell object")
    void board_getCellUsesOneBasedIndicesAndReturnsSameCell(){
        var board=createEmptyBoard();

        assertSame(board.getCell(1, 1), board.getCell(1, 1));
        assertSame(board.getCell(9, 9), board.getCell(9, 9));
        assertNotSame(board.getCell(1, 1), board.getCell(1, 2));
        assertTrue(board.checkInv());
    }

    @Test
    @DisplayName("board: row, column, square, and diagonal constraints are connected")
    void board_constraintsAreConnected(){
        var board=createEmptyBoard();

        assertTrue(board.getCell(1, 1).setNumber(1));

        assertFalse(board.getCell(1, 2).containsPossibility(1));
        assertFalse(board.getCell(2, 1).containsPossibility(1));
        assertFalse(board.getCell(2, 2).containsPossibility(1));
        assertFalse(board.getCell(9, 9).containsPossibility(1));

        assertTrue(board.getCell(1, 9).setNumber(2));

        assertFalse(board.getCell(1, 8).containsPossibility(2));
        assertFalse(board.getCell(2, 9).containsPossibility(2));
        assertFalse(board.getCell(2, 8).containsPossibility(2));

        assertTrue(board.checkInv());
    }

    @Test
    @DisplayName("board: constructor rejects an invalid initial puzzle")
    void board_constructorRejectsInvalidInitialPuzzle(){
        assertThrows(IllegalArgumentException.class, ()-> new Board((i, j)->{
            if(i==1 && (j==1 || j==2)){
                return Optional.of(1);
            }
            return Optional.empty();
        }));
    }

    @Test
    @DisplayName("board: checkInv detects invalid cell and group arrays")
    void board_checkInvDetectsInvalidArrays(){
        var shortCellArrayBoard=createEmptyBoard();
        setPrivateField(shortCellArrayBoard, "cells", new Cell[BOARD_SIZE-1][BOARD_SIZE]);
        assertFalse(shortCellArrayBoard.checkInv());

        var nullRowBoard=createEmptyBoard();
        var nullRowCells=createCellMatrix();
        nullRowCells[0]=null;
        setPrivateField(nullRowBoard, "cells", nullRowCells);
        assertFalse(nullRowBoard.checkInv());

        var shortRowBoard=createEmptyBoard();
        var shortRowCells=createCellMatrix();
        shortRowCells[0]=new Cell[BOARD_SIZE-1];
        setPrivateField(shortRowBoard, "cells", shortRowCells);
        assertFalse(shortRowBoard.checkInv());

        var nullCellBoard=createEmptyBoard();
        var nullCellMatrix=createCellMatrix();
        nullCellMatrix[0][0]=null;
        setPrivateField(nullCellBoard, "cells", nullCellMatrix);
        assertFalse(nullCellBoard.checkInv());

        var nullGroupArrayBoard=createEmptyBoard();
        setPrivateField(nullGroupArrayBoard, "rowGroups", null);
        assertFalse(nullGroupArrayBoard.checkInv());

        var shortGroupArrayBoard=createEmptyBoard();
        setPrivateField(shortGroupArrayBoard, "columnGroups", new Group[BOARD_SIZE-1]);
        assertFalse(shortGroupArrayBoard.checkInv());

        var nullGroupBoard=createEmptyBoard();
        var groups=createGroupArray(BOARD_SIZE);
        groups[0]=null;
        setPrivateField(nullGroupBoard, "squareGroups", groups);
        assertFalse(nullGroupBoard.checkInv());
    }

    @Test
    @DisplayName("board: checkInv detects duplicate numbers in rows, columns, squares, and diagonals")
    void board_checkInvDetectsDuplicateNumbers(){
        var rowDuplicate=createCellMatrix();
        setCellNumber(rowDuplicate[0][0], 1);
        setCellNumber(rowDuplicate[0][1], 1);
        assertBoardWithCellsIsInvalid(rowDuplicate);

        var columnDuplicate=createCellMatrix();
        setCellNumber(columnDuplicate[0][0], 1);
        setCellNumber(columnDuplicate[1][0], 1);
        assertBoardWithCellsIsInvalid(columnDuplicate);

        var squareDuplicate=createCellMatrix();
        setCellNumber(squareDuplicate[0][0], 1);
        setCellNumber(squareDuplicate[1][1], 1);
        assertBoardWithCellsIsInvalid(squareDuplicate);

        var leftDiagonalDuplicate=createCellMatrix();
        setCellNumber(leftDiagonalDuplicate[0][0], 1);
        setCellNumber(leftDiagonalDuplicate[3][3], 1);
        assertBoardWithCellsIsInvalid(leftDiagonalDuplicate);

        var rightDiagonalDuplicate=createCellMatrix();
        setCellNumber(rightDiagonalDuplicate[0][8], 1);
        setCellNumber(rightDiagonalDuplicate[3][5], 1);
        assertBoardWithCellsIsInvalid(rightDiagonalDuplicate);
    }
    private static Board createEmptyBoard(){
        return new Board((i, j)-> Optional.empty());
    }
    private static Cell[][] createCellMatrix(){
        var cells=new Cell[BOARD_SIZE][BOARD_SIZE];
        for(int i=0; i<=BOARD_SIZE-1; ++i)
            for(int j=0; j<=BOARD_SIZE-1; ++j){
                cells[i][j]=new Cell();
            }
        return cells;
    }
    private static Group[] createGroupArray(int size){
        var groups=new Group[size];
        for(int i=0; i<size; ++i){
            groups[i]=new Group();
        }
        return groups;
    }
    private static void setCellNumber(@NotNull Cell cell, int number){
        setPrivateField(cell, "number", number);
    }
    private static void assertBoardWithCellsIsInvalid(@NotNull Cell[][] cells){
        var board=createEmptyBoard();
        setPrivateField(board, "cells", cells);
        assertFalse(board.checkInv());
    }
    private static void setPrivateField(@NotNull Object target, @NotNull String fieldName, Object value){
        try{
            Field field=target.getClass().getDeclaredField(fieldName);
            field.setAccessible(true);
            field.set(target, value);
        } catch(ReflectiveOperationException e){
            throw new AssertionError(e);
        }
    }

    @SuppressWarnings("unchecked")
    private static <T> T getPrivateField(@NotNull Object target, @NotNull String fieldName){
        try{
            Field field=target.getClass().getDeclaredField(fieldName);
            field.setAccessible(true);
            return (T) field.get(target);
        } catch(ReflectiveOperationException e){
            throw new AssertionError(e);
        }
    }
}