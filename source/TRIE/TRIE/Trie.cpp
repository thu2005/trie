#include "all.h"

long long count_compare = 0;

// Anh xa ki tu thanh chi so de de dang tim kiem
int charToIndex(char c) {
    if (isalpha(c)) return tolower(c) - 'a';   // Chu cai: 0 -> 25
    if (isdigit(c)) return 26 + (c - '0');     // Chu so: 26 -> 35
    // Ký tự đặc biệt: 36 tro di
    //string specialChars = "!@#$%^&*()_-+={}[]:;\"'<>,.?/~| ";
    string specialChars = "!&-',./ ";
    size_t pos = specialChars.find(c);
    if (pos != string::npos)    
        return 36 + static_cast<int>(pos);
    return -1;
}


TrieNode* createNode() {
    TrieNode* a = new TrieNode;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        a->children[i] = NULL;
    }
    a->isEndOfWord = false;
    return a;
}

void Release(TrieNode*& root) {
    if (root == NULL)
        return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i])
            Release(root->children[i]);
    }
    delete root;
    root = NULL;
}


void insert(TrieNode* root, const string& word) {
    TrieNode* p = root;
    for (char c : word) {
        int index = charToIndex(c);
        if (index == -1)
            return;
        if (!p->children[index]) {
            p->children[index] = createNode();
        }
        p = p->children[index];
    }
    p->isEndOfWord = true;
}

bool search(TrieNode* root, const string& word) {
    TrieNode* p = root;
    for (char c : word) {
        int index = charToIndex(c);
        if (index == -1)
            return false;
        if (!p->children[index])
            return false;
        p = p->children[index];
    }
    return p and p->isEndOfWord;
}

bool startWith(TrieNode* root, const string& prefix) {
    TrieNode* p = root;
    for (char c : prefix) {
        int index = charToIndex(c);
        if (!p->children[index])
            return false;
        p = p->children[index];
    }
    return true;
}

// Tim node tuong ung voi prefix
TrieNode* searchPrefix(TrieNode* root, const string& prefix) {
    TrieNode* p = root;
    for (char c : prefix) {
        ++count_compare;
        int index = charToIndex(c);
        if (++count_compare and index == -1)
            return NULL;
        if (++count_compare and !p->children[index])
            return NULL;
        p = p->children[index];
    }
    ++count_compare;
    return p;
}

// Duyet DFS thu thap tu khoa tu 1 nut
void collectWords(TrieNode* root, string word, vector<string>& res, int k) {
    if (++count_compare and root->isEndOfWord) {
        res.push_back(word);
        if (++count_compare and res.size() == k)
            return;
    }

    const string specialChars = "!&-',./ ";

    for (int i = 0; ++count_compare and i < ALPHABET_SIZE; i++) {
        if (++count_compare and root->children[i]) {
            char c;
            if (++count_compare and i < 26)
                c = 'a' + i;
            else if (++count_compare and i < 36)
                c = '0' + (i - 26);
            else
                c = specialChars[i - 36];

            collectWords(root->children[i], word + c, res, k);
            
            if (++count_compare and res.size() == k)
                return;
        }
    }
}


void suggestWords(TrieNode* root, const string& prefix, vector<string>& suggestion, int k) {
    TrieNode* p = searchPrefix(root, prefix);

    if (++count_compare and p == NULL) {
        suggestion.push_back("No suggestions found for prefix: " + prefix);
        return;
    }

    collectWords(p, prefix, suggestion, k);

    // Khong co phep so sanh vi cai nay chi phuc vu in man hinh
    if (suggestion.empty()) {
        suggestion.push_back("No suggestions found for prefix: " + prefix);
    }
}

double suggestWordsTime(TrieNode* root, const string& prefix, vector<string>& res, int k) {
    auto start = chrono::high_resolution_clock::now();

    suggestWords(root, prefix, res, k);

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, micro>duration = end - start;

    return duration.count();
}

void loadPrefixTest(const string& fileName, vector<string>& prefixTest) {
    fstream input;
    input.open(fileName, ios::in);

    if (!input.is_open()) {
        cout << "Cannot open this file! " << endl;
        return;
    }

    string pre;
    while (getline(input, pre)) {
        prefixTest.push_back(pre);
    }

    input.close();
}

