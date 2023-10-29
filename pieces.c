char pieceL[8][4] = {
    {0,1,2,2},
    {2,2,2,1},      //       x x x x
    {1,1,1,2},      //       0 1 2 3
    {0,1,2,2},      //    y0        
    {1,2,3,1},      //    y1 X X X 
    {1,1,1,2},      //    y2     X
    {1,2,2,2},      //    y3     
    {1,1,2,3}        
};

char piece7[8][4] = {
    {1,1,2,3},
    {1,2,2,2},
    {1,2,1,1},
    {1,1,2,3},
    {0,1,2,2},
    {1,1,1,2},
    {2,2,2,1},
    {0,1,2,2}
};
          
char pieceS[8][4] = {         
    {1,2,0,1},
    {0,0,1,1},
    {1,1,2,2},
    {0,1,1,2},
    {1,2,0,1},
    {0,0,1,1},
    {1,1,2,2},
    {0,1,1,2}
};
 
char pieceZ[8][4] = { 
    {0,1,1,2},
    {0,0,1,1},
    {1,1,0,0},
    {0,1,1,2},
    {0,1,1,2},
    {0,0,1,1},
    {1,1,0,0},
    {0,1,1,2}
};
 
char pieceT[8][4] = { 
    {0,1,2,1},
    {1,1,1,0},
    {1,1,2,1},
    {0,1,1,2},
    {0,1,2,1},    
    {1,1,1,2},    
    {0,1,1,1},    
    {1,1,0,2}    
};

char pieceSquare[8][4] = {                     
    {2,2,1,1},
    {0,1,0,1},
    {2,2,1,1},
    {0,1,0,1},
    {2,2,1,1},
    {0,1,0,1},
    {2,2,1,1},
    {0,1,0,1}
};
 
char pieceLine[8][4] = { 
    {0,1,2,3},
    {1,1,1,1},
    {2,2,2,2},
    {0,1,2,3},
    {0,1,2,3},
    {1,1,1,1},
    {2,2,2,2},
    {0,1,2,3}
};

    // I might have made this function overcomplicated
    // reason: I didn't realize arrays are passed as pointers by default 
    // and so I did it all explicitly. oh well, it works so I'm not messing with it
char* getPiece(int index){
    switch(index){
        case 0:
            return &pieceL[0][0];
        case 1:
            return &piece7[0][0];
        case 2:
            return &pieceS[0][0];
        case 3:
            return &pieceZ[0][0];
        case 4:
            return &pieceT[0][0];
        case 5:
            return &pieceSquare[0][0];
        default:
            return &pieceLine[0][0];
    }
}