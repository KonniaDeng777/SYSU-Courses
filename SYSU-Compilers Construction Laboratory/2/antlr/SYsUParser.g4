parser grammar SYsUParser;

options {
  tokenVocab=SYsULexer;
}

primaryExpression
    :   Identifier
    |   Constant
    |   LeftParen additiveExpression RightParen
    ;

arrayAccessExpression
    :   postfixExpression LeftBracket additiveExpression RightBracket
    ;

postfixExpression
    :   primaryExpression  
    ;

unaryExpression
    :
    (postfixExpression
    |   unaryOperator unaryExpression
    )
    ;

//(c+a)*(b-d)
parenthesizedExpression
    :   LeftParen additiveExpression RightParen
    ;

arrayAccess
    :   Identifier LeftBracket additiveExpression RightBracket LeftBracket additiveExpression RightBracket
    |   Identifier LeftBracket additiveExpression RightBracket
    ;


unaryOperator
    :   Plus | Minus | Exclaim
    ;

additiveExpression
    :   multiplicativeExpression ((Plus|Minus) multiplicativeExpression)*
    ;

relationExpression
    :   additiveExpression ((Greater|Less|GreaterEqual|LessEqual|ExclaimEqual) additiveExpression)*
    ;

multiplicativeExpression
    :   unaryExpression ((Star|Slash|Percent) unaryExpression)*
    |   primaryExpression ((Star|Slash|Percent) primaryExpression)*
    |   parenthesizedExpression ((Star|Slash|Percent) parenthesizedExpression)*
    ;

assignmentExpression
    :   additiveExpression
    |   relationExpression
    |   unaryExpression Equal assignmentExpression
    ;

expression
    :   assignmentExpression (Comma assignmentExpression)*
    ;


declaration
    :   declarationSpecifiers initDeclaratorList? Semi
    ;

declarationSpecifiers
    :   declarationSpecifier+
    ;

declarationSpecifier
    :   typeSpecifier
    ;

initDeclaratorList
    :   initDeclarator (Comma initDeclarator)*
    ;

initDeclarator
    :   declarator (Equal initializer)?
    ;


typeSpecifier
    :   Int
    |   Const
    ;


declarator
    :   directDeclarator
    ;

directDeclarator
    :   Identifier
    |   directDeclarator LeftBracket assignmentExpression? RightBracket
    ;

identifierList
    :   Identifier (Comma Identifier)*
    ;

initializer
    :   assignmentExpression
    |   LeftBrace initializerList? Comma? RightBrace
    ;

initializerList
    // :   designation? initializer (Comma designation? initializer)*
    :   initializer (Comma initializer)*
    |   assignmentExpression (Comma assignmentExpression)*
    ;

statement
    :   compoundStatement
    |   expressionStatement
    |   jumpStatement
    |   whileStatement
    ;

compoundStatement
    :   LeftBrace blockItemList? RightBrace
    ;


ifStatement
    :   If LeftParen expression RightParen statement
    |   If LeftParen expression RightParen statement Else statement
    ;


whileStatement
    :   While LeftParen expression RightParen statement
    ;


returnStatement
    :   Return expression? 
    ;

blockItemList
    :   blockItem+
    ;

blockItem
    :   statement
    |   declaration
    ;

expressionStatement
    :   expression? Semi
    ;


jumpStatement
    :   (Return expression?) 
    Semi
    ;

compilationUnit
    :   translationUnit? EOF
    ;

translationUnit
    :   externalDeclaration+
    ;

externalDeclaration
    :   functionDefinition
    |   declaration
    ;

functionDefinition
    : declarationSpecifiers directDeclarator LeftParen RightParen compoundStatement
    ;


functionCall
    :   Identifier LeftParen (expression (Comma expression)*)? RightParen
    ;

arrayInitializer
    :   LeftBrace initializerList RightBrace
    ;

