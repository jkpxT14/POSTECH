package edu.postech.csed232.exercise5;

import java.util.Map;
import java.util.Set;

import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.jupiter.api.Test;

public class PolynomialEvaluatorTest{
    private final Arithmetic<Double> num = new Arithmetic<>(){
        @Override
        public Double zero(){
            return 0.0;
        }
        @Override
        public Double one(){
            return 1.0;
        }
        @Override
        public Double add(Double x, Double y){
            return x+y;
        }
        @Override
        public Double mul(Double x, Double y){
            return x*y;
        }
        @Override
        public Double neg(Double x){
            return -x;
        }
    };

    private final PolynomialEvaluator<Double> evaluator = new PolynomialEvaluator<>(num);

    private Monomial<Double> one(){
        return Polynomial.one();
    }
    private Monomial<Double> term(int variable, int exponent){
        return Polynomial.term(Map.of(variable, exponent));
    }

    @Test
    public void getVariables(){
        var poly=Polynomial.sum(Map.of(
                term(1, 1), 2.0,
                term(2, 3), 3.0,
                one(), -5.0
        ));
        assertEquals(Set.of(1, 2), evaluator.getVariables(poly));
    }
    @Test
    public void evaluate_product(){
        var poly=Polynomial.<Double>term(Map.of(1, 2, 2, 1));
        assertEquals(12.0, evaluator.evaluate(poly, Map.of(1, 2.0, 2, 3.0)));
    }
    @Test
    public void evaluate_sum(){
        var poly=Polynomial.sum(Map.of(
                term(1, 2), 3.0,
                term(2, 1), 4.0,
                one(), -5.0
        ));
        assertEquals(23.0, evaluator.evaluate(poly, Map.of(1, 2.0, 2, 4.0)));
    }
    @Test
    public void negatePoly(){
        var poly=Polynomial.sum(Map.of(
                term(1, 1), 2.0,
                term(2, 1), -3.0
        ));
        var result=evaluator.negatePoly(poly);
        assertEquals("-2.0*x1 + 3.0*x2", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void polySum(){
        var p=Polynomial.sum(Map.of(
                term(1, 1), 2.0,
                one(), 3.0
        ));
        var q=Polynomial.sum(Map.of(
                term(1, 1), 4.0,
                term(2, 1), 5.0
        ));
        var result=evaluator.polySum(p, q);
        assertEquals("3.0 + 6.0*x1 + 5.0*x2", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void polyProduct(){
        var p=Polynomial.sum(Map.of(
                term(1, 1), 1.0,
                one(), 2.0
        ));
        var q=Polynomial.sum(Map.of(
                term(1, 1), 1.0,
                one(), 3.0
        ));
        var result=evaluator.polyProduct(p, q);
        assertEquals("6.0 + 5.0*x1 + x1^2", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void toPrettyString(){
        var poly=Polynomial.sum(Map.of(
                term(1, 1), 3.0,
                term(2, 1), 4.0,
                one(), -5.0
        ));
        assertEquals("-5.0 + 3.0*x1 + 4.0*x2", PolynomialEvaluator.toPrettyString(poly));
    }
    @Test
    public void getVariables_zero(){
        assertEquals(Set.of(), evaluator.getVariables(Polynomial.zero()));
    }
    @Test
    public void getVariables_one(){
        assertEquals(Set.of(), evaluator.getVariables(Polynomial.one()));
    }
    @Test
    public void getVariables_term(){
        var poly=Polynomial.<Double>term(Map.of(1, 2, 3, 1));
        assertEquals(Set.of(1, 3), evaluator.getVariables(poly));
    }
    @Test
    public void evaluate_zero(){
        assertEquals(0.0, evaluator.evaluate(Polynomial.zero(), Map.of()));
    }
    @Test
    public void evaluate_one(){
        assertEquals(1.0, evaluator.evaluate(Polynomial.one(), Map.of()));
    }
    @Test
    public void toPrettyString_zero(){
        assertEquals("0", PolynomialEvaluator.toPrettyString(Polynomial.zero()));
    }
    @Test
    public void toPrettyString_one(){
        assertEquals("1", PolynomialEvaluator.toPrettyString(Polynomial.one()));
    }
    @Test
    public void toPrettyString_term(){
        var poly=Polynomial.<Double>term(Map.of(1, 1, 2, 3));
        assertEquals("x1*x2^3", PolynomialEvaluator.toPrettyString(poly));
    }
    @Test
    public void negatePoly_zero(){
        var result=evaluator.negatePoly(Polynomial.zero());
        assertEquals("0", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void negatePoly_monomial(){
        var result=evaluator.negatePoly(term(1, 1));
        assertEquals("-1.0*x1", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void polySum_zeroAndTerm(){
        var result=evaluator.polySum(Polynomial.zero(), term(1, 1));
        assertEquals("x1", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void polySum_combinesLikeTerms(){
        var p=Polynomial.sum(Map.of(term(1, 1), 2.0));
        var q=Polynomial.sum(Map.of(term(1, 1), 3.0));
        var result=evaluator.polySum(p, q);
        assertEquals("5.0*x1", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void polySum_cancelsToZero(){
        var p=Polynomial.sum(Map.of(term(1, 1), 2.0));
        var q=Polynomial.sum(Map.of(term(1, 1), -2.0));
        var result=evaluator.polySum(p, q);
        assertEquals("0", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void polySum_keepsDifferentTerms(){
        var p=Polynomial.sum(Map.of(term(1, 1), 2.0));
        var q=Polynomial.sum(Map.of(term(2, 1), 3.0));
        var result=evaluator.polySum(p, q);
        assertEquals("2.0*x1 + 3.0*x2", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void polyProduct_zero(){
        var result=evaluator.polyProduct(Polynomial.zero(), term(1, 1));
        assertEquals("0", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void polyProduct_oneAndTerm(){
        var result=evaluator.polyProduct(Polynomial.one(), term(1, 2));
        assertEquals("x1^2", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void polyProduct_termAndOne(){
        var result=evaluator.polyProduct(term(1, 2), Polynomial.one());
        assertEquals("x1^2", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void polyProduct_terms(){
        var result=evaluator.polyProduct(term(1, 2), term(1, 1));
        assertEquals("x1^3", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void polyProduct_combinesLikeTerms(){
        var p=Polynomial.sum(Map.of(
                term(1, 1), 1.0,
                one(), 1.0
        ));
        var q=Polynomial.sum(Map.of(
                term(1, 1), 1.0,
                one(), -1.0
        ));
        var result=evaluator.polyProduct(p, q);
        assertEquals("-1.0 + x1^2", PolynomialEvaluator.toPrettyString(result));
    }
}