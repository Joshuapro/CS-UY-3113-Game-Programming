//
//  Level1.cpp
//  SDLProject
//
//  Created by Joshua Young on 12/8/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Level1.h"

#define OBJECT_COUNT 1
#define LEVEL1_ENEMY_COUNT 10






void Level1::Initialize() {
    
    state.nextScene = -1;
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, 0.5f, 0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 1.0f;
    state.player->lives = 3;
    state.player->enemiesKilled = 0;
    
    state.objects = new Entity[OBJECT_COUNT];
    
    GLuint floorTextureID = Util::LoadTexture("grass.jpg");
    Mesh *cubeMesh = new Mesh();
    cubeMesh->LoadOBJ("cube.obj",20);
    
    state.objects[0].textureID = floorTextureID;
    state.objects[0].mesh = cubeMesh;
    state.objects[0].position = glm::vec3(0,-0.35,0);
    state.objects[0].entityType = FLOOR;
    state.objects[0].rotation = glm::vec3(0,0,0);
    state.objects[0].scale = glm::vec3(100,0.5,100);
    
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");
    for (int i = 0; i<LEVEL1_ENEMY_COUNT; i++){
        state.enemies[i].billboard = true;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].position = glm::vec3(rand() % 40 - 10, 0.5, -(rand() % 30) - 80);
        state.enemies[i].rotation = glm::vec3(0, 0, 0);
        state.enemies[i].acceleration = glm::vec3(0, 0, 0);
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].enemySpeed = -4.5;
    }

    
}


void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.objects, OBJECT_COUNT,state.enemies,LEVEL1_ENEMY_COUNT);
    state.objects[0].Update(deltaTime, state.player, state.objects, OBJECT_COUNT, state.enemies,LEVEL1_ENEMY_COUNT);
    for (int i = 0; i< LEVEL1_ENEMY_COUNT; i++){
        state.enemies[i].Update(deltaTime, state.player, state.objects, OBJECT_COUNT, state.enemies,LEVEL1_ENEMY_COUNT);
    }
    
    for (int i = 0; i<state.bullets.size(); i++){
        state.bullets[i]->Update(deltaTime, state.player, state.objects, OBJECT_COUNT, state.enemies,LEVEL1_ENEMY_COUNT);
    }

    
}



void Level1::Render(ShaderProgram *program) {
//    state.player->level1::Render(program);
    state.objects[0].Render(program);
    for (int i = 0; i<LEVEL1_ENEMY_COUNT; i++){
        state.enemies[i].Render(program);
    }
    for (int i = 0; i<state.bullets.size(); i++){
        state.bullets[i]->Render(program);
    }
}

