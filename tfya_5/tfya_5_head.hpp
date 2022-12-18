#pragma once
#include <iostream>
#include <list>
#include <string>
#include <iterator>
using namespace std;

struct Node {
    string name;
    string A; //переход по а
    string B; //переход по b
};

enum AorB {
    A, B
};

// состояния парсера
enum states {
    OpBr,
    ClBr,
    CR_ST,
    LETTER,
    NXT_ST
};

class Converter {
    list <Node*> NKA;       //    список для недетерминированного
    list <Node*> DKA;       //    список для детерминированного
    enum AorB ab = A;       //    состояние перехода
    string current_pos;     //    текущая позиция
    string init_state, fin_state; // начальные и конечные состояния

public:
    // Конструктор класса 
    Converter(string b, string e) :init_state(b), fin_state(e) {}

    // Обработка строки с переходами
    void NKA_list(string);

    // Флаги сущетсвования узла в списке НКА
    bool check_node_for_CR_ST(string);
    void check_nodes_for_NXT_ST(string);
    // Добавление элемента в список НКА
    void new_list_NKA(string);

    // Создание списка ДКА
    void DKAlist(Node*); 
    // Создание узлов ДКА
    string create_A_node(Node*);
    string create_B_node(Node*);

    // Проверка наличия состояния в списке ДКА по имени
    bool check_notes_DKA(string);
    // Добавление в список ДКА нового состояния(узла)
    void new_node_DKA(string);
    // Сортировка строки, представляющей собой набор индексов состояний
    string sort(string);
    // Вывод получившегося ДКА
    void DKA_print();
    // Поиск конечных состояний
    string find_fin_states();
};

/*Структура списка недетерминированного КА:
первое число - номер вершины, переход из которой мы рассматриваем
буква - нетерминал, по которому переходим
второе число - номер вершины, куда переходим
*/
void Converter::NKA_list(string str) {
    int i = 0;
    string temp;
    enum states CS = OpBr;
    while (i < str.size()) {
        switch (CS) {
            case OpBr:  { //Состояние открывающей скобки
                CS = CR_ST;
                i++;
                temp = str[i];
                break;
            }
            case CR_ST: { //Состояние рассмотрения текущей вершины
                if (str[i + 1] != ',') {
                    temp += str[i];
                    i += 1;
                    break;
                }
                if (!check_node_for_CR_ST(temp)) {
                    //Если не нашли вершину, то добавляем ее в список
                    new_list_NKA(temp);
                }
                //Переходим в состояние рассмотрения нетерминала
                CS = LETTER;
                i += 2;
                current_pos = temp;
                break;
            }
            case LETTER: {
                //В зависимости от того, по какому нетерминалу идет переход
                if (str[i] == 'a') {
                    ab = A;
                }
                else {
                    ab = B;
                }
                //Переходим в состояние рассмотрения новой вершины
                CS = NXT_ST;
                i += 2;
                temp = str[i];
                break;
            }
            case NXT_ST:{
                if (str[i + 1] != ')') {
                    temp += str[i];
                    i++;
                    break;
                }
                check_nodes_for_NXT_ST(temp);
                CS = ClBr;
                i++;
                break;
            }
            case ClBr: { //Состояние закрывающей скобки
                CS = OpBr;
                i += 2;
                temp = "";
                break;
            }
        }
    }
    //Создаем список ДКА
    //В первую очередь помещаем в список запись начального состояния
    Node* node = new Node;
    node->name = init_state;
    DKAlist(node);
}

//Добавление элемента в список НКА
void Converter::new_list_NKA(string str) {
    Node* node = new Node;
    node->name = str;
    NKA.push_back(node);
}

