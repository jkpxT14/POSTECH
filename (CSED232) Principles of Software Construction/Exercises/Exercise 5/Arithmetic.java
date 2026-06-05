package edu.postech.csed232.exercise5;

import org.jetbrains.annotations.NotNull;

public interface Arithmetic<T extends Number>{
    @NotNull T add(@NotNull T a, @NotNull T b);
    @NotNull T mul(@NotNull T a, @NotNull T b);
    @NotNull T neg(@NotNull T a);
    @NotNull T zero();
    @NotNull T one();
}