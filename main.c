#include <stdio.h>
#include <stdint.h>
#include <string.h>

// ************************************************************
//                      MILESTONE 1

int32_t is_ascii(char str[]) {
    int index = 0;
    int ascii = 1;

    while (str[index] != '\0'){
        char c = str[index];
        if ((c & 0b10000000) != 0){
            ascii = 0;
            break;
        }
        index = index + 1;
    }
    return ascii;
}

int32_t capitalize_ascii(char str[]){
    int32_t chars_updated = 0; // Counter for the updated characters
    int index = 0; // Index to travese the string

    //Iterate through each character in the string
    while (str[index] != '\0') {
        // Check if the character is a lowercase ASCII letter (a-z)
        if (str[index] >= 'a' && str[index] <= 'z') {
            // Convert lowecase to uppercase by subtracting 32 from ASCII value
            str[index] = str[index] - 32;
            chars_updated += 1;
        }
        index += 1;
    }
    return chars_updated;
}


// **********************************************************
//                      MILESTONE 2

int32_t width_from_start_byte(char start_byte) {
    uint8_t byte = (uint8_t)start_byte;

    // Check for ASCII (0xxxxxxx)
    if ((byte & 0b10000000) == 0b00000000) {
        return 1;
    }
    // Check for 2-byte sequence (110xxxxx)
    else if ((byte & 0b11100000) == 0b11000000) {
        return 2;
    }
    // Check for 3-byte sequence (1110xxxx)
    else if ((byte & 0b11110000) == 0b11100000) {
        return 3;
    }
    // Check for 4-byte sequence (11110xxx)
    else if ((byte & 0b11111000) == 0b11110000) {
        return 4;
    }
    // Invalid start byte
    else{
        return -1;
    }
}

int32_t utf8_strlen(char str[]) {
    int32_t length = 0;
    int32_t i = 0;

    while (str[i] != '\0') {
        int32_t char_width = width_from_start_byte(str[i]);

        if (char_width == -1) {
            // Invalid UTF-8 sequence encountered
            return -1;
        } else if (char_width > 0) {
            // Valid UTF-8 character
            length ++;
            i += char_width;
        }
    }
    return length;
}

int32_t codepoint_index_to_byte_index(char str[], int32_t cpi) {
    int32_t byte_index = 0;
    int32_t codepoint_count = 0;

    while (str[byte_index] != '\0' && codepoint_count < cpi) {
        int32_t char_width = width_from_start_byte(str[byte_index]);
        
        if (char_width <= 0) {
            // Invalid UTF-8 sequence treated as 1-byte
            char_width = 1;
        } 
        byte_index += char_width;
        codepoint_count += 1; 
    }
    return (str[byte_index] != '\0') ? byte_index : -1;
}

void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[]) {
    //Check for invalid input conditions
    if (cpi_start < 0 || cpi_end < 0 || cpi_start >= cpi_end) {
        result[0] = '\0'; // Return an empty string
        return;
    }

    int32_t start_byte = codepoint_index_to_byte_index(str, cpi_start);
    int32_t end_byte = codepoint_index_to_byte_index(str, cpi_end);

    //Check if either start or end index is invalid
    if (start_byte == -1) {
        result[0] = '\0';
        return;
    }

    if (end_byte == -1){
        strcpy(result, &str[start_byte]);
    }  else {
        int32_t length = end_byte - start_byte;
        strncpy(result, &str[start_byte], length);
        result[length] = '\0';  // Null-terminate the result string
    }
}


// **********************************************************
//                      MILESTONE 3

int32_t codepoint_at(char str[], int32_t cpi) {
    int32_t byte_index = codepoint_index_to_byte_index(str, cpi);
    
    if (byte_index == -1) {
        return -1;  // Invalid index or error in UTF-8 string
    }

    uint8_t first_byte = (uint8_t)str[byte_index];
    int32_t codepoint = 0;

    if ((first_byte & 0b10000000) == 0) {
        // 1-byte sequence (ASCII)
        codepoint = first_byte;
    } else if ((first_byte & 0b11100000) == 0b11000000) {
        // 2-bytes
        codepoint = ((first_byte & 0b00011111) << 6) | 
                    ((uint8_t)str[byte_index + 1] & 0b00111111);
    } else if ((first_byte & 0b11110000) == 0b11100000) {
        // 3-bytes
        codepoint = ((first_byte & 0b00001111) << 12) | 
                    (((uint8_t)str[byte_index + 1] & 0b00111111) << 6) | 
                    ((uint8_t)str[byte_index + 2] & 0b00111111);
    } else if ((first_byte & 0b11111000) == 0b11110000) {
        // 4-bytes
        codepoint = ((first_byte & 0b00000111) << 18) | 
                    (((uint8_t)str[byte_index + 1] & 0b00111111) << 12) | 
                    (((uint8_t)str[byte_index + 2] & 0b00111111) << 6) | 
                    ((uint8_t)str[byte_index + 3] & 0b00111111);
    } else {
        return -1;  // Invalid UTF-8 sequence
    }

    return codepoint;
}

