#include <Rcpp.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>

using namespace std;
using namespace Rcpp;

// Amino acid to index mapping
const map<char, int> aa_to_index = {
  {'A', 0}, {'R', 1}, {'N', 2}, {'D', 3}, {'C', 4},
  {'Q', 5}, {'E', 6}, {'G', 7}, {'H', 8}, {'I', 9},
  {'L',10}, {'K',11}, {'M',12}, {'F',13}, {'P',14},
  {'S',15}, {'T',16}, {'W',17}, {'Y',18}, {'V',19},
  {'B',20}, {'Z',21}, {'X',22}, {'*', 23}
};

const int BLOSUM62[24][24] = {
  { 4,-1,-2,-2, 0,-1,-1, 0,-2,-1,-1,-1,-1,-2,-1, 1, 0,-3,-2, 0,-2,-1, 0,-4}, // A
  {-1, 5, 0,-2,-3, 1, 0,-2, 0,-3,-2, 2,-1,-3,-2,-1,-1,-3,-2,-3,-1, 0,-1,-4}, // R
  {-2, 0, 6, 1,-3, 0, 0, 0, 1,-3,-3, 0,-2,-3,-2, 1, 0,-4,-2,-3, 3, 0,-1,-4}, // N
  {-2,-2, 1, 6,-3, 0, 2,-1,-1,-3,-4,-1,-3,-3,-1, 0,-1,-4,-3,-3, 4, 1,-1,-4}, // D
  { 0,-3,-3,-3, 9,-3,-4,-3,-3,-1,-1,-3,-1,-2,-3,-1,-1,-2,-2,-1,-3,-3,-2,-4}, // C
  {-1, 1, 0, 0,-3, 5, 2,-2, 0,-3,-2, 1, 0,-3,-1, 0,-1,-2,-1,-2, 0, 3,-1,-4}, // Q
  {-1, 0, 0, 2,-4, 2, 5,-2, 0,-3,-3, 1,-2,-3,-1, 0,-1,-3,-2,-2, 1, 4,-1,-4}, // E
  { 0,-2, 0,-1,-3,-2,-2, 6,-2,-4,-4,-2,-3,-3,-2, 0,-2,-2,-3,-3,-1,-2,-1,-4}, // G
  {-2, 0, 1,-1,-3, 0, 0,-2, 8,-3,-3,-1,-2,-1,-2,-1,-2,-2, 2,-3, 0, 0,-1,-4}, // H
  {-1,-3,-3,-3,-1,-3,-3,-4,-3, 4, 2,-3, 1, 0,-3,-2,-1,-3,-1, 3,-3,-3,-1,-4}, // I
  {-1,-2,-3,-4,-1,-2,-3,-4,-3, 2, 4,-2, 2, 0,-3,-2,-1,-2,-1, 1,-4,-3,-1,-4}, // L
  {-1, 2, 0,-1,-3, 1, 1,-2,-1,-3,-2, 5,-1,-3,-1, 0,-1,-3,-2,-2, 0, 1,-1,-4}, // K
  {-1,-1,-2,-3,-1, 0,-2,-3,-2, 1, 2,-1, 5, 0,-2,-1,-1,-1,-1, 1,-3,-1,-1,-4}, // M
  {-2,-3,-3,-3,-2,-3,-3,-3,-1, 0, 0,-3, 0, 6,-4,-2,-2, 1, 3,-1,-3,-3,-1,-4}, // F
  {-1,-2,-2,-1,-3,-1,-1,-2,-2,-3,-3,-1,-2,-4, 7,-1,-1,-4,-3,-2,-2,-1,-2,-4}, // P
  { 1,-1, 1, 0,-1, 0, 0, 0,-1,-2,-2, 0,-1,-2,-1, 4, 1,-3,-2,-2, 0, 0, 0,-4}, // S
  { 0,-1, 0,-1,-1,-1,-1,-2,-2,-1,-1,-1,-1,-2,-1, 1, 5,-2,-2, 0,-1,-1, 0,-4}, // T
  {-3,-3,-4,-4,-2,-2,-3,-2,-2,-3,-2,-3,-1, 1,-4,-3,-2,11, 2,-3,-4,-3,-2,-4}, // W
  {-2,-2,-2,-3,-2,-1,-2,-3, 2,-1,-1,-2,-1, 3,-3,-2,-2, 2, 7,-1,-3,-2,-1,-4}, // Y
  { 0,-3,-3,-3,-1,-2,-2,-3,-3, 3, 1,-2, 1,-1,-2,-2, 0,-3,-1, 4,-3,-2,-1,-4}, // V
  {-2,-1, 3, 4,-3, 0, 1,-1, 0,-3,-4, 0,-3,-3,-2, 0,-1,-4,-3,-3, 4, 1,-1,-4}, // B
  {-1, 0, 0, 1,-3, 3, 4,-2, 0,-3,-3, 1,-1,-3,-1, 0,-1,-3,-2,-2, 1, 4,-1,-4}, // Z
  { 0,-1,-1,-1,-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-2, 0, 0,-2,-1,-1,-1,-1,-1,-4}, // X
  {-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4,-4, 1}  // *
};


