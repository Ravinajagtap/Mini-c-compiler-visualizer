
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.7.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtable.h"
#include "tac.h"

char token_output[50000];
char parse_output[50000];
char symtable_output[50000];
char tac_output[50000];
char tree_output[100000];

/* Semantic error/warning buffer */
char semantic_output[50000];

void append_to(char *buf, const char *text) {
    strncat(buf, text, 50000 - strlen(buf) - 1);
}

void tree_append(const char *s) {
    strncat(tree_output, s, 100000 - strlen(tree_output) - 1);
}

void tree_leaf(const char *name) {
    char buf[256];
    char safe[128];
    int i=0, j=0;
    while(name[i] && j<126) {
        if(name[i]=='"') { safe[j++]='\\'; }
        safe[j++] = name[i++];
    }
    safe[j]='\0';
    snprintf(buf, sizeof(buf), "{\"name\":\"%s\"}", safe);
    tree_append(buf);
}

void tree_leaf_c(const char *name) {
    tree_append(",");
    tree_leaf(name);
}

void fix_tree_commas() {
    char fixed[100000];
    char *src = tree_output;
    char *dst = fixed;
    while (*src) {
        if (src[0] == ',' && src[1] == ',') {
            *dst++ = ',';
            src += 2;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
    strncpy(tree_output, fixed, 99999);
    int len = strlen(tree_output);
    if (len > 0 && tree_output[len-1] == ',')
        tree_output[len-1] = '\0';
}

void tac_emit_and_append(const char *line) {
    char clean[256];
    strncpy(clean, line, 255);
    clean[255] = '\0';
    int len = strlen(clean);
    if (len > 0 && clean[len-1] == '\n')
        clean[len-1] = '\0';
    if (strlen(clean) > 0)
        emit_tac(clean);
    append_to(tac_output, line);
}

/* ============================================================
   SEMANTIC CHECK FUNCTIONS
   ============================================================ */

/* 1. Check if variable was declared */
void sem_check_declared(const char *name) {
    if (lookup_symbol(name) == NULL) {
        char msg[256];
        snprintf(msg, sizeof(msg),
            "[SEMANTIC ERROR] Variable '%s' used without declaration.\n", name);
        append_to(semantic_output, msg);
        append_to(parse_output,    msg);
    }
}

/* 2. Check if variable is initialized before use */
void sem_check_initialized(const char *name) {
    Symbol *s = lookup_symbol(name);
    if (s != NULL && s->is_initialized == 0) {
        char msg[256];
        snprintf(msg, sizeof(msg),
            "[SEMANTIC WARNING] Variable '%s' used before initialization.\n", name);
        append_to(semantic_output, msg);
        append_to(parse_output,    msg);
    }
}

/* 3. Check type mismatch — int var assigned float value */
void sem_check_type(const char *varname, const char *valtype) {
    Symbol *s = lookup_symbol(varname);
    if (s == NULL) return;
    if (strcmp(s->type, "int") == 0 && strcmp(valtype, "float") == 0) {
        char msg[256];
        snprintf(msg, sizeof(msg),
            "[SEMANTIC WARNING] Type mismatch: assigning float value to int variable '%s'. Precision loss may occur.\n",
            varname);
        append_to(semantic_output, msg);
        append_to(parse_output,    msg);
    }
}

/* 4. Check division by zero */
void sem_check_div_zero(const char *divisor) {
    if (strcmp(divisor, "0") == 0) {
        char msg[256];
        snprintf(msg, sizeof(msg),
            "[SEMANTIC ERROR] Division by zero detected.\n");
        append_to(semantic_output, msg);
        append_to(parse_output,    msg);
    }
}

/* 5. Check void function return */
void sem_check_void_return(const char *funcname, const char *retval) {
    if (strcmp(funcname, "void") == 0 && retval != NULL) {
        char msg[256];
        snprintf(msg, sizeof(msg),
            "[SEMANTIC WARNING] void function should not return a value.\n");
        append_to(semantic_output, msg);
        append_to(parse_output,    msg);
    }
}

/* Track current function return type */
char current_func_type[32] = "";

void yyerror(const char *msg) {
    char err[256];
    snprintf(err, sizeof(err), "Syntax Error: %s\n", msg);
    append_to(parse_output, err);
    fprintf(stderr, "%s\n", err);
}

extern int yylex();
extern int line_num;

int temp_count  = 0;
int label_count = 0;

char* new_temp() {
    char *t = malloc(10);
    snprintf(t, 10, "t%d", ++temp_count);
    return t;
}

char* new_label() {
    char *l = malloc(10);
    snprintf(l, 10, "L%d", ++label_count);
    return l;
}


#line 238 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT = 3,                        /* INT  */
  YYSYMBOL_FLOAT_TYPE = 4,                 /* FLOAT_TYPE  */
  YYSYMBOL_VOID = 5,                       /* VOID  */
  YYSYMBOL_IF = 6,                         /* IF  */
  YYSYMBOL_ELSE = 7,                       /* ELSE  */
  YYSYMBOL_WHILE = 8,                      /* WHILE  */
  YYSYMBOL_RETURN = 9,                     /* RETURN  */
  YYSYMBOL_NUMBER = 10,                    /* NUMBER  */
  YYSYMBOL_FLOAT_NUM = 11,                 /* FLOAT_NUM  */
  YYSYMBOL_IDENTIFIER = 12,                /* IDENTIFIER  */
  YYSYMBOL_PLUS = 13,                      /* PLUS  */
  YYSYMBOL_MINUS = 14,                     /* MINUS  */
  YYSYMBOL_MULTIPLY = 15,                  /* MULTIPLY  */
  YYSYMBOL_DIVIDE = 16,                    /* DIVIDE  */
  YYSYMBOL_ASSIGN = 17,                    /* ASSIGN  */
  YYSYMBOL_EQ = 18,                        /* EQ  */
  YYSYMBOL_NEQ = 19,                       /* NEQ  */
  YYSYMBOL_LT = 20,                        /* LT  */
  YYSYMBOL_GT = 21,                        /* GT  */
  YYSYMBOL_LEQ = 22,                       /* LEQ  */
  YYSYMBOL_GEQ = 23,                       /* GEQ  */
  YYSYMBOL_AND = 24,                       /* AND  */
  YYSYMBOL_OR = 25,                        /* OR  */
  YYSYMBOL_NOT = 26,                       /* NOT  */
  YYSYMBOL_LPAREN = 27,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 28,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 29,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 30,                    /* RBRACE  */
  YYSYMBOL_SEMICOLON = 31,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 32,                     /* COMMA  */
  YYSYMBOL_UMINUS = 33,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 34,                  /* $accept  */
  YYSYMBOL_program = 35,                   /* program  */
  YYSYMBOL_decl_list = 36,                 /* decl_list  */
  YYSYMBOL_decl = 37,                      /* decl  */
  YYSYMBOL_func_decl = 38,                 /* func_decl  */
  YYSYMBOL_param_list = 39,                /* param_list  */
  YYSYMBOL_param = 40,                     /* param  */
  YYSYMBOL_var_decl = 41,                  /* var_decl  */
  YYSYMBOL_stmt_list = 42,                 /* stmt_list  */
  YYSYMBOL_stmt = 43,                      /* stmt  */
  YYSYMBOL_assign_stmt = 44,               /* assign_stmt  */
  YYSYMBOL_if_stmt = 45,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 46,                /* while_stmt  */
  YYSYMBOL_return_stmt = 47,               /* return_stmt  */
  YYSYMBOL_condition = 48,                 /* condition  */
  YYSYMBOL_expr = 49,                      /* expr  */
  YYSYMBOL_term = 50,                      /* term  */
  YYSYMBOL_factor = 51                     /* factor  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   175

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  34
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  48
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  112

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   288


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   201,   201,   223,   224,   228,   229,   233,   248,   262,
     279,   280,   284,   292,   300,   318,   342,   363,   380,   381,
     385,   386,   387,   388,   389,   390,   397,   426,   439,   455,
     471,   495,   512,   527,   541,   552,   563,   574,   588,   597,
     606,   613,   622,   634,   641,   647,   653,   663,   667
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INT", "FLOAT_TYPE",
  "VOID", "IF", "ELSE", "WHILE", "RETURN", "NUMBER", "FLOAT_NUM",
  "IDENTIFIER", "PLUS", "MINUS", "MULTIPLY", "DIVIDE", "ASSIGN", "EQ",
  "NEQ", "LT", "GT", "LEQ", "GEQ", "AND", "OR", "NOT", "LPAREN", "RPAREN",
  "LBRACE", "RBRACE", "SEMICOLON", "COMMA", "UMINUS", "$accept", "program",
  "decl_list", "decl", "func_decl", "param_list", "param", "var_decl",
  "stmt_list", "stmt", "assign_stmt", "if_stmt", "while_stmt",
  "return_stmt", "condition", "expr", "term", "factor", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288
};
#endif

#define YYPACT_NINF (-54)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      77,    16,    29,    34,    39,    77,   -54,   -54,   -54,   125,
      -9,    21,   -54,   -54,   127,    31,   -54,   127,   -54,   109,
     -54,   -54,   -54,   127,   127,    18,   128,   -54,    73,    79,
      68,   -27,   -54,    41,   -12,   -54,    38,   127,   127,   -54,
     127,   127,   -54,   -54,   -54,    86,   109,   -54,    93,   -54,
     128,   128,   -54,   -54,     3,   -54,   -54,   -54,    97,   101,
     126,    26,   138,   -54,   -54,   -54,   -54,   -54,   -54,   -54,
      70,    15,    59,    27,   127,   127,   -54,   103,   127,   -54,
     -54,   -54,   131,   144,   132,   -54,   105,   139,   127,   127,
     127,   127,   127,   127,   141,   -54,   -54,   133,   133,   133,
     133,   133,   133,   -54,    84,    96,   154,   -54,   142,   -54,
     121,   -54
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     2,     4,     5,     6,     0,
       0,     0,     1,     3,     0,     0,    14,     0,    17,     0,
      44,    45,    46,     0,     0,     0,    40,    43,     0,     0,
       0,     0,    11,     0,     0,    48,     0,     0,     0,    15,
       0,     0,    12,    13,    19,     0,     0,    16,     0,    47,
      38,    39,    41,    42,     0,    19,    10,    19,     0,     0,
       0,     0,    46,     9,    20,    18,    21,    22,    23,    24,
       0,     0,     0,     0,     0,     0,    31,     0,     0,    25,
       7,     8,     0,     0,     0,    30,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    26,    19,    34,    35,    32,
      33,    36,    37,    19,     0,     0,    28,    29,     0,    19,
       0,    27
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -54,   -54,   -54,   164,   -54,   153,   129,   140,   -53,   -54,
     -54,   -54,   -54,   -54,    98,   -14,   112,    80
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,     7,    31,    32,    64,    54,    65,
      66,    67,    68,    69,    82,    70,    26,    27
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      25,    45,    71,    33,    72,    46,    58,     2,    17,    59,
      36,    60,    61,    20,    21,    62,    48,    23,    58,     2,
      46,    59,    18,    60,    61,    20,    21,    62,     9,    23,
      24,    37,    38,    63,    28,    29,    20,    21,    22,    12,
      23,    10,    24,   104,    14,    80,    11,    77,    19,    39,
     105,    37,    38,    24,    37,    38,   110,    76,    16,    30,
      83,    83,    58,     2,    86,    59,    49,    60,    61,    20,
      21,    62,    47,    23,    97,    98,    99,   100,   101,   102,
       1,     2,     3,    37,    38,    42,    24,    58,     2,    81,
      59,    43,    60,    61,    20,    21,    62,    44,    23,    58,
       2,    79,    59,    35,    60,    61,    20,    21,    62,    73,
      23,    24,    28,    29,   106,    55,    37,    38,    37,    38,
      52,    53,    57,    24,    58,     2,   107,    59,    74,    60,
      61,    20,    21,    62,    85,    23,    95,    20,    21,    22,
       8,    23,    14,    40,    41,     8,    37,    38,    24,    50,
      51,   111,    15,    75,    24,    78,    16,    37,    38,    87,
      94,   108,    88,    89,    90,    91,    92,    93,    96,    13,
     103,   109,    34,    84,     0,    56
};

static const yytype_int8 yycheck[] =
{
      14,    28,    55,    17,    57,    32,     3,     4,    17,     6,
      24,     8,     9,    10,    11,    12,    28,    14,     3,     4,
      32,     6,    31,     8,     9,    10,    11,    12,    12,    14,
      27,    13,    14,    30,     3,     4,    10,    11,    12,     0,
      14,    12,    27,    96,    17,    30,    12,    61,    27,    31,
     103,    13,    14,    27,    13,    14,   109,    31,    31,    28,
      74,    75,     3,     4,    78,     6,    28,     8,     9,    10,
      11,    12,    31,    14,    88,    89,    90,    91,    92,    93,
       3,     4,     5,    13,    14,    12,    27,     3,     4,    30,
       6,    12,     8,     9,    10,    11,    12,    29,    14,     3,
       4,    31,     6,    23,     8,     9,    10,    11,    12,    12,
      14,    27,     3,     4,    30,    29,    13,    14,    13,    14,
      40,    41,    29,    27,     3,     4,    30,     6,    27,     8,
       9,    10,    11,    12,    31,    14,    31,    10,    11,    12,
       0,    14,    17,    15,    16,     5,    13,    14,    27,    37,
      38,    30,    27,    27,    27,    17,    31,    13,    14,    28,
      28,     7,    18,    19,    20,    21,    22,    23,    29,     5,
      29,    29,    19,    75,    -1,    46
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,    35,    36,    37,    38,    41,    12,
      12,    12,     0,    37,    17,    27,    31,    17,    31,    27,
      10,    11,    12,    14,    27,    49,    50,    51,     3,     4,
      28,    39,    40,    49,    39,    51,    49,    13,    14,    31,
      15,    16,    12,    12,    29,    28,    32,    31,    28,    28,
      50,    50,    51,    51,    42,    29,    40,    29,     3,     6,
       8,     9,    12,    30,    41,    43,    44,    45,    46,    47,
      49,    42,    42,    12,    27,    27,    31,    49,    17,    31,
      30,    30,    48,    49,    48,    31,    49,    28,    18,    19,
      20,    21,    22,    23,    28,    31,    29,    49,    49,    49,
      49,    49,    49,    29,    42,    42,    30,    30,     7,    29,
      42,    30
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    34,    35,    36,    36,    37,    37,    38,    38,    38,
      39,    39,    40,    40,    41,    41,    41,    41,    42,    42,
      43,    43,    43,    43,    43,    43,    44,    45,    45,    46,
      47,    47,    48,    48,    48,    48,    48,    48,    49,    49,
      49,    50,    50,    50,    51,    51,    51,    51,    51
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     8,     8,     7,
       3,     1,     2,     2,     3,     5,     5,     3,     2,     0,
       1,     1,     1,     1,     1,     2,     4,    11,     7,     7,
       3,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     3,     3,     1,     1,     1,     1,     3,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;



yynewstate:

  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
      if (yytable_value_is_error (yyn))
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: decl_list  */
#line 202 "parser.y"
        {
            fix_tree_commas();
            char full[110000];
            snprintf(full, sizeof(full),
                "{\"name\":\"program\",\"children\":[%s]}",
                tree_output);
            strncpy(tree_output, full, 99999);

            /* Add semantic summary to parse output */
            if (strlen(semantic_output) == 0) {
                append_to(parse_output,
                    "\n[SEMANTIC] All checks passed. No errors found.\n");
            } else {
                append_to(parse_output,
                    "\n[SEMANTIC] Checks completed. See warnings/errors above.\n");
            }
            append_to(parse_output, "Program parsed successfully.\n");
        }
#line 1367 "parser.tab.c"
    break;

  case 7: /* func_decl: INT IDENTIFIER LPAREN param_list RPAREN LBRACE stmt_list RBRACE  */
#line 234 "parser.y"
        {
            /* Semantic: track function type for return check */
            strncpy(current_func_type, "int", 31);

            char node[256];
            snprintf(node, sizeof(node),
                "{\"name\":\"func: int %s()\",\"children\":[]},", (yyvsp[-6].sval));
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Function declared: int %s()\n", (yyvsp[-6].sval));
            append_to(parse_output, msg);
            free((yyvsp[-6].sval));
        }
#line 1386 "parser.tab.c"
    break;

  case 8: /* func_decl: VOID IDENTIFIER LPAREN param_list RPAREN LBRACE stmt_list RBRACE  */
#line 249 "parser.y"
        {
            strncpy(current_func_type, "void", 31);

            char node[256];
            snprintf(node, sizeof(node),
                "{\"name\":\"func: void %s\",\"children\":[]},", (yyvsp[-6].sval));
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Function declared: void %s(...)\n", (yyvsp[-6].sval));
            append_to(parse_output, msg);
            free((yyvsp[-6].sval));
        }
#line 1404 "parser.tab.c"
    break;

  case 9: /* func_decl: INT IDENTIFIER LPAREN RPAREN LBRACE stmt_list RBRACE  */
#line 263 "parser.y"
        {
            strncpy(current_func_type, "int", 31);

            char node[256];
            snprintf(node, sizeof(node),
                "{\"name\":\"func: int %s()\",\"children\":[]},", (yyvsp[-5].sval));
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Function declared: int %s()\n", (yyvsp[-5].sval));
            append_to(parse_output, msg);
            free((yyvsp[-5].sval));
        }
#line 1422 "parser.tab.c"
    break;

  case 12: /* param: INT IDENTIFIER  */
#line 285 "parser.y"
        {
            insert_symbol((yyvsp[0].sval), "int", 0);
            char msg[128];
            snprintf(msg, sizeof(msg), "Parameter: int %s\n", (yyvsp[0].sval));
            append_to(parse_output, msg);
            free((yyvsp[0].sval));
        }
#line 1434 "parser.tab.c"
    break;

  case 13: /* param: FLOAT_TYPE IDENTIFIER  */
#line 293 "parser.y"
        {
            insert_symbol((yyvsp[0].sval), "float", 0);
            free((yyvsp[0].sval));
        }
#line 1443 "parser.tab.c"
    break;

  case 14: /* var_decl: INT IDENTIFIER SEMICOLON  */
#line 301 "parser.y"
        {
            insert_symbol((yyvsp[-1].sval), "int", 0);
            char node[256];
            snprintf(node, sizeof(node),
                "{\"name\":\"var_decl\",\"children\":[{\"name\":\"int\"},{\"name\":\"%s\"}]},",
                (yyvsp[-1].sval));
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Variable declared: int %s\n", (yyvsp[-1].sval));
            append_to(parse_output, msg);
            char tac[128];
            snprintf(tac, sizeof(tac), "declare int %s\n", (yyvsp[-1].sval));
            tac_emit_and_append(tac);
            free((yyvsp[-1].sval));
        }
#line 1464 "parser.tab.c"
    break;

  case 15: /* var_decl: INT IDENTIFIER ASSIGN expr SEMICOLON  */
#line 319 "parser.y"
        {
            insert_symbol((yyvsp[-3].sval), "int", 0);
            update_symbol((yyvsp[-3].sval), 0);

            /* Semantic: type check — warn if float assigned to int */
            sem_check_type((yyvsp[-3].sval), "float_check");

            char node[512];
            snprintf(node, sizeof(node),
                "{\"name\":\"var_decl\",\"children\":[{\"name\":\"int\"},{\"name\":\"%s\"},{\"name\":\"=\"},{\"name\":\"%s\"}]},",
                (yyvsp[-3].sval), (yyvsp[-1].sval));
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Variable declared + assigned: int %s = %s\n", (yyvsp[-3].sval), (yyvsp[-1].sval));
            append_to(parse_output, msg);
            char tac[128];
            snprintf(tac, sizeof(tac), "%s = %s\n", (yyvsp[-3].sval), (yyvsp[-1].sval));
            tac_emit_and_append(tac);
            free((yyvsp[-3].sval));
            if((yyvsp[-1].sval)) free((yyvsp[-1].sval));
        }
#line 1491 "parser.tab.c"
    break;

  case 16: /* var_decl: FLOAT_TYPE IDENTIFIER ASSIGN expr SEMICOLON  */
#line 343 "parser.y"
        {
            insert_symbol((yyvsp[-3].sval), "float", 0);
            update_symbol((yyvsp[-3].sval), 0);

            char node[512];
            snprintf(node, sizeof(node),
                "{\"name\":\"var_decl\",\"children\":[{\"name\":\"float\"},{\"name\":\"%s\"},{\"name\":\"=\"},{\"name\":\"%s\"}]},",
                (yyvsp[-3].sval), (yyvsp[-1].sval));
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Variable declared: float %s = %s\n", (yyvsp[-3].sval), (yyvsp[-1].sval));
            append_to(parse_output, msg);
            char tac[128];
            snprintf(tac, sizeof(tac), "%s = %s\n", (yyvsp[-3].sval), (yyvsp[-1].sval));
            tac_emit_and_append(tac);
            free((yyvsp[-3].sval));
            if((yyvsp[-1].sval)) free((yyvsp[-1].sval));
        }
#line 1515 "parser.tab.c"
    break;

  case 17: /* var_decl: FLOAT_TYPE IDENTIFIER SEMICOLON  */
#line 364 "parser.y"
        {
            insert_symbol((yyvsp[-1].sval), "float", 0);
            char node[256];
            snprintf(node, sizeof(node),
                "{\"name\":\"var_decl\",\"children\":[{\"name\":\"float\"},{\"name\":\"%s\"}]},",
                (yyvsp[-1].sval));
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Variable declared: float %s\n", (yyvsp[-1].sval));
            append_to(parse_output, msg);
            free((yyvsp[-1].sval));
        }
#line 1533 "parser.tab.c"
    break;

  case 25: /* stmt: expr SEMICOLON  */
#line 391 "parser.y"
        {
            if((yyvsp[-1].sval)) free((yyvsp[-1].sval));
        }
#line 1541 "parser.tab.c"
    break;

  case 26: /* assign_stmt: IDENTIFIER ASSIGN expr SEMICOLON  */
#line 398 "parser.y"
        {
            /* Semantic Rule 1: Check variable declared */
            sem_check_declared((yyvsp[-3].sval));

            /* Semantic Rule 2: Check initialized (reading $3 which may be a var) */
            Symbol *s = lookup_symbol((yyvsp[-3].sval));
            if (s != NULL) {
                update_symbol((yyvsp[-3].sval), 0);
            }

            char node[512];
            snprintf(node, sizeof(node),
                "{\"name\":\"assign\",\"children\":[{\"name\":\"%s\"},{\"name\":\"=\"},{\"name\":\"%s\"}]},",
                (yyvsp[-3].sval), (yyvsp[-1].sval));
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Assignment: %s = %s\n", (yyvsp[-3].sval), (yyvsp[-1].sval));
            append_to(parse_output, msg);
            char tac[128];
            snprintf(tac, sizeof(tac), "%s = %s\n", (yyvsp[-3].sval), (yyvsp[-1].sval));
            tac_emit_and_append(tac);
            free((yyvsp[-3].sval));
            if((yyvsp[-1].sval)) free((yyvsp[-1].sval));
        }
#line 1571 "parser.tab.c"
    break;

  case 27: /* if_stmt: IF LPAREN condition RPAREN LBRACE stmt_list RBRACE ELSE LBRACE stmt_list RBRACE  */
#line 427 "parser.y"
        {
            char node[512];
            snprintf(node, sizeof(node),
                "{\"name\":\"if_else\",\"children\":[{\"name\":\"cond: %s\"},{\"name\":\"then\"},{\"name\":\"else\"}]},",
                (yyvsp[-8].sval));
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "If-else statement parsed. Condition: %s\n", (yyvsp[-8].sval));
            append_to(parse_output, msg);
            if((yyvsp[-8].sval)) free((yyvsp[-8].sval));
        }
#line 1588 "parser.tab.c"
    break;

  case 28: /* if_stmt: IF LPAREN condition RPAREN LBRACE stmt_list RBRACE  */
#line 440 "parser.y"
        {
            char node[512];
            snprintf(node, sizeof(node),
                "{\"name\":\"if_stmt\",\"children\":[{\"name\":\"cond: %s\"},{\"name\":\"then\"}]},",
                (yyvsp[-4].sval));
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "If statement parsed. Condition: %s\n", (yyvsp[-4].sval));
            append_to(parse_output, msg);
            if((yyvsp[-4].sval)) free((yyvsp[-4].sval));
        }
