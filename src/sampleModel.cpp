#include "../thirdparty/irrKlang/include/irrKlang.h"

#include "sampleModel.h"

SampleModel::~SampleModel()
{
	printf("Destroying Model Import Sample\n");
    wolf::MaterialManager::DestroyMaterial(m_pMat);
    delete m_pBuilding;
    delete m_pGrid;
    delete m_pOrbitCam;
    m_pEngine->drop();
}

void SampleModel::init()
{
	// Only init if not already done
    if(!m_pBuilding)
    {
        m_pBuilding = new wolf::Model("data/citadel_trimmed.fbx");
        m_pBuildingPiece = new wolf::Model("data/citadel_piece.fbx");

        glm::vec3 min = m_pBuilding->getAABBMin();
        glm::vec3 max = m_pBuilding->getAABBMax();
        glm::vec3 center = m_pBuilding->getAABBCenter();

        m_pOrbitCam = new OrbitCamera(m_pApp);
        m_pOrbitCam->focusOn(min,max);

        float gridSize = 2.5f * wolf::max(max.x,max.z);
        m_pGrid = new Grid3D(10, gridSize / 10.0f);
        m_pGrid->hideAxes();

        m_pEngine = irrklang::createIrrKlangDevice();

	    if (!m_pEngine)
	    {
	    	printf("Could not startup m_pEngine\n");
	    }

	    m_pEngine->play2D("data/citadel_alert.ogg", true);
    }

    printf("Successfully initialized Model Import Sample\n");
}

void SampleModel::update(float dt) 
{
    m_pOrbitCam->update(dt);
    m_pGrid->update(dt);
    m_pBuildingPiece->Update(dt);
}

void SampleModel::render(int width, int height)
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 mProj = m_pOrbitCam->getProjMatrix(width, height);
	glm::mat4 mView = m_pOrbitCam->getViewMatrix();
	glm::mat4 mWorld = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 100.0f, 5.0f));

    m_pGrid->render(mView, mProj);
    m_pBuilding->Render(mWorld, mView, mProj);

	glm::mat4 mWorldPiece = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 100.0f, 5.0f));

    m_pBuildingPiece->Render(mWorldPiece, mView, mProj);
}
