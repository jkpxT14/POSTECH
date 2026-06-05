package edu.postech.csed232.exercise5;

import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import org.jetbrains.annotations.NotNull;

public final class AExpEvaluator<T extends Number>{
    private final @NotNull Arithmetic<T> num;

    public AExpEvaluator(@NotNull Arithmetic<T> num){
        this.num=num;
    }

    public @NotNull Set<Integer> getVariables(@NotNull AExp<T> expr){
        return switch(expr){
            case Const<T> ignored -> Set.of();
            case Var<T> v -> Set.of(v.id());
            case Neg<T> n -> getVariables(n.expr());
            case Add<T> a -> {
                Set<Integer> variables = new HashSet<>(getVariables(a.left()));
                variables.addAll(getVariables(a.right()));
                yield variables;
            }
            case Mul<T> m -> {
                Set<Integer> variables = new HashSet<>(getVariables(m.left()));
                variables.addAll(getVariables(m.right()));
                yield variables;
            }
        };
    }
    public @NotNull T evaluate(@NotNull AExp<T> expr, @NotNull Map<Integer, T> env){
        return switch(expr){
            case Const<T> c -> c.value();
            case Var<T> v -> {
                if(!env.containsKey(v.id())){
                    throw new IllegalArgumentException("missing variable: x"+v.id());
                }
                yield env.get(v.id());
            }
            case Neg<T> n -> num.neg(evaluate(n.expr(), env));
            case Add<T> a -> num.add(evaluate(a.left(), env), evaluate(a.right(), env));
            case Mul<T> m -> num.mul(evaluate(m.left(), env), evaluate(m.right(), env));
        };
    }
    public @NotNull Polynomial<T> expand(@NotNull AExp<T> expr){
        var pev = new PolynomialEvaluator<>(num);
        return switch(expr){
            case Const<T> c -> {
                if(c.value().equals(num.zero())){
                    yield Polynomial.zero();
                }
                yield Polynomial.sum(Map.of(Polynomial.one(), c.value()));
            }
            case Var<T> v -> Polynomial.term(Map.of(v.id(), 1));
            case Neg<T> n -> pev.negatePoly(expand(n.expr()));
            case Add<T> a -> pev.polySum(expand(a.left()), expand(a.right()));
            case Mul<T> m -> pev.polyProduct(expand(m.left()), expand(m.right()));
        };
    }
    public static <T extends Number> @NotNull String toPrettyString(@NotNull AExp<T> e){
        return switch(e){
            case Const<T> c -> c.value().toString();
            case Var<T> v -> "x"+v.id();
            case Neg<T> n -> "-("+toPrettyString(n.expr())+")";
            case Add<T> a -> "("+toPrettyString(a.left())+" + "+toPrettyString(a.right())+")";
            case Mul<T> m -> "("+toPrettyString(m.left())+" * "+toPrettyString(m.right())+")";
        };
    }
}