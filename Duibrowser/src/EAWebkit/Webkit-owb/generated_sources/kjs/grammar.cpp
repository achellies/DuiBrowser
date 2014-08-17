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
#define YYLSP_NEEDED 1

/* Substitute the variable and function names.  */
#define yyparse kjsyyparse
#define yylex   kjsyylex
#define yyerror kjsyyerror
#define yylval  kjsyylval
#define yychar  kjsyychar
#define yydebug kjsyydebug
#define yynerrs kjsyynerrs
#define yylloc kjsyylloc

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NULLTOKEN = 258,
     TRUETOKEN = 259,
     FALSETOKEN = 260,
     BREAK = 261,
     CASE = 262,
     DEFAULT = 263,
     FOR = 264,
     NEW = 265,
     VAR = 266,
     CONSTTOKEN = 267,
     CONTINUE = 268,
     FUNCTION = 269,
     RETURN = 270,
     VOIDTOKEN = 271,
     DELETETOKEN = 272,
     IF = 273,
     THISTOKEN = 274,
     DO = 275,
     WHILE = 276,
     INTOKEN = 277,
     INSTANCEOF = 278,
     TYPEOF = 279,
     SWITCH = 280,
     WITH = 281,
     RESERVED = 282,
     THROW = 283,
     TRY = 284,
     CATCH = 285,
     FINALLY = 286,
     DEBUGGER = 287,
     IF_WITHOUT_ELSE = 288,
     ELSE = 289,
     EQEQ = 290,
     NE = 291,
     STREQ = 292,
     STRNEQ = 293,
     LE = 294,
     GE = 295,
     OR = 296,
     AND = 297,
     PLUSPLUS = 298,
     MINUSMINUS = 299,
     LSHIFT = 300,
     RSHIFT = 301,
     URSHIFT = 302,
     PLUSEQUAL = 303,
     MINUSEQUAL = 304,
     MULTEQUAL = 305,
     DIVEQUAL = 306,
     LSHIFTEQUAL = 307,
     RSHIFTEQUAL = 308,
     URSHIFTEQUAL = 309,
     ANDEQUAL = 310,
     MODEQUAL = 311,
     XOREQUAL = 312,
     OREQUAL = 313,
     OPENBRACE = 314,
     CLOSEBRACE = 315,
     NUMBER = 316,
     STRING = 317,
     IDENT = 318,
     AUTOPLUSPLUS = 319,
     AUTOMINUSMINUS = 320
   };
#endif
/* Tokens.  */
#define NULLTOKEN 258
#define TRUETOKEN 259
#define FALSETOKEN 260
#define BREAK 261
#define CASE 262
#define DEFAULT 263
#define FOR 264
#define NEW 265
#define VAR 266
#define CONSTTOKEN 267
#define CONTINUE 268
#define FUNCTION 269
#define RETURN 270
#define VOIDTOKEN 271
#define DELETETOKEN 272
#define IF 273
#define THISTOKEN 274
#define DO 275
#define WHILE 276
#define INTOKEN 277
#define INSTANCEOF 278
#define TYPEOF 279
#define SWITCH 280
#define WITH 281
#define RESERVED 282
#define THROW 283
#define TRY 284
#define CATCH 285
#define FINALLY 286
#define DEBUGGER 287
#define IF_WITHOUT_ELSE 288
#define ELSE 289
#define EQEQ 290
#define NE 291
#define STREQ 292
#define STRNEQ 293
#define LE 294
#define GE 295
#define OR 296
#define AND 297
#define PLUSPLUS 298
#define MINUSMINUS 299
#define LSHIFT 300
#define RSHIFT 301
#define URSHIFT 302
#define PLUSEQUAL 303
#define MINUSEQUAL 304
#define MULTEQUAL 305
#define DIVEQUAL 306
#define LSHIFTEQUAL 307
#define RSHIFTEQUAL 308
#define URSHIFTEQUAL 309
#define ANDEQUAL 310
#define MODEQUAL 311
#define XOREQUAL 312
#define OREQUAL 313
#define OPENBRACE 314
#define CLOSEBRACE 315
#define NUMBER 316
#define STRING 317
#define IDENT 318
#define AUTOPLUSPLUS 319
#define AUTOMINUSMINUS 320




/* Copy the first part of user declarations.  */
//#line 3 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"


/*
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2006, 2007 Apple Inc. All rights reserved.
 *  Copyright (C) 2007 Eric Seidel <eric@webkit.org>
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

#include <string.h>
#include <stdlib.h>
#include "JSValue.h"
#include "JSObject.h"
#include "nodes.h"
#include "lexer.h"
#include "JSString.h"
#include "JSGlobalData.h"
#include "CommonIdentifiers.h"
#include "NodeInfo.h"
#include "Parser.h"
#include <wtf/MathExtras.h>

#define YYMAXDEPTH 10000
#define YYENABLE_NLS 0

/* default values for bison */
#define YYDEBUG 0 // Set to 1 to debug a parse error.
#define kjsyydebug 0 // Set to 1 to debug a parse error.
#if !PLATFORM(DARWIN)
    // avoid triggering warnings in older bison
#define YYERROR_VERBOSE
#endif

int kjsyylex(void* lvalp, void* llocp, void* globalPtr);
int kjsyyerror(const char*);
static inline bool allowAutomaticSemicolon(KJS::Lexer&, int);

#define GLOBAL_DATA static_cast<JSGlobalData*>(globalPtr)
#define LEXER (GLOBAL_DATA->lexer)

#define AUTO_SEMICOLON do { if (!allowAutomaticSemicolon(*LEXER, yychar)) YYABORT; } while (0)
#define DBG(l, s, e) (l)->setLoc((s).first_line, (e).last_line)

using namespace KJS;
using namespace std;

static ExpressionNode* makeAssignNode(void*, ExpressionNode* loc, Operator, ExpressionNode* expr, bool locHasAssignments, bool exprHasAssignments);
static ExpressionNode* makePrefixNode(void*, ExpressionNode* expr, Operator);
static ExpressionNode* makePostfixNode(void*, ExpressionNode* expr, Operator);
static PropertyNode* makeGetterOrSetterPropertyNode(void*, const Identifier &getOrSet, const Identifier& name, ParameterNode*, FunctionBodyNode*, const SourceRange&);
static ExpressionNodeInfo makeFunctionCallNode(void*, ExpressionNodeInfo func, ArgumentsNodeInfo);
static ExpressionNode* makeTypeOfNode(void*, ExpressionNode*);
static ExpressionNode* makeDeleteNode(void*, ExpressionNode*);
static ExpressionNode* makeNegateNode(void*, ExpressionNode*);
static NumberNode* makeNumberNode(void*, double);
static StatementNode* makeVarStatementNode(void*, ExpressionNode*);
static ExpressionNode* combineVarInitializers(void*, ExpressionNode* list, AssignResolveNode* init);


#if COMPILER(MSVC)

#pragma warning(disable: 4065)
#pragma warning(disable: 4244)
#pragma warning(disable: 4702)

// At least some of the time, the declarations of malloc and free that bison
// generates are causing warnings. A way to avoid this is to explicitly define
// the macros so that bison doesn't try to declare malloc and free.
#define YYMALLOC malloc
#define YYFREE free

#endif

#define YYPARSE_PARAM globalPtr
#define YYLEX_PARAM globalPtr

template <typename T> NodeDeclarationInfo<T> createNodeDeclarationInfo(T node, ParserRefCountedData<DeclarationStacks::VarStack>* varDecls, 
                                                                       ParserRefCountedData<DeclarationStacks::FunctionStack>* funcDecls,
                                                                       FeatureInfo info) 
{
    ASSERT((info & ~(EvalFeature | ClosureFeature | AssignFeature)) == 0);
    NodeDeclarationInfo<T> result = {node, varDecls, funcDecls, info};
    return result;
}

template <typename T> NodeFeatureInfo<T> createNodeFeatureInfo(T node, FeatureInfo info) 
{
    ASSERT((info & ~(EvalFeature | ClosureFeature | AssignFeature)) == 0);
    NodeFeatureInfo<T> result = {node, info};
    return result;
}

template <typename T> T mergeDeclarationLists(T decls1, T decls2) 
{
    // decls1 or both are null
    if (!decls1)
        return decls2;
    // only decls1 is non-null
    if (!decls2)
        return decls1;

    // Both are non-null
    decls1->data.append(decls2->data);
    
    // We manually release the declaration lists to avoid accumulating many many
    // unused heap allocated vectors
    decls2->ref();
    decls2->deref();
    return decls1;
}

static void appendToVarDeclarationList(void* globalPtr, ParserRefCountedData<DeclarationStacks::VarStack>*& varDecls, const Identifier& ident, unsigned attrs)
{
    if (!varDecls)
        varDecls = new ParserRefCountedData<DeclarationStacks::VarStack>(GLOBAL_DATA);

    varDecls->data.append(make_pair(ident, attrs));

}

