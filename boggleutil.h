/* Names: Luoxiao Li, Kevin Zhang
 *  PIDs: A10940782, A10810276
 *  logins: lul017, ktzhang
 */

#ifndef BOGGLEUTIL_H
#define BOGGLEUTIL_H
#include <utility> // for std::pair
#include <iostream>
#include <iterator>
#include <algorithm>  
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <stack>
#include <set>
//#include <boggleutil.cpp>

using namespace std;
//extern
class TrieNode {

public:
  char data; // byte in the file we're keeping track of
  TrieNode* left;  // pointer to left child
  TrieNode* middle;  // pointer to middle child
  TrieNode* right;  // pointer to right child
  bool end;

  TrieNode(char data, TrieNode* left = 0, TrieNode* middle = 0,
      TrieNode* right = 0);
};

//extern
class Trie {

private:
  void
  deleteNode(TrieNode* node);

protected:
  unsigned int mSize;

public:
  TrieNode* root;
  Trie();

  ~Trie();

  void
  build(const vector<string>& vector);

  bool
  insert(const string& item);

  void
  insertHelper(TrieNode* currNode, const string& item);

  TrieNode*
  getNextNode(TrieNode* currNode, const char& character);

  bool
  find(const string& item) const;

  unsigned int
  size() const;

  void
  clear();

  bool
  empty() const;
};

#endif
