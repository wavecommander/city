#ifndef S_CITYSAMPLE_H
#define S_CITYSAMPLE_H

#include "../thirdparty/irrKlang/include/irrKlang.h"
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"
#include "../samplefw/Grid3D.h"
#include "../samplefw/OrbitCamera.h"
#include "plane.h"

class CitySample: public Sample
{
public:
    CitySample(wolf::App* pApp) : Sample(pApp, "Cityscape") {}
    ~CitySample();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

    void pressedKey(char key) override;
    void goToSleep() override;

private:
    void _initAudio();
    void _initImGui();
    void _renderImGui();
    float _calculateHammer(float time);
    float _pulse(float t);

    bool m_renderDebugUI = false;

    float m_time = 0.0f;

    wolf::Model* m_pCitadel = nullptr;
    wolf::Model* m_pCitadelPiece = nullptr;
    wolf::Material* m_pMat = nullptr;
    Grid3D* m_pGrid = nullptr;
    OrbitCamera* m_pOrbitCam = nullptr;
    irrklang::ISoundEngine* m_pAudioEngine = nullptr;

    Plane *m_pCityGrid = nullptr;

    ImVec4 m_clearColor = ImVec4(0.0,0.0,0.0,0.0);
};

#endif // S_CITYSAMPLE_H
