
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
     QSTRING = 258,
     T_STRING = 259,
     SITE_PATTERN = 260,
     NUMBER = 261,
     K_HISTORY = 262,
     K_NAMESCASESENSITIVE = 263,
     K_DESIGN = 264,
     K_VIAS = 265,
     K_TECH = 266,
     K_UNITS = 267,
     K_ARRAY = 268,
     K_FLOORPLAN = 269,
     K_SITE = 270,
     K_CANPLACE = 271,
     K_CANNOTOCCUPY = 272,
     K_DIEAREA = 273,
     K_PINS = 274,
     K_DEFAULTCAP = 275,
     K_MINPINS = 276,
     K_WIRECAP = 277,
     K_TRACKS = 278,
     K_GCELLGRID = 279,
     K_DO = 280,
     K_BY = 281,
     K_STEP = 282,
     K_LAYER = 283,
     K_ROW = 284,
     K_RECT = 285,
     K_COMPS = 286,
     K_COMP_GEN = 287,
     K_SOURCE = 288,
     K_WEIGHT = 289,
     K_EEQMASTER = 290,
     K_FIXED = 291,
     K_COVER = 292,
     K_UNPLACED = 293,
     K_PLACED = 294,
     K_FOREIGN = 295,
     K_REGION = 296,
     K_REGIONS = 297,
     K_NETS = 298,
     K_START_NET = 299,
     K_MUSTJOIN = 300,
     K_ORIGINAL = 301,
     K_USE = 302,
     K_STYLE = 303,
     K_PATTERN = 304,
     K_PATTERNNAME = 305,
     K_ESTCAP = 306,
     K_ROUTED = 307,
     K_NEW = 308,
     K_SNETS = 309,
     K_SHAPE = 310,
     K_WIDTH = 311,
     K_VOLTAGE = 312,
     K_SPACING = 313,
     K_NONDEFAULTRULE = 314,
     K_NONDEFAULTRULES = 315,
     K_N = 316,
     K_S = 317,
     K_E = 318,
     K_W = 319,
     K_FN = 320,
     K_FE = 321,
     K_FS = 322,
     K_FW = 323,
     K_GROUPS = 324,
     K_GROUP = 325,
     K_SOFT = 326,
     K_MAXX = 327,
     K_MAXY = 328,
     K_MAXHALFPERIMETER = 329,
     K_CONSTRAINTS = 330,
     K_NET = 331,
     K_PATH = 332,
     K_SUM = 333,
     K_DIFF = 334,
     K_SCANCHAINS = 335,
     K_START = 336,
     K_FLOATING = 337,
     K_ORDERED = 338,
     K_STOP = 339,
     K_IN = 340,
     K_OUT = 341,
     K_RISEMIN = 342,
     K_RISEMAX = 343,
     K_FALLMIN = 344,
     K_FALLMAX = 345,
     K_WIREDLOGIC = 346,
     K_MAXDIST = 347,
     K_ASSERTIONS = 348,
     K_DISTANCE = 349,
     K_MICRONS = 350,
     K_END = 351,
     K_IOTIMINGS = 352,
     K_RISE = 353,
     K_FALL = 354,
     K_VARIABLE = 355,
     K_SLEWRATE = 356,
     K_CAPACITANCE = 357,
     K_DRIVECELL = 358,
     K_FROMPIN = 359,
     K_TOPIN = 360,
     K_PARALLEL = 361,
     K_TIMINGDISABLES = 362,
     K_THRUPIN = 363,
     K_MACRO = 364,
     K_PARTITIONS = 365,
     K_TURNOFF = 366,
     K_FROMCLOCKPIN = 367,
     K_FROMCOMPPIN = 368,
     K_FROMIOPIN = 369,
     K_TOCLOCKPIN = 370,
     K_TOCOMPPIN = 371,
     K_TOIOPIN = 372,
     K_SETUPRISE = 373,
     K_SETUPFALL = 374,
     K_HOLDRISE = 375,
     K_HOLDFALL = 376,
     K_VPIN = 377,
     K_SUBNET = 378,
     K_XTALK = 379,
     K_PIN = 380,
     K_SYNTHESIZED = 381,
     K_DEFINE = 382,
     K_DEFINES = 383,
     K_DEFINEB = 384,
     K_IF = 385,
     K_THEN = 386,
     K_ELSE = 387,
     K_FALSE = 388,
     K_TRUE = 389,
     K_EQ = 390,
     K_NE = 391,
     K_LE = 392,
     K_LT = 393,
     K_GE = 394,
     K_GT = 395,
     K_OR = 396,
     K_AND = 397,
     K_NOT = 398,
     K_SPECIAL = 399,
     K_DIRECTION = 400,
     K_RANGE = 401,
     K_FPC = 402,
     K_HORIZONTAL = 403,
     K_VERTICAL = 404,
     K_ALIGN = 405,
     K_MIN = 406,
     K_MAX = 407,
     K_EQUAL = 408,
     K_BOTTOMLEFT = 409,
     K_TOPRIGHT = 410,
     K_ROWS = 411,
     K_TAPER = 412,
     K_TAPERRULE = 413,
     K_VERSION = 414,
     K_DIVIDERCHAR = 415,
     K_BUSBITCHARS = 416,
     K_PROPERTYDEFINITIONS = 417,
     K_STRING = 418,
     K_REAL = 419,
     K_INTEGER = 420,
     K_PROPERTY = 421,
     K_BEGINEXT = 422,
     K_ENDEXT = 423,
     K_NAMEMAPSTRING = 424,
     K_ON = 425,
     K_OFF = 426,
     K_X = 427,
     K_Y = 428,
     K_COMPONENT = 429,
     K_MASK = 430,
     K_MASKSHIFT = 431,
     K_COMPSMASKSHIFT = 432,
     K_SAMEMASK = 433,
     K_PINPROPERTIES = 434,
     K_TEST = 435,
     K_COMMONSCANPINS = 436,
     K_SNET = 437,
     K_COMPONENTPIN = 438,
     K_REENTRANTPATHS = 439,
     K_SHIELD = 440,
     K_SHIELDNET = 441,
     K_NOSHIELD = 442,
     K_VIRTUAL = 443,
     K_ANTENNAPINPARTIALMETALAREA = 444,
     K_ANTENNAPINPARTIALMETALSIDEAREA = 445,
     K_ANTENNAPINGATEAREA = 446,
     K_ANTENNAPINDIFFAREA = 447,
     K_ANTENNAPINMAXAREACAR = 448,
     K_ANTENNAPINMAXSIDEAREACAR = 449,
     K_ANTENNAPINPARTIALCUTAREA = 450,
     K_ANTENNAPINMAXCUTCAR = 451,
     K_SIGNAL = 452,
     K_POWER = 453,
     K_GROUND = 454,
     K_CLOCK = 455,
     K_TIEOFF = 456,
     K_ANALOG = 457,
     K_SCAN = 458,
     K_RESET = 459,
     K_RING = 460,
     K_STRIPE = 461,
     K_FOLLOWPIN = 462,
     K_IOWIRE = 463,
     K_COREWIRE = 464,
     K_BLOCKWIRE = 465,
     K_FILLWIRE = 466,
     K_BLOCKAGEWIRE = 467,
     K_PADRING = 468,
     K_BLOCKRING = 469,
     K_BLOCKAGES = 470,
     K_PLACEMENT = 471,
     K_SLOTS = 472,
     K_FILLS = 473,
     K_PUSHDOWN = 474,
     K_NETLIST = 475,
     K_DIST = 476,
     K_USER = 477,
     K_TIMING = 478,
     K_BALANCED = 479,
     K_STEINER = 480,
     K_TRUNK = 481,
     K_FIXEDBUMP = 482,
     K_FENCE = 483,
     K_FREQUENCY = 484,
     K_GUIDE = 485,
     K_MAXBITS = 486,
     K_PARTITION = 487,
     K_TYPE = 488,
     K_ANTENNAMODEL = 489,
     K_DRCFILL = 490,
     K_OXIDE1 = 491,
     K_OXIDE2 = 492,
     K_OXIDE3 = 493,
     K_OXIDE4 = 494,
     K_CUTSIZE = 495,
     K_CUTSPACING = 496,
     K_DESIGNRULEWIDTH = 497,
     K_DIAGWIDTH = 498,
     K_ENCLOSURE = 499,
     K_HALO = 500,
     K_GROUNDSENSITIVITY = 501,
     K_HARDSPACING = 502,
     K_LAYERS = 503,
     K_MINCUTS = 504,
     K_NETEXPR = 505,
     K_OFFSET = 506,
     K_ORIGIN = 507,
     K_ROWCOL = 508,
     K_STYLES = 509,
     K_POLYGON = 510,
     K_PORT = 511,
     K_SUPPLYSENSITIVITY = 512,
     K_VIA = 513,
     K_VIARULE = 514,
     K_WIREEXT = 515,
     K_EXCEPTPGNET = 516,
     K_FILLWIREOPC = 517,
     K_OPC = 518,
     K_PARTIAL = 519,
     K_ROUTEHALO = 520
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 227 "def.y"

        double dval ;
        int    integer ;
        char * string ;
        int    keyword ;  // really just a nop 
        struct LefDefParser::defpoint pt;
        LefDefParser::defTOKEN *tk;



/* Line 1676 of yacc.c  */
#line 328 "def.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE defyylval;

