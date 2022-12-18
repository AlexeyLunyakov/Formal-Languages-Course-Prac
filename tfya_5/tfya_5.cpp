#include "tfya_5_head.hpp"
using namespace std;

int main() {
    //setlocale(LC_ALL, "Russian");
    cout << "Your set of states: \n1 2 3\nYour alphabet: \na, b\n";
    string st_transitions, init, fin;
    
    //cout << "Enter your  state-transitions functions:\n"; cin >> st_transitions;
    st_transitions = "(1,a,1) (1,a,2) (1,b,3) (2,a,2) (2,b,1) (2,b,3) (3,a,3) (3,b,3)";
    cout << "\nYour state-transitions functions: \n" << st_transitions << "\n";

    //cout << "Enter your  initial states:\n"; cin >> init;
    //cout << "Enter your  final states:\n"; cin >> fin;
    init = "1", fin = "3";

    cout << "\nYour initial states: \n" << init << " \nYour final states: \n" << fin << "\n\n";

    Converter conv(init, fin);
    conv.NKA_list(st_transitions);
    conv.DKA_print();
}