#line 1605 "parser.tab.c"
    break;

  case 29: /* while_stmt: WHILE LPAREN condition RPAREN LBRACE stmt_list RBRACE  */
#line 456 "parser.y"
        {
            char node[512];
            snprintf(node, sizeof(node),
                "{\"name\":\"while\",\"children\":[{\"name\":\"cond: %s\"},{\"name\":\"body\"}]},",
                (yyvsp[-4].sval));
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "While loop parsed. Condition: %s\n", (yyvsp[-4].sval));
            append_to(parse_output, msg);
            if((yyvsp[-4].sval)) free((yyvsp[-4].sval));
        }
#line 1622 "parser.tab.c"
    break;

  case 30: /* return_stmt: RETURN expr SEMICOLON  */
#line 472 "parser.y"
        {
            /* Semantic Rule 5: void function returning value */
            if (strcmp(current_func_type, "void") == 0) {
                append_to(semantic_output,
                    "[SEMANTIC WARNING] void function should not return a value.\n");
                append_to(parse_output,
                    "[SEMANTIC WARNING] void function should not return a value.\n");
            }

            char node[256];
            snprintf(node, sizeof(node),
                "{\"name\":\"return\",\"children\":[{\"name\":\"%s\"}]},",
                (yyvsp[-1].sval));
            tree_append(node);
            char msg[256];
            snprintf(msg, sizeof(msg),
                "Return statement: return %s\n", (yyvsp[-1].sval));
            append_to(parse_output, msg);
            char tac[128];
            snprintf(tac, sizeof(tac), "return %s\n", (yyvsp[-1].sval));
            tac_emit_and_append(tac);
            if((yyvsp[-1].sval)) free((yyvsp[-1].sval));
        }
