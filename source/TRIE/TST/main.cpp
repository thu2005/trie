#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>

using namespace std;

long long count_compare = 0;

struct TSTNode {
	char data;
	bool isEndOfWord;
	TSTNode* left, * mid, * right;
};

TSTNode* createNode(char data) {
	TSTNode* a = new TSTNode;
	a->data = data;
	a->isEndOfWord = false;
	a->left = NULL;
	a->right = NULL;
	a->mid = NULL;

	return a;
}

TSTNode* insert(TSTNode*& root, const string& word, int index = 0) {
	if (root == NULL) {
		root = createNode(word[index]);
	}

	if (word[index] < root->data)
		root->left = insert(root->left, word, index);
	else if (word[index] > root->data)
		root->right = insert(root->right, word, index);
	else {
		if (index + 1 < word.size())
			root->mid = insert(root->mid, word, index + 1);
		else
			root->isEndOfWord = true;
	}

	return root;
}

void collectWords(TSTNode* root, const string& prefix, vector<string>& res, int k) {
	if ((++count_compare and root == NULL) or (++count_compare and res.size() >= k))
		return;

	collectWords(root->left, prefix, res, k);

	if (++count_compare and root->isEndOfWord) {
		res.push_back(prefix + root->data);
		if (++count_compare and res.size() >= k)
			return;
	}

	collectWords(root->mid, prefix + root->data, res, k);

	collectWords(root->right, prefix, res, k);
}

void autocomplete(TSTNode* root, const string& prefix, vector<string>&res, int index, int k) {
	if ((++count_compare and root == NULL) or (++count_compare and res.size() >= k))
		return;

	if (++count_compare and prefix[index] < root->data)
		autocomplete(root->left, prefix, res, index, k);
	else if (++count_compare and prefix[index] > root->data)
		autocomplete(root->right, prefix, res, index, k);
	else {
		if (++count_compare and index + 1 < prefix.size())
			autocomplete(root->mid, prefix, res, index + 1, k);
		else {
			if (++count_compare and root->isEndOfWord) {
				res.push_back(prefix);
				if (++count_compare and res.size() >= k)
					return;
			}
			collectWords(root->mid, prefix, res, k);
		}
	}
}

double autocompleteTime(TSTNode* root, const string& prefix, vector<string>& res, int k) {
	auto start = chrono::high_resolution_clock::now();

	autocomplete(root, prefix, res, 0, k);

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

double averageTime(TSTNode* root, const vector<string>& prefixTest, int k, vector<double>& timeEachPrefix, vector<long long>& cnt_cp) {
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
			totalTimeTemp += autocompleteTime(root, x, res, k);
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

void deleteTST(TSTNode*& root) {
	if (root == NULL)
		return;
	deleteTST(root->left);
	deleteTST(root->mid);
	deleteTST(root->right);
	delete root;
	root = NULL;
}

void loadDictionary(TSTNode*& root, const string& fileName) {
	fstream input;
	input.open(fileName, ios::in);

	if (!input.is_open()) {
		cout << "Cannot open this file! " << endl;
		return;
	}

	string word;
	while (getline(input, word)) {
		insert(root, word, 0);
	}
	input.close();
}

int main() {
	TSTNode* root = NULL;

	loadDictionary(root, "words.txt");

	//cout << root->data << endl;

#if 1
	int cnt = 1; int k = 150;
	
	vector<string>prefixTest;
	loadPrefixTest("prefixTest3.txt", prefixTest);

	cout << prefixTest.size() << endl;

	while (cnt) {
		string pre; vector<string>res;
		vector<double>timeEachPrefix;
		vector<long long> cnt_cp;
		count_compare = 0;

		double time = averageTime(root, prefixTest, k, timeEachPrefix, cnt_cp);

		cout << "Time average: " << time << " microseconds " << endl;

		for (int i = 0; i < prefixTest.size(); i++) {
			cout << prefixTest[i] << "      " << timeEachPrefix[i] << "        " << cnt_cp[i] << endl;
		}

		res.clear();

		cout << "enter prefix: "; cin >> pre;
		
		cout << "Time: " << autocompleteTime(root, pre, res, k) << endl;

		cout << "Prefix found: " << endl;
		cout << res.size() << endl;

		int i = 1;
		for (auto x : res) {
			cout << i << ": ";
			cout << x << endl;
			i++;
		}

		cout << "contine: ?";
		cin >> cnt;
	}
#endif

	//cout << root->data << endl <<  root->mid->data << endl << root->right->data << endl;
	deleteTST(root);

	return 0;
}