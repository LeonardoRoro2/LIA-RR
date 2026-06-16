//
// Creado por:
// Angel Giovanni Romero Salazar
// Leonardo Antonio Romero Rodriguez
//

#define STRSAFE_NO_DEPRECATE

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "portable-file-dialogs.h"
#include <fstream>
#include <sstream>
#include <string.h>

// Buffers para mostrar el texto en el GUI
char bufCodigo[ 50000 ] = "";
char bufTokens[ 50000 ] = "";
char bufSintaxis[ 50000 ] = "";
char bufErrores[ 50000 ] = "";

int transiciones[ 20 ][ 35 ] =
    {
    //  l    L    _    d    .    E    e    +    -    /    *    %    '    "    $     &   |     =    !    <    >   {    }    [    ]    (    )    ,    :    ;    \t    \b  \n   dif  EOF
    //  0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30    31  32   33   34
    {1  , 2  , 2  , 3  , 132, 2  , 1  , 105, 106, 108, 107, 128, 15 , 17 , 19 , 13 , 14 , 9  , 12 , 10 , 11 , 129, 130, 121, 122, 119, 120, 124, 131, 123, 507, 507, 507, 500, 509}, // Estado 0
    {1  , 2  , 2  , 2  , 100, 2  , 1  , 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100}, // Estado 1
    {2  , 2  , 2  , 2  , 101, 2  , 1  , 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101}, // Estado 2
    {102, 102, 102, 3  , 4  , 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102}, // Estado 3
    {501, 501, 501, 5  , 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501}, // Estado 4
    {103, 103, 103, 5  , 103, 6  , 6  , 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103}, // Estado 5
    {502, 502, 502, 8  , 502, 502, 502, 7  , 7  , 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502}, // Estado 6
    {503, 503, 503, 8  , 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503}, // Estado 7
    {104, 104, 104, 8  , 104, 104, 104 ,104 ,104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104}, // Estado 8
    {109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 110, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109}, // Estado 9
    {111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 112, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111}, // Estado 10
    {113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 114, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113}, // Estado 11
    {116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 115, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116}, // Estado 12
    {504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 117, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504}, // Estado 13
    {505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 118, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505}, // Estado 14
    {16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 506, 16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 508, 16 , 508}, // Estado 15
    {508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 125, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508, 508}, // Estado 16
    {17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 18 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 17 , 508, 17 , 508}, // Estado 17
    {126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 17 , 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126}, // Estado 18
    {19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 19 , 127, 19 , 127}, // Estado 19
    };

