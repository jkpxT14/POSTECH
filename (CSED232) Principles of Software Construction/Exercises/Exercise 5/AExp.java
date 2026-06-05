package edu.postech.csed232.exercise5;

import org.jetbrains.annotations.NotNull;

public sealed interface AExp<T extends Number> permits Const, Var, Add, Mul, Neg {
    static <T extends Number> @NotNull AExp<T> c(@NotNull T value){
        return new Const<>(value);
    }
    static <T extends Number> @NotNull AExp<T> x(int id){
        return new Var<>(id);
    }
    static <T extends Number> @NotNull AExp<T> add(@NotNull AExp<T> left, @NotNull AExp<T> right){
        return new Add<>(left, right);
    }
    static <T extends Number> @NotNull AExp<T> mul(@NotNull AExp<T> left, @NotNull AExp<T> right){
        return new Mul<>(left, right);
    }
    static <T extends Number> @NotNull AExp<T> neg(@NotNull AExp<T> expr){
        return new Neg<>(expr);
    }
    default @NotNull String toPrettyString(){
        return AExpEvaluator.toPrettyString(this);
    }
}

record Const<T extends Number>(@NotNull T value) implements AExp<T> {}
record Var<T extends Number>(int id) implements AExp<T>, Comparable<Var<T>> {
    public Var{
        if(id<0){
            throw new IllegalArgumentException("variable id cannot be negative");
        }
    }
    @Override
    public int compareTo(@NotNull Var<T> o){
        return Integer.compare(this.id, o.id());
    }
}
record Neg<T extends Number>(@NotNull AExp<T> expr) implements AExp<T> {}
record Add<T extends Number>(@NotNull AExp<T> left, @NotNull AExp<T> right) implements AExp<T> {}
record Mul<T extends Number>(@NotNull AExp<T> left, @NotNull AExp<T> right) implements AExp<T> {}