#line 1650 "parser.tab.c"
    break;

  case 31: /* return_stmt: RETURN SEMICOLON  */
#line 496 "parser.y"
        {
            /* Semantic Rule 5: int function with no return value */
            if (strcmp(current_func_type, "int") == 0) {
                append_to(semantic_output,
                    "[SEMANTIC WARNING] int function has empty return — no value returned.\n");
                append_to(parse_output,
                    "[SEMANTIC WARNING] int function has empty return — no value returned.\n");
            }

            tree_append("{\"name\":\"return\",\"children\":[]},");
            append_to(parse_output, "Return statement: return\n");
            tac_emit_and_append("return\n");
        }
#line 1668 "parser.tab.c"
    break;

  case 32: /* condition: expr LT expr  */
#line 513 "parser.y"
        {
            /* Semantic: check both sides declared */
            sem_check_declared((yyvsp[-2].sval));
            sem_check_declared((yyvsp[0].sval));

            char *l=new_label(), *l2=new_label();
            char tac[256];
            snprintf(tac, sizeof(tac),
                "if %s < %s goto %s\ngoto %s\n%s:\n", (yyvsp[-2].sval),(yyvsp[0].sval),l,l2,l);
            tac_emit_and_append(tac);
            (yyval.sval) = malloc(strlen((yyvsp[-2].sval))+strlen((yyvsp[0].sval))+6);
            snprintf((yyval.sval), strlen((yyvsp[-2].sval))+strlen((yyvsp[0].sval))+6, "%s < %s", (yyvsp[-2].sval), (yyvsp[0].sval));
            free((yyvsp[-2].sval)); free((yyvsp[0].sval));
        }
