#include "AppClass.h"

void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Mitch Steffens - mds4685@rit.edu";
	
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUpward(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}
	
	//if there are no segments create 7
	if(m_uOrbits < 1)
		m_uOrbits = 7;

	float fSize = 1.0f; //initial size of orbits

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/

	uint uSides = 3; //start with the minimal 3 sides
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager

		std::vector<vector3> stopList;
		float angle = 2.0f * PI / i;  //determine angle between each point
		for (uint p = 0; p <= i; p++) {
			float currentAngle = angle * p;
			vector3 currentPoint = vector3(cosf(currentAngle) * fSize, sinf(currentAngle) * fSize, 0); //determine position of the point in world space
			stopList.push_back(currentPoint); //add the points of this torus to the current vector
		}
		stopLists.push_back(stopList); //add the vector for this torus to the vector of vectors

		fSize += 0.5f; //increment the size for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength
	}


	pointCounter = new int[m_uOrbits];
	for (uint i = 0; i < m_uOrbits; i++) {
		pointCounter[i] = 0;
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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system
	/*
		The following offset will orient the orbits as in the demo, start without it to make your life easier.
	*/
	m4Offset = glm::rotate(IDENTITY_M4, 1.5708f, AXIS_Z);

	// draw a shapes
	static float fTimer = 0;
	static uint uClock = m_pSystem->GenClock();
	fTimer += m_pSystem->GetDeltaTime(uClock);
	float fTotalAnimationTime = 3.0f;
	float fPercentage;

	for (uint i = 0; i < m_uOrbits; ++i)
	{
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 1.5708f, AXIS_X));

		//calculate the current position
		vector3 v3CurrentPos = ZERO_V3;

		fPercentage = fTimer - (fTotalAnimationTime * pointCounter[i]) / fTotalAnimationTime;

		// increment the counter for this circle
		if (fPercentage >= 1.0f) {
			pointCounter[i]++;
		}

		std::vector<vector3> currentList = stopLists[i];
		int currentCount = pointCounter[i];

		//if (pointCounter[i] < currentList.size() - 1) {
		//	v3CurrentPos = glm::lerp(currentList[currentCount], currentList[currentCount + 1], fPercentage);
		//}
		//else {
		//	v3CurrentPos = glm::lerp(currentList[currentCount % (i+3)], currentList[currentCount % (i + 3) + 1], fPercentage)/*currentList[currentList.size() - 1]*/;
		//}

		v3CurrentPos = glm::lerp(currentList[currentCount % (i + 3)], currentList[currentCount % (i + 3) + 1], fPercentage)/*currentList[currentList.size() - 1]*/;


		matrix4 m4Model = glm::translate(m4Offset, v3CurrentPos);

		//draw spheres
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.1)), C_WHITE);
	}

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
	delete[] pointCounter;
	//release GUI
	ShutdownGUI();
}