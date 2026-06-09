package edu.postech.csed232.exercise6.view;

import edu.postech.csed232.exercise6.base.GameInstanceFactory;
import edu.postech.csed232.exercise6.control.CellInputDocument;
import edu.postech.csed232.exercise6.control.CellActivationController;
import edu.postech.csed232.exercise6.control.MessageUIController;
import edu.postech.csed232.exercise6.control.MessageService;
import edu.postech.csed232.exercise6.model.Board;
import edu.postech.csed232.exercise6.model.Cell;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;

import static edu.postech.csed232.exercise6.base.GameConstants.*;

/**
 * The main GUI class for the X-Sudoku game. Responsible for displaying the board,
 * handling user interaction, and coordinating message updates.
 */
public class GameUI {
    @NotNull
    private final MessageService msgManager = new MessageService();
    @Nullable
    private Cell selectedCell;

    /**
     * Constructs and displays the game UI with the given board configuration.
     *
     * @param board the initial board configuration
     */
    public GameUI(@NotNull Board board) {
        var frame = createMainFrame();
        frame.add(buildBoardPanel(board), BorderLayout.CENTER);
        frame.add(buildControlPanel(), BorderLayout.SOUTH);
        frame.pack();
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }

    @NotNull
    private JFrame createMainFrame() {
        var frame = new JFrame("X-Sudoku");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLayout(new BorderLayout());
        return frame;
    }

    @NotNull
    private JPanel buildBoardPanel(@NotNull Board board) {
        var boardPanel = new JPanel(new GridLayout(GRID_SIZE, GRID_SIZE));
        var subgrids = createSubgridArray(boardPanel);
        fillBoardCells(board, subgrids);
        boardPanel.setBorder(BorderFactory.createEmptyBorder(UNIT_SIZE, UNIT_SIZE, UNIT_SIZE, UNIT_SIZE));
        return boardPanel;
    }

    @NotNull
    private JPanel @NotNull [][] createSubgridArray(@NotNull JPanel boardPanel) {
        var subgrids = new JPanel[GRID_SIZE][GRID_SIZE];
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                subgrids[i][j] = createSubgridPanel();
                boardPanel.add(subgrids[i][j]);
            }
        }
        return subgrids;
    }

    private void fillBoardCells(@NotNull Board board, @NotNull JPanel[][] subgrids) {
        for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                var cell = board.getCell(row + 1, col + 1);
                boolean isDiagonal = row == col || row + col == BOARD_SIZE - 1;
                var cellUI = createCellUI(cell, isDiagonal);
                subgrids[row / GRID_SIZE][col / GRID_SIZE].add(cellUI);
            }
        }
    }

    @NotNull
    private JPanel createSubgridPanel() {
        var panel = new JPanel(new GridLayout(GRID_SIZE, GRID_SIZE));
        panel.setBorder(BorderFactory.createLineBorder(Color.BLACK));
        return panel;
    }

    @NotNull
    private CellUI createCellUI(@NotNull Cell cell, boolean isDiagonal) {
        var cellUI = new CellUI(cell, isDiagonal);
        cellUI.setPreferredSize(new Dimension(CELL_SIZE, CELL_SIZE));
        cellUI.addFocusListener(new FocusAdapter() {
            @Override
            public void focusGained(FocusEvent e) {
                selectedCell = cell;
                msgManager.handleClearMessage();
            }
        });
        if (cell.getNumber().isEmpty()) {
            cellUI.setDocument(new CellInputDocument(cell, msgManager));
        }
        new CellActivationController(cellUI);
        return cellUI;
    }

    @NotNull
    private JPanel buildControlPanel() {
        var controlPanel = new JPanel(new BorderLayout());
        controlPanel.add(createMessageUI(), BorderLayout.CENTER);
        controlPanel.add(createHintButton(), BorderLayout.SOUTH);
        return controlPanel;
    }

    @NotNull
    private MessageUI createMessageUI() {
        var messageUI = new MessageUI();
        new MessageUIController(messageUI, msgManager);
        return messageUI;
    }

    @NotNull
    private JButton createHintButton() {
        var hintButton = new JButton("Hint");
        hintButton.addActionListener(_ -> handleHintRequest());
        return hintButton;
    }

    private void handleHintRequest() {
        if (selectedCell == null) {
            msgManager.handleHintNoSelection();
        } else if (selectedCell.getNumber().isPresent()) {
            msgManager.handleHintHasNumber();
        } else {
            msgManager.handleHintPossibilities(selectedCell);
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new GameUI(new Board(GameInstanceFactory.createGameInstance())));
    }
}