#line 1687 "parser.tab.c"
    break;

  case 33: /* condition: expr GT expr  */
#line 528 "parser.y"
        {
            sem_check_declared((yyvsp[-2].sval));
            sem_check_declared((yyvsp[0].sval));

            char *l=new_label(), *l2=new_label();
            char tac[256];
            snprintf(tac, sizeof(tac),
                "if %s > %s goto %s\ngoto %s\n%s:\n", (yyvsp[-2].sval),(yyvsp[0].sval),l,l2,l);
            tac_emit_and_append(tac);
            (yyval.sval) = malloc(strlen((yyvsp[-2].sval))+strlen((yyvsp[0].sval))+6);
            snprintf((yyval.sval), strlen((yyvsp[-2].sval))+strlen((yyvsp[0].sval))+6, "%s > %s", (yyvsp[-2].sval), (yyvsp[0].sval));
            free((yyvsp[-2].sval)); free((yyvsp[0].sval));
        }
#line 1705 "parser.tab.c"
    break;

  case 34: /* condition: expr EQ expr  */
#line 542 "parser.y"
        {
            char *l=new_label(), *l2=new_label();
            char tac[256];
            snprintf(tac, sizeof(tac),
                "if %s == %s goto %s\ngoto %s\n%s:\n", (yyvsp[-2].sval),(yyvsp[0].sval),l,l2,l);
            tac_emit_and_append(tac);
            (yyval.sval) = malloc(strlen((yyvsp[-2].sval))+strlen((yyvsp[0].sval))+7);
            snprintf((yyval.sval), strlen((yyvsp[-2].sval))+strlen((yyvsp[0].sval))+7, "%s == %s", (yyvsp[-2].sval), (yyvsp[0].sval));
            free((yyvsp[-2].sval)); free((yyvsp[0].sval));
        }