int predicciones[ 29 ][ 45 ] =
    {
    //  class   id   def  int  float  char  bool  string  if   else  do   while  dowhile  input  output  break  loop  cteentera  ctereal  ctenotacion  +    -    *    /    =    ==   <    <=   >    >=   !=   !    &&   ||   (    )    ;    ,    :    {    }    ctecaracter  ctestring  %    $
    //  200     201  202  203  204    205   206   207     208  209   210  211    212      213    214     215    216   217        218      219          220  221  222  223  224  225  226  227  228  229  230  231  232  233  234  235  236  237  238  239  240  241          242        243  244
    {0     , 550, 550, 550, 550  , 550 , 550 , 550   , 550, 550 , 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 550, 590}, // PROGRAM 0
    {551   , 2  , 1  , 551, 551  , 551 , 551 , 551   , 2  , 2   , 2  , 2  , 2  , 2  , 2  , 2  , 2  , 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 551, 2  , 551, 551, 551, 551}, // DECLARAR 1
    {552   , 552, 552, 552, 552  , 552 , 552 , 552   , 552, 552 , 552, 552, 552, 552, 552, 552, 552, 552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,5,552, 3  , 4  ,552,552,552,552,552,552}, // IDN 2
    {553   , 553, 553, 5  , 6    , 7   , 8   , 9     , 553, 553 , 553, 553, 553, 553, 553, 553, 553, 553,553,553,553,553,553,553,553,553,553,553,553,553,553,553,553,553,553,553,553,553,553,553,553,553,553,553,553}, // TIPO 3
    {554   , 10 , 554, 554, 554  , 554 , 554 , 554   , 11 , 18  , 13 , 12 , 18 , 14 , 15 , 17 , 18 ,554,554,554,554,554,554,554,554,554,554,554,554,554,554,554,554,554,554,554,554,554,554,554, 18 ,554,554,554,554}, // ESTATUTOS 4
    {555   , 19 , 555, 555, 555  , 555 , 555 , 555   , 555, 555 , 555, 555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555}, // EST_ASIG 5
    {556   , 556, 556, 556, 556  , 556 , 556 , 556   , 556, 556 , 556, 556,556, 20 ,556,556,556,556,556,556,556,556,556,556,556,556,556,556,556,556,556,556,556,556,556,556,556,556,556,556,556,556,556,556,556}, // EST_WRITE 6
    {557   , 557, 557, 557, 557  , 557 , 557 , 557   , 557, 557 , 557, 557,557,557, 21 ,557,557,557,557,557,557,557,557,557,557,557,557,557,557,557,557,557,557,557,557,557,557,557,557,557,557,557,557,557,557}, // EST_READ 7
    {558   , 558, 558, 558, 558  , 558 , 558 , 558   , 558, 558 , 558, 558,558,558,558, 22 ,558,558,558,558,558,558,558,558,558,558,558,558,558,558,558,558,558,558,558,558,558,558,558,558,558,558,558,558,558}, // EST_BREAK 8
    {559   , 559, 559, 559, 559  , 559 , 559 , 559   , 559, 559 , 23 , 559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559,559}, // EST_DO 9
    {560   , 560, 560, 560, 560  , 560 , 560 , 560   , 24 , 560 , 560, 560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560}, // EST_IF 10
    {561   , 561, 561, 561, 561  , 561 , 561 , 561   , 561, 561 , 561, 27 ,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561,561}, // EST_WHILE 11
    {562   , 562, 562, 562, 562  , 562 , 562 , 562   , 562, 562 , 562, 562,562,562,562,562, 28 ,562,562,562,562,562,562,562,562,562,562,562,562,562,562,562,562,562,562,562,562,562,562,562,562,562,562,562,562}, // EST_LOOP 12
    {563   , 563, 563, 563, 563  , 563 , 563 , 563   , 563, 25  , 563, 563,563,563,563,563,563,563,563,563,563,563,563,563,563,563,563,563,563,563,563,563,563,563,563,563,563,563,563,563, 26 ,563,563,563,563}, // ELSES 13
    {564   , 564, 564, 564, 564  , 564 , 564 , 564   , 564, 564 , 564, 564,564,564,564,564,564,564,564,564,564,564,564,564,564,564,564,564,564,564,564,564,564,564,564, 30 ,564, 29 ,564,564,564,564,564,564,564}, // EXPRN 14
    {565   , 31 , 565, 565, 565  , 565 , 565 , 565   , 565, 565 , 565, 565,565,565,565,565,565, 31 , 31 , 31 ,565,565,565,565,565,565,565,565,565,565,565, 31 ,565,565, 31 ,565,565,565,565,565,565, 31 , 31 ,565,565}, // EXPR 15
    {566   , 566, 566, 566, 566  , 566 , 566 , 566   , 566, 566 , 566, 566,566,566,566,566,566,566,566,566,566,566,566,566,566,566,566,566,566,566,566,566,566, 32 ,566, 33 , 33 , 33 ,566,566,566,566,566,566,566}, // EXPROR 16
    {567   , 34 , 567, 567, 567  , 567 , 567 , 567   , 567, 567 , 567, 567,567,567,567,567,567, 34 , 34 , 34 ,567,567,567,567,567,567,567,567,567,567,567, 34 ,567,567, 34 ,567,567,567,567,567,567, 34 , 34 ,567,567}, // EXPR2 17
    {568   , 568, 568, 568, 568  , 568 , 568 , 568   , 568, 568 , 568, 568,568,568,568,568,568,568,568,568,568,568,568,568,568,568,568,568,568,568,568,568, 35 , 36 ,568, 36 , 36 , 36 ,568,568,568,568,568,568,568}, // EXPRAND 18
    {569   , 37 , 569, 569, 569  , 569 , 569 , 569   , 569, 569 , 569, 569,569,569,569,569,569, 37 , 37 , 37 ,569,569,569,569,569,569,569,569,569,569,569, 37 ,569,569, 37 ,569,569,569,569,569,569, 37 , 37 ,569,569}, // EXPR3 19
    {570   , 39 , 570, 570, 570  , 570 , 570 , 570   , 570, 570 , 570, 570,570,570,570,570,570, 39 , 39 ,39 ,570,570,570,570,570,570,570,570,570,570,570, 38 ,570,570, 39 ,570,570,570,570,570,570, 39 , 39 ,570,570}, // NOT 20
    {571   , 40 , 571, 571, 571  , 571 , 571 , 571   , 571, 571 , 571, 571,571,571,571,571,571, 40 , 40 , 40 ,571,571,571,571,571,571,571,571,571,571,571,571,571,571, 40 ,571,571,571,571,571,571, 40 , 40 ,571,571}, // EXPR4 21
    {572   , 572, 572, 572, 572  , 572 , 572 , 572   , 572, 572 , 572, 572,572,572,572,572,572,572,572,572,572,572,572,572,572, 41 , 41 , 41 , 41 , 41 , 41 ,572, 42 , 42 ,572, 42 , 42 , 42 ,572,572,572,572,572,572,572}, // EXPRREL 22
    {573   , 573, 573, 573, 573  , 573 , 573 , 573   , 573, 573 , 573, 573,573,573,573,573,573,573,573,573,573,573,573,573,573, 43 , 45 , 46 , 47 , 48 , 44 ,573,573,573,573,573,573,573,573,573,573,573,573,573,573}, // OPREL 23
    {574   , 49 , 574, 574, 574  , 574 , 574 , 574   , 574, 574 , 574, 574,574,574,574,574,574, 49 , 49 , 49 ,574,574,574,574,574,574,574,574,574,574,574,574,574,574, 49 ,574,574,574,574,574,574, 49 , 49 ,574,574}, // EXPR5 24
    {575   , 575, 575, 575, 575  , 575 , 575 , 575   , 575, 575 , 575, 575,575,575,575,575,575,575,575,575, 50 , 51 ,575,575,575, 52 , 52 , 52 , 52 , 52 , 52 ,575, 52 , 52 ,575, 52 , 52 , 52 ,575,575,575,575,575,575,575}, // ADI 25
    {576   , 53 , 576, 576, 576  , 576 , 576 , 576   , 576, 576 , 576, 576,576,576,576,576,576, 53 , 53 , 53 ,576,576,576,576,576,576,576,576,576,576,576,576,576,576, 53 ,576,576,576,576,576,576, 53 , 53 ,576,576}, // TERM 26
    {577   , 577, 577, 577, 577  , 577 , 577 , 577   , 577, 577 , 577, 577,577,577,577,577,577,577,577,577, 57 , 57 , 54 , 55 , 577, 57 , 57 , 57 , 57 , 57 , 57 , 577, 57 , 57 , 577, 57 , 57 , 57 , 577, 577, 577, 577, 577, 56 ,577}, // PROD 27
    {578   , 58 , 578, 578, 578  , 578 , 578 , 578   , 578, 578 , 578, 578, 578, 578, 578, 578, 578, 59 , 60 , 61 , 578, 578, 578, 578, 578, 578, 578, 578, 578, 578, 578, 578, 578, 578, 64 , 578, 578, 578, 578, 578, 578, 62 , 63 , 578, 578}, // FACT 28
    };

