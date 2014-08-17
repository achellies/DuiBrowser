#ifndef CSSGrammar_h
#define CSSGrammar_h

/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOKEN_EOF = 0,
     UNIMPORTANT_TOK = 258,
     WHITESPACE = 259,
     SGML_CD = 260,
     INCLUDES = 261,
     DASHMATCH = 262,
     BEGINSWITH = 263,
     ENDSWITH = 264,
     CONTAINS = 265,
     STRING = 266,
     IDENT = 267,
     NTH = 268,
     HEX = 269,
     IDSEL = 270,
     IMPORT_SYM = 271,
     PAGE_SYM = 272,
     MEDIA_SYM = 273,
     FONT_FACE_SYM = 274,
     CHARSET_SYM = 275,
     NAMESPACE_SYM = 276,
     WEBKIT_RULE_SYM = 277,
     WEBKIT_DECLS_SYM = 278,
     WEBKIT_KEYFRAME_RULE_SYM = 279,
     WEBKIT_KEYFRAMES_SYM = 280,
     WEBKIT_VALUE_SYM = 281,
     WEBKIT_MEDIAQUERY_SYM = 282,
     WEBKIT_SELECTOR_SYM = 283,
     WEBKIT_VARIABLES_SYM = 284,
     WEBKIT_DEFINE_SYM = 285,
     VARIABLES_FOR = 286,
     WEBKIT_VARIABLES_DECLS_SYM = 287,
     ATKEYWORD = 288,
     IMPORTANT_SYM = 289,
     MEDIA_ONLY = 290,
     MEDIA_NOT = 291,
     MEDIA_AND = 292,
     QEMS = 293,
     EMS = 294,
     EXS = 295,
     PXS = 296,
     CMS = 297,
     MMS = 298,
     INS = 299,
     PTS = 300,
     PCS = 301,
     DEGS = 302,
     RADS = 303,
     GRADS = 304,
     MSECS = 305,
     SECS = 306,
     HERZ = 307,
     KHERZ = 308,
     DIMEN = 309,
     PERCENTAGE = 310,
     FLOATTOKEN = 311,
     INTEGER = 312,
     URI = 313,
     FUNCTION = 314,
     NOTFUNCTION = 315,
     UNICODERANGE = 316,
     VARCALL = 317,
     LOWEST_PREC = 318
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 57 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"

    bool boolean;
    char character;
    int integer;
    double number;
    CSSParserString string;

    CSSRule* rule;
    CSSRuleList* ruleList;
    CSSSelector* selector;
    CSSSelector::Relation relation;
    MediaList* mediaList;
    MediaQuery* mediaQuery;
    MediaQuery::Restrictor mediaQueryRestrictor;
    MediaQueryExp* mediaQueryExp;
    CSSParserValue value;
    CSSParserValueList* valueList;
    Vector<MediaQueryExp*>* mediaQueryExpList;
    WebKitCSSKeyframeRule* keyframeRule;
    WebKitCSSKeyframesRule* keyframesRule;
    float val;



/* Line 1676 of yacc.c  */
#line 141 "CSSGrammar.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif




#endif
