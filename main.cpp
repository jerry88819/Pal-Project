# include <iostream>
# include <stdio.h>
# include <cstring>
# include <sstream>
# include <vector>
# include <iomanip>
# include <queue>
# include <stack>
# include <string>
# include <string.h>
# include <stdlib.h>
# include <ctype.h>
# include <exception>

using namespace std;

typedef enum Type {
  // constant  ==  true false 'char' "string" num

  PLUS, MINUS, MUL, DIV,  ID, SIGNNUM, LEFTPAR, RIGHTPAR, NUM,
  // +  -  *  /  123  asd  +123 (  )
  ASSIGN, SEMICOLON, SIGNNUMWITHPOINT, NUMWITHPOINT, // �o�ӬO���F ���p���I���Ʀr
  // :=  ;           +1.23             12.3
  INT, FLOAT, BOOL, CHAR, STRING, VOID, IF, ELSE, WHILE, DO, RETURN,

  LEFTMID, RIGHTMID, LEFTBIG, RIGHTBIG, PERCENTAGE, GREATER, LESSER, GE, LE,
  //  [         ]      {        }           %          >        <     >=  <=
  EQ, NEQ, SAND, SOR, SEQUAL, EXCLAMATION,
  //  ==  !=  &   |      =         !
  AND, OR, PE, ME, TE, DE, RE, PP, MM, RS, LS,
  // && ||  +=  -= *=  /=  %=  ++  --  >>  <<

  QUES, COMMA, COLON,
  // ?  ,  :
  CARET, CONSTANT,  // ^

  TRUE, FALSE, ONEPOINTCHAR, TWOPOINTSTRING, CIN, COUT
}
T;


struct Token {

  string content ;
  T type ; // �� enum �ۭq�q�����O
  T constantDetailType ; // pal_3 �|�Ψ�
  int line ;
  float value ;
  bool isfunction ;
  string ifisarray ; // store array. ex : a[30]  store the part of [30]
  string idtype ; // store what type is the ID is.

}; // Token()


// �����ܼư�

int gline_num = 0 ;
int gEnd = 0 ; // ���ӨS�� ���i�o��
int guseless = 0 ;
int gthrowspecial = 0 ;
string g_if_function_wrong ;
string gnowtype ;

bool gIsEnd = false ;

bool gIs_Definition = false ;
bool gGlobal_mode = true ; // ���`���p ����  global mode �o�O�� definition �����L
bool gIs_function = false ; // �O function ���ܤ��� statement executed
bool gIs_If_Else_ = false ;
bool gPass_compound = false ;

vector <Token> gPeekBuffer ; // �ΨӦs���ݪ� token
vector <Token> gCorrectGrammer ; // �ΨӦs���T����k Token
vector <Token> gDefinitionglobal ;
vector <Token> gTempDglobal ;
vector <Token> gTempDlocal ; // local �Χ��N��F �s�@�ӴN�i�H

vector <string> gSystemFunction ;
vector <string> gFunctionName ; // �u�s function ���W�r
vector < vector <Token> > gWholeFunction ; // ��  function �{���X��

// Function ��

void Testall() {

  cout << "+++++++++++ Test All ++++++++++++" << endl ;
  for ( int i = 0 ; i < gDefinitionglobal.size() ; i ++ ) {
    cout << "Global : " << gDefinitionglobal[i].content ;
    cout << "   " << gDefinitionglobal[i].ifisarray << "   " << gDefinitionglobal[i].idtype << endl ;
  } // for()

  for ( int i = 0 ; i < gTempDglobal.size() ; i ++ ) {
    cout << "temp : " << gTempDglobal[i].content << endl ;
  } // for()

  for ( int i = 0 ; i < gTempDlocal.size() ; i ++ ) {
    cout << "Local : " << gTempDlocal[i].content << endl ;
  } // for()

  for ( int i = 0 ; i < gFunctionName.size() ; i ++ ) {
    cout << "Function name : " << gFunctionName[i] << endl ;
  } // for()

  for ( int i = 0 ; i < gWholeFunction.size() ; i ++ ) {
    cout << "whole name : " << gWholeFunction[i][1].content << endl ;
  } // for()

  for ( int i = 0 ; i < gWholeFunction.size() ; i ++ ) {
    for ( int p = 0 ; p < gWholeFunction[i].size() ; p ++ ) {
      cout << gWholeFunction[i][p].content ;
    } // for()

    cout << endl ;
  } // for()

  if ( gIs_Definition ) cout << "gIs_Definition" << endl ;
  else if ( gGlobal_mode ) cout << "gGlobal_mode" << endl ;
  else if ( gIs_function ) cout << "gIs_function" << endl ;
  else if ( gIs_If_Else_ ) cout << "gIs_If_Else_" << endl ;
  else if ( gPass_compound ) cout << "gPass_compound" << endl ;

  if ( ! gIs_Definition ) cout << "Not gIs_Definition" << endl ;
  else if ( ! gGlobal_mode ) cout << "Not gGlobal_mode" << endl ;
  else if ( ! gIs_function ) cout << "Not gIs_function" << endl ;
  else if ( ! gIs_If_Else_ ) cout << "Not gIs_If_Else_" << endl ;
  else if ( ! gPass_compound ) cout << "Not gPass_compound" << endl ;

  cout << "+++++++++++ Test All End ++++++++++++" << endl ;

} // Testall()


char F1() ; // F1()
char F2() ; // F2()
string F3( char a ) ; // F3()
string F4( char a ) ;
string F5( char a ) ;
void ReadtoEnd() ;
bool Realfloat( string a ) ;
Token SetToken( T type, string name ) ;
Token GetToken() ;

char F1() {

  char a ;
  a = getchar() ;

  if ( a == '\n' ) {
    gline_num = gline_num + 1 ;
  } // if()

  return a ;

} // F1()  Ū���r�� �� �O����C

char F2() {

  char first ;
  char peek ;
  first = F1() ;

  while ( first == ' ' || first == '\n' || first == '\t' || first == '/' ) {

    if ( first == '/' ) {
      peek = cin.peek() ;

      if ( peek == '/' ) { // �N��X�{ comment "//"
        peek = F1() ;  // ����ĤG�ӱ׽uŪ��
        first = F1() ;
        while ( first != '\n' ) {
          first = F1() ;
        } // while()  Ū���� ���� '\n' ����
      } // if() �J��ĤG�ӵ��ѱ׽u '/'

      else { // �N��u�O���k
        return first ;
      } // else()

    } // if()  �J��Ĥ@�ӵ��ѱ׽u '/'

    else if ( first == ' ' || first == '\n' || first == '\t' ) {
      first = F1() ;
    } // else if() �Owhite-space �N�h�s F1() Ū���r��


  }  // while() white-space �N�h�s F1() Ū���r��

  return first ;
} // F2()  �t�d���Lcomment �� ���owhitespace�᪺�Ĥ@�Ӧr��

//  ///////////////////////////////////////////pal�w�ץ������ˬd

string F3( char a ) {

  string temp = "" ;
  char tchar ;

  temp = temp + a ;

  if ( a == '+' || a == '-' || a == '*' || a == '/' || a == '=' || a == ';' || a == '(' || a == ')' ) {

    return temp ;

  } // if() ��@���I�Ÿ�

  else if ( a == ':' ) {

    tchar = cin.peek() ;  // ���� �T�w�O�n�o�A��

    if ( tchar == '=' ) {

      tchar = F1() ;
      temp = temp + tchar ;

    } // if()   �O ':=' �����p

    else {

      string error = "" ;
      error = "Unrecognized token with first char : '" + temp + "'" ;
      ReadtoEnd() ;
      throw error ;

    } // else()

  } // else if()

  else if ( a == '>' ) {

    tchar = cin.peek() ;  // ���� �T�w�O�n�o�A��

    if ( tchar == '=' ) {

      tchar = F1() ;
      temp = temp + tchar ;

    } // if()   �O '>=' �����p

  } // else if()

  else if ( a == '<' ) {

    tchar = cin.peek() ;  // ���� �T�w�O�n�o�A��

    if ( tchar == '=' ) {

      tchar = F1() ;
      temp = temp + tchar ;

    } // if()   �O '<=' �����p

    else if ( tchar == '>' ) {

      tchar = F1() ;
      temp = temp + tchar ;

    } // else if() �O '<>'�����p

  } // else if()



  return temp ;


} // F3()  �t�d���I�Ÿ�

string F4( char a ) {

  string temp = "" ;
  char tchar ;

  temp = temp + a ;

  tchar = cin.peek() ;  // ���� �T�w�O�n�o�A��

  while ( ( tchar >= 65 && tchar <= 90 ) || ( tchar >= 97 && tchar <= 122 )
          || ( tchar >= 48 && tchar <= 57 ) || tchar == '_' ) {

    tchar = F1() ;  // �T�w�O�n���N��  F1() �h���r��
    temp  = temp + tchar ;
    tchar = cin.peek() ;

  } // while()  �O a - z  A - Z  0 - 9  '_'  ( ID �᭱�౵�o�� )

  return temp ;

} // F4()  �t�d Id

string F5( char a ) {

  string temp = "" ;
  char tchar ;
  int countpoint = 0 ; // ( a >= 48 && a <= 57 )

  temp = temp + a ;

  if ( a == '.' ) { // �� .3 .03 .156 ......�����p

    tchar = cin.peek() ;

    while ( tchar >= 48 && tchar <= 57 ) {  // ���p peek �쪺 next char �� 0 - 9 ����

      tchar = F1() ; // �T�w�O�n���N��  F1() �h���r��
      temp = temp + tchar ;
      tchar = cin.peek() ;

    } // while()
  } // if()

  else if (  a >= 48 && a <= 57 ) {

    tchar = cin.peek() ;

    while ( tchar >= 48 && tchar <= 57 ) {  // ���p peek �쪺 next char �� 0 - 9 ����

      tchar = F1() ; // �T�w�O�n���N��  F1() �h���r��
      temp = temp + tchar ;
      tchar = cin.peek() ;

    } // while()

    if ( tchar == '.' ) {

      tchar = F1() ; // �T�w�O�n���N��  F1() �h���r��
      temp = temp + tchar ;
      tchar = cin.peek() ;

      while ( tchar >= 48 && tchar <= 57 ) {  // ���p peek �쪺 next char �� 0 - 9 ����

        tchar = F1() ; // �T�w�O�n���N��  F1() �h���r��
        temp = temp + tchar ;
        tchar = cin.peek() ;

      } // while()
    } // if()  ��  1.022  213.066 ......�����p
  } // else if()

  return temp ;

} // F5()  �t�d Num

