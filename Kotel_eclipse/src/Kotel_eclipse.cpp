//============================================================================
// Name        : Kotel_eclipse.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <cstdio>
#include <string>
#include <iostream>
#include <stack>
#include <vector>
using std::string;

class Rope {
	class Node {
	public:
		char key;
		long long sum;
		Node* left;
		Node* right;
		Node* parent;

		Node(char key, long long sum, Node* left, Node* right, Node* parent) :
				key(key), sum(sum), left(left), right(right), parent(parent) {
		}
	};
	std::string s;
	Node* root;
	std::vector<char> c;
public:

	Rope() {
		root = NULL;
		s = " ";
		c.clear();
	}
	Rope(const std::string &s) :
			s(s) {
		root = NULL;
		for (unsigned int w = 0; w < s.length(); w++) {
			Node* next = new Node(s[w], 1ll, NULL, NULL, NULL);
			root = merge(root, next);
		}
		result();
	}

	~Rope() {
		free_space(root);
	}

	void free_space(Node* root) {
		if (root != NULL) {
			if (root->left != NULL) {
				free_space(root->left);
			}
			if (root->right != NULL) {
				free_space(root->right);
			}
			delete root;
		}
	}
	void update(Node* v) {
		if (v == NULL)
			return;
		v->sum = 1 + (v->left != NULL ? v->left->sum : 0ll)
				+ (v->right != NULL ? v->right->sum : 0ll);
		if (v->left != NULL) {
			v->left->parent = v;
		}
		if (v->right != NULL) {
			v->right->parent = v;
		}
	}

	void small_rotation(Node* v) {
		Node* parent = v->parent;
		if (parent == NULL) {
			return;
		}
		Node* grandparent = v->parent->parent;
		if (parent->left == v) {
			Node* m = v->right;
			v->right = parent;
			parent->left = m;
		} else {
			Node* m = v->left;
			v->left = parent;
			parent->right = m;
		}
		update(parent);
		update(v);
		v->parent = grandparent;
		if (grandparent != NULL) {
			if (grandparent->left == parent) {
				grandparent->left = v;
			} else {
				grandparent->right = v;
			}
		}
	}

	void big_rotation(Node* v) {
		if (v->parent->left == v && v->parent->parent->left == v->parent) {

			small_rotation(v->parent);
			small_rotation(v);
		} else if (v->parent->right == v
				&& v->parent->parent->right == v->parent) {

			small_rotation(v->parent);
			small_rotation(v);
		} else {

			small_rotation(v);
			small_rotation(v);
		}
	}

	void splay(Node*& root, Node* v) {
		if (v == NULL)
			return;
		while (v->parent != NULL) {
			if (v->parent->parent == NULL) {
				small_rotation(v);
				break;
			}
			big_rotation(v);
		}
		root = v;
	}

	Node* find(Node*& root, int leftNum) {
		Node* v = root;

		while (v != NULL) {
			long long s = (v->left != NULL) ? v->left->sum : 0ll;
			if (leftNum == (s + 1)) {
				break;
			} else if (leftNum < (s + 1)) {
				v = v->left;
			} else if (leftNum > (s + 1)) {
				v = v->right;
				leftNum = leftNum - s - 1;
			}
		}
		splay(root, v);
		return v;
	}

	void split(Node* root, int key, Node*& left, Node*& right) {
		right = find(root, key);

		if (right == NULL) {
			left = root;
			return;
		}

		left = right->left;

		right->left = NULL;

		if (left != NULL) {
			left->parent = NULL;
		}

		update(left);
		update(right);
	}

	Node* merge(Node* left, Node* right) {
		if (left == NULL)
			return right;
		if (right == NULL)
			return left;
		Node* min_right = right;
		while (min_right->left != NULL) {
			min_right = min_right->left;
		}
		splay(right, min_right);
		right->left = left;
		update(right);
		return right;
	}