#line 1720 "parser.tab.c"
    break;

  case 35: /* condition: expr NEQ expr  */
#line 553 "parser.y"
        {
            char *l=new_label(), *l2=new_label();
            char tac[256];
            snprintf(tac, sizeof(tac),
                "if %s != %s goto %s\ngoto %s\n%s:\n", (yyvsp[-2].sval),(yyvsp[0].sval),l,l2,l);
            tac_emit_and_append(tac);
            (yyval.sval) = malloc(strlen((yyvsp[-2].sval))+strlen((yyvsp[0].sval))+7);
            snprintf((yyval.sval), strlen((yyvsp[-2].sval))+strlen((yyvsp[0].sval))+7, "%s != %s", (yyvsp[-2].sval), (yyvsp[0].sval));
            free((yyvsp[-2].sval)); free((yyvsp[0].sval));
        }
#line 1735 "parser.tab.c"
    break;

  case 36: /* condition: expr LEQ expr  */
#line 564 "parser.y"
        {
            char *l=new_label(), *l2=new_label();
            char tac[256];
            snprintf(tac, sizeof(tac),
                "if %s <= %s goto %s\ngoto %s\n%s:\n", (yyvsp[-2].sval),(yyvsp[0].sval),l,l2,l);
            tac_emit_and_append(tac);
            (yyval.sval) = malloc(strlen((yyvsp[-2].sval))+strlen((yyvsp[0].sval))+7);
            snprintf((yyval.sval), strlen((yyvsp[-2].sval))+strlen((yyvsp[0].sval))+7, "%s <= %s", (yyvsp[-2].sval), (yyvsp[0].sval));
            free((yyvsp[-2].sval)); free((yyvsp[0].sval));
        }
