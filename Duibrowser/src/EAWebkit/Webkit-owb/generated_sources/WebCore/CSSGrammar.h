#ifndef CSSGRAMMAR_H 
#define CSSGRAMMAR_H 
/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
     WEBKIT_VALUE_SYM = 279,
     WEBKIT_MEDIAQUERY_SYM = 280,
     WEBKIT_VARIABLES_SYM = 281,
     WEBKIT_VARIABLES_DECLS_SYM = 282,
     IMPORTANT_SYM = 283,
     MEDIA_ONLY = 284,
     MEDIA_NOT = 285,
     MEDIA_AND = 286,
     QEMS = 287,
     EMS = 288,
     EXS = 289,
     PXS = 290,
     CMS = 291,
     MMS = 292,
     INS = 293,
     PTS = 294,
     PCS = 295,
     DEGS = 296,
     RADS = 297,
     GRADS = 298,
     MSECS = 299,
     SECS = 300,
     HERZ = 301,
     KHERZ = 302,
     DIMEN = 303,
     PERCENTAGE = 304,
     FLOATTOKEN = 305,
     INTEGER = 306,
     URI = 307,
     FUNCTION = 308,
     NOTFUNCTION = 309,
     UNICODERANGE = 310,
     VARCALL = 311
   };
#endif
/* Tokens.  */
#define UNIMPORTANT_TOK 258
#define WHITESPACE 259
#define SGML_CD 260
#define INCLUDES 261
#define DASHMATCH 262
#define BEGINSWITH 263
#define ENDSWITH 264
#define CONTAINS 265
#define STRING 266
#define IDENT 267
#define NTH 268
#define HEX 269
#define IDSEL 270
#define IMPORT_SYM 271
#define PAGE_SYM 272
#define MEDIA_SYM 273
#define FONT_FACE_SYM 274
#define CHARSET_SYM 275
#define NAMESPACE_SYM 276
#define WEBKIT_RULE_SYM 277
#define WEBKIT_DECLS_SYM 278
#define WEBKIT_VALUE_SYM 279
#define WEBKIT_MEDIAQUERY_SYM 280
#define WEBKIT_VARIABLES_SYM 281
#define WEBKIT_VARIABLES_DECLS_SYM 282
#define IMPORTANT_SYM 283
#define MEDIA_ONLY 284
#define MEDIA_NOT 285
#define MEDIA_AND 286
#define QEMS 287
#define EMS 288
#define EXS 289
#define PXS 290
#define CMS 291
#define MMS 292
#define INS 293
#define PTS 294
#define PCS 295
#define DEGS 296
#define RADS 297
#define GRADS 298
#define MSECS 299
#define SECS 300
#define HERZ 301
#define KHERZ 302
#define DIMEN 303
#define PERCENTAGE 304
#define FLOATTOKEN 305
#define INTEGER 306
#define URI 307
#define FUNCTION 308
#define NOTFUNCTION 309
#define UNICODERANGE 310
#define VARCALL 311




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
//#line 54 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
{
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
}
/* Line 1489 of yacc.c.  */
//#line 181 "CSSGrammar.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



#endif 
