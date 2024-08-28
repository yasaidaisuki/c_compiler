digit                   [0-9]
line                    [a-zA-Z_]
%include {
    #include "y.tab.h"
    int coutn = 0; // line counter
%}

%%

"{"                 {return OPEN_BRACE;}
"}"                 {return CLOSE_BRACE;}
"("                 {return OPEN_PAREN;}
")"                 {return CLOSE PAREN;}
";"                 {return SEMICOLON;}
"int"               {return KEYWORD_INT;}
"return"            {return RETURN;}    
[0-9]+              {yyval=atoi(yytext); return INT_LITERAL;}        
[a-zA-Z]\w*         {yyval=atoi(yytext); return IDENTIFIER;}
%%

int main() {
    yylex();
}