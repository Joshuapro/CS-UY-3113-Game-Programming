#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;


SDL_Window* displayWindow;
bool gameIsRunning = true;
bool start = false;

float ball_x, ball_y, ball_speed_x = 3, ball_speed_y = 3;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix, modelMatrix_player2, modelMatrix_ball;

glm::vec3 player_position = glm::vec3(-4.8,0,0);
glm::vec3 player_movement = glm::vec3(0,0,0);


glm::vec3 player_position_two = glm::vec3(4.8,0,0);
glm::vec3 player_movement_two = glm::vec3(0,0,0);

glm::vec3 ball_position = glm::vec3(0,0,0);

float player_speed = 2.5;

Mix_Music *music;
Mix_Chunk *bounce;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Project 2!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640*2, 480*2);
    
    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("dooblydoo.mp3");
    Mix_PlayMusic(music, -1);
    
    bounce = Mix_LoadWAV("bounce.wav");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    modelMatrix_player2 = glm::mat4(1.0f);
    modelMatrix_ball = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    int num = rand()%4;
    
    if (num == 0){
        ball_x = 1;
        ball_y = 1;
    }
    else if (num == 1){
        ball_x = -1;
        ball_y = 1;
    }
    else if (num == 2){
        ball_x = -1;
        ball_y = -1;
    }else{
        ball_x = 1;
        ball_y = -1;
    }
    

}

void ProcessInput() {
    
    player_movement = glm::vec3(0);
    player_movement_two = glm::vec3(0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                    case SDLK_RIGHT:
                        // Move the player right

                        break;
                    case SDLK_SPACE:
                        // Some sort of action
                        start = true;
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    if (start){
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_UP]) {
            if (player_position_two.y <= 3.15){
                player_movement_two.y = 1.0f;
            }
        }
        else if (keys[SDL_SCANCODE_DOWN]) {
            if (player_position_two.y >= -3.15){
                player_movement_two.y = -1.0f;
            }
        }
        
        if (keys[SDL_SCANCODE_W]) {
            if (player_position.y <= 3.15){
                player_movement.y = 1.0f;
            }
        }
        else if (keys[SDL_SCANCODE_S]) {
            if (player_position.y >= -3.15){
                player_movement.y = -1.0f;
            }
        }
    }
    
}

float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    // Add (direction * units per second * elapsed time)
    player_position += player_movement * player_speed * deltaTime;
    player_position_two += player_movement_two * player_speed * deltaTime;
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix,player_position);
    
    modelMatrix_player2 = glm::mat4(1.0f);
    modelMatrix_player2 = glm::translate(modelMatrix_player2,player_position_two);
    
    modelMatrix_ball = glm::mat4(1.0f);

    if(start){
        
        if((ball_position.x <= -5) or (ball_position.x >= 5)){
            gameIsRunning = false;
        }
        
        if (ball_position.y >= 3.55 or ball_position.y <= -3.55){
            ball_speed_y *= -1;
        }
        
        float xdist = fabs(player_position.x - ball_position.x) - (0.9 / 2.0f);
        float ydist = fabs(player_position.y - ball_position.y) - (2.1 / 2.0f);
        
        float xdist_p2 = fabs(player_position_two.x - ball_position.x) - (0.9 / 2.0f);
        float ydist_p2 = fabs(player_position_two.y - ball_position.y) - (2.1 / 2.0f);
        
        if ((xdist < 0 and ydist < 0) or (xdist_p2 < 0 and ydist_p2 < 0)){
            ball_speed_x *= -1;
        }
        
        ball_position.x += ball_x * ball_speed_x * deltaTime;
        ball_position.y += ball_y * ball_speed_y * deltaTime;
        
        modelMatrix_ball = glm::translate(modelMatrix_ball, ball_position);

        
    }
        
    
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    float vertices[] = { -0.2, -0.8, 0.2, 0.8, -0.2, 0.8, 0.2, 0.8, -0.2, -0.8, 0.2, -0.8 };
    float ballCoords[] = { -0.25, -0.25, -0.25, 0.25, 0.25, 0.25, 0.25, 0.25, -0.25, -0.25, 0.25, -0.25 };
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    //first paddle
    program.SetModelMatrix(modelMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    //second paddle
    program.SetModelMatrix(modelMatrix_player2);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ballCoords);
    program.SetModelMatrix(modelMatrix_ball);
    
    //ball
    glEnableVertexAttribArray(program.texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
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
