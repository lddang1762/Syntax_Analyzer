#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <iomanip>
#include "lexer.h"
//                           id  =   +  -   *    /   (   )   ;   , int fl bo  $
int parse_table[10][14] = { {1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2, 2, 2, -1},
                            {3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                            {4, -1, -1, -1, -1, -1,  4, -1, -1, -1, -1, -1, -1, -1},
                            {1, -1,  5,  6, -1, -1, -1,  0,  0, -1, -1, -1, 2,  0},
                            {8, -1, -1, -1, -1, -1,  8, -1, -1, -1, -1, -1, -1, -1},
                            {1, -1,  0,  0,  9, 10, -1,  0,  0, -1, -1, -1, -1,  0},
                            {12, -1, -1, -1, -1, -1, 13, -1, -1, -1, -1, -1, -1, -1},
                            {1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14, 14, 14,  0},
                            {1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 16, 17, 18, -1},
                            {1, -1, -1, -1, -1, -1, -1, -1,  0, 19, -1, -1, -1,  0} };



enum Symbols{
  //Non-terminal Symbols:
  N_S,  // S
  N_A,  // A
  N_E,  // E
  N_E_PRIME,  // E'
  N_T,  // T
  N_T_PRIME,  // T'
  N_F,  // F
  N_D,  // D
  N_TYPE, // Ty
  N_M,  // M

  //Terminal Symbols:

  T_ID, // id
  T_EQ, // =
  T_PLUS, // +
  T_MINUS,  // -
  T_MUL,  // *
  T_DIV,  // /
  T_L_PARENS, // (
  T_R_PARENS, // )
  T_S_COLON, // ;
  T_COMMA,  // ,
  T_INT,  // int
  T_FLOAT, // float
  T_BOOL,  // bool
  T_EOS,  // $

  T_ERR // error
};

stack<Symbols> s_stack;

void parse(vector<token> &statement);
void rule(int ruleNum);
Symbols tok_to_sym(token tok);
vector<token> getStatement(vector<token> &statement);

int main(int argc, char* argv[]){
  readfile();
  lexer();
  while(stream_ptr != tok_stream.size()){
    vector<token> statement;
    getStatement(statement);
    try{
      parse(statement);
    }
    catch(string e){
      cout << e;
      exit(0);
    }
  }
  //print_tokens();
  inFile.close();
  outFile.close();
  return 0;
}

void parse(vector<token> &statement){
  int statement_ptr = 0;
  bool print = true;
  //subtract 10 from terminals
  //s_stack.push(T_EOS);
  //put $ end of string
  s_stack.push(N_S);
  while(!s_stack.empty()){
    Symbols TOS = s_stack.top();
    token i = statement[statement_ptr];
    if(print){
      cout << endl;
      outFile << endl;
      i.print();
      print = false;
    }
    if(TOS >= 10 && TOS <= 23){ // if terminal
      if(TOS == tok_to_sym(i)){
        s_stack.pop();
        statement_ptr++;
        print = true;
      }
      else{
        string err = "Error: Incorrect Syntax.\n";
        throw err;
        break;
      }
    }
    else{
      if(parse_table[TOS][tok_to_sym(i) - 10] != -1){ // subtract 10 in order to match table index
        s_stack.pop();
        if(parse_table[TOS][tok_to_sym(i) - 10] == 0){
          if(TOS == N_E_PRIME){ cout << "\t<Expression Prime> -> "; outFile << "\t<Expression Prime> -> "; }
          else if(TOS == N_T_PRIME){ cout << "\t<Term Prime> -> "; outFile << "\t<Term Prime> -> "; }
          else if(TOS == N_D){ cout << "\t<Declarative> -> "; outFile << "\t<Declarative> -> "; }
          else if(TOS == N_M){ cout << "\t<MoreIds> -> "; outFile << "\t<MoreIds> -> "; }
        }
        rule(parse_table[TOS][tok_to_sym(i) - 10]);
      }
      else{
        string err = "Error: Incorrect Syntax.\n";
        throw err;
        break;
      }
    }
  }
}

void rule(int ruleNum){
  switch(ruleNum){
    case 0:
    cout << "<Epsilon>\n";
    outFile << "<Epsilon>\n";
    break;
    case 1:
      s_stack.push(N_A);
      cout << "\t<Statement> -> <Assign>\n";
      outFile << "\t<Statement> -> <Assign>\n";
      break;
    case 2:
      s_stack.push(N_D);
      cout << "\t<Statement> -> <Declarative>\n";
      outFile << "\t<Statement> -> <Declarative>\n";
      break;
    case 3:
      s_stack.push(T_S_COLON);
      s_stack.push(N_E);
      s_stack.push(T_EQ);
      s_stack.push(T_ID);
      cout << "\t<Assign> -> <Identifier> = <Expression> ;\n";
      outFile << "\t<Assign> -> <Identifier> = <Expression> ;\n";
      break;
    case 4:
      s_stack.push(N_E_PRIME);
      s_stack.push(N_T);
      cout << "\t<Expression> -> <Term> <Expression Prime>\n";
      outFile << "\t<Expression> -> <Term> <Expression Prime>\n";
      break;
    case 5:
      s_stack.push(N_E_PRIME);
      s_stack.push(N_T);
      s_stack.push(T_PLUS);
      cout << "\t<Expression Prime> -> + <Term> <Expression Prime>\n";
      outFile << "\t<Expression Prime> -> + <Term> <Expression Prime>\n";
      break;
    case 6:
      s_stack.push(N_E_PRIME);
      s_stack.push(N_T);
      s_stack.push(T_MINUS);
      cout << "\t<Expression Prime> -> - <Term> <Expression Prime>\n";
      outFile << "\t<Expression Prime> -> - <Term> <Expression Prime>\n";
      break;
    case 8:
      s_stack.push(N_T_PRIME);
      s_stack.push(N_F);
      cout << "\t<Term> -> <Factor> <Term Prime>\n";
      outFile << "\t<Term> -> <Factor> <Term Prime>\n";
      break;
    case 9:
      s_stack.push(N_T_PRIME);
      s_stack.push(N_F);
      s_stack.push(T_MUL);
      cout << "\t<Term Prime> -> * <Factor> <Term Prime>\n";
      outFile << "\t<Term Prime> -> * <Factor> <Term Prime>\n";
      break;
    case 10:
      s_stack.push(N_T_PRIME);
      s_stack.push(N_F);
      s_stack.push(T_DIV);
      cout << "\t<Term Prime> -> / <Factor> <Term Prime>\n";
      outFile << "\t<Term Prime> -> / <Factor> <Term Prime>\n";
      break;
    case 12:
      s_stack.push(T_ID);
      cout << "\t<Factor> -> <Identifier>\n";
      outFile << "\t<Factor> -> <Identifier>\n";
      break;
    case 13:
      s_stack.push(T_R_PARENS);
      s_stack.push(N_E);
      s_stack.push(T_L_PARENS);
      cout << "\t<Factor> -> ( <Expression> )\n";
      outFile << "\t<Factor> -> ( <Expression> )\n";
      break;
    case 14:
      s_stack.push(T_S_COLON);
      s_stack.push(N_M);
      s_stack.push(T_ID);
      s_stack.push(N_TYPE);
      cout << "\t<Declarative> -> <Type> <Identifier> <MoreIds>;\n";
      outFile << "\t<Declarative> -> <Type> <Identifier> <MoreIds>;\n";
      break;
    case 16:
      s_stack.push(T_INT);
      cout << "\t<Type> -> int\n";
      outFile << "\t<Type> -> int\n";
      break;
    case 17:
      s_stack.push(T_FLOAT);
      cout << "\t<Type> -> float\n";
      outFile << "\t<Type> -> float\n";
      break;
    case 18:
      s_stack.push(T_BOOL);
      cout << "\t<Type> -> bool\n";
      outFile << "\t<Type> -> bool\n";
      break;
    case 19:
      s_stack.push(N_M);
      s_stack.push(T_ID);
      s_stack.push(T_COMMA);
      cout << "\t<MoreIds> -> , <Identifier> <MoreIds>\n";
      outFile << "\t<MoreIds> -> , <Identifier> <MoreIds>\n";
      break;
    default:
      cout << "Error: Big Oops.\n";
      break;
  }
}

Symbols tok_to_sym(token tok){
  if(tok.tok_type == 2){ return T_ID; }
  if(tok.lex == "="){ return T_EQ; }
  if(tok.lex == "+"){ return T_PLUS; }
  if(tok.lex == "-"){ return T_MINUS; }
  if(tok.lex == "*"){ return T_MUL; }
  if(tok.lex == "/"){ return T_DIV; }
  if(tok.lex == "("){ return T_L_PARENS; }
  if(tok.lex == ")"){ return T_R_PARENS; }
  if(tok.lex == ";"){ return T_S_COLON; }
  if(tok.lex == ","){ return T_COMMA; }
  if(tok.lex == "int"){ return T_INT; }
  if(tok.lex == "float"){ return T_FLOAT; }
  if(tok.lex == "bool"){ return T_BOOL; }
  return T_ERR;
}

vector<token> getStatement(vector<token> &statement){
  while(stream_ptr != tok_stream.size() && tok_stream[stream_ptr].lex != ";"){
    statement.push_back(tok_stream[stream_ptr++]);
  }
  if(stream_ptr != tok_stream.size()){
    statement.push_back(tok_stream[stream_ptr++]);
  }
  return statement;
}

void readfile(){
  string ifname = "", ofname = "";
  cout << "Enter input file name: ";
  cin >> ifname;
  try{
    inFile.open(ifname);
    //inFile.open("a.txt");
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
}
