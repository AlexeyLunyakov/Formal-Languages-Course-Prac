#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

string keywords[2] = { "for", "do" };  // ключевые слова языка 
enum states { H, ID, NM, ASGN, DLM, ERR }; // состояния лексического анализатора
enum token_names { KEYWORD, DELIMIT, OPERATION, IDENT, NUM }; // ключ.слова | разделители | операции | идентификаторы | числа

struct token {
	enum token_names token_name;
	char* token_value = NULL;
};

struct lexeme_table {
	struct token tkn;
	struct lexeme_table* next = nullptr;
};

struct lexeme_table* lex_table = NULL;
struct lexeme_table* lex_table_head = NULL;

int new_token(struct token* tkn) { // присвоение текущему символу соотвествующей лексемы
	if (lex_table_head == NULL) {
		lex_table_head = new lexeme_table();
		lex_table = lex_table_head;
		lex_table_head->tkn = *tkn;
	}
	else {
		lex_table->next = new lexeme_table();
		lex_table = lex_table->next;
		lex_table->tkn = *tkn;
	}
	return 1;
}

int is_num(const char* id) { // проверка на число
	const string s = id;
	const regex re1(R"(^[+-]?([0-9]*[.])?($|[0-9]+)$)");
	const regex re2(R"(^[+-]?([0-9]*[.])?[/0-9]+(e|E)(\+|-)[0-9]+$)");
	smatch m;
	if (regex_search(s, m, re1)) {
		return 1;
	}
	else if (regex_search(s, m, re2)) {
		return 2;
	}
	return 0;
}

int is_keyword(const char* id) { // проверка на ключевое слово
	for (int kword = 0; kword < 2; kword++) {
		bool b = true;
		for (int i = 0; i < keywords[kword].length(); i++) {
			if (id[i] != keywords[kword][i]) {
				b = false;
				break;
			}
		}
		auto end_of_id = id[keywords[kword].length()];
		if (end_of_id != '\0') {
			b = false;
		}
		if (b == true) {
			return 1;
		}
	}
	return 0;
}

