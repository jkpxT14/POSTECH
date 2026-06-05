package edu.postech.csed232.exercise5;

import java.util.Map;
import java.util.stream.Collectors;

import org.jetbrains.annotations.NotNull;

public sealed interface Polynomial<T extends Number> permits PolySum, Monomial, PolyZero {
    static <T extends Number> @NotNull Polynomial<T> zero(){
        return new PolyZero<>();
    }
    static <T extends Number> @NotNull Monomial<T> one(){
        return new PolyOne<>();
    }
    static <T extends Number> @NotNull Monomial<T> term(@NotNull Map<@NotNull Integer, @NotNull Integer> vars){
        if(vars.isEmpty()){
            return one();
        } else{
            return new PolyTerm<>(vars.entrySet().stream().collect(Collectors.toMap(entry -> new Var<>(entry.getKey()), Map.Entry::getValue)));
        }
    }
    static <T extends Number> @NotNull Polynomial<T> sum(@NotNull Map<@NotNull Monomial<T>, @NotNull T> terms){
        return new PolySum<>(terms);
    }
    default @NotNull String toPrettyString(){
        return PolynomialEvaluator.toPrettyString(this);
    }
}

record PolySum<T extends Number>(@NotNull Map<@NotNull Monomial<T>, @NotNull T> terms) implements Polynomial<T> {
    public PolySum{
        if(terms.isEmpty()){
            throw new IllegalArgumentException("empty polynomial");
        }
        terms=Map.copyOf(terms);
    }
}
record PolyZero<T extends Number>() implements Polynomial<T> {}