int producciones[ 65 ][ 9 ] =
    {
    //Cantidad de Tokens, Token 1, Token 2, Token 3, Token 4, Token 5, Token 6, Token 7, Token 8
    {8, 200, 234, 201, 235, 239, 1  , 4  , 240}, // 0
    {7, 202, 201, 2  , 238, 3  , 236, 1  },      // 1
    {0, -1 },                                    // 2
    {3, 237, 201, 2  },                          // 3
    {0, -1 },                                    // 4
    {1, 203},                                    // 5
    {1, 204},                                    // 6
    {1, 205},                                    // 7
    {1, 206},                                    // 8
    {1, 207},                                    // 9
    {2, 5  , 4  },                               // 10
    {2, 10 , 4  },                               // 11
    {2, 11 , 4  },                               // 12
    {2, 9  , 4  },                               // 13
    {2, 7  , 4  },                               // 14
    {2, 6  , 4  },                               // 15
    {2, 12 , 4  },                               // 16
    {2, 8  , 4  },                               // 17
    {0, -1 },                                    // 18
    {4, 201, 224, 15 , 236},                     // 19
    {6, 213, 234, 15 , 14 , 235, 236},           // 20
    {5, 214, 234, 2  , 235, 236},                // 21
    {2, 215, 236},                               // 22
    {7, 210, 4  , 212, 234, 15 , 235, 236},      // 23
    {8, 208, 234, 15 , 235, 239, 4  , 13 , 240}, // 24
    {2, 209, 4  },                               // 25
    {0, -1 },                                    // 26
    {7, 211, 234, 15 , 235, 239, 4  , 240},      // 27
    {4, 216, 239, 4  , 240},                     // 28
    {3, 237, 15 , 14 },                          // 29
    {0, -1 },                                    // 30
    {2, 17 , 16 },                               // 31
    {2, 233, 15 },                               // 32
    {0, -1 },                                    // 33
    {2, 19 , 18 },                               // 34
    {2, 232, 17 },                               // 35
    {0, -1 },                                    // 36
    {2, 20 , 21 },                               // 37
    {1, 231},                                    // 38
    {0, -1 },                                    // 39
    {2, 24 , 22 },                               // 40
    {2, 23 , 24 },                               // 41
    {0, -1 },                                    // 42
    {1, 225},                                    // 43
    {1, 230},                                    // 44
    {1, 226},                                    // 45
    {1, 227},                                    // 46
    {1, 228},                                    // 47
    {1, 229},                                    // 48
    {2, 26 , 25 },                               // 49
    {2, 220, 24 },                               // 50
    {2, 221, 24 },                               // 51
    {0, -1 },                                    // 52
    {2, 28 , 27 },                               // 53
    {2, 222, 26 },                               // 54
    {2, 223, 26 },                               // 55
    {2, 243, 26 },                               // 56
    {0, -1 },                                    // 57
    {1, 201},                                    // 58
    {1, 217},                                    // 59
    {1, 218},                                    // 60
    {1, 219},                                    // 61
    {1, 241},                                    // 62
    {1, 242},                                    // 63
    {3, 234, 15 , 235},                          // 64
    };

