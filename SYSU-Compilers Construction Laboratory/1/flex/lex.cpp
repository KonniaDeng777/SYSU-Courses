#include "lex.hpp"
#include <iostream>

void print_token();

namespace lex {

static const char* kTokenNames[] = 
{
  "identifier",   "constant",   "string_literal",
  "int",          "return",             "l_brace",
  "r_brace",      "l_square",           "r_square",
  "l_paren",      "r_paren",            "semi",
  "equal",        "plus",               "comma",
  "auto",         "_bool",              
  "numeric_constant", "minus",    "star",
  "greater","equalequal","void","if","pipepipe","else","ampamp","less","while",
  "slash","lessequal","exclaim","exclaimequal","percent","greaterequal","const",
  "continue"
};

const char* id2str(Id id)
{
  static char sCharBuf[2] = { 0, 0 };
  if (id == Id::YYEOF) {
    return "eof";
  }
  else if (id < Id::IDENTIFIER) 
  {
    sCharBuf[0] = char(id);
    return sCharBuf;
  }
  return kTokenNames[int(id) - int(Id::IDENTIFIER)];
}

G g;

int come(int tokenId, const char* yytext, int yyleng, int yylineno)
{
  g.mId = Id(tokenId);
  g.mText = { yytext, std::size_t(yyleng) };
  //g.mLine = yylineno;
  //g.mLeadingSpace = (yytext[-1] == ' ');
  print_token();
  g.mLeadingSpace=false;
  g.mStartOfLine = false;
  return tokenId;
}

} // namespace lex