static inline void appendToVarDeclarationList(void* globalPtr, ParserRefCountedData<DeclarationStacks::VarStack>*& varDecls, ConstDeclNode* decl)
{
    unsigned attrs = DeclarationStacks::IsConstant;
    if (decl->m_init)
        attrs |= DeclarationStacks::HasInitializer;        
    appendToVarDeclarationList(globalPtr, varDecls, decl->m_ident, attrs);
}



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
//#line 149 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
{
    int                 intValue;
    double              doubleValue;
    UString*            string;
    Identifier*         ident;

    // expression subtrees
    ExpressionNodeInfo  expressionNode;
    FuncDeclNode*       funcDeclNode;
    PropertyNodeInfo    propertyNode;
    ArgumentsNodeInfo   argumentsNode;
    ConstDeclNodeInfo   constDeclNode;
    CaseBlockNodeInfo   caseBlockNode;
    CaseClauseNodeInfo  caseClauseNode;
    FuncExprNodeInfo    funcExprNode;

    // statement nodes
    StatementNodeInfo   statementNode;
    FunctionBodyNode*   functionBodyNode;
    ProgramNode*        programNode;

    SourceElementsInfo  sourceElements;
    PropertyListInfo    propertyList;
    ArgumentListInfo    argumentList;
    VarDeclListInfo     varDeclList;
    ConstDeclListInfo   constDeclList;
    ClauseListInfo      clauseList;
    ElementListInfo     elementList;
    ParameterList       parameterList;

    Operator            op;
}
/* Line 187 of yacc.c.  */
//#line 413 "grammar.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
//#line 438 "grammar.tab.c"

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
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  207
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1295

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  88
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  99
/* YYNRULES -- Number of rules.  */
#define YYNRULES  301
/* YYNRULES -- Number of states.  */
#define YYNSTATES  544

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   320

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    77,     2,     2,     2,    79,    82,     2,
      68,    69,    78,    74,    70,    75,    73,    66,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    67,    87,
      80,    86,    81,    85,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    71,     2,    72,    83,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    84,     2,    76,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    13,    15,    17,
      21,    25,    29,    37,    46,    48,    52,    54,    57,    61,
      66,    68,    70,    72,    74,    78,    82,    86,    92,    95,
     100,   101,   103,   105,   108,   110,   112,   117,   121,   125,
     127,   132,   136,   140,   142,   145,   147,   150,   153,   156,
     161,   165,   168,   171,   176,   180,   183,   187,   189,   193,
     195,   197,   199,   201,   203,   206,   209,   211,   214,   217,
     220,   223,   226,   229,   232,   235,   238,   241,   244,   247,
     250,   252,   254,   256,   258,   260,   264,   268,   272,   274,
     278,   282,   286,   288,   292,   296,   298,   302,   306,   308,
     312,   316,   320,   322,   326,   330,   334,   336,   340,   344,
     348,   352,   356,   360,   362,   366,   370,   374,   378,   382,
     384,   388,   392,   396,   400,   404,   408,   410,   414,   418,
     422,   426,   428,   432,   436,   440,   444,   446,   450,   454,
     458,   462,   464,   468,   470,   474,   476,   480,   482,   486,
     488,   492,   494,   498,   500,   504,   506,   510,   512,   516,
     518,   522,   524,   528,   530,   534,   536,   540,   542,   546,
     548,   552,   554,   560,   562,   568,   570,   576,   578,   582,
     584,   588,   590,   594,   596,   598,   600,   602,   604,   606,
     608,   610,   612,   614,   616,   618,   620,   624,   626,   630,
     632,   636,   638,   640,   642,   644,   646,   648,   650,   652,
     654,   656,   658,   660,   662,   664,   666,   668,   671,   675,
     679,   683,   685,   688,   692,   697,   699,   702,   706,   711,
     715,   719,   721,   725,   727,   730,   733,   736,   738,   741,
     744,   750,   758,   766,   774,   780,   790,   801,   809,   818,
     828,   829,   831,   832,   834,   837,   840,   844,   848,   851,
     854,   858,   862,   865,   868,   872,   876,   882,   888,   892,
     898,   899,   901,   903,   906,   910,   915,   918,   922,   926,
     930,   934,   939,   947,   957,   960,   963,   971,   980,   987,
     995,  1003,  1012,  1014,  1018,  1019,  1021,  1022,  1024,  1026,
    1029,  1031
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     184,     0,    -1,     3,    -1,     4,    -1,     5,    -1,    61,
      -1,    62,    -1,    66,    -1,    51,    -1,    63,    67,   143,
      -1,    62,    67,   143,    -1,    61,    67,   143,    -1,    63,
      63,    68,    69,    59,   183,    60,    -1,    63,    63,    68,
     182,    69,    59,   183,    60,    -1,    90,    -1,    91,    70,
      90,    -1,    93,    -1,    59,    60,    -1,    59,    91,    60,
      -1,    59,    91,    70,    60,    -1,    19,    -1,    89,    -1,
      94,    -1,    63,    -1,    68,   147,    69,    -1,    71,    96,
      72,    -1,    71,    95,    72,    -1,    71,    95,    70,    96,
      72,    -1,    96,   143,    -1,    95,    70,    96,   143,    -1,
      -1,    97,    -1,    70,    -1,    97,    70,    -1,    92,    -1,
     181,    -1,    98,    71,   147,    72,    -1,    98,    73,    63,
      -1,    10,    98,   104,    -1,    93,    -1,    99,    71,   147,
      72,    -1,    99,    73,    63,    -1,    10,    98,   104,    -1,
      98,    -1,    10,   100,    -1,    99,    -1,    10,   100,    -1,
      98,   104,    -1,   102,   104,    -1,   102,    71,   147,    72,
      -1,   102,    73,    63,    -1,    99,   104,    -1,   103,   104,
      -1,   103,    71,   147,    72,    -1,   103,    73,    63,    -1,
      68,    69,    -1,    68,   105,    69,    -1,   143,    -1,   105,
      70,   143,    -1,   100,    -1,   102,    -1,   101,    -1,   103,
      -1,   106,    -1,   106,    43,    -1,   106,    44,    -1,   107,
      -1,   107,    43,    -1,   107,    44,    -1,    17,   111,    -1,
      16,   111,    -1,    24,   111,    -1,    43,   111,    -1,    64,
     111,    -1,    44,   111,    -1,    65,   111,    -1,    74,   111,
      -1,    75,   111,    -1,    76,   111,    -1,    77,   111,    -1,
     108,    -1,   110,    -1,   109,    -1,   110,    -1,   111,    -1,
     113,    78,   111,    -1,   113,    66,   111,    -1,   113,    79,
     111,    -1,   112,    -1,   114,    78,   111,    -1,   114,    66,
     111,    -1,   114,    79,   111,    -1,   113,    -1,   115,    74,
     113,    -1,   115,    75,   113,    -1,   114,    -1,   116,    74,
     113,    -1,   116,    75,   113,    -1,   115,    -1,   117,    45,
     115,    -1,   117,    46,   115,    -1,   117,    47,   115,    -1,
     116,    -1,   118,    45,   115,    -1,   118,    46,   115,    -1,
     118,    47,   115,    -1,   117,    -1,   119,    80,   117,    -1,
     119,    81,   117,    -1,   119,    39,   117,    -1,   119,    40,
     117,    -1,   119,    23,   117,    -1,   119,    22,   117,    -1,
     117,    -1,   120,    80,   117,    -1,   120,    81,   117,    -1,
     120,    39,   117,    -1,   120,    40,   117,    -1,   120,    23,
     117,    -1,   118,    -1,   121,    80,   117,    -1,   121,    81,
     117,    -1,   121,    39,   117,    -1,   121,    40,   117,    -1,
     121,    23,   117,    -1,   121,    22,   117,    -1,   119,    -1,
     122,    35,   119,    -1,   122,    36,   119,    -1,   122,    37,
     119,    -1,   122,    38,   119,    -1,   120,    -1,   123,    35,
     120,    -1,   123,    36,   120,    -1,   123,    37,   120,    -1,
     123,    38,   120,    -1,   121,    -1,   124,    35,   119,    -1,
     124,    36,   119,    -1,   124,    37,   119,    -1,   124,    38,
     119,    -1,   122,    -1,   125,    82,   122,    -1,   123,    -1,
     126,    82,   123,    -1,   124,    -1,   127,    82,   122,    -1,
     125,    -1,   128,    83,   125,    -1,   126,    -1,   129,    83,
     126,    -1,   127,    -1,   130,    83,   125,    -1,   128,    -1,
     131,    84,   128,    -1,   129,    -1,   132,    84,   129,    -1,
     130,    -1,   133,    84,   128,    -1,   131,    -1,   134,    42,
     131,    -1,   132,    -1,   135,    42,   132,    -1,   133,    -1,
     136,    42,   131,    -1,   134,    -1,   137,    41,   134,    -1,
     135,    -1,   138,    41,   135,    -1,   136,    -1,   139,    41,
     134,    -1,   137,    -1,   137,    85,   143,    67,   143,    -1,
     138,    -1,   138,    85,   144,    67,   144,    -1,   139,    -1,
     139,    85,   143,    67,   143,    -1,   140,    -1,   106,   146,
     143,    -1,   141,    -1,   106,   146,   144,    -1,   142,    -1,
     107,   146,   143,    -1,    86,    -1,    48,    -1,    49,    -1,
      50,    -1,    51,    -1,    52,    -1,    53,    -1,    54,    -1,
      55,    -1,    57,    -1,    58,    -1,    56,    -1,   143,    -1,
     147,    70,   143,    -1,   144,    -1,   148,    70,   144,    -1,
     145,    -1,   149,    70,   143,    -1,   151,    -1,   152,    -1,
     155,    -1,   160,    -1,   161,    -1,   162,    -1,   163,    -1,
     166,    -1,   167,    -1,   168,    -1,   169,    -1,   170,    -1,
     176,    -1,   177,    -1,   178,    -1,   179,    -1,    59,    60,
      -1,    59,   185,    60,    -1,    11,   153,    87,    -1,    11,
     153,     1,    -1,    63,    -1,    63,   158,    -1,   153,    70,
      63,    -1,   153,    70,    63,   158,    -1,    63,    -1,    63,
     159,    -1,   154,    70,    63,    -1,   154,    70,    63,   159,
      -1,    12,   156,    87,    -1,    12,   156,     1,    -1,   157,
      -1,   156,    70,   157,    -1,    63,    -1,    63,   158,    -1,
      86,   143,    -1,    86,   144,    -1,    87,    -1,   149,    87,
      -1,   149,     1,    -1,    18,    68,   147,    69,   150,    -1,
      18,    68,   147,    69,   150,    34,   150,    -1,    20,   150,
      21,    68,   147,    69,    87,    -1,    20,   150,    21,    68,
     147,    69,     1,    -1,    21,    68,   147,    69,   150,    -1,
       9,    68,   165,    87,   164,    87,   164,    69,   150,    -1,
       9,    68,    11,   154,    87,   164,    87,   164,    69,   150,
      -1,     9,    68,   106,    22,   147,    69,   150,    -1,     9,
      68,    11,    63,    22,   147,    69,   150,    -1,     9,    68,
      11,    63,   159,    22,   147,    69,   150,    -1,    -1,   147,
      -1,    -1,   148,    -1,    13,    87,    -1,    13,     1,    -1,
      13,    63,    87,    -1,    13,    63,     1,    -1,     6,    87,
      -1,     6,     1,    -1,     6,    63,    87,    -1,     6,    63,
       1,    -1,    15,    87,    -1,    15,     1,    -1,    15,   147,
      87,    -1,    15,   147,     1,    -1,    26,    68,   147,    69,
     150,    -1,    25,    68,   147,    69,   171,    -1,    59,   172,
      60,    -1,    59,   172,   175,   172,    60,    -1,    -1,   173,
      -1,   174,    -1,   173,   174,    -1,     7,   147,    67,    -1,
       7,   147,    67,   185,    -1,     8,    67,    -1,     8,    67,
     185,    -1,    63,    67,   150,    -1,    28,   147,    87,    -1,
      28,   147,     1,    -1,    29,   151,    31,   151,    -1,    29,
     151,    30,    68,    63,    69,   151,    -1,    29,   151,    30,
      68,    63,    69,   151,    31,   151,    -1,    32,    87,    -1,
      32,     1,    -1,    14,    63,    68,    69,    59,   183,    60,
      -1,    14,    63,    68,   182,    69,    59,   183,    60,    -1,
      14,    68,    69,    59,   183,    60,    -1,    14,    68,   182,
      69,    59,   183,    60,    -1,    14,    63,    68,    69,    59,
     183,    60,    -1,    14,    63,    68,   182,    69,    59,   183,
      60,    -1,    63,    -1,   182,    70,    63,    -1,    -1,   185,
      -1,    -1,   185,    -1,   186,    -1,   185,   186,    -1,   180,
      -1,   150,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   281,   281,   282,   283,   284,   285,   286,   292,   301,
     302,   303,   304,   305,   310,   313,   319,   320,   321,   323,
     327,   328,   329,   330,   331,   335,   336,   337,   341,   344,
     351,   352,   356,   357,   361,   362,   363,   364,   365,   369,
     370,   371,   372,   376,   377,   381,   382,   386,   387,   388,
     389,   393,   394,   395,   396,   400,   401,   405,   408,   414,
     415,   419,   420,   424,   425,   426,   430,   431,   432,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,   446,
     449,   450,   454,   455,   459,   460,   461,   462,   466,   467,
     469,   471,   476,   477,   478,   482,   483,   485,   490,   491,
     492,   493,   497,   498,   499,   500,   504,   505,   506,   507,
     508,   509,   510,   514,   515,   516,   517,   518,   519,   524,
     525,   526,   527,   528,   529,   531,   535,   536,   537,   538,
     539,   543,   544,   546,   548,   550,   555,   556,   558,   559,
     561,   566,   567,   571,   572,   577,   578,   582,   583,   587,
     588,   593,   594,   599,   600,   604,   605,   610,   611,   616,
     617,   621,   622,   627,   628,   633,   634,   638,   639,   644,
     645,   649,   650,   655,   656,   661,   662,   667,   668,   673,
     674,   679,   680,   685,   686,   687,   688,   689,   690,   691,
     692,   693,   694,   695,   696,   700,   701,   705,   706,   710,
     711,   715,   716,   717,   718,   719,   720,   721,   722,   723,
     724,   725,   726,   727,   728,   729,   730,   734,   736,   741,
     743,   749,   755,   761,   768,   778,   784,   790,   797,   807,
     809,   815,   822,   833,   834,   838,   842,   846,   850,   852,
     857,   860,   868,   870,   872,   874,   879,   885,   894,   899,
     908,   909,   913,   914,   918,   920,   922,   924,   929,   930,
     931,   932,   936,   937,   938,   939,   943,   949,   955,   956,
     964,   965,   969,   974,   983,   984,   988,   989,   993,   998,
     999,  1003,  1008,  1013,  1022,  1024,  1029,  1030,  1035,  1036,
    1037,  1038,  1042,  1044,  1049,  1050,  1067,  1068,  1074,  1080,
    1088,  1089
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NULLTOKEN", "TRUETOKEN", "FALSETOKEN",
  "BREAK", "CASE", "DEFAULT", "FOR", "NEW", "VAR", "CONSTTOKEN",
  "CONTINUE", "FUNCTION", "RETURN", "VOIDTOKEN", "DELETETOKEN", "IF",
  "THISTOKEN", "DO", "WHILE", "INTOKEN", "INSTANCEOF", "TYPEOF", "SWITCH",
  "WITH", "RESERVED", "THROW", "TRY", "CATCH", "FINALLY", "DEBUGGER",
  "IF_WITHOUT_ELSE", "ELSE", "EQEQ", "NE", "STREQ", "STRNEQ", "LE", "GE",
  "OR", "AND", "PLUSPLUS", "MINUSMINUS", "LSHIFT", "RSHIFT", "URSHIFT",
  "PLUSEQUAL", "MINUSEQUAL", "MULTEQUAL", "DIVEQUAL", "LSHIFTEQUAL",
  "RSHIFTEQUAL", "URSHIFTEQUAL", "ANDEQUAL", "MODEQUAL", "XOREQUAL",
  "OREQUAL", "OPENBRACE", "CLOSEBRACE", "NUMBER", "STRING", "IDENT",
  "AUTOPLUSPLUS", "AUTOMINUSMINUS", "'/'", "':'", "'('", "')'", "','",
  "'['", "']'", "'.'", "'+'", "'-'", "'~'", "'!'", "'*'", "'%'", "'<'",
  "'>'", "'&'", "'^'", "'|'", "'?'", "'='", "';'", "$accept", "Literal",
  "Property", "PropertyList", "PrimaryExpr", "PrimaryExprNoBrace",
  "ArrayLiteral", "ElementList", "ElisionOpt", "Elision", "MemberExpr",
  "MemberExprNoBF", "NewExpr", "NewExprNoBF", "CallExpr", "CallExprNoBF",
  "Arguments", "ArgumentList", "LeftHandSideExpr", "LeftHandSideExprNoBF",
  "PostfixExpr", "PostfixExprNoBF", "UnaryExprCommon", "UnaryExpr",
  "UnaryExprNoBF", "MultiplicativeExpr", "MultiplicativeExprNoBF",
  "AdditiveExpr", "AdditiveExprNoBF", "ShiftExpr", "ShiftExprNoBF",
  "RelationalExpr", "RelationalExprNoIn", "RelationalExprNoBF",
  "EqualityExpr", "EqualityExprNoIn", "EqualityExprNoBF", "BitwiseANDExpr",
  "BitwiseANDExprNoIn", "BitwiseANDExprNoBF", "BitwiseXORExpr",
  "BitwiseXORExprNoIn", "BitwiseXORExprNoBF", "BitwiseORExpr",
  "BitwiseORExprNoIn", "BitwiseORExprNoBF", "LogicalANDExpr",
  "LogicalANDExprNoIn", "LogicalANDExprNoBF", "LogicalORExpr",
  "LogicalORExprNoIn", "LogicalORExprNoBF", "ConditionalExpr",
  "ConditionalExprNoIn", "ConditionalExprNoBF", "AssignmentExpr",
  "AssignmentExprNoIn", "AssignmentExprNoBF", "AssignmentOperator", "Expr",
  "ExprNoIn", "ExprNoBF", "Statement", "Block", "VariableStatement",
  "VariableDeclarationList", "VariableDeclarationListNoIn",
  "ConstStatement", "ConstDeclarationList", "ConstDeclaration",
  "Initializer", "InitializerNoIn", "EmptyStatement", "ExprStatement",
  "IfStatement", "IterationStatement", "ExprOpt", "ExprNoInOpt",
  "ContinueStatement", "BreakStatement", "ReturnStatement",
  "WithStatement", "SwitchStatement", "CaseBlock", "CaseClausesOpt",
  "CaseClauses", "CaseClause", "DefaultClause", "LabelledStatement",
  "ThrowStatement", "TryStatement", "DebuggerStatement",
  "FunctionDeclaration", "FunctionExpr", "FormalParameterList",
  "FunctionBody", "Program", "SourceElements", "SourceElement", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,    47,    58,    40,    41,
      44,    91,    93,    46,    43,    45,   126,    33,    42,    37,
      60,    62,    38,    94,   124,    63,    61,    59
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    88,    89,    89,    89,    89,    89,    89,    89,    90,
      90,    90,    90,    90,    91,    91,    92,    92,    92,    92,
      93,    93,    93,    93,    93,    94,    94,    94,    95,    95,
      96,    96,    97,    97,    98,    98,    98,    98,    98,    99,
      99,    99,    99,   100,   100,   101,   101,   102,   102,   102,
     102,   103,   103,   103,   103,   104,   104,   105,   105,   106,
     106,   107,   107,   108,   108,   108,   109,   109,   109,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     111,   111,   112,   112,   113,   113,   113,   113,   114,   114,
     114,   114,   115,   115,   115,   116,   116,   116,   117,   117,
     117,   117,   118,   118,   118,   118,   119,   119,   119,   119,
     119,   119,   119,   120,   120,   120,   120,   120,   120,   121,
     121,   121,   121,   121,   121,   121,   122,   122,   122,   122,
     122,   123,   123,   123,   123,   123,   124,   124,   124,   124,
     124,   125,   125,   126,   126,   127,   127,   128,   128,   129,
     129,   130,   130,   131,   131,   132,   132,   133,   133,   134,
     134,   135,   135,   136,   136,   137,   137,   138,   138,   139,
     139,   140,   140,   141,   141,   142,   142,   143,   143,   144,
     144,   145,   145,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   147,   147,   148,   148,   149,
     149,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   151,   151,   152,
     152,   153,   153,   153,   153,   154,   154,   154,   154,   155,
     155,   156,   156,   157,   157,   158,   159,   160,   161,   161,
     162,   162,   163,   163,   163,   163,   163,   163,   163,   163,
     164,   164,   165,   165,   166,   166,   166,   166,   167,   167,
     167,   167,   168,   168,   168,   168,   169,   170,   171,   171,
     172,   172,   173,   173,   174,   174,   175,   175,   176,   177,
     177,   178,   178,   178,   179,   179,   180,   180,   181,   181,
     181,   181,   182,   182,   183,   183,   184,   184,   185,   185,
     186,   186
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     7,     8,     1,     3,     1,     2,     3,     4,
       1,     1,     1,     1,     3,     3,     3,     5,     2,     4,
       0,     1,     1,     2,     1,     1,     4,     3,     3,     1,
       4,     3,     3,     1,     2,     1,     2,     2,     2,     4,
       3,     2,     2,     4,     3,     2,     3,     1,     3,     1,
       1,     1,     1,     1,     2,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       1,     1,     1,     1,     1,     3,     3,     3,     1,     3,
       3,     3,     1,     3,     3,     1,     3,     3,     1,     3,
       3,     3,     1,     3,     3,     3,     1,     3,     3,     3,
       3,     3,     3,     1,     3,     3,     3,     3,     3,     1,
       3,     3,     3,     3,     3,     3,     1,     3,     3,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     3,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     5,     1,     5,     1,     5,     1,     3,     1,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     3,     3,
       3,     1,     2,     3,     4,     1,     2,     3,     4,     3,
       3,     1,     3,     1,     2,     2,     2,     1,     2,     2,
       5,     7,     7,     7,     5,     9,    10,     7,     8,     9,
       0,     1,     0,     1,     2,     2,     3,     3,     2,     2,
       3,     3,     2,     2,     3,     3,     5,     5,     3,     5,
       0,     1,     1,     2,     3,     4,     2,     3,     3,     3,
       3,     4,     7,     9,     2,     2,     7,     8,     6,     7,
       7,     8,     1,     3,     0,     1,     0,     1,     1,     2,
       1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     296,     2,     3,     4,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    20,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     8,     0,     5,     6,
      23,     0,     0,     7,     0,    30,     0,     0,     0,     0,
     237,    21,    39,    22,    45,    61,    62,    66,    82,    83,
      88,    95,   102,   119,   136,   145,   151,   157,   163,   169,
     175,   181,   199,     0,   301,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   300,     0,   297,   298,   259,     0,   258,   252,     0,
       0,     0,    23,    34,    16,    43,    46,    35,   221,     0,
     233,     0,   231,   255,     0,   254,     0,   263,   262,    43,
      59,    60,    63,    80,    81,    84,    92,    98,   106,   126,
     141,   147,   153,   159,   165,   171,   177,   195,     0,    63,
      70,    69,     0,     0,     0,    71,     0,     0,     0,     0,
     285,   284,    72,    74,   217,     0,     0,    73,    75,     0,
      32,     0,     0,    31,    76,    77,    78,    79,     0,     0,
       0,    51,     0,     0,    52,    67,    68,   184,   185,   186,
     187,   188,   189,   190,   191,   194,   192,   193,   183,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   239,     0,   238,     1,   299,   261,
     260,     0,    63,   113,   131,   143,   149,   155,   161,   167,
     173,   179,   197,   253,     0,    43,    44,     0,     0,    17,
       0,     0,     0,    14,     0,     0,     0,    42,     0,   222,
     220,     0,   219,   234,   230,     0,   229,   257,   256,     0,
      47,     0,     0,    48,    64,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   265,     0,   264,     0,     0,     0,     0,     0,   280,
     279,     0,     0,   218,   278,    24,    30,    26,    25,    28,
      33,    55,     0,    57,     0,    41,     0,    54,   182,    90,
      89,    91,    96,    97,   103,   104,   105,   125,   124,   122,
     123,   120,   121,   137,   138,   139,   140,   146,   152,   158,
     164,   170,     0,   200,   225,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   250,    38,     0,   292,     0,     0,
       0,     0,     0,     0,    18,     0,     0,    37,   235,   223,
     232,     0,     0,     0,    50,   178,    86,    85,    87,    93,
      94,    99,   100,   101,   112,   111,   109,   110,   107,   108,
     127,   128,   129,   130,   142,   148,   154,   160,   166,     0,
     196,     0,     0,     0,     0,     0,     0,   281,     0,    56,
       0,    40,    53,     0,     0,     0,   226,     0,   250,     0,
      63,   180,   118,   116,   117,   114,   115,   132,   133,   134,
     135,   144,   150,   156,   162,   168,     0,   198,   251,     0,
       0,     0,   294,     0,     0,    11,    10,     0,     9,    19,
      15,    36,   224,   294,     0,    49,     0,   240,     0,   244,
     270,   267,   266,     0,    27,    29,    58,   176,     0,   236,
       0,   227,     0,     0,     0,   250,   294,     0,     0,   295,
     294,   293,     0,     0,     0,   294,   172,     0,     0,     0,
       0,   271,   272,     0,     0,     0,   228,   250,   247,   174,
       0,     0,   294,   288,     0,   294,     0,   286,     0,   241,
     243,   242,     0,     0,   268,   270,   273,   282,   248,     0,
       0,     0,   290,     0,   289,     0,   294,   287,   274,   276,
       0,     0,   249,     0,   245,   291,    12,     0,   275,   277,
     269,   283,   246,    13
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    41,   233,   234,    93,    94,    43,   151,   152,   153,
     109,    44,   110,    45,   111,    46,   161,   302,   129,    47,
     113,    48,   114,   115,    50,   116,    51,   117,    52,   118,
      53,   119,   214,    54,   120,   215,    55,   121,   216,    56,
     122,   217,    57,   123,   218,    58,   124,   219,    59,   125,
     220,    60,   126,   221,    61,   127,   222,    62,   337,   438,
     223,    63,    64,    65,    66,    99,   335,    67,   101,   102,
     239,   416,    68,    69,    70,    71,   439,   224,    72,    73,
      74,    75,    76,   461,   490,   491,   492,   515,    77,    78,
      79,    80,    81,    97,   359,   478,    82,   479,    84
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -369
static const yytype_int16 yypact[] =
{
     798,  -369,  -369,  -369,     5,   -53,   246,    -8,     4,     6,
      19,   561,  1174,  1174,    26,  -369,   877,    60,  1174,    74,
     102,  1174,   142,    13,  1174,  1174,  -369,   640,  -369,  -369,
       3,  1174,  1174,  -369,  1174,   123,  1174,  1174,  1174,  1174,
    -369,  -369,  -369,  -369,   134,  -369,   140,  1157,  -369,  -369,
    -369,    83,   221,   106,   156,   147,   127,   149,   150,   201,
     -33,  -369,  -369,    11,  -369,  -369,  -369,  -369,  -369,  -369,
    -369,  -369,  -369,  -369,  -369,  -369,  -369,  -369,  -369,  -369,
    -369,  -369,   259,   798,  -369,  -369,    33,  -369,   952,   246,
     -20,   206,  -369,  -369,  -369,   162,  -369,  -369,   178,    18,
     178,    31,  -369,  -369,    38,  -369,   230,  -369,  -369,   162,
    -369,   173,  1209,  -369,  -369,  -369,    94,   248,   226,   158,
     247,   228,   232,   244,   274,   -31,  -369,  -369,    32,   204,
    -369,  -369,  1174,   304,  1174,  -369,  1174,  1174,    44,   249,
    -369,  -369,  -369,  -369,  -369,   719,   877,  -369,  -369,   257,
    -369,   -48,   988,   262,  -369,  -369,  -369,  -369,  1063,  1174,
     303,  -369,  1174,   306,  -369,  -369,  -369,  -369,  -369,  -369,
    -369,  -369,  -369,  -369,  -369,  -369,  -369,  -369,  -369,  1174,
    1174,  1174,  1174,  1174,  1174,  1174,  1174,  1174,  1174,  1174,
    1174,  1174,  1174,  1174,  1174,  1174,  1174,  1174,  1174,  1174,
    1174,  1174,  1174,  1174,  -369,  1174,  -369,  -369,  -369,  -369,
    -369,   327,   545,   226,   181,   256,   310,   311,   309,   353,
      50,  -369,  -369,   326,   313,   162,  -369,   329,    54,  -369,
     331,   340,   -32,  -369,   -47,  1174,   345,  -369,  1174,  -369,
    -369,   347,  -369,  -369,  -369,     4,  -369,  -369,  -369,   105,
    -369,  1174,   348,  -369,  -369,  -369,  1174,  1174,  1174,  1174,
    1174,  1174,  1174,  1174,  1174,  1174,  1174,  1174,  1174,  1174,
    1174,  1174,  1174,  1174,  1174,  1174,  1174,  1174,  1174,  1174,
    1174,  -369,  1174,  -369,   268,   344,   270,   272,   276,  -369,
    -369,   346,   142,  -369,  -369,  -369,   123,  -369,  -369,  -369,
    -369,  -369,   278,  -369,   185,  -369,   205,  -369,  -369,  -369,
    -369,  -369,    94,    94,   248,   248,   248,   226,   226,   226,
     226,   226,   226,   158,   158,   158,   158,   247,   228,   232,
     244,   274,   349,  -369,   -11,   -45,  1174,  1174,  1174,  1174,
    1174,  1174,  1174,  1174,  1174,  1174,  1174,  1174,  1174,  1174,
    1174,  1174,  1174,  1174,  1174,  -369,   108,  -369,   354,   280,
    1174,  1174,   350,  1174,  -369,   258,   216,  -369,  -369,   178,
    -369,   360,   282,   241,  -369,  -369,  -369,  -369,  -369,    94,
      94,   248,   248,   248,   226,   226,   226,   226,   226,   226,
     158,   158,   158,   158,   247,   228,   232,   244,   274,   356,
    -369,   877,  1174,   877,   365,   877,   363,  -369,  1099,  -369,
    1174,  -369,  -369,  1174,  1174,  1174,   405,   366,  1174,   284,
    1209,  -369,   226,   226,   226,   226,   226,   181,   181,   181,
     181,   256,   310,   311,   309,   353,   361,  -369,   362,   343,
     375,   286,   798,   377,   374,  -369,  -369,   137,  -369,  -369,
    -369,  -369,  -369,   798,   379,  -369,  1174,   406,   288,  -369,
     432,  -369,  -369,   372,  -369,  -369,  -369,  -369,   293,  -369,
    1174,   357,   358,   877,  1174,  1174,   798,   385,   386,   798,
     798,  -369,   388,   295,   390,   798,  -369,   877,    39,  1174,
      56,   432,  -369,   142,   877,   319,  -369,  1174,  -369,  -369,
     382,   392,   798,  -369,   394,   798,   389,  -369,   395,  -369,
    -369,  -369,   148,   393,  -369,   432,  -369,   425,  -369,   877,
     396,   877,  -369,   401,  -369,   402,   798,  -369,   798,   798,
     404,   142,  -369,   877,  -369,  -369,  -369,   407,   798,   798,
    -369,  -369,  -369,  -369
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -369,  -369,   101,  -369,  -369,     0,  -369,  -369,   172,  -369,
      43,  -369,    45,  -369,  -369,  -369,   -37,  -369,     7,  -369,
    -369,  -369,    30,    72,  -369,   -97,  -369,  -126,  -369,    34,
    -369,  -117,   -10,  -369,  -160,   124,  -369,  -155,   122,  -369,
    -147,   129,  -369,  -145,   136,  -369,  -139,   133,  -369,  -369,
    -369,  -369,  -369,  -369,  -369,  -132,  -316,  -369,    15,    55,
    -369,  -369,     1,   -18,  -369,  -369,  -369,  -369,  -369,   245,
     -99,    20,  -369,  -369,  -369,  -369,  -368,  -369,  -369,  -369,
    -369,  -369,  -369,  -369,   -26,  -369,     8,  -369,  -369,  -369,
    -369,  -369,  -369,  -369,  -244,  -286,  -369,     2,   -80
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
      42,   243,    83,   208,   139,   372,    85,   103,   202,   164,
     279,   414,   204,   364,   140,    88,    42,   133,   112,   240,
     299,   421,   296,   365,   297,   417,   303,    42,   112,   145,
      49,   362,   244,   281,   209,   363,   436,   437,   327,   247,
     510,   112,   418,   227,   328,   289,    49,   308,   228,    95,
     472,    96,   203,   329,   280,    98,   330,    49,   237,   314,
     315,   316,   179,   331,   513,   208,   128,   100,    86,   104,
     146,   332,   250,   333,   253,   415,   138,   323,   324,   325,
     326,   205,   106,    42,   130,   131,   312,   313,   241,   149,
     135,   351,    87,   105,   132,   212,   142,   143,   206,   469,
     141,   245,   282,   147,   148,   242,   368,   500,   154,   155,
     156,   157,   441,    49,   282,   394,   514,   357,   246,   283,
     210,   395,   213,   358,   375,   248,   511,   256,   134,   520,
     396,   290,   225,   397,   226,   352,   381,   382,   383,   112,
     398,   112,   136,   112,   112,    42,    42,   294,   399,   180,
     400,   185,   186,   187,   390,   391,   392,   393,   499,   112,
     257,   181,   182,   379,   380,   112,   112,   484,   357,   112,
     137,   357,   258,   259,   371,    49,    49,   440,   188,   189,
     265,   266,   194,   195,   196,   197,   112,   284,   355,   286,
     501,   287,   288,   150,   504,   190,   191,   267,   268,   508,
     357,    27,   158,   483,   338,   159,   482,   160,   158,   198,
     112,   162,   112,   163,   304,   528,   523,   306,   282,   525,
     339,   340,   317,   318,   319,   320,   321,   322,   445,   446,
     158,   448,   199,   235,   200,   236,   192,   193,   269,   270,
     537,   158,   112,   201,   251,   112,   252,   254,   255,     1,
       2,     3,   309,   310,   311,   282,    89,   411,   112,   207,
      90,   341,   342,   112,   238,    15,   229,   230,   231,   232,
     452,   262,   263,   264,   407,   282,   465,   412,   466,   291,
     292,   467,   271,   272,   273,   274,   282,   112,   451,   112,
     366,   343,   344,   345,   346,   183,   184,    26,   249,   384,
     385,   386,   387,   388,   389,    91,   373,    28,    29,    92,
     275,   282,    33,   455,    34,   276,   278,    35,   449,   230,
     231,   232,   260,   261,   486,   285,   295,   282,   277,   376,
     377,   378,   300,   427,   428,   429,   430,   401,   282,   403,
     282,   404,   282,   112,   420,   405,   282,   409,   410,   443,
     444,   454,   444,   473,   282,   477,   444,   488,   282,   420,
     420,   112,   494,   282,   506,   444,   305,   112,   112,   307,
     112,   213,   422,   423,   424,   425,   426,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   519,   282,
     334,   419,   347,   349,   348,   350,   353,   356,   360,   208,
     354,    42,   457,    42,   459,    42,   462,   361,   367,   112,
     369,   374,   402,   442,   406,   112,   413,   112,   447,   453,
     112,   112,   420,   456,   460,   112,   463,   470,   474,   471,
     475,    49,   282,    49,   476,    49,   480,   481,   485,   489,
     487,   493,    42,   415,   502,   497,   503,   505,   526,   213,
     507,   521,   522,    42,   524,   527,   531,   458,   208,   208,
     529,   535,   536,   112,   540,   533,   450,   543,   408,   468,
     432,   431,    49,    42,   498,   517,    42,   112,   433,    42,
      42,   420,   112,    49,   435,    42,   434,    42,   509,   530,
     370,   496,     0,     0,    42,   518,   112,     0,     0,   516,
       0,     0,    42,    49,   112,    42,    49,     0,   213,    49,
      49,     0,     0,   541,     0,    49,     0,    49,     0,    42,
     532,    42,   534,     0,    49,   495,    42,     0,    42,    42,
     538,   539,    49,    42,   542,    49,     0,     0,    42,    42,
       0,     0,     0,     0,   512,     0,     0,     0,     0,    49,
       0,    49,     0,     0,     0,     0,    49,     0,    49,    49,
       0,     0,   107,    49,     1,     2,     3,   336,    49,    49,
       0,    89,     0,     0,     0,    90,     0,    12,    13,     0,
      15,     0,     0,     0,     0,    18,     0,     0,   254,   255,
       0,     0,     0,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,    24,    25,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,     0,     0,
      91,     0,    28,    29,    92,    31,    32,    33,     0,    34,
       0,   178,    35,     0,     0,    36,    37,    38,    39,     0,
       0,     0,     0,     1,     2,     3,     4,     0,   108,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,     0,     0,    18,    19,    20,     0,    21,    22,
       0,     0,    23,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    24,    25,     0,     0,     0,     0,     0,
       0,    26,     0,     0,     0,     0,     0,     0,     0,    27,
     144,    28,    29,    30,    31,    32,    33,     0,    34,     0,
       0,    35,     0,     0,    36,    37,    38,    39,     0,     0,
       0,     0,     1,     2,     3,     4,     0,    40,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,     0,     0,    18,    19,    20,     0,    21,    22,     0,
       0,    23,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    24,    25,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,     0,     0,    27,   293,
      28,    29,    30,    31,    32,    33,     0,    34,     0,     0,
      35,     0,     0,    36,    37,    38,    39,     0,     0,     0,
       0,     1,     2,     3,     4,     0,    40,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
       0,     0,    18,    19,    20,     0,    21,    22,     0,     0,
      23,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    24,    25,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,     0,     0,    27,     0,    28,
      29,    30,    31,    32,    33,     0,    34,     0,     0,    35,
       0,     0,    36,    37,    38,    39,     0,     0,     0,     0,
       1,     2,     3,     4,     0,    40,     5,     6,     7,     8,
       9,     0,    11,    12,    13,    14,    15,    16,    17,     0,
       0,    18,    19,    20,     0,    21,    22,     0,     0,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      24,    25,     0,     0,     0,     0,     0,     0,    26,     0,
       0,     0,     0,     0,     0,     0,    27,     0,    28,    29,
      30,    31,    32,    33,     0,    34,     0,     0,    35,     0,
       0,    36,    37,    38,    39,     1,     2,     3,     0,     0,
       0,     0,    89,   211,    40,     0,    90,     0,    12,    13,
       0,    15,     0,     0,     0,     0,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     1,     2,     3,     0,    24,    25,     0,    89,     0,
       0,     0,    90,    26,    12,    13,     0,    15,     0,     0,
       0,    91,    18,    28,    29,    92,    31,    32,    33,     0,
      34,     0,     0,    35,     0,     0,    36,    37,    38,    39,
       0,    24,    25,     0,     0,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,     0,     0,    91,     0,    28,
      29,    92,    31,    32,    33,     0,    34,     0,     0,    35,
     298,     0,    36,    37,    38,    39,     1,     2,     3,     0,
       0,     0,     0,    89,     0,     0,     0,    90,     0,    12,
      13,     0,    15,     0,     0,     0,     0,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     1,     2,     3,     0,    24,    25,     0,    89,
       0,     0,     0,    90,    26,    12,    13,     0,    15,     0,
       0,     0,    91,    18,    28,    29,    92,    31,    32,    33,
       0,    34,   301,     0,    35,     0,     0,    36,    37,    38,
      39,     0,    24,    25,     0,     0,     0,     0,     0,     0,
      26,     0,     0,     0,     0,     0,     0,     0,    91,     0,
      28,    29,    92,    31,    32,    33,     0,    34,     0,     0,
      35,   464,     0,    36,    37,    38,    39,     1,     2,     3,
       0,     0,     0,     0,    89,     0,     0,     0,    90,     0,
      12,    13,     0,    15,     0,     0,     0,     0,    18,     0,
     165,   166,     0,     0,     0,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,     0,    24,    25,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,     0,
       0,     0,     0,    91,     0,    28,    29,    92,    31,    32,
      33,     0,    34,   178,     0,    35,     0,     0,    36,    37,
      38,    39,   254,   255,     0,     0,     0,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   178
};