void ReadtoEnd() {  // Ū���@��
  char a ;
  a = F1() ;

  while ( a != '\n' ) {

    a = F1() ;

  } // while()

  // cout << "Reading to End " <<endl ;


} // ReadtoEnd()

bool Realfloat( string a ) {

  for ( int i = 0 ; i < a.length() ; i++ ) {
    if ( a[i] == '.' ) {
      return true ;
    } // if()
  } // for()

  return false ;
} // Realfloat()

Token SetToken( T type, string name ) {
  Token temp ;
  temp.content = name ;
  temp.type = type ;

  if ( type == CONSTANT ) {

    if ( name == "true" ) {
      temp.constantDetailType = TRUE ;
    } // if()

    else if ( name == "false" ) {
      temp.constantDetailType = FALSE ;
    } // else if()

    else if ( name[0] == '\"' ) {
      temp.constantDetailType = TWOPOINTSTRING ;
    } // else if()

    else if ( name[0] == '\'' ) {
      temp.constantDetailType = ONEPOINTCHAR ;
    } // else if()

    else if ( Realfloat( name ) ) {
      temp.constantDetailType = NUMWITHPOINT ;
    } // else if()

    else {
      temp.constantDetailType = NUM ;
    } // else()
  } // if()

  return temp ;

} // SetToken()

Token GetToken() {

  char target ;
  string tempans ;
  Token ans ;
  target = F2() ; // ���o�Ĥ@�Ӧr���h�P�_

  // PAL_2 ���s��

  if ( ( target >= 65 && target <= 90 ) || ( target >= 97 && target <= 122 )  ) { // ���� id �����p
        // ID�����p ( �b A - Z a - z ���� )

    tempans = F4( target ) ; // ������r��

    if ( tempans.size() == 3 && tempans == "int" ) {
      ans = SetToken( INT, tempans ) ;
    } // if()

    else if ( tempans.size() == 5 && tempans == "float" ) {
      ans = SetToken( FLOAT, tempans ) ;
    } // else if()

    else if ( tempans.size() == 4 && tempans == "char" ) {
      ans = SetToken( CHAR, tempans ) ;
    } // else if()

    else if ( tempans.size() == 4 && tempans == "bool" ) {
      ans = SetToken( BOOL, tempans ) ;
    } // else if()

    else if ( tempans.size() == 6 && tempans == "string" ) {
      ans = SetToken( STRING, tempans ) ;
    } // else if()

    else if ( tempans.size() == 4 && tempans == "void" ) {
      ans = SetToken( VOID, tempans ) ;
    } // else if()

    else if ( tempans.size() == 2 && tempans == "if" ) {
      ans = SetToken( IF, tempans ) ;
    } // else if()

    else if ( tempans.size() == 4 && tempans == "else" ) {
      ans = SetToken( ELSE, tempans ) ;
    } // else if()

    else if ( tempans.size() == 5 && tempans == "while" ) {
      ans = SetToken( WHILE, tempans ) ;
    } // else if()

    else if ( tempans.size() == 2 && tempans == "do" ) {
      ans = SetToken( DO, tempans ) ;
    } // else if()

    else if ( tempans.size() == 6 && tempans == "return" ) {
      ans = SetToken( RETURN, tempans ) ;
    } // else if()

    else if ( tempans.size() == 4 && tempans == "true" ) {
      ans = SetToken( CONSTANT, tempans ) ;
    } // else if()

    else if ( tempans.size() == 5 && tempans == "false" ) {
      ans = SetToken( CONSTANT, tempans ) ;
    } // else if()

    else if ( tempans.size() == 3 && tempans == "cin" ) {
      ans = SetToken( ID, tempans ) ;
    } // else if()

    else if ( tempans.size() == 4 && tempans == "cout" ) {
      ans = SetToken( ID, tempans ) ;
    } // else if()

    else {
      ans = SetToken( ID, tempans ) ;
    } // else()

  } // if()

  else if ( ( target >= 48 && target <= 57 ) || target == '.' ) { // NUM�����p ( �b 0 - 9 ���� �� '.' )

    if ( target == '.' ) {

      char temp = cin.peek() ;

      if ( ! ( temp >= 48 && temp <= 57 ) ) { // �᭱���O 0 - 9  ��W�@�� �I( '.' ) �����p
        string ansss = "" ;
        ansss = ansss + target ;
        tempans = "Unrecognized token with first char : '" + ansss + "'" ;
        // ReadtoEnd() ;
        throw tempans ; // ///////////
      } // if()

      else {
        tempans = F5( target ) ;
        ans = SetToken( CONSTANT, tempans ) ;

      } // else()
    } // if()

    else {
      tempans = F5( target ) ;
      ans = SetToken( CONSTANT, tempans ) ;
    } // else()

  } // else if()

  else if ( target == '(' ) {  // �O���I�Ÿ������p
    tempans = tempans + target ;
    ans = SetToken( LEFTPAR, tempans ) ;
  } // else if()

  else if ( target == ')' ) {  // �O���I�Ÿ������p
    tempans = tempans + target ;
    ans = SetToken( RIGHTPAR, tempans ) ;
  } // else if()

  else if ( target == '[' ) {  // �O���I�Ÿ������p
    tempans = tempans + target ;
    ans = SetToken( LEFTMID, tempans ) ;
  } // else if()

  else if ( target == ']' ) {  // �O���I�Ÿ������p
    tempans = tempans + target ;
    ans = SetToken( RIGHTMID, tempans ) ;
  } // else if()

  else if ( target == '{' ) {  // �O���I�Ÿ������p
    tempans = tempans + target ;
    ans = SetToken( LEFTBIG, tempans ) ;
  } // else if()

  else if ( target == '}' ) {  // �O���I�Ÿ������p
    tempans = tempans + target ;
    ans = SetToken( RIGHTBIG, tempans ) ;
  } // else if()

  else if ( target == '+' ) {  // �[�� ���T�ر��p + ++ +=
    tempans = tempans + target ;
    char temp = cin.peek() ;
    if ( temp == '+' ) {
      temp = F1() ; // �T�w�h��
      tempans = tempans + temp ;
      ans = SetToken( PP, tempans ) ;
    } // if()

    else if ( temp == '=' ) {
      temp = F1() ; // �T�w�h��
      tempans = tempans + temp ;
      ans = SetToken( PE, tempans ) ;
    } // else if()

    else {
      ans = SetToken( PLUS, tempans ) ;
    } // else()
  } // else if()

  else if ( target == '-' ) {  // � ���T�ر��p - -- -=
    tempans = tempans + target ;
    char temp = cin.peek() ;
    if ( temp == '-' ) {
      temp = F1() ; // �T�w�h��
      tempans = tempans + temp ;
      ans = SetToken( MM, tempans ) ;
    } // if()

    else if ( temp == '=' ) {
      temp = F1() ; // �T�w�h��
      tempans = tempans + temp ;
      ans = SetToken( ME, tempans ) ;
    } // else if()

    else {
      ans = SetToken( MINUS, tempans ) ;
    } // else()
  } // else if()

  else if ( target == '*' ) {  // ���� ���G�ر��p * *=
    tempans = tempans + target ;
    char temp = cin.peek() ;
    if ( temp == '=' ) {
      temp = F1() ; // �T�w�h��
      tempans = tempans + temp ;
      ans = SetToken( TE, tempans ) ;
    } // if()

    else {
      ans = SetToken( MUL, tempans ) ;
    } // else()
  } // else if()

  else if ( target == '/' ) {  // ���� ���G�ر��p / /=
    tempans = tempans + target ;
    char temp = cin.peek() ;
    if ( temp == '=' ) {
      temp = F1() ; // �T�w�h��
      tempans = tempans + temp ;
      ans = SetToken( DE, tempans ) ;
    } // if()

    else {
      ans = SetToken( DIV, tempans ) ;
    } // else()
  } // else if()

  else if ( target == '%' ) {  // ���� ���G�ر��p % %=
    tempans = tempans + target ;
    char temp = cin.peek() ;
    if ( temp == '=' ) {
      temp = F1() ; // �T�w�h��
      tempans = tempans + temp ;
      ans = SetToken( RE, tempans ) ;
    } // if()

    else {
      ans = SetToken( PERCENTAGE, tempans ) ;
    } // else()
  } // else if()

  else if ( target == '!' ) {  // ���� ���G�ر��p ! !=
    tempans = tempans + target ;
    char temp = cin.peek() ;
    if ( temp == '=' ) {
      temp = F1() ; // �T�w�h��
      tempans = tempans + temp ;
      ans = SetToken( NEQ, tempans ) ;
    } // if()

    else {
      ans = SetToken( EXCLAMATION, tempans ) ;
    } // else()
  } // else if()

  else if ( target == '=' ) {  // ���� ���G�ر��p = ==
    tempans = tempans + target ;
    char temp = cin.peek() ;
    if ( temp == '=' ) {
      temp = F1() ; // �T�w�h��
      tempans = tempans + temp ;
      ans = SetToken( EQ, tempans ) ;
    } // if()

    else {
      ans = SetToken( SEQUAL, tempans ) ;
    } // else()
  } // else if()

  else if ( target == '&' ) {  // ���� ���G�ر��p & &&
    tempans = tempans + target ;
    char temp = cin.peek() ;
    if ( temp == '&' ) {
      temp = F1() ; // �T�w�h��
      tempans = tempans + temp ;
      ans = SetToken( AND, tempans ) ;
    } // if()

    else {
      ans = SetToken( SAND, tempans ) ;
    } // else()
  } // else if()

  else if ( target == '|' ) {  // ���� ���G�ر��p | ||
    tempans = tempans + target ;
    char temp = cin.peek() ;
    if ( temp == '|' ) {
      temp = F1() ; // �T�w�h��
      tempans = tempans + temp ;
      ans = SetToken( OR, tempans ) ;
    } // if()

    else {
      ans = SetToken( SOR, tempans ) ;
    } // else()
  } // else if()

  else if ( target == '>' ) {  // ���T�ر��p > >> >=
    tempans = tempans + target ;
    char temp = cin.peek() ;
    if ( temp == '>' ) {
      temp = F1() ; // �T�w�h��
      tempans = tempans + temp ;
      ans = SetToken( RS, tempans ) ;
    } // if()

    else if ( temp == '=' ) {
      temp = F1() ; // �T�w�h��
      tempans = tempans + temp ;
      ans = SetToken( GE, tempans ) ;
    } // else if()

    else {
      ans = SetToken( GREATER, tempans ) ;
    } // else()
  } // else if()

  else if ( target == '<' ) {  // ���T�ر��p < << <=
    tempans = tempans + target ;
    char temp = cin.peek() ;
    if ( temp == '<' ) {
      temp = F1() ; // �T�w�h��
      tempans = tempans + temp ;
      ans = SetToken( LS, tempans ) ;
    } // if()

    else if ( temp == '=' ) {
      temp = F1() ; // �T�w�h��
      tempans = tempans + temp ;
      ans = SetToken( LE, tempans ) ;
    } // else if()

    else {
      ans = SetToken( LESSER, tempans ) ;
    } // else()
  } // else if()

  else if ( target == '^' ) {  // �O���I�Ÿ������p
    tempans = tempans + target ;
    ans = SetToken( CARET, tempans ) ;
  } // else if()

  else if ( target == ';' ) {  // �O���I�Ÿ������p
    tempans = tempans + target ;
    ans = SetToken( SEMICOLON, tempans ) ;
  } // else if()

  else if ( target == ',' ) {  // �O���I�Ÿ������p
    tempans = tempans + target ;
    ans = SetToken( COMMA, tempans ) ;
  } // else if()

  else if ( target == '?' ) {  // �O���I�Ÿ������p
    tempans = tempans + target ;
    ans = SetToken( QUES, tempans ) ;
  } // else if()

  else if ( target == ':' ) {  // �O���I�Ÿ������p
    tempans = tempans + target ;
    ans = SetToken( COLON, tempans ) ;
  } // else if()

  else if ( target == '\"' ) {  // �O���I�Ÿ������p
    tempans = tempans + target ;
    target = cin.peek() ;
    while ( target != '\"' ) {
      target = F1() ;
      tempans = tempans + target ;
      target = cin.peek() ;
    } // while()

    target = F1() ;
    tempans = tempans + target ;

    ans = SetToken( CONSTANT, tempans ) ;
  } // else if()

  else if ( target == '\'' ) {  // �O���I�Ÿ������p
    tempans = tempans + target ;
    target = cin.peek() ;
    while ( target != '\'' ) {
      target = F1() ;
      tempans = tempans + target ;
      target = cin.peek() ;
    } // while()

    target = F1() ;
    tempans = tempans + target ;

    ans = SetToken( CONSTANT, tempans ) ;
  } // else if()

  else if ( target == EOF ) {
    tempans = target ;
    gEnd = 1 ; // ����
    string q = "Is EOF" ;
    throw q ;
    return ans ;
  } // else if()

  else {
    string anss = "" ;
    anss = anss + target ;
    tempans = "Unrecognized token with first char : '" + anss + "'" ;
    // ReadtoEnd() ;
    throw tempans ;
  } // else()

  return ans ;
} // GetToken()

