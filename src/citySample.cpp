#include <iostream>
#include <math.h>

#include "../thirdparty/irrKlang/include/irrKlang.h"

#include "citySample.h"

CitySample::~CitySample()
{
	printf("Destroying Model Import Sample\n");
    wolf::MaterialManager::DestroyMaterial(m_pMat);
    delete m_pCitadel;
    delete m_pGrid;
    delete m_pOrbitCam;
    m_pAudioEngine->drop();
}

void CitySample::init()
{
	// Only init if not already done
    if(!m_pCitadel)
    {
        m_pCitadel = new wolf::Model("data/citadel_trimmed.fbx");
        m_pCitadelPiece = new wolf::Model("data/citadel_piece.fbx");

        glm::vec3 min = m_pCitadel->getAABBMin();
        glm::vec3 max = m_pCitadel->getAABBMax();
        glm::vec3 center = m_pCitadel->getAABBCenter();

        m_pOrbitCam = new OrbitCamera(m_pApp);
        m_pOrbitCam->focusOn(min,max);

        std::cout << "MIN:X" << min.x << ",Y" << min.y << ",Z" << min.z << 
                   ";MAX:X" << max.x << ",Y" << max.y << ",Z" << max.z <<  '\n';

        float gridSize = 2.5f * wolf::max(max.x,max.z);
        m_pGrid = new Grid3D(10, gridSize / 10.0f);
        m_pGrid->hideAxes();

        m_pAudioEngine = irrklang::createIrrKlangDevice();

	    if (!m_pAudioEngine)
	    {
	    	printf("Could not startup Audio Engine\n");
	    }

        // Citadel piece should hammer down every 6.494s
	    m_pAudioEngine->play2D("data/citadel_alert_cut.ogg", true);
    }

    printf("Successfully initialized Model Import Sample\n");
}

const float TOTAL_PERIOD = 6.494;
const float DEFAULT_VALUE = 23500.0f, TRANSLATION = -7500.0f, PULSE_WIDTH = 6.0f, SLAM_FRACTION = 0.1f;

float pulse(float t) {
    if (t < SLAM_FRACTION * PULSE_WIDTH) 
        return DEFAULT_VALUE + TRANSLATION * t / (SLAM_FRACTION * PULSE_WIDTH);
    return DEFAULT_VALUE + TRANSLATION * (PULSE_WIDTH - t) / ((1.0 - SLAM_FRACTION) * PULSE_WIDTH);
}

float thing(float time) {
    int t = fmod(time, TOTAL_PERIOD);
    if (t > (TOTAL_PERIOD - PULSE_WIDTH))
        return pulse(t - TOTAL_PERIOD + PULSE_WIDTH);
    return 0;
}

void CitySample::update(float dt) 
{
    m_pOrbitCam->update(dt);
    m_pGrid->update(dt);
    m_pCitadelPiece->Update(dt);
    m_rot += dt;
    m_hammer += dt;
}

void CitySample::render(int width, int height)
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 mProj = m_pOrbitCam->getProjMatrix(width, height);
	glm::mat4 mView = m_pOrbitCam->getViewMatrix();
  	glm::mat4 mWorld = glm::rotate(glm::mat4(1.0f), -PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    mWorld = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 100.0f, 5.0f)) * mWorld;

    m_pGrid->render(mView, mProj);
    m_pCitadel->Render(mWorld, mView, mProj);

	glm::mat4 mWorldPiece = glm::rotate(glm::mat4(1.0f), -PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    mWorldPiece = glm::rotate(glm::mat4(1.0f), PI/2.5f, glm::vec3(0.0f, 1.0f, 0.0f)) * mWorldPiece;
	mWorldPiece = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, thing(m_hammer), -1000.0f)) * mWorldPiece; // Start Y = 23500.0f, End Y = 16000.0f

    m_pCitadelPiece->Render(mWorldPiece, mView, mProj);
}
