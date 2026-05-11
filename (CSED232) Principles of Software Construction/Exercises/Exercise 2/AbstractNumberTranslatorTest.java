package edu.postech.csed232.exercise2;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.List;
import java.util.Locale;

import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

/**
 * Template-method tests for {@link AbstractNumberTranslator} using a small synthetic
 * language (Lunari) with chunk size 2. The focus is orchestration behavior:
 * chunk splitting, unit placement, ordering, skipping zero chunks, and input validation.
 */
class AbstractNumberTranslatorTest {

    /**
     * A test implementation of {@link AbstractNumberTranslator} for the Lunari language.
     *
     * <p>Lunari is a constructed numerical language system that uses a base-100 chunk size
     * for number translation. Numbers are broken down into chunks of two digits each, with
     * each chunk translated to Lunari words and combined with appropriate large-unit labels
     * (zen, tor, lum, vek, ryn) to denote magnitude.
     *
     * <p>Key features of Lunari:
     * <ul>
     *   <li>Digit words: nul (0), ka (1), tu (2), mi (3), na (4), po (5), ru (6), se (7), vi (8), zo (9)</li>
     *   <li>Tens formation: appends "dek" to the tens digit (e.g., "kadek" for 10, "kadek-tu" for 12)</li>
     *   <li>Large units: zen (10^2), tor (10^4), lum (10^6), vek (10^8), ryn (10^10)</li>
     *   <li>Chunk separator: pipe character (|) is used to join translated chunks</li>
     * </ul>
     *
     * <p>Example: The number 1234 would be translated as two chunks (12 and 34):
     * "kadek-tu|mi-dek-na"
     */
    private static final class LunariNumberTranslator extends AbstractNumberTranslator {
        // Digit words for 0..9 in Lunari.
        private static final String[] DIGITS = {
            "nul", "ka", "tu", "mi", "na", "po", "ru", "se", "vi", "zo"
        };
        // Large-unit labels by chunk index (index 0 is empty).
        private static final String[] LARGE_UNITS = {
            "", " zen", " tor", " lum", " vek", " ryn"
        };

        LunariNumberTranslator() {
            super(Locale.ROOT);
        }

        @Override
        protected int getChunkSize() {
            return 2;
        }

        @Override
        protected String zeroWord() {
            return "nul";
        }

        @Override
        protected String chunkToWords(int chunk) {
            assert 0 <= chunk && chunk < 100 : "chunk should be in [0, 100)";
            int tens = chunk / 10;
            int ones = chunk % 10;

            return switch (tens) {
                case 0 -> DIGITS[ones];
                default -> ones == 0
                        ? DIGITS[tens] + "dek"
                        : DIGITS[tens] + "dek-" + DIGITS[ones];
            };
        }

        @Override
        protected String chunkUnit(int index) {
            assert index >= 0 : "chunk index should be non-negative";
            if (index >= LARGE_UNITS.length) {
                throw new IllegalArgumentException("unsupported unit index");
            }
            return LARGE_UNITS[index];
        }

        @Override
        protected String joinChunks(List<String> translatedChunks) {
            return String.join("|", translatedChunks);
        }
    }

    private final AbstractNumberTranslator translator = new LunariNumberTranslator();

    @DisplayName("AbstractNumberTranslator template method - Lunari language")
    @ParameterizedTest(name = "{index} => number={0}, expected={1}")
    @CsvSource({
            // Single-chunk values (0..99)
            "0, nul",
            "1, ka",
            "9, zo",
            "10, kadek",
            "12, kadek-tu",
            "20, tudek",
            "45, nadek-po",
            "99, zodek-zo",

            // Multi-chunk values and zero-chunk skipping
            "100, ka zen",
            "101, ka zen|ka",
            "120, ka zen|tudek",
            "1234, kadek-tu zen|midek-na",
            "10000, ka tor",
            "10001, ka tor|ka",
            "120056, kadek-tu tor|podek-ru",
            "123456, kadek-tu tor|midek-na zen|podek-ru",
            "9000001, zo lum|ka",
            "987654321, zo vek|videk-se lum|rudek-po tor|nadek-mi zen|tudek-ka",
            "99999999999, zo ryn|zodek-zo vek|zodek-zo lum|zodek-zo tor|zodek-zo zen|zodek-zo"
    })
    void testToWords(long number, String expected) {
        assertEquals(expected, translator.toWords(number));
    }

    @DisplayName("Exception when input number is negative")
    @ParameterizedTest(name = "exception => number={0}")
    @CsvSource({
            "-1",
            "-10",
            "-9999"
    })
    void testToWordsException(long number) {
        assertThrows(IllegalArgumentException.class, () -> translator.toWords(number));
    }
}