	void insert(Node*& root, int k, Node*& subString) {
		Node* left = NULL;
		Node* right = NULL;
		split(root, k, left, right);
		root = merge(merge(left, subString), right);
	}

//	std::string traversal_in_order(Node* root) const {
//		std::string printS = "";
//		if (root == NULL) {
//			return printS;
//		}
//		std::stack<Node*> S;
//		Node* p = root;
//
//		while (p != NULL) {
//			S.push(p);
//			p = p->left;
//		}
//
////		while (!S.empty()) {
//			p = S.top();
//			printS.push_back(p->key);
//			S.pop();
//			p = p->right;
//			while (p != NULL) {
//				S.push(p);
//				p = p->left;
//			}
//
//		}
//		return printS;
//	}

	std::vector<char> traversal_in_order(Node* root) const {
		std::vector<char> printS;
		printS.clear();
		if (root == NULL) {
			return printS;
		}
		std::stack<Node*> S;
		Node* p = root;

		while (p != NULL) {
			S.push(p);
			p = p->left;
		}

		while (!S.empty()) {
			p = S.top();
			printS.push_back(p->key);
			S.pop();
			p = p->right;
			while (p != NULL) {
				S.push(p);
				p = p->left;
			}

		}
		return printS;
	}

	void process(int i, int j, int k) {
		if (i > j) {
			int temp = i;
			i = j;
			j = temp;
		}
		Node* left = NULL;
		Node* right = NULL;
		Node* middle = NULL;
		split(root, i + 1, left, middle);
		split(middle, j - i + 2, middle, right);
		root = merge(left, right);
		insert(root, k + 1, middle);
	}

