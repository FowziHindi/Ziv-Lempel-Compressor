#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>

// A hash table that works with any key and value type
template <typename K, typename V>
class HashTable {
private:
    // Each slot holds a key, value, and a flag to show if it's used
    struct Slot {
        bool used;
        K key;
        V value;
        Slot() : used(false) {}
    };
    std::vector<Slot> table;
    int table_size;

    // Turns a key into a number to find its spot
    unsigned long hash(const K& key) const {
        std::hash<K> hasher;
        return hasher(key) % table_size;
    }

public:
    // Set up the hash table with a given size (default is 16384)
    HashTable(int size = 16384) : table_size(size) {
        // Make sure size isn't zero or negative, use default if it is
        if (table_size <= 0) table_size = 16384;
        table.resize(table_size);
    }

    // Add a key and value to the table
    void add(const K& key, const V& value) {
        // Find where the key should go using the hash
        unsigned long spot = hash(key);
        unsigned long start = spot;
        // Keep looking for an empty spot or the same key
        while (table[spot].used && table[spot].key != key) {
            spot = (spot + 1) % table_size;
            // If we loop back to start, table is full, so stop
            if (spot == start) return;
        }
        // If the spot is empty, fill it with the new key and value
        if (!table[spot].used) {
            table[spot].used = true;
            table[spot].key = key;
            table[spot].value = value;
        } else {
            // If key exists, just update the value
            table[spot].value = value;
        }
    }

    // Look for a key and get its value
    bool get(const K& key, V& value) const {
        // Find where the key might be using the hash
        unsigned long spot = hash(key);
        unsigned long start = spot;
        // Keep looking until we find the key or loop back
        while (table[spot].used) {
            if (table[spot].key == key) {
                // Found it! Set the value and return true
                value = table[spot].value;
                return true;
            }
            spot = (spot + 1) % table_size;
            // If we loop back to start, key isn't here
            if (spot == start) break;
        }
        // Key not found, return false
        return false;
    }
};

// Turns a string into a list of numbers
void compress(const std::string& text) {
    // Set up a dictionary to store strings and their codes
    HashTable<std::string, int> dictionary(16384);
    // Fill dictionary with all single letters (0-255)
    for (int i = 0; i < 256; ++i) {
        dictionary.add(std::string(1, static_cast<char>(i)), i);
    }

    // Start assigning new codes from 256
    int next_code = 256;
    // Start with an empty string to build strings
    std::string current_prefix;
    // If there's text, begin with the first character
    if (!text.empty()) {
        current_prefix += text[0];
    }

    // Loop through the text to compress it
    for (size_t i = 1; i <= text.length(); ++i) {
        // Get the next character, or 0 if we're at the end
        char current_char = (i < text.length()) ? text[i] : 0;
        // Add the current character to the prefix
        std::string extended_prefix = current_prefix;
        if (current_char != 0) {
            extended_prefix += current_char;
        }

        // Check if the extended prefix is in the dictionary
        int code_check;
        if (dictionary.get(extended_prefix, code_check) && current_char != 0) {
            // If it is, keep building the prefix
            current_prefix = extended_prefix;
        } else {
            // If not, output the code for the current prefix
            int output_code;
            if (dictionary.get(current_prefix, output_code)) {
                std::cout << output_code << " ";
            }
            // Add the extended prefix to the dictionary with a new code
            if (current_char != 0 && next_code < 4096) {
                dictionary.add(extended_prefix, next_code++);
            }
            // Start a new prefix with the current character
            if (current_char != 0) {
                current_prefix = std::string(1, current_char);
            } else {
                // If we're at the end, stop
                break;
            }
        }
    }
    // Finish with a new line
    std::cout << std::endl;
}

// Turns a list of numbers back into a string
void decompress(const std::vector<int>& numbers) {
    // Set up a dictionary to store codes and their strings
    std::vector<std::string> dictionary(4096);
    // Fill dictionary with all single letters (0-255)
    for (int i = 0; i < 256; ++i) {
        dictionary[i] = std::string(1, static_cast<char>(i));
    }

    // If there are no numbers, just print a new line and stop
    if (numbers.empty()) {
        std::cout << std::endl;
        return;
    }

    // Make sure the first number is a valid single character code
    if (numbers[0] < 0 || numbers[0] > 255) {
        std::cerr << "Error: Invalid starting code " << numbers[0] << std::endl;
        return;
    }

    // Start with the first code's string
    std::string previous_entry = dictionary[numbers[0]];
    std::cout << previous_entry;
    // Start assigning new codes from 256
    int next_available_code = 256;
    std::string current_entry;

    // Loop through the rest of the codes to decompress
    for (size_t i = 1; i < numbers.size(); ++i) {
        int current_code = numbers[i];

        // Check if the code is in the dictionary
        if (current_code >= 0 && current_code < next_available_code && current_code < dictionary.size() && !dictionary[current_code].empty()) {
            current_entry = dictionary[current_code];
        } else if (current_code == next_available_code) {
            // Special case: code matches the next available code
            current_entry = previous_entry + previous_entry[0];
        } else {
            // If the code is invalid, show an error and stop
            std::cerr << "\nError: Invalid code encountered during decompression: " << current_code << std::endl;
            return;
        }

        // Print the current string
        std::cout << current_entry;

        // Add a new string to the dictionary if we haven't hit the limit
        if (next_available_code < 4096) {
            dictionary[next_available_code++] = previous_entry + current_entry[0];
        }

        // Update the previous entry for the next round
        previous_entry = current_entry;
    }

    // Finish with a new line
    std::cout << std::endl;
}

// Main
int main() {
    // Ask the user what they want to do
    std::cout << "To compress a file, press 1. To decompress a file, press 2: ";
    int option;
    // Check if the input is a valid number
    if (!(std::cin >> option)) {
        std::cerr << "Invalid input. Please enter 1 or 2." << std::endl;
        return 1;
    }
    // Clear the input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Handle the user's choice
    if (option == 1) {
        // Get the string to compress
        std::cout << "Enter the input string: ";
        std::string input_string;
        std::getline(std::cin, input_string);
        std::cout << "Compressed output: ";
        compress(input_string);
    } else if (option == 2) {
        // Get the numbers to decompress
        std::cout << "Enter the compressed string: ";
        std::string line;
        std::getline(std::cin, line);
        // Turn the input string into a list of numbers
        std::stringstream stream(line);
        std::vector<int> codes;
        int number;
        while (stream >> number) {
            codes.push_back(number);
        }
        std::cout << "Decompressed string: ";
        decompress(codes);
    } else {
        // If the choice isn't 1 or 2, show an error
        std::cerr << "Invalid option selected. Please run again and enter 1 or 2." << std::endl;
        return 1;
    }
    return 0;
}