const int BLOSUM45[24][24] = {
  { 5,-2,-1,-2,-1,-1,-1, 0,-2,-1,-1,-1,-1,-2,-1, 1, 0,-2,-2, 0,-1,-1, 0,-5}, 
  {-2, 7, 0,-1,-3, 1, 0,-2, 0,-3,-2, 3,-1,-2,-2,-1,-1,-2,-1,-2,-1, 0,-1,-5},
  {-1, 0, 6, 2,-2, 0, 0, 0, 1,-2,-3, 0,-2,-2,-2, 1, 0,-4,-2,-3, 4, 0,-1,-5},
  {-2,-1, 2, 7,-3, 0, 2,-1, 0,-4,-3, 0,-3,-4,-1, 0,-1,-4,-2,-3, 5, 1,-1,-5},
  {-1,-3,-2,-3,12,-3,-3,-3,-3,-3,-2,-3,-2,-2,-4,-1,-1,-5,-3,-1,-2,-3,-2,-5},
  {-1, 1, 0, 0,-3, 6, 2,-2, 1,-2,-2, 1, 0,-4,-1, 0,-1,-2,-1,-3, 0, 4,-1,-5},
  {-1, 0, 0, 2,-3, 2, 6,-2, 0,-3,-2, 1,-2,-3, 0, 0,-1,-3,-2,-3, 1, 4,-1,-5},
  { 0,-2, 0,-1,-3,-2,-2, 7,-2,-4,-3,-2,-2,-3,-2, 0,-2,-2,-3,-3,-1,-2,-1,-5},
  {-2, 0, 1, 0,-3, 1, 0,-2,10,-3,-2,-1, 0,-2,-2,-1,-2,-3, 2,-3, 0, 0,-1,-5},
  {-1,-3,-2,-4,-3,-2,-3,-4,-3, 5, 2,-3, 2, 0,-2,-2,-1,-2, 0, 3,-3,-3,-1,-5},
  {-1,-2,-3,-3,-2,-2,-2,-3,-2, 2, 5,-3, 2, 1,-3,-3,-1,-2, 0, 1,-3,-2,-1,-5},
  {-1, 3, 0, 0,-3, 1, 1,-2,-1,-3,-3, 5,-1,-3,-1,-1,-1,-2,-1,-2, 0, 1,-1,-5},
  {-1,-1,-2,-3,-2, 0,-2,-2, 0, 2, 2,-1, 6, 0,-2,-2,-1,-2, 0, 1,-2,-1,-1,-5},
  {-2,-2,-2,-4,-2,-4,-3,-3,-2, 0, 1,-3, 0, 8,-3,-2,-1, 1, 3, 0,-3,-3,-1,-5},
  {-1,-2,-2,-1,-4,-1, 0,-2,-2,-2,-3,-1,-2,-3, 9,-1,-1,-3,-3,-3,-2,-1,-1,-5},
  { 1,-1, 1, 0,-1, 0, 0, 0,-1,-2,-3,-1,-2,-2,-1, 4, 2,-4,-2,-1, 0, 0, 0,-5},
  { 0,-1, 0,-1,-1,-1,-1,-2,-2,-1,-1,-1,-1,-1,-1, 2, 5,-3,-1, 0, 0,-1, 0,-5},
  {-2,-2,-4,-4,-5,-2,-3,-2,-3,-2,-2,-2,-2, 1,-3,-4,-3,15, 3,-3,-4,-2,-2,-5},
  {-2,-1,-2,-2,-3,-1,-2,-3, 2, 0, 0,-1, 0, 3,-3,-2,-1, 3, 8,-1,-2,-2,-1,-5},
  { 0,-2,-3,-3,-1,-3,-3,-3,-3, 3, 1,-2, 1, 0,-3,-1, 0,-3,-1, 5,-3,-3,-1,-5},
  {-1,-1, 4, 5,-2, 0, 1,-1, 0,-3,-3, 0,-2,-3,-2, 0, 0,-4,-2,-3, 5, 2,-1,-5},
  {-1, 0, 0, 1,-3, 4, 4,-2, 0,-3,-2, 1,-1,-3,-1, 0,-1,-2,-2,-3, 2, 4,-1,-5},
  { 0,-1,-1,-1,-2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0,-2,-1,-1,-1,-1,-1,-5},
  {-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5, 1}
};