static const yytype_int16 yycheck[] =
{
       0,   100,     0,    83,    22,   249,     1,     1,    41,    46,
      41,    22,     1,    60,     1,    68,    16,    16,    11,     1,
     152,   337,    70,    70,    72,    70,   158,    27,    21,    27,
       0,    63,     1,     1,     1,    67,   352,   353,   198,     1,
       1,    34,    87,    63,   199,     1,    16,   179,    68,     6,
     418,     6,    85,   200,    85,    63,   201,    27,    95,   185,
     186,   187,    47,   202,     8,   145,    11,    63,    63,    63,
      67,   203,   109,   205,   111,    86,    21,   194,   195,   196,
     197,    70,    63,    83,    12,    13,   183,   184,    70,    34,
      18,    41,    87,    87,    68,    88,    24,    25,    87,   415,
      87,    70,    70,    31,    32,    87,   238,   475,    36,    37,
      38,    39,   356,    83,    70,   275,    60,    63,    87,    87,
      87,   276,    88,    69,   256,    87,    87,   112,    68,   497,
     277,    87,    89,   278,    89,    85,   262,   263,   264,   132,
     279,   134,    68,   136,   137,   145,   146,   146,   280,    66,
     282,    45,    46,    47,   271,   272,   273,   274,   474,   152,
      66,    78,    79,   260,   261,   158,   159,   453,    63,   162,
      68,    63,    78,    79,    69,   145,   146,    69,    22,    23,
      22,    23,    35,    36,    37,    38,   179,   132,   225,   134,
     476,   136,   137,    70,   480,    39,    40,    39,    40,   485,
      63,    59,    68,   447,    23,    71,    69,    73,    68,    82,
     203,    71,   205,    73,   159,    67,   502,   162,    70,   505,
      39,    40,   188,   189,   190,   191,   192,   193,   360,   361,
      68,   363,    83,    71,    84,    73,    80,    81,    80,    81,
     526,    68,   235,    42,    71,   238,    73,    43,    44,     3,
       4,     5,   180,   181,   182,    70,    10,    72,   251,     0,
      14,    80,    81,   256,    86,    19,    60,    61,    62,    63,
     369,    45,    46,    47,   292,    70,   408,    72,   410,    30,
      31,   413,    35,    36,    37,    38,    70,   280,    72,   282,
     235,    35,    36,    37,    38,    74,    75,    51,    68,   265,
     266,   267,   268,   269,   270,    59,   251,    61,    62,    63,
      82,    70,    66,    72,    68,    83,    42,    71,    60,    61,
      62,    63,    74,    75,   456,    21,    69,    70,    84,   257,
     258,   259,    70,   343,   344,   345,   346,    69,    70,    69,
      70,    69,    70,   336,   337,    69,    70,    69,    70,    69,
      70,    69,    70,    69,    70,    69,    70,    69,    70,   352,
     353,   354,    69,    70,    69,    70,    63,   360,   361,    63,
     363,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,    69,    70,
      63,   336,    82,    84,    83,    42,    70,    68,    67,   479,
      87,   401,   401,   403,   403,   405,   405,    67,    63,   402,
      63,    63,    68,    59,    68,   408,    67,   410,    68,    59,
     413,   414,   415,    67,    59,   418,    63,    22,    67,    63,
      87,   401,    70,   403,    59,   405,    59,    63,    59,     7,
      34,    69,   442,    86,    59,    87,    60,    59,    59,   415,
      60,    69,    60,   453,    60,    60,    31,   402,   538,   539,
      67,    60,    60,   456,    60,    69,   365,    60,   296,   414,
     348,   347,   442,   473,   473,   493,   476,   470,   349,   479,
     480,   474,   475,   453,   351,   485,   350,   487,   487,   515,
     245,   471,    -1,    -1,   494,   494,   489,    -1,    -1,   491,
      -1,    -1,   502,   473,   497,   505,   476,    -1,   474,   479,
     480,    -1,    -1,   531,    -1,   485,    -1,   487,    -1,   519,
     519,   521,   521,    -1,   494,   470,   526,    -1,   528,   529,
     528,   529,   502,   533,   533,   505,    -1,    -1,   538,   539,
      -1,    -1,    -1,    -1,   489,    -1,    -1,    -1,    -1,   519,
      -1,   521,    -1,    -1,    -1,    -1,   526,    -1,   528,   529,
      -1,    -1,     1,   533,     3,     4,     5,    22,   538,   539,
      -1,    10,    -1,    -1,    -1,    14,    -1,    16,    17,    -1,
      19,    -1,    -1,    -1,    -1,    24,    -1,    -1,    43,    44,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    -1,    61,    62,    63,    64,    65,    66,    -1,    68,
      -1,    86,    71,    -1,    -1,    74,    75,    76,    77,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    87,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    -1,    24,    25,    26,    -1,    28,    29,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,
      60,    61,    62,    63,    64,    65,    66,    -1,    68,    -1,
      -1,    71,    -1,    -1,    74,    75,    76,    77,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    87,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    -1,    24,    25,    26,    -1,    28,    29,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    68,    -1,    -1,
      71,    -1,    -1,    74,    75,    76,    77,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    87,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    -1,    24,    25,    26,    -1,    28,    29,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    -1,    -1,    71,
      -1,    -1,    74,    75,    76,    77,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    87,     9,    10,    11,    12,
      13,    -1,    15,    16,    17,    18,    19,    20,    21,    -1,
      -1,    24,    25,    26,    -1,    28,    29,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,    61,    62,
      63,    64,    65,    66,    -1,    68,    -1,    -1,    71,    -1,
      -1,    74,    75,    76,    77,     3,     4,     5,    -1,    -1,
      -1,    -1,    10,    11,    87,    -1,    14,    -1,    16,    17,
      -1,    19,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,    -1,    43,    44,    -1,    10,    -1,
      -1,    -1,    14,    51,    16,    17,    -1,    19,    -1,    -1,
      -1,    59,    24,    61,    62,    63,    64,    65,    66,    -1,
      68,    -1,    -1,    71,    -1,    -1,    74,    75,    76,    77,
      -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,    61,
      62,    63,    64,    65,    66,    -1,    68,    -1,    -1,    71,
      72,    -1,    74,    75,    76,    77,     3,     4,     5,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,    16,
      17,    -1,    19,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,    43,    44,    -1,    10,
      -1,    -1,    -1,    14,    51,    16,    17,    -1,    19,    -1,
      -1,    -1,    59,    24,    61,    62,    63,    64,    65,    66,
      -1,    68,    69,    -1,    71,    -1,    -1,    74,    75,    76,
      77,    -1,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,
      61,    62,    63,    64,    65,    66,    -1,    68,    -1,    -1,
      71,    72,    -1,    74,    75,    76,    77,     3,     4,     5,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    -1,    14,    -1,
      16,    17,    -1,    19,    -1,    -1,    -1,    -1,    24,    -1,
      43,    44,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    -1,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    -1,    61,    62,    63,    64,    65,
      66,    -1,    68,    86,    -1,    71,    -1,    -1,    74,    75,
      76,    77,    43,    44,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    86
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    24,    25,
      26,    28,    29,    32,    43,    44,    51,    59,    61,    62,
      63,    64,    65,    66,    68,    71,    74,    75,    76,    77,
      87,    89,    93,    94,    99,   101,   103,   107,   109,   110,
     112,   114,   116,   118,   121,   124,   127,   130,   133,   136,
     139,   142,   145,   149,   150,   151,   152,   155,   160,   161,
     162,   163,   166,   167,   168,   169,   170,   176,   177,   178,
     179,   180,   184,   185,   186,     1,    63,    87,    68,    10,
      14,    59,    63,    92,    93,    98,   100,   181,    63,   153,
      63,   156,   157,     1,    63,    87,    63,     1,    87,    98,
     100,   102,   106,   108,   110,   111,   113,   115,   117,   119,
     122,   125,   128,   131,   134,   137,   140,   143,   147,   106,
     111,   111,    68,   150,    68,   111,    68,    68,   147,   151,
       1,    87,   111,   111,    60,   185,    67,   111,   111,   147,
      70,    95,    96,    97,   111,   111,   111,   111,    68,    71,
      73,   104,    71,    73,   104,    43,    44,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    86,   146,
      66,    78,    79,    74,    75,    45,    46,    47,    22,    23,
      39,    40,    80,    81,    35,    36,    37,    38,    82,    83,
      84,    42,    41,    85,     1,    70,    87,     0,   186,     1,
      87,    11,   106,   117,   120,   123,   126,   129,   132,   135,
     138,   141,   144,   148,   165,    98,   100,    63,    68,    60,
      61,    62,    63,    90,    91,    71,    73,   104,    86,   158,
       1,    70,    87,   158,     1,    70,    87,     1,    87,    68,
     104,    71,    73,   104,    43,    44,   146,    66,    78,    79,
      74,    75,    45,    46,    47,    22,    23,    39,    40,    80,
      81,    35,    36,    37,    38,    82,    83,    84,    42,    41,
      85,     1,    70,    87,   147,    21,   147,   147,   147,     1,
      87,    30,    31,    60,   150,    69,    70,    72,    72,   143,
      70,    69,   105,   143,   147,    63,   147,    63,   143,   111,
     111,   111,   113,   113,   115,   115,   115,   117,   117,   117,
     117,   117,   117,   119,   119,   119,   119,   122,   125,   128,
     131,   134,   143,   143,    63,   154,    22,   146,    23,    39,
      40,    80,    81,    35,    36,    37,    38,    82,    83,    84,
      42,    41,    85,    70,    87,   104,    68,    63,    69,   182,
      67,    67,    63,    67,    60,    70,   147,    63,   143,    63,
     157,    69,   182,   147,    63,   143,   111,   111,   111,   113,
     113,   115,   115,   115,   117,   117,   117,   117,   117,   117,
     119,   119,   119,   119,   122,   125,   128,   131,   134,   143,
     143,    69,    68,    69,    69,    69,    68,   151,    96,    69,
      70,    72,    72,    67,    22,    86,   159,    70,    87,   147,
     106,   144,   117,   117,   117,   117,   117,   120,   120,   120,
     120,   123,   126,   129,   132,   135,   144,   144,   147,   164,
      69,   182,    59,    69,    70,   143,   143,    68,   143,    60,
      90,    72,   158,    59,    69,    72,    67,   150,   147,   150,
      59,   171,   150,    63,    72,   143,   143,   143,   147,   144,
      22,    63,   164,    69,    67,    87,    59,    69,   183,   185,
      59,    63,    69,   182,   183,    59,   143,    34,    69,     7,
     172,   173,   174,    69,    69,   147,   159,    87,   150,   144,
     164,   183,    59,    60,   183,    59,    69,    60,   183,   150,
       1,    87,   147,     8,    60,   175,   174,   151,   150,    69,
     164,    69,    60,   183,    60,   183,    59,    60,    67,    67,
     172,    31,   150,    69,   150,    60,    60,   183,   185,   185,
      60,   151,   150,    60
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
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc)
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
		  Type, Value, Location); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
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
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
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
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

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
/* Location data for the look-ahead symbol.  */
YYLTYPE yylloc;

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

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[2];

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

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
  yylsp = yyls;
