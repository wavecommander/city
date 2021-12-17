#ifndef S_CITYSAMPLE_H
#define S_CITYSAMPLE_H

#include <imgui.h>
#include <irrKlang.h>
#include "../samplefw/Sample.h"
#include "../wolf/wolf.h"

#include "FirstPersonCamera.h"
#include "Grid.h"
#include "Skybox.h"
#include "types.h"

class CitySample : public Sample
{
public:
    CitySample(wolf::App *pApp);
    ~CitySample();

    void init() override;
    void update(float dt) override;
    void render(int width, int height) override;

    void goToSleep() override;

private:
    void _initAudio();
    void _handle_keys();
    void _renderImGui();
    float _calculateCitadelPieceHammer() const;
    float _citadelPiecePulse(float t) const;
    void _setCitadelToDefaultValues();

    static int gridLength;
    static int gridWidth;

    float m_time = 0.0f;
    ImVec4 m_clearColor = ImVec4(0.3529f, 0.8627f, 1.0f, 1.0f);;
    bool m_showDemoWindow = false;

    FirstPersonCamera *m_pCamera = nullptr;

    Grid *m_pGrid = nullptr;

    Plane *m_pGrass = nullptr;

    Skybox *m_pSkybox = nullptr;

    wolf::Model *m_pCitadel = nullptr;
    wolf::Model *m_pCitadelPiece = nullptr;

    // wolf::Model *m_pHouse = nullptr;
    // wolf::Model *m_pHouse1 = nullptr;
    // wolf::Model *m_pOffice = nullptr;
    // wolf::Model *m_pFactory = nullptr;
    // wolf::Model *m_pSkyscraper = nullptr;

    static float m_citadelAlarmVolume;

    float m_citadelScale;
    float m_citadelTotalPeriod;
    float m_citadelPhaseShift;
    float m_citadelStartingYValue;
    float m_citadelTranslation;
    float m_citadelPulseWidth;
    float m_citadelSlamFraction;

    irrklang::ISoundEngine *m_pAudioEngine = nullptr;

    static constexpr size_t NUM_KEYS = 256;
    bool m_pressedKeys[NUM_KEYS];
    bool m_aKeyIsPressed = false;
    bool m_renderDebugUI = false;
};

#endif // S_CITYSAMPLE_H
