
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse         cssyyparse
#define yylex           cssyylex
#define yyerror         cssyyerror
#define yylval          cssyylval
#define yychar          cssyychar
#define yydebug         cssyydebug
#define yynerrs         cssyynerrs


/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"


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
#include "CSSPropertyNames.h"
#include "CSSRuleList.h"
#include "CSSSelector.h"
#include "CSSStyleSheet.h"
#include "Document.h"
#include "HTMLNames.h"
#include "MediaList.h"
#include "WebKitCSSKeyframeRule.h"
#include "WebKitCSSKeyframesRule.h"
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



/* Line 189 of yacc.c  */
#line 136 "CSSGrammar.cpp"

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

/* Line 214 of yacc.c  */
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



/* Line 214 of yacc.c  */
#line 261 "CSSGrammar.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 264 of yacc.c  */
#line 80 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"


static inline int cssyyerror(const char*)
{
    return 1;
}

static int cssyylex(YYSTYPE* yylval, void* parser)
{
    return static_cast<CSSParser*>(parser)->lex(yylval);
}



/* Line 264 of yacc.c  */
#line 288 "CSSGrammar.cpp"

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
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  28
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1350

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  85
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  90
/* YYNRULES -- Number of rules.  */
#define YYNRULES  268
/* YYNRULES -- Number of states.  */
#define YYNSTATES  516

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   318

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    82,    84,    83,     2,     2,
      72,    73,    19,    75,    74,    78,    17,    81,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    16,    71,
       2,    80,    77,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    18,     2,    79,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    68,    20,    69,    76,     2,     2,     2,
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
      60,    61,    62,    63,    64,    65,    66,    67,    70
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,    10,    13,    16,    19,    22,    25,    28,
      31,    33,    35,    42,    49,    55,    61,    67,    73,    79,
      80,    83,    84,    87,    90,    91,    93,    95,    97,   103,
     107,   111,   112,   116,   118,   119,   123,   124,   128,   129,
     133,   135,   137,   139,   141,   143,   145,   147,   149,   151,
     152,   156,   158,   160,   162,   164,   166,   168,   170,   172,
     174,   181,   188,   192,   196,   204,   212,   213,   217,   219,
     222,   224,   228,   230,   233,   237,   242,   246,   252,   257,
     262,   269,   274,   282,   285,   291,   295,   300,   303,   310,
     314,   318,   319,   322,   324,   326,   329,   330,   335,   343,
     345,   351,   352,   356,   357,   359,   361,   363,   368,   369,
     371,   373,   378,   381,   389,   396,   399,   408,   410,   412,
     413,   417,   424,   426,   432,   434,   436,   440,   444,   452,
     456,   460,   463,   466,   469,   471,   473,   479,   481,   486,
     489,   492,   494,   496,   499,   503,   506,   508,   511,   514,
     516,   519,   521,   524,   528,   531,   533,   535,   537,   540,
     543,   545,   547,   549,   551,   553,   556,   559,   564,   573,
     579,   589,   591,   593,   595,   597,   599,   601,   603,   605,
     608,   612,   617,   622,   627,   634,   636,   639,   641,   645,
     647,   650,   653,   657,   662,   666,   672,   677,   682,   689,
     695,   698,   701,   708,   715,   718,   722,   727,   730,   733,
     736,   737,   739,   743,   746,   749,   752,   753,   755,   758,
     761,   764,   767,   771,   774,   777,   779,   782,   784,   787,
     790,   793,   796,   799,   802,   805,   808,   811,   814,   817,
     820,   823,   826,   829,   832,   835,   838,   841,   844,   847,
     849,   853,   856,   862,   866,   869,   872,   874,   877,   881,
     885,   888,   892,   894,   896,   899,   905,   909,   911
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      86,     0,    -1,    97,    96,   100,   101,   102,   103,    -1,
      88,    95,    -1,    90,    95,    -1,    92,    95,    -1,    93,
      95,    -1,    94,    95,    -1,    91,    95,    -1,    89,    95,
      -1,   104,    -1,   109,    -1,    27,    68,    95,    87,    95,
      69,    -1,    29,    68,    95,   133,    95,    69,    -1,    28,
      68,    95,   155,    69,    -1,    37,    68,    95,   112,    69,
      -1,    31,    68,    95,   160,    69,    -1,    32,     4,    95,
     125,    69,    -1,    33,    68,    95,   141,    69,    -1,    -1,
      95,     4,    -1,    -1,    96,     5,    -1,    96,     4,    -1,
      -1,    99,    -1,    69,    -1,     0,    -1,    25,    95,    11,
      95,    71,    -1,    25,     1,   173,    -1,    25,     1,    71,
      -1,    -1,   100,   109,    96,    -1,   169,    -1,    -1,   101,
     110,    96,    -1,    -1,   102,   116,    96,    -1,    -1,   103,
     105,    96,    -1,   140,    -1,   128,    -1,   136,    -1,   137,
      -1,   130,    -1,   104,    -1,   172,    -1,   168,    -1,   170,
      -1,    -1,   106,   108,    96,    -1,   140,    -1,   136,    -1,
     137,    -1,   130,    -1,   107,    -1,   172,    -1,   168,    -1,
     170,    -1,   171,    -1,    21,    95,   118,    95,   126,    71,
      -1,    21,    95,   118,    95,   126,   173,    -1,    21,     1,
      71,    -1,    21,     1,   173,    -1,    34,    95,   126,    68,
      95,   112,    69,    -1,    35,    95,   111,    68,    95,   112,
      69,    -1,    -1,    36,     4,   127,    -1,   114,    -1,   113,
     114,    -1,   113,    -1,     1,   174,     1,    -1,     1,    -1,
     113,     1,    -1,   114,    71,    95,    -1,   114,   174,    71,
      95,    -1,     1,    71,    95,    -1,     1,   174,     1,    71,
      95,    -1,   113,   114,    71,    95,    -1,   113,     1,    71,
      95,    -1,   113,     1,   174,     1,    71,    95,    -1,   115,
      16,    95,   160,    -1,   115,    95,    68,    95,   155,    69,
      95,    -1,   115,     1,    -1,   115,    16,    95,     1,   160,
      -1,   115,    16,    95,    -1,   115,    16,    95,     1,    -1,
      12,    95,    -1,    26,    95,   117,   118,    95,    71,    -1,
      26,     1,   173,    -1,    26,     1,    71,    -1,    -1,    12,
       4,    -1,    11,    -1,    63,    -1,    12,    95,    -1,    -1,
      16,    95,   160,    95,    -1,    72,    95,   119,    95,   120,
      73,    95,    -1,   121,    -1,   122,    95,    42,    95,   121,
      -1,    -1,    42,    95,   122,    -1,    -1,    40,    -1,    41,
      -1,   122,    -1,   124,    95,   129,   123,    -1,    -1,   127,
      -1,   125,    -1,   127,    74,    95,   125,    -1,   127,     1,
      -1,    23,    95,   127,    68,    95,   106,   167,    -1,    23,
      95,    68,    95,   106,   167,    -1,    12,    95,    -1,    30,
      95,   131,    95,    68,    95,   132,    69,    -1,    12,    -1,
      11,    -1,    -1,   132,   133,    95,    -1,   134,    95,    68,
      95,   155,    69,    -1,   135,    -1,   134,    95,    74,    95,
     135,    -1,    60,    -1,    12,    -1,    22,     1,   173,    -1,
      22,     1,    71,    -1,    24,    95,    68,    95,   155,    69,
      95,    -1,    24,     1,   173,    -1,    24,     1,    71,    -1,
      75,    95,    -1,    76,    95,    -1,    77,    95,    -1,    78,
      -1,    75,    -1,   141,    68,    95,   155,    98,    -1,   143,
      -1,   141,    74,    95,   143,    -1,   141,     1,    -1,   143,
       4,    -1,   145,    -1,   142,    -1,   142,   145,    -1,   143,
     138,   145,    -1,   143,     1,    -1,    20,    -1,    19,    20,
      -1,    12,    20,    -1,   146,    -1,   146,   147,    -1,   147,
      -1,   144,   146,    -1,   144,   146,   147,    -1,   144,   147,
      -1,    12,    -1,    19,    -1,   148,    -1,   147,   148,    -1,
     147,     1,    -1,    15,    -1,    14,    -1,   149,    -1,   151,
      -1,   154,    -1,    17,    12,    -1,    12,    95,    -1,    18,
      95,   150,    79,    -1,    18,    95,   150,   152,    95,   153,
      95,    79,    -1,    18,    95,   144,   150,    79,    -1,    18,
      95,   144,   150,   152,    95,   153,    95,    79,    -1,    80,
      -1,     6,    -1,     7,    -1,     8,    -1,     9,    -1,    10,
      -1,    12,    -1,    11,    -1,    16,    12,    -1,    16,    16,
      12,    -1,    16,    64,    13,    73,    -1,    16,    64,    62,
      73,    -1,    16,    64,    12,    73,    -1,    16,    65,    95,
     145,    95,    73,    -1,   157,    -1,   156,   157,    -1,   156,
      -1,     1,   174,     1,    -1,     1,    -1,   156,     1,    -1,
     156,   174,    -1,   157,    71,    95,    -1,   157,   174,    71,
      95,    -1,     1,    71,    95,    -1,     1,   174,     1,    71,
      95,    -1,   156,   157,    71,    95,    -1,   156,     1,    71,
      95,    -1,   156,     1,   174,     1,    71,    95,    -1,   158,
      16,    95,   160,   159,    -1,   164,    95,    -1,   158,     1,
      -1,   158,    16,    95,     1,   160,   159,    -1,   158,    16,
      95,   160,   159,     1,    -1,    39,    95,    -1,   158,    16,
      95,    -1,   158,    16,    95,     1,    -1,   158,   173,    -1,
      12,    95,    -1,    39,    95,    -1,    -1,   162,    -1,   160,
     161,   162,    -1,   160,     1,    -1,    81,    95,    -1,    74,
      95,    -1,    -1,   163,    -1,   139,   163,    -1,    11,    95,
      -1,    12,    95,    -1,    59,    95,    -1,   139,    59,    95,
      -1,    63,    95,    -1,    66,    95,    -1,   166,    -1,    82,
      95,    -1,   165,    -1,   164,    95,    -1,    83,    95,    -1,
      62,    95,    -1,    61,    95,    -1,    60,    95,    -1,    46,
      95,    -1,    47,    95,    -1,    48,    95,    -1,    49,    95,
      -1,    50,    95,    -1,    51,    95,    -1,    52,    95,    -1,
      53,    95,    -1,    54,    95,    -1,    55,    95,    -1,    56,
      95,    -1,    57,    95,    -1,    58,    95,    -1,    44,    95,
      -1,    43,    95,    -1,    45,    95,    -1,    67,    -1,    80,
      12,    80,    -1,    84,    12,    -1,    64,    95,   160,    73,
      95,    -1,    64,    95,     1,    -1,    14,    95,    -1,    15,
      95,    -1,    98,    -1,     1,    98,    -1,    38,     1,   173,
      -1,    38,     1,    71,    -1,   168,    96,    -1,   169,   168,
      96,    -1,   109,    -1,   128,    -1,     1,   173,    -1,    68,
       1,   174,     1,    98,    -1,    68,     1,    98,    -1,   173,
      -1,   174,     1,   173,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   261,   261,   262,   263,   264,   265,   266,   267,   268,
     272,   273,   277,   283,   289,   295,   301,   315,   322,   329,
     330,   333,   335,   336,   339,   341,   346,   347,   351,   357,
     359,   363,   365,   370,   374,   376,   383,   385,   388,   390,
     398,   399,   400,   401,   402,   406,   407,   408,   409,   413,
     414,   425,   426,   427,   428,   432,   433,   434,   435,   436,
     441,   444,   447,   450,   456,   460,   466,   470,   476,   479,
     484,   487,   490,   493,   499,   502,   505,   508,   511,   516,
     519,   525,   529,   533,   537,   541,   546,   553,   559,   564,
     565,   569,   570,   574,   575,   579,   585,   588,   594,   601,
     606,   613,   616,   622,   625,   628,   634,   639,   647,   650,
     654,   659,   664,   670,   673,   679,   685,   692,   693,   697,
     698,   706,   712,   717,   726,   727,   751,   754,   760,   764,
     767,   773,   774,   775,   779,   780,   784,   790,   793,   801,
     807,   813,   816,   820,   836,   856,   862,   863,   864,   868,
     873,   880,   886,   896,   908,   921,   929,   937,   940,   953,
     959,   968,   981,   982,   983,   987,   999,  1010,  1015,  1021,
    1029,  1041,  1044,  1047,  1050,  1053,  1056,  1062,  1063,  1067,
    1092,  1107,  1125,  1143,  1162,  1177,  1180,  1185,  1188,  1191,
    1194,  1197,  1203,  1206,  1209,  1212,  1215,  1220,  1223,  1229,
    1243,  1255,  1259,  1266,  1271,  1276,  1281,  1286,  1293,  1299,
    1300,  1304,  1309,  1323,  1329,  1332,  1335,  1341,  1342,  1343,
    1344,  1350,  1351,  1352,  1353,  1354,  1355,  1357,  1360,  1363,
    1367,  1368,  1369,  1370,  1371,  1372,  1373,  1374,  1375,  1376,
    1377,  1378,  1379,  1380,  1381,  1382,  1383,  1384,  1385,  1389,
    1394,  1399,  1407,  1416,  1432,  1433,  1440,  1443,  1449,  1452,
    1458,  1459,  1463,  1469,  1475,  1493,  1494,  1498,  1499
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "TOKEN_EOF", "error", "$undefined", "UNIMPORTANT_TOK", "WHITESPACE",
  "SGML_CD", "INCLUDES", "DASHMATCH", "BEGINSWITH", "ENDSWITH", "CONTAINS",
  "STRING", "IDENT", "NTH", "HEX", "IDSEL", "':'", "'.'", "'['", "'*'",
  "'|'", "IMPORT_SYM", "PAGE_SYM", "MEDIA_SYM", "FONT_FACE_SYM",
  "CHARSET_SYM", "NAMESPACE_SYM", "WEBKIT_RULE_SYM", "WEBKIT_DECLS_SYM",
  "WEBKIT_KEYFRAME_RULE_SYM", "WEBKIT_KEYFRAMES_SYM", "WEBKIT_VALUE_SYM",
  "WEBKIT_MEDIAQUERY_SYM", "WEBKIT_SELECTOR_SYM", "WEBKIT_VARIABLES_SYM",
  "WEBKIT_DEFINE_SYM", "VARIABLES_FOR", "WEBKIT_VARIABLES_DECLS_SYM",
  "ATKEYWORD", "IMPORTANT_SYM", "MEDIA_ONLY", "MEDIA_NOT", "MEDIA_AND",
  "QEMS", "EMS", "EXS", "PXS", "CMS", "MMS", "INS", "PTS", "PCS", "DEGS",
  "RADS", "GRADS", "MSECS", "SECS", "HERZ", "KHERZ", "DIMEN", "PERCENTAGE",
  "FLOATTOKEN", "INTEGER", "URI", "FUNCTION", "NOTFUNCTION",
  "UNICODERANGE", "VARCALL", "'{'", "'}'", "LOWEST_PREC", "';'", "'('",
  "')'", "','", "'+'", "'~'", "'>'", "'-'", "']'", "'='", "'/'", "'#'",
  "'%'", "'$'", "$accept", "stylesheet", "valid_rule_or_import",
  "webkit_rule", "webkit_keyframe_rule", "webkit_decls",
  "webkit_variables_decls", "webkit_value", "webkit_mediaquery",
  "webkit_selector", "maybe_space", "maybe_sgml", "maybe_charset",
  "closing_brace", "charset", "import_list", "variables_list",
  "namespace_list", "rule_list", "valid_rule", "rule", "block_rule_list",
  "block_valid_rule", "block_rule", "import", "variables_rule",
  "variables_media_list", "variables_declaration_list",
  "variables_decl_list", "variables_declaration", "variable_name",
  "namespace", "maybe_ns_prefix", "string_or_uri", "media_feature",
  "maybe_media_value", "media_query_exp", "media_query_exp_list",
  "maybe_and_media_query_exp_list", "maybe_media_restrictor",
  "media_query", "maybe_media_list", "media_list", "media", "medium",
  "keyframes", "keyframe_name", "keyframes_rule", "keyframe_rule",
  "key_list", "key", "page", "font_face", "combinator", "unary_operator",
  "ruleset", "selector_list", "selector_with_trailing_whitespace",
  "selector", "namespace_selector", "simple_selector", "element_name",
  "specifier_list", "specifier", "class", "attr_name", "attrib", "match",
  "ident_or_string", "pseudo", "declaration_list", "decl_list",
  "declaration", "property", "prio", "expr", "operator", "term",
  "unary_term", "variable_reference", "function", "hexcolor", "save_block",
  "invalid_at", "invalid_at_list", "invalid_import", "invalid_media",
  "invalid_rule", "invalid_block", "invalid_block_list", 0
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
     310,   311,   312,   313,   314,   315,   316,   317,   123,   125,
     318,    59,    40,    41,    44,    43,   126,    62,    45,    93,
      61,    47,    35,    37,    36
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    85,    86,    86,    86,    86,    86,    86,    86,    86,
      87,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      95,    96,    96,    96,    97,    97,    98,    98,    99,    99,
      99,   100,   100,   100,   101,   101,   102,   102,   103,   103,
     104,   104,   104,   104,   104,   105,   105,   105,   105,   106,
     106,   107,   107,   107,   107,   108,   108,   108,   108,   108,
     109,   109,   109,   109,   110,   110,   111,   111,   112,   112,
     112,   112,   112,   112,   113,   113,   113,   113,   113,   113,
     113,   114,   114,   114,   114,   114,   114,   115,   116,   116,
     116,   117,   117,   118,   118,   119,   120,   120,   121,   122,
     122,   123,   123,   124,   124,   124,   125,   125,   126,   126,
     127,   127,   127,   128,   128,   129,   130,   131,   131,   132,
     132,   133,   134,   134,   135,   135,   136,   136,   137,   137,
     137,   138,   138,   138,   139,   139,   140,   141,   141,   141,
     142,   143,   143,   143,   143,   143,   144,   144,   144,   145,
     145,   145,   145,   145,   145,   146,   146,   147,   147,   147,
     148,   148,   148,   148,   148,   149,   150,   151,   151,   151,
     151,   152,   152,   152,   152,   152,   152,   153,   153,   154,
     154,   154,   154,   154,   154,   155,   155,   155,   155,   155,
     155,   155,   156,   156,   156,   156,   156,   156,   156,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   158,   159,
     159,   160,   160,   160,   161,   161,   161,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   164,
     164,   164,   165,   165,   166,   166,   167,   167,   168,   168,
     169,   169,   170,   171,   172,   173,   173,   174,   174
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     6,     2,     2,     2,     2,     2,     2,     2,
       1,     1,     6,     6,     5,     5,     5,     5,     5,     0,
       2,     0,     2,     2,     0,     1,     1,     1,     5,     3,
       3,     0,     3,     1,     0,     3,     0,     3,     0,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       6,     6,     3,     3,     7,     7,     0,     3,     1,     2,
       1,     3,     1,     2,     3,     4,     3,     5,     4,     4,
       6,     4,     7,     2,     5,     3,     4,     2,     6,     3,
       3,     0,     2,     1,     1,     2,     0,     4,     7,     1,
       5,     0,     3,     0,     1,     1,     1,     4,     0,     1,
       1,     4,     2,     7,     6,     2,     8,     1,     1,     0,
       3,     6,     1,     5,     1,     1,     3,     3,     7,     3,
       3,     2,     2,     2,     1,     1,     5,     1,     4,     2,
       2,     1,     1,     2,     3,     2,     1,     2,     2,     1,
       2,     1,     2,     3,     2,     1,     1,     1,     2,     2,
       1,     1,     1,     1,     1,     2,     2,     4,     8,     5,
       9,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       3,     4,     4,     4,     6,     1,     2,     1,     3,     1,
       2,     2,     3,     4,     3,     5,     4,     4,     6,     5,
       2,     2,     6,     6,     2,     3,     4,     2,     2,     2,
       0,     1,     3,     2,     2,     2,     0,     1,     2,     2,
       2,     2,     3,     2,     2,     1,     2,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       3,     2,     5,     3,     2,     2,     1,     2,     3,     3,
       2,     3,     1,     1,     2,     5,     3,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
      24,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      19,    19,    19,    19,    19,    19,    19,    21,    25,     0,
       0,    19,    19,    19,    19,    19,    19,    19,     1,     3,
       9,     4,     8,     5,     6,     7,    31,     0,    30,    29,
      20,    19,     0,     0,     0,     0,   103,     0,     0,    23,
      22,     0,    34,    21,    33,     0,     0,   155,   161,   160,
       0,     0,    19,   156,   146,     0,     0,    19,     0,    19,
      19,    10,    11,    41,    44,    42,    43,    40,     0,   142,
       0,     0,   141,   149,     0,   157,   162,   163,   164,   189,
      19,    19,   249,     0,     0,     0,     0,   185,     0,    19,
     125,   124,    19,    19,   122,    19,    19,    19,    19,    19,
      19,    19,    19,    19,    19,    19,    19,    19,    19,    19,
      19,    19,    19,    19,    19,    19,    19,    19,    19,    19,
      19,    19,   135,   134,    19,    19,     0,     0,   211,   217,
      19,   227,   225,   104,   105,    19,    99,   106,    19,     0,
       0,    72,    19,     0,     0,    68,     0,     0,    36,    21,
     260,    21,    27,    26,   266,   267,     0,    28,   148,   179,
       0,     0,    19,   165,     0,   147,     0,     0,     0,   103,
       0,     0,     0,     0,   139,    19,    19,   143,   145,   140,
      19,    19,    19,     0,   155,   156,   152,     0,     0,   159,
     158,    19,     0,   208,   204,     0,   251,    14,   190,   186,
       0,    19,     0,   201,    19,   207,   200,     0,     0,   219,
     220,   254,   255,   247,   246,   248,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   221,
     232,   231,   230,   223,     0,   224,   226,   229,    19,   218,
     213,    16,    19,    19,     0,   228,     0,     0,     0,    17,
      18,    19,     0,    87,    15,    73,    69,    19,     0,    83,
      19,     0,   259,   258,    19,    19,    38,    21,    32,   261,
       0,   180,     0,     0,     0,     0,    19,     0,     0,     0,
      62,    63,    93,    94,    19,   127,   126,    19,   110,     0,
     130,   129,    19,   118,   117,    19,    12,     0,     0,   131,
     132,   133,   144,     0,   194,   188,   250,    19,     0,    19,
       0,   192,    19,     0,    13,    19,    19,   253,     0,   222,
     215,   214,   212,    19,    19,    19,    19,   101,    76,    71,
      19,     0,    19,    74,    19,     0,    19,   103,    66,     0,
       0,    21,    35,   265,   268,   183,   181,   182,    19,   166,
      19,     0,   172,   173,   174,   175,   176,   167,   171,    19,
     108,    49,   112,    19,    19,     0,     0,     0,     0,    19,
     197,     0,   196,   193,   206,     0,     0,     0,    19,    95,
      96,     0,   115,    19,   107,    19,    79,     0,    78,    75,
      86,     0,     0,     0,     0,     0,     0,     0,    91,     0,
      45,    21,   262,    47,    48,    46,    37,     0,   169,    19,
       0,     0,     0,    49,   103,     0,    19,   136,   195,    19,
       0,    19,     0,     0,   123,   252,    19,     0,   100,     0,
      77,    19,     0,     0,    19,   103,    19,    90,    89,     0,
       0,   264,    39,   184,     0,   178,   177,    19,    60,    61,
       0,   256,    55,    21,   263,    54,    52,    53,    51,   114,
      57,    58,    59,    56,     0,   111,    19,   119,   198,   202,
     209,   203,   121,     0,    19,   102,    80,    19,     0,     0,
       0,    92,    19,    19,     0,   257,    50,   113,   128,     0,
       0,    98,    82,     0,     0,     0,     0,   168,   116,    19,
      97,    64,    65,    88,   170,   120
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     9,    70,    10,    11,    12,    13,    14,    15,    16,
     257,    36,    17,   461,    18,    52,   158,   276,   350,    71,
     411,   422,   462,   463,   412,   277,   406,   153,   154,   155,
     156,   351,   450,   294,   334,   437,   146,   147,   394,   148,
     298,   403,   404,    73,   337,    74,   305,   499,   102,   103,
     104,    75,    76,   193,   136,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,   289,    87,   369,   457,    88,
      95,    96,    97,    98,   432,   137,   254,   138,   139,   140,
     141,   142,   469,   470,    54,   471,   472,   473,   165,   166
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -304
static const yytype_int16 yypact[] =
{
     469,   198,   106,   125,   135,   147,    70,   153,   209,   283,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,    -6,
     366,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,   296,
     296,   296,   296,   296,   296,   296,    40,   319,  -304,  -304,
    -304,  -304,   857,   155,   202,  1151,   257,   565,   437,  -304,
    -304,   336,   325,  -304,   320,   143,   159,   373,  -304,  -304,
     400,   354,  -304,   391,  -304,   387,   403,  -304,   219,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,     7,   640,
     168,   542,  -304,   454,   346,  -304,  -304,  -304,  -304,   102,
    -304,  -304,  -304,   420,   421,   348,   149,   112,   218,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,   899,   926,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,    35,  -304,   377,
      83,   342,  -304,   382,   186,   356,   224,   363,   341,  -304,
     379,  -304,  -304,  -304,  -304,  -304,   456,  -304,  -304,  -304,
     451,   343,  -304,  -304,    34,  -304,   376,    30,   384,   191,
     405,   242,    45,    96,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,   640,  -304,  -304,   454,   385,   574,  -304,
    -304,  -304,   473,   296,   296,   409,  -304,  -304,   411,   414,
     181,  -304,    87,  -304,  -304,  -304,   296,   250,   133,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,  1076,   296,   296,   296,  -304,  -304,
    -304,  -304,  -304,  -304,  1208,   296,    59,    38,   295,  -304,
    -304,  -304,   494,   296,  -304,   416,   432,  -304,    91,  -304,
    -304,   268,  -304,  -304,  -304,  -304,   505,  -304,   379,   379,
     143,  -304,   465,   470,   480,   565,   373,   391,   561,   169,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,    29,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,   155,   565,   296,
     296,   296,  -304,   724,   296,   422,  -304,  -304,   573,  -304,
     508,   296,  -304,   551,  -304,  -304,  -304,  -304,  1001,   296,
     296,   296,  -304,  -304,  -304,  -304,  -304,   544,   296,   436,
    -304,   586,  -304,   296,  -304,   776,  -304,   254,    43,   425,
    1306,  -304,   379,  -304,  -304,  -304,  -304,  -304,  -304,   296,
    -304,   272,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
     367,   296,  -304,  -304,  -304,   155,   285,    16,   237,  -304,
     296,   468,   296,   296,  1208,   466,   155,   202,  -304,   296,
      39,   132,   296,  -304,  -304,  -304,   296,   474,   296,   296,
    1208,   626,   155,   525,     2,   612,   553,   496,   305,   508,
    -304,  -304,  -304,  -304,  -304,  -304,   379,    23,  -304,  -304,
     370,   500,  1281,   296,   257,   554,  -304,  -304,   296,  -304,
     466,  -304,   196,   555,  -304,   296,  -304,   552,  -304,   132,
     296,  -304,   701,   559,  -304,    10,  -304,  -304,  -304,   628,
     265,  -304,   379,  -304,   370,  -304,  -304,  -304,  -304,  -304,
     143,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  1281,  -304,  -304,   296,   296,  -304,
     296,  -304,  -304,  1151,  -304,    35,   296,  -304,   437,    86,
     437,  -304,  -304,  -304,    15,  -304,   379,  -304,   296,   215,
     851,   296,   296,   567,   570,   182,    56,  -304,  -304,  -304,
     296,  -304,  -304,  -304,  -304,   296
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
      -1,   -21,  -304,   -54,  -304,  -304,  -304,  -304,  -304,   280,
    -304,   221,  -304,  -304,     6,  -304,  -304,    82,  -304,   491,
    -304,  -304,  -304,   203,  -304,  -304,   255,   222,  -304,  -304,
     -44,   292,  -174,  -134,  -304,  -132,  -304,  -304,   164,  -304,
     277,  -120,  -107,  -304,  -304,   -78,   618,  -304,   358,   493,
     -61,   587,   -50,   -56,  -304,   408,  -304,   330,   244,  -304,
    -303,  -304,   603,  -304,   273,  -192,  -304,   457,   569,   -37,
    -304,  -304,   240,   -19,  -304,   368,  -304,   369,   -12,   -60
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -217
static const yytype_int16 yytable[] =
{
      20,   164,   149,   372,   377,   299,    99,    39,   184,    29,
      30,    31,    32,    33,    34,    35,   162,    53,   187,    40,
      42,    43,    44,    45,    46,    47,    48,    40,   200,   202,
     372,   197,   160,   198,    40,   161,   210,   212,    40,   -19,
      56,   292,    40,    40,    49,    50,   286,    40,    72,    40,
     143,   144,   328,   287,    64,   436,   303,   304,   159,    99,
      40,   174,    37,    40,   177,    38,   179,   181,   182,   183,
    -109,   333,   425,  -109,    25,   185,   374,   -19,    51,   405,
     335,   186,   145,   433,   184,   163,   215,   372,   320,   203,
     204,   262,   320,   293,   507,   268,   453,   373,   216,   443,
      40,   217,   218,   374,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   385,   312,   246,   247,   514,    40,    40,   278,   255,
     279,   200,   200,   162,   256,   273,   313,   258,   318,  -187,
     208,   263,   260,   401,   -67,   271,    89,   186,   322,    40,
     374,    90,   344,    40,   291,   306,   296,    90,   301,   188,
      37,   285,   189,   201,    21,   362,   363,   364,   365,   366,
      37,  -191,   320,   211,   307,   308,    40,   265,    91,   309,
     310,   311,   430,    22,    91,    40,  -199,   481,   152,    19,
     314,   325,   -19,    23,   145,   341,    40,   326,   442,   -19,
     321,    37,   163,   323,   100,    24,    92,    37,  -187,   213,
     180,    26,    92,   -19,   358,   269,   353,   100,   -19,    93,
     167,   143,   144,    94,   214,    93,  -137,  -137,   188,    94,
     270,   189,  -137,   190,   191,   192,    40,   329,   367,   368,
    -191,   330,   331,   513,    40,   -70,   352,   200,    40,   297,
     338,    40,   101,   145,  -199,  -199,   343,  -199,   354,   345,
      99,   489,    40,   347,   348,   101,   292,    27,   362,   363,
     364,   365,   366,    28,   508,   359,    37,   -19,   464,    40,
     465,   500,   -19,   370,   143,   144,   371,   143,   144,    40,
      40,   375,   466,   354,   376,  -138,  -138,   336,   354,    40,
     302,  -138,   190,   191,   192,   467,   380,   449,   382,   324,
      55,   383,  -108,   427,   386,   387,   145,   354,   293,   145,
     416,   413,   389,   390,   391,   392,   346,   157,    99,   396,
     464,   398,   465,   399,   468,   402,    65,   199,   408,    99,
    -151,   418,   368,   426,   466,   282,   283,   417,    51,   359,
      58,    59,    60,    61,    62,    99,   173,   467,   420,   354,
      40,    40,   423,   424,    40,   274,   275,    41,   428,  -103,
     475,   455,   456,    49,    50,   354,   199,   435,   176,  -154,
     452,   -19,   439,   168,   440,   448,   468,   451,   -19,    58,
      59,    60,    61,    62,   178,   284,   495,   143,   144,   459,
      37,   175,   169,   261,  -151,  -151,   170,   207,   454,  -151,
    -151,  -151,  -151,  -151,    37,   477,   407,   267,   478,   -19,
     480,    37,   205,   206,   272,   483,   -19,   -19,   151,   145,
     486,    40,   496,   488,    37,   490,   259,   290,   451,   152,
     -19,   264,    37,  -154,  -154,   295,   494,   280,  -154,  -154,
    -154,  -154,  -154,   281,   171,   172,  -210,   250,    58,    59,
      60,    61,    62,    37,   315,   498,   300,  -216,  -216,    37,
    -216,  -216,   317,   501,    37,   319,   502,   340,   -19,   316,
      37,   505,   506,   379,     1,   339,     2,     3,     4,   510,
       5,     6,     7,   342,    37,   431,     8,   395,   515,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,   349,  -216,  -216,  -210,  -210,    37,  -210,   355,   429,
     252,  -216,    37,   356,  -216,   441,  -216,   253,  -216,  -216,
    -216,  -205,   384,   357,   194,    40,    58,    59,    60,    61,
      62,   195,   105,   106,    37,   107,   108,   447,    37,    40,
     503,   458,   504,   360,   381,   199,    37,    57,  -150,    58,
      59,    60,    61,    62,    63,    64,   393,   397,    58,    59,
      60,    61,    62,   444,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   445,   131,    92,  -205,
    -205,   446,  -205,   476,   482,   484,   132,   250,   487,   133,
     410,    93,   491,   134,   135,    94,   511,  -216,  -216,   512,
    -216,  -216,  -150,  -150,   474,   266,   438,  -150,  -150,  -150,
    -150,  -150,    57,   492,    58,    59,    60,    61,    62,    63,
      64,   485,   421,   509,   434,   150,   378,   288,   196,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,   419,  -216,  -216,   -81,   -81,   361,   -81,   493,   209,
     252,  -216,   250,   479,  -216,   249,  -216,   253,  -216,  -216,
    -216,   332,  -216,  -216,   497,  -216,  -216,     0,   414,   415,
       0,     0,     0,     0,     0,   199,     0,     0,  -153,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,     0,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,     0,  -216,  -216,   -84,
     -84,     0,   -84,     0,     0,   252,  -216,   400,     0,  -216,
      40,  -216,   253,  -216,  -216,  -216,     0,   105,   106,     0,
     107,   108,  -153,  -153,     0,     0,     0,  -153,  -153,  -153,
    -153,  -153,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,     0,   131,    92,   -85,   -85,     0,   -85,     0,     0,
       0,   132,   250,     0,   133,   -19,    93,     0,   134,   135,
      94,    40,  -216,  -216,     0,  -216,  -216,     0,     0,    57,
       0,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,     0,     0,     0,     0,     0,    69,     0,     0,
       0,     0,     0,     0,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,     0,  -216,  -216,     0,
       0,     0,     0,     0,   -19,   252,  -216,   250,     0,  -216,
       0,  -216,   253,  -216,  -216,  -216,     0,  -216,  -216,     0,
    -216,  -216,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   248,   126,
     127,   128,     0,     0,     0,     0,     0,     0,     0,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,     0,  -216,  -216,     0,   251,     0,     0,     0,     0,
     252,  -216,   250,     0,  -216,     0,  -216,   253,  -216,  -216,
    -216,     0,  -216,  -216,     0,  -216,  -216,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,  -216,
    -216,  -216,  -216,  -216,  -216,  -216,     0,  -216,  -216,     0,
       0,     0,     0,     0,   388,   252,  -216,   327,     0,  -216,
      40,  -216,   253,  -216,  -216,  -216,     0,   105,   106,     0,
     107,   108,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,     0,   131,    92,     0,     0,     0,     0,     0,     0,
       0,   132,     0,     0,   133,    40,    93,     0,   134,   135,
      94,     0,   105,   106,     0,   107,   108,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,     0,   131,    92,   105,
     106,     0,   107,   108,     0,     0,   132,     0,     0,   133,
       0,    93,     0,   134,   135,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,     0,   131,    92,     0,     0,     0,     0,
       0,   162,   460,   132,     0,     0,   133,     0,    93,     0,
     134,   135,    94,    57,     0,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    -2,   409,     0,     0,
       0,    69,     0,     0,     0,     0,     0,     0,    57,    51,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,     0,     0,     0,     0,     0,    69,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     0,     0,
     163
};

static const yytype_int16 yycheck[] =
{
       1,    55,    46,     1,   307,   179,    43,    19,     1,    10,
      11,    12,    13,    14,    15,    16,     0,    36,    79,     4,
      21,    22,    23,    24,    25,    26,    27,     4,    84,    89,
       1,    81,    53,    83,     4,    54,    96,    97,     4,     4,
      41,    11,     4,     4,     4,     5,    12,     4,    42,     4,
      40,    41,   244,    19,    20,    16,    11,    12,    52,    96,
       4,    62,    68,     4,    65,    71,    67,    68,    69,    70,
      68,    12,   375,    71,     4,    68,    74,    42,    38,    36,
      42,    74,    72,   386,     1,    69,    98,     1,     1,    90,
      91,   151,     1,    63,    79,   155,    73,    68,    99,   402,
       4,   102,   103,    74,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   323,   193,   134,   135,    79,     4,     4,   159,   140,
     161,   197,   198,     0,   145,   157,   196,   148,   208,     0,
       1,   152,    69,   345,    68,   156,     1,    74,    71,     4,
      74,    12,    71,     4,   176,    69,   178,    12,   180,     1,
      68,   172,     4,    71,    68,     6,     7,     8,     9,    10,
      68,     0,     1,    71,   185,   186,     4,     1,    39,   190,
     191,   192,   384,    68,    39,     4,     0,     1,    12,     1,
     201,    68,     4,    68,    72,   265,     4,    74,   400,    11,
     211,    68,    69,   214,    12,    68,    67,    68,    69,     1,
       1,    68,    67,     4,   285,     1,   280,    12,     4,    80,
      71,    40,    41,    84,    16,    80,    68,    69,     1,    84,
      16,     4,    74,    75,    76,    77,     4,   248,    79,    80,
      69,   252,   253,    71,     4,    69,   277,   313,     4,    68,
     261,     4,    60,    72,    68,    69,   267,    71,   280,   270,
     307,   445,     4,   274,   275,    60,    11,    68,     6,     7,
       8,     9,    10,     0,    69,   286,    68,    68,   422,     4,
     422,   483,    68,   294,    40,    41,   297,    40,    41,     4,
       4,   302,   422,   315,   305,    68,    69,    12,   320,     4,
      68,    74,    75,    76,    77,   422,   317,    12,   319,    69,
       1,   322,    68,   377,   325,   326,    72,   339,    63,    72,
     351,   350,   333,   334,   335,   336,    68,     1,   375,   340,
     474,   342,   474,   344,   422,   346,    21,     1,   349,   386,
       4,    79,    80,    68,   474,    12,    13,   358,    38,   360,
      14,    15,    16,    17,    18,   402,    12,   474,   369,   381,
       4,     4,   373,   374,     4,    34,    35,    11,   379,    12,
     424,    11,    12,     4,     5,   397,     1,   388,     1,     4,
     411,     4,   393,    20,   395,   407,   474,   409,    11,    14,
      15,    16,    17,    18,     1,    62,   460,    40,    41,   421,
      68,    20,    12,    71,    68,    69,    16,    69,   419,    73,
      74,    75,    76,    77,    68,   426,     1,    71,   429,     4,
     431,    68,    12,    12,    71,   436,    11,    12,     1,    72,
     441,     4,   463,   444,    68,   446,    69,    71,   460,    12,
      63,    69,    68,    68,    69,    71,   457,     1,    73,    74,
      75,    76,    77,    12,    64,    65,     0,     1,    14,    15,
      16,    17,    18,    68,     1,   476,    71,    11,    12,    68,
      14,    15,    71,   484,    68,    71,   487,    71,    63,    80,
      68,   492,   493,    71,    25,     1,    27,    28,    29,   500,
      31,    32,    33,    71,    68,    39,    37,    71,   509,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    26,    66,    67,    68,    69,    68,    71,    73,    71,
      74,    75,    68,    73,    78,    71,    80,    81,    82,    83,
      84,     0,     1,    73,    12,     4,    14,    15,    16,    17,
      18,    19,    11,    12,    68,    14,    15,    71,    68,     4,
     488,    71,   490,    12,     1,     1,    68,    12,     4,    14,
      15,    16,    17,    18,    19,    20,    42,     1,    14,    15,
      16,    17,    18,    68,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,     4,    66,    67,    68,
      69,    68,    71,    69,    69,    73,    75,     1,    69,    78,
     350,    80,     4,    82,    83,    84,    69,    11,    12,    69,
      14,    15,    68,    69,   423,   154,   391,    73,    74,    75,
      76,    77,    12,   450,    14,    15,    16,    17,    18,    19,
      20,   439,   370,   499,   387,    47,   308,   174,    81,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,   361,    66,    67,    68,    69,   288,    71,   454,    96,
      74,    75,     1,   430,    78,   136,    80,    81,    82,    83,
      84,   254,    11,    12,   474,    14,    15,    -1,   350,   350,
      -1,    -1,    -1,    -1,    -1,     1,    -1,    -1,     4,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    -1,    66,    67,    68,
      69,    -1,    71,    -1,    -1,    74,    75,     1,    -1,    78,
       4,    80,    81,    82,    83,    84,    -1,    11,    12,    -1,
      14,    15,    68,    69,    -1,    -1,    -1,    73,    74,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    -1,    66,    67,    68,    69,    -1,    71,    -1,    -1,
      -1,    75,     1,    -1,    78,     4,    80,    -1,    82,    83,
      84,     4,    11,    12,    -1,    14,    15,    -1,    -1,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    -1,    66,    67,    -1,
      -1,    -1,    -1,    -1,    73,    74,    75,     1,    -1,    78,
      -1,    80,    81,    82,    83,    84,    -1,    11,    12,    -1,
      14,    15,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    -1,    66,    67,    -1,    69,    -1,    -1,    -1,    -1,
      74,    75,     1,    -1,    78,    -1,    80,    81,    82,    83,
      84,    -1,    11,    12,    -1,    14,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    -1,    66,    67,    -1,
      -1,    -1,    -1,    -1,    73,    74,    75,     1,    -1,    78,
       4,    80,    81,    82,    83,    84,    -1,    11,    12,    -1,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    78,     4,    80,    -1,    82,    83,
      84,    -1,    11,    12,    -1,    14,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    -1,    66,    67,    11,
      12,    -1,    14,    15,    -1,    -1,    75,    -1,    -1,    78,
      -1,    80,    -1,    82,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    -1,    66,    67,    -1,    -1,    -1,    -1,
      -1,     0,     1,    75,    -1,    -1,    78,    -1,    80,    -1,
      82,    83,    84,    12,    -1,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,     1,    -1,    -1,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    12,    38,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      69
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    25,    27,    28,    29,    31,    32,    33,    37,    86,
      88,    89,    90,    91,    92,    93,    94,    97,    99,     1,
      95,    68,    68,    68,    68,     4,    68,    68,     0,    95,
      95,    95,    95,    95,    95,    95,    96,    68,    71,   173,
       4,    11,    95,    95,    95,    95,    95,    95,    95,     4,
       5,    38,   100,   168,   169,     1,    95,    12,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    30,
      87,   104,   109,   128,   130,   136,   137,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   151,   154,     1,
      12,    39,    67,    80,    84,   155,   156,   157,   158,   164,
      12,    60,   133,   134,   135,    11,    12,    14,    15,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    66,    75,    78,    82,    83,   139,   160,   162,   163,
     164,   165,   166,    40,    41,    72,   121,   122,   124,   125,
     141,     1,    12,   112,   113,   114,   115,     1,   101,   109,
      96,   168,     0,    69,    98,   173,   174,    71,    20,    12,
      16,    64,    65,    12,    95,    20,     1,    95,     1,    95,
       1,    95,    95,    95,     1,    68,    74,   145,     1,     4,
      75,    76,    77,   138,    12,    19,   146,   147,   147,     1,
     148,    71,   174,    95,    95,    12,    12,    69,     1,   157,
     174,    71,   174,     1,    16,   173,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    59,   163,
       1,    69,    74,    81,   161,    95,    95,    95,    95,    69,
      69,    71,   174,    95,    69,     1,   114,    71,   174,     1,
      16,    95,    71,   173,    34,    35,   102,   110,    96,    96,
       1,    12,    12,    13,    62,    95,    12,    19,   144,   150,
      71,   173,    11,    63,   118,    71,   173,    68,   125,   127,
      71,   173,    68,    11,    12,   131,    69,    95,    95,    95,
      95,    95,   145,   147,    95,     1,    80,    71,   174,    71,
       1,    95,    71,    95,    69,    68,    74,     1,   160,    95,
      95,    95,   162,    12,   119,    42,    12,   129,    95,     1,
      71,   174,    71,    95,    71,    95,    68,    95,    95,    26,
     103,   116,    96,    98,   173,    73,    73,    73,   145,    95,
      12,   150,     6,     7,     8,     9,    10,    79,    80,   152,
      95,    95,     1,    68,    74,    95,    95,   155,   143,    71,
      95,     1,    95,    95,     1,   160,    95,    95,    73,    95,
      95,    95,    95,    42,   123,    71,    95,     1,    95,    95,
       1,   160,    95,   126,   127,    36,   111,     1,    95,     1,
     104,   105,   109,   168,   170,   172,    96,    95,    79,   152,
      95,   126,   106,    95,    95,   155,    68,    98,    95,    71,
     160,    39,   159,   155,   135,    95,    16,   120,   121,    95,
      95,    71,   160,   155,    68,     4,    68,    71,   173,    12,
     117,   173,    96,    73,    95,    11,    12,   153,    71,   173,
       1,    98,   107,   108,   128,   130,   136,   137,   140,   167,
     168,   170,   171,   172,   106,   125,    69,    95,    95,   159,
      95,     1,    69,    95,    73,   122,    95,    69,    95,   127,
      95,     4,   118,   153,    95,    98,    96,   167,    95,   132,
     160,    95,    95,   112,   112,    95,    95,    79,    69,   133,
      95,    69,    69,    71,    79,    95
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
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
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
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
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





/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

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
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

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
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
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

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
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

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
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
        case 12:

/* Line 1455 of yacc.c  */
#line 277 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        static_cast<CSSParser*>(parser)->m_rule = (yyvsp[(4) - (6)].rule);
    ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 283 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        static_cast<CSSParser*>(parser)->m_keyframe = (yyvsp[(4) - (6)].keyframeRule);
    ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 289 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        /* can be empty */
    ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 295 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        /* can be empty */
    ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 301 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 17:

/* Line 1455 of yacc.c  */
#line 315 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
         CSSParser* p = static_cast<CSSParser*>(parser);
         p->m_mediaQuery = p->sinkFloatingMediaQuery((yyvsp[(4) - (5)].mediaQuery));
     ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 322 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        p->m_floatingSelector = p->sinkFloatingSelector((yyvsp[(4) - (5)].selector));
    ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 341 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
  ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 351 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
     CSSParser* p = static_cast<CSSParser*>(parser);
     (yyval.rule) = static_cast<CSSParser*>(parser)->createCharsetRule((yyvsp[(3) - (5)].string));
     if ((yyval.rule) && p->m_styleSheet)
         p->m_styleSheet->append((yyval.rule));
  ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 357 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
  ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 359 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
  ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 365 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
     CSSParser* p = static_cast<CSSParser*>(parser);
     if ((yyvsp[(2) - (3)].rule) && p->m_styleSheet)
         p->m_styleSheet->append((yyvsp[(2) - (3)].rule));
 ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 370 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
 ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 376 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
    CSSParser* p = static_cast<CSSParser*>(parser);
     if ((yyvsp[(2) - (3)].rule) && p->m_styleSheet)
         p->m_styleSheet->append((yyvsp[(2) - (3)].rule));
;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 390 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
     CSSParser* p = static_cast<CSSParser*>(parser);
     if ((yyvsp[(2) - (3)].rule) && p->m_styleSheet)
         p->m_styleSheet->append((yyvsp[(2) - (3)].rule));
 ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 413 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.ruleList) = 0; ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 414 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
      (yyval.ruleList) = (yyvsp[(1) - (3)].ruleList);
      if ((yyvsp[(2) - (3)].rule)) {
          if (!(yyval.ruleList))
              (yyval.ruleList) = static_cast<CSSParser*>(parser)->createRuleList();
          (yyval.ruleList)->append((yyvsp[(2) - (3)].rule));
      }
  ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 441 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createImportRule((yyvsp[(3) - (6)].string), (yyvsp[(5) - (6)].mediaList));
    ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 444 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 447 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 450 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 456 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createVariablesRule((yyvsp[(3) - (7)].mediaList), true);
    ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 460 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createVariablesRule((yyvsp[(3) - (7)].mediaList), false);
    ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 466 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaList) = static_cast<CSSParser*>(parser)->createMediaList();
    ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 470 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaList) = (yyvsp[(3) - (3)].mediaList);
    ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 476 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (1)].boolean);
    ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 479 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (2)].boolean);
        if ((yyvsp[(2) - (2)].boolean))
            (yyval.boolean) = (yyvsp[(2) - (2)].boolean);
    ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 484 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (1)].boolean);
    ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 487 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 490 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 493 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (2)].boolean);
    ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 499 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (3)].boolean);
    ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 502 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 505 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 508 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 511 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (4)].boolean);
        if ((yyvsp[(2) - (4)].boolean))
            (yyval.boolean) = (yyvsp[(2) - (4)].boolean);
    ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 516 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (4)].boolean);
    ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 519 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (6)].boolean);
    ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 525 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = static_cast<CSSParser*>(parser)->addVariable((yyvsp[(1) - (4)].string), (yyvsp[(4) - (4)].valueList));
    ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 529 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = static_cast<CSSParser*>(parser)->addVariableDeclarationBlock((yyvsp[(1) - (7)].string));
    ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 533 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 537 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 541 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        /* @variables { varname: } Just reduce away this variable with no value. */
        (yyval.boolean) = false;
    ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 546 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        /* if we come across rules with invalid values like this case: @variables { varname: *; }, just discard the property/value pair */
        (yyval.boolean) = false;
    ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 553 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.string) = (yyvsp[(1) - (2)].string);
    ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 559 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
    CSSParser* p = static_cast<CSSParser*>(parser);
    if (p->m_styleSheet)
        p->m_styleSheet->addNamespace(p, (yyvsp[(3) - (6)].string), (yyvsp[(4) - (6)].string));
