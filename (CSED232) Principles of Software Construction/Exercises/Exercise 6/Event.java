package edu.postech.csed232.exercise6.event;

/**
 * A marker interface for different types of events in the Sudoku game.
 * Each event represents a change that observers can react to.
 */
public sealed interface Event permits NumberEvent, ActivationEvent, MessageEvent {
}

