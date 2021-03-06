#include "Scene.h"
#include <SDL_mixer.h>

class TutorialLevel : public Scene {
public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};