const int BLOSUM50[24][24] = {
  { 5,-2,-1,-2,-1,-1,-1, 0,-2,-1,-2,-1,-1,-3,-1, 1, 0,-3,-2, 0,-2,-1,-1,-5},
  {-2, 7,-1,-2,-4, 1, 0,-3, 0,-4,-3, 3,-2,-3,-3,-1,-1,-3,-1,-3,-1, 0,-1,-5},
  {-1,-1, 7, 2,-2, 0, 0, 0, 1,-3,-4, 0,-2,-4,-2, 1, 0,-4,-2,-3, 4, 0,-1,-5},
  {-2,-2, 2, 8,-4, 0, 2,-1,-1,-4,-4,-1,-4,-5,-1, 0,-1,-5,-3,-4, 5, 1,-1,-5},
  {-1,-4,-2,-4,13,-3,-3,-3,-3,-2,-2,-3,-2,-2,-4,-1,-1,-5,-3,-1,-3,-3,-2,-5},
  {-1, 1, 0, 0,-3, 7, 2,-2, 1,-3,-2, 2, 0,-4,-1, 0,-1,-1,-1,-3, 0, 4,-1,-5},
  {-1, 0, 0, 2,-3, 2, 6,-3, 0,-4,-3, 1,-2,-3,-1,-1,-1,-3,-2,-3, 1, 5,-1,-5},
  { 0,-3, 0,-1,-3,-2,-3, 8,-2,-4,-4,-2,-3,-4,-2, 0,-2,-3,-3,-4,-1,-2,-2,-5},
  {-2, 0, 1,-1,-3, 1, 0,-2,10,-4,-3, 0,-1,-1,-2,-1,-2,-3, 2,-4, 0, 0,-1,-5},
  {-1,-4,-3,-4,-2,-3,-4,-4,-4, 5, 2,-3, 2, 0,-3,-3,-1,-3,-1, 4,-4,-3,-1,-5},
  {-2,-3,-4,-4,-2,-2,-3,-4,-3, 2, 5,-3, 3, 1,-4,-3,-1,-2,-1, 1,-4,-3,-1,-5},
  {-1, 3, 0,-1,-3, 2, 1,-2, 0,-3,-3, 6,-2,-4,-1, 0,-1,-3,-2,-3, 0, 1,-1,-5},
  {-1,-2,-2,-4,-2, 0,-2,-3,-1, 2, 3,-2, 7, 0,-3,-2,-1,-1, 0, 1,-3,-1,-1,-5},
  {-3,-3,-4,-5,-2,-4,-3,-4,-1, 0, 1,-4, 0, 8,-4,-3,-2, 1, 4,-1,-4,-4,-2,-5},
  {-1,-3,-2,-1,-4,-1,-1,-2,-2,-3,-4,-1,-3,-4,10,-1,-1,-4,-3,-3,-2,-1,-2,-5},
  { 1,-1, 1, 0,-1, 0,-1, 0,-1,-3,-3, 0,-2,-3,-1, 5, 2,-4,-2,-2, 0, 0,-1,-5},
  { 0,-1, 0,-1,-1,-1,-1,-2,-2,-1,-1,-1,-1,-2,-1, 2, 5,-3,-2, 0, 0,-1, 0,-5},
  {-3,-3,-4,-5,-5,-1,-3,-3,-3,-3,-2,-3,-1, 1,-4,-4,-3,15, 2,-3,-5,-2,-3,-5},
  {-2,-1,-2,-3,-3,-1,-2,-3, 2,-1,-1,-2, 0, 4,-3,-2,-2, 2, 8,-1,-3,-2,-1,-5},
  { 0,-3,-3,-4,-1,-3,-3,-4,-4, 4, 1,-3, 1,-1,-3,-2, 0,-3,-1, 5,-4,-3,-1,-5},
  {-2,-1, 4, 5,-3, 0, 1,-1, 0,-4,-4, 0,-3,-4,-2, 0, 0,-5,-3,-4, 5, 2,-1,-5},
  {-1, 0, 0, 1,-3, 4, 5,-2, 0,-3,-3, 1,-1,-4,-1, 0,-1,-2,-2,-3, 2, 5,-1,-5},
  {-1,-1,-1,-1,-2,-1,-1,-2,-1,-1,-1,-1,-1,-2,-2,-1, 0,-3,-1,-1,-1,-1,-1,-5},
  {-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5,-5, 1}
};

