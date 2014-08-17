/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/*
* This file was modified by Electronic Arts Inc Copyright © 2009-2010
*/

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse cssyyparse
#define yylex   cssyylex
#define yyerror cssyyerror
#define yylval  cssyylval
#define yychar  cssyychar
#define yydebug cssyydebug
#define yynerrs cssyynerrs


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




/* Copy the first part of user declarations.  */
//#line 1 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"


/*
 *  Copyright (C) 2002-2003 Lars Knoll (knoll@kde.org)
 *  Copyright (C) 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 *  Copyright (C) 2006 Alexey Proskuryakov (ap@nypop.com)
 *  Copyright (C) 2008 Eric Seidel <eric@webkit.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "config.h"

#include "CSSMediaRule.h"
#include "CSSParser.h"
#include "CSSRuleList.h"
#include "CSSSelector.h"
#include "CSSStyleSheet.h"
#include "Document.h"
#include "HTMLNames.h"
#include "MediaList.h"
#include <stdlib.h>
#include <string.h>

using namespace WebCore;
using namespace HTMLNames;

#define YYENABLE_NLS 0
#define YYLTYPE_IS_TRIVIAL 1
#define YYMAXDEPTH 10000
#define YYDEBUG 0

// FIXME: Replace with %parse-param { CSSParser* parser } once we can depend on bison 2.x
#define YYPARSE_PARAM parser
#define YYLEX_PARAM parser



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 187 of yacc.c.  */
//#line 287 "CSSGrammar.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */
//#line 74 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"


static inline int cssyyerror(const char*)
{
    return 1;
}

static int cssyylex(YYSTYPE* yylval, void* parser)
{
    return static_cast<CSSParser*>(parser)->lex(yylval);
}



