#include "Scene.h"
class Menu : public Scene {
    
public:
    bool start = false;
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};

