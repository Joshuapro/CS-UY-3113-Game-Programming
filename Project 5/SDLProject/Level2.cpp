#include "Level2.h"
#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8

#define LEVEL2_ENEMY_COUNT 1

unsigned int level2_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 1, 0, 0, 2, 0, 0, 3, 3, 3, 0, 0,
    3, 1, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 3,
    3, 2, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 3,
    3, 2, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 3
};


Mix_Chunk* bouncey;



void Level2::Initialize() {
    
    state.nextScene = -1;
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    bouncey = Mix_LoadWAV("bounce.wav");
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);
    
    state.player = new Entity();
    state.player->entitytype = PLAYER;
    state.player->position = glm::vec3(1,-2,0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0,-9.81f,0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("george_0.png");
    
    state.player->animRight = new int[4] {3, 7, 11, 15};
    state.player->animLeft = new int[4] {1, 5, 9, 13};
    state.player->animUp = new int[4] {2, 6, 10, 14};
    state.player->animDown = new int[4] {0, 4, 8, 12};

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    state.player->height = 0.8f;
    state.player->width = 0.6f;
    
    state.player->jumpPower = 3.5f;
    
    
    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");
    
    
    state.enemies[0].entitytype = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(11,0,0);
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].isActive = true;
    state.enemies[0].acceleration = glm::vec3(0,-9.81f,0);
    
}
void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
    
    state.enemies[0].Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
    
    if(state.player->position.x >= 12 and state.player->collidedBottom){
        state.nextScene = 3;
    }
    
    if(state.enemies[0].collidedTop){
        Mix_PlayChannel(-1, bouncey, 0);
        state.enemies[0].collidedTop = false;
    }
//    std::cout << "a"<< state.player->position.x;
//    std::cout<<".b";
//    std::cout<<state.player->position.y ;
}
void Level2::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    state.enemies[0].Render(program);
}
