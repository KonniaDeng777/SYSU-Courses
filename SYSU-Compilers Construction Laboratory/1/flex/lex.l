%{
#include "lex.hpp"
/* 所有代码全部抽离出来，放到 lex.hpp 和 lex.cpp 里 */

using namespace lex;

#define ADDCOL() g.mColumn += yyleng;

#define COME(id) return come(id, yytext, yyleng, yylineno)
int number=1;
std::string filename;

%}

%option 8bit warn noyywrap yylineno

D     [0-9]
L     [a-zA-Z_]
IS    ((u|U)|(u|U)?(l|L|ll|LL)|(l|L|ll|LL)(u|U))

%%

"auto"      {ADDCOL();COME(AUTO);}
"_bool"     {ADDCOL();COME(_BOOL);}
"int"       {ADDCOL();COME(INT);}
"return"    { ADDCOL();COME(RETURN); }
"yyempty"   {ADDCOL();COME(YYEMPTY);} 
"yyerror"   {ADDCOL();COME(YYerror);}
"yyundef"   {ADDCOL();COME(YYUNDEF);}
"("         { ADDCOL();COME(L_PAREN);}
")"         { ADDCOL();COME(R_PAREN); }
"["         { ADDCOL(); COME(L_SQUARE); }
"]"         { ADDCOL(); COME(R_SQUARE); }
"{"         { ADDCOL(); COME(L_BRACE); }
"}"         { ADDCOL(); COME(R_BRACE); }
"+"         { ADDCOL();COME(PLUS); }

";"         { ADDCOL(); COME(SEMI); }
","         { ADDCOL(); COME(COMMA); }

"="         { ADDCOL(); COME(EQUAL); }
"-"         { ADDCOL();COME(MINUS);}
"*"         {ADDCOL();COME(STAR);}
">"         {ADDCOL();COME(GREATER);}
"=="        {ADDCOL();COME(EQUALEQUAL);}
"void"      {ADDCOL();COME(VOID);}
"if"         {ADDCOL();COME(IF);}
"||"         {ADDCOL();COME(PIPEPIPE);}
"else"       {ADDCOL();COME(ELSE);}
"&&"         {ADDCOL();COME(AMPAMP);}
"<"          {ADDCOL();COME(LESS);}
"while"      {ADDCOL();COME(WHILE);}
"/"          {ADDCOL();COME(SLASH);}
"<="         {ADDCOL();COME(LESSEQUAL);}
"!"          {ADDCOL();COME(EXCLAIM);}
"!="         {ADDCOL();COME(EXCLAIMEQUAL);}
"%"          {ADDCOL();COME(PERCENT);}
">="         {ADDCOL();COME(GREATEREQUAL);}
"const"      {ADDCOL();COME(CONST);}
"continue"   {ADDCOL();COME(CONTINUE);}


{L}({L}|{D})*         { ADDCOL();COME(IDENTIFIER);}
L?\"(\\.|[^\\"\n])*\" { ADDCOL(); COME(STRING_LITERAL);}

[0-9]+                {ADDCOL();COME(NUMERIC_CONSTANT);}
0x[0-9a-fA-F]+        {ADDCOL(); COME(NUMERIC_CONSTANT);}
0[0-7]*{IS}?          { ADDCOL(); COME(CONSTANT); }
[1-9]{D}*{IS}?        { ADDCOL(); COME(CONSTANT); }

^#[^\n]*              { 
                        g.mStartOfLine=true;
                        g.mColumn=0;
                        if(g.mFile.empty())
                        {
                            std::string line(yytext);
                            size_t start = line.find('"'); 
                            size_t end = line.find('"', start + 1); 
                            if (start != std::string::npos && end != std::string::npos) 
                            {
                                g.mFile = line.substr(start + 1, end - start - 1); 
                                filename=g.mFile;
                            }
                            size_t hashPos = line.find('#');
                            size_t numberPos = line.find_first_of("0123456789", hashPos + 1);
                            if (numberPos != std::string::npos)
                            {
                                std::string numberString = line.substr(numberPos);
                                number = std::stoi(numberString);
                                g.mLine = number - 1; 
                            }
                        }
                        else 
                        {                   
                            std::string line(yytext);
                            size_t start = line.find('"'); 
                            size_t end = line.find('"', start + 1); 
                            if (start != std::string::npos && end != std::string::npos) 
                            {
                                std::string str = line.substr(start + 1, end - start - 1); 
                                if (str != filename) {
                                    g.mLine = yylineno;
                                    g.mFile=str;
                                } 
                                else 
                                {
                                    size_t hashPos = line.find('#');
                                    size_t numberPos = line.find_first_of("0123456789", hashPos + 1);
                                    if (numberPos != std::string::npos)
                                    {
                                        std::string numberString = line.substr(numberPos);
                                        number = std::stoi(numberString);
                                        g.mLine = number - 1; 
                                        g.mFile=str;
                                    }
                                }
                            
                            }
                        }
                       return ~YYEOF; } /* 预处理信息处理，可以从预处理信息中获得文件名以及行号 */



[ \t\v]+                  { g.mLeadingSpace = true;  ADDCOL(); return ~YYEOF; }
[\n]           { g.mStartOfLine = true; g.mLine++; g.mColumn=0; return ~YYEOF;} 
[\f]            { g.mStartOfLine = true; g.mLine++; g.mColumn=0;return ~YYEOF;} 
<<EOF>>     { g.mStartOfLine=false;g.mColumn+=2; g.mLine--; g.mText=""; COME(YYEOF); }


%%

/* about symbols avaliable (yytext, yyleng etc.) in the context of Flex:
 * https://ftp.gnu.org/old-gnu/Manuals/flex-2.5.4/html_node/flex_14.html
 * https://ftp.gnu.org/old-gnu/Manuals/flex-2.5.4/html_node/flex_15.html
*/
