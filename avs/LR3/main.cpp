// Разработать алгоритм и создать программу, реализующую
// построения таблицы истинности для логической функции до четырех
// аргументов.
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <sstream>
#include "operations.h"

using namespace std;

const vector<uint8_t> values_1 = { 0, 1 };

const vector<vector<uint8_t>> values_2 = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };

const vector<vector<uint8_t>> values_3 = { {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1}, 
									       {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1} };

const vector<vector<uint8_t>> values_4 = { {0, 0, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}, {0, 0, 1, 1},
										   {0, 1, 0, 0}, {0, 1, 0, 1}, {0, 1, 1, 0}, {0, 1, 1, 1},
										   {1, 0, 0, 0}, {1, 0, 0, 1}, {1, 0, 1, 0}, {1, 0, 1, 1},
										   {1, 1, 0, 0}, {1, 1, 0, 1}, {1, 1, 1, 0}, {1, 1, 1, 1} };

int priority(char op);
int variables(char var, int x1, int x2, int x3, int x4);
int operations(char op, int l, int r);
bool correct_symbol(char c);
bool correct_string(string s);
bool correct_brackets(string s);
int number_vars(string s);
vector<string> read_file(ifstream& input);
bool result_on_set(string s, int x1, int x2, int x3, int x4);
string to_postfix(string s);
void print_result(ofstream& out, string s);

int main() {
	ifstream in("input.txt", ios::in);
	if (!in.is_open()) {
		cout << "Input file can't be opened!" << endl;
		return 1;
	}
	else if (in.peek() == EOF) {
		cout << "Input file is empty!" << endl;
		return 1;
	}
	vector<string> inpt_strs = read_file(in);

	ofstream out("output.txt", ios::out);
	if (!out.is_open()) {
		cout << "Output file can't be opened!" << endl;
		return 1;
	}

	for (string s : inpt_strs) {
		print_result(out, s);
	}

	return 0;
}

int priority(char op)
{
	switch (op) {
		case '(': return 0;
		case '|': return 1; // стрелка Пирса
		case '/': return 2; // штрих Шеффера
		case '=': return 3; // эквивалентность
		case '-': return 4; // импликация
		case '^': return 4; // исключающее ИЛИ
		case 'v': return 5; 
		case '&': return 6;
		case '!': return 7;
	}
}

int operations(char op, int l, int r) 
{
	switch (op) {
		case '|': return PIRS(l, r); // стрелка Пирса
		case '/': return SHEFF(l, r); // штрих Шеффера
		case '=': return EQ(l, r); // эквивалентность
		case '-': return IMPL(l, r); // импликация
		case '^': return XOR(l, r); // исключающее ИЛИ
		case 'v': return OR(l, r);
		case '&': return AND(l, r);
	}
}

int variables(char var, int x1, int x2, int x3, int x4) {
	switch (var) {
		case 'a': return x1;
		case 'b': return x2;
		case 'c': return x3;
		case 'd': return x4;
	}
}

bool correct_symbol(char c)
{
	if (c == '(' || c == ')' || c == 'a' || c == 'b' || c == 'c' || c == 'd' || 
		c == '/' || c == '|' || c == 'v' || c == '&' || c == '^' || c == '=' ||	
		c == '!' || c == '-')
		return true;
	return false;
}

bool correct_string(string s)
{
	for (char c : s)
		if (!correct_symbol(c))
			return false;
	return true;
}

int number_vars(string s) 
{
	int num = 0;
	if (s.find_first_of('a') != s.npos) num++;
	if (s.find_first_of('b') != s.npos) num++;
	if (s.find_first_of('c') != s.npos) num++;
	if (s.find_first_of('d') != s.npos) num++;
	return num;
}

bool correct_brackets(string s)
{
	int balance = 0;
	for (char c : s) 
		if (c == '(') balance++;
		else if (c == ')') balance--;
	return (balance == 0);
}

vector<string> read_file(ifstream& input) {
	vector<string> result;
	for (string line; getline(input, line); ) {
		result.push_back(line);
	}
	return result;
}

string to_postfix(string s) {
	stack<char> st;
	string output = "";
	for (char c : s) {
		if (c == '(') {
			st.push(c);
		}
		else if (c == ')') {
			while (st.top() != '(') {
				output += st.top();
				st.pop();
			}
			st.pop();
		}
		else if (c == 'a' || c == 'b' || c == 'c' || c == 'd') {
			output += c;
		}
		else {
			while (!st.empty() && priority(st.top()) >= priority(c)) {
				output += st.top();
				st.pop();
			}
			st.push(c);
		}
	}
	while (!st.empty()) {
		output += st.top();
		st.pop();
	}
	return output;
}

bool result_on_set(string s, int x1, int x2, int x3, int x4) {
	stack<int> st;
	int k1, k2;
	for (char c : s) {
		if (c == 'a' || c == 'b' || c == 'c' || c == 'd') {
			st.push(variables(c, x1, x2, x3, x4));
		}
		else if (c == '!') {
			k1 = st.top();
			st.pop();
			st.push(NEG(k1));
		}
		else {
			k1 = st.top();
			st.pop();
			k2 = st.top();
			st.pop();
			st.push(operations(c, k2, k1));
		}
	}
	return st.top();
}

void print_result(ofstream& out, string s)
{
	out << "-----------------------------------------------------------------------" << endl;
	out << "Input string: " << s << endl;
	if (!correct_string(s)) {
		out << "Incorrect symbols input string!" << endl << endl;
		return;
	}
	if (!correct_brackets(s)) {
		out << "Incorrect brackets' sequence input string!" << endl << endl;
		return;
	}
	int num_vars = number_vars(s);

	if (num_vars == 0) {
		out << "There are not variables input string!" << endl << endl;
		return;
	}

	string to_postfx = to_postfix(s);
	out << "Postfix notation: " << to_postfx << endl;

	if (num_vars == 1) {
		out << "|a|" << "\n" << "(" << (int)values_1[0] << ") -> " << (int)values_1[0] << "\n"
			<< "(" << (int)values_1[1] << ") -> " << (int)values_1[1] << "\n";
	}
	else if (num_vars == 2) {
		out << "|a, b|" << endl;
		for (auto value : values_2) {
			out << "(" << (int)value[0] << ", " << (int)value[1] << ") -> "
				<< result_on_set(to_postfx, (int)value[0], (int)value[1], 0, 0) << endl;
		}
	}
	else if (num_vars == 3) {
		out << "|a, b, c|" << endl;
		for (auto value : values_3) {
			out << "(" << (int)value[0] << ", " << (int)value[1] << ", " << (int)value[2] << ") -> "
				<< result_on_set(to_postfx, (int)value[0], (int)value[1], (int)value[2], 0) << endl;
		}
	}
	else {
		out << "|a, b, c, d|" << endl;
		for (auto value : values_4) {
			out << "(" << (int)value[0] << ", " << (int)value[1] << ", " << (int)value[2] << ", " << (int)value[3] << ") -> "
				<< result_on_set(to_postfx, (int)value[0], (int)value[1], (int)value[2], (int)value[3]) << endl;
		}
	}

	out << endl;
}