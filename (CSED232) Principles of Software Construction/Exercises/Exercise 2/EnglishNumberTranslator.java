package edu.postech.csed232.exercise2;

import java.util.List;
import java.util.Locale;

public class EnglishNumberTranslator extends AbstractNumberTranslator{
    public static final String[] belowTwenty={
        "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
    };
    public static final String[] tens={
        "", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"
    };
    public static final String[] thousands={
        "", " thousand", " million", " billion", " trillion", " quadrillion", " quintillion"
    };

    EnglishNumberTranslator(){
        super(Locale.ENGLISH);
    }

    @Override
    protected int getChunkSize(){
        return 3;
    }

    @Override
    protected String zeroWord(){
        return "zero";
    }

    private String twoDigitToWords(int number){
        if(number<20){
            return belowTwenty[number];
        }
        int ten=number/10, one=number%10;
        if(one==0){
            return tens[ten];
        }
        return tens[ten]+" "+belowTwenty[one];
    }

    @Override
    protected String chunkToWords(int chunk){
        StringBuilder builder = new StringBuilder();

        int hundred=chunk/100, remainder=chunk%100;
        if(hundred>0){
            builder.append(belowTwenty[hundred]).append(" hundred");
        }
        if(remainder>0){
            if(builder.length()>0){
                builder.append(" ");
            }
            builder.append(twoDigitToWords(remainder));
        }
        return builder.toString();
    }

    @Override
    protected String chunkUnit(int index){
        return thousands[index];
    }

    @Override
    protected String joinChunks(List<String> translatedChunks){
        return String.join(" ", translatedChunks);
    }

    public static void main(String[] args){
        long input=10000001201L;
        EnglishNumberTranslator translator = new EnglishNumberTranslator();

        System.out.println(input+" -> "+translator.toWords(input));
    }
}