/* Line 216 of yacc.c.  */
//#line 313 "CSSGrammar.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  22
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1075

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  78
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  74
/* YYNRULES -- Number of rules.  */
#define YYNRULES  224
/* YYNRULES -- Number of states.  */
#define YYNSTATES  428

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   311

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    75,     2,    76,     2,     2,
      65,    66,    19,    68,    67,    71,    17,    74,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    16,    64,
       2,    73,    70,     2,    77,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    18,     2,    72,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    62,    20,    63,    69,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,    10,    13,    16,    19,    22,    25,    27,
      29,    36,    42,    48,    54,    60,    61,    64,    65,    68,
      71,    72,    74,    80,    84,    88,    89,    93,    94,    98,
      99,   103,   104,   108,   110,   112,   114,   116,   118,   120,
     122,   124,   131,   135,   139,   147,   149,   152,   154,   158,
     160,   163,   167,   172,   176,   182,   187,   192,   199,   204,
     207,   213,   217,   222,   225,   232,   236,   240,   241,   244,
     246,   248,   251,   252,   257,   265,   267,   273,   274,   278,
     279,   281,   283,   285,   290,   291,   293,   295,   300,   303,
     311,   318,   319,   323,   326,   330,   334,   342,   346,   350,
     353,   356,   359,   361,   363,   369,   371,   376,   379,   382,
     384,   386,   389,   393,   396,   398,   401,   404,   406,   409,
     411,   414,   418,   421,   423,   425,   427,   430,   433,   435,
     437,   439,   441,   443,   446,   449,   454,   463,   469,   479,
     481,   483,   485,   487,   489,   491,   493,   495,   498,   502,
     507,   512,   517,   524,   526,   529,   531,   535,   537,   540,
     544,   549,   553,   559,   564,   569,   576,   582,   585,   592,
     595,   599,   604,   607,   610,   611,   613,   617,   620,   623,
     626,   627,   629,   632,   635,   638,   641,   645,   648,   651,
     653,   656,   658,   661,   664,   667,   670,   673,   676,   679,
     682,   685,   688,   691,   694,   697,   700,   703,   706,   709,
     712,   715,   718,   721,   727,   731,   734,   737,   741,   745,
     747,   750,   756,   760,   762
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      79,     0,    -1,    88,    87,    90,    91,    92,    93,    -1,
      81,    86,    -1,    82,    86,    -1,    84,    86,    -1,    85,
      86,    -1,    83,    86,    -1,    94,    -1,    96,    -1,    27,
      62,    86,    80,    86,    63,    -1,    28,    62,    86,   136,
      63,    -1,    32,    62,    86,    98,    63,    -1,    29,    62,
      86,   141,    63,    -1,    30,     4,    86,   111,    63,    -1,
      -1,    86,     4,    -1,    -1,    87,     5,    -1,    87,     4,
      -1,    -1,    89,    -1,    25,    86,    11,    86,    64,    -1,
      25,     1,   150,    -1,    25,     1,    64,    -1,    -1,    90,
      96,    87,    -1,    -1,    91,    97,    87,    -1,    -1,    92,
     102,    87,    -1,    -1,    93,    95,    87,    -1,   121,    -1,
     114,    -1,   117,    -1,   118,    -1,    94,    -1,   149,    -1,
     147,    -1,   148,    -1,    21,    86,   104,    86,   112,    64,
      -1,    21,     1,   150,    -1,    21,     1,    64,    -1,    31,
      86,   112,    62,    86,    98,    63,    -1,   100,    -1,    99,
     100,    -1,    99,    -1,     1,   151,     1,    -1,     1,    -1,
      99,     1,    -1,   100,    64,    86,    -1,   100,   151,    64,
      86,    -1,     1,    64,    86,    -1,     1,   151,     1,    64,
      86,    -1,    99,   100,    64,    86,    -1,    99,     1,    64,
      86,    -1,    99,     1,   151,     1,    64,    86,    -1,   101,
      16,    86,   141,    -1,   101,     1,    -1,   101,    16,    86,
       1,   141,    -1,   101,    16,    86,    -1,   101,    16,    86,
       1,    -1,    12,    86,    -1,    26,    86,   103,   104,    86,
      64,    -1,    26,     1,   150,    -1,    26,     1,    64,    -1,
      -1,    12,     4,    -1,    11,    -1,    57,    -1,    12,    86,
      -1,    -1,    16,    86,   141,    86,    -1,    65,    86,   105,
      86,   106,    66,    86,    -1,   107,    -1,   108,    86,    36,
      86,   107,    -1,    -1,    36,    86,   108,    -1,    -1,    34,
      -1,    35,    -1,   108,    -1,   110,    86,   116,   109,    -1,
      -1,   113,    -1,   111,    -1,   113,    67,    86,   111,    -1,
     113,     1,    -1,    23,    86,   113,    62,    86,   115,    63,
      -1,    23,    86,    62,    86,   115,    63,    -1,    -1,   115,
     121,    86,    -1,    12,    86,    -1,    22,     1,   150,    -1,
      22,     1,    64,    -1,    24,    86,    62,    86,   136,    63,
      86,    -1,    24,     1,   150,    -1,    24,     1,    64,    -1,
      68,    86,    -1,    69,    86,    -1,    70,    86,    -1,    71,
      -1,    68,    -1,   122,    62,    86,   136,    63,    -1,   124,
      -1,   122,    67,    86,   124,    -1,   122,     1,    -1,   124,
       4,    -1,   126,    -1,   123,    -1,   123,   126,    -1,   124,
     119,   126,    -1,   124,     1,    -1,    20,    -1,    19,    20,
      -1,    12,    20,    -1,   127,    -1,   127,   128,    -1,   128,
      -1,   125,   127,    -1,   125,   127,   128,    -1,   125,   128,
      -1,    12,    -1,    19,    -1,   129,    -1,   128,   129,    -1,
     128,     1,    -1,    15,    -1,    14,    -1,   130,    -1,   132,
      -1,   135,    -1,    17,    12,    -1,    12,    86,    -1,    18,
      86,   131,    72,    -1,    18,    86,   131,   133,    86,   134,
      86,    72,    -1,    18,    86,   125,   131,    72,    -1,    18,
      86,   125,   131,   133,    86,   134,    86,    72,    -1,    73,
      -1,     6,    -1,     7,    -1,     8,    -1,     9,    -1,    10,
      -1,    12,    -1,    11,    -1,    16,    12,    -1,    16,    16,
      12,    -1,    16,    58,    13,    66,    -1,    16,    58,    56,
      66,    -1,    16,    58,    12,    66,    -1,    16,    59,    86,
     126,    86,    66,    -1,   138,    -1,   137,   138,    -1,   137,
      -1,     1,   151,     1,    -1,     1,    -1,   137,     1,    -1,
     138,    64,    86,    -1,   138,   151,    64,    86,    -1,     1,
      64,    86,    -1,     1,   151,     1,    64,    86,    -1,   137,
     138,    64,    86,    -1,   137,     1,    64,    86,    -1,   137,
       1,   151,     1,    64,    86,    -1,   139,    16,    86,   141,
     140,    -1,   139,     1,    -1,   139,    16,    86,     1,   141,
     140,    -1,    33,    86,    -1,   139,    16,    86,    -1,   139,
      16,    86,     1,    -1,    12,    86,    -1,    33,    86,    -1,
      -1,   143,    -1,   141,   142,   143,    -1,   141,     1,    -1,
      74,    86,    -1,    67,    86,    -1,    -1,   144,    -1,   120,
     144,    -1,    11,    86,    -1,    12,    86,    -1,    53,    86,
      -1,   120,    53,    86,    -1,    57,    86,    -1,    60,    86,
      -1,   146,    -1,    75,    86,    -1,   145,    -1,    61,    86,
      -1,    76,    86,    -1,    56,    86,    -1,    55,    86,    -1,
      54,    86,    -1,    40,    86,    -1,    41,    86,    -1,    42,
      86,    -1,    43,    86,    -1,    44,    86,    -1,    45,    86,
      -1,    46,    86,    -1,    47,    86,    -1,    48,    86,    -1,
      49,    86,    -1,    50,    86,    -1,    51,    86,    -1,    52,
      86,    -1,    38,    86,    -1,    37,    86,    -1,    39,    86,
      -1,    58,    86,   141,    66,    86,    -1,    58,    86,     1,
      -1,    14,    86,    -1,    15,    86,    -1,    77,     1,   150,
      -1,    77,     1,    64,    -1,    96,    -1,     1,   150,    -1,
      62,     1,   151,     1,    63,    -1,    62,     1,    63,    -1,
     150,    -1,   151,     1,   150,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   235,   235,   236,   237,   238,   239,   240,   244,   245,
     249,   255,   261,   267,   281,   288,   289,   292,   294,   295,
     298,   300,   305,   311,   313,   317,   319,   326,   328,   335,
     337,   340,   342,   350,   351,   352,   353,   357,   358,   359,
     360,   364,   367,   370,   376,   382,   385,   390,   393,   396,
     399,   405,   408,   411,   414,   417,   422,   425,   431,   435,
     439,   443,   448,   455,   461,   466,   467,   471,   472,   476,
     477,   481,   487,   490,   496,   503,   508,   515,   518,   524,
     527,   530,   536,   541,   549,   552,   556,   561,   566,   572,
     575,   581,   582,   593,   610,   613,   619,   623,   626,   632,
     633,   634,   638,   639,   643,   649,   652,   660,   666,   672,
     675,   679,   695,   715,   721,   722,   723,   727,   732,   739,
     745,   755,   767,   780,   788,   796,   799,   812,   818,   827,
     840,   841,   842,   846,   858,   869,   874,   880,   888,   900,
     903,   906,   909,   912,   915,   921,   922,   926,   951,   966,
     984,  1002,  1021,  1036,  1039,  1044,  1047,  1050,  1053,  1059,
    1062,  1065,  1068,  1071,  1076,  1079,  1085,  1099,  1103,  1110,
    1115,  1120,  1127,  1133,  1134,  1138,  1143,  1157,  1163,  1166,
    1169,  1175,  1176,  1177,  1178,  1184,  1185,  1186,  1187,  1188,
    1189,  1191,  1194,  1199,  1203,  1204,  1205,  1206,  1207,  1208,
    1209,  1210,  1211,  1212,  1213,  1214,  1215,  1216,  1217,  1218,
    1219,  1220,  1221,  1226,  1235,  1251,  1252,  1259,  1262,  1268,
    1274,  1291,  1292,  1296,  1297
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "UNIMPORTANT_TOK", "WHITESPACE",
  "SGML_CD", "INCLUDES", "DASHMATCH", "BEGINSWITH", "ENDSWITH", "CONTAINS",
  "STRING", "IDENT", "NTH", "HEX", "IDSEL", "':'", "'.'", "'['", "'*'",
  "'|'", "IMPORT_SYM", "PAGE_SYM", "MEDIA_SYM", "FONT_FACE_SYM",
  "CHARSET_SYM", "NAMESPACE_SYM", "WEBKIT_RULE_SYM", "WEBKIT_DECLS_SYM",
  "WEBKIT_VALUE_SYM", "WEBKIT_MEDIAQUERY_SYM", "WEBKIT_VARIABLES_SYM",
  "WEBKIT_VARIABLES_DECLS_SYM", "IMPORTANT_SYM", "MEDIA_ONLY", "MEDIA_NOT",
  "MEDIA_AND", "QEMS", "EMS", "EXS", "PXS", "CMS", "MMS", "INS", "PTS",
  "PCS", "DEGS", "RADS", "GRADS", "MSECS", "SECS", "HERZ", "KHERZ",
  "DIMEN", "PERCENTAGE", "FLOATTOKEN", "INTEGER", "URI", "FUNCTION",
  "NOTFUNCTION", "UNICODERANGE", "VARCALL", "'{'", "'}'", "';'", "'('",
  "')'", "','", "'+'", "'~'", "'>'", "'-'", "']'", "'='", "'/'", "'#'",
  "'%'", "'@'", "$accept", "stylesheet", "valid_rule_or_import",
  "webkit_rule", "webkit_decls", "webkit_variables_decls", "webkit_value",
  "webkit_mediaquery", "maybe_space", "maybe_sgml", "maybe_charset",
  "charset", "import_list", "variables_list", "namespace_list",
  "rule_list", "valid_rule", "rule", "import", "variables_rule",
  "variables_declaration_list", "variables_decl_list",
  "variables_declaration", "variable_name", "namespace", "maybe_ns_prefix",
  "string_or_uri", "media_feature", "maybe_media_value", "media_query_exp",
  "media_query_exp_list", "maybe_and_media_query_exp_list",
  "maybe_media_restrictor", "media_query", "maybe_media_list",
  "media_list", "media", "ruleset_list", "medium", "page", "font_face",
  "combinator", "unary_operator", "ruleset", "selector_list",
  "selector_with_trailing_whitespace", "selector", "namespace_selector",
  "simple_selector", "element_name", "specifier_list", "specifier",
  "class", "attr_name", "attrib", "match", "ident_or_string", "pseudo",
  "declaration_list", "decl_list", "declaration", "property", "prio",
  "expr", "operator", "term", "unary_term", "function", "hexcolor",
  "invalid_at", "invalid_import", "invalid_rule", "invalid_block",
  "invalid_block_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,    58,    46,    91,    42,
     124,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   123,   125,    59,    40,    41,    44,    43,   126,
      62,    45,    93,    61,    47,    35,    37,    64
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    78,    79,    79,    79,    79,    79,    79,    80,    80,
      81,    82,    83,    84,    85,    86,    86,    87,    87,    87,
      88,    88,    89,    89,    89,    90,    90,    91,    91,    92,
      92,    93,    93,    94,    94,    94,    94,    95,    95,    95,
      95,    96,    96,    96,    97,    98,    98,    98,    98,    98,
      98,    99,    99,    99,    99,    99,    99,    99,   100,   100,
     100,   100,   100,   101,   102,   102,   102,   103,   103,   104,
     104,   105,   106,   106,   107,   108,   108,   109,   109,   110,
     110,   110,   111,   111,   112,   112,   113,   113,   113,   114,
     114,   115,   115,   116,   117,   117,   118,   118,   118,   119,
     119,   119,   120,   120,   121,   122,   122,   122,   123,   124,
     124,   124,   124,   124,   125,   125,   125,   126,   126,   126,
     126,   126,   126,   127,   127,   128,   128,   128,   129,   129,
     129,   129,   129,   130,   131,   132,   132,   132,   132,   133,
     133,   133,   133,   133,   133,   134,   134,   135,   135,   135,
     135,   135,   135,   136,   136,   136,   136,   136,   136,   137,
     137,   137,   137,   137,   137,   137,   138,   138,   138,   138,
     138,   138,   139,   140,   140,   141,   141,   141,   142,   142,
     142,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   145,   145,   146,   146,   147,   147,   148,
     149,   150,   150,   151,   151
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     6,     2,     2,     2,     2,     2,     1,     1,
       6,     5,     5,     5,     5,     0,     2,     0,     2,     2,
       0,     1,     5,     3,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     6,     3,     3,     7,     1,     2,     1,     3,     1,
       2,     3,     4,     3,     5,     4,     4,     6,     4,     2,
       5,     3,     4,     2,     6,     3,     3,     0,     2,     1,
       1,     2,     0,     4,     7,     1,     5,     0,     3,     0,
       1,     1,     1,     4,     0,     1,     1,     4,     2,     7,
       6,     0,     3,     2,     3,     3,     7,     3,     3,     2,
       2,     2,     1,     1,     5,     1,     4,     2,     2,     1,
       1,     2,     3,     2,     1,     2,     2,     1,     2,     1,
       2,     3,     2,     1,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     2,     2,     4,     8,     5,     9,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     3,     4,
       4,     4,     6,     1,     2,     1,     3,     1,     2,     3,
       4,     3,     5,     4,     4,     6,     5,     2,     6,     2,
       3,     4,     2,     2,     0,     1,     3,     2,     2,     2,
       0,     1,     2,     2,     2,     2,     3,     2,     2,     1,
       2,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     5,     3,     2,     2,     3,     3,     1,
       2,     5,     3,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      20,     0,     0,     0,     0,     0,     0,     0,    15,    15,
      15,    15,    15,    17,    21,     0,     0,    15,    15,    15,
      15,    15,     1,     3,     4,     7,     5,     6,    25,     0,
      24,    23,    16,    15,     0,     0,     0,    79,     0,    19,
      18,    27,     0,     0,   123,   129,   128,     0,     0,    15,
     124,   114,     0,     0,    15,     0,    15,     8,     9,    34,
      35,    36,    33,     0,   110,     0,     0,   109,   117,     0,
     125,   130,   131,   132,   157,    15,    15,     0,     0,   153,
       0,    15,    15,    15,    15,    15,    15,    15,    15,    15,
      15,    15,    15,    15,    15,    15,    15,    15,    15,    15,
      15,    15,    15,    15,    15,    15,    15,    15,    15,   103,
     102,    15,    15,     0,     0,   175,   181,   191,   189,    80,
      81,    15,    75,    82,    15,     0,    49,    15,     0,     0,
      45,     0,    29,    17,   222,   223,     0,    22,   116,   147,
       0,     0,    15,   133,     0,   115,     0,     0,     0,    79,
       0,     0,     0,   107,    15,    15,   111,   113,   108,    15,
      15,    15,     0,   123,   124,   120,     0,     0,   127,   126,
      15,     0,   172,   169,    11,   158,   154,    15,     0,   167,
      15,   183,   184,   215,   216,   211,   210,   212,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   185,   196,   195,   194,   187,     0,   188,   192,   190,
     193,    15,   182,   177,    13,    15,    15,     0,     0,     0,
       0,    14,    15,     0,    63,    12,    50,    46,    15,     0,
      59,    15,    15,    31,    17,    26,     0,   148,     0,     0,
       0,     0,    15,     0,     0,     0,    43,    42,    69,    70,
      15,    95,    94,    15,    86,     0,    98,    97,    15,    10,
       0,     0,    99,   100,   101,   112,     0,   161,   156,    15,
       0,    15,   159,     0,    15,     0,   214,     0,   186,   179,
     178,   176,    15,    15,    15,    15,    77,    53,    48,    15,
       0,    15,    51,    15,     0,    79,     0,     0,    17,    28,
     221,   224,   151,   149,   150,    15,   134,    15,     0,   140,
     141,   142,   143,   144,   135,   139,    15,    79,    91,    88,
      15,    15,     0,     0,     0,    15,   164,     0,   163,   160,
     171,     0,    15,    71,    72,     0,    93,    15,    83,    15,
      56,     0,    55,    52,    62,     0,     0,     0,     0,    67,
       0,     0,    37,    17,   219,    39,    40,    38,    30,     0,
     137,    15,     0,     0,     0,    91,    79,     0,   104,   162,
      15,     0,    15,   166,   213,    15,     0,    76,     0,    54,
      15,     0,    15,    66,    65,     0,     0,   220,     0,    32,
     152,     0,   146,   145,    15,    41,    90,    15,     0,    87,
      15,   165,   168,   173,     0,    15,    78,    57,     0,    68,
      15,   218,   217,    15,     0,    92,    89,    96,     0,    74,
       0,     0,     0,   136,    73,    44,    64,   138
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     7,    56,     8,     9,    10,    11,    12,   219,    28,
      13,    14,    41,   132,   233,   297,    57,   353,    58,   234,
     128,   129,   130,   131,   298,   386,   250,   283,   376,   122,
     123,   338,   124,   254,   346,   347,    59,   364,   286,    60,
      61,   162,   113,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,   245,    72,   316,   394,    73,    77,    78,
      79,    80,   373,   114,   217,   115,   116,   117,   118,   355,
     356,   357,   135,   136
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -341
static const yytype_int16 yypact[] =
{
     692,    34,   -26,   -23,    -2,   134,    85,   152,  -341,  -341,
    -341,  -341,  -341,  -341,  -341,   158,   179,  -341,  -341,  -341,
    -341,  -341,  -341,   157,   157,   157,   157,   157,   167,   193,
    -341,  -341,  -341,  -341,   645,    29,   928,   275,   161,  -341,
    -341,   185,   295,    52,   187,  -341,  -341,   253,   213,  -341,
     219,  -341,   345,   242,  -341,    63,  -341,  -341,  -341,  -341,
    -341,  -341,  -341,   113,   371,    62,   450,  -341,   516,   285,
    -341,  -341,  -341,  -341,   180,  -341,  -341,   183,    14,   192,
      36,  -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,
    -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,
    -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,
    -341,  -341,  -341,  1019,   757,  -341,  -341,  -341,  -341,  -341,
    -341,  -341,  -341,    25,  -341,   190,   199,  -341,   204,    45,
     200,    43,   240,  -341,  -341,  -341,   279,  -341,  -341,  -341,
     273,    15,  -341,  -341,    30,  -341,   212,   215,   229,   243,
     259,   130,   111,  -341,  -341,  -341,  -341,  -341,  -341,  -341,
    -341,  -341,   371,  -341,  -341,   516,   459,   584,  -341,  -341,
    -341,   293,   157,   157,  -341,   277,   234,  -341,     5,  -341,
    -341,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   873,   157,   157,   157,
     157,  -341,  -341,  -341,  -341,  -341,  -341,   979,   145,   132,
     233,  -341,  -341,   312,   157,  -341,   280,   252,  -341,    12,
    -341,  -341,  -341,   281,  -341,   167,   310,  -341,   263,   268,
     284,   773,   187,   219,   331,   136,  -341,  -341,  -341,  -341,
    -341,  -341,  -341,  -341,  -341,   162,  -341,  -341,  -341,  -341,
      29,   773,   157,   157,   157,  -341,   714,   157,   306,  -341,
     375,  -341,   157,   315,  -341,   567,  -341,   815,   157,   157,
     157,  -341,  -341,  -341,  -341,  -341,   356,   157,   318,  -341,
     399,  -341,   157,  -341,   633,   283,   321,   181,  -341,   167,
    -341,  -341,  -341,  -341,  -341,  -341,   157,  -341,   178,  -341,
    -341,  -341,  -341,  -341,  -341,  -341,  -341,   262,   157,  -341,
    -341,  -341,    29,   351,   166,  -341,   157,   339,   157,   157,
     979,   383,  -341,   157,    27,   147,   157,  -341,  -341,  -341,
     157,   343,   157,   157,   979,   441,   353,   149,   344,   236,
     315,   417,  -341,  -341,  -341,  -341,  -341,  -341,   167,    74,
    -341,  -341,   144,   355,   347,   157,   275,   385,  -341,   157,
    -341,   383,  -341,  -341,   157,  -341,   395,  -341,   147,   157,
    -341,   499,  -341,  -341,  -341,   445,    11,  -341,   349,   167,
    -341,   144,  -341,  -341,  -341,  -341,  -341,  -341,   577,  -341,
    -341,   157,  -341,   157,   928,  -341,    25,   157,   161,  -341,
    -341,  -341,  -341,  -341,     0,   157,  -341,   157,   699,   157,
     407,    75,     1,  -341,   157,  -341,  -341,  -341
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -341,  -341,  -341,  -341,  -341,  -341,  -341,  -341,    -1,  -121,
    -341,  -341,  -341,  -341,  -341,  -341,   174,  -341,   -38,  -341,
      64,  -341,   377,  -341,  -341,  -341,   121,  -341,  -341,   184,
     140,  -341,  -341,   -36,   203,   373,  -341,   159,  -341,  -341,
    -341,  -341,  -341,  -340,  -341,  -341,   274,   379,   -50,   492,
     -43,   -48,  -341,   320,  -341,   257,   186,  -341,  -133,  -341,
     491,  -341,   201,  -166,  -341,   359,   467,  -341,  -341,  -341,
    -341,  -341,   -13,    -9
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -181
static const yytype_int16 yytable[] =
{
      16,   125,    31,   133,    32,    32,   273,    23,    24,    25,
      26,    27,   235,   273,   156,   175,    34,    35,    36,    37,
      38,   169,   248,   166,   397,   167,    75,   238,   239,   -15,
      74,    32,    43,    32,    32,    15,    17,   179,   -15,    18,
     277,    75,   242,   375,   230,   -15,   226,    76,   144,   243,
      51,   147,   180,   149,   151,   152,    32,   127,   397,   231,
      19,   -15,    76,   157,   150,   171,   158,   -15,   249,   274,
     178,   240,   423,   427,   172,   173,   293,  -155,    32,    32,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   -47,   331,
     209,   210,   265,   299,   153,    32,   137,   223,   169,   169,
     218,   229,   266,   220,  -105,   -15,   224,   323,   345,  -105,
     159,   160,   161,   247,    32,   252,    32,   257,    20,   426,
     390,   241,   309,   310,   311,   312,   313,    21,    32,    32,
     319,    32,    22,   260,   261,   392,   393,   282,   262,   263,
     264,    32,   126,   319,   371,    32,   270,   157,   284,   267,
     158,    39,    40,   127,   259,   154,   272,   358,   381,   275,
     155,    -2,   350,    32,   309,   310,   311,   312,   313,   367,
      33,   305,   258,    44,    42,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    52,   138,   314,   315,
     278,   -85,   121,   -85,   279,   280,   321,   290,   169,    32,
      29,   287,    30,   301,   320,   143,   248,   292,  -106,   321,
     294,   295,   389,  -106,   159,   160,   161,    32,   418,   145,
      32,   306,    29,   148,   170,   285,   174,    32,   385,   317,
     360,   315,   318,   221,    29,   301,   177,   322,   351,   354,
     301,    29,    29,   222,   228,   139,    32,   225,   326,   140,
     328,   232,   249,   329,    29,   301,   246,   119,   120,    32,
     236,   333,   334,   335,   336,   237,   168,    32,   340,  -119,
     342,    29,   343,   251,   268,   349,   119,   120,   271,    45,
      46,    47,    48,    49,   359,   253,   306,   296,   121,   119,
     120,   141,   142,   288,   301,   362,   291,   119,   120,   365,
     366,    29,   348,   256,   369,   -15,   -84,   121,   301,   302,
     399,   374,   -15,   -15,   303,   384,   378,   387,   379,    29,
     121,   269,    29,   307,   289,   -84,   146,  -119,   121,   -15,
     304,  -119,  -119,  -119,  -119,  -119,   -15,    29,   134,    44,
     391,    45,    46,    47,    48,    49,    50,    51,    29,   401,
     325,   403,    29,   300,   404,   412,   327,    29,   -15,   407,
      29,   408,   339,    44,   213,    45,    46,    47,    48,    49,
      50,    51,   337,   414,  -180,  -180,   415,  -180,  -180,   417,
     341,    29,   -15,   370,   419,    29,    29,   380,   383,   421,
     396,    29,   422,   411,   368,   382,   372,   424,   388,   395,
    -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,
    -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,
    -180,  -180,   213,  -180,  -180,  -174,  -174,  -174,   400,   409,
     215,  -180,  -180,  -180,  -180,  -180,  -180,   216,  -180,  -180,
     168,   405,   163,  -122,    45,    46,    47,    48,    49,   164,
     425,   352,   420,    45,    46,    47,    48,    49,  -180,  -180,
    -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,
    -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,
     213,  -180,  -180,   -58,   -58,   -58,   227,   410,   215,  -180,
    -180,  -180,  -180,  -180,  -180,   216,  -180,  -180,   406,   377,
     363,  -122,   255,   244,   398,  -122,  -122,  -122,  -122,  -122,
      45,    46,    47,    48,    49,   324,  -180,  -180,  -180,  -180,
    -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,
    -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,   165,  -180,
    -180,   -60,   -60,   -60,   308,   361,   215,  -180,   330,   176,
    -180,    32,   402,   216,  -180,  -180,   281,   413,    81,    82,
     212,    83,    84,     0,     0,   168,     0,     0,  -118,    44,
       0,    45,    46,    47,    48,    49,    50,    51,    45,    46,
      47,    48,    49,     0,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,     0,   107,   108,  -170,
    -170,  -170,     0,     0,   344,   109,     0,    32,   110,     0,
     416,     0,   111,   112,    81,    82,  -118,    83,    84,    32,
    -118,  -118,  -118,  -118,  -118,     0,     0,    44,     0,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,     0,   107,   108,   -61,   -61,   -61,     0,     0,
     213,   109,     0,   -15,   110,     0,     0,     0,   111,   112,
    -180,  -180,     0,  -180,  -180,   168,     0,     1,  -121,     2,
       3,     4,     5,     0,     6,     0,     0,     0,    45,    46,
      47,    48,    49,     0,     0,     0,  -180,  -180,  -180,  -180,
    -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,
    -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,   213,  -180,
    -180,     0,     0,     0,     0,   -15,   215,  -180,  -180,  -180,
    -180,  -180,  -180,   216,  -180,  -180,  -121,    32,     0,     0,
    -121,  -121,  -121,  -121,  -121,    44,     0,    45,    46,    47,
      48,    49,    50,    51,  -180,  -180,  -180,  -180,  -180,  -180,
    -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,
    -180,  -180,  -180,  -180,  -180,  -180,   213,  -180,  -180,     0,
     214,     0,     0,     0,   215,  -180,  -180,  -180,  -180,  -180,
    -180,   216,  -180,  -180,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,
    -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,
    -180,  -180,  -180,  -180,   276,  -180,  -180,    32,     0,     0,
       0,   332,   215,  -180,    81,    82,  -180,    83,    84,   216,
    -180,  -180,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,    32,   107,   108,     0,     0,     0,     0,    81,
      82,   109,    83,    84,   110,     0,     0,     0,   111,   112,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,     0,   107,   108,
      81,    82,     0,    83,    84,     0,   109,     0,     0,   110,
       0,     0,     0,   111,   112,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,     0,   107,
     108,     0,     0,     0,     0,     0,     0,   109,     0,     0,
     110,     0,     0,     0,   111,   112,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   211,   102,   103,   104
};

static const yytype_int16 yycheck[] =
{
       1,    37,    15,    41,     4,     4,     1,     8,     9,    10,
      11,    12,   133,     1,    64,     1,    17,    18,    19,    20,
      21,    69,    11,    66,   364,    68,    12,    12,    13,     4,
       1,     4,    33,     4,     4,     1,    62,     1,     4,    62,
     206,    12,    12,    16,     1,    11,     1,    33,    49,    19,
      20,    52,    16,    54,    55,    56,     4,    12,   398,    16,
      62,    36,    33,     1,     1,    74,     4,     4,    57,    64,
      79,    56,    72,    72,    75,    76,    64,    63,     4,     4,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,    63,   275,
     111,   112,   162,   234,     1,     4,    64,   126,   166,   167,
     121,   130,   165,   124,    62,    62,   127,   260,   294,    67,
      68,    69,    70,   146,     4,   148,     4,   150,     4,    64,
      66,   142,     6,     7,     8,     9,    10,    62,     4,     4,
       1,     4,     0,   154,   155,    11,    12,    12,   159,   160,
     161,     4,     1,     1,   330,     4,   175,     1,    36,   170,
       4,     4,     5,    12,    63,    62,   177,   298,   344,   180,
      67,     0,     1,     4,     6,     7,     8,     9,    10,   322,
      11,   241,    62,    12,     1,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    21,    20,    72,    73,
     211,    62,    65,    64,   215,   216,    67,   226,   266,     4,
      62,   222,    64,   236,    62,    12,    11,   228,    62,    67,
     231,   232,   353,    67,    68,    69,    70,     4,   404,    20,
       4,   242,    62,     1,    64,    12,    63,     4,    12,   250,
      72,    73,   253,    63,    62,   268,    64,   258,    77,   297,
     273,    62,    62,    64,    64,    12,     4,    63,   269,    16,
     271,    31,    57,   274,    62,   288,    64,    34,    35,     4,
       1,   282,   283,   284,   285,    12,     1,     4,   289,     4,
     291,    62,   293,    64,     1,   296,    34,    35,    64,    14,
      15,    16,    17,    18,   305,    62,   307,    26,    65,    34,
      35,    58,    59,     1,   327,   316,    64,    34,    35,   320,
     321,    62,     1,    64,   325,     4,    64,    65,   341,    66,
     366,   332,    11,    12,    66,   348,   337,   350,   339,    62,
      65,    64,    62,    12,    64,    62,     1,    62,    65,     4,
      66,    66,    67,    68,    69,    70,    11,    62,    63,    12,
     361,    14,    15,    16,    17,    18,    19,    20,    62,   370,
      64,   372,    62,    63,   375,   388,     1,    62,    57,   380,
      62,   382,    64,    12,     1,    14,    15,    16,    17,    18,
      19,    20,    36,   394,    11,    12,   397,    14,    15,   400,
       1,    62,    57,    64,   405,    62,    62,    64,    64,   410,
      63,    62,   413,    64,    63,    62,    33,   418,     1,    64,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,     1,    60,    61,    62,    63,    64,    63,     4,
      67,    68,    11,    12,    71,    14,    15,    74,    75,    76,
       1,    66,    12,     4,    14,    15,    16,    17,    18,    19,
      63,   297,   408,    14,    15,    16,    17,    18,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
       1,    60,    61,    62,    63,    64,   129,   386,    67,    68,
      11,    12,    71,    14,    15,    74,    75,    76,   378,   335,
     317,    62,   149,   144,   365,    66,    67,    68,    69,    70,
      14,    15,    16,    17,    18,   261,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    66,    60,
      61,    62,    63,    64,   244,   308,    67,    68,     1,    78,
      71,     4,   371,    74,    75,    76,   217,   391,    11,    12,
     113,    14,    15,    -1,    -1,     1,    -1,    -1,     4,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    14,    15,
      16,    17,    18,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    -1,    60,    61,    62,
      63,    64,    -1,    -1,     1,    68,    -1,     4,    71,    -1,
      63,    -1,    75,    76,    11,    12,    62,    14,    15,     4,
      66,    67,    68,    69,    70,    -1,    -1,    12,    -1,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    -1,    60,    61,    62,    63,    64,    -1,    -1,
       1,    68,    -1,     4,    71,    -1,    -1,    -1,    75,    76,
      11,    12,    -1,    14,    15,     1,    -1,    25,     4,    27,
      28,    29,    30,    -1,    32,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,     1,    60,
      61,    -1,    -1,    -1,    -1,    66,    67,    68,    11,    12,
      71,    14,    15,    74,    75,    76,    62,     4,    -1,    -1,
      66,    67,    68,    69,    70,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,     1,    60,    61,    -1,
      63,    -1,    -1,    -1,    67,    68,    11,    12,    71,    14,
      15,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,     1,    60,    61,     4,    -1,    -1,
      -1,    66,    67,    68,    11,    12,    71,    14,    15,    74,
      75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,     4,    60,    61,    -1,    -1,    -1,    -1,    11,
      12,    68,    14,    15,    71,    -1,    -1,    -1,    75,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    -1,    60,    61,
      11,    12,    -1,    14,    15,    -1,    68,    -1,    -1,    71,
      -1,    -1,    -1,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    -1,    60,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,
      71,    -1,    -1,    -1,    75,    76,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    25,    27,    28,    29,    30,    32,    79,    81,    82,
      83,    84,    85,    88,    89,     1,    86,    62,    62,    62,
       4,    62,     0,    86,    86,    86,    86,    86,    87,    62,
      64,   150,     4,    11,    86,    86,    86,    86,    86,     4,
       5,    90,     1,    86,    12,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    80,    94,    96,   114,
     117,   118,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   132,   135,     1,    12,    33,   136,   137,   138,
     139,    11,    12,    14,    15,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    60,    61,    68,
      71,    75,    76,   120,   141,   143,   144,   145,   146,    34,
      35,    65,   107,   108,   110,   111,     1,    12,    98,    99,
     100,   101,    91,    96,    63,   150,   151,    64,    20,    12,
      16,    58,    59,    12,    86,    20,     1,    86,     1,    86,
       1,    86,    86,     1,    62,    67,   126,     1,     4,    68,
      69,    70,   119,    12,    19,   127,   128,   128,     1,   129,
      64,   151,    86,    86,    63,     1,   138,    64,   151,     1,
      16,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    53,   144,     1,    63,    67,    74,   142,    86,    86,
      86,    63,    64,   151,    86,    63,     1,   100,    64,   151,
       1,    16,    31,    92,    97,    87,     1,    12,    12,    13,
      56,    86,    12,    19,   125,   131,    64,   150,    11,    57,
     104,    64,   150,    62,   111,   113,    64,   150,    62,    63,
      86,    86,    86,    86,    86,   126,   128,    86,     1,    64,
     151,    64,    86,     1,    64,    86,     1,   141,    86,    86,
      86,   143,    12,   105,    36,    12,   116,    86,     1,    64,
     151,    64,    86,    64,    86,    86,    26,    93,   102,    87,
      63,   150,    66,    66,    66,   126,    86,    12,   131,     6,
       7,     8,     9,    10,    72,    73,   133,    86,    86,     1,
      62,    67,    86,   136,   124,    64,    86,     1,    86,    86,
       1,   141,    66,    86,    86,    86,    86,    36,   109,    64,
      86,     1,    86,    86,     1,   141,   112,   113,     1,    86,
       1,    77,    94,    95,    96,   147,   148,   149,    87,    86,
      72,   133,    86,   112,   115,    86,    86,   136,    63,    86,
      64,   141,    33,   140,    86,    16,   106,   107,    86,    86,
      64,   141,    62,    64,   150,    12,   103,   150,     1,    87,
      66,    86,    11,    12,   134,    64,    63,   121,   115,   111,
      63,    86,   140,    86,    86,    66,   108,    86,    86,     4,
     104,    64,   150,   134,    86,    86,    63,    86,   141,    86,
      98,    86,    86,    72,    86,    63,    64,    72
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 10:
//#line 249 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        static_cast<CSSParser*>(parser)->m_rule = (yyvsp[(4) - (6)].rule);
    ;}
    break;

  case 11:
//#line 255 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        /* can be empty */
    ;}
    break;

  case 12:
//#line 261 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        /* can be empty */
    ;}
    break;

  case 13:
//#line 267 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        if ((yyvsp[(4) - (5)].valueList)) {
            p->m_valueList = p->sinkFloatingValueList((yyvsp[(4) - (5)].valueList));
            int oldParsedProperties = p->m_numParsedProperties;
            if (!p->parseValue(p->m_id, p->m_important))
                p->rollbackLastProperties(p->m_numParsedProperties - oldParsedProperties);
            delete p->m_valueList;
            p->m_valueList = 0;
        }
    ;}
    break;

  case 14:
//#line 281 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
         CSSParser* p = static_cast<CSSParser*>(parser);
         p->m_mediaQuery = p->sinkFloatingMediaQuery((yyvsp[(4) - (5)].mediaQuery));
     ;}
    break;

  case 21:
//#line 300 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
  ;}
    break;

  case 22:
//#line 305 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
     CSSParser* p = static_cast<CSSParser*>(parser);
     (yyval.rule) = static_cast<CSSParser*>(parser)->createCharsetRule((yyvsp[(3) - (5)].string));
     if ((yyval.rule) && p->m_styleSheet)
         p->m_styleSheet->append((yyval.rule));
  ;}
    break;

  case 23:
//#line 311 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
  ;}
    break;

  case 24:
