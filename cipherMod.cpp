#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <bitset>
#include <cmath>
#include<vector>
#include <fstream>
/*
Amado Garcia 181469
Sara Zavala 18893
Proyecto 1: Encriptador DES
26/09/2019
*/

using namespace std;
int counter = 0;
//variables globales
string text;
vector<string> parser;
vector<string> keys;
//Funcion para escribir sobre un archivo txt
void writeFile(string text) {
	int counter = 1;
	// ofstream writePrimes("primes.txt", ios::out);
	ofstream write("FUENTE.txt", ios::app);
	if (!write) {
		cerr << "Fail to create FUENTE.txt" << endl;
		exit(EXIT_FAILURE);
	}
	write << text;

}
//Cambiar el orden de bits a la izquierda
string shiftleft(string lado) {
    string nlado;
    for (int i = 1; i <= 27; i++) {
        string a(1, lado[i]);
        nlado = nlado + a;
    }
    nlado = nlado + "0";
    return nlado;
}
//Generador de llaves para todas las rondas necesarias
vector<string> keyGenerator(string iniStr) {
    int KeyC[48] = {14, 17, 11, 24, 1, 5, 3, 28,
        15, 6, 21, 10, 23, 19, 12, 4,
        26, 8, 16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55, 30, 40,
        51, 45, 33, 48, 44, 49, 39, 56,
        34, 53, 46, 42, 50, 36, 29, 32};
    int ParityBit[56] = { 57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2
        , 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39
        , 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37
        , 29, 21, 13, 5, 28, 20, 12, 4};
    string temp = "";
    for (int q = 0; q < 56; q++) {
        int w = ParityBit[q];
        string c(1, iniStr[w - 1]);
        temp += c;
    }
    iniStr = temp;
    vector<string> llaves;
    string derecha = iniStr.substr(0, 28);
    string izquier = iniStr.substr(28, 56);;
    
    for (int j = 0; j < 8; j++) {
        derecha = shiftleft(derecha);
        izquier = shiftleft(izquier);
        
        string todo = derecha + izquier;
        string co = "";
        for (int i = 0; i < 48; i++) {
            int a = KeyC[i];
            string b(1, todo[a - 1]);
            co += b;
        }
        llaves.push_back(co);
    }
    return llaves;
}
//Funcion de string a binario
string strtobin(string iniStr) {
    string bitstr = "";
    for (int i = 0; i < iniStr.length(); i++) {
        char current = iniStr[i];
        string hola = bitset<8>(current).to_string();
        bitstr = bitstr + hola;
    }
    return bitstr;
}

//Funcion para leer un txt
void readFile(){
    string temp_text;
    ifstream readText("./FUENTE.txt",ios::in);
    if(!readText)
    {
        cerr<<"Fail to read FUENTE.txt"<<endl;
        exit(EXIT_FAILURE);
    }
    while (readText>>temp_text)
    {
        text = text + temp_text;
        
    }
    
}
//Dividir las palabras en bits
vector<string> divide() {
    vector<string> all8letterwords;
    string current = "";
    for (int i = 0; i < text.length(); i = i + 8)
    {
        current = text.substr(i, 8);
        if (current.length() == 8) {
            all8letterwords.push_back(current);
        }
        else {
            int act = current.length();
            int todo = 8 - act;
            for (int j = 0; j < todo; ++j) {
                current += " ";
            }
            all8letterwords.push_back(current);
        }
    }
    return all8letterwords;
}

//Funcion binario a Decimal
int binaryToDecimal(string n)
{
    string num = n;
    int dec_value = 0;
    
    // Initializing base value to 1, i.e 2^0
    int base = 1;
    
    int len = num.length();
    for (int i = len - 1; i >= 0; i--)
    {
        if (num[i] == '1')
            dec_value += base;
        base = base * 2;
    }
    
    return dec_value;
}
//Binario a String
string bintostr(string iniStr)
{
    string normalstr = "";
    string current = "";
    for (int i = 0; i < iniStr.length(); i=i+8)
    {
        for (int j = 0; j < 8; j++)
        {
            string a(1, iniStr[i + j]);
            current += a;
        }
        int number = binaryToDecimal(current);
        string x(1, char(number));
        normalstr = normalstr + x;
    }
    return normalstr;
}

