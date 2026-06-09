package edu.postech.csed232.exercise6.base;

import org.jetbrains.annotations.NotNull;

import javax.swing.BorderFactory;
import javax.swing.border.Border;
import java.awt.Color;
import java.awt.Font;

public final class GameConstants {

    // Logic Constants
    public static final int GRID_SIZE = 3;
    public static final int BOARD_SIZE = GRID_SIZE * GRID_SIZE;
    public static final int MIN_CELL_NUMBER = 1;
    public static final int MAX_CELL_NUMBER = 9;

    // UI Constants
    public static final int CELL_SIZE = 60;
    public static final int UNIT_SIZE = 10;

    @NotNull
    public static final Font CELL_FONT = new Font("Times", Font.BOLD, 30);
    @NotNull
    public static final Font MESSAGE_FONT = new Font("SansSerif", Font.PLAIN, 14);

    @NotNull
    public static final Border ACTIVE_BORDER = BorderFactory.createLineBorder(Color.BLACK);
    @NotNull
    public static final Border INACTIVE_BORDER = BorderFactory.createLineBorder(Color.RED);


    private GameConstants() {
        throw new AssertionError("Cannot instantiate GameConstants.");
    }
}