const char *reservadas[] = {
    "class","endclass","int","float","char","string","bool","if","else","do","while","input","output","def","to","break","loop","dowhile"
};

typedef struct token {
    char lexema[ 50 ];
    char gramema[ 50 ];
    int estado;
} token;

typedef struct nodo {
    token tk;
    nodo* siguiente;
} nodo;

nodo* primero = nullptr;

void crearNodo( const token &info_tk ) {
    const auto nuevoNodo = static_cast< nodo * >( malloc( sizeof( nodo ) ) );
    nuevoNodo -> tk = info_tk;
    nuevoNodo -> siguiente = nullptr;
    if ( primero == nullptr ) {
        primero = nuevoNodo;
    } else {
        nodo* actual = primero;
        while ( actual->siguiente != nullptr ) {
            actual = actual -> siguiente;
        }
        actual -> siguiente = nuevoNodo;
    }
}

int relaciona( const char car, const int estado ) {
    if ( car >= 'a' && car <= 'z' ) {
        if ( estado == 5 && car == 'e' ) return 6;
        return 0;
    }
    if ( car >= 'A' && car <= 'Z' ) {
        if ( estado == 5 && car == 'E' ) return 5;
        return 1;
    }
    if ( car >= '0' && car <= '9' ) return 3;

    switch ( car ) {
        case '_': return 2;
        case '.': return 4;
        case '+': return 7;
        case '-': return 8;
        case '/': return 9;
        case '*': return 10;
        case '%': return 11;
        case '\'': return 12;
        case '"': return 13;
        case '$': return 14;
        case '&': return 15;
        case '|': return 16;
        case '=': return 17;
        case '!': return 18;
        case '<': return 19;
        case '>': return 20;
        case '{': return 21;
        case '}': return 22;
        case '[': return 23;
        case ']': return 24;
        case '(': return 25;
        case ')': return 26;
        case ',': return 27;
        case ':': return 28;
        case ';': return 29;
        case '\t': return 30;
        case ' ': return 31;
        case '\n': return 32;
        case '\r': return 32;
        case EOF: return 34;
        default: return 33;
    }
}

