package edu.postech.csed232.exercise2;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

public abstract class AbstractNumberTranslator implements NumberTranslator{
    private final Locale locale;

    /**
     * @param locale locale associated with this translator
     */
    AbstractNumberTranslator(Locale locale){
        this.locale=locale;
    }

    /**
     * @return translator locale
     */
    @Override
    public Locale getLocale(){
        return locale;
    }

    /**
     * @param number input number
     * @return translated words for {@code number}
     * @throws IllegalArgumentException if {@code number < 0}
     * @throws IllegalStateException    if {@code getChunkSize() <= 0}
     */
    @Override
    final public String toWords(long number){
        if(number<0L){
            throw new IllegalArgumentException();
        }
        if(number==0L){
            return zeroWord();
        }
        int chunkSize=getChunkSize();
        if(chunkSize<=0){
            throw new IllegalStateException();
        }
        long base=1L;
        for(int i=0; i<=chunkSize-1; ++i){
            base*=10L;
        }
        List<String> translatedChunks = new ArrayList<>();
        int chunkIndex=0;
        while(number>0){
            int chunk=(int)(number%base); number/=base;
            if(chunk!=0){
                translatedChunks.add(0, chunkToWords(chunk)+chunkUnit(chunkIndex));
            }
            ++chunkIndex;
        }
        return joinChunks(translatedChunks);
    }

    /**
     * @return positive chunk size
     */
    protected abstract int getChunkSize();

    /**
     * @return zero word for this locale
     */
    protected abstract String zeroWord();

    /**
     * @param chunk one chunk value
     * @return words for that chunk (without higher-unit text for other chunks)
     */
    protected abstract String chunkToWords(int chunk);

    /**
     * @param index non-negative chunk position from low to high
     * @return unit label for the given chunk position
     */
    protected abstract String chunkUnit(int index);

    /**
     * @param translatedChunks translated chunk strings in high-to-low order
     * @return final joined translation
     */
    protected abstract String joinChunks(List<String> translatedChunks);
}