package edu.postech.csed232.exercise5;

import java.util.Map;
import java.util.Set;

import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.jupiter.api.Test;

public class AExpEvaluatorTest{
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

    private final AExpEvaluator<Double> evaluator = new AExpEvaluator<>(num);

    private AExp<Double> c(double value){
        return new Const<>(value);
    }
    private Var<Double> x(int id){
        return new Var<>(id);
    }
    private AExp<Double> neg(AExp<Double> expr){
        return new Neg<>(expr);
    }
    private AExp<Double> add(AExp<Double> left, AExp<Double> right){
        return new Add<>(left, right);
    }
    private AExp<Double> mul(AExp<Double> left, AExp<Double> right){
        return new Mul<>(left, right);
    }

    @Test
    public void getVariables_add(){
        var expr=add(x(1), x(2));
        assertEquals(Set.of(1, 2), evaluator.getVariables(expr));
    }
    @Test
    public void evaluate_add(){
        var expr=add(x(1), c(2.0));
        assertEquals(5.0, evaluator.evaluate(expr, Map.of(1, 3.0)));
    }
    @Test
    public void toPrettyString_add(){
        var expr=add(x(1), c(2.0));
        assertEquals("(x1 + 2.0)", AExpEvaluator.toPrettyString(expr));
    }
    @Test
    public void expand_squaredAdd(){
        var expr=mul(add(x(1), c(2.0)), add(x(1), c(3.0)));
        var result=evaluator.expand(expr);
        assertEquals("6.0 + 5.0*x1 + x1^2", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void expand_zeroConst(){
        var expr=c(0.0);
        var result=evaluator.expand(expr);
        assertEquals("0", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void getVariables_const(){
        assertEquals(Set.of(), evaluator.getVariables(c(3.0)));
    }
    @Test
    public void getVariables_var(){
        assertEquals(Set.of(1), evaluator.getVariables(x(1)));
    }
    @Test
    public void getVariables_neg(){
        var expr=neg(add(x(1), x(2)));
        assertEquals(Set.of(1, 2), evaluator.getVariables(expr));
    }
    @Test
    public void getVariables_mul(){
        var expr=mul(add(x(1), c(2.0)), x(3));
        assertEquals(Set.of(1, 3), evaluator.getVariables(expr));
    }
    @Test
    public void evaluate_const(){
        assertEquals(4.0, evaluator.evaluate(c(4.0), Map.of()));
    }
    @Test
    public void evaluate_var(){
        assertEquals(7.0, evaluator.evaluate(x(1), Map.of(1, 7.0)));
    }
    @Test
    public void evaluate_neg(){
        var expr=neg(x(1));
        assertEquals(-3.0, evaluator.evaluate(expr, Map.of(1, 3.0)));
    }
    @Test
    public void evaluate_mul(){
        var expr=mul(add(x(1), c(2.0)), x(2));
        assertEquals(15.0, evaluator.evaluate(expr, Map.of(1, 3.0, 2, 3.0)));
    }
    @Test
    public void toPrettyString_const(){
        assertEquals("4.0", AExpEvaluator.toPrettyString(c(4.0)));
    }
    @Test
    public void toPrettyString_var(){
        assertEquals("x1", AExpEvaluator.toPrettyString(x(1)));
    }
    @Test
    public void toPrettyString_neg(){
        var expr=neg(add(x(1), c(2.0)));
        assertEquals("-((x1 + 2.0))", AExpEvaluator.toPrettyString(expr));
    }
    @Test
    public void toPrettyString_mul(){
        var expr=mul(add(x(1), c(2.0)), x(3));
        assertEquals("((x1 + 2.0) * x3)", AExpEvaluator.toPrettyString(expr));
    }
    @Test
    public void expand_const(){
        var result=evaluator.expand(c(5.0));
        assertEquals("5.0", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void expand_var(){
        var result=evaluator.expand(x(1));
        assertEquals("x1", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void expand_neg(){
        var result=evaluator.expand(neg(x(1)));
        assertEquals("-1.0*x1", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void expand_add(){
        var result=evaluator.expand(add(x(1), x(1)));
        assertEquals("2.0*x1", PolynomialEvaluator.toPrettyString(result));
    }
    @Test
    public void expand_mul(){
        var result=evaluator.expand(mul(add(x(1), c(2.0)), x(3)));
        assertEquals("x1*x3 + 2.0*x3", PolynomialEvaluator.toPrettyString(result));
    }
}