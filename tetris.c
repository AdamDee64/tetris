#include <stdio.h>
#include <time.h>
#include "raylib.h"
#include "pieces.h"

int main(void)
{
    // Initialization
    const int screenWidth = 500;
    const int screenHeight = 600;
    const int FPS = 60;
    const double dt = 1.0/FPS;
    
    typedef enum {
        GAME_BEGIN = 0,
        GAME_PLAY = 1,
        GAME_OVER = 2
    } GameState;
    
    enum collisionFlags {
        PASS = 0,
        AOB_LEFT = 1,
        AOB_RIGHT = 2,
        AOB_DOWN = 3,
        BLOCKED = 4
    };
    
    const int gridWidth = 10;
    const int gridHeight = 20;
    const int xStart = 3;
    const int yStart = 0;

    const int cellSize = 25;
    const int squareSize = cellSize - 2;
    const int borderThiccness = 3;
    
    const int xGridOffset = -50;
    const int yGridOffset = 0;
    const int borderX = (screenWidth / 2) - (cellSize * gridWidth / 2) - borderThiccness + xGridOffset;
    const int borderY = (screenHeight / 2) - (cellSize * gridHeight / 2) - borderThiccness + yGridOffset;
    const int borderW = gridWidth * cellSize + borderThiccness * 2;
    const int borderH = gridHeight * cellSize + borderThiccness * 2;
    const int gridX = (screenWidth / 2) - (cellSize * gridWidth / 2) + xGridOffset;
    const int gridY = (screenHeight / 2) - (cellSize * gridHeight / 2) + yGridOffset;
   
    // Game State
    GameState gameState = GAME_PLAY; // change to begin later
   
    // Game speed variables
    const int resetSpeed = 53;
    int gameSpeed = resetSpeed;
    int timer = gameSpeed;
    int dropSpeed = 5;
    double waitTime = 0.0;
    
    // Scoring
    int score = 0;
    char scoreText[16];
    
    int lines = 0;
    char linesText[16];
    
    int level = 0;
    char levelText[16];


    //int temp = 6;
    // next piece variables
    int nextIndex = GetRandomValue(0,6);
    char* nextPiece;
    char nextX[4];
    char nextY[4];
    char nextOffsetX = 0;
    char nextOffsetY = 0;

    // Active Piece Variables
    int activeIndex;
    char* activePiece;
    char activeX[4];
    char activeY[4];
    char xOffset;
    char yOffset;
    int rotationIndex;
    bool pieceActive = false;
    
    char grid[gridWidth][gridHeight];
    
    // Color index. index 0 should never be used,
    Color colors[] = {BLACK, PINK, GREEN, PURPLE, BEIGE, BROWN, LIME, SKYBLUE};
    
    void GameOver(){
        gameState = GAME_OVER;
        pieceActive = false;
    }
    
    int CheckCollision(char* nextMoveX, char* nextMoveY){

        for (int i = 0; i < 4; i++){
            if(*(nextMoveX + i) + xOffset < 0){
                return AOB_LEFT;
            }
            if(*(nextMoveX + i) + xOffset> gridWidth - 1){
                return AOB_RIGHT;
            }
            if(*(nextMoveY + i) + yOffset > gridHeight - 1){
                return AOB_DOWN;
            }
            if(grid[*(nextMoveX + i) + xOffset][*(nextMoveY + i) + yOffset] != 0){
                return BLOCKED;
            }
        }
        return PASS;
    }
    
    void AddScore(int linesCleared){
        
        // Original game boy/nintendo scoring multiplier
        int n = 0;
        switch (linesCleared) {
            case 1:
            n = 40;
            break;
            case 2:
            n = 100;
            break;
            case 3:
            n = 300;
            break;
            case 4:
            n = 1200;
            break;
        }
        
        score += n * (level + 1);  

        for(int i = 0; i < linesCleared; i++){
            lines++;
            if (lines % 10 == 0){
                level++;
                
                switch (level){
                    case 1:
                    gameSpeed = 49;
                    break;
                    case 2:
                    gameSpeed = 45;
                    break;
                    case 3:
                    gameSpeed = 41;
                    break;
                    case 4:
                    gameSpeed = 37;
                    break;
                    case 5:
                    gameSpeed = 33;
                    break;
                    case 6:
                    gameSpeed = 28;
                    break;
                    case 7:
                    gameSpeed = 22;
                    break;
                    case 8:
                    gameSpeed = 17;
                    break;
                    case 9:
                    gameSpeed = 11;
                    break;
                    case 10:
                    gameSpeed = 10;
                    break;
                    case 11:
                    gameSpeed = 9;
                    break;
                    case 12:
                    gameSpeed = 8;
                    break;
                    case 13:
                    gameSpeed = 7;
                    break;
                    case 14:
                    gameSpeed = 6;
                    break;
                    case 15:
                    gameSpeed = 6;
                    break;
                    case 16:
                    gameSpeed = 5;
                    break;
                    case 17:
                    gameSpeed = 5;
                    break;
                    case 18:
                    gameSpeed = 4;
                    break;
                    case 19:
                    gameSpeed = 4;
                    break;
                    case 20:
                    gameSpeed = 3;
                    break;
                    default:
                    gameSpeed = resetSpeed;                   
                }
            }
        }
    }
    
    void CheckLines(){

        int lineCount = 0;
        int checkLines = 0;

        for(int i = gridHeight -1; i >= 0; i--){

            int counter = 0;
            
            for(int j = 0; j < gridWidth; j++){
                if(grid[j][i] == 0)continue;
                counter++;
            }
            if(checkLines > 0) checkLines++;
            if(checkLines >= 4) break;
            if(counter == 0 && checkLines == 0) checkLines++;
            if(counter < gridWidth) continue;

            lineCount++;
            
            char* p = &grid[0][0];
            int x = 0;
            int y = i;
            
            while(x < gridWidth){
                *(p + y + x * gridHeight) = *(p + y + x * gridHeight - 1);
                y--;
                if (y == 0) {
                    *(p + y + x * gridHeight) = 0;
                    x++;
                    y = i;
                }
                
            }
            i++; // after clearing line, recheck same line
        }
        
        if (lineCount > 0) AddScore(lineCount);
    }
   
    void LockPiece(){
        for (int i = 0; i < 4; i++){
            grid[activeX[i] + xOffset][activeY[i] + yOffset] = activeIndex + 1;
        }
        pieceActive = false;
        timer = dropSpeed;
        CheckLines();
    }
    
    void NextOffsets(){
        switch(nextIndex){
            case 0:
            nextOffsetX = cellSize / 2;
            nextOffsetY = 0;
            break;
            case 1:
            nextOffsetX = -cellSize / 2;
            nextOffsetY = 0;
            break;
            case 2:
            nextOffsetX = cellSize / 2;
            nextOffsetY = cellSize;
            break;
            case 3:
            nextOffsetX = cellSize /2;
            nextOffsetY = cellSize;
            break;
            case 4:
            nextOffsetX = cellSize /2;
            nextOffsetY = cellSize;
            break;
            case 5:
            nextOffsetX = 0;
            nextOffsetY = cellSize;
            break;
            case 6:
            nextOffsetX = 0;
            nextOffsetY = cellSize / 2;
            break;
            default:
            nextOffsetX = 0;
            nextOffsetY = 0;            
        }
    }
    
    void SpawnPiece(){
        activeIndex = nextIndex;
        nextIndex = GetRandomValue(0,6);
        //NextOffsets(); // this is a problematic function
        nextPiece = getPiece(nextIndex);
        activePiece = getPiece(activeIndex);
        
        for(int i = 0; i < 4; i++){
            activeX[i] = *(activePiece + i);
            activeY[i] = *(activePiece + 4 + i);
            nextX[i] = *(nextPiece + i);
            nextY[i] = *(nextPiece + 4 + i);
        }
        
        xOffset = xStart;
        yOffset = yStart;
        rotationIndex = 0;
        NextOffsets();
        pieceActive = true;
        
        if (CheckCollision(&activeX[0], &activeY[0]) != PASS) {
            LockPiece();
            GameOver();
        }
    }

    void ClearGrid(){
        for (int i = 0; i < gridWidth; i++){
            for(int j = 0; j < gridHeight; j++){
                grid[i][j] = 0;
            }
        }
    }
    
    void MoveX(int dir){
        char nextMoveX[] = {activeX[0] + dir , activeX[1] + dir, activeX[2] + dir, activeX[3] + dir};
        char nextMoveY[] = {activeY[0], activeY[1], activeY[2], activeY[3]};
        if(CheckCollision(&nextMoveX[0], &nextMoveY[0]) == PASS){
            xOffset += dir;
        }
    }
    
    void DropPiece(){
        char nextMoveY[] = {activeY[0] + 1, activeY[1] + 1, activeY[2] + 1, activeY[3] + 1};
        char nextMoveX[] = {activeX[0], activeX[1], activeX[2], activeX[3]};
        if(CheckCollision(&nextMoveX[0], &nextMoveY[0]) == PASS){
            yOffset += 1;
        } else LockPiece();
    }
    
    void RotatePiece(int dir) {
        char nextMoveX[4];
        char nextMoveY[4];
        int nextRotation = rotationIndex + 4 + dir; // add 4 to keep the value always above zero
        nextRotation = nextRotation % 4;
        
        for(int i = 0; i < 4; i++){
            nextMoveX[i] = *(activePiece + (nextRotation * 8) + i);
            nextMoveY[i] = *(activePiece + (nextRotation * 8) + i + 4);
        }
        
        void apply_rotation(){
            for(int i = 0; i< 4; i++){
                activeX[i] = nextMoveX[i];
                activeY[i] = nextMoveY[i];
            }
            rotationIndex = nextRotation;
        }
        
        switch(CheckCollision(&nextMoveX[0], &nextMoveY[0])){
            case PASS:
            apply_rotation();
            break;
            case AOB_LEFT:
            for(int i = 0; i < 4; i++){
                nextMoveX[i] = nextMoveX[i] + 1;
            }
            if(CheckCollision(&nextMoveX[0], &nextMoveY[0]) == PASS) apply_rotation();
            
            break;
            case AOB_RIGHT:
            for(int i = 0; i < 4; i++){
                nextMoveX[i] = nextMoveX[i] - 1;
            }
            if(CheckCollision(&nextMoveX[0], &nextMoveY[0]) == PASS) apply_rotation();            
            break;
            default:
            break;
            
        }
        
        
        
        
        
        if(CheckCollision(&nextMoveX[0], &nextMoveY[0]) == PASS){
            for(int i = 0; i< 4; i++){
                activeX[i] = nextMoveX[i];
                activeY[i] = nextMoveY[i];
            }
            rotationIndex = nextRotation;
        } 
    }
    
    void Input(){
        // Debug input *********
        if(IsKeyPressed(KEY_G)){
            GameOver();
        }
        // *********************
        
        if(gameState == GAME_OVER && IsKeyPressed(KEY_SPACE)){
            ClearGrid();
            score = 0;
            lines = 0;
            level = 0;
            gameSpeed = resetSpeed;
            gameState = GAME_PLAY;
            SpawnPiece();
        }
        
        if(IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)){
            MoveX(1);
        }
        if(IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)){
            MoveX(-1);
        }
        
        if(IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)){
            timer = dropSpeed;
        } else {
            if(pieceActive) timer = gameSpeed;
        }
        
        if(IsKeyPressed(KEY_E) || IsKeyPressed(KEY_RIGHT_CONTROL)){
            RotatePiece(1);
        }
        if(IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP) ){
            RotatePiece(-1);
        }
    }

    void Update(){
        switch(gameState){
            case GAME_BEGIN:
            break;
            case GAME_PLAY:
            
            sprintf(levelText, "Level %d", level);
            sprintf(scoreText, "Score: %d", score);
            sprintf(linesText, "Lines: %d", lines);
            
            waitTime += dt;
                if (waitTime >= timer * dt){
                    waitTime = 0.0;
                    if(pieceActive) DropPiece();
                    else SpawnPiece();
                }
            break;
            case GAME_OVER:
            default:
            break;
        }
    }

    void Draw(){
        DrawRectangle(borderX, borderY, borderW, borderH, GOLD);
        DrawRectangle(gridX, gridY, gridWidth * cellSize, gridHeight * cellSize, DARKGRAY);
        
        DrawRectangle(346, borderY, 108, 108, GOLD);
        DrawRectangle(349, gridY, 102, 102, DARKGRAY);
            

            
        // draw active blocks and next block
        if(pieceActive){
            for (int i = 0; i < 4; i++){
                DrawRectangle(gridX + 1 + (xOffset + activeX[i]) * cellSize, gridY + 1 + (yOffset + activeY[i]) * cellSize, squareSize, squareSize, colors[activeIndex + 1]);
            }
            
            // draw next piece
            for (int i = 0; i < 4; i++){
                DrawRectangle(350 + 1 + nextX[i] * cellSize + nextOffsetX, 50 + 1 + nextY[i] * cellSize + nextOffsetY, squareSize, squareSize, colors[nextIndex + 1]);
            }
            
        }

        // draw static blocks in grid
        for (int i = 0; i < gridWidth; i++){
            for(int j = 0; j < gridHeight; j++){
               if (grid[i][j] == 0) continue; // skip drawing if a cell is empty 
               DrawRectangle(gridX + i * cellSize + 1, gridY + j * cellSize + 1, squareSize, squareSize, colors[(int)grid[i][j]]);
            }
        }
        
        // display score
        DrawText("-next-", 372, 130, 18, GOLD);
        DrawText(levelText, 350, 200, 20, BLACK);
        DrawText(scoreText, 350, 240, 20, BLACK);
        DrawText(linesText, 350, 280, 20, BLACK);
        
        if(gameState == GAME_OVER){
            DrawRectangle(5,340, screenWidth - 10, 100, DARKBROWN);
            DrawText("YOU LOSE, GOOD DAY SIR!", 20, screenHeight / 2 + 50, 35, RAYWHITE);
            DrawText("press space to start again", 80, screenHeight / 2 + 100, 25, GRAY);
        }
    }
    
    InitWindow(screenWidth, screenHeight, "Tetris 2077!");
    SetTargetFPS(FPS);

    SetRandomSeed((int)time(NULL));
    
    ClearGrid();
    SpawnPiece();

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        
        Input();
        
        Update();

        // Draw

        BeginDrawing();

            ClearBackground(GRAY);
                        
            Draw();

        EndDrawing();

    }
    // close audio/end music stream, etc. 

    CloseWindow();        // Close window and OpenGL context

    return 0;
}