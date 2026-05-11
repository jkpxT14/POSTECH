package edu.postech.csed232.exercise1;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

class EnglishNumberTranslatorTest {

    final EnglishNumberTranslator translator = new EnglishNumberTranslator();

    @DisplayName("EnglishNumberTranslator - various numbers to words")
    @ParameterizedTest(name = "{index} => number={0}, expected={1}")
    @CsvSource({
            "0, zero",
            "12, twelve",
            "716, seven hundred sixteen",
            "1234, one thousand two hundred thirty four",
            "91817, ninety one thousand eight hundred seventeen",
            "1000000, one million",
            "100200300, one hundred million two hundred thousand three hundred",
            "1234567890, one billion two hundred thirty four million five hundred sixty seven thousand eight hundred ninety",
            "10000001201, ten billion one thousand two hundred one",
            "31057642375, thirty one billion fifty seven million six hundred forty two thousand three hundred seventy five",
            "53987654321, fifty three billion nine hundred eighty seven million six hundred fifty four thousand three hundred twenty one",
            "112358132134, one hundred twelve billion three hundred fifty eight million one hundred thirty two thousand one hundred thirty four",
            "567890123456, five hundred sixty seven billion eight hundred ninety million one hundred twenty three thousand four hundred fifty six",
            "1000000000000, one trillion",
            "234567890123456, two hundred thirty four trillion five hundred sixty seven billion eight hundred ninety million one hundred twenty three thousand four hundred fifty six",
            "987654321000987, nine hundred eighty seven trillion six hundred fifty four billion three hundred twenty one million nine hundred eighty seven",
            "7654321987654321, seven quadrillion six hundred fifty four trillion three hundred twenty one billion nine hundred eighty seven million six hundred fifty four thousand three hundred twenty one",
            "1234567890123456789, one quintillion two hundred thirty four quadrillion five hundred sixty seven trillion eight hundred ninety billion one hundred twenty three million four hundred fifty six thousand seven hundred eighty nine",
            "9223344556677889011, nine quintillion two hundred twenty three quadrillion three hundred forty four trillion five hundred fifty six billion six hundred seventy seven million eight hundred eighty nine thousand eleven"
    })
    void testToWords(long number, String expected) {
        assertEquals(expected, translator.toWords(number));
    }

    @DisplayName("Exception when input number is negative")
    @ParameterizedTest(name = "exception => number={0}")
    @CsvSource({"-1"})
    void testToWordsException(long number) {
        assertThrows(IllegalArgumentException.class, () -> translator.toWords(number));
    }
}