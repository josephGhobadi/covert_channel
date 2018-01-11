//
// Created by uoosef on 1/10/18.
//
#include <iostream>
#include <cstring>
using namespace std;

#include "morse.h"

std::string morseEncode(int c) {
    c = toupper(c);
    switch (c) {
        case 'A':
            return "01";
        case 'B':
            return "1000";
        case 'C':
            return "1010";
        case 'D':
            return "100";
        case 'E':
            return "0";
        case 'F':
            return "0010";
        case 'G':
            return "110";
        case 'H':
            return "0000";
        case 'I':
            return "00";
        case 'J':
            return "0111";
        case 'K':
            return "101";
        case 'L':
            return "0100";
        case 'M':
            return "11";
        case 'N':
            return "10";
        case 'O':
            return "111";
        case 'P':
            return "0110";
        case 'Q':
            return "1101";
        case 'R':
            return "010";
        case 'S':
            return "000";
        case 'T':
            return "1";
        case 'U':
            return "001";
        case 'V':
            return "0001";
        case 'W':
            return "011";
        case 'X':
            return "1001";
        case 'Y':
            return "1011";
        case 'Z':
            return "1100";
        case '0':
            return "11111";
        case '1':
            return "01111";
        case '2':
            return "00111";
        case '3':
            return "00011";
        case '4':
            return "00001";
        case '5':
            return "00000";
        case '6':
            return "10000";
        case '7':
            return "11000";
        case '8':
            return "11100";
        case '9':
            return "11110";
        case ' ':
            return "0000011";
        default:
            return "11111";
    }
}

char morseDecode(std::string code){
    if(code == "01"){
        return 'A';
    }else if(code == "1000"){
        return 'B';
    }else if(code == "1010"){
        return 'C';
    }else if(code == "100"){
        return 'D';
    }else if(code == "0"){
        return 'E';
    }else if(code == "0010"){
        return 'F';
    }else if(code == "110"){
        return 'G';
    }else if(code == "0000"){
        return 'H';
    }else if(code == "00"){
        return 'I';
    }else if(code == "0111"){
        return 'J';
    }else if(code == "101"){
        return 'K';
    }else if(code == "0100"){
        return 'L';
    }else if(code == "11"){
        return 'M';
    }else if(code == "10"){
        return 'N';
    }else if(code == "111"){
        return 'O';
    }else if(code == "0110"){
        return 'P';
    }else if(code == "1101"){
        return 'Q';
    }else if(code == "010"){
        return 'R';
    }else if(code == "000"){
        return 'S';
    }else if(code == "1"){
        return 'T';
    }else if(code == "001"){
        return 'U';
    }else if(code == "0001"){
        return 'V';
    }else if(code == "011"){
        return 'W';
    }else if(code == "1001"){
        return 'X';
    }else if(code == "1011"){
        return 'Y';
    }else if(code == "1100"){
        return 'Z';
    }else if(code == "11111"){
        return '0';
    }else if(code == "01111"){
        return '1';
    }else if(code == "00111"){
        return '2';
    }else if(code == "00011"){
        return '3';
    }else if(code == "00001"){
        return '4';
    }else if(code == "00000"){
        return '5';
    }else if(code == "10000"){
        return '6';
    }else if(code == "11000"){
        return '7';
    }else if(code == "11100"){
        return '8';
    }else if(code == "11110"){
        return '9';
    }else if(code == "0000011"){
        return ' ';
    }else
        return '0';
}