const int BLOSUM80[24][24] = {
  { 7,-3,-3,-3,-1,-2,-2, 0,-3,-3,-3,-1,-2,-4,-1, 2, 0,-5,-4,-1,-3,-2,-1,-8},
  {-3, 9,-1,-3,-6, 1,-1,-4, 0,-5,-4, 3,-3,-5,-3,-2,-2,-5,-4,-4,-2, 0,-2,-8},
  {-3,-1, 9, 2,-5, 0,-1,-1, 1,-6,-6, 0,-4,-6,-4, 1, 0,-7,-4,-5, 5,-1,-2,-8},
  {-3,-3, 2,10,-7,-1, 2,-3,-2,-7,-7,-2,-6,-6,-3,-1,-2,-8,-6,-6, 6, 1,-3,-8},
  {-1,-6,-5,-7,13,-5,-7,-6,-7,-2,-3,-6,-3,-4,-6,-2,-2,-5,-5,-2,-6,-7,-4,-8},
  {-2, 1, 0,-1,-5, 9, 3,-4, 1,-5,-4, 2,-1,-5,-3,-1,-1,-4,-3,-4,-1, 5,-2,-8},
  {-2,-1,-1, 2,-7, 3, 8,-4, 0,-6,-6, 1,-4,-6,-2,-1,-2,-6,-5,-4, 1, 6,-2,-8},
  { 0,-4,-1,-3,-6,-4,-4, 9,-4,-7,-7,-3,-5,-6,-5,-1,-3,-6,-6,-6,-2,-4,-3,-8},
  {-3, 0, 1,-2,-7, 1, 0,-4,12,-6,-5,-1,-4,-2,-4,-2,-3,-4, 3,-5,-1, 0,-2,-8},
  {-3,-5,-6,-7,-2,-5,-6,-7,-6, 7, 2,-5, 2,-1,-5,-4,-2,-5,-3, 4,-6,-6,-2,-8},
  {-3,-4,-6,-7,-3,-4,-6,-7,-5, 2, 6,-4, 3, 0,-5,-4,-3,-4,-2, 1,-7,-5,-2,-8},
  {-1, 3, 0,-2,-6, 2, 1,-3,-1,-5,-4, 8,-3,-5,-2,-1,-1,-6,-4,-4,-1, 1,-2,-8},
  {-2,-3,-4,-6,-3,-1,-4,-5,-4, 2, 3,-3, 9, 0,-4,-3,-1,-3,-3, 1,-5,-3,-2,-8},
  {-4,-5,-6,-6,-4,-5,-6,-6,-2,-1, 0,-5, 0,10,-6,-4,-4, 0, 4,-2,-6,-6,-3,-8},
  {-1,-3,-4,-3,-6,-3,-2,-5,-4,-5,-5,-2,-4,-6,12,-2,-3,-7,-6,-4,-4,-2,-3,-8},
  { 2,-2, 1,-1,-2,-1,-1,-1,-2,-4,-4,-1,-3,-4,-2, 7, 2,-6,-3,-3, 0,-1,-1,-8},
  { 0,-2, 0,-2,-2,-1,-2,-3,-3,-2,-3,-1,-1,-4,-3, 2, 8,-5,-3, 0,-1,-2,-1,-8},
  {-5,-5,-7,-8,-5,-4,-6,-6,-4,-5,-4,-6,-3, 0,-7,-6,-5,16, 3,-5,-8,-5,-5,-8},
  {-4,-4,-4,-6,-5,-3,-5,-6, 3,-3,-2,-4,-3, 4,-6,-3,-3, 3,11,-3,-5,-4,-3,-8},
  {-1,-4,-5,-6,-2,-4,-4,-6,-5, 4, 1,-4, 1,-2,-4,-3, 0,-5,-3, 7,-6,-4,-2,-8},
  {-3,-2, 5, 6,-6,-1, 1,-2,-1,-6,-7,-1,-5,-6,-4, 0,-1,-8,-5,-6, 6, 0,-3,-8},
  {-2, 0,-1, 1,-7, 5, 6,-4, 0,-6,-5, 1,-3,-6,-2,-1,-2,-5,-4,-4, 0, 6,-2,-8},
  {-1,-2,-2,-3,-4,-2,-2,-3,-2,-2,-2,-2,-2,-3,-3,-1,-1,-5,-3,-2,-3,-2,-2,-8},
  {-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8,-8, 1}
};


