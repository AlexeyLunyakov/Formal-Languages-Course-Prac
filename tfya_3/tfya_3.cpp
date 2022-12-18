#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
using namespace std;

// Виды лексем:
enum lexems {
	BEGIN_OBJECT,
	END_OBJECT,
	BEGIN_ARRAY,
	END_ARRAY,
	COMMA,
	COLON,
	STRING,
	NUMBER,
	KWORD,
	ERROR_TYPE
};

// Счетчики парных символов:
int begin_obj = 0;
int end_obj = 0;
int begin_array = 0;
int end_array = 0;
int coms_and_colons = 0;
// Вспомогательные счетчики:
int strings = 0;
long long ch = 0;
int ch_begin_obj = 0;
int ch_end_obj = 0;
// Литералы:
string literal_words[3] = { "true", "false", "null" };
// Структура токена: название лексемы и само считанное значение
struct token {
	lexems class_;
	string value;
};
// Проверка на лексему строки:
bool is_string(string word) {
	if (word[0] == '"' and word[word.length() - 1] == '"') return true;
	return false;
}
// Проверка на лексему числа:
bool is_num(string id) {
	const string s = id;
	const regex re1(R"(^[+-]?([0-9]*[.])?($|[0-9]+)$)");
	const regex re2(R"(^[+-]?([0-9]*[.])?[/0-9]+(e|E)(\+|-)[0-9]+$)");
	smatch m;
	// Сравнение с регулярными выражениями через regex
	if (regex_search(s, m, re1)) {
		return true;
	}
	else if (regex_search(s, m, re2)) {
		return true;
	}
	return false;
}
// Проверка на принадлежность к литералам
bool is_lit_word(string id) {
	for (int kword = 0; kword < 3; kword++) {
		bool b = true;
		if (id.size() != literal_words[kword].size()) {
			continue;
		}
		for (int i = 0; i < literal_words[kword].length(); i++) {
			if (id[i] != literal_words[kword][i]) {
				b = false;
				break;
			}
		}
		if (b == true) {
			return true;
		}
	}
	return false;
}

// Анализатор:
vector<token*> lexer(string filename) {
	FILE* file_fil;
	int c;
	struct token* token_;
	vector<token*> lex_table;
	if ((file_fil = fopen(filename.c_str(), "r")) == NULL) {
		printf("Не возможно открыть файл!" );
		return lex_table;
	}
	c = fgetc(file_fil);
	while (!feof(file_fil)) {
		token_ = new token;
		// Пропускаем пробелы:
		while (c == '\n' || c == '\t' || c == ' ') {
			c = fgetc(file_fil);
		}
		// Если находим парные символы:
		if (c == '{' || c == '}' || c == ':' || c == ',' || c == '[' || c == ']') {
			ch++;
			token_->value = c;
			switch (c) {
			case '{':
				begin_obj++;
				token_->class_ = BEGIN_OBJECT;
				break;
			case '}':
				ch_end_obj = ch;
				end_obj++;
				if (end_obj > begin_obj) {
					cout << "Неправильное положение фигурных скобок!\n";
					exit(-1);
				}
				token_->class_ = END_OBJECT;
				break;
			case '[':
				begin_array++;
				token_->class_ = BEGIN_ARRAY;
				break;
			case ']':
				end_array++;
				if (end_array > begin_array) {
					cout << "Неверное положение скобок массива!\n";
					exit(-1);
				}
				token_->class_ = END_ARRAY;
				break;
			case ',':
				token_->class_ = COMMA;
				coms_and_colons++;
				break;
			case ':':
				coms_and_colons++;
				token_->class_ = COLON;
				break;
			}
			c = fgetc(file_fil);
		}
		// если находим буквы (т.е слова):
		else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' or c == '"') {
			string buff = "";
			while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9') or c == '"') {
				ch++;
				buff += c;
				c = fgetc(file_fil);
			}
			string tmp = buff.substr(1, buff.size() - 2);
			token_->value = tmp;
			strings++;
			if (is_lit_word(buff)) {
				token_->class_ = KWORD;
				token_->value = buff;
			}
			else if (is_string(buff)) {
				
				token_->class_ = STRING;
			}
			else {
				token_->class_ = ERROR_TYPE;
			}

		}
		// если находим числа:
		else if ((c >= '0' && c <= '9') || c == '.' || c == '-') {
			string buff = "";
			while ((c >= '0' && c <= '9') || c == '.' || c == '-' || c == '+' || c == 'e' || c == 'E') {
				ch++;
				buff += c;
				c = fgetc(file_fil);
			}
			strings++;
			if (is_num(buff)) {
				token_->class_ = NUMBER;
				token_->value = buff;
			}
			else {
				token_->class_ = ERROR_TYPE;
				token_->value = buff;
			}
		}
		lex_table.push_back(token_);
	}
	return lex_table;
}

void output(vector<token*> lex_table) {
	// Проверяем на наличие ошибок:
	if (lex_table.size() == 0) 
		cout << "\nОшибка лексического анализатора!\n";

	if (begin_obj != 1 or end_obj != 1) {
		cout << "Ошибка с парами знаков типа '{' или '}' - недопустимое количество объектов!\n";
		exit(-1);
	}
	if (begin_array != end_array) {
		cout << "Ошибка с парами знаков типа '[' или ']' - недопустимое количество объектов!\n";
		exit(-1);
	}

	if (coms_and_colons + 1 != strings) {
		cout << "Ошибка с парами знаков типа ':' или ',' - недопустимое количество объектов!\n";
		exit(-1);
	}
	// Если всё нормально, то выводим все найденные лексемы:
	cout << "Tokenlist:\n";
	for (token* i : lex_table) {
		switch (i->class_) {
			case STRING:
			{
				cout << "(STRING, '" << i->value << "')";
				break;
			}
			case NUMBER:
			{
				cout << "(NUMBER, '" << i->value << "')";
				break;
			}
			case KWORD:
			{
				cout << "(LITERAL, '" << i->value << "')";
				break;
			}
			case COLON:
			{
				cout << "(COLON, '" << i->value << "')";
				break;
			}
			case COMMA:
			{
				cout << "(COMMA, '" << i->value << "')";
				break;
			}
			case BEGIN_OBJECT:
			{
				cout << "(BEGIN_OBJECT, '" << i->value << "')";
				break;
			}
			case END_OBJECT:
			{
				cout << "(END_OBJECT, '" << i->value << "')";
				break;
			}
			case BEGIN_ARRAY:
			{
				cout << "(BEGIN_ARRAY, '" << i->value << "')";
				break;
			}
			case END_ARRAY:
			{
				cout << "(END_ARRAY, '" << i->value << "')";
				break;
			}
			case ERROR_TYPE:
			{
				cout << "Ошибочный тип!\n Выражение " << i->value << " не корректно!";
				exit(1);
			}
			default:
				break;
		}
		cout << endl;
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	vector<token*> l = lexer("C:\\Users\\AlexLnv\\source\\repos\\tfya_3\\tfya_3\\test3.txt"); //
	output(l);
}


//cout << "Total number of characters: " << ch << endl;
