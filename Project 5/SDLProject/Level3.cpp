#include "Level3.h"
#define LEVEL3_WIDTH 14
#define LEVEL3_HEIGHT 8

#define LEVEL3_ENEMY_COUNT 3

unsigned int level3_data[] =
{
    3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 3, 3, 3,
    3, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
    3, 3, 3, 3, 3, 0, 3, 3, 0, 0, 3, 3, 0, 0,
    3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3,
    3, 3, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};




Mix_Chunk* bounceie;

void Level3::Initialize() {
    
    state.nextScene = -1;
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    bounceie = Mix_LoadWAV("bounce.wav");
    
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
    
    state.player = new Entity();
    state.player->entitytype = PLAYER;
    state.player->position = glm::vec3(1,3 ,0);
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
    
    state.player->jumpPower = 4.5f;
    
    
    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");
    
    
    state.enemies[0].entitytype = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(4,-1,0);
    state.enemies[0].speed = 1;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].isActive = true;
    state.enemies[0].acceleration = glm::vec3(0,-9.81f,0);
    
    state.enemies[1].entitytype = ENEMY;
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(5,-5.25,0);
    state.enemies[1].speed = 1;
    state.enemies[1].aiType = WALKER;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].isActive = true;
    state.enemies[1].acceleration = glm::vec3(0,-9.81f,0);
    
    state.enemies[2].entitytype = ENEMY;
    state.enemies[2].textureID = enemyTextureID;
    state.enemies[2].position = glm::vec3(3,-5.25,0);
    state.enemies[2].speed = 1;
    state.enemies[2].aiType = WALKER;
    state.enemies[2].aiState = IDLE;
    state.enemies[2].isActive = true;
    state.enemies[2].acceleration = glm::vec3(0,-9.81f,0);
    // Move over all of the player and enemy code from initialization.
}
void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    state.enemies[0].Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    state.enemies[1].Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    state.enemies[2].Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    if(state.enemies[0].collidedTop){
        Mix_PlayChannel(-1, bounceie, 0);
        state.enemies[0].collidedTop = false;
    }
    if(state.enemies[1].collidedTop){
        Mix_PlayChannel(-1, bounceie, 0);
        state.enemies[1].collidedTop = false;
    }
    if(state.enemies[2].collidedTop){
        Mix_PlayChannel(-1, bounceie, 0);
        state.enemies[2].collidedTop = false;
    }
    if(state.player->position.x >= 12 and state.player->collidedBottom){
        state.player->winGame = true;
    }
}
void Level3::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player->Render(program);
    state.enemies[0].Render(program);
    state.enemies[1].Render(program);
    state.enemies[2].Render(program);
}