const int BLOSUM90[24][24] = {
  {5,-2,-2,-3,-1,-1,-1, 0,-2,-2,-2,-1,-2,-3,-1, 1, 0,-4,-3,-1,-2,-1,-1,-6},
  {-2, 6,-1,-3,-5, 1,-1,-3, 0,-4,-3, 2,-2,-4,-3,-1,-2,-4,-3,-3,-2, 0,-2,-6},
  {-2,-1, 7, 1,-4, 0,-1,-1, 0,-4,-4, 0,-3,-4,-3, 0, 0,-5,-3,-4, 4,-1,-2,-6},
  {-3,-3, 1, 7,-5,-1, 1,-2,-2,-5,-5,-1,-4,-5,-3,-1,-2,-6,-4,-5, 4, 0,-2,-6},
  {-1,-5,-4,-5, 9,-4,-6,-4,-5,-2,-2,-4,-2,-3,-4,-2,-2,-4,-4,-2,-4,-5,-3,-6},
  {-1, 1, 0,-1,-4, 7, 2,-3, 1,-4,-3, 1, 0,-4,-2,-1,-1,-3,-3,-3,-1, 4,-1,-6},
  {-1,-1,-1, 1,-6, 2, 6,-3,-1,-4,-4, 0,-3,-5,-2,-1,-1,-5,-4,-3, 0, 4,-2,-6},
  { 0,-3,-1,-2,-4,-3,-3, 6,-3,-5,-5,-2,-4,-5,-3,-1,-3,-4,-5,-5,-2,-3,-2,-6},
  {-2, 0, 0,-2,-5, 1,-1,-3, 8,-4,-4,-1,-3,-2,-3,-2,-2,-3, 1,-4,-1, 0,-2,-6},
  {-2,-4,-4,-5,-2,-4,-4,-5,-4, 5, 1,-4, 1,-1,-4,-3,-1,-4,-2, 3,-5,-4,-2,-6},
  {-2,-3,-4,-5,-2,-3,-4,-5,-4, 1, 5,-3, 2, 0,-4,-3,-2,-3,-2, 0,-5,-4,-2,-6},
  {-1, 2, 0,-1,-4, 1, 0,-2,-1,-4,-3, 6,-2,-4,-2,-1,-1,-5,-3,-3,-1, 1,-1,-6},
  {-2,-2,-3,-4,-2, 0,-3,-4,-3, 1, 2,-2, 7,-1,-3,-2,-1,-2,-2, 0,-4,-2,-1,-6},
  {-3,-4,-4,-5,-3,-4,-5,-5,-2,-1, 0,-4,-1, 7,-4,-3,-3, 0, 3,-2,-4,-4,-2,-6},
  {-1,-3,-3,-3,-4,-2,-2,-3,-3,-4,-4,-2,-3,-4, 8,-2,-2,-5,-4,-3,-3,-2,-2,-6},
  { 1,-1, 0,-1,-2,-1,-1,-1,-2,-3,-3,-1,-2,-3,-2, 5, 1,-4,-3,-2, 0,-1,-1,-6},
  { 0,-2, 0,-2,-2,-1,-1,-3,-2,-1,-2,-1,-1,-3,-2, 1, 6,-4,-2,-1,-1,-1,-1,-6},
  {-4,-4,-5,-6,-4,-3,-5,-4,-3,-4,-3,-5,-2, 0,-5,-4,-4,11, 2,-3,-6,-4,-3,-6},
  {-3,-3,-3,-4,-4,-3,-4,-5, 1,-2,-2,-3,-2, 3,-4,-3,-2, 2, 8,-3,-4,-3,-2,-6},
  {-1,-3,-4,-5,-2,-3,-3,-5,-4, 3, 0,-3, 0,-2,-3,-2,-1,-3,-3, 5,-4,-3,-2,-6},
  {-2,-2, 4, 4,-4,-1, 0,-2,-1,-5,-5,-1,-4,-4,-3, 0,-1,-6,-4,-4, 4, 0,-2,-6},
  {-1, 0,-1, 0,-5, 4, 4,-3, 0,-4,-4, 1,-2,-4,-2,-1,-1,-4,-3,-3, 0, 4,-1,-6},
  {-1,-2,-2,-2,-3,-1,-2,-2,-2,-2,-2,-1,-1,-2,-2,-1,-1,-3,-2,-2,-2,-1,-2,-6},
  {-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6, 1}
};

