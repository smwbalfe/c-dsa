
# C Data Structures and Algorithms (DSA)

This repository is a collection of implementations of various data structures and algorithms in C. It aims to provide practical examples and applications of DSA concepts, focusing on ciphers, hash maps, redacting words, and XOR linked lists. Below is a summary of each feature covered in the repository:

## Cipher

- **Columnar Transposition Cipher** (`cipher/columnar_transposition_cipher.c`): This file implements the columnar transposition cipher algorithm, a method to encrypt or decrypt a text message by arranging the characters in columns and then transposing them according to a predetermined key. The implementation demonstrates how to apply this cipher to text data, providing both encryption and decryption functionalities.

## Hash Map

- **Hash Map** (`hash_map/hash_map.c`): A custom hash map data structure is implemented here, demonstrating key-value pair storage with efficient lookup, insertion, and deletion operations. The hash map uses a simple hashing function to map keys to their respective values, with considerations for collision handling. This implementation is exemplified using strings stored in `names.txt` as sample data.

## Redact Words

- **Redacting Words** (`redact_words/redact_words.c`): This feature focuses on redacting specific words from a text. It shows how to search for and replace designated words or phrases within a given text, effectively redacting them. This could be useful for censoring sensitive information or unwanted content from documents or strings.

## XOR Linked List

- **XOR Linked List** (`xor_linked_list/xor_linked_list.c`): An XOR linked list is a memory-efficient linked list where each node stores the XOR of the addresses of the previous and next nodes. This implementation showcases how to perform basic linked list operations such as insertion, deletion, and traversal in an XOR linked list setup, utilizing `names.txt` for demonstration with string data.
