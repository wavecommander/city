#ifndef S_CITYSAMPLE_H
#define S_CITYSAMPLE_H

#include <irrKlang.h>
#include <imgui.h>
#include "../samplefw/Sample.h"
#include "../wolf/wolf.h"

#include "FirstPersonCamera.h"
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
    void _initImGui();
    void _renderImGui();
    float _calculateCitadelPieceHammer();
    float _citadelPiecePulse(float t);
    void _handle_keys();

    float m_time = 0.0f;
    ImVec4 m_clearColor = ImVec4(0.0, 0.0, 0.0, 0.0);
    bool m_showDemoWindow = false;

    GLuint m_shader = 0;
    wolf::Material *m_pMat = nullptr;
    wolf::Program *m_pProgram = nullptr;
    FirstPersonCamera *m_pCamera = nullptr;

    wolf::Model *m_pCitadel = nullptr;
    wolf::Model *m_pCitadelPiece = nullptr;

    irrklang::ISoundEngine *m_pAudioEngine = nullptr;

    static constexpr size_t NUM_KEYS = 256;
    bool m_pressedKeys[NUM_KEYS];
    bool m_aKeyIsPressed = false;
    bool m_renderDebugUI = false;

};

#endif // S_CITYSAMPLE_H