const int BLOSUM100[24][24] = {
  { 8,-3,-4,-5,-2,-2,-3,-1,-4,-4,-4,-2,-3,-5,-2, 1,-1,-6,-5,-2,-4,-2,-2,-10},
  {-3,10,-2,-5,-8, 0,-2,-6,-1,-7,-6, 3,-4,-6,-5,-3,-3,-7,-5,-6,-4,-1,-3,-10},
  {-4,-2,11, 1,-5,-1,-2,-2, 0,-7,-7,-1,-5,-7,-5, 0,-1,-8,-5,-7, 5,-2,-3,-10},
  {-5,-5, 1,10,-8,-2, 2,-4,-3,-8,-8,-3,-8,-8,-5,-2,-4,-10,-7,-8, 6, 0,-4,-10},
  {-2,-8,-5,-8,14,-7,-9,-7,-8,-3,-5,-8,-4,-4,-8,-3,-3,-7,-6,-3,-7,-8,-5,-10},
  {-2, 0,-1,-2,-7,11, 2,-5, 1,-6,-5, 2,-2,-6,-4,-2,-3,-5,-4,-5,-2, 5,-2,-10},
  {-3,-2,-2, 2,-9, 2,10,-6,-2,-7,-7, 0,-5,-8,-4,-2,-3,-8,-7,-5, 0, 7,-3,-10},
  {-1,-6,-2,-4,-7,-5,-6, 9,-6,-9,-8,-5,-7,-8,-6,-2,-5,-7,-8,-8,-3,-5,-4,-10},
  {-4,-1, 0,-3,-8, 1,-2,-6,13,-7,-6,-3,-5,-4,-5,-3,-4,-5, 1,-7,-2,-1,-4,-10},
  {-4,-7,-7,-8,-3,-6,-7,-9,-7, 8, 2,-6, 1,-2,-7,-5,-3,-6,-4, 4,-8,-7,-3,-10},
  {-4,-6,-7,-8,-5,-5,-7,-8,-6, 2, 8,-6, 3, 0,-7,-6,-4,-5,-4, 0,-8,-6,-3,-10},
  {-2, 3,-1,-3,-8, 2, 0,-5,-3,-6,-6,10,-4,-6,-3,-2,-3,-8,-5,-5,-2, 0,-3,-10},
  {-3,-4,-5,-8,-4,-2,-5,-7,-5, 1, 3,-4,12,-1,-5,-4,-2,-4,-5, 0,-7,-4,-3,-10},
  {-5,-6,-7,-8,-4,-6,-8,-8,-4,-2, 0,-6,-1,11,-7,-5,-5, 0, 4,-3,-7,-7,-4,-10},
  {-2,-5,-5,-5,-8,-4,-4,-6,-5,-7,-7,-3,-5,-7,12,-3,-4,-8,-7,-6,-5,-4,-4,-10},
  { 1,-3, 0,-2,-3,-2,-2,-2,-3,-5,-6,-2,-4,-5,-3, 9, 2,-7,-5,-4,-1,-2,-2,-10},
  {-1,-3,-1,-4,-3,-3,-3,-5,-4,-3,-4,-3,-2,-5,-4, 2, 9,-7,-5,-1,-2,-3,-2,-10},
  {-6,-7,-8,-10,-7,-5,-8,-7,-5,-6,-5,-8,-4, 0,-8,-7,-7,17, 2,-5,-9,-7,-6,-10},
  {-5,-5,-5,-7,-6,-4,-7,-8, 1,-4,-4,-5,-5, 4,-7,-5,-5, 2,12,-5,-6,-6,-4,-10},
  {-2,-6,-7,-8,-3,-5,-5,-8,-7, 4, 0,-5, 0,-3,-6,-4,-1,-5,-5, 8,-7,-5,-3,-10},
  {-4,-4, 5, 6,-7,-2, 0,-3,-2,-8,-8,-2,-7,-7,-5,-1,-2,-9,-6,-7, 6, 0,-4,-10},
  {-2,-1,-2, 0,-8, 5, 7,-5,-1,-7,-6, 0,-4,-7,-4,-2,-3,-7,-6,-5, 0, 6,-2,-10},
  {-2,-3,-3,-4,-5,-2,-3,-4,-4,-3,-3,-3,-3,-4,-4,-2,-2,-6,-4,-3,-4,-2,-3,-10},
  {-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,1}
};