#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 0;
#endif

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
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
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
	YYSTACK_RELOCATE (yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
//#line 281 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new NullNode(GLOBAL_DATA), 0); ;}
    break;

  case 3:
//#line 282 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new BooleanNode(GLOBAL_DATA, true), 0); ;}
    break;

  case 4:
//#line 283 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new BooleanNode(GLOBAL_DATA, false), 0); ;}
    break;

  case 5:
//#line 284 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(makeNumberNode(GLOBAL_DATA, (yyvsp[(1) - (1)].doubleValue)), 0); ;}
    break;

  case 6:
//#line 285 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new StringNode(GLOBAL_DATA, (yyvsp[(1) - (1)].string)), 0); ;}
    break;

  case 7:
//#line 286 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    {
                                            Lexer& l = *LEXER;
                                            if (!l.scanRegExp())
                                                YYABORT;
                                            (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new RegExpNode(GLOBAL_DATA, l.pattern(), l.flags()), 0);
                                        ;}
    break;

  case 8:
//#line 292 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    {
                                            Lexer& l = *LEXER;
                                            if (!l.scanRegExp())
                                                YYABORT;
                                            (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new RegExpNode(GLOBAL_DATA, "=" + l.pattern(), l.flags()), 0);
                                        ;}
    break;

  case 9:
//#line 301 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.propertyNode) = createNodeFeatureInfo<PropertyNode*>(new PropertyNode(GLOBAL_DATA, *(yyvsp[(1) - (3)].ident), (yyvsp[(3) - (3)].expressionNode).m_node, PropertyNode::Constant), (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 10:
//#line 302 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.propertyNode) = createNodeFeatureInfo<PropertyNode*>(new PropertyNode(GLOBAL_DATA, Identifier(GLOBAL_DATA, *(yyvsp[(1) - (3)].string)), (yyvsp[(3) - (3)].expressionNode).m_node, PropertyNode::Constant), (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 11:
//#line 303 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.propertyNode) = createNodeFeatureInfo<PropertyNode*>(new PropertyNode(GLOBAL_DATA, Identifier(GLOBAL_DATA, UString::from((yyvsp[(1) - (3)].doubleValue))), (yyvsp[(3) - (3)].expressionNode).m_node, PropertyNode::Constant), (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 12:
//#line 304 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.propertyNode) = createNodeFeatureInfo<PropertyNode*>(makeGetterOrSetterPropertyNode(globalPtr, *(yyvsp[(1) - (7)].ident), *(yyvsp[(2) - (7)].ident), 0, (yyvsp[(6) - (7)].functionBodyNode), LEXER->sourceRange((yyvsp[(5) - (7)].intValue), (yyvsp[(7) - (7)].intValue))), ClosureFeature); DBG((yyvsp[(6) - (7)].functionBodyNode), (yylsp[(5) - (7)]), (yylsp[(7) - (7)])); if (!(yyval.propertyNode).m_node) YYABORT; ;}
    break;

  case 13:
//#line 306 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.propertyNode) = createNodeFeatureInfo<PropertyNode*>(makeGetterOrSetterPropertyNode(globalPtr, *(yyvsp[(1) - (8)].ident), *(yyvsp[(2) - (8)].ident), (yyvsp[(4) - (8)].parameterList).head, (yyvsp[(7) - (8)].functionBodyNode), LEXER->sourceRange((yyvsp[(6) - (8)].intValue), (yyvsp[(8) - (8)].intValue))), ClosureFeature); DBG((yyvsp[(7) - (8)].functionBodyNode), (yylsp[(6) - (8)]), (yylsp[(8) - (8)])); if (!(yyval.propertyNode).m_node) YYABORT; ;}
    break;

  case 14:
//#line 310 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.propertyList).m_node.head = new PropertyListNode(GLOBAL_DATA, (yyvsp[(1) - (1)].propertyNode).m_node); 
                                          (yyval.propertyList).m_node.tail = (yyval.propertyList).m_node.head;
                                          (yyval.propertyList).m_featureInfo = (yyvsp[(1) - (1)].propertyNode).m_featureInfo; ;}
    break;

  case 15:
//#line 313 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.propertyList).m_node.head = (yyvsp[(1) - (3)].propertyList).m_node.head;
                                          (yyval.propertyList).m_node.tail = new PropertyListNode(GLOBAL_DATA, (yyvsp[(3) - (3)].propertyNode).m_node, (yyvsp[(1) - (3)].propertyList).m_node.tail);
                                          (yyval.propertyList).m_featureInfo = (yyvsp[(1) - (3)].propertyList).m_featureInfo | (yyvsp[(3) - (3)].propertyNode).m_featureInfo;  ;}
    break;

  case 17:
//#line 320 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new ObjectLiteralNode(GLOBAL_DATA), 0); ;}
    break;

  case 18:
//#line 321 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new ObjectLiteralNode(GLOBAL_DATA, (yyvsp[(2) - (3)].propertyList).m_node.head), (yyvsp[(2) - (3)].propertyList).m_featureInfo); ;}
    break;

  case 19:
//#line 323 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new ObjectLiteralNode(GLOBAL_DATA, (yyvsp[(2) - (4)].propertyList).m_node.head), (yyvsp[(2) - (4)].propertyList).m_featureInfo); ;}
    break;

  case 20:
//#line 327 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new ThisNode(GLOBAL_DATA), 0); ;}
    break;

  case 23:
//#line 330 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new ResolveNode(GLOBAL_DATA, *(yyvsp[(1) - (1)].ident)), 0); ;}
    break;

  case 24:
//#line 331 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = (yyvsp[(2) - (3)].expressionNode); ;}
    break;

  case 25:
//#line 335 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new ArrayNode(GLOBAL_DATA, (yyvsp[(2) - (3)].intValue)), 0); ;}
    break;

  case 26:
//#line 336 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new ArrayNode(GLOBAL_DATA, (yyvsp[(2) - (3)].elementList).m_node.head), (yyvsp[(2) - (3)].elementList).m_featureInfo); ;}
    break;

  case 27:
//#line 337 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new ArrayNode(GLOBAL_DATA, (yyvsp[(4) - (5)].intValue), (yyvsp[(2) - (5)].elementList).m_node.head), (yyvsp[(2) - (5)].elementList).m_featureInfo); ;}
    break;

  case 28:
//#line 341 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.elementList).m_node.head = new ElementNode(GLOBAL_DATA, (yyvsp[(1) - (2)].intValue), (yyvsp[(2) - (2)].expressionNode).m_node);
                                          (yyval.elementList).m_node.tail = (yyval.elementList).m_node.head;
                                          (yyval.elementList).m_featureInfo = (yyvsp[(2) - (2)].expressionNode).m_featureInfo; ;}
    break;

  case 29:
//#line 345 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.elementList).m_node.head = (yyvsp[(1) - (4)].elementList).m_node.head;
                                          (yyval.elementList).m_node.tail = new ElementNode(GLOBAL_DATA, (yyvsp[(1) - (4)].elementList).m_node.tail, (yyvsp[(3) - (4)].intValue), (yyvsp[(4) - (4)].expressionNode).m_node);
                                          (yyval.elementList).m_featureInfo = (yyvsp[(1) - (4)].elementList).m_featureInfo | (yyvsp[(4) - (4)].expressionNode).m_featureInfo; ;}
    break;

  case 30:
//#line 351 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.intValue) = 0; ;}
    break;

  case 32:
//#line 356 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.intValue) = 1; ;}
    break;

  case 33:
//#line 357 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.intValue) = (yyvsp[(1) - (2)].intValue) + 1; ;}
    break;

  case 35:
//#line 362 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>((yyvsp[(1) - (1)].funcExprNode).m_node, (yyvsp[(1) - (1)].funcExprNode).m_featureInfo); ;}
    break;

  case 36:
//#line 363 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new BracketAccessorNode(GLOBAL_DATA, (yyvsp[(1) - (4)].expressionNode).m_node, (yyvsp[(3) - (4)].expressionNode).m_node, (yyvsp[(3) - (4)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (4)].expressionNode).m_featureInfo | (yyvsp[(3) - (4)].expressionNode).m_featureInfo); ;}
    break;

  case 37:
//#line 364 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new DotAccessorNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, *(yyvsp[(3) - (3)].ident)), (yyvsp[(1) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 38:
//#line 365 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new NewExprNode(GLOBAL_DATA, (yyvsp[(2) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].argumentsNode).m_node), (yyvsp[(2) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].argumentsNode).m_featureInfo); ;}
    break;

  case 40:
//#line 370 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new BracketAccessorNode(GLOBAL_DATA, (yyvsp[(1) - (4)].expressionNode).m_node, (yyvsp[(3) - (4)].expressionNode).m_node, (yyvsp[(3) - (4)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (4)].expressionNode).m_featureInfo | (yyvsp[(3) - (4)].expressionNode).m_featureInfo); ;}
    break;

  case 41:
//#line 371 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new DotAccessorNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, *(yyvsp[(3) - (3)].ident)), (yyvsp[(1) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 42:
//#line 372 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new NewExprNode(GLOBAL_DATA, (yyvsp[(2) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].argumentsNode).m_node), (yyvsp[(2) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].argumentsNode).m_featureInfo); ;}
    break;

  case 44:
//#line 377 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new NewExprNode(GLOBAL_DATA, (yyvsp[(2) - (2)].expressionNode).m_node), (yyvsp[(2) - (2)].expressionNode).m_featureInfo); ;}
    break;

  case 46:
//#line 382 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new NewExprNode(GLOBAL_DATA, (yyvsp[(2) - (2)].expressionNode).m_node), (yyvsp[(2) - (2)].expressionNode).m_featureInfo); ;}
    break;

  case 47:
//#line 386 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = makeFunctionCallNode(globalPtr, (yyvsp[(1) - (2)].expressionNode), (yyvsp[(2) - (2)].argumentsNode)); ;}
    break;

  case 48:
//#line 387 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = makeFunctionCallNode(globalPtr, (yyvsp[(1) - (2)].expressionNode), (yyvsp[(2) - (2)].argumentsNode)); ;}
    break;

  case 49:
//#line 388 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new BracketAccessorNode(GLOBAL_DATA, (yyvsp[(1) - (4)].expressionNode).m_node, (yyvsp[(3) - (4)].expressionNode).m_node, (yyvsp[(3) - (4)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (4)].expressionNode).m_featureInfo | (yyvsp[(3) - (4)].expressionNode).m_featureInfo); ;}
    break;

  case 50:
//#line 389 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new DotAccessorNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, *(yyvsp[(3) - (3)].ident)), (yyvsp[(1) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 51:
//#line 393 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = makeFunctionCallNode(globalPtr, (yyvsp[(1) - (2)].expressionNode), (yyvsp[(2) - (2)].argumentsNode)); ;}
    break;

  case 52:
//#line 394 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = makeFunctionCallNode(globalPtr, (yyvsp[(1) - (2)].expressionNode), (yyvsp[(2) - (2)].argumentsNode)); ;}
    break;

  case 53:
//#line 395 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new BracketAccessorNode(GLOBAL_DATA, (yyvsp[(1) - (4)].expressionNode).m_node, (yyvsp[(3) - (4)].expressionNode).m_node, (yyvsp[(3) - (4)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (4)].expressionNode).m_featureInfo | (yyvsp[(3) - (4)].expressionNode).m_featureInfo); ;}
    break;

  case 54:
//#line 396 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new DotAccessorNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, *(yyvsp[(3) - (3)].ident)), (yyvsp[(1) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 55:
//#line 400 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.argumentsNode) = createNodeFeatureInfo<ArgumentsNode*>(new ArgumentsNode(GLOBAL_DATA), 0); ;}
    break;

  case 56:
//#line 401 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.argumentsNode) = createNodeFeatureInfo<ArgumentsNode*>(new ArgumentsNode(GLOBAL_DATA, (yyvsp[(2) - (3)].argumentList).m_node.head), (yyvsp[(2) - (3)].argumentList).m_featureInfo); ;}
    break;

  case 57:
//#line 405 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.argumentList).m_node.head = new ArgumentListNode(GLOBAL_DATA, (yyvsp[(1) - (1)].expressionNode).m_node);
                                          (yyval.argumentList).m_node.tail = (yyval.argumentList).m_node.head;
                                          (yyval.argumentList).m_featureInfo = (yyvsp[(1) - (1)].expressionNode).m_featureInfo; ;}
    break;

  case 58:
//#line 408 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.argumentList).m_node.head = (yyvsp[(1) - (3)].argumentList).m_node.head;
                                          (yyval.argumentList).m_node.tail = new ArgumentListNode(GLOBAL_DATA, (yyvsp[(1) - (3)].argumentList).m_node.tail, (yyvsp[(3) - (3)].expressionNode).m_node);
                                          (yyval.argumentList).m_featureInfo = (yyvsp[(1) - (3)].argumentList).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo; ;}
    break;

  case 64:
//#line 425 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(makePostfixNode(GLOBAL_DATA, (yyvsp[(1) - (2)].expressionNode).m_node, OpPlusPlus), (yyvsp[(1) - (2)].expressionNode).m_featureInfo | AssignFeature); ;}
    break;

  case 65:
//#line 426 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(makePostfixNode(GLOBAL_DATA, (yyvsp[(1) - (2)].expressionNode).m_node, OpMinusMinus), (yyvsp[(1) - (2)].expressionNode).m_featureInfo | AssignFeature); ;}
    break;

  case 67:
//#line 431 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(makePostfixNode(GLOBAL_DATA, (yyvsp[(1) - (2)].expressionNode).m_node, OpPlusPlus), (yyvsp[(1) - (2)].expressionNode).m_featureInfo | AssignFeature); ;}
    break;

  case 68:
//#line 432 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(makePostfixNode(GLOBAL_DATA, (yyvsp[(1) - (2)].expressionNode).m_node, OpMinusMinus), (yyvsp[(1) - (2)].expressionNode).m_featureInfo | AssignFeature); ;}
    break;

  case 69:
//#line 436 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(makeDeleteNode(GLOBAL_DATA, (yyvsp[(2) - (2)].expressionNode).m_node), (yyvsp[(2) - (2)].expressionNode).m_featureInfo); ;}
    break;

  case 70:
//#line 437 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new VoidNode(GLOBAL_DATA, (yyvsp[(2) - (2)].expressionNode).m_node), (yyvsp[(2) - (2)].expressionNode).m_featureInfo); ;}
    break;

  case 71:
//#line 438 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(makeTypeOfNode(GLOBAL_DATA, (yyvsp[(2) - (2)].expressionNode).m_node), (yyvsp[(2) - (2)].expressionNode).m_featureInfo); ;}
    break;

  case 72:
//#line 439 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(makePrefixNode(GLOBAL_DATA, (yyvsp[(2) - (2)].expressionNode).m_node, OpPlusPlus), (yyvsp[(2) - (2)].expressionNode).m_featureInfo | AssignFeature); ;}
    break;

  case 73:
//#line 440 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(makePrefixNode(GLOBAL_DATA, (yyvsp[(2) - (2)].expressionNode).m_node, OpPlusPlus), (yyvsp[(2) - (2)].expressionNode).m_featureInfo | AssignFeature); ;}
    break;

  case 74:
//#line 441 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(makePrefixNode(GLOBAL_DATA, (yyvsp[(2) - (2)].expressionNode).m_node, OpMinusMinus), (yyvsp[(2) - (2)].expressionNode).m_featureInfo | AssignFeature); ;}
    break;

  case 75:
//#line 442 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(makePrefixNode(GLOBAL_DATA, (yyvsp[(2) - (2)].expressionNode).m_node, OpMinusMinus), (yyvsp[(2) - (2)].expressionNode).m_featureInfo | AssignFeature); ;}
    break;

  case 76:
//#line 443 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new UnaryPlusNode(GLOBAL_DATA, (yyvsp[(2) - (2)].expressionNode).m_node), (yyvsp[(2) - (2)].expressionNode).m_featureInfo); ;}
    break;

  case 77:
//#line 444 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(makeNegateNode(GLOBAL_DATA, (yyvsp[(2) - (2)].expressionNode).m_node), (yyvsp[(2) - (2)].expressionNode).m_featureInfo); ;}
    break;

  case 78:
//#line 445 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new BitwiseNotNode(GLOBAL_DATA, (yyvsp[(2) - (2)].expressionNode).m_node), (yyvsp[(2) - (2)].expressionNode).m_featureInfo); ;}
    break;

  case 79:
//#line 446 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new LogicalNotNode(GLOBAL_DATA, (yyvsp[(2) - (2)].expressionNode).m_node), (yyvsp[(2) - (2)].expressionNode).m_featureInfo); ;}
    break;

  case 85:
//#line 460 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new MultNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 86:
//#line 461 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new DivNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 87:
//#line 462 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new ModNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 89:
//#line 468 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new MultNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 90:
//#line 470 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new DivNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 91:
//#line 472 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new ModNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 93:
//#line 477 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new AddNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 94:
//#line 478 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new SubNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 96:
//#line 484 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new AddNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 97:
//#line 486 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new SubNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 99:
//#line 491 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new LeftShiftNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 100:
//#line 492 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new RightShiftNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 101:
//#line 493 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new UnsignedRightShiftNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 103:
//#line 498 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new LeftShiftNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 104:
//#line 499 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new RightShiftNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 105:
//#line 500 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new UnsignedRightShiftNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 107:
//#line 505 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new LessNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 108:
//#line 506 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new GreaterNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 109:
//#line 507 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new LessEqNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 110:
//#line 508 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new GreaterEqNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 111:
//#line 509 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new InstanceOfNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 112:
//#line 510 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new InNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 114:
//#line 515 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new LessNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 115:
//#line 516 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new GreaterNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 116:
//#line 517 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new LessEqNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 117:
//#line 518 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new GreaterEqNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 118:
//#line 520 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new InstanceOfNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 120:
//#line 525 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new LessNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 121:
//#line 526 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new GreaterNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 122:
//#line 527 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new LessEqNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 123:
//#line 528 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new GreaterEqNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 124:
//#line 530 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new InstanceOfNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 125:
//#line 531 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new InNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 127:
//#line 536 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new EqualNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 128:
//#line 537 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new NotEqualNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 129:
//#line 538 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new StrictEqualNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 130:
//#line 539 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new NotStrictEqualNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 132:
//#line 545 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new EqualNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 133:
//#line 547 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new NotEqualNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 134:
//#line 549 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new StrictEqualNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 135:
//#line 551 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new NotStrictEqualNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 137:
//#line 557 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new EqualNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 138:
//#line 558 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new NotEqualNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 139:
//#line 560 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new StrictEqualNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 140:
//#line 562 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new NotStrictEqualNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 142:
//#line 567 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new BitAndNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 144:
//#line 573 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new BitAndNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 146:
//#line 578 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new BitAndNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 148:
//#line 583 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new BitXOrNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 150:
//#line 589 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new BitXOrNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 152:
//#line 595 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new BitXOrNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 154:
//#line 600 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new BitOrNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 156:
//#line 606 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new BitOrNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 158:
//#line 612 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new BitOrNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 160:
//#line 617 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new LogicalAndNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 162:
//#line 623 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new LogicalAndNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 164:
//#line 629 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new LogicalAndNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 166:
//#line 634 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new LogicalOrNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 168:
//#line 640 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new LogicalOrNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 170:
//#line 645 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new LogicalOrNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 172:
//#line 651 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new ConditionalNode(GLOBAL_DATA, (yyvsp[(1) - (5)].expressionNode).m_node, (yyvsp[(3) - (5)].expressionNode).m_node, (yyvsp[(5) - (5)].expressionNode).m_node), (yyvsp[(1) - (5)].expressionNode).m_featureInfo | (yyvsp[(3) - (5)].expressionNode).m_featureInfo | (yyvsp[(5) - (5)].expressionNode).m_featureInfo); ;}
    break;

  case 174:
//#line 657 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new ConditionalNode(GLOBAL_DATA, (yyvsp[(1) - (5)].expressionNode).m_node, (yyvsp[(3) - (5)].expressionNode).m_node, (yyvsp[(5) - (5)].expressionNode).m_node), (yyvsp[(1) - (5)].expressionNode).m_featureInfo | (yyvsp[(3) - (5)].expressionNode).m_featureInfo | (yyvsp[(5) - (5)].expressionNode).m_featureInfo); ;}
    break;

  case 176:
//#line 663 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new ConditionalNode(GLOBAL_DATA, (yyvsp[(1) - (5)].expressionNode).m_node, (yyvsp[(3) - (5)].expressionNode).m_node, (yyvsp[(5) - (5)].expressionNode).m_node), (yyvsp[(1) - (5)].expressionNode).m_featureInfo | (yyvsp[(3) - (5)].expressionNode).m_featureInfo | (yyvsp[(5) - (5)].expressionNode).m_featureInfo); ;}
    break;

  case 178:
//#line 669 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(makeAssignNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(2) - (3)].op), (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(1) - (3)].expressionNode).m_featureInfo & AssignFeature, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo | AssignFeature); ;}
    break;

  case 180:
//#line 675 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(makeAssignNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(2) - (3)].op), (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(1) - (3)].expressionNode).m_featureInfo & AssignFeature, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo | AssignFeature); ;}
    break;

  case 182:
//#line 681 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(makeAssignNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(2) - (3)].op), (yyvsp[(3) - (3)].expressionNode).m_node, (yyvsp[(1) - (3)].expressionNode).m_featureInfo & AssignFeature, (yyvsp[(3) - (3)].expressionNode).m_featureInfo & AssignFeature), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo | AssignFeature); ;}
    break;

  case 183:
//#line 685 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.op) = OpEqual; ;}
    break;

  case 184:
//#line 686 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.op) = OpPlusEq; ;}
    break;

  case 185:
//#line 687 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.op) = OpMinusEq; ;}
    break;

  case 186:
//#line 688 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.op) = OpMultEq; ;}
    break;

  case 187:
//#line 689 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.op) = OpDivEq; ;}
    break;

  case 188:
//#line 690 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.op) = OpLShift; ;}
    break;

  case 189:
//#line 691 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.op) = OpRShift; ;}
    break;

  case 190:
//#line 692 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.op) = OpURShift; ;}
    break;

  case 191:
//#line 693 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.op) = OpAndEq; ;}
    break;

  case 192:
//#line 694 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.op) = OpXOrEq; ;}
    break;

  case 193:
//#line 695 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.op) = OpOrEq; ;}
    break;

  case 194:
//#line 696 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.op) = OpModEq; ;}
    break;

  case 196:
//#line 701 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new CommaNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 198:
//#line 706 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new CommaNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 200:
//#line 711 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(new CommaNode(GLOBAL_DATA, (yyvsp[(1) - (3)].expressionNode).m_node, (yyvsp[(3) - (3)].expressionNode).m_node), (yyvsp[(1) - (3)].expressionNode).m_featureInfo | (yyvsp[(3) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 217:
//#line 734 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new BlockNode(GLOBAL_DATA, 0), 0, 0, 0);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 218:
//#line 736 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new BlockNode(GLOBAL_DATA, (yyvsp[(2) - (3)].sourceElements).m_node), (yyvsp[(2) - (3)].sourceElements).m_varDeclarations, (yyvsp[(2) - (3)].sourceElements).m_funcDeclarations, (yyvsp[(2) - (3)].sourceElements).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 219:
//#line 741 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(makeVarStatementNode(GLOBAL_DATA, (yyvsp[(2) - (3)].varDeclList).m_node), (yyvsp[(2) - (3)].varDeclList).m_varDeclarations, (yyvsp[(2) - (3)].varDeclList).m_funcDeclarations, (yyvsp[(2) - (3)].varDeclList).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 220:
//#line 743 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(makeVarStatementNode(GLOBAL_DATA, (yyvsp[(2) - (3)].varDeclList).m_node), (yyvsp[(2) - (3)].varDeclList).m_varDeclarations, (yyvsp[(2) - (3)].varDeclList).m_funcDeclarations, (yyvsp[(2) - (3)].varDeclList).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
                                          AUTO_SEMICOLON; ;}
    break;

  case 221:
//#line 749 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.varDeclList).m_node = 0;
                                          (yyval.varDeclList).m_varDeclarations = new ParserRefCountedData<DeclarationStacks::VarStack>(GLOBAL_DATA);
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[(1) - (1)].ident), 0);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_featureInfo = 0;
                                        ;}
    break;

  case 222:
//#line 755 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.varDeclList).m_node = new AssignResolveNode(GLOBAL_DATA, *(yyvsp[(1) - (2)].ident), (yyvsp[(2) - (2)].expressionNode).m_node, (yyvsp[(2) - (2)].expressionNode).m_featureInfo & AssignFeature);
                                          (yyval.varDeclList).m_varDeclarations = new ParserRefCountedData<DeclarationStacks::VarStack>(GLOBAL_DATA);
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[(1) - (2)].ident), DeclarationStacks::HasInitializer);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_featureInfo = (yyvsp[(2) - (2)].expressionNode).m_featureInfo;
                                        ;}
    break;

  case 223:
//#line 762 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.varDeclList).m_node = (yyvsp[(1) - (3)].varDeclList).m_node;
                                          (yyval.varDeclList).m_varDeclarations = (yyvsp[(1) - (3)].varDeclList).m_varDeclarations;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[(3) - (3)].ident), 0);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_featureInfo = (yyvsp[(1) - (3)].varDeclList).m_featureInfo;
                                        ;}
    break;

  case 224:
//#line 769 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.varDeclList).m_node = combineVarInitializers(GLOBAL_DATA, (yyvsp[(1) - (4)].varDeclList).m_node, new AssignResolveNode(GLOBAL_DATA, *(yyvsp[(3) - (4)].ident), (yyvsp[(4) - (4)].expressionNode).m_node, (yyvsp[(4) - (4)].expressionNode).m_featureInfo & AssignFeature));
                                          (yyval.varDeclList).m_varDeclarations = (yyvsp[(1) - (4)].varDeclList).m_varDeclarations;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[(3) - (4)].ident), DeclarationStacks::HasInitializer);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_featureInfo = (yyvsp[(1) - (4)].varDeclList).m_featureInfo | (yyvsp[(4) - (4)].expressionNode).m_featureInfo;
                                        ;}
    break;

  case 225:
//#line 778 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.varDeclList).m_node = 0;
                                          (yyval.varDeclList).m_varDeclarations = new ParserRefCountedData<DeclarationStacks::VarStack>(GLOBAL_DATA);
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[(1) - (1)].ident), 0);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_featureInfo = 0;
                                        ;}
    break;

  case 226:
//#line 784 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.varDeclList).m_node = new AssignResolveNode(GLOBAL_DATA, *(yyvsp[(1) - (2)].ident), (yyvsp[(2) - (2)].expressionNode).m_node, (yyvsp[(2) - (2)].expressionNode).m_featureInfo & AssignFeature);
                                          (yyval.varDeclList).m_varDeclarations = new ParserRefCountedData<DeclarationStacks::VarStack>(GLOBAL_DATA);
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[(1) - (2)].ident), DeclarationStacks::HasInitializer);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_featureInfo = (yyvsp[(2) - (2)].expressionNode).m_featureInfo;
                                        ;}
    break;

  case 227:
//#line 791 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.varDeclList).m_node = (yyvsp[(1) - (3)].varDeclList).m_node;
                                          (yyval.varDeclList).m_varDeclarations = (yyvsp[(1) - (3)].varDeclList).m_varDeclarations;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[(3) - (3)].ident), 0);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_featureInfo = (yyvsp[(1) - (3)].varDeclList).m_featureInfo;
                                        ;}
    break;

  case 228:
//#line 798 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.varDeclList).m_node = combineVarInitializers(GLOBAL_DATA, (yyvsp[(1) - (4)].varDeclList).m_node, new AssignResolveNode(GLOBAL_DATA, *(yyvsp[(3) - (4)].ident), (yyvsp[(4) - (4)].expressionNode).m_node, (yyvsp[(4) - (4)].expressionNode).m_featureInfo & AssignFeature));
                                          (yyval.varDeclList).m_varDeclarations = (yyvsp[(1) - (4)].varDeclList).m_varDeclarations;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.varDeclList).m_varDeclarations, *(yyvsp[(3) - (4)].ident), DeclarationStacks::HasInitializer);
                                          (yyval.varDeclList).m_funcDeclarations = 0;
                                          (yyval.varDeclList).m_featureInfo = (yyvsp[(1) - (4)].varDeclList).m_featureInfo | (yyvsp[(4) - (4)].expressionNode).m_featureInfo;
                                        ;}
    break;

  case 229:
//#line 807 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new ConstStatementNode(GLOBAL_DATA, (yyvsp[(2) - (3)].constDeclList).m_node.head), (yyvsp[(2) - (3)].constDeclList).m_varDeclarations, (yyvsp[(2) - (3)].constDeclList).m_funcDeclarations, (yyvsp[(2) - (3)].constDeclList).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 230:
//#line 810 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new ConstStatementNode(GLOBAL_DATA, (yyvsp[(2) - (3)].constDeclList).m_node.head), (yyvsp[(2) - (3)].constDeclList).m_varDeclarations, (yyvsp[(2) - (3)].constDeclList).m_funcDeclarations, (yyvsp[(2) - (3)].constDeclList).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (3)]), (yylsp[(2) - (3)])); AUTO_SEMICOLON; ;}
    break;

  case 231:
//#line 815 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.constDeclList).m_node.head = (yyvsp[(1) - (1)].constDeclNode).m_node;
                                          (yyval.constDeclList).m_node.tail = (yyval.constDeclList).m_node.head;
                                          (yyval.constDeclList).m_varDeclarations = new ParserRefCountedData<DeclarationStacks::VarStack>(GLOBAL_DATA);
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.constDeclList).m_varDeclarations, (yyvsp[(1) - (1)].constDeclNode).m_node);
                                          (yyval.constDeclList).m_funcDeclarations = 0; 
                                          (yyval.constDeclList).m_featureInfo = (yyvsp[(1) - (1)].constDeclNode).m_featureInfo;
    ;}
    break;

  case 232:
//#line 823 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    {  (yyval.constDeclList).m_node.head = (yyvsp[(1) - (3)].constDeclList).m_node.head;
                                          (yyvsp[(1) - (3)].constDeclList).m_node.tail->m_next = (yyvsp[(3) - (3)].constDeclNode).m_node;
                                          (yyval.constDeclList).m_node.tail = (yyvsp[(3) - (3)].constDeclNode).m_node;
                                          (yyval.constDeclList).m_varDeclarations = (yyvsp[(1) - (3)].constDeclList).m_varDeclarations;
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyval.constDeclList).m_varDeclarations, (yyvsp[(3) - (3)].constDeclNode).m_node);
                                          (yyval.constDeclList).m_funcDeclarations = 0;
                                          (yyval.constDeclList).m_featureInfo = (yyvsp[(1) - (3)].constDeclList).m_featureInfo | (yyvsp[(3) - (3)].constDeclNode).m_featureInfo;;}
    break;

  case 233:
//#line 833 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.constDeclNode) = createNodeFeatureInfo<ConstDeclNode*>(new ConstDeclNode(GLOBAL_DATA, *(yyvsp[(1) - (1)].ident), 0), 0); ;}
    break;

  case 234:
//#line 834 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.constDeclNode) = createNodeFeatureInfo<ConstDeclNode*>(new ConstDeclNode(GLOBAL_DATA, *(yyvsp[(1) - (2)].ident), (yyvsp[(2) - (2)].expressionNode).m_node), (yyvsp[(2) - (2)].expressionNode).m_featureInfo); ;}
    break;

  case 235:
//#line 838 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = (yyvsp[(2) - (2)].expressionNode); ;}
    break;

  case 236:
//#line 842 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = (yyvsp[(2) - (2)].expressionNode); ;}
    break;

  case 237:
//#line 846 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new EmptyStatementNode(GLOBAL_DATA), 0, 0, 0); ;}
    break;

  case 238:
//#line 850 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new ExprStatementNode(GLOBAL_DATA, (yyvsp[(1) - (2)].expressionNode).m_node), 0, 0, (yyvsp[(1) - (2)].expressionNode).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 239:
//#line 852 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new ExprStatementNode(GLOBAL_DATA, (yyvsp[(1) - (2)].expressionNode).m_node), 0, 0, (yyvsp[(1) - (2)].expressionNode).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (2)]), (yylsp[(1) - (2)])); AUTO_SEMICOLON; ;}
    break;

  case 240:
//#line 858 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new IfNode(GLOBAL_DATA, (yyvsp[(3) - (5)].expressionNode).m_node, (yyvsp[(5) - (5)].statementNode).m_node), (yyvsp[(5) - (5)].statementNode).m_varDeclarations, (yyvsp[(5) - (5)].statementNode).m_funcDeclarations, (yyvsp[(3) - (5)].expressionNode).m_featureInfo | (yyvsp[(5) - (5)].statementNode).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (5)]), (yylsp[(4) - (5)])); ;}
    break;

  case 241:
//#line 861 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new IfElseNode(GLOBAL_DATA, (yyvsp[(3) - (7)].expressionNode).m_node, (yyvsp[(5) - (7)].statementNode).m_node, (yyvsp[(7) - (7)].statementNode).m_node), 
                                                                                         mergeDeclarationLists((yyvsp[(5) - (7)].statementNode).m_varDeclarations, (yyvsp[(7) - (7)].statementNode).m_varDeclarations), mergeDeclarationLists((yyvsp[(5) - (7)].statementNode).m_funcDeclarations, (yyvsp[(7) - (7)].statementNode).m_funcDeclarations),
                                                                                         (yyvsp[(3) - (7)].expressionNode).m_featureInfo | (yyvsp[(5) - (7)].statementNode).m_featureInfo | (yyvsp[(7) - (7)].statementNode).m_featureInfo); 
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (7)]), (yylsp[(4) - (7)])); ;}
    break;

  case 242:
//#line 868 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new DoWhileNode(GLOBAL_DATA, (yyvsp[(2) - (7)].statementNode).m_node, (yyvsp[(5) - (7)].expressionNode).m_node), (yyvsp[(2) - (7)].statementNode).m_varDeclarations, (yyvsp[(2) - (7)].statementNode).m_funcDeclarations, (yyvsp[(2) - (7)].statementNode).m_featureInfo | (yyvsp[(5) - (7)].expressionNode).m_featureInfo);
                                             DBG((yyval.statementNode).m_node, (yylsp[(1) - (7)]), (yylsp[(3) - (7)])); ;}
    break;

  case 243:
//#line 870 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new DoWhileNode(GLOBAL_DATA, (yyvsp[(2) - (7)].statementNode).m_node, (yyvsp[(5) - (7)].expressionNode).m_node), (yyvsp[(2) - (7)].statementNode).m_varDeclarations, (yyvsp[(2) - (7)].statementNode).m_funcDeclarations, (yyvsp[(2) - (7)].statementNode).m_featureInfo | (yyvsp[(5) - (7)].expressionNode).m_featureInfo);
                                             DBG((yyval.statementNode).m_node, (yylsp[(1) - (7)]), (yylsp[(3) - (7)])); ;}
    break;

  case 244:
//#line 872 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new WhileNode(GLOBAL_DATA, (yyvsp[(3) - (5)].expressionNode).m_node, (yyvsp[(5) - (5)].statementNode).m_node), (yyvsp[(5) - (5)].statementNode).m_varDeclarations, (yyvsp[(5) - (5)].statementNode).m_funcDeclarations, (yyvsp[(3) - (5)].expressionNode).m_featureInfo | (yyvsp[(5) - (5)].statementNode).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (5)]), (yylsp[(4) - (5)])); ;}
    break;

  case 245:
//#line 875 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new ForNode(GLOBAL_DATA, (yyvsp[(3) - (9)].expressionNode).m_node, (yyvsp[(5) - (9)].expressionNode).m_node, (yyvsp[(7) - (9)].expressionNode).m_node, (yyvsp[(9) - (9)].statementNode).m_node, false), (yyvsp[(9) - (9)].statementNode).m_varDeclarations, (yyvsp[(9) - (9)].statementNode).m_funcDeclarations, 
                                                                                         (yyvsp[(3) - (9)].expressionNode).m_featureInfo | (yyvsp[(5) - (9)].expressionNode).m_featureInfo | (yyvsp[(7) - (9)].expressionNode).m_featureInfo | (yyvsp[(9) - (9)].statementNode).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (9)]), (yylsp[(8) - (9)])); 
                                        ;}
    break;

  case 246:
//#line 880 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new ForNode(GLOBAL_DATA, (yyvsp[(4) - (10)].varDeclList).m_node, (yyvsp[(6) - (10)].expressionNode).m_node, (yyvsp[(8) - (10)].expressionNode).m_node, (yyvsp[(10) - (10)].statementNode).m_node, true),
                                                                                         mergeDeclarationLists((yyvsp[(4) - (10)].varDeclList).m_varDeclarations, (yyvsp[(10) - (10)].statementNode).m_varDeclarations),
                                                                                         mergeDeclarationLists((yyvsp[(4) - (10)].varDeclList).m_funcDeclarations, (yyvsp[(10) - (10)].statementNode).m_funcDeclarations),
                                                                                         (yyvsp[(4) - (10)].varDeclList).m_featureInfo | (yyvsp[(6) - (10)].expressionNode).m_featureInfo | (yyvsp[(8) - (10)].expressionNode).m_featureInfo | (yyvsp[(10) - (10)].statementNode).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (10)]), (yylsp[(9) - (10)])); ;}
    break;

  case 247:
//#line 886 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    {
                                            ExpressionNode* n = (yyvsp[(3) - (7)].expressionNode).m_node;
                                            if (!n->isLocation())
                                                YYABORT;
                                            (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new ForInNode(GLOBAL_DATA, (yyvsp[(3) - (7)].expressionNode).m_node, (yyvsp[(5) - (7)].expressionNode).m_node, (yyvsp[(7) - (7)].statementNode).m_node), (yyvsp[(7) - (7)].statementNode).m_varDeclarations, (yyvsp[(7) - (7)].statementNode).m_funcDeclarations,
                                                                                           (yyvsp[(3) - (7)].expressionNode).m_featureInfo | (yyvsp[(5) - (7)].expressionNode).m_featureInfo | (yyvsp[(7) - (7)].statementNode).m_featureInfo);
                                            DBG((yyval.statementNode).m_node, (yylsp[(1) - (7)]), (yylsp[(6) - (7)]));
                                        ;}
    break;

  case 248:
//#line 895 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { ForInNode *forIn = new ForInNode(GLOBAL_DATA, *(yyvsp[(4) - (8)].ident), 0, (yyvsp[(6) - (8)].expressionNode).m_node, (yyvsp[(8) - (8)].statementNode).m_node);
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyvsp[(8) - (8)].statementNode).m_varDeclarations, *(yyvsp[(4) - (8)].ident), DeclarationStacks::HasInitializer);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(forIn, (yyvsp[(8) - (8)].statementNode).m_varDeclarations, (yyvsp[(8) - (8)].statementNode).m_funcDeclarations, (yyvsp[(6) - (8)].expressionNode).m_featureInfo | (yyvsp[(8) - (8)].statementNode).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (8)]), (yylsp[(7) - (8)])); ;}
    break;

  case 249:
//#line 900 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { ForInNode *forIn = new ForInNode(GLOBAL_DATA, *(yyvsp[(4) - (9)].ident), (yyvsp[(5) - (9)].expressionNode).m_node, (yyvsp[(7) - (9)].expressionNode).m_node, (yyvsp[(9) - (9)].statementNode).m_node);
                                          appendToVarDeclarationList(GLOBAL_DATA, (yyvsp[(9) - (9)].statementNode).m_varDeclarations, *(yyvsp[(4) - (9)].ident), DeclarationStacks::HasInitializer);
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(forIn, (yyvsp[(9) - (9)].statementNode).m_varDeclarations, (yyvsp[(9) - (9)].statementNode).m_funcDeclarations,
                                                                                         (yyvsp[(5) - (9)].expressionNode).m_featureInfo | (yyvsp[(7) - (9)].expressionNode).m_featureInfo | (yyvsp[(9) - (9)].statementNode).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (9)]), (yylsp[(8) - (9)])); ;}
    break;

  case 250:
//#line 908 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(0, 0); ;}
    break;

  case 252:
//#line 913 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.expressionNode) = createNodeFeatureInfo<ExpressionNode*>(0, 0); ;}
    break;

  case 254:
//#line 918 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new ContinueNode(GLOBAL_DATA), 0, 0, 0);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 255:
//#line 920 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new ContinueNode(GLOBAL_DATA), 0, 0, 0);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (2)]), (yylsp[(1) - (2)])); AUTO_SEMICOLON; ;}
    break;

  case 256:
//#line 922 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new ContinueNode(GLOBAL_DATA, *(yyvsp[(2) - (3)].ident)), 0, 0, 0);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 257:
//#line 924 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new ContinueNode(GLOBAL_DATA, *(yyvsp[(2) - (3)].ident)), 0, 0, 0);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (3)]), (yylsp[(2) - (3)])); AUTO_SEMICOLON; ;}
    break;

  case 258:
//#line 929 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new BreakNode(GLOBAL_DATA), 0, 0, 0); DBG((yyval.statementNode).m_node, (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 259:
//#line 930 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new BreakNode(GLOBAL_DATA), 0, 0, 0); DBG((yyval.statementNode).m_node, (yylsp[(1) - (2)]), (yylsp[(1) - (2)])); AUTO_SEMICOLON; ;}
    break;

  case 260:
//#line 931 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new BreakNode(GLOBAL_DATA, *(yyvsp[(2) - (3)].ident)), 0, 0, 0); DBG((yyval.statementNode).m_node, (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 261:
//#line 932 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new BreakNode(GLOBAL_DATA, *(yyvsp[(2) - (3)].ident)), 0, 0, 0); DBG((yyval.statementNode).m_node, (yylsp[(1) - (3)]), (yylsp[(2) - (3)])); AUTO_SEMICOLON; ;}
    break;

  case 262:
//#line 936 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new ReturnNode(GLOBAL_DATA, 0), 0, 0, 0); DBG((yyval.statementNode).m_node, (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 263:
//#line 937 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new ReturnNode(GLOBAL_DATA, 0), 0, 0, 0); DBG((yyval.statementNode).m_node, (yylsp[(1) - (2)]), (yylsp[(1) - (2)])); AUTO_SEMICOLON; ;}
    break;

  case 264:
//#line 938 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new ReturnNode(GLOBAL_DATA, (yyvsp[(2) - (3)].expressionNode).m_node), 0, 0, (yyvsp[(2) - (3)].expressionNode).m_featureInfo); DBG((yyval.statementNode).m_node, (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 265:
//#line 939 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new ReturnNode(GLOBAL_DATA, (yyvsp[(2) - (3)].expressionNode).m_node), 0, 0, (yyvsp[(2) - (3)].expressionNode).m_featureInfo); DBG((yyval.statementNode).m_node, (yylsp[(1) - (3)]), (yylsp[(2) - (3)])); AUTO_SEMICOLON; ;}
    break;

  case 266:
//#line 943 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new WithNode(GLOBAL_DATA, (yyvsp[(3) - (5)].expressionNode).m_node, (yyvsp[(5) - (5)].statementNode).m_node), (yyvsp[(5) - (5)].statementNode).m_varDeclarations, (yyvsp[(5) - (5)].statementNode).m_funcDeclarations,
                                                                                         (yyvsp[(3) - (5)].expressionNode).m_featureInfo | (yyvsp[(5) - (5)].statementNode).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (5)]), (yylsp[(4) - (5)])); ;}
    break;

  case 267:
//#line 949 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new SwitchNode(GLOBAL_DATA, (yyvsp[(3) - (5)].expressionNode).m_node, (yyvsp[(5) - (5)].caseBlockNode).m_node), (yyvsp[(5) - (5)].caseBlockNode).m_varDeclarations, (yyvsp[(5) - (5)].caseBlockNode).m_funcDeclarations,
                                                                                         (yyvsp[(3) - (5)].expressionNode).m_featureInfo | (yyvsp[(5) - (5)].caseBlockNode).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (5)]), (yylsp[(4) - (5)])); ;}
    break;

  case 268:
//#line 955 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.caseBlockNode) = createNodeDeclarationInfo<CaseBlockNode*>(new CaseBlockNode(GLOBAL_DATA, (yyvsp[(2) - (3)].clauseList).m_node.head, 0, 0), (yyvsp[(2) - (3)].clauseList).m_varDeclarations, (yyvsp[(2) - (3)].clauseList).m_funcDeclarations, (yyvsp[(2) - (3)].clauseList).m_featureInfo); ;}
    break;

  case 269:
//#line 957 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.caseBlockNode) = createNodeDeclarationInfo<CaseBlockNode*>(new CaseBlockNode(GLOBAL_DATA, (yyvsp[(2) - (5)].clauseList).m_node.head, (yyvsp[(3) - (5)].caseClauseNode).m_node, (yyvsp[(4) - (5)].clauseList).m_node.head),
                                                                                         mergeDeclarationLists(mergeDeclarationLists((yyvsp[(2) - (5)].clauseList).m_varDeclarations, (yyvsp[(3) - (5)].caseClauseNode).m_varDeclarations), (yyvsp[(4) - (5)].clauseList).m_varDeclarations),
                                                                                         mergeDeclarationLists(mergeDeclarationLists((yyvsp[(2) - (5)].clauseList).m_funcDeclarations, (yyvsp[(3) - (5)].caseClauseNode).m_funcDeclarations), (yyvsp[(4) - (5)].clauseList).m_funcDeclarations),
                                                                                         (yyvsp[(2) - (5)].clauseList).m_featureInfo | (yyvsp[(3) - (5)].caseClauseNode).m_featureInfo | (yyvsp[(4) - (5)].clauseList).m_featureInfo); ;}
    break;

  case 270:
//#line 964 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.clauseList).m_node.head = 0; (yyval.clauseList).m_node.tail = 0; (yyval.clauseList).m_varDeclarations = 0; (yyval.clauseList).m_funcDeclarations = 0; (yyval.clauseList).m_featureInfo = 0; ;}
    break;

  case 272:
//#line 969 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.clauseList).m_node.head = new ClauseListNode(GLOBAL_DATA, (yyvsp[(1) - (1)].caseClauseNode).m_node);
                                          (yyval.clauseList).m_node.tail = (yyval.clauseList).m_node.head;
                                          (yyval.clauseList).m_varDeclarations = (yyvsp[(1) - (1)].caseClauseNode).m_varDeclarations;
                                          (yyval.clauseList).m_funcDeclarations = (yyvsp[(1) - (1)].caseClauseNode).m_funcDeclarations; 
                                          (yyval.clauseList).m_featureInfo = (yyvsp[(1) - (1)].caseClauseNode).m_featureInfo; ;}
    break;

  case 273:
//#line 974 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.clauseList).m_node.head = (yyvsp[(1) - (2)].clauseList).m_node.head;
                                          (yyval.clauseList).m_node.tail = new ClauseListNode(GLOBAL_DATA, (yyvsp[(1) - (2)].clauseList).m_node.tail, (yyvsp[(2) - (2)].caseClauseNode).m_node);
                                          (yyval.clauseList).m_varDeclarations = mergeDeclarationLists((yyvsp[(1) - (2)].clauseList).m_varDeclarations, (yyvsp[(2) - (2)].caseClauseNode).m_varDeclarations);
                                          (yyval.clauseList).m_funcDeclarations = mergeDeclarationLists((yyvsp[(1) - (2)].clauseList).m_funcDeclarations, (yyvsp[(2) - (2)].caseClauseNode).m_funcDeclarations);
                                          (yyval.clauseList).m_featureInfo = (yyvsp[(1) - (2)].clauseList).m_featureInfo | (yyvsp[(2) - (2)].caseClauseNode).m_featureInfo;
                                        ;}
    break;

  case 274:
//#line 983 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.caseClauseNode) = createNodeDeclarationInfo<CaseClauseNode*>(new CaseClauseNode(GLOBAL_DATA, (yyvsp[(2) - (3)].expressionNode).m_node), 0, 0, (yyvsp[(2) - (3)].expressionNode).m_featureInfo); ;}
    break;

  case 275:
//#line 984 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.caseClauseNode) = createNodeDeclarationInfo<CaseClauseNode*>(new CaseClauseNode(GLOBAL_DATA, (yyvsp[(2) - (4)].expressionNode).m_node, (yyvsp[(4) - (4)].sourceElements).m_node), (yyvsp[(4) - (4)].sourceElements).m_varDeclarations, (yyvsp[(4) - (4)].sourceElements).m_funcDeclarations, (yyvsp[(2) - (4)].expressionNode).m_featureInfo | (yyvsp[(4) - (4)].sourceElements).m_featureInfo); ;}
    break;

  case 276:
//#line 988 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.caseClauseNode) = createNodeDeclarationInfo<CaseClauseNode*>(new CaseClauseNode(GLOBAL_DATA, 0), 0, 0, 0); ;}
    break;

  case 277:
//#line 989 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.caseClauseNode) = createNodeDeclarationInfo<CaseClauseNode*>(new CaseClauseNode(GLOBAL_DATA, 0, (yyvsp[(3) - (3)].sourceElements).m_node), (yyvsp[(3) - (3)].sourceElements).m_varDeclarations, (yyvsp[(3) - (3)].sourceElements).m_funcDeclarations, (yyvsp[(3) - (3)].sourceElements).m_featureInfo); ;}
    break;

  case 278:
//#line 993 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyvsp[(3) - (3)].statementNode).m_node->pushLabel(*(yyvsp[(1) - (3)].ident));
                                          (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new LabelNode(GLOBAL_DATA, *(yyvsp[(1) - (3)].ident), (yyvsp[(3) - (3)].statementNode).m_node), (yyvsp[(3) - (3)].statementNode).m_varDeclarations, (yyvsp[(3) - (3)].statementNode).m_funcDeclarations, (yyvsp[(3) - (3)].statementNode).m_featureInfo); ;}
    break;

  case 279:
//#line 998 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new ThrowNode(GLOBAL_DATA, (yyvsp[(2) - (3)].expressionNode).m_node), 0, 0, (yyvsp[(2) - (3)].expressionNode).m_featureInfo); DBG((yyval.statementNode).m_node, (yylsp[(1) - (3)]), (yylsp[(3) - (3)])); ;}
    break;

  case 280:
//#line 999 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new ThrowNode(GLOBAL_DATA, (yyvsp[(2) - (3)].expressionNode).m_node), 0, 0, (yyvsp[(2) - (3)].expressionNode).m_featureInfo); DBG((yyval.statementNode).m_node, (yylsp[(1) - (3)]), (yylsp[(2) - (3)])); AUTO_SEMICOLON; ;}
    break;

  case 281:
//#line 1003 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new TryNode(GLOBAL_DATA, (yyvsp[(2) - (4)].statementNode).m_node, GLOBAL_DATA->propertyNames->nullIdentifier, 0, (yyvsp[(4) - (4)].statementNode).m_node),
                                                                                         mergeDeclarationLists((yyvsp[(2) - (4)].statementNode).m_varDeclarations, (yyvsp[(4) - (4)].statementNode).m_varDeclarations),
                                                                                         mergeDeclarationLists((yyvsp[(2) - (4)].statementNode).m_funcDeclarations, (yyvsp[(4) - (4)].statementNode).m_funcDeclarations),
                                                                                         (yyvsp[(2) - (4)].statementNode).m_featureInfo | (yyvsp[(4) - (4)].statementNode).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (4)]), (yylsp[(2) - (4)])); ;}
    break;

  case 282:
//#line 1008 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new TryNode(GLOBAL_DATA, (yyvsp[(2) - (7)].statementNode).m_node, *(yyvsp[(5) - (7)].ident), (yyvsp[(7) - (7)].statementNode).m_node, 0),
                                                                                         mergeDeclarationLists((yyvsp[(2) - (7)].statementNode).m_varDeclarations, (yyvsp[(7) - (7)].statementNode).m_varDeclarations),
                                                                                         mergeDeclarationLists((yyvsp[(2) - (7)].statementNode).m_funcDeclarations, (yyvsp[(7) - (7)].statementNode).m_funcDeclarations),
                                                                                         (yyvsp[(2) - (7)].statementNode).m_featureInfo | (yyvsp[(7) - (7)].statementNode).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (7)]), (yylsp[(2) - (7)])); ;}
    break;

  case 283:
//#line 1014 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new TryNode(GLOBAL_DATA, (yyvsp[(2) - (9)].statementNode).m_node, *(yyvsp[(5) - (9)].ident), (yyvsp[(7) - (9)].statementNode).m_node, (yyvsp[(9) - (9)].statementNode).m_node),
                                                                                         mergeDeclarationLists(mergeDeclarationLists((yyvsp[(2) - (9)].statementNode).m_varDeclarations, (yyvsp[(7) - (9)].statementNode).m_varDeclarations), (yyvsp[(9) - (9)].statementNode).m_varDeclarations),
                                                                                         mergeDeclarationLists(mergeDeclarationLists((yyvsp[(2) - (9)].statementNode).m_funcDeclarations, (yyvsp[(7) - (9)].statementNode).m_funcDeclarations), (yyvsp[(9) - (9)].statementNode).m_funcDeclarations),
                                                                                         (yyvsp[(2) - (9)].statementNode).m_featureInfo | (yyvsp[(7) - (9)].statementNode).m_featureInfo | (yyvsp[(9) - (9)].statementNode).m_featureInfo);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (9)]), (yylsp[(2) - (9)])); ;}
    break;

  case 284:
//#line 1022 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new DebuggerStatementNode(GLOBAL_DATA), 0, 0, 0);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (2)]), (yylsp[(2) - (2)])); ;}
    break;

  case 285:
//#line 1024 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>(new DebuggerStatementNode(GLOBAL_DATA), 0, 0, 0);
                                          DBG((yyval.statementNode).m_node, (yylsp[(1) - (2)]), (yylsp[(1) - (2)])); AUTO_SEMICOLON; ;}
    break;

  case 286:
//#line 1029 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.funcDeclNode) = new FuncDeclNode(GLOBAL_DATA, *(yyvsp[(2) - (7)].ident), (yyvsp[(6) - (7)].functionBodyNode), LEXER->sourceRange((yyvsp[(5) - (7)].intValue), (yyvsp[(7) - (7)].intValue))); DBG((yyvsp[(6) - (7)].functionBodyNode), (yylsp[(5) - (7)]), (yylsp[(7) - (7)])); ;}
    break;

  case 287:
//#line 1031 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.funcDeclNode) = new FuncDeclNode(GLOBAL_DATA, *(yyvsp[(2) - (8)].ident), (yyvsp[(7) - (8)].functionBodyNode), LEXER->sourceRange((yyvsp[(6) - (8)].intValue), (yyvsp[(8) - (8)].intValue)), (yyvsp[(4) - (8)].parameterList).head); DBG((yyvsp[(7) - (8)].functionBodyNode), (yylsp[(6) - (8)]), (yylsp[(8) - (8)])); ;}
    break;

  case 288:
//#line 1035 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.funcExprNode) = createNodeFeatureInfo(new FuncExprNode(GLOBAL_DATA, GLOBAL_DATA->propertyNames->nullIdentifier, (yyvsp[(5) - (6)].functionBodyNode), LEXER->sourceRange((yyvsp[(4) - (6)].intValue), (yyvsp[(6) - (6)].intValue))), ClosureFeature); DBG((yyvsp[(5) - (6)].functionBodyNode), (yylsp[(4) - (6)]), (yylsp[(6) - (6)])); ;}
    break;

  case 289:
//#line 1036 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.funcExprNode) = createNodeFeatureInfo(new FuncExprNode(GLOBAL_DATA, GLOBAL_DATA->propertyNames->nullIdentifier, (yyvsp[(6) - (7)].functionBodyNode), LEXER->sourceRange((yyvsp[(5) - (7)].intValue), (yyvsp[(7) - (7)].intValue)), (yyvsp[(3) - (7)].parameterList).head), ClosureFeature); DBG((yyvsp[(6) - (7)].functionBodyNode), (yylsp[(5) - (7)]), (yylsp[(7) - (7)])); ;}
    break;

  case 290:
//#line 1037 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.funcExprNode) = createNodeFeatureInfo(new FuncExprNode(GLOBAL_DATA, *(yyvsp[(2) - (7)].ident), (yyvsp[(6) - (7)].functionBodyNode), LEXER->sourceRange((yyvsp[(5) - (7)].intValue), (yyvsp[(7) - (7)].intValue))), ClosureFeature); DBG((yyvsp[(6) - (7)].functionBodyNode), (yylsp[(5) - (7)]), (yylsp[(7) - (7)])); ;}
    break;

  case 291:
//#line 1038 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.funcExprNode) = createNodeFeatureInfo(new FuncExprNode(GLOBAL_DATA, *(yyvsp[(2) - (8)].ident), (yyvsp[(7) - (8)].functionBodyNode), LEXER->sourceRange((yyvsp[(6) - (8)].intValue), (yyvsp[(8) - (8)].intValue)), (yyvsp[(4) - (8)].parameterList).head), ClosureFeature); DBG((yyvsp[(7) - (8)].functionBodyNode), (yylsp[(6) - (8)]), (yylsp[(8) - (8)])); ;}
    break;

  case 292:
//#line 1042 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.parameterList).head = new ParameterNode(GLOBAL_DATA, *(yyvsp[(1) - (1)].ident));
                                          (yyval.parameterList).tail = (yyval.parameterList).head; ;}
    break;

  case 293:
//#line 1044 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.parameterList).head = (yyvsp[(1) - (3)].parameterList).head;
                                          (yyval.parameterList).tail = new ParameterNode(GLOBAL_DATA, (yyvsp[(1) - (3)].parameterList).tail, *(yyvsp[(3) - (3)].ident)); ;}
    break;

  case 294:
//#line 1049 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.functionBodyNode) = FunctionBodyNode::create(GLOBAL_DATA, 0, 0, 0, false, false); ;}
    break;

  case 295:
//#line 1050 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.functionBodyNode) = FunctionBodyNode::create(GLOBAL_DATA, (yyvsp[(1) - (1)].sourceElements).m_node, (yyvsp[(1) - (1)].sourceElements).m_varDeclarations ? &(yyvsp[(1) - (1)].sourceElements).m_varDeclarations->data : 0, 
                                                                (yyvsp[(1) - (1)].sourceElements).m_funcDeclarations ? &(yyvsp[(1) - (1)].sourceElements).m_funcDeclarations->data : 0,
                                                                ((yyvsp[(1) - (1)].sourceElements).m_featureInfo & EvalFeature) != 0, ((yyvsp[(1) - (1)].sourceElements).m_featureInfo & ClosureFeature) != 0);
                                  // As in mergeDeclarationLists() we have to ref/deref to safely get rid of
                                  // the declaration lists.
                                  if ((yyvsp[(1) - (1)].sourceElements).m_varDeclarations) {
                                      (yyvsp[(1) - (1)].sourceElements).m_varDeclarations->ref();
                                      (yyvsp[(1) - (1)].sourceElements).m_varDeclarations->deref();
                                  }
                                  if ((yyvsp[(1) - (1)].sourceElements).m_funcDeclarations) {
                                      (yyvsp[(1) - (1)].sourceElements).m_funcDeclarations->ref();
                                      (yyvsp[(1) - (1)].sourceElements).m_funcDeclarations->deref();
                                  }
                                ;}
    break;

  case 296:
//#line 1067 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { GLOBAL_DATA->parser->didFinishParsing(new SourceElements(GLOBAL_DATA), 0, 0, false, false, (yylsp[(0) - (0)]).last_line); ;}
    break;

  case 297:
//#line 1068 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { GLOBAL_DATA->parser->didFinishParsing((yyvsp[(1) - (1)].sourceElements).m_node, (yyvsp[(1) - (1)].sourceElements).m_varDeclarations, (yyvsp[(1) - (1)].sourceElements).m_funcDeclarations, 
                                                                    ((yyvsp[(1) - (1)].sourceElements).m_featureInfo & EvalFeature) != 0, ((yyvsp[(1) - (1)].sourceElements).m_featureInfo & ClosureFeature) != 0,
                                                                    (yylsp[(1) - (1)]).last_line); ;}
    break;

  case 298:
//#line 1074 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.sourceElements).m_node = new SourceElements(GLOBAL_DATA);
                                          (yyval.sourceElements).m_node->append((yyvsp[(1) - (1)].statementNode).m_node);
                                          (yyval.sourceElements).m_varDeclarations = (yyvsp[(1) - (1)].statementNode).m_varDeclarations;
                                          (yyval.sourceElements).m_funcDeclarations = (yyvsp[(1) - (1)].statementNode).m_funcDeclarations;
                                          (yyval.sourceElements).m_featureInfo = (yyvsp[(1) - (1)].statementNode).m_featureInfo;
                                        ;}
    break;

  case 299:
//#line 1080 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.sourceElements).m_node->append((yyvsp[(2) - (2)].statementNode).m_node);
                                          (yyval.sourceElements).m_varDeclarations = mergeDeclarationLists((yyvsp[(1) - (2)].sourceElements).m_varDeclarations, (yyvsp[(2) - (2)].statementNode).m_varDeclarations);
                                          (yyval.sourceElements).m_funcDeclarations = mergeDeclarationLists((yyvsp[(1) - (2)].sourceElements).m_funcDeclarations, (yyvsp[(2) - (2)].statementNode).m_funcDeclarations);
                                          (yyval.sourceElements).m_featureInfo = (yyvsp[(1) - (2)].sourceElements).m_featureInfo | (yyvsp[(2) - (2)].statementNode).m_featureInfo;
                                        ;}
    break;

  case 300:
//#line 1088 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = createNodeDeclarationInfo<StatementNode*>((yyvsp[(1) - (1)].funcDeclNode), 0, new ParserRefCountedData<DeclarationStacks::FunctionStack>(GLOBAL_DATA), ClosureFeature); (yyval.statementNode).m_funcDeclarations->data.append((yyvsp[(1) - (1)].funcDeclNode)); ;}
    break;

  case 301:
//#line 1089 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"
    { (yyval.statementNode) = (yyvsp[(1) - (1)].statementNode); ;}
    break;


/* Line 1267 of yacc.c.  */
//#line 3662 "grammar.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

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

  yyerror_range[0] = yylloc;

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
		      yytoken, &yylval, &yylloc);
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

  yyerror_range[0] = yylsp[1-yylen];
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

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the look-ahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

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
		 yytoken, &yylval, &yylloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
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


//#line 1092 "D:/Development/Internet/OWB/doduo/JavaScriptCore/kjs/grammar.y"


static ExpressionNode* makeAssignNode(void* globalPtr, ExpressionNode* loc, Operator op, ExpressionNode* expr, bool locHasAssignments, bool exprHasAssignments)
{
    if (!loc->isLocation())
        return new AssignErrorNode(GLOBAL_DATA, loc, op, expr);

    if (loc->isResolveNode()) {
        ResolveNode* resolve = static_cast<ResolveNode*>(loc);
        if (op == OpEqual)
            return new AssignResolveNode(GLOBAL_DATA, resolve->identifier(), expr, exprHasAssignments);
        else
            return new ReadModifyResolveNode(GLOBAL_DATA, resolve->identifier(), op, expr, exprHasAssignments);
    }
    if (loc->isBracketAccessorNode()) {
        BracketAccessorNode* bracket = static_cast<BracketAccessorNode*>(loc);
        if (op == OpEqual)
            return new AssignBracketNode(GLOBAL_DATA, bracket->base(), bracket->subscript(), expr, locHasAssignments, exprHasAssignments);
        else
            return new ReadModifyBracketNode(GLOBAL_DATA, bracket->base(), bracket->subscript(), op, expr, locHasAssignments, exprHasAssignments);
    }
    ASSERT(loc->isDotAccessorNode());
    DotAccessorNode* dot = static_cast<DotAccessorNode*>(loc);
    if (op == OpEqual)
        return new AssignDotNode(GLOBAL_DATA, dot->base(), dot->identifier(), expr, exprHasAssignments);
    return new ReadModifyDotNode(GLOBAL_DATA, dot->base(), dot->identifier(), op, expr, exprHasAssignments);
}

static ExpressionNode* makePrefixNode(void* globalPtr, ExpressionNode* expr, Operator op)
{ 
    if (!expr->isLocation())
        return new PrefixErrorNode(GLOBAL_DATA, expr, op);
    
    if (expr->isResolveNode()) {
        ResolveNode* resolve = static_cast<ResolveNode*>(expr);
        if (op == OpPlusPlus)
            return new PreIncResolveNode(GLOBAL_DATA, resolve->identifier());
        else
            return new PreDecResolveNode(GLOBAL_DATA, resolve->identifier());
    }
    if (expr->isBracketAccessorNode()) {
        BracketAccessorNode* bracket = static_cast<BracketAccessorNode*>(expr);
        if (op == OpPlusPlus)
            return new PreIncBracketNode(GLOBAL_DATA, bracket->base(), bracket->subscript());
        else
            return new PreDecBracketNode(GLOBAL_DATA, bracket->base(), bracket->subscript());
    }
    ASSERT(expr->isDotAccessorNode());
    DotAccessorNode* dot = static_cast<DotAccessorNode*>(expr);
    if (op == OpPlusPlus)
        return new PreIncDotNode(GLOBAL_DATA, dot->base(), dot->identifier());
    return new PreDecDotNode(GLOBAL_DATA, dot->base(), dot->identifier());
}

static ExpressionNode* makePostfixNode(void* globalPtr, ExpressionNode* expr, Operator op)
{ 
    if (!expr->isLocation())
        return new PostfixErrorNode(GLOBAL_DATA, expr, op);
    
    if (expr->isResolveNode()) {
        ResolveNode* resolve = static_cast<ResolveNode*>(expr);
        if (op == OpPlusPlus)
            return new PostIncResolveNode(GLOBAL_DATA, resolve->identifier());
        else
            return new PostDecResolveNode(GLOBAL_DATA, resolve->identifier());
    }
    if (expr->isBracketAccessorNode()) {
        BracketAccessorNode* bracket = static_cast<BracketAccessorNode*>(expr);
        if (op == OpPlusPlus)
            return new PostIncBracketNode(GLOBAL_DATA, bracket->base(), bracket->subscript());
        else
            return new PostDecBracketNode(GLOBAL_DATA, bracket->base(), bracket->subscript());
    }
    ASSERT(expr->isDotAccessorNode());
    DotAccessorNode* dot = static_cast<DotAccessorNode*>(expr);
    
    if (op == OpPlusPlus)
        return new PostIncDotNode(GLOBAL_DATA, dot->base(), dot->identifier());
    return new PostDecDotNode(GLOBAL_DATA, dot->base(), dot->identifier());
}

static ExpressionNodeInfo makeFunctionCallNode(void* globalPtr, ExpressionNodeInfo func, ArgumentsNodeInfo args)
{
    FeatureInfo features = func.m_featureInfo | args.m_featureInfo;
    if (!func.m_node->isLocation())
        return createNodeFeatureInfo<ExpressionNode*>(new FunctionCallValueNode(GLOBAL_DATA, func.m_node, args.m_node), features);
    if (func.m_node->isResolveNode()) {
        ResolveNode* resolve = static_cast<ResolveNode*>(func.m_node);
        const Identifier& identifier = resolve->identifier();
        if (identifier == GLOBAL_DATA->propertyNames->eval)
            return createNodeFeatureInfo<ExpressionNode*>(new EvalFunctionCallNode(GLOBAL_DATA, args.m_node), EvalFeature | features);
        return createNodeFeatureInfo<ExpressionNode*>(new FunctionCallResolveNode(GLOBAL_DATA, identifier, args.m_node), features);
    }
    if (func.m_node->isBracketAccessorNode()) {
        BracketAccessorNode* bracket = static_cast<BracketAccessorNode*>(func.m_node);
        return createNodeFeatureInfo<ExpressionNode*>(new FunctionCallBracketNode(GLOBAL_DATA, bracket->base(), bracket->subscript(), args.m_node), features);
    }
    ASSERT(func.m_node->isDotAccessorNode());
    DotAccessorNode* dot = static_cast<DotAccessorNode*>(func.m_node);
    return createNodeFeatureInfo<ExpressionNode*>(new FunctionCallDotNode(GLOBAL_DATA, dot->base(), dot->identifier(), args.m_node), features);
}

static ExpressionNode* makeTypeOfNode(void* globalPtr, ExpressionNode* expr)
{
    if (expr->isResolveNode()) {
        ResolveNode* resolve = static_cast<ResolveNode*>(expr);
        return new TypeOfResolveNode(GLOBAL_DATA, resolve->identifier());
    }
    return new TypeOfValueNode(GLOBAL_DATA, expr);
}

static ExpressionNode* makeDeleteNode(void* globalPtr, ExpressionNode* expr)
{
    if (!expr->isLocation())
        return new DeleteValueNode(GLOBAL_DATA, expr);
    if (expr->isResolveNode()) {
        ResolveNode* resolve = static_cast<ResolveNode*>(expr);
        return new DeleteResolveNode(GLOBAL_DATA, resolve->identifier());
    }
    if (expr->isBracketAccessorNode()) {
        BracketAccessorNode* bracket = static_cast<BracketAccessorNode*>(expr);
        return new DeleteBracketNode(GLOBAL_DATA, bracket->base(), bracket->subscript());
    }
    ASSERT(expr->isDotAccessorNode());
    DotAccessorNode* dot = static_cast<DotAccessorNode*>(expr);
    return new DeleteDotNode(GLOBAL_DATA, dot->base(), dot->identifier());
}

static PropertyNode* makeGetterOrSetterPropertyNode(void* globalPtr, const Identifier& getOrSet, const Identifier& name, ParameterNode* params, FunctionBodyNode* body, const SourceRange& source)
{
    PropertyNode::Type type;
    if (getOrSet == "get")
        type = PropertyNode::Getter;
    else if (getOrSet == "set")
        type = PropertyNode::Setter;
    else
        return 0;
    return new PropertyNode(GLOBAL_DATA, name, new FuncExprNode(GLOBAL_DATA, GLOBAL_DATA->propertyNames->nullIdentifier, body, source, params), type);
}

static ExpressionNode* makeNegateNode(void* globalPtr, ExpressionNode* n)
{
    if (n->isNumber()) {
        NumberNode* number = static_cast<NumberNode*>(n);

        if (number->value() > 0.0) {
            number->setValue(-number->value());
            return number;
        }
    }

    return new NegateNode(GLOBAL_DATA, n);
}

static NumberNode* makeNumberNode(void* globalPtr, double d)
{
    JSValue* value = JSImmediate::from(d);
    if (value)
        return new ImmediateNumberNode(GLOBAL_DATA, value, d);
    return new NumberNode(GLOBAL_DATA, d);
}

/* called by yyparse on error */
int yyerror(const char *)
{
    return 1;
}

/* may we automatically insert a semicolon ? */
static bool allowAutomaticSemicolon(Lexer& lexer, int yychar)
{
    return yychar == CLOSEBRACE || yychar == 0 || lexer.prevTerminator();
}

static ExpressionNode* combineVarInitializers(void* globalPtr, ExpressionNode* list, AssignResolveNode* init)
{
    if (!list)
        return init;
    return new VarDeclCommaNode(GLOBAL_DATA, list, init);
}

// We turn variable declarations into either assignments or empty
// statements (which later get stripped out), because the actual
// declaration work is hoisted up to the start of the function body
static StatementNode* makeVarStatementNode(void* globalPtr, ExpressionNode* expr)
{
    if (!expr)
        return new EmptyStatementNode(GLOBAL_DATA);
    return new VarStatementNode(GLOBAL_DATA, expr);
}

#undef GLOBAL_DATA

