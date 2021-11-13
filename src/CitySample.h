#ifndef S_CITYSAMPLE_H
#define S_CITYSAMPLE_H

#include "../thirdparty/irrKlang/include/irrKlang.h"
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"
#include "../samplefw/Grid3D.h"
#include "../samplefw/OrbitCamera.h"

#include "types.h"
#include "Plane.h"

class CitySample: public Sample
{
public:
    CitySample(wolf::App* pApp);
    ~CitySample();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

    void handleKeyPress(char key) override;
    void goToSleep() override;

private:
    void _initAudio();
    void _initImGui();
    void _renderImGui();
    float _calculateHammer(float time);
    float _pulse(float t);

    bool m_renderDebugUI = false;

    float m_time = 0.0f;

    wolf::Material* m_pMat = nullptr;
    wolf::Program* m_pProgram = nullptr;
    GLuint m_shader = 0;

    wolf::Model* m_pCitadel = nullptr;
    wolf::Model* m_pCitadelPiece = nullptr;
    Plane *m_pPlane = nullptr;

    Grid3D* m_pGrid = nullptr;
    OrbitCamera* m_pOrbitCam = nullptr;
    irrklang::ISoundEngine* m_pAudioEngine = nullptr;

    ImVec4 m_clearColor = ImVec4(0.0,0.0,0.0,0.0);
};

#endif // S_CITYSAMPLE_H
