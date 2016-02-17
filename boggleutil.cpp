/* Names: Luoxiao Li, Kevin Zhang
 *  PIDs: A10940782, A10810276
 *  logins: lul017, ktzhang
 */

#include "boggleutil.h"

/**
 * Trie node constructor
 */
TrieNode::TrieNode(char data, TrieNode* left, TrieNode* middle,
    TrieNode* right) :
    data(data), left(left), middle(middle), right(right), end(false) {
}

/**
 * Delete the node
 */
void
Trie::deleteNode(TrieNode* node) {
  if(node->left != 0)
    deleteNode(node->left);

  if(node->middle != 0)
    deleteNode(node->middle);

  if(node->right != 0)
    deleteNode(node->right);

  delete node;
  return;
}

Trie::Trie() :
    mSize(0), root(0) {
}

Trie::~Trie() {
  //Recursively deletes all nodes in trie using helper method
  clear();
}

void
Trie::build(const vector<string>& vector) {
  int i = 0;
  for(std::vector<string>::const_iterator it = vector.begin();
      it < vector.end(); it++, i++) {
    insert(*it);
  }
  cout << endl;
}

bool
Trie::insert(const string& item) {
  // Tree is empty, insert at root
  if(item.empty()) {
    return false;
  }
  if(root == 0) {
//      cout << "root init is called with value " << item[0] << endl;
    root = new TrieNode(item[0]);
    mSize++;
  }

  //Calling the insert helper
  insertHelper(root, item);
  return true;
}

void
Trie::insertHelper(TrieNode* currNode, const string& item) {

  // Start at top of trie
  // Check to to see if the data item is less
  if(item[0] < currNode->data) {

    if(currNode->left != 0) {
      insertHelper(currNode->left, item);
    }
    else {
//        cout << "  inserting to left " << item[0] << endl;

      TrieNode* temp = new TrieNode(item[0]);
      mSize++;

      currNode->left = temp;
      insertHelper(currNode->left, item);
    }
  }

  // Check to to see if the data item is greater than
  else if(currNode->data < item[0]) {

    if(currNode->right != 0) {
      insertHelper(currNode->right, item);
    }
    else {

//        cout << "  inserting to right " << item[0] << endl;
      TrieNode* temp = new TrieNode(item[0]);
      mSize++;

      currNode->right = temp;
      insertHelper(currNode->right, item);
    }
  }

  // Check if the first character of the string equals
  else {
    if(item.length() > 1) {

      //Move down the trie
      if(currNode->middle != 0) {
        // Cut off the first letter in the string and restart search
        insertHelper(currNode->middle, item.substr(1));
      }
      else {
        // Create a new leaf below and cut off first letter
        TrieNode* temp = new TrieNode(item[1]);
//          cout << "  middle is "<< item.substr(1) << endl;
        mSize++;

        currNode->middle = temp;

        //Setting so that it is true and then ending the function

        insertHelper(currNode->middle, item.substr(1));
      }
    }
    else {
      //Setting to last letter
//        cout << currNode->data << " is last letter" << endl;
      currNode->end = true;
      return;
    }
  }
  //End the thing if the length is only 1
}

TrieNode*
Trie::getNextNode(TrieNode* currNode, const char& character) {
  while(true) {
    if(character < currNode->data) {
//        cout << "  character " <<     character << " is < " << currNode->data << " moving left" <<  endl;
      if(currNode->left == 0) {
        return 0;
      }
      //Character is on the left
      currNode = currNode->left;
      continue;

    }
    else if(currNode->data < character) {
//        cout << "  character " << character << " is > " << currNode->data << " moving right" << endl;

      if(currNode->right == 0) {
        return 0;
      }

      //Character is on the right
      currNode = currNode->right;
      continue;
    }
    else {
      //They are equal
      break;
    }
  }
  return currNode;
}

bool
Trie::find(const string& item) const {
  int stringLen = item.length();
  int i = 0;
  //Which letter to start looking at
  char letter;
  //If trie is empty, return false
  if(root == 0) {
    return false;
  }
  TrieNode* currNode = root;

  //Loop start here
  while(i < stringLen) {
    letter = item[i];
    if(currNode == 0) {
      return false;
    }
    //Check if the word exists
    if(currNode->data == letter && currNode->end && i + 1 == stringLen) {
      return true;

    }

    //Check if it is left
    if(letter < currNode->data) {
      currNode = currNode->left;
    }
    else if(currNode->data < letter) {
      currNode = currNode->right;
    }
    else {
      i++;
      currNode = currNode->middle;
    }
  }
  //Return false if everything doesn't work
  return false;
}

unsigned int
Trie::size() const {
  return mSize;
}

void
Trie::clear() {
  if(root == 0)
    return;
  mSize = 0;
  deleteNode(root); //Same thing as destructor method
  root = 0;
}

bool
Trie::empty() const {
  if(mSize == 0)
    return true;
  return false;
}

