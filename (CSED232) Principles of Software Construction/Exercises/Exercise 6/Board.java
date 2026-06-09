package edu.postech.csed232.exercise6.model;

import org.jetbrains.annotations.NotNull;

import static edu.postech.csed232.exercise6.base.GameConstants.BOARD_SIZE;
import static edu.postech.csed232.exercise6.base.GameConstants.GRID_SIZE;
import edu.postech.csed232.exercise6.base.GameInstance;

/**
 * Represents an X-Sudoku board. It organizes a 9x9 grid of cells into rows, columns,
 * squares, and two main diagonals, enforcing Sudoku constraints across these groups.
 */
public class Board{
    @NotNull
    private final Cell[][] cells;
    @NotNull
    private final Group[] rowGroups;
    @NotNull
    private final Group[] columnGroups;
    @NotNull
    private final Group[] squareGroups;
    @NotNull
    private final Group[] diagonalGroups;

    /**
     * Constructs an X-Sudoku board with initial values taken from a game instance.
     * All constraint groups (rows, columns, squares, diagonals) are initialized,
     * and cells are wired into those groups accordingly.
     *
     * @param game an initial game instance providing the starting configuration
     */
    public Board(@NotNull GameInstance game){
        cells=new Cell[BOARD_SIZE][BOARD_SIZE];
        rowGroups=new Group[BOARD_SIZE];
        columnGroups=new Group[BOARD_SIZE];
        squareGroups=new Group[BOARD_SIZE];
        diagonalGroups=new Group[2];

        initializeCells();
        initializeGroups();
        connectCellsToGroups();
        setInitialNumbers(game);
    }

    /**
     * Returns the cell located in the specified row and column.
     * Indices are 1-based to match standard Sudoku notation.
     *
     * @param i the 1-based row index (1 to 9)
     * @param j the 1-based column index (1 to 9)
     * @return the cell at position (i, j)
     */
    @NotNull
    public Cell getCell(int i, int j){
        return cells[i-1][j-1];
    }