// ////////////////  ��k���ΰ�   ////////////////////

Token PeekToken() ;
Token RealGetToken() ;
void Error1( Token a ) ;
void Error2( Token a ) ;
void Error3( Token a ) ;
void Error3steps() ;
bool Is_statement( Token a ) ;
bool Is_basic_expression( Token a ) ;
bool Is_sign( Token a ) ;
bool Is_unary_exp( Token a ) ;
void Is_Done() ;
bool CheckDefinition( Token a ) ;
bool CheckFunctionDefinition( Token a ) ;

// ////////////////  ��k��   ////////////////////

bool Type_specifier( Token a ) ;
bool Assignment_operator( Token a ) ;
void User_input() ;
void Definition() ;
void Function_definition_or_declarators() ;
void Function_definition_without_ID() ;
void Rest_of_declarators() ;
void Formal_parameter_list() ;
void Compound_statement() ;
void Declaration() ;
void Statement() ;
void Expression() ;
void Basic_expression() ;
void Unsigned_unary_exp() ;
void Actual_parameter_list() ;
void Signed_unary_exp() ;
void Unary_exp() ;
void Rest_of_maybe_mult_exp() ;
void Maybe_mult_exp() ;
void Rest_of_maybe_additive_exp() ;
void Maybe_additive_exp() ;
void Rest_of_maybe_shift_exp() ;
void Maybe_shift_exp() ;
void Rest_of_maybe_relational_exp() ;
void Maybe_relational_exp() ;
void Maybe_equality_exp() ;
void Rest_of_maybe_bit_AND_exp() ;
void Maybe_bit_AND_exp() ;
void Rest_of_maybe_bit_ex_OR_exp() ;
void Maybe_bit_ex_OR_exp() ;
void Rest_of_maybe_bit_OR_exp() ;
void Maybe_bit_OR_exp() ;
void Rest_of_maybe_logical_AND_exp() ;
void Maybe_logical_AND_exp() ;
void Rest_of_maybe_logical_OR_exp() ;
void Romce_and_romloe() ;
void Rest_of_PPMM_Identifier_started_basic_exp() ;
void Rest_of_Identifier_started_basic_exp() ;
void Rest_of_maybe_equality_exp() ;
void Is_SystemFunction() ;
void PrintFunction( vector <Token> a ) ;
void PrintWhiteSpace( int a ) ;
bool CheckFunctionName( string a ) ;


// //////////////// ��k�}�l  ////////////////////

Token PeekToken() {

  Token temp ;
  temp = GetToken() ;
  gPeekBuffer.push_back( temp ) ;
  return temp ;

} // PeekToken()

Token RealGetToken() {

  if ( gPeekBuffer.size() != 0 ) {
    return gPeekBuffer[0] ;
  } // if()

  else {
    Token temp ;
    temp = PeekToken() ;
    return gPeekBuffer[0] ;
  } // else()
} // RealGetToken()

bool Is_statement( Token a ) {

  if ( a.type == SEMICOLON || a.type == RETURN || a.type == IF
       || a.type == WHILE || a.type == DO || a.type == LEFTBIG ) {
    return true ;
  } // if()

  else {
    if ( Is_basic_expression( a ) ) {
      return true ;
    } // if()

    else return false ;

  } // else()

} // Is_statement()

bool Is_basic_expression( Token a ) {

  if ( a.type == ID || a.type == PP || a.type == MM || Is_sign( a ) || a.type == CONSTANT
       || a.type == LEFTPAR ) {
    return true ;
  } // if()

  else return false ; // else()

} // Is_basic_expression()

bool Is_sign( Token a ) {

  if ( a.type == PLUS || a.type == MINUS || a.type == EXCLAMATION ) {
    return true ;
  } // if()

  else return false ; // else()

} // Is_sign()

bool Is_unary_exp( Token a ) {

  if ( a.type == ID || a.type == CONSTANT || a.type == LEFTPAR ) {
    return true ;
  } // if()

  else return false ;

} // Is_unary_exp()

void Is_Done() {
  if ( gCorrectGrammer.size() == 4 ) {
    string temp ;
    int i ;
    temp = gCorrectGrammer[0].content + gCorrectGrammer[1].content ;
    temp = temp + gCorrectGrammer[2].content + gCorrectGrammer[3].content ;
    if ( temp == "Done();" ) {
      temp = "Our-C exited ..." ;
      gIsEnd = true ;
      gthrowspecial = 1 ;
      throw temp ;
    } // if()
  } // if()
} // Is_Done()

void Error1( Token a ) { // unrecognized token with first char
  string str = "unrecognized token with first char : '" ;
  str = str + a.content + "'" ;
  throw str ;
} // Error1()

void Error2( Token a ) { // unexpected token
  string str = "unexpected token : '" ;
  str = str + a.content + "'" ;
  throw str ;
} // Error2()

void Error3( Token a ) { // undefined identifier
  string str = "undefined identifier : '" ;
  str = str + a.content + "'" ;
  throw str ;
} // Error3()

void Error3steps() {
  gPeekBuffer.clear() ;
  gCorrectGrammer.clear() ;
} // Error3steps()

bool CheckDefinition( Token a ) { //  need to check local and global place

  if ( a.content == "cin" || a.content == "cout" || a.content == "Done"
       || a.content == "ListAllVariables" || a.content == "ListVariable"
       || a.content == "ListAllFunctions" || a.content == "ListFunction" ) {
    return true ;
  } // if()

  if ( gDefinitionglobal.size() != 0 ) {
    for ( int i = 0 ; i < gDefinitionglobal.size() ; i++ ) {
      if ( a.content == gDefinitionglobal[i].content ) {
        return true ;
      } // if()
    } // for()
  } // if()

  if ( gTempDlocal.size() != 0 ) {
    for ( int i = 0 ; i < gTempDlocal.size() ; i++ ) {
      if ( a.content == gTempDlocal[i].content ) {
        return true ;
      } // if()
    } // for()
  } // if()

  for ( int i = 0 ; i < gFunctionName.size() ; i++ ) {
    if ( a.content == gFunctionName[i] ) {
      return true ;
    } // if()
  } // for()

  return false ;
} // CheckDefinition()

bool CheckFunctionDefinition( Token a ) {

  for ( int i = 0 ; i < gFunctionName.size() ; i ++ ) {
    if ( a.content == gFunctionName[i] ) return true ; // if()
  } // for()

  for ( int i = 0 ; i < gSystemFunction.size() ; i ++ ) {
    if ( a.content == gSystemFunction[i] ) return true ; // if()
  } // for()

  return false ;
} // CheckFunctionDefinition()

bool CheckFunctionName( string a ) {
  for ( int i = 0 ; i < gFunctionName.size() ; i ++ ) {
    if ( a == gFunctionName[i] ) return true ; // if()
  } // for()

  return false ;
} // CheckFunctionName()

