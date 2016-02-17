/**
 *  CSE 100 C++ Boggle
 *  Authors: Vivek Ramavajjala, Paul Kube
 */

#ifndef BOGGLEBOARD_H
#define BOGGLEBOARD_H

#include <string>
#include <vector>
#include <set>

class GUIDie {
public:
    std::vector<std::string> faces;
    GUIDie();
    GUIDie(const char* f1, const char* f2, const char* f3, const char* f4, const char* f5, const char* f6);
    ~GUIDie();
    std::string getRandomFace();
};

class GUIBoggleBoard {
  
public:
    std::string lexicon_filename;
    std::vector<std::string> lexicon_words;
    unsigned int ROWS;
    unsigned int COLS;
    std::string **board;
    std::vector<GUIDie*> diceBag;

private:
    void destroyBoard();
    void createBoard(unsigned int rows, unsigned int cols);

public:
    GUIBoggleBoard(const char* lexfilename, unsigned int rows, unsigned int cols);
    ~GUIBoggleBoard();
    /*Helper functions*/
    int returnIndex(unsigned int row, unsigned int col);
    std::vector<int> returnGridLocation(unsigned int index);

    /*Game functions*/
    void initDiceBag();
    void initRandomBoard();
    void setBoard(unsigned int rows, unsigned int cols, std::string **list);
};



#endif // BOGGLEBOARD_H
