/* Names: Luoxiao Li, Kevin Zhang
 *  PIDs: A10940782, A10810276
 *  logins: lul017, ktzhang
 */

#include <stdio.h>
#include <fstream>
#include <deque>
#include <sstream>
#include <string>
#include "boggleutil.h"
#include "boggleplayer.h"
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <unistd.h>

using namespace std;
class BogglePlayer : public IBogglePlayer {
private:
  int maxRows;
  int maxCols;
  bool isBoardSet;
  bool isLexSet;

  Trie* trie;
  set<string> lexSet;
  //Board that checks the thing
  vector<vector<bool> > checkBoard;
  string** board;

public:
  BogglePlayer() :
      //Initializes the values of CheckBoard
          maxRows(0), maxCols(0), isBoardSet(false), isLexSet(false), board(0) {
  }

//Default Destructor
  virtual
  ~BogglePlayer() {
    delete trie;
  }
  /**
   * Name: buildLexicon
   * Parameter: const set<string> &list
   * Description: build lexicons from the given list
   * Returns: void
   */
  void
  buildLexicon(const vector<string>& word_list) {
    if(!word_list.empty()) {
      trie = new Trie();

      vector<string> newList = word_list;
      random_shuffle(newList.begin(), newList.end());

      trie->build(newList);
      isLexSet = true;
    }
  }

  /**
   * Sets up the board based on the number of rows and cols
   */
  void
  setBoard(unsigned int rows, unsigned int cols, string** diceArray) {
    maxRows = rows;
    maxCols = cols;
    //Initialze the board and the checkBoard
    for(int i = 0; i < maxRows; i++) {
      vector<bool> colVector;
      for(int j = 0; j < maxCols; j++) {
        colVector.push_back(false);
      }
      checkBoard.push_back(colVector);
    }

    board = diceArray;
    isBoardSet = true;
  }
  /*
   * Name: getAllValidWords
   * Parameter: unsigned int minLength
   *            set<string>* words
   * Description: get all valid words according to the lexicon
   * Returns: false -- if setBoard() or buildLexicon() is not called.
   *          true -- otherwise.
   */
  bool
  getAllValidWords(unsigned int minimum_word_length, set<string>* words) {

    if(isBoardSet == false || isLexSet == false) {
      return false;
    }

    vector<string> set = getAnswers();

    vector<string>::iterator it;
    for(it = set.begin(); it != set.end(); it++) {
      //checks to see if the value is at least the minimum word length
      if(it->length() >= minimum_word_length) {
        //if isOnBoard was successful and temp is not empty
        words->insert(*it);

      }
    }
    return true;
  }

  /**
   * Reset the checkboard
   */
  void
  checkBoardReset() {
    for(int i = 0; i < maxRows; i++) {
      for(int j = 0; j < maxCols; j++) {
        checkBoard[i][j] = false;
      }
    }
  }

  /*
   * Name: isInLexicon
   * Parameter: const string& toCheck
   * Description: check whether the given word is within the lexicon list.
   * Returns: true -- if the word is in the lexicon
   *          false -- if buildLexicon is not called or the word is not in the
   *                   lexicon
   */
  bool
  isInLexicon(const string& word_to_check) {
    if(trie->find(word_to_check)) {
      return true;
    }
    else {
      return false;
    }

  }

  /**
   * Check to see if a string is on the board
   */
  vector<int>
  isOnBoard(const string& toCheck) {
    //On board stores a vector
    tolowerstring(toCheck);
    vector<int> onBoardVector;
    for(int currRow = 0; currRow < maxRows; currRow++) {
      for(int currCol = 0; currCol < maxCols; currCol++) {
        checkBoardReset();
        checkBoard[currRow][currCol] = true;
        onBoardVector.push_back(currRow * maxCols + currCol);

        if(isOnBoardHelper(currRow, currCol, 0, toCheck, onBoardVector)) {
          for(std::vector<int>::const_iterator i = onBoardVector.begin();
              i != onBoardVector.end(); ++i) {
          }
          return onBoardVector;
        }
        onBoardVector.pop_back();

        checkBoard[currRow][currCol] = true;
        cout << "checking hte board now" << endl;
      }
    }

    //If nothing, return 0
    return onBoardVector;
  }