;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 569 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.string).characters = 0; ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 570 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.string) = (yyvsp[(1) - (2)].string); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 579 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.string) = (yyvsp[(1) - (2)].string);
    ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 585 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.valueList) = 0;
    ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 588 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.valueList) = (yyvsp[(3) - (4)].valueList);
    ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 594 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyvsp[(3) - (7)].string).lower();
        (yyval.mediaQueryExp) = static_cast<CSSParser*>(parser)->createFloatingMediaQueryExp((yyvsp[(3) - (7)].string), (yyvsp[(5) - (7)].valueList));
    ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 601 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.mediaQueryExpList) = p->createFloatingMediaQueryExpList();
        (yyval.mediaQueryExpList)->append(p->sinkFloatingMediaQueryExp((yyvsp[(1) - (1)].mediaQueryExp)));
    ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 606 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryExpList) = (yyvsp[(1) - (5)].mediaQueryExpList);
        (yyval.mediaQueryExpList)->append(static_cast<CSSParser*>(parser)->sinkFloatingMediaQueryExp((yyvsp[(5) - (5)].mediaQueryExp)));
    ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 613 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryExpList) = static_cast<CSSParser*>(parser)->createFloatingMediaQueryExpList();
    ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 616 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryExpList) = (yyvsp[(3) - (3)].mediaQueryExpList);
    ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 622 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryRestrictor) = MediaQuery::None;
    ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 625 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryRestrictor) = MediaQuery::Only;
    ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 628 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaQueryRestrictor) = MediaQuery::Not;
    ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 634 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.mediaQuery) = p->createFloatingMediaQuery(p->sinkFloatingMediaQueryExpList((yyvsp[(1) - (1)].mediaQueryExpList)));
    ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 639 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyvsp[(3) - (4)].string).lower();
        (yyval.mediaQuery) = p->createFloatingMediaQuery((yyvsp[(1) - (4)].mediaQueryRestrictor), (yyvsp[(3) - (4)].string), p->sinkFloatingMediaQueryExpList((yyvsp[(4) - (4)].mediaQueryExpList)));
    ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 647 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaList) = static_cast<CSSParser*>(parser)->createMediaList();
     ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 654 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.mediaList) = p->createMediaList();
        (yyval.mediaList)->appendMediaQuery(p->sinkFloatingMediaQuery((yyvsp[(1) - (1)].mediaQuery)));
    ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 659 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaList) = (yyvsp[(1) - (4)].mediaList);
        if ((yyval.mediaList))
            (yyval.mediaList)->appendMediaQuery(static_cast<CSSParser*>(parser)->sinkFloatingMediaQuery((yyvsp[(4) - (4)].mediaQuery)));
    ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 664 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.mediaList) = 0;
    ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 670 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createMediaRule((yyvsp[(3) - (7)].mediaList), (yyvsp[(6) - (7)].ruleList));
    ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 673 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createMediaRule(0, (yyvsp[(5) - (6)].ruleList));
    ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 679 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
      (yyval.string) = (yyvsp[(1) - (2)].string);
  ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 685 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = (yyvsp[(7) - (8)].keyframesRule);
        (yyvsp[(7) - (8)].keyframesRule)->setName((yyvsp[(3) - (8)].string));
    ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 697 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.keyframesRule) = static_cast<CSSParser*>(parser)->createKeyframesRule(); ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 698 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.keyframesRule) = (yyvsp[(1) - (3)].keyframesRule);
        if ((yyvsp[(2) - (3)].keyframeRule))
            (yyval.keyframesRule)->append((yyvsp[(2) - (3)].keyframeRule));
    ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 706 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.keyframeRule) = static_cast<CSSParser*>(parser)->createKeyframeRule((yyvsp[(1) - (6)].valueList));
    ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 712 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.valueList) = p->createFloatingValueList();
        (yyval.valueList)->addValue(p->sinkFloatingValue((yyvsp[(1) - (1)].value)));
    ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 717 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.valueList) = (yyvsp[(1) - (5)].valueList);
        if ((yyval.valueList))
            (yyval.valueList)->addValue(p->sinkFloatingValue((yyvsp[(5) - (5)].value)));
    ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 726 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).isInt = false; (yyval.value).fValue = (yyvsp[(1) - (1)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER; ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 727 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.value).id = 0; (yyval.value).isInt = false; (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER;
        CSSParserString& str = (yyvsp[(1) - (1)].string);
        if (equalIgnoringCase(static_cast<const String&>(str), "from"))
            (yyval.value).fValue = 0;
        else if (equalIgnoringCase(static_cast<const String&>(str), "to"))
            (yyval.value).fValue = 100;
        else
            YYERROR;
    ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 751 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
      (yyval.rule) = 0;
    ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 754 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
      (yyval.rule) = 0;
    ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 761 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createFontFaceRule();
    ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 764 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
      (yyval.rule) = 0;
    ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 767 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
      (yyval.rule) = 0;
    ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 773 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.relation) = CSSSelector::DirectAdjacent; ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 774 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.relation) = CSSSelector::IndirectAdjacent; ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 775 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.relation) = CSSSelector::Child; ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 779 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.integer) = -1; ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 780 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.integer) = 1; ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 784 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = static_cast<CSSParser*>(parser)->createStyleRule((yyvsp[(1) - (5)].selector));
    ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 790 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (1)].selector);
    ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 793 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        if ((yyvsp[(1) - (4)].selector) && (yyvsp[(4) - (4)].selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selector) = (yyvsp[(1) - (4)].selector);
            (yyval.selector)->append(p->sinkFloatingSelector((yyvsp[(4) - (4)].selector)));
        } else
            (yyval.selector) = 0;
    ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 801 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = 0;
    ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 807 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (2)].selector);
    ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 813 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (1)].selector);
    ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 817 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (1)].selector);
    ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 821 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 144:

