/**
 *  CSE 100 C++ Boggle
 *  Authors: Vivek Ramavajjala, Paul Kube
 */

#include "boggleboard.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

GUIBoggleBoard::GUIBoggleBoard(const char* lexfilename,
			 unsigned int rows,
			 unsigned int cols) {
  // Initialize the board
  createBoard(rows, cols);
  initDiceBag();

  // Initialize the lexicon
  this->lexicon_filename = lexfilename;
  std::ifstream infile;
  infile.open(this->lexicon_filename.c_str());
  if(! infile.is_open()) {
    std::cout<<"Could not open lexicon file " << lexfilename << ", exiting." <<std::endl;
    exit(-1);
  }
  std::cout<<"Reading lexicon from " << lexfilename << "..." <<std::endl;
  std::string word;
  int i=0;
  this->lexicon_words.clear();
  while(infile.is_open() && infile.good())
    {
      std::getline(infile,word);
      if(word.size() < 1) continue;
      // lowercase the word
      //      std::cerr << word << std::endl;
      std::transform(word.begin(), word.end(), word.begin(), ::tolower);
      //      std::cerr << " to " << word << std::endl;
      this->lexicon_words.push_back(word);
      i++;
    }
  std::cout << this->lexicon_words.size() << " distinct words read from " << lexfilename << "." <<std::endl;
  infile.close();


  srand(time(NULL));
}

GUIBoggleBoard::~GUIBoggleBoard() {
  diceBag.clear();
}

int GUIBoggleBoard::returnIndex(unsigned int row, unsigned int col) {
  //Assuming row,col are indexed from 0
  return row*COLS + col;
}

std::vector<int> GUIBoggleBoard::returnGridLocation(unsigned int index) {
  //Assuming indexing starts from 0
  std::vector<int> gridLocation;
  int rowIndex = index / this->COLS;
  int colIndex = index % this->COLS;
  gridLocation.push_back(rowIndex);
  gridLocation.push_back(colIndex);
  return gridLocation;
}

void GUIBoggleBoard::initDiceBag() {
  this->diceBag.push_back(new GUIDie("A", "O", "B", "B", "O", "J"));
  this->diceBag.push_back(new GUIDie("W", "H", "G", "E", "E", "N"));
  this->diceBag.push_back(new GUIDie("N", "R", "N", "Z", "H", "L"));
  this->diceBag.push_back(new GUIDie("N", "A", "E", "A", "G", "E"));
  this->diceBag.push_back(new GUIDie("D", "I", "Y", "S", "T", "T"));
  this->diceBag.push_back(new GUIDie("I", "E", "S", "T", "S", "O"));
  this->diceBag.push_back(new GUIDie("A", "O", "T", "T", "W", "O"));
  this->diceBag.push_back(new GUIDie("H", "Qu", "U", "M", "N", "I"));
  this->diceBag.push_back(new GUIDie("R", "Y", "T", "L", "T", "E"));
  this->diceBag.push_back(new GUIDie("P", "O", "H", "C", "S", "A"));
  this->diceBag.push_back(new GUIDie("L", "R", "E", "V", "Y", "D"));
  this->diceBag.push_back(new GUIDie("E", "X", "L", "D", "I", "R"));
  this->diceBag.push_back(new GUIDie("I", "E", "N", "S", "U", "E"));
  this->diceBag.push_back(new GUIDie("S", "F", "F", "K", "A", "P"));
  this->diceBag.push_back(new GUIDie("I", "O", "T", "M", "U", "C"));
  this->diceBag.push_back(new GUIDie("E", "H", "W", "V", "T", "R"));
}

void GUIBoggleBoard::initRandomBoard() {
  //Random boards are 4x4
  destroyBoard();
  createBoard(4, 4);
  for(unsigned int r=0;r<ROWS;r++)  {
    for(unsigned int c=0;c<COLS;c++)   {
      int index = this->returnIndex(r, c) % this->diceBag.size();
      std::string face = this->diceBag[index]->getRandomFace();
      this->board[r][c] = face;
    }
  }
}

void GUIBoggleBoard::setBoard(unsigned int rows, unsigned int cols, std::string **list) {
  destroyBoard();
  createBoard(rows, cols);
  //    std::cout<<ROWS<<" "<<COLS<<std::endl;
  for(unsigned int r=0;r<ROWS;r++)    {
    for(unsigned int c=0;c<COLS;c++)        {
      this->board[r][c] = list[r][c];
    }
  }
}


void GUIBoggleBoard::destroyBoard() {
  for(unsigned int i=0;i<ROWS;i++) {
    delete[] board[i];
  }
  delete[] board;
}

void GUIBoggleBoard::createBoard(unsigned int rows, unsigned int cols) {
  //Assumes boggle_board and boggle_board_indexed don't already exist.
  ROWS = rows;
  COLS = cols;
  this->board = new std::string*[ROWS];
  for(unsigned int r=0; r < ROWS; r++) {
    this->board[r] = new std::string[COLS];
  }
}


GUIDie::GUIDie() { }

GUIDie::GUIDie(const char* f1, const char* f2, const char* f3, const char* f4, const char* f5, const char* f6) {
  faces.push_back(std::string(f1));
  faces.push_back(std::string(f2));
  faces.push_back(std::string(f3));
  faces.push_back(std::string(f4));
  faces.push_back(std::string(f5));
  faces.push_back(std::string(f6));
}

GUIDie::~GUIDie() {
  faces.clear();
}

std::string GUIDie::getRandomFace() {
  int rand_num = rand() % 6;
  return faces[rand_num];
}

