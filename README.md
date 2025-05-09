# Spell Checker

## Overview

This program is an interactive **Spell Checker** that reads a text file, identifies misspelled words not found in a dictionary, and offers the user options for handling each misspelled word. The core components of the algorithm include **hash tables**, a **binary search tree (BST)**, and **vectors** to facilitate fast and organized data operations.

---

## Problem Description

The Spell Checker Algorithm aims to read an input file (e.g., `myFile.txt`), identify words that are not present in a dictionary file (`dict.txt`), and prompt the user with a set of actions to handle each unrecognized word.

### Key Features:
- Efficient word lookups using a **HashTable**.
- Sorted tracking of misspelled words with a **BinarySearchTree**.
- User interaction through a dynamic **menu system**.
- Spelling suggestions generated through character transformations and validated against the dictionary.

---

## Class Relationships

### `HashTable`
- Used to store:
  - All valid dictionary words (`dict.txt`).
  - Words the user chooses to **ignore**.
- Operations used:
  - `find()` to check existence.
  - `insert()` to add new words.
- Used extensively in `spellCheckFile` to validate each word against dictionary and ignore list.

### `BinarySearchTree` (BST)
- Used to store **misspelled words** along with their **line numbers**.
- Supports:
  - Sorted insertion and traversal.
  - Efficient search and storage of unique words.
- Each `BinaryNode` stores:
  - A word (element).
  - Line number from input file.
- Misspelled words are inserted into the BST via the `processMisspelledWord` function.

---

## Client Algorithm

### Workflow:
1. **Dictionary Initialization**
   - Loads valid words from `dict.txt` into the `dictionary` hash table.
   - If file not found, initializes with an empty dictionary.

2. **Data Structure Setup**
   - `HashTable` for dictionary and ignored words.
   - `BinarySearchTree` for misspelled words.

3. **Spell Checking Process**
   - Reads `myFile.txt` line by line.
   - Tokenizes lines into words.
   - Checks each word against `dictionary` and `ignoredWords`.

4. **Menu Interaction for Misspelled Words**
   - If a word is not found, user is prompted with:
     - `A` – Add word to dictionary
     - `I` – Ignore word (skip future checks)
     - `G` – Go on to next word
     - `S` – Search for suggested spellings
     - `Q` – Quit spell checking

5. **Output**
   - After processing, outputs all misspelled words and their line numbers to `notfound.txt`.
   - User is notified of completion.

---

## Menu Options

| Option | Action |
|--------|--------|
| A | Add the word to the dictionary and update `dict.txt` |
| I | Ignore the word for future occurrences |
| G | Skip the current word and continue |
| S | Generate spelling suggestions |
| Q | Quit the spell-checking session |

---

## Spelling Suggestions

Suggestions are generated using a **vector of strings** and validated against the dictionary:

- **Swapping** adjacent characters
- **Inserting** a character at every possible position
- **Removing** one character at a time
- **Replacing** each character with every letter of the alphabet

Only valid dictionary matches are included in the suggestion list.

---

## Compilation & Execution

### Compile and Run:
```bash
c++ spellCheck.cpp
./a.out myFile.txt
```

## License
 No license has been provided for this project