int lexer(const char* filename) {
	int elem;
	int err_symbol;
	struct token tkn;

	FILE* fd;
	if ((fd = fopen(filename, "r")) == NULL)
	{
		printf("\nCannot open file %s.\n", filename);
		return -1;
	}

	enum states CS = H;
	elem = fgetc(fd);
	while (!feof(fd))
	{
		switch (CS) {
			case H: { 
				// Начальное состояние -> переход в следующее состояние
				while ((elem == ' ') || (elem == '\t') || (elem == '\n')) {
					elem = fgetc(fd);
				}
				if (((elem >= 'A') && (elem <= 'Z')) || ((elem >= 'a') && (elem <= 'z')) || (elem == '_')) {
					CS = ID; // переход в идентификатор
				}
				else if (((elem >= '0') && (elem <= '9')) || (elem == '.') || (elem == '+') || (elem == '-')) {
					CS = NM; // переход в числа
				}
				else if (elem == ':') {
					CS = ASGN; // переход в присваивание
				}
				else {
					CS = DLM; // переход в разделители
				}
				break;
			}
			case ID: {
				// Идентификаторы
				int size = 0;
				char tmp_buff[256];

				tmp_buff[size] = elem;
				size++;
				elem = fgetc(fd);
				while (((elem >= 'A') && (elem <= 'Z')) || ((elem >= 'a') && (elem <= 'z')) || ((elem >= '0') && (elem <= '9')) || (elem == '_')) {
					tmp_buff[size] = elem;
					size++;
					elem = fgetc(fd);
				}

				tmp_buff[size] = '\0';
				if (is_keyword(tmp_buff)) {
					tkn.token_name = KEYWORD;
				}
				else {
					tkn.token_name = IDENT;
				}
				if ((tkn.token_value = (char*)malloc(strlen(tmp_buff))) == NULL) {
					printf("\nMemory allocation error in function\"lexer\"\n");
					return -1;
				}
				strcpy(tkn.token_value, tmp_buff);
				new_token(&tkn);
				CS = H; // переход в начальное состояние 
				break;
			}
			case NM: {
				// Числа 
				int size = 0;
				char tmp_buff2[256];

				tmp_buff2[size] = elem;
				size++;
				elem = fgetc(fd);

				while (((elem >= '0') && (elem <= '9'))) {
					tmp_buff2[size] = elem;
					size++;
					elem = fgetc(fd);
				}
				tmp_buff2[size] = '\0';

				const string s = tmp_buff2;
				if (s.length() == 1) {
					if (s == "+" or s == "-") {
						tkn.token_name = OPERATION;
					}
					else {
						tkn.token_name = NUM;
					}
				}
				else if (is_num(tmp_buff2)) {
					tkn.token_name = NUM;
				}
				else {
					CS = ERR;
					break;
				}

				if ((tkn.token_value = (char*)malloc(strlen(tmp_buff2))) == NULL) {
					printf("\nMemory allocation error in function\"lexer\"\n");
					return -1;
				}
				strcpy(tkn.token_value, tmp_buff2);
				new_token(&tkn);
				CS = H;
				break;
			}
			case ASGN: {
				// Знак присваивания
				int colon = elem;
				elem = fgetc(fd);
				if (elem == '=') {
					tkn.token_name = OPERATION;
					if ((tkn.token_value = (char*)malloc(sizeof(2))) == NULL) {
						printf("\nMemory allocation error in function \"lexer\"\n");
						return -1;
					}
					strcpy(tkn.token_value, ":=");
					new_token(&tkn);
					elem = fgetc(fd);
					CS = H;
				}
				else {
					err_symbol = colon;
					CS = ERR;
				}
				break;
			}
			case DLM: {
				// Разделители 
				if ((elem == '(') || (elem == ')') || (elem == ';')) {
					tkn.token_name = DELIMIT;
					if ((tkn.token_value = (char*)malloc(sizeof(1))) == NULL) {
						printf("\nMemory allocation error in function \"lexer\"\n");
						return -1;
					}
					sprintf(tkn.token_value, "%c", elem);
					new_token(&tkn);
					elem = fgetc(fd);
					CS = H;
				}
				else if ((elem == '<') || (elem == '>') || (elem == '=')) {
					tkn.token_name = OPERATION;
					if ((tkn.token_value = (char*)malloc(sizeof(1))) == NULL) {
						printf("\nMemory allocation error in function \"lexer\"\n");
						return -1;
					}
					sprintf(tkn.token_value, "%c", elem);
					new_token(&tkn);
					elem = fgetc(fd);
					CS = H;
				}
				else {
					err_symbol = elem;
					elem = fgetc(fd);
					CS = ERR;
				}
				break;
			}
			case ERR: {
				// состояние ошибки \ нераспознанные символы
				printf("\nUnknown character: %c\n", err_symbol);
				CS = H;
				break;
			}
			
		} // switch
	}
	return 1;
}

void print_lexeme_table() {
	lexeme_table* tmp_lext = lex_table_head;
	bool for_flag = true;
	cout << "\nСодержимое файла, поступившее на анализ: \n";
	while (true) {
		string s = tmp_lext->tkn.token_value;
		cout << s;
		if (s == "for") { 
			for_flag = true; 
		}
		else if (s == "do") { 
			for_flag = false; 
		}
		if (!for_flag and (s == ";" or s == "do")) {
			cout << "\n\t";
		}
		else {
			cout << " ";
		}
		if (tmp_lext->next == nullptr) { 
			break; 
		}
		tmp_lext = tmp_lext->next;
	}

	cout << "\n\nРезультат работы лексического анализотора: \n\n";

	tmp_lext = lex_table_head;
	while (true) {
		string s = tmp_lext->tkn.token_value;
		cout << s << "   -   ";
		// { KEYWORD, DELIMIT, OPERATION, IDENT, NUM }
		switch (tmp_lext->tkn.token_name) {
		case KEYWORD:
			cout << "keyword";
			break;
		case DELIMIT:
			cout << "delimiter";
			break;
		case OPERATION:
			cout << "operation";
			break;
		case IDENT:
			cout << "identificator";
			break;
		case NUM:
			cout << "number";
			break;
		default:
			break;
		}
		cout << "\n";
		if (tmp_lext->next == nullptr) { 
			break;
		}
		tmp_lext = tmp_lext->next;
	}
}


int main() {
	setlocale(LC_ALL, "Russian");
	lexer("C:\\Users\\AlexLnv\\source\\repos\\tfya_4\\tfya4\\tfya4.txt");
	print_lexeme_table();
}
