#include "AppClass.h"
void Application::InitVariables(void)
{
	//init the mesh


	// Make 46 Cubes and add to list
	for (int i = 0; i < totalParts; i++)
	{
		MyMesh* m_pMesh = new MyMesh();
		m_pMesh->GenerateCube(1.0f, C_BLACK);
		m_pMeshList.push_back(m_pMesh);
	}

}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// Variables
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	int i = 0;
	vector3 currentPos(-4.0f, 6.0f, 0.0f);
	static float translateX = 0.0f;
	static float translateY = 0.0f;
	static float intervalX = .01;
	static float intervalY = .01;
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(translateX, translateY, 0.0f));

	// Translate
	if (translateX > 10.0f || translateX < -8.0f)
	{
		intervalX = -intervalX;
	}
	if (translateY > 4.0f || translateY < -4.0f)
	{
		intervalY = -intervalY;
	}

	translateX += intervalX;
	translateY += intervalY;

	// Draw
	// Top Left (-6, 6)
	// Bottom Right (4, -1)

	// Row 1 (00100000100) (2)
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Translate, currentPos));
	i++;
	currentPos.x += 6.0f;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Translate, currentPos));
	i++;

	// Row 2 (00010001000) (2)
	currentPos.x = -3.0f;
	currentPos.y--;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Translate, currentPos));
	i++;
	currentPos.x += 4.0f;
	m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Translate, currentPos));
	i++;

	// Row 3 (00111111100) (7)
	currentPos.x = -4.0f;
	currentPos.y--;
	for (int j = 0; j < 7; j++)
	{
		m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Translate, currentPos));
		i++;
		currentPos.x++;
	}

	// Row 4 (01101110110) (7)
	currentPos.x = -5.0f;
	currentPos.y--;
	for (int j = 0; j < 7;)
	{
		if (currentPos.x != -3.0f && currentPos.x != 1.0f)
		{
			m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Translate, currentPos));
			i++;
			j++;
		}
		currentPos.x++;
	}

	// Row 5 (11111111111) (11)
	currentPos.x = -6.0f;
	currentPos.y--;
	for (int j = 0; j < 11; j++)
	{
		m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Translate, currentPos));
		i++;
		currentPos.x++;
	}

	// Row 6 (10111111101) (9)
	currentPos.x = -6.0f;
	currentPos.y--;
	for (int j = 0; j < 9;)
	{
		if (currentPos.x != -5.0f && currentPos.x != 3.0f)
		{
			m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Translate, currentPos));
			i++;
			j++;
		}
		currentPos.x++;
	}

	// Row 7 (10100000101) (4)
	currentPos.x = -6.0f;
	currentPos.y--;
	for (int j = 0; j < 4;)
	{
		if (currentPos.x == -6.0f || currentPos.x == -4.0f || currentPos.x == 2.0f || currentPos.x == 4.0f)
		{
			m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Translate, currentPos));
			i++;
			j++;
		}
		currentPos.x++;
	}

	// Row 8 (00011011000) (4)
	currentPos.x = -3.0f;
	currentPos.y--;
	for (int j = 0; j < 4;)
	{
		if (currentPos.x == -3.0f || currentPos.x == -2.0f || currentPos.x == 0.0f || currentPos.x == 1.0f)
		{
			m_pMeshList[i]->Render(m4Projection, m4View, glm::translate(m4Translate, currentPos));
			i++;
			j++;
		}
		currentPos.x++;
	}

	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	for (int i = 0; i < m_pMeshList.size(); i++)
	{
		SafeDelete(m_pMeshList[i]);
	}

	//release GUI
	ShutdownGUI();
}