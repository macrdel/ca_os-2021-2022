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
vector<Number> convert_numbers(const vector<Number>& numbers);
vector<double> summation(const vector<Number>& converted_numbers, const vector<double>& correct_numbers);
double sum_positive_int_part(int left, int right);
double sum_positive_frac_part(double left, double right);
double sum_negative_int_part(int left, int right);
double to_decimal(double number);


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
    
    vector<double> correct_numbers;
    if (!convert_strs_to_double(inpt_strs, correct_numbers)) {
        cout << "Input file doesn't contain valid data!" << endl;
        return 1;
    }

    vector<Number> numbers_int_frac = divide_int_frac(correct_numbers);
    if (numbers_int_frac.size() == 1) {
        cout << "Not enough parameters in input file!" << endl;
        return 1;
    }
    vector<Number> converted_numbers = convert_numbers(numbers_int_frac);
    vector<double> result = summation(converted_numbers, correct_numbers);

    ofstream out("output.txt", ios::out);

    for (int i = 0; i < result.size() - 2; i += 3) {
        out << setprecision(12) << result[i] << " + " << result[i + 1] << " = " << result[i + 2] << endl;
        out << "Convert to decimal: " << to_decimal(result[i + 2]) << endl;
    }
    for (int i = 0; i < correct_numbers.size() - 1; i += 2) 
        out << correct_numbers[i] << " + " << correct_numbers[i + 1] << " = " << correct_numbers[i] + correct_numbers[i + 1] << endl;
    return 0;
}


