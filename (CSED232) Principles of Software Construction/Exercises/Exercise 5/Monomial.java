package edu.postech.csed232.exercise5;

import java.util.Map;

import org.jetbrains.annotations.NotNull;

public sealed interface Monomial<T extends Number> extends Polynomial<T>, Comparable<Monomial<T>> permits PolyTerm, PolyOne {
    @Override
    default int compareTo(@NotNull Monomial<T> other){
        if(this instanceof PolyTerm<T>(var t1) && other instanceof PolyTerm<T>(var t2)){
            var entries1=t1.entrySet().stream().sorted(Map.Entry.comparingByKey()).toList();
            var entries2=t2.entrySet().stream().sorted(Map.Entry.comparingByKey()).toList();
            var cmp=Map.Entry.<Var<T>, Integer>comparingByKey().thenComparing(Map.Entry.comparingByValue());
            for(int i=0; i<=Math.min(entries1.size(), entries2.size())-1; ++i){
                int c=cmp.compare(entries1.get(i), entries2.get(i));
                if(c!=0){
                    return c;
                }
            }
            return Integer.compare(entries1.size(), entries2.size());
        } else{
            return (this instanceof PolyOne)?(other instanceof PolyOne?0:-1):1;
        }
    }
}

record PolyOne<T extends Number>() implements Monomial<T> {}
record PolyTerm<T extends Number>(@NotNull Map<@NotNull Var<T>, @NotNull Integer> vars) implements Monomial<T> {
    public PolyTerm{
        if(vars.isEmpty()){
            throw new IllegalArgumentException("empty monomial");
        }
        for(var e: vars.entrySet()){
            if(e.getValue()<=0){
                throw new IllegalArgumentException("exponent must be positive");
            }
        }
        vars=Map.copyOf(vars);
    }
}