#line 1750 "parser.tab.c"
    break;

  case 37: /* condition: expr GEQ expr  */
#line 575 "parser.y"
        {
            char *l=new_label(), *l2=new_label();
            char tac[256];
            snprintf(tac, sizeof(tac),
                "if %s >= %s goto %s\ngoto %s\n%s:\n", (yyvsp[-2].sval),(yyvsp[0].sval),l,l2,l);
            tac_emit_and_append(tac);
            (yyval.sval) = malloc(strlen((yyvsp[-2].sval))+strlen((yyvsp[0].sval))+7);
            snprintf((yyval.sval), strlen((yyvsp[-2].sval))+strlen((yyvsp[0].sval))+7, "%s >= %s", (yyvsp[-2].sval), (yyvsp[0].sval));
            free((yyvsp[-2].sval)); free((yyvsp[0].sval));
        }
#line 1765 "parser.tab.c"
    break;

  case 38: /* expr: expr PLUS term  */
#line 589 "parser.y"
        {
            char *t = new_temp();
            char tac[128];
            snprintf(tac, sizeof(tac), "%s = %s + %s\n", t, (yyvsp[-2].sval), (yyvsp[0].sval));
            tac_emit_and_append(tac);
            (yyval.sval) = t;
            free((yyvsp[-2].sval)); free((yyvsp[0].sval));
        }
