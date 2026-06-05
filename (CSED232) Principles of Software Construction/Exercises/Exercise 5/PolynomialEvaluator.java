package edu.postech.csed232.exercise5;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

import org.jetbrains.annotations.NotNull;

import static edu.postech.csed232.exercise5.Polynomial.sum;

public final class PolynomialEvaluator<T extends Number>{
    private final @NotNull Arithmetic<T> num;

    public PolynomialEvaluator(@NotNull Arithmetic<T> num){
        this.num=num;
    }

    public @NotNull Set<Integer> getVariables(@NotNull Polynomial<T> poly){
        return switch(poly){
            case PolyZero<T> ignored -> Set.of();
            case Monomial<T> mono -> getVariables(mono);
            case PolySum<T> p -> p.terms().keySet().stream().flatMap(term -> getVariables(term).stream()).collect(Collectors.toSet());
        };
    }
    public @NotNull T evaluate(@NotNull Polynomial<T> poly, @NotNull Map<Integer, T> env){
        return switch(poly){
            case PolyZero<T> ignored -> num.zero();
            case Monomial<T> mono -> evaluate(mono, env);
            case PolySum<T> p -> {
                T result=num.zero();
                for(var entry: p.terms().entrySet()){
                    T coefficient=entry.getValue();
                    T termValue=evaluate(entry.getKey(), env);
                    result=num.add(result, num.mul(coefficient, termValue));
                }
                yield result;
            }
        };
    }
    public static <T extends Number> @NotNull String toPrettyString(@NotNull Polynomial<T> poly){
        return switch(poly){
            case PolyZero<T> ignored -> "0";
            case Monomial<T> mono -> toPrettyString(mono);
            case PolySum<T> p -> p.terms().entrySet().stream().sorted(Map.Entry.comparingByKey()).map(entry -> toPrettyString(entry.getKey(), entry.getValue())).collect(Collectors.joining(" + "));
        };
    }
    public @NotNull Polynomial<T> negatePoly(@NotNull Polynomial<T> poly){
        return switch(poly){
            case PolyZero<T> zero -> zero;
            case Monomial<T> mono -> sum(Map.of(mono, num.neg(num.one())));
            case PolySum<T> p -> {
                Map<Monomial<T>, T> negTerms = new HashMap<>();
                for(var entry: p.terms().entrySet()){
                    var term=entry.getKey();
                    T coefficient=entry.getValue();
                    negTerms.put(term, num.neg(coefficient));
                }
                yield sum(negTerms);
            }
        };
    }
    public @NotNull Polynomial<T> polySum(@NotNull Polynomial<T> p, @NotNull Polynomial<T> q){
        Map<Monomial<T>, T> terms=toTerms(p);
        for(var entry: toTerms(q).entrySet()){
            addTerm(terms, entry.getKey(), entry.getValue());
        }
        return makePolynomial(terms);
    }
    public @NotNull Polynomial<T> polyProduct(@NotNull Polynomial<T> p, @NotNull Polynomial<T> q){
        Map<Monomial<T>, T> terms = new HashMap<>();
        for(var pEntry: toTerms(p).entrySet()){
            for(var qEntry: toTerms(q).entrySet()){
                Monomial<T> term=multiply(pEntry.getKey(), qEntry.getKey());
                T coefficient=num.mul(pEntry.getValue(), qEntry.getValue());
                addTerm(terms, term, coefficient);
            }
        }
        return makePolynomial(terms);
    }
    private @NotNull Map<Monomial<T>, T> toTerms(@NotNull Polynomial<T> poly){
        return switch(poly){
            case PolyZero<T> ignored -> new HashMap<>();
            case Monomial<T> mono -> new HashMap<>(Map.of(mono, num.one()));
            case PolySum<T> p -> new HashMap<>(p.terms());
        };
    }
    private void addTerm(@NotNull Map<Monomial<T>, T> terms, @NotNull Monomial<T> term, @NotNull T coefficient){
        if(isZero(coefficient)){
            return;
        }
        T newCoefficient=terms.containsKey(term)?num.add(terms.get(term), coefficient):coefficient;
        if(isZero(newCoefficient)){
            terms.remove(term);
        } else{
            terms.put(term, newCoefficient);
        }
    }
    private @NotNull Polynomial<T> makePolynomial(@NotNull Map<Monomial<T>, T> terms){
        Map<Monomial<T>, T> nonZeroTerms = new HashMap<>();
        for(var entry: terms.entrySet()){
            if(!isZero(entry.getValue())){
                nonZeroTerms.put(entry.getKey(), entry.getValue());
            }
        }
        if(nonZeroTerms.isEmpty()){
            return Polynomial.zero();
        }
        if(nonZeroTerms.size()==1){
            var entry=nonZeroTerms.entrySet().iterator().next();
            if(isOne(entry.getValue())){
                return entry.getKey();
            }
        }
        return sum(nonZeroTerms);
    }
    private @NotNull Monomial<T> multiply(@NotNull Monomial<T> p, @NotNull Monomial<T> q){
        return switch(p){
            case PolyOne<T> ignored -> q;
            case PolyTerm<T> pTerm -> switch(q){
                case PolyOne<T> ignored -> p;
                case PolyTerm<T> qTerm -> {
                    Map<Integer, Integer> variables = new HashMap<>();
                    for(var entry: pTerm.vars().entrySet()){
                        variables.put(entry.getKey().id(), entry.getValue());
                    }
                    for(var entry: qTerm.vars().entrySet()){
                        variables.merge(entry.getKey().id(), entry.getValue(), Integer::sum);
                    }
                    yield Polynomial.term(variables);
                }
            };
        };
    }
    private @NotNull Set<Integer> getVariables(@NotNull Monomial<T> mono){
        return switch(mono){
            case PolyOne<T> ignored -> Set.of();
            case PolyTerm<T> term -> term.vars().keySet().stream().map(Var::id).collect(Collectors.toSet());
        };
    }
    private @NotNull T evaluate(@NotNull Monomial<T> mono, @NotNull Map<Integer, T> env){
        return switch(mono){
            case PolyOne<T> ignored -> num.one();
            case PolyTerm<T> term -> {
                T result=num.one();
                for(var entry: term.vars().entrySet()){
                    int id=entry.getKey().id();
                    if(!env.containsKey(id)){
                        throw new IllegalArgumentException("missing variable: x"+id);
                    }
                    for(int i=0; i<=entry.getValue()-1; ++i){
                        result=num.mul(result, env.get(id));
                    }
                }
                yield result;
            }
        };
    }
    private boolean isZero(@NotNull T value){
        return value.equals(num.zero());
    }
    private boolean isOne(@NotNull T value){
        return value.equals(num.one());
    }
    private static <T extends Number> @NotNull String toPrettyString(@NotNull Monomial<T> mono){
        return switch(mono){
            case PolyOne<T> ignored -> "1";
            case PolyTerm<T> term -> term.vars().entrySet().stream().sorted(Map.Entry.comparingByKey()).map(entry -> {
                        int exponent=entry.getValue();
                        String variable="x"+entry.getKey().id();
                        if(exponent==1){
                            return variable;
                        } else{
                            return variable+"^"+exponent;
                        }
                    }).collect(Collectors.joining("*"));
        };
    }
    private static <T extends Number> @NotNull String toPrettyString(@NotNull Monomial<T> mono, @NotNull T coefficient){
        if(mono instanceof PolyOne<T>){
            return coefficient.toString();
        }
        if(coefficient.doubleValue()==1.0){
            return toPrettyString(mono);
        } else{
            return coefficient+"*"+toPrettyString(mono);
        }
    }
}