//
//  TutorialLevel.cpp
//  SDLProject
//
//  Created by Joshua Young on 12/11/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "TutorialLevel.h"

#define OBJECT_COUNT 1
#define ENEMY_COUNT 10






void TutorialLevel::Initialize() {
    
    state.nextScene = -1;
    
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, 0.5f, 0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->speed = 4;
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
    
    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("keiko.png");
    for (int i = 0; i<ENEMY_COUNT; i++){
        state.enemies[i].billboard = true;
        state.enemies[i].textureID = enemyTextureID;
        int selector = rand() % 4;
        int xval = rand() % 5 + 5;
        int zval = rand() % 5 + 5;
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
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].enemySpeed = -4.5;
        state.enemies[i].noMove = true;
    }

    
}


void TutorialLevel::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.objects, OBJECT_COUNT,state.enemies,ENEMY_COUNT);
    state.objects[0].Update(deltaTime, state.player, state.objects, OBJECT_COUNT, state.enemies,ENEMY_COUNT);
    for (int i = 0; i< ENEMY_COUNT; i++){
        state.enemies[i].Update(deltaTime, state.player, state.objects, OBJECT_COUNT, state.enemies,ENEMY_COUNT);
    }
    
    for (int i = 0; i<state.bullets.size(); i++){
        state.bullets[i]->Update(deltaTime, state.player, state.objects, OBJECT_COUNT, state.enemies,ENEMY_COUNT);
    }

    
}



void TutorialLevel::Render(ShaderProgram *program) {
    state.objects[0].Render(program);
    for (int i = 0; i<ENEMY_COUNT; i++){
        state.enemies[i].Render(program);
    }
    for (int i = 0; i<state.bullets.size(); i++){
        state.bullets[i]->Render(program);
    }
}