char is_animal_emoji_at(char str[], int32_t cpi) {
    int32_t codepoint = codepoint_at(str, cpi);

    if (codepoint == -1) {
        return 0;
    }

    // 🐁 (U+1F400) to 🐿 (U+1F43F)
    if (codepoint >= 0x1F400 && codepoint <= 0x1F43F) {
        return 1;
    }

    // 🦀 (U+1F980) to 🦮 (U+1F9AE)
    if (codepoint >= 0x1F980 && codepoint <= 0x1F9AE) {
        return 1;
    }

    // Not animal
    return 0;
}

// ********************* RESUBMISSION ****************************


void next_utf8_char(char str[], int32_t cpi, char result[]) {
    // Gets current codepoint value
    int32_t current = codepoint_at(str, cpi);

    // If valid codepoint or string is too short
    if (current == -1) {
        result[0] = '\0';
        return;
    }

    // Calculate the next codepoint in UTF-8
    int32_t next = current + 1;

    // Encode the next codepoint in UTF-8
    if (next <= 0x7F) {
        // 1-byte sequence 
        result[0] = (char)next;
        result[1] = '\0';
    }
    else if (next <= 0x7FF) {
        // 2-byte sequence
        result[0] = (char)(0xC0 | (next >> 6));
        result[1] = (char)(0x80 | (next & 0x3F));
        result[2] = '\0';
    }
    else if (next <= 0xFFFF) {
        // 3-byte sequence 
        result[0] = (char)(0xE0 | (next >> 12));
        result[1] = (char)(0x80 | ((next >> 6) & 0x3F));
        result[2] = (char)(0x80 | (next & 0x3F));
        result[3] = '\0';
    }
    else if (next <= 0x10FFFF){
        // 4-byte sequence
        result[0] = (char)(0xF0 | (next >> 18));
        result[1] = (char)(0x80 | ((next >> 12) & 0x3F));
        result[2] = (char)(0x80 | ((next >> 6) & 0x3F));
        result[3] = (char)(0x80 | (next & 0x3F));
        result[4] = '\0';
    }
    else {
        // Invalid Unicode range
        result[0] = '\0';
    }

}




// *********************************************
//              MAIN FUNCTION

int main() {
    char str[100];
    printf("Enter a UTF-8 encoded string: ");
    scanf("%99[^\n]", str);

    // Valid ASCII
    int32_t is_ascii_result = is_ascii(str);
    printf("\nValid ASCII: %s\n", is_ascii_result ? "true" : "false");

    // Uppercased ASCII
    char uppercased[100];
    strcpy(uppercased, str);
    capitalize_ascii(uppercased);
    printf("Uppercased ASCII: \"%s\"\n", uppercased);

    // Length in bytes
    printf("Length in bytes: %lu\n", strlen(str));

    // Number of code points
    int32_t num_codepoints = utf8_strlen(str);
    printf("Number of code points: %d\n", num_codepoints);

    // Bytes per code point
    printf("Bytes per code point:");
    for (int i = 0; i < num_codepoints; i++) {
        int32_t byte_index = codepoint_index_to_byte_index(str, i);
        int32_t width = width_from_start_byte(str[byte_index]);
        printf(" %d", width);
    }
    printf("\n");

    // Substring of the first 6 code points
    char substring[100];
    utf8_substring(str, 0, 6, substring);
    printf("Substring of the first 6 code points: \"%s\"\n", substring);

    // Code points as decimal numbers
    printf("Code points as decimal numbers:");
    for (int i = 0; i < num_codepoints; i++) {
        printf(" %d", codepoint_at(str, i));
    }
    printf("\n");

    // Animal emojis
    printf("Animal emojis: ");
    for (int i = 0; i < num_codepoints; i++) {
        if (is_animal_emoji_at(str, i)) {
            char emoji[5];
            utf8_substring(str, i, i+1, emoji);
            printf("%s", emoji);
        }
    }
    printf("\n");

    // RESUBMIT

    char next_char[5];
    next_utf8_char(str, 3, next_char);
    printf("Next Character of Codepoint at Index 3: %s\n", next_char);
    printf("\n");


    return 0;
    printf("\n");

    // RESUBMIT

}