//Funcion para convertir a binario
string toBinary(int n)
{
    string r;
    while(n!=0) {r=(n%2==0 ?"0":"1")+r; n/=2;}
    return r;
}
string s1(string row, string column)
{
    int s1Matrix[4][16] = {
        {14, 4,  13, 1, 2,  15, 11, 8,  3,  10, 6,  12, 5,  9,  0, 7},
        {0,  15, 7,  4, 14, 2,  13, 10, 3,  6,  12, 11, 9,  5,  3, 8},
        {4,  1,  14, 8, 13, 6,  2,  11, 15, 12, 9,  7,  3,  10, 5, 0},
        {15, 12, 8,  2, 4,  9,  1,  7,  5,  11, 3,  14, 10, 0,  6, 13}};
    
    int value = s1Matrix[stoi(row)][stoi(column)];
    // cout<<row<<"-"<<column<<"\n";
    //  cout<<value<<"\n";
    string result = toBinary(value);
    
    if(result == "")
    {
        result = "0000";
    }
    if (result.length() == 1) {
        string temp = result;
        result = "000" + temp;
    }
    if (result.length() == 2) {
        string temp = result;
        result = "00" + temp;
    } else if (result.length() == 3) {
        string temp = result;
        result = "0" + temp;
    }
    return result;
    
}

string XOR(string a, string  b)
{
    
    if(a == "1" && b =="1")
    {
        return "0";
    }
    if(a == "0" && b =="0")
    {
        return "0";
    }
    if(a == "1" && b =="0")
    {
        return "1";
    }
    if(a == "0" && b =="1")
    {
        return "1";
    }
    
}

int getS1Row(string text)
{
    
    string row;
    row +=text[0];
    row +=text[5];
    int result;
    
    
    result = binaryToDecimal(row);
    return result;
}

int getS1Column(string text)
{
    string column;
    
    for (int i = 1; i < 5; ++i) {
        column += text[i];
    }
    int result = binaryToDecimal(column);
    
    return result;
}


string straightPermutate(string s1bit32Word)
{
    counter++;
    //15, 6, 19, 20, 28, 11, 27, 16, 0, 14, 22, 25, 4, 17, 30, 9, 1, 7, 23, 13, 31, 26, 2, 8, 18, 12, 29, 5, 21, 10, 3, 24
    int sPTable[32] = {15, 6, 19, 20, 28, 11, 27, 16, 0, 14, 22, 25, 4, 17, 30, 9, 1, 7, 23, 13, 31, 26, 2, 8, 18, 12, 29, 5, 21, 10, 3, 24};
    string permutateds1Bit32Word;
    for (int i = 0; i < s1bit32Word.length(); ++i) {
        int position = sPTable[i];
        permutateds1Bit32Word += s1bit32Word[position];
    }
    
    return permutateds1Bit32Word;
    
}

string DES(string bit32Right, string bitKey48)
{
    string bit48Word;
    bit48Word += bit32Right[31];
    int counter = 0;
    int lineCounter = 1;
    int counterChange = 1;
    bool firstIteration = true;
    while (lineCounter < 8) {
        bit48Word += bit32Right[counter];
        counter++;
        counterChange++;
        if (counter == 4 && firstIteration == true) {
            firstIteration = false;
            bit48Word += bit32Right[counter];
            counter = 3;
            counterChange = 1;
        }
        
        if (counter == 32) {
            bit48Word += bit32Right[0];
            break;
        }
        
        if (counterChange == 6) {
            bit48Word += bit32Right[counter];
            counter = counter - 1;
            counterChange = 1;
            lineCounter++;
        }
    }
    
    
    string tempBit48Word = bit48Word;
    bit48Word = "";
    for (int i = 0; i < 48; i++) {
        string a(1, tempBit48Word[i]);
        string b(1, bitKey48[i]);
        bit48Word += XOR(a, b);
    }
    
    //cout<<bit48Word<<"\n";
    string splitter[8] = {};
    int lowLimit = 0;
    int highLimit = 5;
    for (int j = 0; j < 8; ++j) {
        for (int k = lowLimit;; ++k) {
            //cout<<bit48Word[k];
            splitter[j] += bit48Word[k];
            if (k == highLimit) {
                highLimit += 6;
                lowLimit += 6;
                break;
            }
        }
    }
    
    string bit32RightWord;
    
    for (int l = 0; l < 8; ++l) {
        int s1Row = getS1Row(splitter[l]);
        int s1Column = getS1Column(splitter[l]);
        
        //cout<<splitter[l]<<"\n";
        bit32RightWord += s1(to_string(s1Row), to_string(s1Column));
    }
    //    cout<<bit32RightWord<<"\n";
    string permutatedbit32RightWord = straightPermutate(bit32RightWord);
    return permutatedbit32RightWord;
}


