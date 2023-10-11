#include <iostream>
#include <string.h>
#include <bitset>

using namespace std;


// LEVEL 1 CODE
bool isPasscode(string passcode) {
    string binaryString = "";
    for (int i = 0; i < passcode.size(); i++) {
        binaryString += bitset<8>(passcode.c_str()[i]).to_string();
    }
    return binaryString == "010100110110000101111001010010000110010101101100011011000110111101010100011011110100110101111001010011000110100101110100011101000110110001100101010001100111001001101001011001010110111001100100";
}

// MAIN FUNCTION

int main() {
    string input;
    cout << "What is the secret passcode?" << endl;
    getline(cin, input);

    // Layer 1, simple reversing of what this code does; passcode will be SayHelloToMyLittleFriend
    if (isPasscode(input)) {
        cout << "This works" << endl;
    } else {
        cout << "Level 1 Failed" << endl;
    }
}