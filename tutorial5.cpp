#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
    string player_1_name, player_2_name;
    int player_1_score = 0, player_2_score = 0;

    cout << "Masukkan nama pemain 1: ";
    cin >> player_1_name;

    cout << "Masukkan nama pemain 2: ";
    cin >> player_2_name;

    bool ongoing = true;
    char command;
    int score;
    while (ongoing) {
        system("clear");
        cout << player_1_name << " " << player_1_score << endl;
        cout << player_2_name << " " << player_2_score << endl;

        cout << "A. Pemain 1 poin" << endl;
        cout << "B. Pemain 2 poin" << endl;
        cout << "C. Selesai" << endl;
        cout << "Pilih: ";
        cin >> command;

        switch (command) {
        case 'A':
            cout << "Poin: ";
            cin >> score;
            player_1_score += score;
            break;
        case 'B':
            cout << "Poin: ";
            cin >> score;
            player_2_score += score;
            break;
        case 'C':
            ongoing = false;
            break;
        default:
            cout << "Perintah tidak diketahui" << endl;
        }
    }
    
    system("clear");
    cout << player_1_name << " " << player_1_score << endl;
    cout << player_2_name << " " << player_2_score << endl;

    if (player_1_score > player_2_score) {
        cout << player_1_name << " menang!";
    } else if (player_2_score > player_1_score) {
        cout << player_2_name << " menang!";
    } else {
        cout << "Pertandingan Seri!";
    }

    return 0;
}