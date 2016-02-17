#ifndef BOGGLEPLAYER_H
#define BOGGLEPLAYER_H

#include <set>
#include <vector>
#include <string>
#include "boggleutil.h"
#include "iboggleplayer.h"

using std::vector;
using std::set;
using std::string;

/**
 *  Class that implements a boggle player.  Implements method that set up
 *  the boggle game by building the dictionary/lexicon and board for the game.
 *  Implements methods for human player to play and for computer player to
 *  find all the words on the board.
 */
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
  void
  buildLexicon(const vector<string>& word_list);
  void
  setBoard(unsigned int rows, unsigned int cols, string** diceArray);
  bool
  getAllValidWords(unsigned int minimum_word_length, set<string>* words);
  bool
  isInLexicon(const string& word_to_check);
  vector<int>
  isOnBoard(const string& word_to_check);
  void
  getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols);

  virtual
  ~BogglePlayer() {
    delete trie;
  }
};

#endif // BOGGLEPLAYER_H
