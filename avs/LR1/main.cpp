#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

struct Number {         // структура числа
    int int_part;       // целая часть
    double frac_part;   // дробная часть
};

vector<string> read_file(ifstream& input);
bool convert_strs_to_double(const vector<string>& src, vector<double>& dst);
bool check_str(const string& str);
vector<Number> divide_int_frac(const vector<double>& numbers);
int convert_int_part(int number);
int precision(double& frac);
double convert_frac_part(double frac);
vector<double> convert_numbers(const vector<Number>& numbers);
bool output(const vector<string>& inpt_strs, const vector<double>& correct_numbers, const vector<double>& converted_numbers);

int main() 
{
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
    /*for (const string& str : inpt_strs) {
        cout << str << endl;
    }*/
    vector<double> correct_numbers;
    if (!convert_strs_to_double(inpt_strs, correct_numbers)) {
        cout << "Input file doesn't contain valid data!" << endl;
        return 1;
    }
    /*for (double number : correct_numbers) {
        cout << number << endl;
    }*/
    vector<Number> numbers_int_frac = divide_int_frac(correct_numbers);
    vector<double> converted_numbers = convert_numbers(numbers_int_frac);
    output(inpt_strs, correct_numbers, converted_numbers);
    return 0;
}

vector<string> read_file(ifstream& input)
{
    vector<string> result;
    for (string line; getline(input, line); ) {
        stringstream ss(line);
        for (string str; getline(ss, str, ' '); )
            result.push_back(str);
    }
    return result;
}

bool convert_strs_to_double(const vector<string>& src, vector<double>& dst)
{
    for (const string& str : src)
        if (check_str(str))
            dst.push_back(atof(str.c_str()));
    return dst.size() > 0;
}

bool check_str(const string& str)
{
    int k_dot = 0;    // количество точек
    int k_minus = 0;  // количество минусов
    int k_plus = 0;   // количество плюсов
    for (char ch : str) {
        if (ch == '.') k_dot++;
        else if (ch == '-' || ch == '+')
            if (ch != str[0])
                return false;
            else 
            {
                k_minus++; 
                k_plus++;
            }

        if (ch < '0' && ch != '.' && ch != '-' && ch != '+' || ch > '9' || k_dot > 1 || k_minus > 1 || k_plus > 1)
            return false;
    }
    return true;
}

vector<Number> divide_int_frac(const vector<double>& numbers) {
    vector<Number> result;
    for (double number : numbers) {
        if (number < 0) {
            number = abs(number);
        }
        Number tmp;
        double frac_part = number;
        while (frac_part >= 1) {
            frac_part--;
        }
        tmp = { int(number - frac_part), frac_part };
        result.push_back(tmp);
    }
    return result;
}

int convert_int_part(int number) {
    int result = 0;
    if (number < 3) {
        result = number;
    }
    else {
        vector<int> moduls;
        while (number > 2) {
            moduls.push_back(number % 3);
            number = number / 3;
        }
        moduls.push_back(number);
        int k = 0;
        for (int digit : moduls) {
            if (digit != 0) {
                result += digit * pow(10, k);
            }
            k++;
        }
    }
    return result;
}

int precision(double& frac) { 
    stringstream ss;
    ss << setprecision(6) << frac;
    string frac_str = ss.str();
    frac = atof(frac_str.c_str());
    return (frac_str.size() - 2) * 2 + 1;
}

double convert_frac_part(double frac) {
    double result = 0.0;
    if (frac > 0.0) {
        int k = precision(frac);
        int i = 1;
        while (i <= k) {
            frac = frac * 3;
            double tmp = 0.0;
            while (frac > 1.0) {
                frac--;
                tmp++;
            }
            if (tmp != 0.0)
                result += tmp * pow(10, -i);
            i++;
        }
    }
    return result;
}

vector<double> convert_numbers(const vector<Number>& numbers) {
    vector<double> result;
    for (const Number& number : numbers) {
        double int_part = convert_int_part(number.int_part);
        double frac_part = convert_frac_part(number.frac_part);
        result.push_back(int_part + frac_part);
    }
    return result;
}

bool output(const vector<string>& inpt_strs, const vector<double>& correct_numbers, const vector<double>& converted_numbers)
{
    ofstream out("output.txt", ios::out);
    if (!out.is_open()) {
        cout << "Output file can't be opened!" << endl;
        return false;
    }
    out << "User's input:" << endl;
    for (const string& s : inpt_strs)
        out << s << " ";
    out << endl << "Correct strings:" << endl;
    for (double d : correct_numbers)
        out << setprecision(15) << d << " ";
    out << endl << "Converted numbers:" << endl;
    for (int i = 0; i < converted_numbers.size(); i++) {
        double d = converted_numbers[i];
        if (correct_numbers[i] < 0)
            d = -d;
        out << setprecision(15) << d << " ";
    }
    out << endl;
}