void tokenizador( char cad[], const int estado ) {
    token tokenNuevo;
    strcpy( tokenNuevo.lexema, cad );
    switch ( estado ) {
        case 100:
            for ( auto & reservada : reservadas ) {
                if ( strcmp( cad, reservada ) == 0 ) {
                    strcpy( tokenNuevo.gramema, "palabra reservada" );
                    break;
                }
                strcpy(tokenNuevo.gramema, "identificador");
            }
            break;
        case 101:
            strcpy( tokenNuevo.gramema, "identificador" ); break;
        case 102:
            strcpy( tokenNuevo.gramema, "constante entera" ); break;
        case 103:
            strcpy( tokenNuevo.gramema, "constante flotante" ); break;
        case 104:
            strcpy( tokenNuevo.gramema, "constante en notacion cientifica" ); break;
        case 105:
            strcpy( tokenNuevo.gramema, "suma" ); break;
        case 106:
            strcpy( tokenNuevo.gramema, "resta" ); break;
        case 107:
            strcpy( tokenNuevo.gramema, "multiplicacion" ); break;
        case 108:
            strcpy( tokenNuevo.gramema, "division" ); break;
        case 109:
            strcpy( tokenNuevo.gramema, "asignacion" ); break;
        case 110:
            strcpy( tokenNuevo.gramema, "igual" ); break;
        case 111:
            strcpy( tokenNuevo.gramema, "menor" ); break;
        case 112:
            strcpy( tokenNuevo.gramema, "menor igual" ); break;
        case 113:
            strcpy( tokenNuevo.gramema, "mayor" ); break;
        case 114:
            strcpy( tokenNuevo.gramema, "mayor igual" ); break;
        case 115:
            strcpy( tokenNuevo.gramema, "diferente" ); break;
        case 116:
            strcpy( tokenNuevo.gramema, "not" ); break;
        case 117:
            strcpy( tokenNuevo.gramema, "and" ); break;
        case 118:
            strcpy( tokenNuevo.gramema, "or" ); break;
        case 119:
            strcpy( tokenNuevo.gramema, "parentesis abre" ); break;
        case 120:
            strcpy( tokenNuevo.gramema, "parentesis cierra" ); break;
        case 121:
            strcpy( tokenNuevo.gramema, "corchete abre" ); break;
        case 122:
            strcpy( tokenNuevo.gramema, "corchete cierra" ); break;
        case 123:
            strcpy( tokenNuevo.gramema, "punto y coma" ); break;
        case 124:
            strcpy( tokenNuevo.gramema, "coma" ); break;
        case 125:
            strcpy( tokenNuevo.gramema, "caracter" ); break;
        case 126:
            strcpy( tokenNuevo.gramema, "string" ); break;
        case 127:
            strcpy( tokenNuevo.gramema, "comentario de linea" ); break;
        case 128:
            strcpy( tokenNuevo.gramema, "modulo" ); break;
        case 129:
            strcpy( tokenNuevo.gramema, "llave abre" ); break;
        case 130:
            strcpy( tokenNuevo.gramema, "llave cierra" ); break;
        case 131:
            strcpy( tokenNuevo.gramema, "dos puntos" ); break;
        case 132:
            strcpy( tokenNuevo.gramema, "punto" ); break;
        default:  strcpy( tokenNuevo.gramema, "error" ); break;
    }
    tokenNuevo.estado = estado;
    crearNodo( tokenNuevo );
}

