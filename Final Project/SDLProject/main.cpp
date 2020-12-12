#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "MenuScreen.h"
#include "TutorialLevel.h"



SDL_Window* displayWindow;
bool gameIsRunning = true;
bool playerStart = false;
bool gameFinish = false;
bool lose = false;
ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Mix_Music* music;
Mix_Chunk* music_shooting;

Scene *sceneList[4];

glm::mat4 uiViewMatrix, uiProjectionMatrix;
GLuint fontTextureID;
GLuint heartTextureID;
//struct GameState {
//    Entity *player;
//    Entity *objects;
//    Entity *enemies;
//};
Scene *currentScene;

void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}

GameState state;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("3D!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 720);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::perspective(glm::radians(45.0f), 1.777f, 0.1f, 100.0f);
    
    
    
    uiViewMatrix = glm::mat4(1.0);
    uiProjectionMatrix = glm::ortho(-6.4f, 6.4f, -3.6f, 3.6f, -1.0f, 1.0f);
    
    fontTextureID = Util::LoadTexture("font1.png");
    heartTextureID = Util::LoadTexture("platformPack_item017.png");
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0, 0, 0, 1.0f);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);


    
    sceneList[0] = new MenuScreen();
    sceneList[1] = new TutorialLevel();
    sceneList[2] = new Level1();
    sceneList[3] = new Level2();
    
    SwitchToScene(sceneList[0]);
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("guccigang.mp3");
    Mix_PlayMusic(music, -1);
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music_shooting = Mix_LoadWAV("shooting.wav");
    
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_RETURN:
                        if (not playerStart){
                            SwitchToScene(sceneList[1]);
                            playerStart = true;
                        }
                        break;
                    case SDLK_SPACE:
                        // Some sort of action
                        GLuint enemytextureID = Util::LoadTexture("heart.png");
                        Entity *bullet = new Entity;
                        bullet->entityType = BULLET;
                        bullet->textureID = enemytextureID;
                        bullet->position = currentScene->state.player->position;
                        bullet->scale = glm::vec3(0.2,0.2,0.2);
                        bullet->billboard = true;
                        bullet->velocity.z = cos(glm::radians(currentScene->state.player->rotation.y)) * -10.0f;
                        bullet->velocity.x = sin(glm::radians(currentScene->state.player->rotation.y)) * -10.0f;
                        currentScene->state.bullets.push_back(bullet);
                        Mix_PlayChannel(-1, music_shooting, 0);
                        break;

                }
                break;
        }
    }
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (not gameFinish){
        if (keys[SDL_SCANCODE_A]) {
            currentScene->state.player->rotation.y += 1.3f;
        } else if (keys[SDL_SCANCODE_D]) {
            currentScene->state.player->rotation.y -= 1.3f;
        }
        
        currentScene->state.player->velocity.x = 0;
        currentScene->state.player->velocity.z = 0;
        
        if (keys[SDL_SCANCODE_W]) {
            currentScene->state.player->velocity.z = cos(glm::radians(currentScene->state.player->rotation.y)) * -currentScene->state.player->speed;
            currentScene->state.player->velocity.x = sin(glm::radians(currentScene->state.player->rotation.y)) * (-currentScene->state.player->speed);
        } else if (keys[SDL_SCANCODE_S]) {
            currentScene->state.player->velocity.z = cos(glm::radians(currentScene->state.player->rotation.y)) * currentScene->state.player->speed;
            currentScene->state.player->velocity.x = sin(glm::radians(currentScene->state.player->rotation.y)) * currentScene->state.player->speed;
        }
    }
    
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP) {
        currentScene->Update(FIXED_TIMESTEP);
        
        deltaTime -= FIXED_TIMESTEP;
    }
    if(not gameFinish){
        if (currentScene == sceneList[1] and currentScene->state.player->enemiesKilled == 10 and currentScene->state.player->lives){
            SwitchToScene(sceneList[2]);
        }
        if (currentScene == sceneList[2] and currentScene->state.player->enemiesKilled == 50 and currentScene->state.player->lives){
            SwitchToScene(sceneList[3]);
        }
        
        if (currentScene == sceneList[3] and currentScene->state.player->enemiesKilled == 30 and currentScene->state.player->lives){
            gameFinish = true;
        }
    }
    
    
    
    if (not currentScene->state.player->lives and currentScene != sceneList[0]){
        lose = true;
        gameFinish = true;
    }
    
    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::rotate(viewMatrix,glm::radians(currentScene->state.player->rotation.y), glm::vec3(0, -1.0f, 0));
    viewMatrix = glm::translate(viewMatrix, -currentScene->state.player->position);
    
    program.SetViewMatrix(viewMatrix);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    currentScene->Render(&program);
    
    
    program.SetProjectionMatrix(uiProjectionMatrix);
    program.SetViewMatrix(uiViewMatrix);
    
    
    if (not playerStart){
        Util::DrawText(&program, fontTextureID, "The Killer", 0.5, 0.005, glm::vec3(-2,1,0));
        Util::DrawText(&program, fontTextureID, "Press Enter to start and space to shoot", 0.5, -0.3f, glm::vec3(-3.5, -1 , 0));
    }
    
    if (playerStart){
        Util::DrawText(&program, fontTextureID, "Lives: ", 0.5, -0.3f, glm::vec3(-6, 3.2, 0));
        for (int i = 0; i < currentScene->state.player->lives; i++){
            Util::DrawIcon(&program, heartTextureID, glm::vec3(-4.5 + (i * 0.5f), 3.2, 0));
        }
    }
    
    if (gameFinish){
        if (lose){
            Util::DrawText(&program, fontTextureID, "You have LOST!", 0.5, -0.3f, glm::vec3(-1, 0 , 0));
        }
        else{
            Util::DrawText(&program, fontTextureID, "Congrats! You have won!", 0.5, -0.3f, glm::vec3(-2, 0 , 0));
        }
    }
    
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