#line 1778 "parser.tab.c"
    break;

  case 39: /* expr: expr MINUS term  */
#line 598 "parser.y"
        {
            char *t = new_temp();
            char tac[128];
            snprintf(tac, sizeof(tac), "%s = %s - %s\n", t, (yyvsp[-2].sval), (yyvsp[0].sval));
            tac_emit_and_append(tac);
            (yyval.sval) = t;
            free((yyvsp[-2].sval)); free((yyvsp[0].sval));
        }
#line 1791 "parser.tab.c"
    break;

  case 40: /* expr: term  */
#line 607 "parser.y"
        {
            (yyval.sval) = (yyvsp[0].sval);
        }
#line 1799 "parser.tab.c"
    break;

  case 41: /* term: term MULTIPLY factor  */
#line 614 "parser.y"
        {
            char *t = new_temp();
            char tac[128];
            snprintf(tac, sizeof(tac), "%s = %s * %s\n", t, (yyvsp[-2].sval), (yyvsp[0].sval));
            tac_emit_and_append(tac);
            (yyval.sval) = t;
            free((yyvsp[-2].sval)); free((yyvsp[0].sval));
        }
#line 1812 "parser.tab.c"
    break;

  case 42: /* term: term DIVIDE factor  */
#line 623 "parser.y"
        {
            /* Semantic Rule 4: Division by zero check */
            sem_check_div_zero((yyvsp[0].sval));

            char *t = new_temp();
            char tac[128];
            snprintf(tac, sizeof(tac), "%s = %s / %s\n", t, (yyvsp[-2].sval), (yyvsp[0].sval));
            tac_emit_and_append(tac);
            (yyval.sval) = t;
            free((yyvsp[-2].sval)); free((yyvsp[0].sval));
        }
