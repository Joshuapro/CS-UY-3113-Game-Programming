
#include "Level2.h"

#define  Level2_OBJECT_COUNT 1
#define LEVEL2_ENEMY_COUNT 30






void Level2::Initialize() {
    
    state.nextScene = -1;
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, 0.5f, 0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 1.0f;
    state.player->lives = 3;
    
    state.objects = new Entity[Level2_OBJECT_COUNT];

    GLuint floorTextureID = Util::LoadTexture("grass.jpg");
    Mesh *cubeMesh = new Mesh();
    cubeMesh->LoadOBJ("cube.obj",20);

    state.objects[0].textureID = floorTextureID;
    state.objects[0].mesh = cubeMesh;
    state.objects[0].position = glm::vec3(0,-0.35,0);
    state.objects[0].entityType = FLOOR;
    state.objects[0].rotation = glm::vec3(0,0,0);
    state.objects[0].scale = glm::vec3(100,0.5,100);

    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");
    for (int i = 0; i<LEVEL2_ENEMY_COUNT; i++){
        state.enemies[i].billboard = true;
        state.enemies[i].textureID = enemyTextureID;
        int selector = rand() % 4;
        int xval = rand() % 20 + 10;
        int zval = rand() % 20 + 10;
        if (selector){
            xval *= -1;
            zval *= -1;
        }
        if (selector == 2){
            xval *= -1;
        }
        if (selector == 3){
            zval *= -1;
        }
        state.enemies[i].position = glm::vec3(xval, 0.5, zval);
        state.enemies[i].rotation = glm::vec3(0, 0, 0);
        state.enemies[i].acceleration = glm::vec3(0, 0, 0);
        state.enemies[i].entityType = ENEMY;
    }

    
}


void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.objects, Level2_OBJECT_COUNT,state.enemies,LEVEL2_ENEMY_COUNT);
    state.objects[0].Update(deltaTime, state.player, state.objects, Level2_OBJECT_COUNT, state.enemies,LEVEL2_ENEMY_COUNT);
    for (int i = 0; i< LEVEL2_ENEMY_COUNT; i++){
        state.enemies[i].Update(deltaTime, state.player, state.objects, Level2_OBJECT_COUNT, state.enemies,LEVEL2_ENEMY_COUNT);
    }

    for (int i = 0; i<state.bullets.size(); i++){
        state.bullets[i]->Update(deltaTime, state.player, state.objects, Level2_OBJECT_COUNT, state.enemies,LEVEL2_ENEMY_COUNT);
    }

    
}



void Level2::Render(ShaderProgram *program) {
    state.objects[0].Render(program);
    for (int i = 0; i<LEVEL2_ENEMY_COUNT; i++){
        state.enemies[i].Render(program);
    }
    for (int i = 0; i<state.bullets.size(); i++){
        state.bullets[i]->Render(program);
    }

}