void Is_SystemFunction() {

  if ( gCorrectGrammer[0].content == "ListAllVariables"
       && gCorrectGrammer[1].content == "("
       && gCorrectGrammer[2].content == ")" ) {

    vector <string> temp ;
    for ( int i = 0 ; i < gDefinitionglobal.size() ; i++ ) {
      temp.push_back( gDefinitionglobal[i].content ) ;
    } // for()

    for ( int i = 0 ; i < temp.size() ; i++ ) {
      for ( int l = i + 1 ; l < temp.size() ; l++ ) {
        if ( temp[i] > temp[l] ) {
          string ttemp ;
          ttemp = temp[l] ;
          temp[l] = temp[i] ;
          temp[i] = ttemp ;
        } // if()
      } // for()
    } // for()

    cout << "> " ;
    for ( int i = 0 ; i < temp.size() ; i++ ) {
      cout << temp[i] << endl ;
    } // for()

    throw guseless ;
  } // if()

  else if ( gCorrectGrammer[0].content == "ListVariable"
            && gCorrectGrammer[1].content == "("
            && gCorrectGrammer[2].constantDetailType == TWOPOINTSTRING ) {
    string target ;
    target = gCorrectGrammer[2].content ;
    target.erase( 0, 1 ) ;
    target.erase( target.size() - 1, 1 ) ;

    for ( int i = 0 ; i < gDefinitionglobal.size() ; i++ ) {
      if ( target == gDefinitionglobal[i].content ) {
        cout << "> " ;
        cout << gDefinitionglobal[i].idtype << " " ;
        cout << gDefinitionglobal[i].content << gDefinitionglobal[i].ifisarray << " ;" << endl ;
      } // if()
    } // for()

    throw guseless ;
  } // else if()

  else if ( gCorrectGrammer[0].content == "ListAllFunctions"
            && gCorrectGrammer[1].content == "("
            && gCorrectGrammer[2].content == ")" ) {

    vector <string> temp ;
    for ( int i = 0 ; i < gFunctionName.size() ; i++ ) {
      temp.push_back( gFunctionName[i] ) ;
    } // for()

    for ( int i = 0 ; i < temp.size() ; i++ ) {
      for ( int l = i + 1 ; l < temp.size() ; l++ ) {
        if ( temp[i] > temp[l] ) {
          string ttemp ;
          ttemp = temp[l] ;
          temp[l] = temp[i] ;
          temp[i] = ttemp ;
        } // if()
      } // for()
    } // for()

    cout << "> " ;
    for ( int i = 0 ; i < temp.size() ; i++ ) {
      cout << temp[i] << "()" << endl ;
    } // for()

    throw guseless ;
  } // else if()

  else if ( gCorrectGrammer[0].content == "ListFunction"
            && gCorrectGrammer[1].content == "("
            && gCorrectGrammer[2].constantDetailType == TWOPOINTSTRING ) {
    string target ;
    target = gCorrectGrammer[2].content ;
    target.erase( 0, 1 ) ;
    target.erase( target.size() - 1, 1 ) ;

    vector <Token> temp ;

    for ( int i = 0 ; i < gWholeFunction.size() ; i++ ) {
      if ( target == gWholeFunction[i][1].content ) {
        temp = gWholeFunction[i] ;
      } // if()
    } // for()

    cout << "> " ;
    PrintFunction( temp ) ;
    throw guseless ;
  } // else if()

} // Is_SystemFunction()

void PrintFunction( vector <Token> a ) {

  int nowspace = 0 ; // �@�}�l�ݭn���ťռƬ� 0
  int funcsize = a.size() ;
  int nowcount = 0 ; // location

  while ( nowcount < funcsize ) {

    if ( a[nowcount].content == "{" ) {
      // cout << "test 0 " <<endl ;
      nowspace = nowspace + 2 ;
      cout << a[nowcount].content << endl ;
      nowcount = nowcount + 1 ;
      if ( a[nowcount].content != "}" ) {
        PrintWhiteSpace( nowspace ) ;
      } // if()
    } // if()

    else if ( a[nowcount].content == "}" ) {
      // cout << "test 1 " <<endl ;
      nowspace = nowspace - 2 ;
      PrintWhiteSpace( nowspace ) ;
      cout << a[nowcount].content << endl ;
      nowcount = nowcount + 1 ;

      if ( nowcount == funcsize ) return ; // if()

      if ( a[nowcount].content != "}" ) {
        PrintWhiteSpace( nowspace ) ;
      } // if()

    } // else if()

    else if ( CheckFunctionName( a[nowcount].content ) ) { // ex : a = a( int a )
      // cout << "test 2" <<endl ;
      cout << a[nowcount].content ;
      nowcount = nowcount + 1 ;
      cout << a[nowcount].content ;
      nowcount = nowcount + 1 ;

      if ( a[nowcount].content == ")" ) {
        cout << a[nowcount].content << " " ;
        nowcount = nowcount + 1 ;
      } // if()

      else {
        cout << " " ;
      } // else()

      /*
      else {
        while ( a[nowcount].content != ")" ) {

          if ( a[nowcount + 1].content == "[" ) {


          } // if()
          cout << a[nowcount].content << " " ;
          nowcount = nowcount + 1 ;
        } // while()

        cout << a[nowcount].content << " " ;
        nowcount = nowcount + 1 ;
      } // else()
      */
    } // else if()

    else if ( a[nowcount + 1].content == "[" ) { // ex : a = a[ 30 ]
      // cout << "test 3 " <<endl ;
      cout << a[nowcount].content ;
      nowcount = nowcount + 1 ;
      cout << a[nowcount].content << " " ;
      nowcount = nowcount + 1 ;
      while ( a[nowcount].content != "]" ) {
        cout << a[nowcount].content << " " ;
        nowcount = nowcount + 1 ;
      } // while()

      if ( a[nowcount + 1].content == "," ) {
        cout << a[nowcount].content << a[nowcount + 1].content << " " ;
        nowcount = nowcount + 2 ;
      } // if()

      else {
        cout << a[nowcount].content << " " ;
        nowcount = nowcount + 1 ;
      } // else()

    } // else if()

    else if ( a[nowcount].type == SEMICOLON ) {
      // cout << "test 4 " <<endl ;
      cout << a[nowcount].content << endl ;
      nowcount = nowcount + 1 ;
      if ( a[nowcount].content != "}" ) {
        PrintWhiteSpace( nowspace ) ;
      } // if()
    } // else if()

    else if (  a[nowcount + 1].type == PP  ) {
      cout << a[nowcount].content << a[nowcount + 1].content << " " ;
      nowcount = nowcount + 2 ;
    } // else if()

    else if (  a[nowcount + 1].type == MM ) {
      cout << a[nowcount].content << a[nowcount + 1].content << " " ;
      nowcount = nowcount + 2 ;
    } // else if()

    else if ( a[nowcount + 1].content == "," ) {
      cout << a[nowcount].content << a[nowcount + 1].content << " " ;
      nowcount = nowcount + 2 ;
    } // else if()

    else {
      // cout << "test 5 " <<endl ;
      cout << a[nowcount].content << " " ;
      nowcount = nowcount + 1 ;
    } // else()
  } // while()

} // PrintFunction()

void PrintWhiteSpace( int a ) {
  if ( a <= 0 ) return ; // if()

  for ( int i = 0 ; i < a ; i++ ) {
    cout << " " ;
  } // for()
} // PrintWhiteSpace()


// /////////////////////////////

void User_input() {  // ( Definition | Statement ) { Definition | Statement }
  // cout << "Path 1" << endl ;
  Token temp ;
  temp = RealGetToken() ; // �q gpeekbuffer�ݭ� �������X

  if ( Type_specifier( temp ) ||  temp.type == VOID ) {
    // cout << "check 1 out" << endl ;
    Definition() ;
    Is_Done() ;
    /*
    for ( int i = 0 ; i < gCorrectGrammer.size() ; i ++ ) {
      cout << gCorrectGrammer[i].content ;
    } // for()
    cout <<endl ;
    */

    if ( gIs_function ) { // �]��o��N�� �@��� function �]���S��
      string function_name ;
      function_name = gCorrectGrammer[1].content ;
      // if ( gCorrectGrammer[1].isfunction ) cout << "543546" <<endl ;

      if ( gWholeFunction.size() != 0 ) {
        int repeat = 0 ;
        for ( int i = 0 ; i < gWholeFunction.size() ; i++ ) {
          if ( function_name == gWholeFunction[i][1].content ) {
            repeat = 1 ;
            gWholeFunction[i] = gCorrectGrammer ;
          } // if()
        } // for()

        if ( repeat == 1 ) {
          cout << "> New definition of " << function_name << "() entered ..." << endl ;
        } // if()

        else {
          gFunctionName.push_back( function_name ) ;
          gWholeFunction.push_back( gCorrectGrammer ) ;
          cout << "> Definition of " << function_name << "() entered ..." << endl ;
        } // else()
      } // if()

      else { // �N���٨S����L function
        gFunctionName.push_back( function_name ) ;
        gWholeFunction.push_back( gCorrectGrammer ) ;
        cout << "> Definition of " << function_name << "() entered ..." << endl ;
      } // else()
    } // if()

    gCorrectGrammer.clear() ;
    gTempDglobal.clear() ;
    gTempDlocal.clear() ;
    gIs_Definition = false ;
    gGlobal_mode = true ;
    gIs_function = false ;
    gIs_If_Else_ = false ;
    gPass_compound = false ;
    g_if_function_wrong = "121321321" ;
  } // if()

  else if ( Is_statement( temp ) ) {
    Statement() ;
    Is_Done() ;

    if ( gIs_If_Else_ ) {
      // if ( gPass_compound ) cout << "> Statement executed ..." << endl ; // if()
      cout << "> Statement executed ..." << endl ;
    } // if()

    else if ( gPass_compound ) cout << "> Statement executed ..." << endl ; // else if()

    gCorrectGrammer.clear() ;
    gTempDglobal.clear() ;
    gTempDlocal.clear() ;
    gIs_Definition = false ;
    gGlobal_mode = true ;
    gIs_function = false ;
    gIs_If_Else_ = false ;
    gPass_compound = false ;
    g_if_function_wrong = "121321321" ;
  } // else if()

  else {
    gPeekBuffer.clear() ;
    Error2( temp ) ;
  } // else()

  gline_num = 0 ;

  temp = RealGetToken() ;

  while ( Type_specifier( temp ) ||  temp.type == VOID || Is_statement( temp ) ) {
    if ( Type_specifier( temp ) ||  temp.type == VOID ) {
      // cout << "check 1" <<endl ;
      Definition() ;
      /*
      for ( int i = 0 ; i < gCorrectGrammer.size() ; i ++ ) {
        cout << gCorrectGrammer[i].content ;
      } // for()
      cout <<endl ;
      */
      Is_Done() ;

      if ( gIs_function ) { // �]��o��N�� �@��� function �]���S��
        string function_name ;
        function_name = gCorrectGrammer[1].content ;

        if ( gWholeFunction.size() != 0 ) {
          int repeat = 0 ;
          for ( int i = 0 ; i < gWholeFunction.size() ; i++ ) {
            if ( function_name == gWholeFunction[i][1].content ) {
              repeat = 1 ;
              gWholeFunction[i] = gCorrectGrammer ;
            } // if()
          } // for()

          if ( repeat == 1 ) {
            cout << "> New definition of " << function_name << "() entered ..." << endl ;
          } // if()

          else {
            gFunctionName.push_back( function_name ) ;
            gWholeFunction.push_back( gCorrectGrammer ) ;
            cout << "> Definition of " << function_name << "() entered ..." << endl ;
          } // else()
        } // if()

        else { // �N���٨S����L function
          gFunctionName.push_back( function_name ) ;
          gWholeFunction.push_back( gCorrectGrammer ) ;
          cout << "> Definition of " << function_name << "() entered ..." << endl ;
        } // else()
      } // if()

      gCorrectGrammer.clear() ;
      gTempDglobal.clear() ;
      gTempDlocal.clear() ;
      gIs_Definition = false ;
      gGlobal_mode = true ;
      gIs_function = false ;
      gIs_If_Else_ = false ;
      gPass_compound = false ;
      g_if_function_wrong = "121321321" ;
    } // if()

    else if ( Is_statement( temp ) ) {
      // cout << "check 2" <<endl ;
      Statement() ;
      /*
      for ( int i = 0 ; i < gCorrectGrammer.size() ; i ++ ) {
        cout << gCorrectGrammer[i].content ;
      } // for()
      cout <<endl ;
      */
      Is_Done() ;
      if ( gIs_If_Else_ ) {
        // if ( gPass_compound ) cout << "> Statement executed ..." << endl ; // if()
        cout << "> Statement executed ..." << endl ;
      } // if()

      else if ( gPass_compound ) cout << "> Statement executed ..." << endl ; // else if()

      gCorrectGrammer.clear() ;
      gTempDlocal.clear() ;
      gTempDglobal.clear() ;
      gIs_Definition = false ;
      gGlobal_mode = true ;
      gIs_function = false ;
      gIs_If_Else_ = false ;
      gPass_compound = false ;
      g_if_function_wrong = "121321321" ;
    } // else if()

    gline_num = 0 ;
    temp = RealGetToken() ;

  } // while()

} // User_input()