void detectarError( char cad[], const int estado ) {
    token tokenNuevo;
    strcpy( tokenNuevo.lexema, cad );
    switch ( estado ) {
        case 500:
            strcpy( tokenNuevo.gramema, "caracter no reconocido" ); break;
        case 501:
            strcpy( tokenNuevo.gramema, "flotante sin digitos" ); break;
        case 502:
            strcpy( tokenNuevo.gramema, "not cientifica esperando digito o signo" ); break;
        case 503:
            strcpy( tokenNuevo.gramema, "not cientifica esperando digito" ); break;
        case 504:
            strcpy( tokenNuevo.gramema, "esperando &" ); break;
        case 505:
            strcpy( tokenNuevo.gramema, "esperando | caracter vacio" ); break;
        case 506:
            strcpy( tokenNuevo.gramema, "Constante Caracter Vacia" ); break;
        case 507:
            strcpy( tokenNuevo.gramema, "delimitador" ); break;
        case 508:
            strcpy( tokenNuevo.gramema, "Comillas simples o dobles sin cerrar" ); break;
        case 509:
            strcpy( tokenNuevo.gramema, "EoF" ); break;
        default:
            strcpy( tokenNuevo.gramema, "error" ); break;
    }
    tokenNuevo.estado = estado;
    crearNodo( tokenNuevo );
}

int pos_tk = 0;

void liberarLista() {
    nodo* actual = primero;
    nodo* aux = nullptr;
    while ( actual != nullptr ) {
        aux = actual;
        actual = actual -> siguiente;
        free( aux );
    }
    primero = nullptr;
}

void analiza( const char* bufferIO ) {
    liberarLista();
    pos_tk = 0;

    char cadena[ 100 ];
    int estado = 0;
    int cont_cadena = 0;
    int i_buf = 0;

    while ( true ) {
        int car = bufferIO[ i_buf ] == '\0' ? EOF : bufferIO[ i_buf ];
        i_buf++;

        cadena[ cont_cadena ] = car;
        int columna = relaciona( car, estado );
        estado = transiciones[ estado ][ columna ];
        cont_cadena++;

        if ( estado > 19 ) {
            if ( ( estado >= 100 && estado <= 104 ) || estado == 109 || estado == 111 || estado == 113 || estado == 116 ) {
                cadena[ cont_cadena - 1 ] = '\0';
                i_buf--;
            } else {
                cadena[ cont_cadena ] = '\0';
            }

            pos_tk++;
            if ( estado >= 100 && estado <= 130 ) {
                tokenizador( cadena, estado );
            }else {
                detectarError( cadena, estado );
            }

            estado = 0;

            if ( car == EOF ) {
                break;
            }
            cont_cadena = 0;
        }
    }
}

void imprimir() {
    nodo* actual = primero;

    // Limpiamos los buffers antes de escribir
    memset( bufTokens, 0, sizeof( bufTokens ) );
    memset( bufErrores, 0, sizeof( bufErrores ) );
    memset( bufSintaxis, 0, sizeof( bufSintaxis ) );

    while ( actual != nullptr ) {
        char linea[ 150 ];
        sprintf( linea, "%s \t -> %s\n", actual -> tk.lexema, actual -> tk.gramema );

        if ( actual -> tk.estado >= 500 && actual -> tk.estado != 507 && actual -> tk.estado != 509 ) {
            strcat( bufErrores, linea );
        }
        else if ( actual -> tk.estado != 507 && actual -> tk.estado != 509 ) {
            strcat( bufTokens, linea );
        }

        actual = actual -> siguiente;
    }
}

