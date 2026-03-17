package edu.postech.csed232.exercise1;

public class ExpressionAddOperator{
    private boolean DFS(String s, int index, int value, int target){
        if(index==s.length()){
            return value==target;
        }
        int delta_value=0;
        for(int i=index; i<=s.length()-1; ++i){
            delta_value=delta_value*10+(s.charAt(i)-'0');
            if(index==0){
                if(DFS(s, i+1, delta_value, target)){
                    return true;
                }
            } else{
                if(DFS(s, i+1, value+delta_value, target) || DFS(s, i+1, value-delta_value, target)){
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * @param number the original number
     * @param target the desired result
     * @return true if target can be formed, false otherwise
     * @throws IllegalArgumentException if number or target is negative
     */
    public boolean solve(int number, int target){
        if(number<0 || target<0){
            throw new IllegalArgumentException();
        }
        return DFS(Integer.toString(number), 0, 0, target);
    }

    public static void main(String[] args){
        int number=1234, target=20;
        ExpressionAddOperator checker = new ExpressionAddOperator();

        System.out.println("Can "+number+" form "+target+"? "+checker.solve(number, target));
    }
}