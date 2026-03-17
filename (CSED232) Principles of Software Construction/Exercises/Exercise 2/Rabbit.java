package edu.postech.csed232.exercise2;

public enum Rabbit{
    X, O, EMPTY;

    public Rabbit other(){
        return switch (this) {
            case X -> O;
            case O -> X;
            default -> EMPTY;
        };
    }
}