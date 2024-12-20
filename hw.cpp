#include <iostream>
#include <stack>
#include <sstream>
#include <cmath>
#include <string>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <windows.h>
#pragma execution_character_set("utf-8")
using namespace std;

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

double woperator(char op, double b, double a) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/') return a / b;
    if (op == '^') return pow(a, b);
}


double wfunction(const string& func, double arg) {
    if (func == "sin") return sin(arg);
    if (func == "cos") return cos(arg);
    if (func == "tan") return tan(arg);
    if (func == "cot") return 1.0 / tan(arg);
    if (func == "exp") return exp(arg);
    throw invalid_argument("Неизвестная функция: " + func);
}


double evaluateexpression(string expr, double xValue) {
    stack<double> values;
    stack<char> operators;


    for (size_t pos = 0; (pos = expr.find("x", pos)) != string::npos; pos += to_string(xValue).length()) {
        expr.replace(pos, 1, to_string(xValue));
    }


    expr.erase(remove(expr.begin(), expr.end(), ' '), expr.end());

    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];


        if (isdigit(c) || c == '.') {
            string numStr;
            while (i < expr.size() && (isdigit(expr[i]) || expr[i] == '.')) {
                numStr += expr[i];
                i++;
            }
            i--;
            values.push(stod(numStr));
        }

        else if (c == '(') {
            operators.push(c);
        }

        else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {

                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = operators.top(); operators.pop();
                values.push(woperator(op, val2, val1));
            }
            operators.pop();
        }

        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            while (!operators.empty() && precedence(operators.top()) >= precedence(c)) {
                double val2 = values.top();values.pop();
                double val1 = values.top(); values.pop();
                char op = operators.top(); operators.pop();
                values.push(woperator(op, val2, val1));
            }
            operators.push(c);
        }
        else if (isalpha(c)) {
            string func;
            while (i < expr.size() && isalpha(expr[i])) {
                func += expr[i];
                i++;
            }
            i++;
            int start = i, brackets = 1;
            while (brackets != 0) {
                i++;
                if (expr[i] == '(') brackets++;
                if (expr[i] == ')') brackets--;
            }
            double result = evaluateexpression(expr.substr(start, i - start), xValue);
            values.push(wfunction(func, result));
        }
         else{
            throw invalid_argument("Invalid argument " + string(1, c));
         }
    }

    while (!operators.empty()) {
        double val2 = values.top(); values.pop();
        double val1 = values.top(); values.pop();
        char op = operators.top(); operators.pop();
        values.push(woperator(op, val2, val1));
    }
    return values.top();

}
int main() {
    string expression;
    SetConsoleOutputCP(CP_UTF8);
    cout << "Enter your problem: ";
    getline(cin, expression);
    double xValue = 0.0;
    if (expression.find('x') != string::npos) {
        cout << "Enter x: ";
        cin >> xValue;
    }
    try {
        double result = evaluateexpression(expression, xValue);
        cout << "Res: " << result << endl;
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    return 0;
}