#pragma once
#ifndef TRIE_H
#define TRIE_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "windows.h"
#include <chrono>
#include <iomanip>
#include <sstream>

extern long long count_compare;
using namespace std;

//===================INTERFACE=================
#define ID_EDIT_INPUT 101 
#define ID_TEXTBOX 101         
#define ID_BUTTON_SEARCH 102
#define ID_BUTTON_ADD 103
#define ID_BUTTON_DELETE 104
#define ID_LIST_RESULTS 105
#define ID_LISTBOX 105
#define ID_BUTTON_CHECK 106
#define ID_BUTTON_TIME 107


void AddControls(HWND hwnd);
void HandleSearchButton(HWND hwnd);
void HandleAddButton(HWND hwnd);
void HandleDeleteButton(HWND hwnd);
void DisplayMessage(HWND hwnd, LPCWSTR message, LPCWSTR title);



// So luong ky tu: Chu cai Latin(26) + Chu so(10) + Ky tu dac biet(vd:32)
const int ALPHABET_SIZE = 26 + 10 + 8;

struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;
};
int charToIndex(char c);
TrieNode* createNode();
void insert(TrieNode* root, const string& word);
bool search(TrieNode* root, const string& word);
bool startWith(TrieNode* root, const string& prefix);
TrieNode* searchPrefix(TrieNode* root, const string& prefix);
void collectWords(TrieNode* root, string word, vector<string>& res, int k);
void suggestWords(TrieNode* root, const string& prefix, vector<string>& suggestion, int k);
double suggestWordsTime(TrieNode* root, const string& prefix, vector<string>& res, int k);
void loadPrefixTest(const string& fileName, vector<string>& prefixTest);
double averageTime(TrieNode* root, const vector<string>& prefixTest, int k, vector<double>& timeEachPrefix, vector<long long>& cnt_cp);
string addNewWord(TrieNode* root, const string& word);
bool deleteWordHelp(TrieNode* root, const string& word, int depth, bool& wordDeleted);
string deleteWord(TrieNode* root, const string& word);
bool loadDictionary(TrieNode* root, const string& fileName);
void Release(TrieNode*& root);




#endif