double averageTime(TrieNode* root, const vector<string>& prefixTest, int k, vector<double>& timeEachPrefix, vector<long long>& cnt_cp) {
    double totalTime = 0;
    vector<string>res;

    fstream output;
    output.open("output.txt", ios::out);

    if (!output.is_open())
        return -1;

    output << "Prefix\tTime\tComparisons\n";

    for (const auto& x : prefixTest) {
        double totalTimeTemp = 0;
        count_compare = 0;

        for (int i = 0; i < 100; i++) {
            res.clear();
            totalTimeTemp += suggestWordsTime(root, x, res, k);
        }
        timeEachPrefix.push_back(totalTimeTemp / 100);
        cnt_cp.push_back(count_compare / 100);

        output << x << "\t"
            << fixed << setprecision(6) << totalTimeTemp / 100 << "\t"
            << count_compare / 100 << "\n";

        totalTime += totalTimeTemp / 100;
    }
    
    output << "Average time\t" << fixed << setprecision(6) << totalTime / prefixTest.size() << "\n";
    output.close();

    return totalTime / prefixTest.size();
}

string addNewWord(TrieNode* root, const string& word) {
    if (search(root, word))
        return "The word '" + word + "' already exists in the dictionary.";
    else {
        insert(root, word);
        return "The word '" + word + "' has been added to the dictionary.";
    }
}


bool deleteWordHelp(TrieNode* root, const string& word, int depth, bool& wordDeleted) {
    if (root == NULL)
        return false;

    if (depth == word.size()) {

        if (root->isEndOfWord == false) // Word nay chua het 1 tu
            return false;

        // Word nay la 1 tu
        // Cap nhat lai day khong con la ket thuc cua 1 tu
        root->isEndOfWord = false;
        wordDeleted = true;

        // Neu khong co con thi xoa luon
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (root->children[i])
                return false; // Co con, khong xoa
        }
        return true; // Khong co con, xoa
    }

    // Xu li ky tu tiep theo
    int index = charToIndex(word[depth]);
    if (deleteWordHelp(root->children[index], word, depth + 1, wordDeleted) == false)
        return false; // Word khong ton tai or co con

    delete root->children[index];
    root->children[index] = NULL;

    // Kiem tra node hien tai co con khong
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->children[i])
            return false; // Co con, khong xoa

    // Node nay khong phai ket thuc tu, xoa tiep
    return !root->isEndOfWord;

}

string deleteWord(TrieNode* root, const string& word) {
    bool wordDeleted = false;
    deleteWordHelp(root, word, 0, wordDeleted);

    if (wordDeleted)
        return "The word '" + word + "' has been successfully deleted.";
    else
        return "The word '" + word + "' does not exist in the dictionary.";

}



bool loadDictionary(TrieNode* root, const string& fileName) {
    fstream input;
    input.open(fileName, ios::in);
    if (!input.is_open()) {
        cout << "Could not open this file!" << endl;
        return false;
    }

    string word;
    while (getline(input, word)) {
        insert(root, word);
    }
    input.close();
    return true;
}



#if 0
int main() {
    TrieNode* root = createNode();
    loadDictionary(root, "words.txt");

    string word;
    int k;
    while (true) {
        system("cls");
        cout << "1. search word" << endl;
        cout << "2. search prefix" << endl;
        cout << "4. suggestions with prefix" << endl;
        cout << "5. add new word" << endl;
        cout << "6. delete word" << endl;
        cout << "3. exit" << endl;
        cin.ignore();
        cout << "Enter an option: "; cin >> k;
        if (k == 1) {
            cout << "Enter a word to search: ";
            cin >> word;

            if (search(root, word))
                cout << "The word exists in the dictionary." << endl;
            else
                cout << "The word does not exist in the dictionary." << endl;
        }
        else if (k == 2) {
            cout << "Enter a prefix to search: ";
            cin >> word;

            if (startWith(root, word))
                cout << "There are words with this prefix in the dictionary." << endl;
            else
                cout << "No words with this prefix found." << endl;
        }
        else if (k == 3)
            break;
        else if (k == 4) {
            cout << "Enter a prefix to suggest: ";
            cin >> word;

            suggestWords(root, word, 30);
        }
        else if (k == 5) {
            cout << "Enter a word to add: ";
            cin >> word;
            addNewWord(root, word);
        }
        else if (k == 6) {
            cout << "Enter a word to delete: ";
            cin >> word;
            deleteWord(root, word);
        }
        else
            cout << "Invalid!" << endl;
        system("pause");
    }
    Release(root);

    return 0;
}
#endif