  /**
   * Helper recursion method that checks to see if a word is on the board
   * or not
   */
  bool
  isOnBoardHelper(int& currRow, int& currCol, unsigned int charPos,
      const string& toCheck, vector<int>& onBoardVector) {
    //Checks the offset of the string
    int row_offset[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int col_offset[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    //Stores the values of the column and rows
    int newRow, newCol;
    bool returnTrue = false;
    bool shouldContinue = false;
    std::string boardString = tolowerstring(board[currRow][currCol]);
    int size = boardString.size();
    if(toCheck.substr(charPos, size).compare(boardString) == 0) {
      charPos += size;
      if(toCheck.size() == charPos) {
        return true;
      }
      shouldContinue = true;
    }

    //Continue exploring the board
    if(shouldContinue) {
      //Cycling through the offset
      for(int offset = 0; offset < 8; offset++) {

        newRow = currRow + row_offset[offset];
        newCol = currCol + col_offset[offset];

        //check to see if it is within boundaries... if its not, then will go on
        if(newRow < 0 || newRow >= maxRows || newCol < 0 || newCol >= maxCols
            || checkBoard[newRow][newCol]) {
          continue;
        }

        //Grab the new character
        string ourChar = tolowerstring(board[newRow][newCol]);
        onBoardVector.push_back(newRow * maxCols + newCol);

        //Update the board stuff
        checkBoard[currRow][currCol] = true;
        //update the current row and column and recall it
        returnTrue = isOnBoardHelper(newRow, newCol, charPos, toCheck,
            onBoardVector);
        if(!returnTrue) {
          onBoardVector.pop_back();
          //If it doesn't reach it, remove it and continue through the loop
          checkBoard[currRow][currCol] = false;
        }
        else {
          return true;
        }
      }
    }
    return false;
  }

  /**
   * Return a vector of all the answers on the board
   */
  vector<string>
  getAnswers() {
    vector<string> masterStrings;
    vector<string> newStrings;
    //Cycle through each board space
    for(int currRow = 0; currRow < maxRows; currRow++) {
      for(int currCol = 0; currCol < maxCols; currCol++) {
        //Reinitialize checkboard
        checkBoardReset();

        //Initializing the recurring method
        checkBoard[currRow][currCol] = true;
        //Returns an array of the character if it matches and if it is the last word
        newStrings = getSingleAnswer(trie->root, currRow, currCol);
        checkBoard[currRow][currCol] = false;

        //Cycles through the current return value and appends the word
        for(std::vector<string>::iterator it = newStrings.begin();
            it < newStrings.end(); it++) {
          //Combine this new vector with the master vector on top
          masterStrings.push_back(*it);
        }
      }
    }
    return masterStrings;
  }

  /**
   * Getting the answers after passing in a previous character, the row and column
   * Will go through each of the things in order
   */
  vector<string>
  getSingleAnswer(TrieNode* currNode, const int& currRow, const int& currCol) {
    int newRow, newCol;
    int row_offset[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int col_offset[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    vector<string> newStrings;
    vector<string> masterStrings;
    string charString = tolowerstring(board[currRow][currCol]);

    bool shouldContinue = false;

    TrieNode* node = currNode;

    for(unsigned int j = 0; j < (charString.length()); j++) {
      //Get the current character node

      node = trie->getNextNode(node, charString.at(j));
      //If this exists
      if(node) {
        //Check if j is the last character
        if(j == (charString.length() - 1)) {
          //Returns an array of the character if it matches and if it is the last word
          if(node->data == charString.at(charString.length() - 1)
              && node->end) {
            masterStrings.push_back(charString);
          }
        }
        //Traverse to the next character
        if(node->middle) {
          //Going down the middle
          node = node->middle;
          shouldContinue = true;
          continue;
        }
        else {
          shouldContinue = false;
        }
      }
      else {
        shouldContinue = false;
        break;
      }
    }

    //Go through the last few letters of the board if everything passes
    if(shouldContinue) {
      for(int offset = 0; offset < 8; offset++) {
        newRow = currRow + row_offset[offset];
        newCol = currCol + col_offset[offset];

        //check to see if it is within boundaries
        if(newRow < 0 || newRow >= maxRows || newCol < 0 || newCol >= maxCols
            || checkBoard[newRow][newCol]) {
          continue;
        }

        //Start the actual finding algorithm
        string newBoardString = tolowerstring(board[newRow][newCol]);

        //Setting the checkBoard to be false
        checkBoard[newRow][newCol] = true;
        newStrings = getSingleAnswer(node, newRow, newCol);
        checkBoard[newRow][newCol] = false;

        //Combine the strings
        //Prepend "character" into each string in the vector
        for(std::vector<string>::iterator it = newStrings.begin();
            it < newStrings.end(); it++) {
          //Combine this new vector with the master vector on top
          masterStrings.push_back(charString + *it);
        }
      }

    }
    return masterStrings;
  }

  /*
   * Name: getCustomBoard
   * Parameter: string** &board
   *            unsigned int *rows
   *            unsigned int *cols
   * Description: creates a customboard according to the given parameters
   * Returns: void
   */
  void
  getCustomBoard(string** &board, unsigned int *rows, unsigned int *cols) {
    //Opening the file stream
    ifstream file;
    file.open("brd.txt");
    string str;
    getline(file, str);
    *rows = atoi(str.c_str());
    getline(file, str);
    *cols = atoi(str.c_str());

    board = new string *[ (*rows)];
    for(unsigned int i = 0; i < (*rows); i++) {
      board[i] = new string[ (*cols)];
    }

    //Sets teh board values
    for(unsigned int i = 0; i < (*rows); i++) {
      for(unsigned int j = 0; j < (*cols); j++) {
        getline(file, str);
        board[i][j] = str;
      }
    }

    file.close();
  }
  /*
   *converts the strings to lower case
   */
  static std::string
  tolowerstring(std::string strToConvert) {
    std::string res;
    for(std::string::iterator p = strToConvert.begin(); strToConvert.end() != p;
        ++p)
      res += tolower(*p);
    return res;
  }
};

