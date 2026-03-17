package edu.postech.csed232.exercise2;

import java.util.Arrays;
import java.util.stream.Collectors;

public class HopingRabbitsGame{
    private Rabbit[] rabbits;

    /**
     * @param n the number of rabbits on each team
     */
    HopingRabbitsGame(int n){
        rabbits = new Rabbit[2*n+1];
        for(int i=0; i<=n-1; ++i){
            rabbits[i]=Rabbit.X;
        }
        rabbits[n]=Rabbit.EMPTY;
        for(int i=n+1; i<=2*n; ++i){
            rabbits[i]=Rabbit.O;
        }
    }

    private int findEmptyIndex(){
        for(int i=0; i<=rabbits.length-1; ++i){
            if(rabbits[i]==Rabbit.EMPTY){
                return i;
            }
        }
        return -1;
    }
    private int findMovableRabbitIndex(Rabbit rabbit, int empty){
        if(rabbit==Rabbit.X){
            if(empty-1>=0 && rabbits[empty-1]==Rabbit.X){
                return empty-1;
            }
            if(empty-2>=0 && rabbits[empty-2]==Rabbit.X && rabbits[empty-1]==Rabbit.O){
                return empty-2;
            }
            return -1;
        } else if(rabbit==Rabbit.O){
            if(empty+1<=rabbits.length-1 && rabbits[empty+1]==Rabbit.O){
                return empty+1;
            }
            if(empty+2<=rabbits.length-1 && rabbits[empty+2]==Rabbit.O && rabbits[empty+1]==Rabbit.X){
                return empty+2;
            }
            return -1;
        }
        return -1;
    }

    /**
     * @param rabbit a rabbit
     * @return true if the rabbit can move, false otherwise
     */
    boolean move(Rabbit rabbit){
        if(rabbit==Rabbit.EMPTY){
            return false;
        }
        int empty=findEmptyIndex();
        int from=findMovableRabbitIndex(rabbit, empty);
        if(from==-1){
            return false;
        }
        rabbits[empty]=rabbit;
        rabbits[from]=Rabbit.EMPTY;
        return true;
    }

    /**
     * @return true if the game is over, false otherwise
     */
    boolean isGoal(){
        int n=rabbits.length/2;
        for(int i=0; i<=n-1; ++i){
            if(rabbits[i]!=Rabbit.O){
                return false;
            }
        }
        if(rabbits[n]!=Rabbit.EMPTY){
            return false;
        }
        for(int i=n+1; i<=2*n; ++i){
            if(rabbits[i]!=Rabbit.X){
                return false;
            }
        }
        return true;
    }

    /**
     * @return true if the game is stuck, false otherwise
     */
    boolean isStuck(){
        int empty=findEmptyIndex();
        return (findMovableRabbitIndex(Rabbit.X, empty)==-1 && findMovableRabbitIndex(Rabbit.O, empty)==-1);
    }

    /**
     * @return a string of length 2N + 1 consisting of x, o, and _.
     */
    @Override
    public String toString(){
        return Arrays.stream(rabbits).map(rabbit -> switch (rabbit) {
            case X -> "x";
            case O -> "o";
            case EMPTY -> "_";
        }).collect(Collectors.joining());
    }

    public static void main(String[] args){
        var game = new HopingRabbitsGame(3);

        System.out.println(game);
        game.move(Rabbit.X);
        System.out.println(game);
        game.move(Rabbit.O);
        System.out.println(game);
    }
}