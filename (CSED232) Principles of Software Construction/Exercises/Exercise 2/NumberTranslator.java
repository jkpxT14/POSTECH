package edu.postech.csed232.exercise2;

import java.util.Locale;

public interface NumberTranslator{
    /**
     * @param number a number
     * @return a string of words
     * @throws IllegalArgumentException if the number is not in the range
     */
    String toWords(long number);

    /**
     * @return a locale
     */
    Locale getLocale();
}