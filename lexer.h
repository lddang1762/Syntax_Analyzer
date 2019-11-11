#include <vector>

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

struct lexeme{
  int tok_type; // 1 = keyword, 2 = identifier, 3 = number, 4 = separator, 5 = operator
  string lex;
};

vector<lexeme> lex_stream;

void lexer();
void readfile();
void analyzeToken(string tok, int state);
int char_to_col(char c);
int transitionState(int state, char c);
bool isAcceptedState(int state);
bool isOperator(char c);
bool isSeparator(char c);
bool isKeyword(string tok);
void print_lexemes();


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
      lexeme kw;
      kw.tok_type = 1;
      kw.lex = tok;
      lex_stream.push_back(kw);
      // cout << left << setw(15) << "IDENTIFIER" << setw(25) << kw.lex << endl;
      // outFile << setw(15) << "KEYWORD" << setw(25) << tok << endl;
    }
    else{
      lexeme id;
      id.tok_type = 2;
      id.lex = tok;
      lex_stream.push_back(id);
      //cout << left << setw(15) << "IDENTIFIER" << setw(25) << id.lex << endl;
      // outFile << setw(15) << "IDENTIFIER" << setw(25) << tok << endl;
    }
  }
  else if(state == 5){
    lexeme num;
    num.tok_type = 3;
    num.lex = tok;
    lex_stream.push_back(num);
    // printf("%-15s %-25s\n", "NUMBER", tok.c_str());
    // outFile << setw(15) << "NUMBER" << setw(25) << tok << endl;
  }
  else if(state == 7){
    //do nothing, state 7 is a comment block
  }
  else if(state == 8){
    lexeme sep;
    sep.tok_type = 4;
    sep.lex = tok;
    lex_stream.push_back(sep);
    // printf("%-15s %-25s\n", "SEPARATOR", tok.c_str());
    // outFile << setw(15) << "SEPARATOR" << setw(25) << tok << endl;
  }
  else if(state == 9){
    lexeme op;
    op.tok_type = 5;
    op.lex = tok;
    lex_stream.push_back(op);
    // printf("%-15s %-25s\n", "OPERATOR", tok.c_str());
    // outFile << setw(15) << "OPERATOR" << setw(25) << tok << endl;
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

void print_lexemes(){ //for reference

    for(auto iter = lex_stream.begin(); iter != lex_stream.end(); iter++){
      string token="";
      if(iter->tok_type == 1) {token = "KEYWORD";}
      if(iter->tok_type == 2) {token = "IDENTIFIER";}
      if(iter->tok_type == 3) {token = "NUMBER";}
      if(iter->tok_type == 4) {token = "SEPARATOR";}
      if(iter->tok_type == 5) {token = "OPERATOR";}
      cout << left << setw(15) << token << setw(25) << iter->lex << endl;
    }

}
