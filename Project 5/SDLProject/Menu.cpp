//
//  Menu.cpp
//  SDLProject
//
//  Created by Joshua Young on 11/18/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Util.h"
#include "Menu.h"
GLuint fontTextureIDlol;


void Menu::Initialize() {
    state.nextScene = -1;
    state.player = new Entity();
    state.player->entitytype = PLAYER;
    fontTextureIDlol = Util::LoadTexture("font1.png");
}

void Menu::Update(float deltaTime) {
    
}


void Menu::Render(ShaderProgram *program) {
    Util::DrawText(program, fontTextureIDlol, "The jumper", 0.5, 0.005, glm::vec3(3,-1,0));
    Util::DrawText(program, fontTextureIDlol, "Press Enter to start", 0.5, 0.003, glm::vec3(0.3,-3,0));
}
