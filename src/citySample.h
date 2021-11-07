#pragma once
#include "../thirdparty/irrKlang/include/irrKlang.h"
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"
#include "../samplefw/Grid3D.h"
#include "../samplefw/OrbitCamera.h"

class CitySample: public Sample
{
public:
    CitySample(wolf::App* pApp) : Sample(pApp,"Alpha Test") {}
    ~CitySample();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

private:
    wolf::Model* m_pCitadel = nullptr;
    wolf::Model* m_pCitadelPiece = nullptr;
    wolf::Material* m_pMat = nullptr;
    Grid3D* m_pGrid = nullptr;
    OrbitCamera* m_pOrbitCam = nullptr;
    irrklang::ISoundEngine* m_pAudioEngine = nullptr;
    float m_rot = 0.0f;
};