//#line 313 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
  ;}
    break;

  case 26:
//#line 319 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
     CSSParser* p = static_cast<CSSParser*>(parser);
     if ((yyvsp[(2) - (3)].rule) && p->m_styleSheet)
         p->m_styleSheet->append((yyvsp[(2) - (3)].rule));
 ;}
    break;

  case 28:
//#line 328 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
    CSSParser* p = static_cast<CSSParser*>(parser);
     if ((yyvsp[(2) - (3)].rule) && p->m_styleSheet)
         p->m_styleSheet->append((yyvsp[(2) - (3)].rule));
;}
    break;

  case 32:
//#line 342 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
     CSSParser* p = static_cast<CSSParser*>(parser);
     if ((yyvsp[(2) - (3)].rule) && p->m_styleSheet)
         p->m_styleSheet->append((yyvsp[(2) - (3)].rule));
 ;}
    break;

  case 41:
//#line 364 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createImportRule((yyvsp[(3) - (6)].string), (yyvsp[(5) - (6)].mediaList));
    ;}
    break;

  case 42:
//#line 367 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 43:
//#line 370 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 44:
//#line 376 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createVariablesRule((yyvsp[(3) - (7)].mediaList));
    ;}
    break;

  case 45:
//#line 382 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (1)].boolean);
    ;}
    break;

  case 46:
//#line 385 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (2)].boolean);
        if ((yyvsp[(2) - (2)].boolean))
            (yyval.boolean) = (yyvsp[(2) - (2)].boolean);
    ;}
    break;

  case 47:
//#line 390 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (1)].boolean);
    ;}
    break;

  case 48:
//#line 393 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 49:
//#line 396 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 50:
//#line 399 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (2)].boolean);
    ;}
    break;

  case 51:
//#line 405 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (3)].boolean);
    ;}
    break;

  case 52:
//#line 408 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 53:
//#line 411 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 54:
//#line 414 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 55:
//#line 417 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (4)].boolean);
        if ((yyvsp[(2) - (4)].boolean))
            (yyval.boolean) = (yyvsp[(2) - (4)].boolean);
    ;}
    break;

  case 56:
//#line 422 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (4)].boolean);
    ;}
    break;

  case 57:
//#line 425 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (6)].boolean);
    ;}
    break;

  case 58:
//#line 431 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = static_cast<CSSParser*>(parser)->addVariable((yyvsp[(1) - (4)].string), (yyvsp[(4) - (4)].valueList));
    ;}
    break;

  case 59:
//#line 435 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 60:
//#line 439 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 61:
//#line 443 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        /* @variables { varname: } Just reduce away this variable with no value. */
        (yyval.boolean) = false;
    ;}
    break;

  case 62:
//#line 448 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        /* if we come across rules with invalid values like this case: @variables { varname: *; }, just discard the property/value pair */
        (yyval.boolean) = false;
    ;}
    break;

  case 63:
//#line 455 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.string) = (yyvsp[(1) - (2)].string);
    ;}
    break;

  case 64:
//#line 461 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
    CSSParser* p = static_cast<CSSParser*>(parser);
    if (p->m_styleSheet)
        p->m_styleSheet->addNamespace(p, (yyvsp[(3) - (6)].string), (yyvsp[(4) - (6)].string));
;}
    break;

  case 67:
//#line 471 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.string).characters = 0; ;}
    break;

  case 68:
//#line 472 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.string) = (yyvsp[(1) - (2)].string); ;}
    break;

  case 71:
//#line 481 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.string) = (yyvsp[(1) - (2)].string);
    ;}
    break;

  case 72:
//#line 487 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.valueList) = 0;
    ;}
    break;

  case 73:
//#line 490 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.valueList) = (yyvsp[(3) - (4)].valueList);
    ;}
    break;

  case 74:
//#line 496 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyvsp[(3) - (7)].string).lower();
        (yyval.mediaQueryExp) = static_cast<CSSParser*>(parser)->createFloatingMediaQueryExp((yyvsp[(3) - (7)].string), (yyvsp[(5) - (7)].valueList));
    ;}
    break;

  case 75:
//#line 503 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.mediaQueryExpList) = p->createFloatingMediaQueryExpList();
        (yyval.mediaQueryExpList)->append(p->sinkFloatingMediaQueryExp((yyvsp[(1) - (1)].mediaQueryExp)));
    ;}
    break;

  case 76:
//#line 508 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryExpList) = (yyvsp[(1) - (5)].mediaQueryExpList);
        (yyval.mediaQueryExpList)->append(static_cast<CSSParser*>(parser)->sinkFloatingMediaQueryExp((yyvsp[(5) - (5)].mediaQueryExp)));
    ;}
    break;

  case 77:
//#line 515 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryExpList) = static_cast<CSSParser*>(parser)->createFloatingMediaQueryExpList();
    ;}
    break;

  case 78:
//#line 518 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryExpList) = (yyvsp[(3) - (3)].mediaQueryExpList);
    ;}
    break;

  case 79:
//#line 524 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryRestrictor) = MediaQuery::None;
    ;}
    break;

  case 80:
//#line 527 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryRestrictor) = MediaQuery::Only;
    ;}
    break;

  case 81:
//#line 530 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryRestrictor) = MediaQuery::Not;
    ;}
    break;

  case 82:
//#line 536 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.mediaQuery) = p->createFloatingMediaQuery(p->sinkFloatingMediaQueryExpList((yyvsp[(1) - (1)].mediaQueryExpList)));
    ;}
    break;

  case 83:
//#line 541 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyvsp[(3) - (4)].string).lower();
        (yyval.mediaQuery) = p->createFloatingMediaQuery((yyvsp[(1) - (4)].mediaQueryRestrictor), (yyvsp[(3) - (4)].string), p->sinkFloatingMediaQueryExpList((yyvsp[(4) - (4)].mediaQueryExpList)));
    ;}
    break;

  case 84:
//#line 549 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaList) = static_cast<CSSParser*>(parser)->createMediaList();
     ;}
    break;

  case 86:
//#line 556 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.mediaList) = p->createMediaList();
        (yyval.mediaList)->appendMediaQuery(p->sinkFloatingMediaQuery((yyvsp[(1) - (1)].mediaQuery)));
    ;}
    break;

  case 87:
//#line 561 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaList) = (yyvsp[(1) - (4)].mediaList);
        if ((yyval.mediaList))
            (yyval.mediaList)->appendMediaQuery(static_cast<CSSParser*>(parser)->sinkFloatingMediaQuery((yyvsp[(4) - (4)].mediaQuery)));
    ;}
    break;

  case 88:
//#line 566 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaList) = 0;
    ;}
    break;

  case 89:
//#line 572 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createMediaRule((yyvsp[(3) - (7)].mediaList), (yyvsp[(6) - (7)].ruleList));
    ;}
    break;

  case 90:
//#line 575 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createMediaRule(0, (yyvsp[(5) - (6)].ruleList));
    ;}
    break;

  case 91:
//#line 581 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.ruleList) = 0; ;}
    break;

  case 92:
//#line 582 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.ruleList) = (yyvsp[(1) - (3)].ruleList);
        if ((yyvsp[(2) - (3)].rule)) {
            if (!(yyval.ruleList))
                (yyval.ruleList) = static_cast<CSSParser*>(parser)->createRuleList();
            (yyval.ruleList)->append((yyvsp[(2) - (3)].rule));
        }
    ;}
    break;

  case 93:
//#line 593 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
      (yyval.string) = (yyvsp[(1) - (2)].string);
  ;}
    break;

  case 94:
//#line 610 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
      (yyval.rule) = 0;
    ;}
    break;

  case 95:
//#line 613 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
      (yyval.rule) = 0;
    ;}
    break;

  case 96:
//#line 620 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createFontFaceRule();
    ;}
    break;

  case 97:
//#line 623 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
      (yyval.rule) = 0;
    ;}
    break;

  case 98:
//#line 626 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
      (yyval.rule) = 0;
    ;}
    break;

  case 99:
//#line 632 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.relation) = CSSSelector::DirectAdjacent; ;}
    break;

  case 100:
//#line 633 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.relation) = CSSSelector::IndirectAdjacent; ;}
    break;

  case 101:
//#line 634 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.relation) = CSSSelector::Child; ;}
    break;

  case 102:
//#line 638 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.integer) = -1; ;}
    break;

  case 103:
//#line 639 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.integer) = 1; ;}
    break;

  case 104:
//#line 643 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createStyleRule((yyvsp[(1) - (5)].selector));
    ;}
    break;

  case 105:
//#line 649 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (1)].selector);
    ;}
    break;

  case 106:
//#line 652 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        if ((yyvsp[(1) - (4)].selector) && (yyvsp[(4) - (4)].selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selector) = (yyvsp[(1) - (4)].selector);
            (yyval.selector)->append(p->sinkFloatingSelector((yyvsp[(4) - (4)].selector)));
        } else
            (yyval.selector) = 0;
    ;}
    break;

  case 107:
//#line 660 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = 0;
    ;}
    break;

  case 108:
//#line 666 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (2)].selector);
    ;}
    break;

  case 109:
//#line 672 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (1)].selector);
    ;}
    break;

  case 110:
//#line 676 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (1)].selector);
    ;}
    break;

  case 111:
//#line 680 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(2) - (2)].selector);
        if (!(yyvsp[(1) - (2)].selector))
            (yyval.selector) = 0;
        else if ((yyval.selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            CSSSelector* end = (yyval.selector);
            while (end->m_tagHistory)
                end = end->m_tagHistory;
            end->m_relation = CSSSelector::Descendant;
            end->m_tagHistory = p->sinkFloatingSelector((yyvsp[(1) - (2)].selector));
            if (Document* doc = p->document())
                doc->setUsesDescendantRules(true);
        }
    ;}
    break;

  case 112:
//#line 695 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(3) - (3)].selector);
        if (!(yyvsp[(1) - (3)].selector))
            (yyval.selector) = 0;
        else if ((yyval.selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            CSSSelector* end = (yyval.selector);
            while (end->m_tagHistory)
                end = end->m_tagHistory;
            end->m_relation = (yyvsp[(2) - (3)].relation);
            end->m_tagHistory = p->sinkFloatingSelector((yyvsp[(1) - (3)].selector));
            if ((yyvsp[(2) - (3)].relation) == CSSSelector::Child) {
                if (Document* doc = p->document())
                    doc->setUsesDescendantRules(true);
            } else if ((yyvsp[(2) - (3)].relation) == CSSSelector::DirectAdjacent || (yyvsp[(2) - (3)].relation) == CSSSelector::IndirectAdjacent) {
                if (Document* doc = p->document())
                    doc->setUsesSiblingRules(true);
            }
        }
    ;}
    break;

  case 113:
//#line 715 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = 0;
    ;}
    break;

  case 114:
//#line 721 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.string).characters = 0; (yyval.string).length = 0; ;}
    break;

  case 115:
//#line 722 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { static UChar star = '*'; (yyval.string).characters = &star; (yyval.string).length = 1; ;}
    break;

  case 116:
//#line 723 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.string) = (yyvsp[(1) - (2)].string); ;}
    break;

  case 117:
//#line 727 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_tag = QualifiedName(nullAtom, (yyvsp[(1) - (1)].string), p->m_defaultNamespace);
    ;}
    break;

  case 118:
//#line 732 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(2) - (2)].selector);
        if ((yyval.selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selector)->m_tag = QualifiedName(nullAtom, (yyvsp[(1) - (2)].string), p->m_defaultNamespace);
        }
    ;}
    break;

  case 119:
//#line 739 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (1)].selector);
        CSSParser* p = static_cast<CSSParser*>(parser);
        if ((yyval.selector) && p->m_defaultNamespace != starAtom)
            (yyval.selector)->m_tag = QualifiedName(nullAtom, starAtom, p->m_defaultNamespace);
    ;}
    break;

  case 120:
//#line 745 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        AtomicString namespacePrefix = (yyvsp[(1) - (2)].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        if (p->m_styleSheet)
            (yyval.selector)->m_tag = QualifiedName(namespacePrefix, (yyvsp[(2) - (2)].string),
                                      p->m_styleSheet->determineNamespace(namespacePrefix));
        else // FIXME: Shouldn't this case be an error?
            (yyval.selector)->m_tag = QualifiedName(nullAtom, (yyvsp[(2) - (2)].string), p->m_defaultNamespace);
    ;}
    break;

  case 121:
//#line 755 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(3) - (3)].selector);
        if ((yyval.selector)) {
            AtomicString namespacePrefix = (yyvsp[(1) - (3)].string);
            CSSParser* p = static_cast<CSSParser*>(parser);
            if (p->m_styleSheet)
                (yyval.selector)->m_tag = QualifiedName(namespacePrefix, (yyvsp[(2) - (3)].string),
                                          p->m_styleSheet->determineNamespace(namespacePrefix));
            else // FIXME: Shouldn't this case be an error?
                (yyval.selector)->m_tag = QualifiedName(nullAtom, (yyvsp[(2) - (3)].string), p->m_defaultNamespace);
        }
    ;}
    break;

  case 122:
//#line 767 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(2) - (2)].selector);
        if ((yyval.selector)) {
            AtomicString namespacePrefix = (yyvsp[(1) - (2)].string);
            CSSParser* p = static_cast<CSSParser*>(parser);
            if (p->m_styleSheet)
                (yyval.selector)->m_tag = QualifiedName(namespacePrefix, starAtom,
                                          p->m_styleSheet->determineNamespace(namespacePrefix));
        }
    ;}
    break;

  case 123:
//#line 780 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        CSSParserString& str = (yyvsp[(1) - (1)].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        Document* doc = p->document();
        if (doc && doc->isHTMLDocument())
            str.lower();
        (yyval.string) = str;
    ;}
    break;

  case 124:
//#line 788 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        static UChar star = '*';
        (yyval.string).characters = &star;
        (yyval.string).length = 1;
    ;}
    break;

  case 125:
//#line 796 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (1)].selector);
    ;}
    break;

  case 126:
//#line 799 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        if (!(yyvsp[(2) - (2)].selector))
            (yyval.selector) = 0;
        else if ((yyvsp[(1) - (2)].selector)) {
            (yyval.selector) = (yyvsp[(1) - (2)].selector);
            CSSParser* p = static_cast<CSSParser*>(parser);
            CSSSelector* end = (yyvsp[(1) - (2)].selector);
            while (end->m_tagHistory)
                end = end->m_tagHistory;
            end->m_relation = CSSSelector::SubSelector;
            end->m_tagHistory = p->sinkFloatingSelector((yyvsp[(2) - (2)].selector));
        }
    ;}
    break;

  case 127:
//#line 812 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = 0;
    ;}
    break;

  case 128:
//#line 818 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::Id;
        if (!p->m_strict)
            (yyvsp[(1) - (1)].string).lower();
        (yyval.selector)->m_attr = idAttr;
        (yyval.selector)->m_value = (yyvsp[(1) - (1)].string);
    ;}
    break;

  case 129:
//#line 827 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        if ((yyvsp[(1) - (1)].string).characters[0] >= '0' && (yyvsp[(1) - (1)].string).characters[0] <= '9') {
            (yyval.selector) = 0;
        } else {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selector) = p->createFloatingSelector();
            (yyval.selector)->m_match = CSSSelector::Id;
            if (!p->m_strict)
                (yyvsp[(1) - (1)].string).lower();
            (yyval.selector)->m_attr = idAttr;
            (yyval.selector)->m_value = (yyvsp[(1) - (1)].string);
        }
    ;}
    break;

  case 133:
//#line 846 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::Class;
        if (!p->m_strict)
            (yyvsp[(2) - (2)].string).lower();
        (yyval.selector)->m_attr = classAttr;
        (yyval.selector)->m_value = (yyvsp[(2) - (2)].string);
    ;}
    break;

  case 134:
//#line 858 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        CSSParserString& str = (yyvsp[(1) - (2)].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        Document* doc = p->document();
        if (doc && doc->isHTMLDocument())
            str.lower();
        (yyval.string) = str;
    ;}
    break;

  case 135:
//#line 869 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_attr = QualifiedName(nullAtom, (yyvsp[(3) - (4)].string), nullAtom);
        (yyval.selector)->m_match = CSSSelector::Set;
    ;}
    break;

  case 136:
//#line 874 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_attr = QualifiedName(nullAtom, (yyvsp[(3) - (8)].string), nullAtom);
        (yyval.selector)->m_match = (CSSSelector::Match)(yyvsp[(4) - (8)].integer);
        (yyval.selector)->m_value = (yyvsp[(6) - (8)].string);
    ;}
    break;

  case 137:
//#line 880 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        AtomicString namespacePrefix = (yyvsp[(3) - (5)].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_attr = QualifiedName(namespacePrefix, (yyvsp[(4) - (5)].string),
                                   p->m_styleSheet->determineNamespace(namespacePrefix));
        (yyval.selector)->m_match = CSSSelector::Set;
    ;}
    break;

  case 138:
//#line 888 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        AtomicString namespacePrefix = (yyvsp[(3) - (9)].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_attr = QualifiedName(namespacePrefix, (yyvsp[(4) - (9)].string),
                                   p->m_styleSheet->determineNamespace(namespacePrefix));
        (yyval.selector)->m_match = (CSSSelector::Match)(yyvsp[(5) - (9)].integer);
        (yyval.selector)->m_value = (yyvsp[(7) - (9)].string);
    ;}
    break;

  case 139:
//#line 900 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::Exact;
    ;}
    break;

  case 140:
//#line 903 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::List;
    ;}
    break;

  case 141:
//#line 906 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::Hyphen;
    ;}
    break;

  case 142:
//#line 909 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::Begin;
    ;}
    break;

  case 143:
//#line 912 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::End;
    ;}
    break;

  case 144:
//#line 915 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::Contain;
    ;}
    break;

  case 147:
//#line 926 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyvsp[(2) - (2)].string).lower();
        (yyval.selector)->m_value = (yyvsp[(2) - (2)].string);
        CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
        if (type == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
        else if (type == CSSSelector::PseudoEmpty ||
                 type == CSSSelector::PseudoFirstChild ||
                 type == CSSSelector::PseudoFirstOfType ||
                 type == CSSSelector::PseudoLastChild ||
                 type == CSSSelector::PseudoLastOfType ||
                 type == CSSSelector::PseudoOnlyChild ||
                 type == CSSSelector::PseudoOnlyOfType) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            Document* doc = p->document();
            if (doc)
                doc->setUsesSiblingRules(true);
        } else if (type == CSSSelector::PseudoFirstLine) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            if (Document* doc = p->document())
                doc->setUsesFirstLineRules(true);
        }
    ;}
    break;

  case 148:
//#line 951 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoElement;
        (yyvsp[(3) - (3)].string).lower();
        (yyval.selector)->m_value = (yyvsp[(3) - (3)].string);
        CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
        if (type == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
        else if (type == CSSSelector::PseudoFirstLine) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            if (Document* doc = p->document())
                doc->setUsesFirstLineRules(true);
        }
    ;}
    break;

  case 149:
//#line 966 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        CSSParser *p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = (yyvsp[(3) - (4)].string);
        (yyval.selector)->m_value = (yyvsp[(2) - (4)].string);
        CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
        if (type == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
        else if (type == CSSSelector::PseudoNthChild ||
                 type == CSSSelector::PseudoNthOfType ||
                 type == CSSSelector::PseudoNthLastChild ||
                 type == CSSSelector::PseudoNthLastOfType) {
            if (p->document())
                p->document()->setUsesSiblingRules(true);
        }
    ;}
    break;

  case 150:
//#line 984 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        CSSParser *p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = String::number((yyvsp[(3) - (4)].number));
        (yyval.selector)->m_value = (yyvsp[(2) - (4)].string);
        CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
        if (type == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
        else if (type == CSSSelector::PseudoNthChild ||
                 type == CSSSelector::PseudoNthOfType ||
                 type == CSSSelector::PseudoNthLastChild ||
                 type == CSSSelector::PseudoNthLastOfType) {
            if (p->document())
                p->document()->setUsesSiblingRules(true);
        }
    ;}
    break;

  case 151:
//#line 1002 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        CSSParser *p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_match = CSSSelector::PseudoClass;
        (yyval.selector)->m_argument = (yyvsp[(3) - (4)].string);
        (yyvsp[(2) - (4)].string).lower();
        (yyval.selector)->m_value = (yyvsp[(2) - (4)].string);
        CSSSelector::PseudoType type = (yyval.selector)->pseudoType();
        if (type == CSSSelector::PseudoUnknown)
            (yyval.selector) = 0;
        else if (type == CSSSelector::PseudoNthChild ||
                 type == CSSSelector::PseudoNthOfType ||
                 type == CSSSelector::PseudoNthLastChild ||
                 type == CSSSelector::PseudoNthLastOfType) {
            if (p->document())
                p->document()->setUsesSiblingRules(true);
        }
    ;}
    break;

  case 152:
//#line 1021 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        if (!(yyvsp[(4) - (6)].selector))
            (yyval.selector) = 0;
        else {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selector) = p->createFloatingSelector();
            (yyval.selector)->m_match = CSSSelector::PseudoClass;
            (yyval.selector)->m_simpleSelector = p->sinkFloatingSelector((yyvsp[(4) - (6)].selector));
            (yyvsp[(2) - (6)].string).lower();
            (yyval.selector)->m_value = (yyvsp[(2) - (6)].string);
        }
    ;}
    break;

  case 153:
//#line 1036 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (1)].boolean);
    ;}
    break;

  case 154:
//#line 1039 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (2)].boolean);
        if ( (yyvsp[(2) - (2)].boolean) )
            (yyval.boolean) = (yyvsp[(2) - (2)].boolean);
    ;}
    break;

  case 155:
//#line 1044 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (1)].boolean);
    ;}
    break;

  case 156:
//#line 1047 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 157:
//#line 1050 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 158:
//#line 1053 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (2)].boolean);
    ;}
    break;

  case 159:
//#line 1059 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (3)].boolean);
    ;}
    break;

  case 160:
//#line 1062 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 161:
//#line 1065 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 162:
//#line 1068 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 163:
//#line 1071 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (4)].boolean);
        if ((yyvsp[(2) - (4)].boolean))
            (yyval.boolean) = (yyvsp[(2) - (4)].boolean);
    ;}
    break;

  case 164:
//#line 1076 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (4)].boolean);
    ;}
    break;

  case 165:
//#line 1079 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (6)].boolean);
    ;}
    break;

  case 166:
//#line 1085 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
        CSSParser* p = static_cast<CSSParser*>(parser);
        if ((yyvsp[(1) - (5)].integer) && (yyvsp[(4) - (5)].valueList)) {
            p->m_valueList = p->sinkFloatingValueList((yyvsp[(4) - (5)].valueList));
            int oldParsedProperties = p->m_numParsedProperties;
            (yyval.boolean) = p->parseValue((yyvsp[(1) - (5)].integer), (yyvsp[(5) - (5)].boolean));
            if (!(yyval.boolean))
                p->rollbackLastProperties(p->m_numParsedProperties - oldParsedProperties);
            delete p->m_valueList;
            p->m_valueList = 0;
        }
    ;}
    break;

  case 167:
//#line 1099 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 168:
//#line 1103 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        /* The default movable type template has letter-spacing: .none;  Handle this by looking for
        error tokens at the start of an expr, recover the expr and then treat as an error, cleaning
        up and deleting the shifted expr.  */
        (yyval.boolean) = false;
    ;}
    break;

  case 169:
//#line 1110 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        /* Handle this case: div { text-align: center; !important } Just reduce away the stray !important. */
        (yyval.boolean) = false;
    ;}
    break;

  case 170:
//#line 1115 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        /* div { font-family: } Just reduce away this property with no value. */
        (yyval.boolean) = false;
    ;}
    break;

  case 171:
//#line 1120 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        /* if we come across rules with invalid values like this case: p { weight: *; }, just discard the rule */
        (yyval.boolean) = false;
    ;}
    break;

  case 172:
//#line 1127 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = cssPropertyID((yyvsp[(1) - (2)].string));
    ;}
    break;

  case 173:
//#line 1133 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.boolean) = true; ;}
    break;

  case 174:
//#line 1134 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.boolean) = false; ;}
    break;

  case 175:
//#line 1138 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.valueList) = p->createFloatingValueList();
        (yyval.valueList)->addValue(p->sinkFloatingValue((yyvsp[(1) - (1)].value)));
    ;}
    break;

  case 176:
//#line 1143 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.valueList) = (yyvsp[(1) - (3)].valueList);
        if ((yyval.valueList)) {
            if ((yyvsp[(2) - (3)].character)) {
                CSSParserValue v;
                v.id = 0;
                v.unit = CSSParserValue::Operator;
                v.iValue = (yyvsp[(2) - (3)].character);
                (yyval.valueList)->addValue(v);
            }
            (yyval.valueList)->addValue(p->sinkFloatingValue((yyvsp[(3) - (3)].value)));
        }
    ;}
    break;

  case 177:
//#line 1157 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.valueList) = 0;
    ;}
    break;

  case 178:
//#line 1163 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.character) = '/';
    ;}
    break;

  case 179:
//#line 1166 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.character) = ',';
    ;}
    break;

  case 180:
//#line 1169 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.character) = 0;
  ;}
    break;

  case 181:
//#line 1175 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value) = (yyvsp[(1) - (1)].value); ;}
    break;

  case 182:
//#line 1176 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value) = (yyvsp[(2) - (2)].value); (yyval.value).fValue *= (yyvsp[(1) - (2)].integer); ;}
    break;

  case 183:
//#line 1177 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_STRING; ;}
    break;

  case 184:
//#line 1178 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
      (yyval.value).id = cssValueKeywordID((yyvsp[(1) - (2)].string));
      (yyval.value).unit = CSSPrimitiveValue::CSS_IDENT;
      (yyval.value).string = (yyvsp[(1) - (2)].string);
  ;}
    break;

  case 185:
//#line 1184 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_DIMENSION ;}
    break;

  case 186:
//#line 1185 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(2) - (3)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_DIMENSION ;}
    break;

  case 187:
//#line 1186 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_URI; ;}
    break;

  case 188:
//#line 1187 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_UNICODE_RANGE ;}
    break;

  case 189:
//#line 1188 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
      { (yyval.value).id = 0; (yyval.value).string =(yyvsp[(1) - (1)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_RGBCOLOR; ;}
    break;

  case 190:
//#line 1189 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
      //Note by Arpit Baldeva:The original code calls CSSParserString() here which results in a garbage object. I can not put a user defined constructor in the class.
      //This is because CSSParserString is part of Union. And Union can only contain POD types.
      CSSParserString temp;
      temp.characters = 0;
      temp.length = 0;
      { (yyval.value).id = 0; (yyval.value).string = temp; (yyval.value).unit = CSSPrimitiveValue::CSS_RGBCOLOR; ;}
    }
    break;

  case 191:
//#line 1191 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
      (yyval.value) = (yyvsp[(1) - (1)].value);
  ;}
    break;

  case 192:
//#line 1194 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
      (yyval.value).id = 0;
      (yyval.value).string = (yyvsp[(1) - (2)].string);
      (yyval.value).unit = CSSPrimitiveValue::CSS_PARSER_VARIABLE;
  ;}
    break;

  case 193:
//#line 1199 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {;}
    break;

  case 194:
//#line 1203 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).isInt = true; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER; ;}
    break;

  case 195:
//#line 1204 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).isInt = false; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER; ;}
    break;

  case 196:
//#line 1205 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_PERCENTAGE; ;}
    break;

  case 197:
//#line 1206 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_PX; ;}
    break;

  case 198:
//#line 1207 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_CM; ;}
    break;

  case 199:
//#line 1208 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_MM; ;}
    break;

  case 200:
//#line 1209 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_IN; ;}
    break;

  case 201:
//#line 1210 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_PT; ;}
    break;

  case 202:
//#line 1211 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_PC; ;}
    break;

  case 203:
//#line 1212 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_DEG; ;}
    break;

  case 204:
//#line 1213 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_RAD; ;}
    break;

  case 205:
//#line 1214 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_GRAD; ;}
    break;

  case 206:
//#line 1215 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_MS; ;}
    break;

  case 207:
//#line 1216 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_S; ;}
    break;

  case 208:
//#line 1217 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_HZ; ;}
    break;

  case 209:
//#line 1218 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_KHZ; ;}
    break;

  case 210:
//#line 1219 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_EMS; ;}
    break;

  case 211:
//#line 1220 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSParserValue::Q_EMS; ;}
    break;

  case 212:
//#line 1221 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_EXS; ;}
    break;

  case 213:
//#line 1226 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        CSSParserFunction* f = p->createFloatingFunction();
        f->name = (yyvsp[(1) - (5)].string);
        f->args = p->sinkFloatingValueList((yyvsp[(3) - (5)].valueList));
        (yyval.value).id = 0;
        (yyval.value).unit = CSSParserValue::Function;
        (yyval.value).function = f;
    ;}
    break;

  case 214:
//#line 1235 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        CSSParserFunction* f = p->createFloatingFunction();
        f->name = (yyvsp[(1) - (3)].string);
        f->args = 0;
        (yyval.value).id = 0;
        (yyval.value).unit = CSSParserValue::Function;
        (yyval.value).function = f;
  ;}
    break;

  case 215:
//#line 1251 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.string) = (yyvsp[(1) - (2)].string); ;}
    break;

  case 216:
//#line 1252 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    { (yyval.string) = (yyvsp[(1) - (2)].string); ;}
    break;

  case 217:
//#line 1259 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 218:
//#line 1262 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 219:
//#line 1268 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 220:
//#line 1274 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;


/* Line 1267 of yacc.c.  */
//#line 3537 "CSSGrammar.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


//#line 1300 "D:/Development/Internet/OWB/doduo/WebCore/css/CSSGrammar.y"


