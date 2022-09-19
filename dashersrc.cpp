#include <raylib.h>


struct AnimData

{
Rectangle rec;
Vector2 pos;
int frame;
float updateTime;
float runningTime;
};


bool isOnGround(AnimData data, int windowHeight)
{

    return data.pos.y >= windowHeight - data.rec.height;

}
// int maxFrame is added because character has a max of 5 frames, whereas nebulae have 7.
AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    //update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        //update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
        
    }
    return data;
}


int main()
{
    //Using an Array for windowDimensions
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;


    /* Window dimensions, const variables must be initialized immediately. 
    So we use curly brackets, or braced initialization. */
    InitWindow(windowDimensions[0], windowDimensions[1], "Dasher!");

    // acceleration due to gravity (pixels/s)/s
    const int gravity{1'000};
    //PPS
    const int jmpVel{-600};


    //Nebula init & variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae{6};
    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].updateTime = 1/12;
        nebulae[i].runningTime = 0.0;
    }

    nebulae[0].pos.x = windowDimensions[0];
    nebulae[1].pos.x = windowDimensions[0] + 300;
    nebulae[2].pos.x = windowDimensions[0] + 600;
    nebulae[3].pos.x = windowDimensions[0] + 900;
    nebulae[4].pos.x = windowDimensions[0] + 1200;
    nebulae[5].pos.x = windowDimensions[0] + 1500;


    //Neb X velocity (PpS)
    int nebVel{-200};


    int velocity{0};
    bool isInAir{};

    //Character texture init
    Texture2D character = LoadTexture("textures/scarfy.png");
    AnimData charData;
    charData.rec.width = character.width/6; 
    charData.rec.height = character.height;
    charData.rec.x = 0;
    charData.rec.y = 0;
    charData.pos.x = windowDimensions[0]/2 - charData.rec.width/2;
    charData.pos.y = windowDimensions[1] - charData.rec.height;
    charData.frame = 0;
    charData.updateTime = 1.0/12;
    charData.runningTime = 0.0;

    Texture2D bg = LoadTexture("textures/far-buildings.png");
    float bgX{};
    // Texture mid-ground and fore-ground
    Texture2D mg = LoadTexture("textures/back-buildings.png");
    float mgX{};
    Texture2D fg = LoadTexture("textures/foreground.png"); 
    float fgX{};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {

        const float dT{GetFrameTime()};

        //Start drawing
        BeginDrawing();
        //We need the background to be white to prevent GI strobing.
        ClearBackground(WHITE);
        //Scrolling by -20 pixels times Delta Time.
        bgX -= 20 * dT;
        if (bgX <= -bg.width*2)
        {
            bgX = 0.0;
        }

        mgX -= 40 * dT;
        if (mgX <= -mg.width*2)
        {
            mgX = 0.0;
        }

        fgX -= 80 * dT;
        if (fgX <= -fg.width*2)
        {
            fgX = 0.0;
        }
        
        //Draw background
        Vector2 bg1Pos {bgX, 0.0};
        DrawTextureEx(bg, bg1Pos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos {bgX + bg.width*2, 0.0};
        DrawTextureEx(bg, bg2Pos, 0.0, 2.0, WHITE);
        //Draw midground
        Vector2 mg1Pos {mgX, 0.0};
        DrawTextureEx(mg, mg1Pos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos {mgX + mg.width*2, 0.0};
        DrawTextureEx(mg, mg2Pos, 0.0, 2.0, WHITE);
        //Draw foreground
        Vector2 fg1Pos {fgX, 0.0};
        DrawTextureEx(fg, fg1Pos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos {fgX + fg.width*2, 0.0};
        DrawTextureEx(fg, fg2Pos, 0.0, 2.0, WHITE);

        //Ground check
        if (isOnGround(charData, windowDimensions[1]))
        {
            // rectangle on ground
            velocity = 0 && isInAir == false;
            isInAir = false;
        }
        else 
        {
            //rectangle in air, apply gravity
            velocity += gravity * dT;
            isInAir = true;
        }
        //check for jumping
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jmpVel;
        }


        for (int i = 0; i < sizeOfNebulae; i++)
        {

            // Update position of nebula hazard
            nebulae[i].pos.x += nebVel * dT;

        }

        // Update pos of second nebula hazard
        nebulae[1].pos.x += nebVel * dT;

        // Update position of character
        charData.pos.y += velocity * dT;

        //Update character anim frame
        if (!isInAir)
        {
            charData = updateAnimData(charData, dT, 5);
        }

        //Update nebula one's animation frame

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }

    for (int i = 0; i < sizeOfNebulae; i++)
    {

        //Draw danger nebula hazard
        DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);

    }

    //Draw character
    DrawTextureRec(character, charData.rec, charData.pos, WHITE);

    //Stop drawing
    EndDrawing();

    }
    //Unload texture
    UnloadTexture(character);
    UnloadTexture(nebula);
    UnloadTexture(bg);
    UnloadTexture(mg);
    UnloadTexture(fg);
    CloseWindow();
}