#ifndef S_CITYSAMPLE_H
#define S_CITYSAMPLE_H

#include "../thirdparty/irrKlang/include/irrKlang.h"
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"
#include "../samplefw/Grid3D.h"
#include "../samplefw/OrbitCamera.h"

#include "types.h"
#include "Plane.h"
#include "FirstPersonCamera.h"

class CitySample: public Sample
{
public:
    CitySample(wolf::App* pApp);
    ~CitySample();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

    void goToSleep() override;
    void setRenderDebugUI(bool renderDebugUI) override;

private:
    void _initAudio();
    void _initImGui();
    void _renderImGui();
    float _calculateCitadelPieceHammer(float time);
    float _citadelPiecePulse(float t);

    float m_time = 0.0f;
    ImVec4 m_clearColor = ImVec4(0.0,0.0,0.0,0.0);

    GLuint m_shader = 0;
    wolf::Material* m_pMat = nullptr;
    wolf::Program* m_pProgram = nullptr;
    Grid3D* m_pGrid = nullptr;
    OrbitCamera* m_pOrbitCam = nullptr;
    FirstPersonCamera* m_pCamera = nullptr;

    wolf::Model* m_pCitadel = nullptr;
    wolf::Model* m_pCitadelPiece = nullptr;
    Plane *m_pPlane = nullptr;

    irrklang::ISoundEngine* m_pAudioEngine = nullptr;
};

#endif // S_CITYSAMPLE_H
