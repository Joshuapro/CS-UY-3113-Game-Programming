#include "Entity.h"


Entity::Entity()
{
    position = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    modelMatrix = glm::mat4(1.0f);
    rotation = glm::vec3(0);
    scale = glm::vec3(1);
    width = 1.0f;
    height = 1.0f;
    depth = 1.0f;
    billboard = false;
    alive = true;
    
    speed = 0.0f;
}

bool Entity::CheckCollision(Entity *other){
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    float zdist = fabs(position.z - other->position.z) - ((depth + other->depth) / 2.0f);
    if (xdist < 0 && ydist < 0 && zdist < 0) return true;
    return false;
}

void Entity::Update(float deltaTime, Entity *player, Entity *objects, int objectCount, Entity *enemies, int enemyCount)
{
    if (not alive) return;
    
    glm::vec3 previousPosition = position;
    
    if (billboard) {
        
        float directionX = position.x - player->position.x;
        float directionZ = position.z - player->position.z;
        rotation.y = glm::degrees(atan2f(directionX, directionZ));
        
        if (entityType == ENEMY){
            velocity.z = cos(glm::radians(rotation.y)) * -3.0f;
            velocity.x = sin(glm::radians(rotation.y)) * -3.0f;
        }
    }
    
    if(entityType == BULLET){
        float zSquare = (pow(player->position.z - position.z,2));
        float xSquare = (pow(player->position.x - position.x,2));
        
        if(sqrt(xSquare + zSquare) >= 10){
            alive = false;
        }
        
        for (int i = 0; i<enemyCount; i++){
            if (CheckCollision(&enemies[i]) and alive){
                if (enemies[i].alive){
                    alive = false;
                    enemies[i].alive = false;
                }
            }
        }
    }
    
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    
    if(entityType == PLAYER){
        for (int i = 0; i < objectCount; i++){
            // Ignore collisions with the floor
            if (objects[i].entityType == FLOOR) continue;
            
            if (CheckCollision(&objects[i])) {
                position = previousPosition;
                break;
            }
        }
        for (int i = 0; i<enemyCount; i++){
            if (CheckCollision(&enemies[i]) and enemies[i].alive){
                lives--;
                enemies[i].alive = false;
            }
        }
    }

    
    if (entityType == CUBE){
        rotation.y += 45 * deltaTime;
        rotation.z += 45 * deltaTime;
    }else if(entityType == ENEMY){
        rotation.y += 30 * deltaTime;
    }
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix,scale);
    modelMatrix = glm::rotate(modelMatrix,glm::radians(rotation.y),glm::vec3(0,1,0));
    modelMatrix = glm::rotate(modelMatrix,glm::radians(rotation.z),glm::vec3(0,0,1));
}

void Entity::Render(ShaderProgram *program) {
    if (not alive) return;

    program->SetModelMatrix(modelMatrix);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    if (billboard) {
        DrawBillboard(program);
    } else {
        mesh->Render(program);
    }
}

void Entity::DrawBillboard(ShaderProgram *program) {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