// Updated selection function
const int (*getSubstitutionMatrix(std::string matrixName))[24] {
  if (matrixName == "BLOSUM62") {
    return BLOSUM62;
  } else if (matrixName == "BLOSUM50") {
    return BLOSUM50;
  } else if (matrixName == "BLOSUM45") {
    return BLOSUM45;
  } else if (matrixName == "BLOSUM80") {
    return BLOSUM80;
  } else if (matrixName == "BLOSUM90") {
    return BLOSUM90;
  } else if (matrixName == "BLOSUM100") {
    return BLOSUM100;
  } else {
    Rcpp::stop("Invalid substitution matrix name: %s", matrixName);
  }
}

// Needleman-Wunsch algorithm to calculate similarity
double calculate_similarity(const string &sequence1, const string &sequence2,
                            const int substitutionMatrix[24][24],
                                                            int gapOpen = 10, int gapExt = 4) {
  size_t m = sequence1.size();
  size_t n = sequence2.size();
  
  // Initialize matrices
  vector<vector<int>> M(m + 1, vector<int>(n + 1, std::numeric_limits<int>::min() / 2));
  vector<vector<int>> Ix(m + 1, vector<int>(n + 1, std::numeric_limits<int>::min() / 2));
  vector<vector<int>> Iy(m + 1, vector<int>(n + 1, std::numeric_limits<int>::min() / 2));
  vector<vector<char>> traceback(m + 1, vector<char>(n + 1, '0'));
  
  // Initialize first row and column
  M[0][0] = 0;
  Ix[0][0] = Iy[0][0] = std::numeric_limits<int>::min() / 2;
  for (size_t i = 1; i <= m; ++i) {
    M[i][0] = std::numeric_limits<int>::min() / 2;
    Ix[i][0] = -gapOpen - (i - 1) * gapExt;
    Iy[i][0] = std::numeric_limits<int>::min() / 2;
    traceback[i][0] = 'U'; // Up
  }
  for (size_t j = 1; j <= n; ++j) {
    M[0][j] = std::numeric_limits<int>::min() / 2;
    Ix[0][j] = std::numeric_limits<int>::min() / 2;
    Iy[0][j] = -gapOpen - (j - 1) * gapExt;
    traceback[0][j] = 'L'; // Left
  }
  
  // Fill matrices
  for (size_t i = 1; i <= m; ++i) {
    char aa1 = sequence1[i - 1];
    auto it1 = aa_to_index.find(aa1);
    if (it1 == aa_to_index.end()) {
      Rcpp::stop("Invalid amino acid in sequence1: %c", aa1);
    }
    int index1 = it1->second;
    for (size_t j = 1; j <= n; ++j) {
      char aa2 = sequence2[j - 1];
      auto it2 = aa_to_index.find(aa2);
      if (it2 == aa_to_index.end()) {
        Rcpp::stop("Invalid amino acid in sequence2: %c", aa2);
      }
      int index2 = it2->second;
      int score = substitutionMatrix[index1][index2];
      
      // Compute Ix[i][j]
      int gapOpenX = M[i - 1][j] - (gapOpen + gapExt);
      int gapExtX = Ix[i - 1][j] - gapExt;
      Ix[i][j] = std::max(gapOpenX, gapExtX);
      
      // Compute Iy[i][j]
      int gapOpenY = M[i][j - 1] - (gapOpen + gapExt);
      int gapExtY = Iy[i][j - 1] - gapExt;
      Iy[i][j] = std::max(gapOpenY, gapExtY);
      
      // Compute M[i][j]
      int diagM = M[i - 1][j - 1] + score;
      int diagIx = Ix[i - 1][j - 1] + score;
      int diagIy = Iy[i - 1][j - 1] + score;
      M[i][j] = std::max({ diagM, diagIx, diagIy });
      
      // Traceback
      if (M[i][j] >= Ix[i][j] && M[i][j] >= Iy[i][j]) {
        traceback[i][j] = 'D'; // Diagonal
      } else if (Ix[i][j] >= Iy[i][j]) {
        M[i][j] = Ix[i][j];
        traceback[i][j] = 'U'; // Up
      } else {
        M[i][j] = Iy[i][j];
        traceback[i][j] = 'L'; // Left
      }
    }
  }
  
  // Backtracking to count matches and alignment length
  int matches = 0;
  int alignment_length = 0;
  size_t i = m;
  size_t j = n;
  
  while (i > 0 || j > 0) {
    char tb = traceback[i][j];
    if (tb == 'D') {
      // Diagonal move
      char a = sequence1[i - 1];
      char b = sequence2[j - 1];
      if (a == b) {
        ++matches;
      }
      --i;
      --j;
    } else if (tb == 'U') {
      // Up move (gap in sequence2)
      --i;
    } else { // 'L'
      // Left move (gap in sequence1)
      --j;
    }
    ++alignment_length;
  }
  
  // Calculate similarity as number of matches divided by alignment length
  double similarity = static_cast<double>(matches) / alignment_length;
  return similarity;
}

