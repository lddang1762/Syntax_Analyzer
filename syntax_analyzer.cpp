#include <iostream>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <iomanip>

using namespace std;

ifstream inFile;
ofstream outFile;

vector<string> keywords = {"int", "float", "bool","else","then","if", "endif","while","whileend","do","doend",
                            "for","forend","input","output","and","or","function"};
vector<char> separators = {'\'', '(', ')', '{', '}', '[', ']', ',',':',';'};
vector<char> operators = {'*', '+', '-', '=', '/', '>', '<', '%'};
int DSM_table[9][9] = { {2, 4, 8, 9, 1, 1, 6, 4, 1},
                        {2, 2, 3, 3, 3, 2, 3, 3, 3},
                        {1, 1, 1, 1, 1, 1, 1, 1, 1},
                        {5, 4, 5, 5, 5, 5, 5, 4, 5},
                        {1, 1, 1, 1, 1, 1, 1, 1, 1},
                        {6, 6, 6, 6, 6, 6, 7, 6, 6},
                        {1, 1, 1, 1, 1, 1, 1, 1, 1},
                        {1, 1, 1, 1, 1, 1, 1, 1, 1},
                        {1, 1, 1, 1, 1, 1, 1, 1, 1}};

void lexer();
void readfile();
void analyzeToken(string tok, int state);
int char_to_col(char c);
int transitionState(int state, char c);
bool isAcceptedState(int state);
bool isOperator(char c);
bool isSeparator(char c);
bool isKeyword(string tok);

int main(int argc, char* argv[]){
  readfile();
  lexer();
  inFile.close();
  outFile.close();
  return 0;
}

void readfile(){
  string ifname = "", ofname = "";
  cout << "Enter input file name: ";
  cin >> ifname;
  try{
    inFile.open(ifname);
    if(!inFile.is_open()){
      string err = "Error: File failed to open.\n";
      throw err;
    }
  }
  catch(string e){
    cout << e;
    exit(0);
  }
  cout << "Enter output file name: ";
  cin >> ofname;
  outFile.open(ofname);
  outFile << left;
}

void lexer(){
  int currentState = 1;
  string token = "";
  char c;
  do{
    inFile.get(c);
    if(isAcceptedState(currentState)){
      inFile.unget();
      analyzeToken(token, currentState);
      currentState = 1;
      token = "";
    }
    else{
      if(isSeparator(c) || isOperator(c) || c == '!'){
        if(token.length() == 0){ token += c; }
        else{
          if(currentState != 6){ inFile.unget(); }
        }
      }
      else if((!(c == ' ' || c == '\n' || c == '\t'))){ token += c; }
      currentState = transitionState(currentState - 1, c);
    }
  }while(!inFile.eof());
}

void analyzeToken(string tok, int state){
  if(state == 3){
    if(isKeyword(tok)){
        printf("%-15s %-25s\n", "KEYWORD", tok.c_str());
        outFile << setw(15) << "KEYWORD" << setw(25) << tok << endl;
    }
    else{
        printf("%-15s %-25s\n", "IDENTIFIER", tok.c_str());
        outFile << setw(15) << "IDENTIFIER" << setw(25) << tok << endl;
    }
  }
  else if(state == 5){
      printf("%-15s %-25s\n", "NUMBER", tok.c_str());
      outFile << setw(15) << "NUMBER" << setw(25) << tok << endl;
  }
  else if(state == 7){
      //do nothing, state 7 is a comment block
  }
  else if(state == 8){
      printf("%-15s %-25s\n", "SEPARATOR", tok.c_str());
      outFile << setw(15) << "SEPARATOR" << setw(25) << tok << endl;
  }
  else if(state == 9){
      printf("%-15s %-25s\n", "OPERATOR", tok.c_str());
      outFile << setw(15) << "OPERATOR" << setw(25) << tok << endl;
  }
}

int char_to_col(char c){
  if(isalpha(c)){ return 0; }
  if(isdigit(c)){ return 1; }
  if(c == ' '){ return 4; }
  if(c == '$'){ return 5; }
  if(c == '!'){ return 6; }
  if(c == '.'){ return 7; }
  if(c == '\n' || c == '\t'){ return 8; }
  if(isSeparator(c)){ return 2; }
  if(isOperator(c)){ return 3; }

  return 0;
}

int transitionState(int state, char c){
  int col = char_to_col(c);
  return DSM_table[state][col];
}

bool isAcceptedState(int state){
  if(state == 3 || state == 5 || state == 7 || state == 8 || state == 9)
    return true;
  return false;
}

bool isOperator(char c){
  for(auto iter = operators.begin(); iter != operators.end(); iter++){
    if(c == *iter){
      return true;
    }
  }
  return false;
}

bool isSeparator(char c){
  for(auto iter = separators.begin(); iter != separators.end(); iter++){
    if(c == *iter){
      return true;
    }
  }
  return false;
}

bool isKeyword(string tok){
  for(auto iter = keywords.begin(); iter != keywords.end(); iter++){
    if(tok == *iter){
      return true;
    }
  }
  return false;
}
