#include "Scene.h"
#include <SDL_mixer.h>
class Level2 : public Scene {
public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};
