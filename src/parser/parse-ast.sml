
structure ParseAST =
struct
open Operators

open OpAST
   datatype ParseRule = OperatorNameComponent of UTF8String.t * operator
                            | OperatorInternal of operator
                            | PrefixNoneAssoc of operator
                            | PrefixRightAssoc of int
                            | PostfixNoneAssoc of operator
                            | PostfixLeftAssoc of int
                            | InfixNoneAssoc of operator
                            | InfixLeftAssoc of int
                            | InfixLeftAssocLeftArrow of operator
                            | InfixRightAssoc of int
                            | InfixRightAssocRightArrow of operator
                            | Many1 
                            | EOF
                            | ExpWithEOF
                            | UnknownId 
                            | UnknownIdComp of UTF8Char.t
                            | Binding of UTF8String.t
                            | QuotedName of UTF8String.t
                            | StringLiteral of UTF8String.t * MixedStr.quoteinfo
                            | UnparsedExpr of (MixedStr.t  * MixedStr.quoteinfo)
                            | UnparsedDecl of (MixedStr.t * MixedStr.endinginfo) list  * MixedStr.quoteinfo (* any quoted thing is treated as unparsed arg *)
                            (* should just go ahead and parse the expression *)
                            (* | UnparsedExpr of MixedStr.t any quoted thing is treated as unparsed arg *)
                            | PlaceHolder (* should not appear anywhere in final result *)
        datatype ParseOpAST = ParseOpAST of ParseRule * ParseOpAST list
        exception ParseFailure of string

    exception InternalFailure of OpAST * OpAST

    fun opastAppendArg  (original :  OpAST )(arg : OpAST)  : OpAST = 
        case original of (OpAST (oper, l)) => OpAST(oper, l@[arg])
    fun opastPrependArg  (arg : OpAST) (original :  OpAST ) : OpAST = 
        case original of (OpAST (oper, l)) => OpAST(oper, arg :: l)
                        | _ => raise InternalFailure (arg, original)
    
end