string initper(string cift)
{
    vector<string> g1;
    for (int i = 0; i < cift.length(); i++)
    {
        string a(1, cift[i]);
        g1.push_back(a);
    }
    
    string primera;
    int changeOfLine = 0;
    int valueChange = 0;
    int i = 57;
    int j = 0;
    while (true)
    {
        primera += g1[i];
        i = i - 8;
        changeOfLine++;
        if (i == 6)
        {
            break;
        }
        if (changeOfLine == 8)
        {
            changeOfLine = 0;
            i = i + 58 + 8;
            valueChange++;
        }
        
        if (valueChange == 4)
        {
            i = 56;
            valueChange = 5;
        }
    }
    
    return primera;
}
string finper(string cift)
{
    string ultima = "";
    vector<string> g1;
    for (int i = 0; i < cift.length(); i++)
    {
        string a(1, cift[i]);
        g1.push_back(a);
    }
    ultima = ultima + g1[39] + g1[7] + g1[47] + g1[15] + g1[55] + g1[23] + g1[63] + g1[31] + g1[38] + g1[6] + g1[46] + g1[14] + g1[54] + g1[22] + g1[62] + g1[30];
    ultima = ultima + g1[37] + g1[5] + g1[45] + g1[13] + g1[53] + g1[21] + g1[61] + g1[29] + g1[36] + g1[4] + g1[44] + g1[12] + g1[52] + g1[20] + g1[60] + g1[28];
    ultima = ultima + g1[35] + g1[3] + g1[43] + g1[11] + g1[51] + g1[19] + g1[59] + g1[27] + g1[34] + g1[2] + g1[42] + g1[10] + g1[50] + g1[18] + g1[58] + g1[26];
    ultima = ultima + g1[33] + g1[1] + g1[41] + g1[9] + g1[49] + g1[17] + g1[57] + g1[25] + g1[32] + g1[0] + g1[40] + g1[8] + g1[48] + g1[16] + g1[56] + g1[24];
    return ultima;
}



string feistelRounds(string bit64Word, vector<string> keys){
    string left32Bits;
    string right32Bits;
    for (int j = 0; j < 8; ++j) {
        left32Bits = bit64Word.substr(0, 32);
        right32Bits = bit64Word.substr(32, 32);
        
        string tempright32Bits = right32Bits;
        string unmutableRight32Bits = right32Bits;
        string key = keys[j];
        tempright32Bits = DES(tempright32Bits, keys[j]);
        
        right32Bits = "";
        
        for (int i = 0; i < 32; ++i) {
            string a(1, tempright32Bits[i]);
            string b(1, left32Bits[i]);
            right32Bits += XOR(a, b);
        }
        
        left32Bits = unmutableRight32Bits;
        
        if (j < 8) {
            string swapped;
            swapped += left32Bits;
            swapped += right32Bits;
            bit64Word = swapped;
        }
        
        if (j == 8) {
            bit64Word = "";
            bit64Word += left32Bits;
            bit64Word += right32Bits;
            bit64Word = finper(bit64Word);
        }
        
    }
    bit64Word = bintostr(bit64Word);
    return bit64Word;
}
void cipher(string completeword)
{
	string binario = strtobin(completeword);
	string perini = initper(binario);
	string cifrado = feistelRounds(perini, keys);
	writeFile(cifrado);
}
int main()
{
	readFile();
	parser = divide();
	keys = keyGenerator(strtobin("ABCDEFGH"));
	for (i = 0; i < parser.size; i++) {
		cipher(parser[i]);
	}
}
