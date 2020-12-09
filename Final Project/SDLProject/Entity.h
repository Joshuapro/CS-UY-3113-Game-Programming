#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Mesh.h"

enum EntityType { PLAYER, PLATFORM, ENEMY, CUBE,SHIP, FLOOR,CRATE, BULLET};

class Entity {
public:
    EntityType entityType;
    
    Mesh *mesh;
    
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 rotation;
    glm::vec3 scale;
    
    float speed;
    
    
    bool gameEnd = false;
    bool billboard;
    float width;
    float height;
    float depth;
    int lives;
    bool alive = true;

    
    GLuint textureID;
    
    glm::mat4 modelMatrix;
    
    Entity();
    
    bool CheckCollision(Entity *other);
    void Update(float deltaTime, Entity *player, Entity *objects, int objectCount, Entity *enemies, int enemyCount);
    void Render(ShaderProgram *program);
    void DrawBillboard(ShaderProgram *program);
//    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
};



