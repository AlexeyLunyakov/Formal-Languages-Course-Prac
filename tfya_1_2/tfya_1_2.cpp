#include <iostream>
#include <sstream>
#include <stack>
#include <map>
using namespace std;

map<string, int> priority = {
        {"(", 1}, {")", 1},
        {"+", 2}, {"-", 2},
        {"*", 3}, {"/", 3}
};

string intoRevPN(string expression) {

    string term;
    stack<string> operands;

    istringstream cin_string(expression);
    string pn_result = "";
    // цикл с условием присутствия ввода выражения
    while (getline(cin_string, term, ' ')) {
        if (term.size() == 0)
            continue;
        // проверка символа строки на соответствие знакам операций
        bool flag = 0;
        for (auto& elem : priority) {
            if (elem.first == term)
                flag = 1;
        }
        // если это операция
        if (flag) {
            // отдельная проверка на скобки, тк в опн их не записываем
            if (term == "(")
                operands.push(term);
            else if (term == ")") {
                while (!operands.empty() && operands.top() != "(") {
                    pn_result += " " + operands.top();
                    operands.pop();
                }
                operands.pop();
            }
            else {
                // в зависимости от сравнения с приоритетным словарем – добавление знака операции в опн
                while (!operands.empty() && priority[operands.top()] >= priority[term]) {
                    pn_result += " " + operands.top();
                    operands.pop();
                }
                operands.push(term);
            }
        }
        // добавление чисел (не знака)
        else {
            pn_result += " " + term;
        }
    }
    // добавление последних операций, когда входных параметров больше нет
    while (!operands.empty()) {
        pn_result += " " + operands.top();
        operands.pop();
    }
    return pn_result;
}

int fromRevPN(string expression) {
    string term;
    stack<int> values;

    istringstream substr(expression + " ");
    // цикл с условием присутствия ввода выражения
    while (getline(substr, term, ' ')) {
        if (term.size() == 0)
            continue;
        // проверка символа строки на соответствие знакам операций
        bool flag = 0;
        for (auto& elem : priority) {
            if (elem.first == term)
                flag = 1;
        }
        // если это операция
        if (flag) {
            int first, last;
            last = values.top(); // last присваивается значение первого элемента стека
            values.pop(); // удаляется первый элемент стека
            first = values.top(); // first присваивается значение нового первого элемента стека
            values.pop(); // удаляется первый элемент стека
            // разбор оставшихся операций
            if (term == "+")
                values.push(first + last);
            else if (term == "-")
                values.push(first - last);
            else if (term == "*")
                values.push(first * last);
            else if (term == "/")
                values.push(first / last);
        }
        else {
            values.push(stoi(term));
        }
    }
    return values.top();
}

int main(){
    string expression;
    int answer;
    cout << "Enter expression in reverse polish notation:";
    getline(cin, expression);
    answer = fromRevPN(expression);
    cout << "Calculation result: " << answer;
    return 0;
}

/*int main(){
    string expression;

    cout << "Enter a mathematical expression:";
    getline(cin, expression);
    expression = intoRevPN(expression);
    cout << "Reverse Polish Notation:" << expression << "\n";
    return 0;
}*/

//int answer;
//    answer = fromRevPN(expression);
//    cout << "Mathematical expression result: " << answer;
/*        if (!flag)
            values.push(stoi(word));
        else {
            int first, last;
            last = values.top();
            values.pop();
            first = values.top();
            values.pop();
            if (word == "+")
                values.push(first + last);
            else if (word == "-")
                values.push(first - last);
            else if (word == "*")
                values.push(first * last);
            else if (word == "/")
                values.push(first / last);
        }*/

/*        if (!flag)
            pn_result += " " + word;
        else {
            if (word == "(")
                operands.push(word);
            else if (word == ")") {
                while (!operands.empty() && operands.top() != "(") {
                    pn_result += " " + operands.top();
                    operands.pop();
                }
                operands.pop();
            }
            else {
                while (!operands.empty() && priority[operands.top()] >= priority[word]) {
                    pn_result += " " + operands.top();
                    operands.pop();
                }
                operands.push(word);
            }
        }*/


/*int main() {

    stack<int> stack_num;
    string substr = "";
    cout << "Enter expression in reverse polish notation:";
    getline(cin, substr);

    for (int i = 0; i < substr.size(); i++) {
        if (isdigit(substr[i])) {
            stack_num.push(substr[i] - 48);
        }
        else {
            int value_1 = 0;
            int value_2 = 0;

            if (substr[i] == '+') {
                value_1 = stack_num.top();
                stack_num.pop();
                value_2 = stack_num.top();
                stack_num.pop();
                stack_num.push(value_1 + value_2);
            }
            else if (substr[i] == '-'){
                value_1 = stack_num.top();
                stack_num.pop();
                value_2 = stack_num.top();
                stack_num.pop();
                stack_num.push(value_2 - value_1);
            }
            else if (substr[i] == '*'){
                value_1 = stack_num.top();
                stack_num.pop();
                value_2 = stack_num.top();
                stack_num.pop();
                stack_num.push(value_1 * value_2);
            }
            else if (substr[i] == '/'){
                value_1 = stack_num.top();
                //cout << value_1;
                stack_num.pop();
                value_2 = stack_num.top();
                //cout << value_2;
                stack_num.pop();
                stack_num.push(value_1 / value_2);
            }
        }
    }
    cout << "Calculation result:" << stack_num.top() << endl;
}*/