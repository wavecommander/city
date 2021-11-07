#include <iostream>

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

void CitySample::update(float dt) 
{
    m_pOrbitCam->update(dt);
    m_pGrid->update(dt);
    m_pCitadelPiece->Update(dt);
    m_rot += dt;
}

void CitySample::render(int width, int height)
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 mProj = m_pOrbitCam->getProjMatrix(width, height);
	glm::mat4 mView = m_pOrbitCam->getViewMatrix();
  	glm::mat4 mWorld = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 100.0f, 5.0f));

    m_pGrid->render(mView, mProj);
    m_pCitadel->Render(mWorld, mView, mProj);

	glm::mat4 mWorldPiece = glm::translate(glm::mat4(1.0f), glm::vec3(m_rot, 100.0f, 5.0f));

    m_pCitadelPiece->Render(mWorldPiece, mView, mProj);
}