bool Type_specifier( Token a ) { // : INT | CHAR | FLOAT | STRING | BOOL

  if ( a.type == INT || a.type == CHAR || a.type == FLOAT || a.type == STRING || a.type == BOOL ) {
    return true ;
  } // if()

  else return false ; // else()

} // Type_specifier()

void Definition() { // gIs_Definition  gGlobal_mode

// VOID Identifier function_definition_without_ID
// | type_specifier Identifier Function_definition_or_declarators

  Token temp ;
  temp = RealGetToken() ;

  if ( temp.type == VOID ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ; // �T�w�� token �ιL�F
    temp = RealGetToken() ;
    if ( temp.type != ID ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    temp.isfunction = true ; // �o�O GLOBAL �� �O FUNCTION
    gIs_function = true ;
    g_if_function_wrong = temp.content ;
    gCorrectGrammer.push_back( temp ) ;
    gTempDglobal.push_back( temp ) ; // ��� temp global ��
    gPeekBuffer.clear() ; // �T�w�� token �ιL�F
    Function_definition_without_ID() ;
  } // if()

  else if ( Type_specifier( temp ) ) {
    gnowtype = temp.content ;
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ;
    if ( temp.type == ID ) {
      Token checkfunction ;
      checkfunction = temp ; // ���� id �Ȧs�_�� ���T�w�O���O function
      gPeekBuffer.clear() ;
      temp = RealGetToken() ; // �o�ɭ� temp �� id �U�@�� token

      if ( temp.type == LEFTPAR ) {
        checkfunction.isfunction = true ;
        checkfunction.idtype = gnowtype ;
        g_if_function_wrong = checkfunction.content ;
        gIs_function = true ;
        gCorrectGrammer.push_back( checkfunction ) ;
        gTempDglobal.push_back( checkfunction ) ;
      } // if()

      else {
        if ( gGlobal_mode ) {
          checkfunction.idtype = gnowtype ;
          gCorrectGrammer.push_back( checkfunction ) ;
          gTempDglobal.push_back( checkfunction ) ;
        } // if()

        else {
          checkfunction.idtype = gnowtype ;
          gCorrectGrammer.push_back( checkfunction ) ;
          gTempDlocal.push_back( checkfunction ) ;
        } // else()
      } // else()

      Function_definition_or_declarators() ;
    } // if()

    else {
      gPeekBuffer.clear() ;
      gCorrectGrammer.clear() ;
      Error2( temp ) ; // unexpected token
    } // else()

  } // else if()

  else {
    gPeekBuffer.clear() ;
    gCorrectGrammer.clear() ;
    Error2( temp ) ; // unexpected token
  } // else()


} // Definition()

void Function_definition_or_declarators() {
// Function_definition_without_ID | Rest_of_declarators
  Token temp ;
  temp = RealGetToken() ;

  if ( temp.type == LEFTPAR ) { // �O���A�� �N��O  function  definition is local
    Function_definition_without_ID() ;
  } // if()

  else if ( temp.type == LEFTMID || temp.type == COMMA || temp.type == SEMICOLON ) {
    gIs_Definition = true ; // ���p�Serror���� �n��� definition
    Rest_of_declarators() ;
  } // else if()

  else {
    gPeekBuffer.clear() ;
    gCorrectGrammer.clear() ;
    Error2( temp ) ; // unexpected token
  } // else()
} // Function_definition_or_declarators()

void Function_definition_without_ID() { // gGlobal_mode

  Token temp ;
  temp = RealGetToken() ;
  gGlobal_mode = false ; //  ��o�@�B definition ���� local mode �ҥH���� false

  if ( temp.type == LEFTPAR ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ; // �A���U�@��
    if ( temp.type == VOID ) {
      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
      temp = RealGetToken() ; // �A���U�@��
    } // if()

    else if ( Type_specifier( temp ) ) {
      Formal_parameter_list() ;
      temp = RealGetToken() ; // �A���U�@��
    } // else if()

  } // if()

  else {
    gPeekBuffer.clear() ;
    gCorrectGrammer.clear() ;
    // ReadtoEnd() ;
    Error2( temp ) ; // unexpected token
  } // else()

  temp = RealGetToken() ;
  if ( temp.type == RIGHTPAR ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Compound_statement() ;
  } // if()

  else {
    gPeekBuffer.clear() ;
    gCorrectGrammer.clear() ;
    // ReadtoEnd() ;
    Error2( temp ) ; // unexpected token
  } // else()

} // Function_definition_without_ID()

void Rest_of_declarators() { // gGlobal_mode *****************************
// [ '[' Constant ']' ] { ',' Identifier [ '[' Constant ']' ] } ';'
  Token temp ;
  temp = RealGetToken() ;
  int size_of_vector ; // ���� id �b correctvector ������m
  string array_info ;

  if ( temp.type == LEFTMID ) { // �O [ ] �N���}�C�n�s
    size_of_vector = gCorrectGrammer.size() - 1 ; // �٨S�񤤬A�����j�p �̫�@�Ӭ��һݭn�� id
    array_info = array_info + temp.content ; // array_info = " [ "
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ; // �A���U�@��
    if ( temp.type == CONSTANT ) {
      array_info = array_info + " " + temp.content ; // array_info = " [ 30 "
      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
      temp = RealGetToken() ; // �A���U�@��
      if ( temp.type == RIGHTMID ) {
        array_info = array_info + " " + temp.content ; // array_info = " [ 30 ] "
        gCorrectGrammer.push_back( temp ) ;
        gPeekBuffer.clear() ;

        gCorrectGrammer[size_of_vector].ifisarray = array_info ; //  ��}�C�j�p�s�i�h
        if ( gGlobal_mode ) {
          int tt ;
          tt = gTempDglobal.size() - 1 ;
          gTempDglobal[tt].ifisarray = array_info ;
        } // if()

        temp = RealGetToken() ; // �A���U�@��
      } // if()

      else {
        Error3steps() ;
        Error2( temp ) ; // unexpected token
      } // else()
    } // if()

    else {
      Error3steps() ;
      Error2( temp ) ; // unexpected token
    } // else()
  } // if()

  temp = RealGetToken() ;

  while ( temp.type == COMMA ) {
    int size_of_vector ; // ���� id �b correctvector ������m
    string array_inf ;

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ; // �A���U�@��
    if ( temp.type == ID ) {
      temp.idtype = gnowtype ;
      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
      size_of_vector = gCorrectGrammer.size() - 1 ; // id ���i�h correct grammmer �H�� �̫�@�Ӭ� id token
      if ( gGlobal_mode ) {
        Token check ;
        check = RealGetToken() ;
        if ( check.content != "[" ) {
          gTempDglobal.push_back( temp ) ;
        } // if()
      } // if()

      else {
        Token check ;
        check = RealGetToken() ;
        if ( check.content != "[" ) {
          gTempDlocal.push_back( temp ) ;
        } // if()
      } // else()

      // gPeekBuffer.clear() ;
      temp = RealGetToken() ; // �A���U�@��
      if ( temp.type == LEFTMID ) {
        array_inf = array_inf + temp.content ;
        gCorrectGrammer.push_back( temp ) ;
        gPeekBuffer.clear() ;
        temp = RealGetToken() ; // �A���U�@��
        if ( temp.type == CONSTANT ) {
          array_inf = array_inf + " " + temp.content ;
          gCorrectGrammer.push_back( temp ) ;
          gPeekBuffer.clear() ;
          temp = RealGetToken() ; // �A���U�@��
          if ( temp.type == RIGHTMID ) {
            array_inf = array_inf + " " + temp.content ;
            gCorrectGrammer.push_back( temp ) ;
            gPeekBuffer.clear() ;
            gCorrectGrammer[size_of_vector].ifisarray = array_inf ;

            if ( gGlobal_mode ) {
              gTempDglobal.push_back( gCorrectGrammer[size_of_vector] ) ;
            } // if()

            else {
              gTempDlocal.push_back( gCorrectGrammer[size_of_vector] ) ;
            } // else()

            temp = RealGetToken() ; // �A���U�@��
          } // if()

          else {
            Error3steps() ;
            Error2( temp ) ;
          } // else()
        } // if()

        else {
          Error3steps() ;
          Error2( temp ) ;
        } // else()
      } // if()
    } // if()

    else {
      Error3steps() ;
      Error2( temp ) ;
    } // else()

    temp = RealGetToken() ;
  } // while()

  // Testall() ;

  if ( temp.type == SEMICOLON ) { // ���_�I
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    if ( gIs_function ) { // ���� function ���I�s���� ���ΰ���
      int guseless = 1 ;
    } // if()

    else if ( gIs_If_Else_ ) {
      int guseless = 2 ;
    } // else if()

    else if ( gPass_compound ) guseless = 3 ; // else if()

    else if ( gIs_Definition ) {

      string idname ;
      int check = 0 ;

      cout << "> " ;

      for ( int i = 0 ; i < gTempDglobal.size() ; i ++ ) {
        idname = gTempDglobal[i].content ;
        for ( int l = 0 ; l < gDefinitionglobal.size() ; l ++ ) {
          if ( idname == gDefinitionglobal[l].content ) {
            cout << "New definition of " << idname <<  " entered ..." << endl ;
            gDefinitionglobal[l] = gTempDglobal[i] ; // �n��Ӵ�������Ƥ~���|��
            check = 1 ;
          } // if()
        } // for()

        if ( check == 0 ) { // �N��S������
          gDefinitionglobal.push_back( gTempDglobal[i] ) ;
          cout << "Definition of " << idname <<  " entered ..." << endl ;
        } // if()
      } // for()


    } // else if()

    return ;
  } // if()

  else {
    Error3steps() ;
    Error2( temp ) ;
  } // else()

} // Rest_of_declarators()

void Formal_parameter_list() {
// type_specifier [ '&' ] Identifier [ '[' Constant ']' ]
// { ',' type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }

  Token temp ;
  temp = RealGetToken() ;

  if ( !Type_specifier( temp ) ) { // ���O type_specifier �N�O error
    Error3steps();
    Error2( temp ) ;
  } // if()

  gCorrectGrammer.push_back( temp ) ;
  gPeekBuffer.clear() ;
  temp = RealGetToken() ; // �A���U�@��

  if ( temp.type == SAND ) { // // �O '&' �N��
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ; // �A���U�@��
  } // if()

  if ( temp.type != ID ) {
    Error3steps();
    Error2( temp ) ;
  } // if()

  if ( gGlobal_mode ) {
    gTempDglobal.push_back( temp ) ; // �O global mode ���� �N��� global temp ���T�w�A��� global definition ��
  } // if()

  else {
    gTempDlocal.push_back( temp ) ;
  } // else()

  gCorrectGrammer.push_back( temp ) ;
  gPeekBuffer.clear() ;
  temp = RealGetToken() ; // �A���U�@��

  if ( temp.type == LEFTMID ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ; // �A���U�@��
    if ( temp.type != CONSTANT ) {
      Error3steps();
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ; // �A���U�@��
    if ( temp.type != RIGHTMID ) {
      Error3steps();
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ; // �A���U�@��
  } // if()


  while ( temp.type == COMMA ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ; // �A���U�@��

    if ( !Type_specifier( temp ) ) { // ���O type_specifier �N�O error
      Error3steps();
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ; // �A���U�@��

    if ( temp.type == SAND ) { // // �O '&' �N��
      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
      temp = RealGetToken() ; // �A���U�@��
    } // if()

    if ( temp.type != ID ) {
      Error3steps();
      Error2( temp ) ;
    } // if()

    if ( gGlobal_mode ) {
      gTempDglobal.push_back( temp ) ; // �O global mode ���� �N��� global temp ���T�w�A��� global definition ��
    } // if()

    else {
      gTempDlocal.push_back( temp ) ;
    } // else()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ; // �A���U�@��

    if ( temp.type == LEFTMID ) {
      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
      temp = RealGetToken() ; // �A���U�@��
      if ( temp.type != CONSTANT ) {
        Error3steps();
        Error2( temp ) ;
      } // if()

      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
      temp = RealGetToken() ; // �A���U�@��
      if ( temp.type != RIGHTMID ) {
        Error3steps();
        Error2( temp ) ;
      } // if()

      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
      temp = RealGetToken() ; // �A���U�@��
    } // if()


    temp = RealGetToken() ;
  } // while()

} // Formal_parameter_list()

void Compound_statement() { // '{' { declaration | statement } '}'

  Token temp ;
  temp = RealGetToken() ;
  gPass_compound = true ; // ��  if else ���D ���S���j�A��

  if ( temp.type != LEFTBIG ) {
    Error3steps() ;
    Error2( temp ) ;
  } // if()

  gCorrectGrammer.push_back( temp ) ;
  gPeekBuffer.clear() ;
  temp = RealGetToken() ; // �A���U�@��

  while ( Type_specifier( temp ) || Is_statement( temp ) ) {

    if ( Type_specifier( temp ) ) {
      Declaration() ;
    } // if()

    else if ( Is_statement( temp ) ) {
      Statement() ;
    } // else if()

    temp = RealGetToken() ;
  }  // while()

  temp = RealGetToken() ;
  if ( temp.type != RIGHTBIG ) {
    Error3steps() ;
    Error2( temp ) ;
  } // if()

  gCorrectGrammer.push_back( temp ) ;
  gPeekBuffer.clear() ;

} // Compound_statement()

void Declaration() { // function if else ..... �̭����ŧi ���� local ,gGlobal_mode
// type_specifier Identifier rest_of_declarators

  Token temp ;
  temp = RealGetToken() ;
  gGlobal_mode = false ;

  if ( !Type_specifier( temp ) ) {
    Error3steps() ;
    Error2( temp ) ;
  } // if()

  gCorrectGrammer.push_back( temp ) ;
  gPeekBuffer.clear() ;
  temp = RealGetToken() ; // �A���U�@��

  if ( temp.type != ID ) {
    Error3steps() ;
    Error2( temp ) ;
  } // if()

  if ( gGlobal_mode ) {
    gTempDglobal.push_back( temp ) ; // �O global mode ���� �N��� global temp ���T�w�A��� global definition ��
  } // if()

  else {
    gTempDlocal.push_back( temp ) ;
  } // else()

  gCorrectGrammer.push_back( temp ) ;
  gPeekBuffer.clear() ;
  Rest_of_declarators() ;
} // Declaration()

void Statement() {
// ';'     // the null statement
// | expression ';'  /* expression here should not be empty */
// | RETURN [ expression ] ';'
// | compound_statement
// | IF '(' expression ')' statement [ ELSE statement ]
// | WHILE '(' expression ')' statement
// | DO statement WHILE '(' expression ')' ';'

  Token temp ;
  temp = RealGetToken() ;

  if ( temp.type == SEMICOLON ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;

    if ( gIs_function ) { // ���� function ���I�s���� ���ΰ���
      int useless = 1 ;
    } // if()

    else if ( gIs_If_Else_ ) {
      // if ( ! gPass_compound ) { // �N��O�W�ߤ@�檺 if else
      // cout << "> Statement executed ..." << endl ;
      // } // if()
    } // else if()

    else if ( gPass_compound ) guseless = 3 ; // else if()

    else {
      cout << "> Statement executed ..." << endl ;

      if ( gTempDglobal.size() != 0 ) {
        for ( int i = 0 ; i < gTempDglobal.size() ; i++ ) {
          gDefinitionglobal.push_back( gTempDglobal[i] ) ; // �����@�� global ���O �� temp �����F���쥿��global�̭�
        } // for()
      } // if()
    } // else()
  } // if()

  else if ( Is_basic_expression( temp ) ) {
    Expression() ;
    temp = RealGetToken() ;
    if ( temp.type != SEMICOLON ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;

    Is_Done() ;
    Is_SystemFunction() ;

    if ( gIs_function ) { // ���� function ���I�s���� ���ΰ���
      int useless = 1 ;
    } // if()

    else if ( gIs_If_Else_ ) {
      // if ( ! gPass_compound ) { // �N��O�W�ߤ@�檺 if else
      // cout << "> Statement executed ..." << endl ;
      // } // if()
    } // else if()

    else if ( gPass_compound ) guseless = 3 ; // else if()

    else {
      cout << "> Statement executed ..." << endl ;

      if ( gTempDglobal.size() != 0 ) {
        for ( int i = 0 ; i < gTempDglobal.size() ; i++ ) {
          gDefinitionglobal.push_back( gTempDglobal[i] ) ; // �����@�� global ���O �� temp �����F���쥿��global�̭�
        } // for()
      } // if()
    } // else()
  } // else if()

  else if ( temp.type == RETURN ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ;
    if ( Is_basic_expression( temp ) ) {
      Expression() ;
    } // if()

    temp = RealGetToken() ;

    if ( temp.type != SEMICOLON ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;

    if ( gIs_function ) { // ���� function ���I�s���� ���ΰ���
      int useless = 1 ;
    } // if()

    else if ( gIs_If_Else_ ) {
      // if ( ! gPass_compound ) { // �N��O�W�ߤ@�檺 if else
      // cout << "> Statement executed ..." << endl ;
      // } // if()
    } // else if()

    else if ( gPass_compound ) guseless = 3 ; // else if()

    else {
      cout << "> Statement executed ..." << endl ;

      if ( gTempDglobal.size() != 0 ) {
        for ( int i = 0 ; i < gTempDglobal.size() ; i++ ) {
          gDefinitionglobal.push_back( gTempDglobal[i] ) ; // �����@�� global ���O �� temp �����F���쥿��global�̭�
        } // for()
      } // if()
    } // else()
  } // else if()

  else if ( temp.type == LEFTBIG ) {
    Compound_statement() ;
  } // else if()

  else if ( temp.type == IF ) { // | IF '(' expression ')' statement [ ELSE statement ]
    gGlobal_mode = false ; // �̭����X�{�w�q�����ŧi ���� local
    gIs_If_Else_ = true ;
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ;

    if ( temp.type != LEFTPAR ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Expression() ;
    temp = RealGetToken() ;
    if ( temp.type != RIGHTPAR ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Statement() ;
    temp = RealGetToken() ;

    if ( temp.type == ELSE ) {
      gGlobal_mode = false ; // �̭����X�{�w�q�����ŧi ���� local
      gIs_If_Else_ = true ;
      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
      Statement() ;
    } // if()
  } // else if()

  else if ( temp.type == WHILE ) {
    gGlobal_mode = false ; // �̭����X�{�w�q�����ŧi ���� local
    gIs_If_Else_ = true ;
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ;
    if ( temp.type != LEFTPAR ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Expression() ;
    temp = RealGetToken() ;
    if ( temp.type != RIGHTPAR ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Statement() ;
  } // else if()

  else if ( temp.type == DO ) {
    gGlobal_mode = false ; // �̭����X�{�w�q�����ŧi ���� local
    gIs_If_Else_ = true ;
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Statement() ;

    temp = RealGetToken() ;

    if ( temp.type != WHILE ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ;
    if ( temp.type != LEFTPAR ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Expression() ;
    temp = RealGetToken() ;
    if ( temp.type != RIGHTPAR ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ;

    if ( temp.type != SEMICOLON ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;

    if ( gIs_function ) { // ���� function ���I�s���� ���ΰ���
      int useless = 1 ;
    } // if()

    else if ( gIs_If_Else_ ) {
      // if ( ! gPass_compound ) { // �N��O�W�ߤ@�檺 if else
      // cout << "> Statement executed ..." << endl ;
      // } // if()
    } // else if()

    else if ( gPass_compound ) guseless = 3 ; // else if()

    else {
      cout << "> Statement executed ..." << endl ;

      if ( gTempDglobal.size() != 0 ) {
        for ( int i = 0 ; i < gTempDglobal.size() ; i++ ) {
          gDefinitionglobal.push_back( gTempDglobal[i] ) ; // �����@�� global ���O �� temp �����F���쥿��global�̭�
        } // for()
      } // if()
    } // else()

  } // else if()

  else {
    Error3steps() ;
    Error2( temp ) ;
  } // else()

} // Statement()

void Expression() { // basic_expression { ',' basic_expression }

  Basic_expression() ;
  Token temp ;
  temp = RealGetToken() ;

  while ( temp.type == COMMA ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Basic_expression() ;
    temp = RealGetToken() ;
  } // while()
} // Expression()

void Basic_expression() {
// Identifier Rest_of_Identifier_started_basic_exp
// | ( PP | MM ) Identifier Rest_of_PPMM_Identifier_started_basic_exp
// | sign { sign } Signed_unary_exp Romce_and_romloe
// | ( Constant | '(' expression ')' ) romce_and_romloe

  Token temp ;
  temp = RealGetToken() ;

  if ( temp.type == ID ) {
    if ( ! CheckDefinition( temp ) ) { // �ˬd define �S�L����
      Error3steps() ;
      Error3( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    /*
    Token peek ;
    peek = temp ; // �� id ���Ȧs
    gPeekBuffer.clear() ;
    temp = RealGetToken() ;

    if ( temp.type == LEFTPAR ) { // �N�� function �I�s
      if ( CheckFunctionDefinition( peek ) ) {
        gCorrectGrammer.push_back( peek ) ; // ��Ȧs�� id �s�i�h
      } // if()

      else {
        Error3steps() ;
        Error3( peek ) ;
      } // else()
    } // if()

    else { // �N��O�O��
      if ( ! CheckDefinition( peek ) ) { // �ˬd define �S�L����
        Error3steps() ;
        Error3( peek ) ;
      } // if()

      gCorrectGrammer.push_back( peek ) ;
    } // else()
    */

    Rest_of_Identifier_started_basic_exp() ;
  } // if()

  else if ( temp.type == PP || temp.type == MM ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ;
    if ( temp.type != ID ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    if ( ! CheckDefinition( temp ) ) { // �ˬd define �S�L����
      Error3steps() ;
      Error3( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Rest_of_PPMM_Identifier_started_basic_exp() ;
  } // else if()

  else if ( Is_sign( temp ) ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ;
    while ( Is_sign( temp ) ) {
      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
      temp = RealGetToken() ;
    } // while()

    Signed_unary_exp() ;
    Romce_and_romloe() ;
  } // else if()

  else if ( temp.type == CONSTANT || temp.type == LEFTPAR ) {
    if ( temp.type == CONSTANT ) {
      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
      Romce_and_romloe() ;
    } // if()

    else if ( temp.type == LEFTPAR ) {
      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
      Expression() ;
      temp = RealGetToken() ;
      if ( temp.type != RIGHTPAR ) {
        Error3steps() ;
        Error2( temp ) ;
      } // if()

      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
      Romce_and_romloe() ;
    } // else if()
  } // else if()

  else {
    Error3steps() ;
    Error2( temp ) ;
  } // else()
} // Basic_expression()

void Rest_of_Identifier_started_basic_exp() {
// [ '[' expression ']' ]
// ( assignment_operator basic_expression
// |
// [ PP | MM ] romce_and_romloe
// )
// | '(' [ actual_parameter_list ] ')' romce_and_romloe

  Token temp ;
  temp = RealGetToken() ;

  if ( temp.type == LEFTPAR ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ;

    if ( Is_basic_expression( temp ) ) {
      Actual_parameter_list() ;
    } // if()

    temp = RealGetToken() ;
    if ( temp.type != RIGHTPAR ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Romce_and_romloe() ;
    return ;
  } // if()

  if ( temp.type == LEFTMID ) {

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Expression() ;
    temp = RealGetToken() ;
    if ( temp.type != RIGHTMID ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ;
  } // if()

  if ( Assignment_operator( temp ) ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Basic_expression() ;
  } // if()

  else {
    if ( temp.type == PP || temp.type == MM ) {
      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
    } // if()

    Romce_and_romloe() ;
  } // else
} // Rest_of_Identifier_started_basic_exp()

void Rest_of_PPMM_Identifier_started_basic_exp() {
// [ '[' Expression ']' ] romce_and_romloe

  Token temp ;
  temp = RealGetToken() ;
  if ( temp.type == LEFTMID ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Expression() ;
    temp = RealGetToken() ;
    if ( temp.type != RIGHTMID ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
  } // if()

  Romce_and_romloe() ;
} // Rest_of_PPMM_Identifier_started_basic_exp()

bool Assignment_operator( Token a ) {
// '=' | TE | DE | RE | PE | ME

  if ( a.type == SEQUAL || a.type == TE || a.type == DE
       || a.type == RE || a.type == PE || a.type == ME ) {
    return true ;
  } // if()

  else return false ; // else()
} // Assignment_operator()

void Romce_and_romloe() {
// Rest_of_maybe_logical_OR_exp [ '?' Basic_expression ':' Basic_expression ]

  Rest_of_maybe_logical_OR_exp() ;
  Token temp ;
  temp = RealGetToken() ;
  if ( temp.type == QUES ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Basic_expression() ;
    temp = RealGetToken() ;
    if ( temp.type != COLON ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Basic_expression() ;
  } // if()
} // Romce_and_romloe()

void Rest_of_maybe_logical_OR_exp() {
// Rest_of_maybe_logical_AND_exp { OR Maybe_logical_AND_exp }

  Rest_of_maybe_logical_AND_exp() ;
  Token temp ;
  temp = RealGetToken() ;
  while ( temp.type == OR ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Maybe_logical_AND_exp() ;
    temp = RealGetToken() ;
  } // while()
} // Rest_of_maybe_logical_OR_exp()

void Maybe_logical_AND_exp() {
// Maybe_bit_OR_exp { AND Maybe_bit_OR_exp }

  Maybe_bit_OR_exp() ;
  Token temp ;
  temp = RealGetToken() ;
  while ( temp.type == AND ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Maybe_bit_OR_exp() ;
    temp = RealGetToken() ;
  } // while()
} // Maybe_logical_AND_exp()

void Rest_of_maybe_logical_AND_exp() {
// Rest_of_maybe_bit_OR_exp { AND Maybe_bit_OR_exp }

  Rest_of_maybe_bit_OR_exp() ;
  Token temp ;
  temp = RealGetToken() ;
  while ( temp.type == AND ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Maybe_bit_OR_exp() ;
    temp = RealGetToken() ;
  } // while()
} // Rest_of_maybe_logical_AND_exp()

void Maybe_bit_OR_exp() {
// Maybe_bit_ex_OR_exp { '|' Maybe_bit_ex_OR_exp }

  Maybe_bit_ex_OR_exp() ;
  Token temp ;
  temp = RealGetToken() ;
  while ( temp.type == SOR ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Maybe_bit_ex_OR_exp() ;
    temp = RealGetToken() ;
  } // while()
} // Maybe_bit_OR_exp()

void Rest_of_maybe_bit_OR_exp() {
// Rest_of_maybe_bit_ex_OR_exp { '|' Maybe_bit_ex_OR_exp }

  Rest_of_maybe_bit_ex_OR_exp() ;
  Token temp ;
  temp = RealGetToken() ;
  while ( temp.type == SOR ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Maybe_bit_ex_OR_exp() ;
    temp = RealGetToken() ;
  } // while()
} // Rest_of_maybe_bit_OR_exp()

void Maybe_bit_ex_OR_exp() {
// Maybe_bit_AND_exp { '^' Maybe_bit_AND_exp }

  Maybe_bit_AND_exp() ;
  Token temp ;
  temp = RealGetToken() ;
  while ( temp.type == CARET ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Maybe_bit_AND_exp() ;
    temp = RealGetToken() ;
  } // while()
} // Maybe_bit_ex_OR_exp()

void Rest_of_maybe_bit_ex_OR_exp() {
// Rest_of_maybe_bit_AND_exp { '^' Maybe_bit_AND_exp }

  Rest_of_maybe_bit_AND_exp() ;
  Token temp ;
  temp = RealGetToken() ;
  while ( temp.type == CARET ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Maybe_bit_AND_exp() ;
    temp = RealGetToken() ;
  } // while()
} // Rest_of_maybe_bit_ex_OR_exp()

void Maybe_bit_AND_exp() {
// Maybe_equality_exp { '&' Maybe_equality_exp }

  Maybe_equality_exp() ;
  Token temp ;
  temp = RealGetToken() ;
  while ( temp.type == SAND ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Maybe_equality_exp() ;
    temp = RealGetToken() ;
  } // while()
} // Maybe_bit_AND_exp()

void Rest_of_maybe_bit_AND_exp() {
// Rest_of_maybe_equality_exp { '&' Maybe_equality_exp }

  Rest_of_maybe_equality_exp() ;
  Token temp ;
  temp = RealGetToken() ;
  while ( temp.type == SAND ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Maybe_equality_exp() ;
    temp = RealGetToken() ;
  } // while()
} // Rest_of_maybe_bit_AND_exp()

void Maybe_equality_exp() {
// Maybe_relational_exp
// { ( EQ | NEQ ) Maybe_relational_exp}

  Maybe_relational_exp() ;
  Token temp ;
  temp = RealGetToken() ;
  while ( temp.type == EQ || temp.type == NEQ ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Maybe_relational_exp() ;
    temp = RealGetToken() ;
  } // while()
} // Maybe_equality_exp()

void Rest_of_maybe_equality_exp() {
// Rest_of_maybe_relational_exp
// { ( EQ | NEQ ) Maybe_relational_exp }

  Rest_of_maybe_relational_exp() ;
  Token temp ;
  temp = RealGetToken() ;
  while ( temp.type == EQ || temp.type == NEQ ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Maybe_relational_exp() ;
    temp = RealGetToken() ;
  } // while()
} // Rest_of_maybe_equality_exp()

void Maybe_relational_exp() {
// Maybe_shift_exp
// { ( '<' | '>' | LE | GE ) Maybe_shift_exp }
  Maybe_shift_exp() ;
  Token temp ;
  temp = RealGetToken() ;
  while ( temp.type == GREATER || temp.type == LESSER
          || temp.type == LE || temp.type == GE ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Maybe_shift_exp() ;
    temp = RealGetToken() ;
  } // while()
} // Maybe_relational_exp()

void Rest_of_maybe_relational_exp() {
// Rest_of_maybe_shift_exp
// { ( '<' | '>' | LE | GE ) Maybe_shift_exp }
  Rest_of_maybe_shift_exp() ;
  Token temp ;
  temp = RealGetToken() ;

  while ( temp.type == GREATER || temp.type == LESSER
          || temp.type == LE || temp.type == GE ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Maybe_shift_exp() ;
    temp = RealGetToken() ;
  } // while()
} // Rest_of_maybe_relational_exp()

void Maybe_shift_exp() {
// Maybe_additive_exp { ( LS | RS ) Maybe_additive_exp }
  Maybe_additive_exp() ;
  Token temp ;
  temp = RealGetToken() ;

  while ( temp.type == LS || temp.type == RS ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Maybe_additive_exp() ;
    temp = RealGetToken() ;
  } // while()

} // Maybe_shift_exp()

void Rest_of_maybe_shift_exp() {
// Rest_of_maybe_additive_exp { ( LS | RS ) Maybe_additive_exp }
  Rest_of_maybe_additive_exp() ;

  Token temp ;
  temp = RealGetToken() ;

  while ( temp.type == LS || temp.type == RS ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Maybe_additive_exp() ;
    temp = RealGetToken() ;
  } // while()

} // Rest_of_maybe_shift_exp()

void Maybe_additive_exp() {
// Maybe_mult_exp { ( '+' | '-' ) Maybe_mult_exp }
  Maybe_mult_exp() ;
  Token temp ;
  temp = RealGetToken() ;

  while ( temp.type == PLUS || temp.type == MINUS ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Maybe_mult_exp() ;
    temp = RealGetToken() ;
  } // while()

} // Maybe_additive_exp()

void Rest_of_maybe_additive_exp() {
// Rest_of_maybe_mult_exp { ( '+' | '-' ) Maybe_mult_exp }

  Rest_of_maybe_mult_exp() ;

  Token temp ;
  temp = RealGetToken() ;

  while ( temp.type == PLUS || temp.type == MINUS ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Maybe_mult_exp() ;
    temp = RealGetToken() ;
  } // while()
} // Rest_of_maybe_additive_exp()

void Maybe_mult_exp() { // unary_exp rest_of_maybe_mult_exp
  Unary_exp() ;
  Rest_of_maybe_mult_exp() ;
} // Maybe_mult_exp()

void Rest_of_maybe_mult_exp() { // { ( '*' | '/' | '%' ) unary_exp }  /* could be empty ! */

  Token temp ;
  temp = RealGetToken() ;

  while ( temp.type == MUL || temp.type == DIV || temp.type == PERCENTAGE ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Unary_exp() ;
    temp = RealGetToken() ;
  } // while()
} // Rest_of_maybe_mult_exp()

void Unary_exp() {
// sign { sign } signed_unary_exp
// | unsigned_unary_exp
// | ( PP | MM ) Identifier [ '[' expression ']' ]

  Token temp ;
  temp = RealGetToken() ;

  if ( Is_sign( temp ) ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ;

    while ( Is_sign( temp ) ) {
      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
      temp = RealGetToken() ;
    } // while()

    Signed_unary_exp() ;
  } // if()

  else if ( temp.type == PP || temp.type == MM ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ;
    if ( temp.type != ID ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    if ( ! CheckDefinition( temp ) ) { // �ˬd define �S�L����
      Error3steps() ;
      Error3( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ;
    if ( temp.type == LEFTMID ) {
      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
      Expression() ;
      temp = RealGetToken() ;
      if ( temp.type != RIGHTMID ) {
        Error3steps() ;
        Error2( temp ) ;
      } // if()

      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
    } // if()
  } // else if()

  else {
    Unsigned_unary_exp() ;
  } // else()
} // Unary_exp()

void Unsigned_unary_exp() {
// Identifier [ '(' [ actual_parameter_list ] ')'
//              | [ '[' expression ']' ] [ ( PP | MM ) ] ]
// | Constant
// | '(' expression ')'

  Token temp ;
  temp = RealGetToken() ;

  if ( temp.type == ID ) {
    if ( ! CheckDefinition( temp ) ) { // �ˬd define �S�L����
      Error3steps() ;
      Error3( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ;

    if ( temp.type == LEFTPAR ) {
      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
      temp = RealGetToken() ;

      if ( Is_basic_expression( temp ) ) {
        Actual_parameter_list() ;
      } // if()

      temp = RealGetToken() ;

      if ( temp.type != RIGHTPAR ) {
        Error3steps() ;
        Error2( temp ) ;
      } // if()

      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
    } // if() ���O leftpar �S����

    else if ( temp.type == LEFTMID ) {
      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;

      Expression() ;
      temp = RealGetToken() ;

      if ( temp.type != RIGHTMID ) {
        Error3steps() ;
        Error2( temp ) ;
      } // if()

      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
      temp = RealGetToken() ;

      if ( temp.type == PP || temp.type == MM ) {
        gCorrectGrammer.push_back( temp ) ;
        gPeekBuffer.clear() ;
      } // if()
    } // else if()

    else if ( temp.type == PP || temp.type == MM ) {
      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
    } // else if()
  } // if()

  else if ( temp.type == CONSTANT ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
  } // else if()

  else if ( temp.type == LEFTPAR ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;

    Expression() ;
    temp = RealGetToken() ;
    if ( temp.type != RIGHTPAR ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
  } // else if()

  else {
    Error3steps() ;
    Error2( temp ) ;
  } // else()

} // Unsigned_unary_exp()

void Signed_unary_exp() {
// Identifier [ '(' [ actual_parameter_list ] ')'
//              | '[' expression ']'  ]
// | Constant
// | '(' expression ')'

  Token temp ;
  temp = RealGetToken() ;

  if ( temp.type == ID ) {

    if ( ! CheckDefinition( temp ) ) { // �ˬd define �S�L����
      Error3steps() ;
      Error3( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ;

    if ( temp.type == LEFTPAR ) {
      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
      temp = RealGetToken() ;

      if ( Is_basic_expression( temp ) ) {
        Actual_parameter_list() ;
      } // if()

      temp = RealGetToken() ;

      if ( temp.type != RIGHTPAR ) {
        Error3steps() ;
        Error2( temp ) ;
      } // if()

      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
    } // if() ���O leftpar �S����

    else if ( temp.type == LEFTMID ) {
      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;

      Expression() ;
      temp = RealGetToken() ;

      if ( temp.type != RIGHTMID ) {
        Error3steps() ;
        Error2( temp ) ;
      } // if()

      gCorrectGrammer.push_back( temp ) ;
      gPeekBuffer.clear() ;
    } // else if()
  } // if()

  else if ( temp.type == CONSTANT ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
  } // else if()

  else if ( temp.type == LEFTPAR ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    temp = RealGetToken() ;

    Expression() ;
    temp = RealGetToken() ;
    if ( temp.type != RIGHTPAR ) {
      Error3steps() ;
      Error2( temp ) ;
    } // if()

    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
  } // else if()

  else {
    Error3steps() ;
    Error2( temp ) ;
  } // else()

} // Signed_unary_exp()

void Actual_parameter_list() { // basic_expression { ',' basic_expression }

  Basic_expression() ;
  Token temp ;
  temp = RealGetToken() ;

  while ( temp.type == COMMA ) {
    gCorrectGrammer.push_back( temp ) ;
    gPeekBuffer.clear() ;
    Basic_expression() ;
    temp = RealGetToken() ;
  } // while()

} // Actual_parameter_list()

void IfFunctionWrong() {
  int temp = 0 ;

  for ( int i = 0 ; i < gFunctionName.size() ; i++ ) {
    if ( g_if_function_wrong == gFunctionName[i] ) {
      gFunctionName.erase( gFunctionName.begin() + i ) ;
      temp = 1 ;
    } // if()
  } // for()

  if ( temp == 1 ) {
    for ( int i = 0 ; i < gWholeFunction.size() ; i++ ) {
      if ( gWholeFunction[i][1].content == g_if_function_wrong ) {
        gWholeFunction.erase( gWholeFunction.begin() + i ) ;
      } // if()
    } // for()
  } // if()

} // IfFunctionWrong()

int main()
{
  int question ;
  cin >> question ;
  cout << "Our-C running ..." << endl ;
  gSystemFunction.push_back( "ListAllVariables" ) ;
  gSystemFunction.push_back( "ListAllFunctions" ) ;
  gSystemFunction.push_back( "ListVariable" ) ;
  gSystemFunction.push_back( "ListFunction" ) ;
  gSystemFunction.push_back( "Done" ) ;
  // char temp ;
  // temp = getchar() ;

  gline_num = 0 ;
  gIs_Definition = false ;
  gGlobal_mode = true ;
  gIs_function = false ;
  gIs_If_Else_ = false ;
  gPass_compound = false ;
  g_if_function_wrong = "121321321" ;

  while ( ! gIsEnd ) {

    try {
      User_input() ;
    } // try()

    catch ( string a ) {

      if ( gthrowspecial == 1 ) {
        cout << "> " << a << endl ;
      } // if()

      else {
        int num ;
        num = gline_num ;
        if ( num == 0 ) num = 1 ; // if()

        cout << "> Line " ;
        cout << num ;
        cout << " : " ;
        cout << a << endl ;
        IfFunctionWrong() ;
      } // else()

      // �����~���� ��l���̪챡�p

      gline_num = 0 ;
      ReadtoEnd() ;
      gIs_Definition = false ;
      gGlobal_mode = true ;
      gIs_function = false ;
      gIs_If_Else_ = false ;
      gPass_compound = false ;
      gTempDlocal.clear() ;
      gTempDglobal.clear() ;
      g_if_function_wrong = "121321321" ;

    } // catch()

    catch ( int a ) {
      cout << "Statement executed ..." << endl ;
      gline_num = 0 ;
      gCorrectGrammer.clear() ;
      gIs_Definition = false ;
      gGlobal_mode = true ;
      gIs_function = false ;
      gIs_If_Else_ = false ;
      gPass_compound = false ;
      gTempDlocal.clear() ;
      gTempDglobal.clear() ;
      g_if_function_wrong = "121321321" ;
    } // catch()
  } // while()

  // Testall() ;

    /*
    cout << gDefinitionglobal.size() <<endl ;
    for ( int i = 0 ; i < gDefinitionglobal.size() ; i ++ ) {
      cout << gDefinitionglobal[i].content << endl ;
    } // for()

    for ( int i = 0 ; i < gFunctionName.size() ; i ++ ) {
      cout << gFunctionName[i] << endl ;
    } // for()
    */

  return 0;
} // main()