/* Line 1455 of yacc.c  */
#line 836 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 145:

/* Line 1455 of yacc.c  */
#line 856 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = 0;
    ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 862 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.string).characters = 0; (yyval.string).length = 0; ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 863 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { static UChar star = '*'; (yyval.string).characters = &star; (yyval.string).length = 1; ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 864 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.string) = (yyvsp[(1) - (2)].string); ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 868 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_tag = QualifiedName(nullAtom, (yyvsp[(1) - (1)].string), p->m_defaultNamespace);
    ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 873 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(2) - (2)].selector);
        if ((yyval.selector)) {
            CSSParser* p = static_cast<CSSParser*>(parser);
            (yyval.selector)->m_tag = QualifiedName(nullAtom, (yyvsp[(1) - (2)].string), p->m_defaultNamespace);
        }
    ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 880 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (1)].selector);
        CSSParser* p = static_cast<CSSParser*>(parser);
        if ((yyval.selector) && p->m_defaultNamespace != starAtom)
            (yyval.selector)->m_tag = QualifiedName(nullAtom, starAtom, p->m_defaultNamespace);
    ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 886 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 153:

/* Line 1455 of yacc.c  */
#line 896 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 154:

/* Line 1455 of yacc.c  */
#line 908 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 155:

/* Line 1455 of yacc.c  */
#line 921 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        CSSParserString& str = (yyvsp[(1) - (1)].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        Document* doc = p->document();
        if (doc && doc->isHTMLDocument())
            str.lower();
        (yyval.string) = str;
    ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 929 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        static UChar star = '*';
        (yyval.string).characters = &star;
        (yyval.string).length = 1;
    ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 937 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = (yyvsp[(1) - (1)].selector);
    ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 940 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 159:

/* Line 1455 of yacc.c  */
#line 953 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = 0;
    ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 959 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 161:

/* Line 1455 of yacc.c  */
#line 968 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 165:

/* Line 1455 of yacc.c  */
#line 987 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 166:

/* Line 1455 of yacc.c  */
#line 999 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        CSSParserString& str = (yyvsp[(1) - (2)].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        Document* doc = p->document();
        if (doc && doc->isHTMLDocument())
            str.lower();
        (yyval.string) = str;
    ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1010 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_attr = QualifiedName(nullAtom, (yyvsp[(3) - (4)].string), nullAtom);
        (yyval.selector)->m_match = CSSSelector::Set;
    ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1015 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.selector) = static_cast<CSSParser*>(parser)->createFloatingSelector();
        (yyval.selector)->m_attr = QualifiedName(nullAtom, (yyvsp[(3) - (8)].string), nullAtom);
        (yyval.selector)->m_match = (CSSSelector::Match)(yyvsp[(4) - (8)].integer);
        (yyval.selector)->m_value = (yyvsp[(6) - (8)].string);
    ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1021 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        AtomicString namespacePrefix = (yyvsp[(3) - (5)].string);
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.selector) = p->createFloatingSelector();
        (yyval.selector)->m_attr = QualifiedName(namespacePrefix, (yyvsp[(4) - (5)].string),
                                   p->m_styleSheet->determineNamespace(namespacePrefix));
        (yyval.selector)->m_match = CSSSelector::Set;
    ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1029 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 171:

/* Line 1455 of yacc.c  */
#line 1041 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::Exact;
    ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1044 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::List;
    ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1047 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::Hyphen;
    ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1050 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::Begin;
    ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1053 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::End;
    ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1056 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = CSSSelector::Contain;
    ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1067 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 180:

/* Line 1455 of yacc.c  */
#line 1092 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 181:

/* Line 1455 of yacc.c  */
#line 1107 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 182:

/* Line 1455 of yacc.c  */
#line 1125 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 183:

/* Line 1455 of yacc.c  */
#line 1143 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 184:

/* Line 1455 of yacc.c  */
#line 1162 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 185:

/* Line 1455 of yacc.c  */
#line 1177 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (1)].boolean);
    ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 1180 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (2)].boolean);
        if ( (yyvsp[(2) - (2)].boolean) )
            (yyval.boolean) = (yyvsp[(2) - (2)].boolean);
    ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 1185 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (1)].boolean);
    ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 1188 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 1191 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 1194 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (2)].boolean);
    ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 1197 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (2)].boolean);
    ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 1203 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (3)].boolean);
    ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1206 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1209 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1212 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1215 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (4)].boolean);
        if ((yyvsp[(2) - (4)].boolean))
            (yyval.boolean) = (yyvsp[(2) - (4)].boolean);
    ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1220 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (4)].boolean);
    ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1223 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = (yyvsp[(1) - (6)].boolean);
    ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1229 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 200:

/* Line 1455 of yacc.c  */
#line 1243 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        p->m_valueList = new CSSParserValueList;
        p->m_valueList->addValue(p->sinkFloatingValue((yyvsp[(1) - (2)].value)));
        int oldParsedProperties = p->m_numParsedProperties;
        (yyval.boolean) = p->parseValue(CSSPropertyWebkitVariableDeclarationBlock, false);
        if (!(yyval.boolean))
            p->rollbackLastProperties(p->m_numParsedProperties - oldParsedProperties);
        delete p->m_valueList;
        p->m_valueList = 0;
    ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1255 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.boolean) = false;
    ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1259 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        /* The default movable type template has letter-spacing: .none;  Handle this by looking for
        error tokens at the start of an expr, recover the expr and then treat as an error, cleaning
        up and deleting the shifted expr.  */
        (yyval.boolean) = false;
    ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1266 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        /* When we encounter something like p {color: red !important fail;} we should drop the declaration */
        (yyval.boolean) = false;
    ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1271 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        /* Handle this case: div { text-align: center; !important } Just reduce away the stray !important. */
        (yyval.boolean) = false;
    ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1276 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        /* div { font-family: } Just reduce away this property with no value. */
        (yyval.boolean) = false;
    ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 1281 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        /* if we come across rules with invalid values like this case: p { weight: *; }, just discard the rule */
        (yyval.boolean) = false;
    ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1286 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        /* if we come across: div { color{;color:maroon} }, ignore everything within curly brackets */
        (yyval.boolean) = false;
    ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1293 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.integer) = cssPropertyID((yyvsp[(1) - (2)].string));
    ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 1299 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.boolean) = true; ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1300 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.boolean) = false; ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1304 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        CSSParser* p = static_cast<CSSParser*>(parser);
        (yyval.valueList) = p->createFloatingValueList();
        (yyval.valueList)->addValue(p->sinkFloatingValue((yyvsp[(1) - (1)].value)));
    ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1309 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 213:

/* Line 1455 of yacc.c  */
#line 1323 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.valueList) = 0;
    ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1329 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.character) = '/';
    ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1332 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.character) = ',';
    ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1335 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.character) = 0;
  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1341 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value) = (yyvsp[(1) - (1)].value); ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1342 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value) = (yyvsp[(2) - (2)].value); (yyval.value).fValue *= (yyvsp[(1) - (2)].integer); ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1343 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_STRING; ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1344 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
      (yyval.value).id = cssValueKeywordID((yyvsp[(1) - (2)].string));
      (yyval.value).unit = CSSPrimitiveValue::CSS_IDENT;
      (yyval.value).string = (yyvsp[(1) - (2)].string);
  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1350 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_DIMENSION ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1351 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(2) - (3)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_DIMENSION ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1352 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_URI; ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1353 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (2)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_UNICODE_RANGE ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1354 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = (yyvsp[(1) - (1)].string); (yyval.value).unit = CSSPrimitiveValue::CSS_PARSER_HEXCOLOR; ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1355 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).string = CSSParserString(); (yyval.value).unit = CSSPrimitiveValue::CSS_PARSER_HEXCOLOR; ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1357 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
      (yyval.value) = (yyvsp[(1) - (1)].value);
  ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1360 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
      (yyval.value) = (yyvsp[(1) - (2)].value);
  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1363 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1367 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).isInt = true; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER; ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1368 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).isInt = false; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_NUMBER; ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1369 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_PERCENTAGE; ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1370 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_PX; ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1371 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_CM; ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1372 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_MM; ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1373 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_IN; ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1374 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_PT; ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1375 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_PC; ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1376 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_DEG; ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1377 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_RAD; ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1378 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_GRAD; ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1379 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_MS; ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1380 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_S; ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1381 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_HZ; ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1382 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_KHZ; ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1383 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_EMS; ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1384 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSParserValue::Q_EMS; ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1385 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.value).id = 0; (yyval.value).fValue = (yyvsp[(1) - (2)].number); (yyval.value).unit = CSSPrimitiveValue::CSS_EXS; ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1389 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
      (yyval.value).id = 0;
      (yyval.value).string = (yyvsp[(1) - (1)].string);
      (yyval.value).unit = CSSPrimitiveValue::CSS_PARSER_VARIABLE_FUNCTION_SYNTAX;
  ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1394 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
      (yyval.value).id = 0;
      (yyval.value).string = (yyvsp[(2) - (3)].string);
      (yyval.value).unit = CSSPrimitiveValue::CSS_PARSER_VARIABLE_EQUALS_SYNTAX;
  ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1399 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
      (yyval.value).id = 0;
      (yyval.value).string = (yyvsp[(2) - (2)].string);
      (yyval.value).unit = CSSPrimitiveValue::CSS_PARSER_VARIABLE_DOLLAR_SYNTAX;
  ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1407 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 253:

/* Line 1455 of yacc.c  */
#line 1416 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
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

  case 254:

/* Line 1455 of yacc.c  */
#line 1432 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.string) = (yyvsp[(1) - (2)].string); ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1433 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    { (yyval.string) = (yyvsp[(1) - (2)].string); ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1440 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1443 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1449 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1452 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 1463 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 1469 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 1475 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"
    {
        (yyval.rule) = 0;
    ;}
    break;



/* Line 1455 of yacc.c  */
#line 4255 "CSSGrammar.cpp"
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
      /* If just tried and failed to reuse lookahead token after an
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

  /* Else will try to reuse lookahead token after shifting the error
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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
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



/* Line 1675 of yacc.c  */
#line 1502 "/cygdrive/e/ISEEBR~1/WebCore/css/CSSGrammar.y"


