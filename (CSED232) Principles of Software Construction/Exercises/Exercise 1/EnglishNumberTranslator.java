package edu.postech.csed232.exercise1;

public class EnglishNumberTranslator{
    private static final String[] belowTwenty={
        "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
    };
    private static final String[] tens={
        "", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"
    };
    private static final String[] thousands={
        "", "thousand", "million", "billion", "trillion", "quadrillion", "quintillion"
    };

    private String convertBelowThousand(int number){
        StringBuilder sb = new StringBuilder();
        if(number>=100){
            sb.append(belowTwenty[number/100]).append(" hundred");
            number%=100;
            if(number==0){
                return sb.toString();
            } else{
                sb.append(" ");
            }
        }
        if(number>=20){
            sb.append(tens[number/10]);
            number%=10;
            if(number==0){
                return sb.toString();
            } else{
                sb.append(" ").append(belowTwenty[number]);
            }
        } else{
            sb.append(belowTwenty[number]);
        }
        return sb.toString();
    }

    /**
     * @param number a number
     * @return a string of words
     * @throws IllegalArgumentException if the number is not in the range
     */
    public String toWords(long number){
        if(number<0L){
            throw new IllegalArgumentException();
        } else if(number==0L){
            return "zero";
        }
        StringBuilder sb = new StringBuilder();
        int unit=0;
        while(number>0L){
            int chunkNumber=(int)(number%1000L);
            if(chunkNumber!=0){
                String chunkEnglish=convertBelowThousand(chunkNumber);
                if(unit>=1){
                    chunkEnglish+=" "+thousands[unit];
                }
                if(sb.length()==0){
                    sb.insert(0, chunkEnglish);
                } else{
                    sb.insert(0, chunkEnglish+" ");
                }
            }
            number/=1000;
            ++unit;
        }
        return sb.toString();
    }

    public static void main(String[] args){
        long input=10000001201L;
        EnglishNumberTranslator translator = new EnglishNumberTranslator();

        System.out.println(input+" -> "+translator.toWords(input));
    }
}