	std::vector<char> result() {
		c.clear();
		c = traversal_in_order(root);
		return c;
	}
	unsigned int length() const {
		return c.size(); //return result().length();
	}
	char index(const unsigned int k) const {
		return c.at(k); // return result().at(k);
	}
	std::string report(unsigned int e, unsigned int d) const {
		return std::string(c.begin() + e, c.begin() + d); //return result().substr(c, d);
	}
	static Rope concat(Rope& r1, Rope& r2) {
		Rope tmp;
		tmp.root->left = r1.root;
		tmp.root->right = r2.root;
		r1.root->parent = r2.root->parent = tmp.root;
		tmp.root->sum = r1.root->sum;

		tmp.root->key = ' ';
		return tmp;
	}
	std::string report_toend(unsigned int e) const {
		return std::string(c.begin() + e, c.end()); //return result().substr(c, d);
	}
	static std::pair<Rope, Rope> split(Rope& _r, const unsigned int k) {
		std::string tmp = _r.report(0, k);
		std::string tmp2 = _r.report_toend(k);
		std::pair<Rope, Rope> p;
		Rope r1(tmp);
		Rope r2(tmp2);
		p.first = r1;
		p.second = r2;
		_r = Rope();
		return p;

	}
};
class TextEditor {
	Rope* r;
	Rope* clipboard;
	unsigned int cursor = 0;
	int select_cursor = -1;
public:
	TextEditor() {
		r = new Rope();
		clipboard = new Rope();
	}
	TextEditor(std::string a) {
		r = new Rope(a);
		clipboard = new Rope();
	}
	unsigned int length() const {
		return r->length();
	}
	void stepLeft() {
		if (cursor > 0)
			cursor -= 1;
		select_cursor = -1;
	}
	void stepRight() {
		if (cursor < length())
			cursor += 1;
		select_cursor = -1;
	}
	void moveCursor(const unsigned int k) {
		if (k > 0 && k < length())
			cursor = k;
		select_cursor = -1;
	}
	void select(const unsigned int k) {
		select_cursor = k;
	}
	std::string report() const {
		if (select_cursor == -1)
			return "";
		return r->report(cursor, select_cursor);

	}
//	void insert(const std::string){
//		if (select_cursor == -1)
//
//	}
	void ctrlC() {
		if (select_cursor == -1)
			return;
		clipboard = new Rope(report());

	}
	void ctrlV() {
		if (select_cursor == -1)
			return;
	}
	void ctrlX() {
		if (select_cursor == -1)
			return;
	}
	void ctrlA() {
		cursor = 0;
		select_cursor = length();
	}
	void ctrlD() {
		if (select_cursor == -1)
			return;
	}

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

struct test_controller {
	bool enabled;
	std::string label;
	int points;
	bool ok;
	bool run;
	int &out;
	test_controller(bool enabled, std::string &&label, int points, int &out) :
			enabled(enabled), label(std::move(label)), points(points), ok(true), run(
					true), out(out) {
		if (enabled) {
			std::cout << ">>> Test: " << this->label << " (" << points << ")"
					<< std::endl;
		}
	}
	~test_controller() {
		if (enabled) {
			out += ok ? points : 0;
			std::cout << ">>> Result: " << (ok ? "PASS" : "FAIL") << std::endl;
		}
	}
};

struct measurement_controller {
	std::string name;
	std::chrono::nanoseconds &out;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	bool run;
	measurement_controller(std::string &&name, std::chrono::nanoseconds &out) :
			name(std::move(name)), out(out), start(
					std::chrono::high_resolution_clock::now()), run(true) {
	}
	~measurement_controller() {
		auto stop = std::chrono::high_resolution_clock::now();
		out = std::chrono::duration_cast<std::chrono::nanoseconds>(
				stop - start);
		std::cout << name << ": " << out.count() << "ns" << std::endl;
	}
};

#define TEST(label, points)                                                    \
  for (test_controller cntr(++counter == test_no || test_no == 0, label,       \
                            points, total);                                    \
       cntr.run; cntr.run = false)

#define CHECK_EQ(val, expr)                                                    \
  if (cntr.ok) {                                                               \
    try {                                                                      \
      cntr.ok = val == (expr);                                                 \
    } catch (...) {                                                            \
      cntr.ok = false;                                                         \
      std::cout << ">>> unexpected exception" << std::endl;                    \
    }                                                                          \
    if (!cntr.ok) {                                                            \
      std::cout << ">>> " << #val << " == " << #expr << " failed at "          \
                << __LINE__ << " Expr result: " << (expr) << std::endl;        \
    }                                                                          \
  }

int main(int argc, char *argv[]) {
	int test_no = 0;
	int counter = 0;
	int total = 0;
	srand(time(nullptr));
	if (argc == 2) {
		test_no = std::atoi(argv[1]);
	}

	TEST("Create an empty tree. Length should be 0.", 2)
	{
		Rope rope;
		CHECK_EQ(0, rope.length());
	}

	TEST("Create a non empty tree. Text should be the same.", 3)
	{
		Rope rope("The quick brown fox jumps over the lazy dog.");
		CHECK_EQ("The quick brown fox jumps over the lazy dog.",
				rope.report(0, 44));
	}

	TEST("Create two trees and check concat and split lengths.", 10)
	{
		Rope r1("I would like ");
		Rope r2("to concatenate two ropes.");

		Rope r3 = Rope::concat(r1, r2);
		CHECK_EQ(38, r3.length());
		CHECK_EQ(0, r1.length());
		CHECK_EQ(0, r2.length());

//    std::pair<Rope, Rope> splitted = Rope::split(r3, 13);
//    CHECK_EQ(0, r3.length());
//    CHECK_EQ(13, splitted.first.length());
//    CHECK_EQ(25, splitted.second.length());
	}

//  TEST("Create a TextEditor and add a few elements. Length should grow.", 5) {
//    TextEditor texteditor("The");
//    CHECK_EQ(3, texteditor.length());
//    texteditor.insert(" quick");
//    CHECK_EQ(9, texteditor.length());
//    texteditor.insert(" brown fox jumps over the lazy dog.");
//    CHECK_EQ(44, texteditor.length());
//  }
//
//  TEST("Create a TextEditor and add a few elements. Elements should be in the tree, should get them using ctrlF().", 5) {
//    TextEditor texteditor;
//    texteditor.insert("Adatszerkezetek");
//    texteditor.insert("Ês");
//    texteditor.insert("algoritmusok");
//    texteditor.insert(".:)");
//    CHECK_EQ(false, texteditor.ctrlF("Adat").empty());
//    CHECK_EQ(false, texteditor.ctrlF("ritmusok").empty());
//    CHECK_EQ(false, texteditor.ctrlF(":)").empty());
//    CHECK_EQ(true, texteditor.ctrlF("Analízis").empty());
//    CHECK_EQ(true, texteditor.ctrlF("Informåciótechnika Ês bionika fizikai alapjai I.").empty());
//    CHECK_EQ(true, texteditor.ctrlF(":(").empty());
//  }
//
//  TEST("Create a TextEditor. Test moving cursor, selection and report.", 5) {
//    TextEditor texteditor("Påzmåny PÊter Katolikus Egyetem, Informåciós Technológiai Ês Bionika Kar");
//    texteditor.moveCursor(0);
//    texteditor.select(7);
//    CHECK_EQ("Påzmåny", texteditor.report());
//    texteditor.stepLeft();
//    texteditor.stepRight();
//    CHECK_EQ("", texteditor.report());
//    texteditor.moveCursor(123456789);
//    texteditor.select(0);
//    CHECK_EQ("P", texteditor.report());
//  }
//
//  TEST("Test deletion. Size should shrink and text should disappear.", 5) {
//    TextEditor texteditor("TEst dEleTIon. sIZe SHouLd ShRInK.");
//    CHECK_EQ(false, texteditor.ctrlF("st ").empty());
//    texteditor.select(27);
//    texteditor.remove();
//    CHECK_EQ(27, texteditor.length());
//    CHECK_EQ(true, texteditor.ctrlF("ShRInk").empty());
//    texteditor.ctrlA();
//    CHECK_EQ("TEst dEleTIon. sIZe SHouLd ", texteditor.report());
//    texteditor.moveCursor(0);
//    texteditor.select(4);
//    texteditor.remove();
//    CHECK_EQ(23, texteditor.length());
//    CHECK_EQ(true, texteditor.ctrlF("TEst").empty());
//    texteditor.ctrlA();
//    CHECK_EQ(" dEleTIon. sIZe SHouLd ", texteditor.report());
//    texteditor.ctrlA();
//    texteditor.remove();
//    CHECK_EQ(0, texteditor.length());
//  }
//
//  TEST("Test more complicated text editing.", 15) {
//    TextEditor texteditor;
//    texteditor.insert("ez a håzi feladat.");
//    texteditor.moveCursor(0);
//    texteditor.insert("RemÊlem, hogy");
//    texteditor.insert(" nagyon rosszul ");
//    texteditor.insert("fog sikerßlni ");
//    vector<unsigned int> rosszulIndexek = texteditor.ctrlF("rosszul");
//    texteditor.moveCursor(rosszulIndexek[0]);
//    texteditor.select(rosszulIndexek[0]+7);
//    texteditor.insert("jól");
//    vector<unsigned int> haziIndexek = texteditor.ctrlF("håzi feladat");
//    texteditor.moveCursor(haziIndexek[0]);
//    texteditor.select(haziIndexek[0]+12);
//    texteditor.ctrlC();
//    texteditor.moveCursor(texteditor.length()-1);
//    texteditor.insert(" Ês az Üsszes tÜbbi ");
//    texteditor.ctrlV();
//    texteditor.insert(" is");
//    texteditor.ctrlA();
//    CHECK_EQ("RemÊlem, hogy nagyon jól fog sikerßlni ez a håzi feladat Ês az Üsszes tÜbbi håzi feladat is.", texteditor.report());
//    texteditor.ctrlX();
//    CHECK_EQ("", texteditor.report());
//    texteditor.ctrlV();
//    texteditor.ctrlA();
//    CHECK_EQ("RemÊlem, hogy nagyon jól fog sikerßlni ez a håzi feladat Ês az Üsszes tÜbbi håzi feladat is.", texteditor.report());
//  }

	std::cout << ">>> TOTAL POINTS: " << total << std::endl;
	return 0;
}