void Abrir() {
    // Abre la ventana y espera al archivo elegido
    auto selecciones = pfd::open_file( "Abrir archivo de codigo", ".",
                                      { "Archivos LIA", "*.lia", "Todos los archivos", "*" } ).result();

    // Verifica si se eligió algo
    if ( !selecciones.empty() ) {
        const std::string& ruta = selecciones[ 0 ];

        // Leer el archivo
        if ( std::ifstream archivo( ruta ); archivo.is_open() ) {
            std::stringstream bufferStr;
            bufferStr << archivo.rdbuf();

            // Copiar el texto al buffer dibujado
            strncpy( bufCodigo, bufferStr.str().c_str(), sizeof( bufCodigo ) - 1 );
            bufCodigo[ sizeof( bufCodigo ) - 1 ] = '\0'; // Medida de seguridad
        } else {
            pfd::message( "Error", "No se pudo leer el archivo.", pfd::choice::ok, pfd::icon::error );
        }
    }
}

void Guardar() {
    // Abre la ventana de Guardar Como
    auto destino = pfd::save_file( "Guardar codigo", "ejemplo.lia",
                                  { "Archivos LIA", "*.lia", "Todos los archivos", "*" } ).result();

    if ( !destino.empty() ) {
        // Escribe lo que hay en el buffer hacia el archivo
        if (std::ofstream archivo(destino); archivo.is_open()) {
            archivo << bufCodigo;
            // Muestra una alerta de éxito
            pfd::message( "Alto: Demasiado Exitoso", "Archivo guardado correctamente.", pfd::choice::ok, pfd::icon::info );
        }
    }
}

int main() {
    // Inicializa GLFW y crea la ventana del sistema
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Analizador Lexico para LIA", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Inicializa el contexto de ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Bucle principal
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Inicia el "Frame" de ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Dibujo interfaz
        ImGui::Begin("Analizador Lexico de LIA");

        // txtCodigo
        ImGui::Text("Codigo a evaluar");
        ImGui::InputTextMultiline("##codigo", bufCodigo, sizeof(bufCodigo), ImVec2(-FLT_MIN, 250));

        // Botones
        if (ImGui::Button("Abrir Archivo", ImVec2(120, 30))) {
            Abrir();
        }
        ImGui::SameLine();
        if (ImGui::Button("Guardar Archivo", ImVec2(120, 30))) {
            Guardar();
        }
        ImGui::SameLine();
        if (ImGui::Button("Limpiar", ImVec2(120, 30))) {
            memset(bufCodigo, 0, sizeof(bufCodigo));
            memset(bufTokens, 0, sizeof(bufTokens));
            memset(bufErrores, 0, sizeof(bufErrores));
            liberarLista();
        }
        ImGui::SameLine();
        if (ImGui::Button("Analizar", ImVec2(120, 30))) {
            if (strlen(bufCodigo) > 0) {
                analiza(bufCodigo);
                imprimir();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Salir", ImVec2(120, 30))) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        ImGui::Spacing();

        // txtResultados (solo lectura)
        ImGui::Text("Tokens generados:");
        ImGui::InputTextMultiline("##tokens", bufTokens, sizeof(bufTokens), ImVec2(-FLT_MIN, 150), ImGuiInputTextFlags_ReadOnly);

        ImGui::Text("Sintaxis:");
        ImGui::InputTextMultiline("##sintaxis", bufSintaxis, sizeof(bufSintaxis), ImVec2(-FLT_MIN, 150), ImGuiInputTextFlags_ReadOnly);

        ImGui::Text("Errores detectados:");
        ImGui::InputTextMultiline("##errores", bufErrores, sizeof(bufErrores), ImVec2(-FLT_MIN, 100), ImGuiInputTextFlags_ReadOnly);

        ImGui::End();

        // Renderizado
        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Limpieza
    liberarLista();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
