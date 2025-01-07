%{
#include <stdio.h>
%}

%%
if|else|printf|for|while|return|int|float|char|void|double      {printf("%s is a keyword\n", yytext);}
[0-9]+                                                          {printf("%s is a number\n", yytext);}
[a-zA-Z]+                                                       {printf("%s is a word\n", yytext);}
"+"|"-"|"*"|"/"|"="|">"|"<"|"&"|"|"                             {printf("%s is an operator\n", yytext);}
"{"|"}"|"("|")"|";"|","                                         {printf("%s is a punctuation\n", yytext);}
[ \t\n]                                                         ; // skip whitespace (spaces, tabs, newlines)
.                                                               {printf("Unknown character: %s\n", yytext);}
%%

int main(){
    printf("\nEnter the string here:\n");
    yylex(); //Call the lexical analyzer to start scanning input 
    return 0;
}

//yywrap function tells end of the file input is reached
int yywrap(){
    return 1;
}