//Проверка на сущестование текущей вершины в списке
bool Converter::check_node_for_CR_ST(string str) {
    if (NKA.empty()) {
        return false;
    }
    for (auto node : NKA) {
        if (node->name == str) {
            return true;
        }
    }
    return false;
}
//Проверка на сущестование следующей вершины в списке
void Converter::check_nodes_for_NXT_ST(string str) {
    string temp;
    list <Node*> ::iterator tmp_it = NKA.begin();
    Node* node = *tmp_it;
    while (tmp_it != NKA.end()) {
        node = *tmp_it;
        if (node->name == current_pos) {
            break;
        }
        tmp_it++;
    }
    //Проверка на A
    if (ab == A) {
        if (node->A.size() == 0) {
            node->A += str;
        }
        for (int i = 0; i < str.size(); i++)
            for (int j = 0; j < node->A.size(); j++) {
                if (str[i] == node->A[j]) {
                    break;
                }
                else {
                    temp += str[i];
                }
            }
        node->A += temp;
    } // Проверка на В
    else {
        if (node->B.size() == 0)
            node->B += str;
        for (int i = 0; i < str.size(); i++) {
            for (int j = 0; j < node->B.size(); j++) {
                if (str[i] == node->B[j]) break;
                else temp += str[i];
            }
        }
        node->B += temp;
    }
}

// Формирование списка ДКА
void Converter::DKAlist(Node* node) {
    //пока не будет найдено повторение 
    if (!check_notes_DKA(node->name)) {
        if (DKA.empty()) {
            new_node_DKA(init_state);
            node = DKA.front();
        }
        string notes = create_A_node(node);
        if (!check_notes_DKA(node->A))
        {
            new_node_DKA(notes);
            DKAlist(DKA.back());
        }
        notes = create_B_node(node);
        if (!check_notes_DKA(node->B))
        {
            new_node_DKA(notes);
            DKAlist(DKA.back());
        }
    }
}

string Converter::create_A_node(Node* node) {
    string temp;
    for (int k = 0; k < node->name.size(); k++) {
        for (auto tmp : NKA) {
            if (tmp->name[0] == node->name[k]) {
                if (temp != "")
                    for (int i = 0; i < tmp->A.size(); i++)
                        for (int j = 0; j < temp.size(); j++) {
                            if (tmp->A[i] == temp[j]) break;
                            else if (j == temp.size() - 1) temp += tmp->A[i];
                        }
                else temp = tmp->A;
            }
        }

    }
    temp = sort(temp);
    node->A = temp;
    return temp;
}

string Converter::create_B_node(Node* node) {
    string temp;
    for (int k = 0; k < node->name.size(); k++) {
        for (auto tmp : NKA) {
            if (tmp->name[0] == node->name[k]) {
                if (temp != "")
                    for (int i = 0; i < tmp->B.size(); i++) {
                        for (int j = 0; j < temp.size(); j++) {
                            if (tmp->B[i] == temp[j]) {
                                break;
                            }
                            else if (j == temp.size() - 1) {
                                temp += tmp->B[i];
                            }
                        }
                    }
                else {
                    temp = tmp->B;
                }
            };
        }
    }
    temp = sort(temp);
    node->B = temp;
    return temp;
}

bool Converter::check_notes_DKA(string str) {
    list <Node*> ::iterator it = DKA.begin();
    Node* node;
    for (auto node : DKA)
        if (node->name == str && (node->A != "" || node->B != "")) {
            return true;
        }
    return false;
}

void Converter::new_node_DKA(string str) {
    Node* node = new Node;
    node->name = str;
    DKA.push_back(node);
}

string Converter::sort(string str) {
    char tmp;
    for (int i = 0; i < str.size() - 1; ++i) // i - номер прохода
        for (int j = 0; j < str.size() - 1; ++j) // внутренний цикл прохода
            if (str[j + 1] < str[j])
                swap(str[j], str[j + 1]);
    return str;
}
//Функция вывода ДКА
void Converter::DKA_print() {
    cout << "--------------\n\nDKA:\nSet of states: ";
    for (auto node : DKA)
        cout << node->name << " ";
    cout << "\n\nInput alphabet: a, b\n\nState-transitions function:\n";
    for (auto node : DKA) {
        cout << "D(" << node->name << ", a) = " << node->A << "\n";
        cout << "D(" << node->name << ", b) = " << node->B << "\n";
    }
    cout << "\nInitial states: " << init_state << "\n";
    cout << "\nFinal states: " << find_fin_states() << "\n";
}
//Поиск конечных состояний
string Converter::find_fin_states() {
    string final_st;
    for (auto node : DKA) {
        for (int i = 0; i < node->name.size(); i++) {
            if (node->name[i] == fin_state[0]) {
                final_st += node->name + " ";
                break;
            }
        }
    }
    return final_st;
}
