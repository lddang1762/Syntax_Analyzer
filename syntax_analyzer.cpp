#include <iostream>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <iomanip>
#include "lexer.h"

/*
<Statement> -> <Assign>         S -> A
<Assign> -> <id> = <Expression>;   A -> id = E

<Expression> -> <Expression> + <Term> | <Expression> - <Term> | <Term>      E -> E+T | E - T | T
  E -> TE'
  E' -> +TE' | -TE' | epsilon

<Term> -> <Term> * <Factor> | <Term> / <Factor> | <Factor>     T -> T*F | T/F | F
  T -> FT'
  T' -> *FT' | /FT' | epsilon

<Factor> -> ( <Expression> ) | <id> | <num>      F -> (E) | id
*/

int index = 0;

bool S();
bool A();
bool E();
bool E_prime();
bool T();
bool T_prime();
bool F();

bool S(){
  //cout << "S\n";
  if(A()){
    cout << "S -> A\n";
    cout << left << setw(25) << "Token: Separator" << "Lexeme: " << setw(25) << lex_stream[++index].lex << endl << endl << endl;
    index ++; // jump over ;
    return true;
  }
  string err = "ERROR: Incorrect Syntax\n";
  throw err;
  return false;
}

bool A(){
  //cout << "A\n";
  if(lex_stream[index].tok_type == 2){ // identifier
    cout << left << setw(25) << "Token: Identifier" << "Lexeme: " << setw(25) << lex_stream[index].lex << endl;
    if(lex_stream[++index].lex == "="){
      cout << left << setw(25) << "Token: Operator" << "Lexeme: " << setw(25) << lex_stream[index].lex << endl;
      if(E()){
        cout << "A -> id = E\n";
        return true;
      }
    }
  }
  return false;
}

bool E(){
  //cout << "E\n";
  if(T()){
    if(E_prime()){
      cout << "E -> TE'\n";
      return true;
    }
  }
  return false;
}

bool E_prime(){
  //cout << "E'\n";
  if(lex_stream[++index].lex == "+"){
    cout << left << setw(25) << "Token: Operator" << "Lexeme: " << setw(25) << lex_stream[index].lex << endl;
    if(T()){
      if(E_prime()){
        cout << "E' -> +TE'\n";
        return true;
      }
    }
  }
  else if(lex_stream[index].lex == "-"){
    cout << left << setw(25) << "Token: Operator" << "Lexeme: " << setw(25) << lex_stream[index].lex << endl;
    if(T()){
      if(E_prime()){
        cout << "E' -> -TE'\n";
        return true;
      }
    }
  }
  else{
    index--;
    cout << "E' -> eps\n";
    return true;
    //epsilon
  }
  return false;
}

bool T(){
  //cout << "T\n";
  if(F()){
    if(T_prime()){
      cout << "T -> FT'\n";
      return true;
    }
  }
  return false;
}

bool T_prime(){
  //cout << "T'\n";
  if(lex_stream[++index].lex == "*"){
    cout << left << setw(25) << "Token: Identifier" << "Lexeme: " << setw(25) << lex_stream[index].lex << endl;
    if(F()){
      if(T_prime()){
        cout << "T' -> *FT'\n";
        return true;
      }
    }
  }
  else if(lex_stream[index].lex == "/"){
    cout << left << setw(25) << "Token: Identifier" << "Lexeme: " << setw(25) << lex_stream[index].lex << endl;
    if(F()){
      if(T_prime()){
        cout << "T' -> /FT'\n";
        return true;
      }
    }
  }
  else{
    index--;
    cout << "T' -> eps\n";
    return true;
    //epsilon
  }
  return false;
}

bool F(){
  //cout << "F\n";
  if(lex_stream[++index].lex == "("){ // identifier
    if(E()){
      if(lex_stream[++index].lex == ")"){
        cout << "F -> (E)\n";
        return true;
      }
    }
  }
  else{
    //cout << "F2\n";
    if(lex_stream[index].tok_type == 2){
      cout << left << setw(25) << "Token: Identifier" << "Lexeme: " << setw(25) << lex_stream[index].lex << endl;
      return true;
    }
    else{
      string err = "ERROR: id expected\n";
      throw err;
    }
  }
  return false;

}



void analyze(){
  while(index != lex_stream.size()){
    try{
      S();
    }
    catch(string e){
      cout << e;
      exit(0);
    }

  }
  // while(S()){
  // }

  //if(index == lex_stream.size()) { } // end of statement
  // string line = "";
  // while((lex_stream[index]).lex != ";"){
  //   line += (lex_stream[index++]).lex;
  // }
  // line += ";";
  // index++;
  //cout << line << endl;
  //return line;
}

int main(int argc, char* argv[]){
  readfile();
  lexer();
  analyze();

  //print_lexemes();
  inFile.close();
  outFile.close();
  return 0;
}

void readfile(){
  string ifname = "", ofname = "";
  // cout << "Enter input file name: ";
  // cin >> ifname;
  try{
    //inFile.open(ifname);
    inFile.open("a.txt");
    if(!inFile.is_open()){
      string err = "Error: File failed to open.\n";
      throw err;
    }
  }
  catch(string e){
    cout << e;
    exit(0);
  }
  // cout << "Enter output file name: ";
  // cin >> ofname;
  // outFile.open(ofname);
  // outFile << left;
}