#line 1828 "parser.tab.c"
    break;

  case 43: /* term: factor  */
#line 635 "parser.y"
        {
            (yyval.sval) = (yyvsp[0].sval);
        }
#line 1836 "parser.tab.c"
    break;

  case 44: /* factor: NUMBER  */
#line 642 "parser.y"
        {
            char *s = malloc(20);
            snprintf(s, 20, "%d", (yyvsp[0].ival));
            (yyval.sval) = s;
        }
#line 1846 "parser.tab.c"
    break;

  case 45: /* factor: FLOAT_NUM  */
#line 648 "parser.y"
        {
            char *s = malloc(20);
            snprintf(s, 20, "%.2f", (yyvsp[0].fval));
            (yyval.sval) = s;
        }
#line 1856 "parser.tab.c"
    break;

  case 46: /* factor: IDENTIFIER  */
#line 654 "parser.y"
        {
            /* Semantic Rule 1: declared check */
            sem_check_declared((yyvsp[0].sval));

            /* Semantic Rule 2: initialized check */
            sem_check_initialized((yyvsp[0].sval));

            (yyval.sval) = (yyvsp[0].sval);
        }
#line 1870 "parser.tab.c"
    break;

  case 47: /* factor: LPAREN expr RPAREN  */
#line 664 "parser.y"
        {
            (yyval.sval) = (yyvsp[-1].sval);
        }
#line 1878 "parser.tab.c"
    break;

  case 48: /* factor: MINUS factor  */
#line 668 "parser.y"
        {
            char *t = new_temp();
            char tac[128];
            snprintf(tac, sizeof(tac), "%s = -%s\n", t, (yyvsp[0].sval));
            tac_emit_and_append(tac);
            (yyval.sval) = t;
            free((yyvsp[0].sval));
        }
#line 1891 "parser.tab.c"
    break;


#line 1895 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

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


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 678 "parser.y"


void reset_outputs() {
    token_output[0]     = '\0';
    parse_output[0]     = '\0';
    symtable_output[0]  = '\0';
    tac_output[0]       = '\0';
    tree_output[0]      = '\0';
    semantic_output[0]  = '\0';
    current_func_type[0]= '\0';
    temp_count          = 0;
    label_count         = 0;
}
