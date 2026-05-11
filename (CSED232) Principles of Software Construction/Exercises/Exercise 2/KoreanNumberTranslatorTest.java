package edu.postech.csed232.exercise2;

import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import org.junit.jupiter.api.DisplayName;

class KoreanNumberTranslatorTest {

    final KoreanNumberTranslator translator = new KoreanNumberTranslator();

    @DisplayName("KoreanNumberTranslator - various numbers to words")
    @ParameterizedTest(name = "{index} => number={0}, expected={1}")
    @CsvSource({
            "0, 영",
            "12, 십이",
            "716, 칠백십육",
            "1234, 일천이백삼십사",
            "91817, 구만일천팔백십칠",
            "1000000, 일백만",
            "100200300, 일억이십만삼백",
            "1234567890, 십이억삼천사백오십육만칠천팔백구십",
            "10000001201, 일백억일천이백일",
            "31057642375, 삼백십억오천칠백육십사만이천삼백칠십오",
            "53987654321, 오백삼십구억팔천칠백육십오만사천삼백이십일",
            "112358132134, 일천일백이십삼억오천팔백십삼만이천일백삼십사",
            "567890123456, 오천육백칠십팔억구천십이만삼천사백오십육",
            "1000000000000, 일조",
            "234567890123456, 이백삼십사조오천육백칠십팔억구천십이만삼천사백오십육",
            "987654321000987, 구백팔십칠조육천오백사십삼억이천일백만구백팔십칠",
            "7654321987654321, 칠천육백오십사조삼천이백십구억팔천칠백육십오만사천삼백이십일",
            "1234567890123456789, 일백이십삼경사천오백육십칠조팔천구백일억이천삼백사십오만육천칠백팔십구",
            "9223344556677889011, 구백이십이경삼천삼백사십사조오천오백육십육억칠천칠백팔십팔만구천십일"
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