// [[Rcpp::export]]
NumericMatrix calculateSimilarityMatrix(CharacterVector sequences,
                                        std::string matrixName = "BLOSUM62",
                                        int gapOpen = 10, int gapExt = 4) {
  size_t n = sequences.length();
  NumericMatrix similarityMatrix(n, n);
  
  // Get the substitution matrix
  const int (*substitutionMatrix)[24] = getSubstitutionMatrix(matrixName);
  // Calculate pairwise similarities
  for (size_t i = 0; i < n; ++i) {
    string seq1 = as<string>(sequences[i]);
    for (size_t j = i; j < n; ++j) {
      string seq2 = as<string>(sequences[j]);
      
      // Get similarity
      double similarity = calculate_similarity(seq1, seq2, substitutionMatrix, gapOpen, gapExt);
      
      // Assign similarity to matrix
      similarityMatrix(i, j) = similarity;
      similarityMatrix(j, i) = similarity; // Symmetric
    }
  }
  
  // Add dimension names
  // Create numeric labels 1,2,3...
  CharacterVector labels(n);
  for(size_t i = 0; i < n; ++i) {
    labels[i] = std::to_string(i + 1);
  }
  
  // Add dimension names using numeric labels
  similarityMatrix.attr("dimnames") = List::create(labels, labels);
  
  return similarityMatrix;
}