
#include "MenuScreen.h"
GLuint fontTextureID;

void MenuScreen::Initialize() {
    
    state.nextScene = -1;
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, 0.5f, 0);
    state.player->acceleration = glm::vec3(0, 0, 0);
    state.player->enemiesKilled = 0;
    fontTextureID = Util::LoadTexture("font1.png");
}


void MenuScreen::Update(float deltaTime) {
    
}



void MenuScreen::Render(ShaderProgram *program) {
    Util::DrawText(program, fontTextureID, "Killer", 1, 1, glm::vec3(2,1,0));
}

