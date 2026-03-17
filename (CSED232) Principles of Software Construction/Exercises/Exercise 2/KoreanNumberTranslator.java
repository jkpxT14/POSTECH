package edu.postech.csed232.exercise2;

import java.util.List;
import java.util.Locale;

public class KoreanNumberTranslator extends AbstractNumberTranslator{
    private static final String[] digits={
        "", "일", "이", "삼", "사", "오", "육", "칠", "팔", "구"
    };
    private static final int[] divisors={
        1000, 100, 10, 1
    };
    private static final String[] smallUnits={
        "천", "백", "십", ""
    };
    private static final String[] largeUnits={
        "", "만", "억", "조", "경"
    };

    KoreanNumberTranslator(){
        super(Locale.KOREAN);
    }

    @Override
    protected int getChunkSize(){
        return 4;
    }

    @Override
    protected String zeroWord(){
        return "영";
    }

    @Override
    protected String chunkToWords(int chunk){
        StringBuilder builder = new StringBuilder();

        for(int i=0; i<=3; ++i){
            int digit=chunk/divisors[i]; chunk%=divisors[i];
            if(digit==0){
                continue;
            }
            if(i==3){
                builder.append(digits[digit]);
            } else if(i==2 && digit==1){
                builder.append("십");
            } else{
                builder.append(digits[digit]).append(smallUnits[i]);
            }
        }
        return builder.toString();
    }

    @Override
    protected String chunkUnit(int index){
        return largeUnits[index];
    }

    @Override
    protected String joinChunks(List<String> translatedChunks){
        return String.join("", translatedChunks);
    }

    public static void main(String[] args){
        long input = 10000001201L;
        KoreanNumberTranslator translator = new KoreanNumberTranslator();

        System.out.println(input+" -> "+translator.toWords(input));
    }
}