    /**
     * Checks if all class invariants and representation invariants hold for this object.
     *
     * @return true if the invariant holds
     */
    public boolean checkInv(){
        return checkCellArray() && checkGroupArray(rowGroups, BOARD_SIZE) && checkGroupArray(columnGroups, BOARD_SIZE) && checkGroupArray(squareGroups, BOARD_SIZE) && checkGroupArray(diagonalGroups, 2) && checkCells() && checkGroups() && checkRows() && checkColumns() && checkSquares() && checkDiagonals();
    }
    private void initializeCells(){
        for(int i=0; i<=BOARD_SIZE-1; ++i)
            for(int j=0; j<=BOARD_SIZE-1; ++j){
                cells[i][j]=new Cell();
            }
    }
    private void initializeGroups(){
        for(int i=0; i<=BOARD_SIZE-1; ++i){
            rowGroups[i]=new Group();
            columnGroups[i]=new Group();
            squareGroups[i]=new Group();
        }
        for(int i=0; i<=diagonalGroups.length-1; ++i){
            diagonalGroups[i]=new Group();
        }
    }
    private void connectCellsToGroups(){
        for(int row=0; row<=BOARD_SIZE-1; ++row)
            for(int column=0; column<=BOARD_SIZE-1; ++column){
                connectCellToGroups(row, column);
            }
    }
    private void connectCellToGroups(int row, int column){
        var cell=cells[row][column];

        rowGroups[row].addCell(cell);
        columnGroups[column].addCell(cell);
        squareGroups[getSquareIndex(row, column)].addCell(cell);

        if(row==column){
            diagonalGroups[0].addCell(cell);
        }
        if(row+column==BOARD_SIZE-1){
            diagonalGroups[1].addCell(cell);
        }
    }
    private int getSquareIndex(int row, int column){
        return row/GRID_SIZE*GRID_SIZE+column/GRID_SIZE;
    }
    private void setInitialNumbers(@NotNull GameInstance game){
        for(int row=0; row<=BOARD_SIZE-1; ++row){
            for(int column=0; column<=BOARD_SIZE-1; ++column){
                var number=game.getNumbers(row+1, column+1);
                if(number.isPresent() && !cells[row][column].setNumber(number.orElseThrow())){
                    throw new IllegalArgumentException("invalid initial game instance");
                }
            }
        }
    }
    private boolean checkCellArray(){
        if(cells.length!=BOARD_SIZE){
            return false;
        }
        for(var row: cells)
            if(row==null || row.length!=BOARD_SIZE){
                return false;
            }
        return true;
    }
    private boolean checkGroupArray(Group[] groups, int size){
        if(groups==null || groups.length!=size){
            return false;
        }
        for(var group: groups)
            if(group==null){
                return false;
            }
        return true;
    }
    private boolean checkCells(){
        for(var row: cells)
            for(var cell: row)
                if(cell==null || !cell.checkInv()){
                    return false;
                }
        return true;
    }
    private boolean checkGroups(){
        return checkGroups(rowGroups) && checkGroups(columnGroups) && checkGroups(squareGroups) && checkGroups(diagonalGroups);
    }
    private boolean checkGroups(Group[] groups){
        for(var group: groups)
            if(!group.checkInv()){
                return false;
            }
        return true;
    }
    private boolean checkRows(){
        for(int row=0; row<=BOARD_SIZE-1; ++row)
            if(!checkRow(row)){
                return false;
            }
        return true;
    }
    private boolean checkRow(int row){
        var used=new boolean[BOARD_SIZE+1];
        for(int column=0; column<=BOARD_SIZE-1; ++column)
            if(!addNumberIfPresent(used, cells[row][column])){
                return false;
            }
        return true;
    }
    private boolean checkColumns(){
        for(int column=0; column<=BOARD_SIZE-1; ++column)
            if(!checkColumn(column)){
                return false;
            }
        return true;
    }
    private boolean checkColumn(int column){
        var used=new boolean[BOARD_SIZE+1];
        for(int row=0; row<=BOARD_SIZE-1; ++row)
            if(!addNumberIfPresent(used, cells[row][column])){
                return false;
            }
        return true;
    }
    private boolean checkSquares(){
        for(int squareRow=0; squareRow<=GRID_SIZE-1; ++squareRow)
            for(int squareColumn=0; squareColumn<=GRID_SIZE-1; ++squareColumn)
                if(!checkSquare(squareRow, squareColumn)){
                    return false;
                }
        return true;
    }
    private boolean checkSquare(int squareRow, int squareColumn){
        var used=new boolean[BOARD_SIZE+1];
        int rowStart=squareRow*GRID_SIZE;
        int columnStart=squareColumn*GRID_SIZE;

        for(int row=rowStart; row<=rowStart+GRID_SIZE-1; ++row)
            for(int column=columnStart; column<=columnStart+GRID_SIZE-1; ++column)
                if(!addNumberIfPresent(used, cells[row][column])){
                    return false;
                }
        return true;
    }
    private boolean checkDiagonals(){
        return checkLeftDiagonal() && checkRightDiagonal();
    }
    private boolean checkLeftDiagonal(){
        var used=new boolean[BOARD_SIZE+1];
        for(int i=0; i<=BOARD_SIZE-1; ++i)
            if(!addNumberIfPresent(used, cells[i][i])){
                return false;
            }
        return true;
    }
    private boolean checkRightDiagonal(){
        var used=new boolean[BOARD_SIZE+1];
        for(int i=0; i<=BOARD_SIZE-1; ++i)
            if(!addNumberIfPresent(used, cells[i][BOARD_SIZE-1-i])){
                return false;
            }
        return true;
    }
    private boolean addNumberIfPresent(boolean[] used, @NotNull Cell cell){
        var number=cell.getNumber();
        if(number.isEmpty()){
            return true;
        }
        int value=number.orElseThrow();
        if(value<1 || value>BOARD_SIZE || used[value]){
            return false;
        }
        used[value]=true;
        return true;
    }
}