vector<string> read_file(ifstream& input)
{
    vector<string> result;
    for (string line; getline(input, line); ) {
        stringstream ss(line);
        for (string str; getline(ss, str, ' '); ) {
            result.push_back(str);
        }
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
        else if (ch == '-')
            if (ch != str[0])
                return false;
            else
            {
                k_minus++;
                k_plus++;
            }
        else if (ch == '+')
            if (ch != str[0])
                return false;
            else
            {
                k_plus++;
                k_minus++;
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
    if (result.size() % 2 != 0)
        result.erase(prev(result.end()));
    return result;
}


int convert_int_part(int number) {
    int result = 0;
    if (number < 4) {
        result = number;
    }
    else {
        vector<int> moduls;
        while (number > 4) {
            moduls.push_back(number % 4);
            number = number / 4;
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
    return (int)((frac_str.size() - 2) * 1.5) + 1;
}


double convert_frac_part(double frac) {
    double result = 0.0;
    if (frac > 0.0) {
        int k = precision(frac);
        int i = 1;
        while (i <= k) {
            frac = frac * 4;
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


vector<Number> convert_numbers(const vector<Number>& numbers) {
    vector<Number> result;
    for (const Number& number : numbers) 
    {
        Number tmp;
        tmp.int_part = convert_int_part(number.int_part);
        tmp.frac_part = convert_frac_part(number.frac_part);
        result.push_back(tmp);
    }
    return result;
}

double sum_positive_int_part(int left, int right) {
    string sresult;
    string stop = to_string(left);
    string sbott = to_string(right);
    int d = 0;
    int n1 = stop.size();
    int n2 = sbott.size();
    while (n1 || n2)
    {
        d += n1 ? stop[--n1] - '0' : 0;
        d += n2 ? sbott[--n2] - '0' : 0;
        sresult = to_string(d % 4) + sresult;
        d /= 4;
    }
    if (d)
        sresult = "1" + sresult;
    return atof(sresult.c_str());
}


double sum_negative_int_part(int left, int right) {
    if (left - right < 0) {
        return (-1) * sum_negative_int_part(right, left);
    }
    string sresult;
    string stop = to_string(left);
    string sbott = to_string(right);
    int d = 0;
    int n1 = stop.size();
    int n2 = sbott.size();
    int zaem = 0;
    while (n1 || n2)
    {
        if ((stop[0] - sbott[0]) == 0) {
            break;
        }
        int s1 = n1 ? stop[--n1] - '0' - zaem : 0 - zaem;
        int s2 = n2 ? sbott[--n2] - '0' : 0;

        if (s1 < s2) {
            if (s1 < 0) {
                while (s1 < 0) {
                    int i = n1 - 1;
                    while (stop[i] == '0') {
                        i--;
                    }
                    stop[i] -= 1;
                    s1++;
                    zaem += 1;
                    d += 1;
                }
            }
            else {
                int i = n1 - 1;
                while (stop[i] == '0') {
                    i--;
                }
                stop[i] -= 1;
                zaem += 1;
                d += s1 + 1;
            }
        }
        else {
            d += s1 - s2;
        }
        sresult = to_string(d % 4) + sresult;
        d /= 4;
    }
    if (d)
        sresult = "1" + sresult;
    return atof(sresult.c_str());
}


double sum_positive_frac_part(double left, double right) {
    string sresult;
    string stop = to_string(left);
    string sbott = to_string(right);
    int d = 0;
    int n1 = stop.size();
    int n2 = sbott.size();
    while (n1 || n2)
    {
        if (n1 > n2) {
            d += stop[--n1] - '0';
        }
        else if (n2 > n1) {
            d += stop[--n2] - '0';
        }
        else {
            d += stop[--n1] - '0';
            d += sbott[--n2] - '0';
        }

        sresult = to_string(d % 4) + sresult;
        d /= 4;
        if (n1 != stop.size())
            if (stop[n1 - 1] == '.') break;
        if (n2 != sbott.size())
            if (sbott[n2 - 1] == '.') break;
    }
    if (d)
        sresult = "1." + sresult;
    else
        sresult = "0." + sresult;
    return atof(sresult.c_str());
}


double to_decimal(double number)
{
    double number_ = number;
    if (number < 0)
        number_ *= -1;
    double result = 0.0;
    string snumber = to_string(number_);
    int i = snumber.find('.') - 1;
    if (i == -1)
        i = snumber.size() - 1;
    int j = 0;
    while (snumber[j]) 
    {
        if (snumber[j] != '.') 
        {
            result += pow(4, i) * (snumber[j] - '0');
            i--;
        }
        j++;
    }
    return number < 0 ? -result : result;
}

vector<double> summation(const vector<Number>& converted_numbers, const vector<double>& correct_numbers)
{
    vector<double> result;
    for (int i = 0; i < converted_numbers.size() - 1; i += 2) {
        if (correct_numbers[i] >= 0 && correct_numbers[i + 1] >= 0)
        {
            double left = converted_numbers[i].int_part + converted_numbers[i].frac_part;
            double right = converted_numbers[i + 1].int_part + converted_numbers[i + 1].frac_part;
            double int_part = sum_positive_int_part(converted_numbers[i].int_part, converted_numbers[i + 1].int_part);
            double frac_part = sum_positive_frac_part(converted_numbers[i].frac_part, converted_numbers[i + 1].frac_part);
            if (frac_part > 1) {
                int_part = sum_positive_int_part((int)int_part, 1);
            }
            double sum = int_part + frac_part;
            result.push_back(left);
            result.push_back(right);
            result.push_back(sum);
        }
        else if (correct_numbers[i] < 0 && correct_numbers[i + 1] < 0)
        {
            double left = converted_numbers[i].int_part + converted_numbers[i].frac_part;
            double right = converted_numbers[i + 1].int_part + converted_numbers[i + 1].frac_part;
            double int_part = sum_positive_int_part(converted_numbers[i].int_part, converted_numbers[i + 1].int_part);
            double frac_part = sum_positive_frac_part(converted_numbers[i].frac_part, converted_numbers[i + 1].frac_part);
            if (frac_part > 1) {
                int_part = sum_positive_int_part((int)int_part, 1);
            }
            double sum = int_part + frac_part;
            result.push_back(-left);
            result.push_back(-right);
            result.push_back(-sum);
        }
        else if (correct_numbers[0] < 0 && correct_numbers[1] >= 0) {
            double left = converted_numbers[i].int_part;
            double right = converted_numbers[i + 1].int_part;
            double int_part = sum_negative_int_part(converted_numbers[i + 1].int_part, converted_numbers[i].int_part);
            double sum = int_part;
            result.push_back(-left);
            result.push_back(right);
            result.push_back(sum);
        }
        else {
            double left = converted_numbers[i].int_part;
            double right = converted_numbers[i + 1].int_part;
            double int_part = sum_negative_int_part(converted_numbers[i].int_part, converted_numbers[i + 1].int_part);
            double sum = int_part;
            result.push_back(left);
            result.push_back(-right);
            result.push_back(sum);
        }
    }
    return result;
}