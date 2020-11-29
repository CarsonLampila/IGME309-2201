#include "AppClass.h"
using namespace Simplex;

// Cell Class
class Cell {
public:
	// Define
	int x;
	int y;
	int state;
	MyRigidBody* body;
	Cell* parent;
	int distance;


	// Constructor
	Cell(int xPos, int yPos, int curState, MyRigidBody* curBody, Cell* curParent, int curDistance)
	{
		x = xPos;
		y = yPos;
		state = curState;
		body = curBody;
		parent = curParent;
		distance = curDistance;

		// Color cell
		ColorCell();
	}


	// Color cell based on state
	void ColorCell()
	{
		switch (state) {
		// Empty
		case 0:
			body->SetColorColliding(C_RED);
			break;

		// Unvisited
		case 1:
			body->SetColorColliding(C_GREEN);
			break;

		// Visited
		case 2:
			body->SetColorColliding(C_BLUE);
			break;

		// Start
		case 3:
			body->SetColorColliding(C_YELLOW);
			break;

		// Target
		case 4:
			body->SetColorColliding(C_PURPLE);
			break;
		}
	}
};

// Global Variables 
const int size = 15;

Cell* grid[size][size];
std::vector<Cell*> path;

Cell* start;
Cell* target;

MyRigidBody* seeker;


void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(15.0f, 30.0f, 30.0f), //Position
		vector3(15.0f, 0.0f, 15.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

	// Steve
	m_pEntityMngr->AddEntity("Minecraft\\Steve.obj", "Steve");
	matrix4 m4PositionMove = glm::translate(vector3(9.0f, 2.1f, 3.0f));
	m_pEntityMngr->SetModelMatrix(m4PositionMove * glm::scale(vector3(2.0f)));

	// Creeper
	m_pEntityMngr->AddEntity("Minecraft\\Creeper.obj", "Creeper");
	matrix4 m4PositionTarget = glm::translate(vector3(23.0f, 2.1f, 21.0f));
	m_pEntityMngr->SetModelMatrix(m4PositionTarget * glm::scale(vector3(2.0f)));
	seeker = m_pEntityMngr->GetRigidBody("Creeper");


	// 2d array for layout
	int floor[size][size] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1},
		{1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1},
		{1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
		{1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
		{1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 3, 1, 1, 1},
		{1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	};


	// Define starting coords
	float xCoors = 0.0f;
	float yCoors = 0.0f;
	float zCoors = 0.0f;


	// Loop to add elements to entity manager
	for (int i = 0; i < size * size; i++)
	{
		yCoors = 0.0f;

		// Translate coords to ints
		int xPos = ((int)zCoors) / 2;
		int yPos = ((int)xCoors) / 2;

		// If wall raise height
		if (floor[xPos][yPos] == 0)
		{
			yCoors += 2.0f;
		}


		// Generate block layout
		m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", "Cube_" + std::to_string(i));
		matrix4 m4Position = glm::translate(vector3(xCoors, yCoors, zCoors));
		m_pEntityMngr->SetModelMatrix(m4Position * glm::scale(vector3(2.0f)));

		// Generate grid
		MyRigidBody* body = m_pEntityMngr->GetRigidBody("Cube_" + std::to_string(i));
		Cell* gridElement = new Cell(xPos, yPos, floor[xPos][yPos], body, nullptr, 0);
		grid[xPos][yPos] = gridElement;


		// Set Start/End
		if (grid[xPos][yPos]->state == 3)
		{
			start = grid[xPos][yPos];
		}
		else if (grid[xPos][yPos]->state == 4)
		{
			target = grid[xPos][yPos];
		}



		// Move blocks + array
		if (xCoors == 28.0f)
		{
			xCoors = 0.0f;
			zCoors += 2.0f;
		}
		else
		{
			xCoors += 2.0f;
		}

	}

	// Path follow
	PathFollowing();

}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Set the model matrix for the main object
	//m_pEntityMngr->SetModelMatrix(m_m4Steve, "Steve");

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
	//m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//draw gui,
	DrawGUI();

	FollowPath(seeker);

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//Release MyEntityManager
	MyEntityManager::ReleaseInstance();

	//release GUI
	ShutdownGUI();
}



void Application::PathFollowing()
{
	// Define
	std::vector<Cell*> edge;
	std::vector<Cell*> closed;
	closed.push_back(start);

	Cell* cur = start;

	bool pathFound = false;


	// Unitl path is found
	while (!pathFound)
	{
		// Loop through rows and columns of grid
		for (int a = cur->x - 1; a <= cur->x + 1; a++)
		{
			for (int b = cur->y - 1; b <= cur->y + 1; b++) 
			{

				// Skips
				// Current
				if (a == cur->x && b == cur->y)
				{
					continue;
				}

				// Diagonals
				if ((a == cur->x - 1 && b == cur->y - 1) ||
					(a == cur->x - 1 && b == cur->y + 1) ||
					(a == cur->x + 1 && b == cur->y - 1) ||
					(a == cur->x + 1 && b == cur->y + 1))
				{
					continue;
				}

				// Out of bounds
				if (a < 0 || a > size - 1 || b < 0 || b > size - 1)
				{
					continue;
				}

				// Walls
				if (grid[a][b]->state == 0)
				{
					continue;
				}

				// Closed list
				for (Cell* check : closed)
				{
					if (check == grid[a][b])
					{
						continue;
					}
				}

				// If end found
				if (grid[a][b]->state == 4)
				{
					// Set parent
					grid[a][b]->parent = cur;

					// Add to closed 
					closed.push_back(cur);

					//Set current
					cur = grid[a][b];

					// Create path
					// Traverse backwards through  tree to find start
					path.insert(path.begin(), target);
					while (cur->parent != start)
					{
						// Add to start of list
						path.insert(path.begin(), cur->parent);
						// Move back
						cur = cur->parent;
					}
					path.insert(path.begin(), start);

					// A path was found
					pathFound = true;


					// ReColor
					for (Cell* order : path)
					{
						// Dont change color of start or stop
						if (order->state != 3 && order->state != 4)
						{
							order->state = 2;
							order->ColorCell();
						}
					}	
					break;
				}

				// Calc distances
				int xDis = grid[a][b]->x - cur->x;
				int yDis = grid[a][b]->y - cur->y;
				int moveDis = cur->distance + sqrt(pow(xDis, 2) + pow(yDis, 2));

				// If no parent or greater distance
				if (grid[a][b]->parent == nullptr || moveDis < grid[a][b]->distance)
				{
					// Set parent and distance
					grid[a][b]->parent = cur;
					grid[a][b]->distance = moveDis;

					// See if edges contatains element
					bool contain = false;
					for (Cell* check : edge)
					{
						if (check == grid[a][b])
						{
							contain = true;
						}
					}
					// If not add to edges
					if (!contain)
					{
						edge.push_back(grid[a][b]);
					}
				}
			}
		}

		// If elements exist within edges
		if (edge.size() != 0)
		{
			// Create default shortest
			int shortest = edge[0]->distance + ManhattenDistance(edge[0]->x, edge[0]->y, target->x, target->y);
			int shortestIndex = 0;

			// Find Shortest
			for (int i = 0; i < edge.size(); i++)
			{
				if (edge[i]->distance + ManhattenDistance(edge[i]->x, edge[i]->y, target->x, target->y) < shortest)
				{
					// Set shortest
					shortest = edge[i]->distance + ManhattenDistance(edge[i]->x, edge[i]->y, target->x, target->y);
					shortestIndex = i;
				}
			}

			// Set shortest as current
			cur = edge[shortestIndex];


			// Create new edges without current
			closed.push_back(cur);
			std::vector<Cell*> edgeNew;
			for (Cell* check : edge)
			{
				if (check != cur)
				{
					edgeNew.push_back(check);
				}
			}
			edge = edgeNew;
		}
		else 
		{
			// No path found
			pathFound = true;
		}
	}
}


int Application::ManhattenDistance(int moveX, int moveY, int targetX, int targetY)
{
	return abs(moveX - targetX) + abs(moveY - targetY);
}


void Application::FollowPath(MyRigidBody* body)
{
	//Get a timer
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer

	// Calculate the current position
	vector3 v3CurrentPos = seeker->GetCenterGlobal();
	v3CurrentPos.y = 2.1f;


	// Define position variables
	vector3 current;
	vector3 next;
	static uint stopNum = 0;
	int destination = stopNum + 1;



	// Define Current
	current = vector3(path[stopNum]->y * 2.0f + 1.0f, 2.1f, path[stopNum]->x * 2.0f + 1.0f);
	// Define Next as following stop and loop back to start
	if (destination >= path.size())
	{
		destination = 0;
	}
	next = vector3(path[destination]->y * 2.0f + 1.0f, 2.1f, path[destination]->x * 2.0f + 1.0f);

	// Define times
	float arriveTime;
	// Regular
	if (stopNum != path.size() - 1)
	{
		arriveTime = 0.5;
	}
	float timePercent = MapValue(fTimer, 0.0f, arriveTime, 0.0f, 1.0f);

	// Calculate the current pos 
	v3CurrentPos = glm::lerp(current, next, timePercent);

	// If arrived at next stop
	if (timePercent >= 1.0f)
	{
		// Alternate 
		stopNum++;
		// Restart time
		fTimer = m_pSystem->GetDeltaTime(uClock);
	}

	// If creeper is colliding
	if (body->GetCollidingCount() > 0)
	{
		// Reset stop
		stopNum = 0;
		// Move target
		NewTarget();
	}


	// Apply movement
	matrix4 m4PositionMove = glm::translate(v3CurrentPos);
	m_pEntityMngr->SetModelMatrix(m4PositionMove* glm::scale(vector3(2.0f)), "Creeper");
}


void Application::NewTarget()
{
	// Define
	int a;
	int b;
	bool viable = false;

	// Clean
	Clean();

	// Loop until not a wall
	while (!viable)
	{
		// Random Pos
		a = rand() % 15;
		b = rand() % 15;

		// If not a wall
		if (grid[a][b]->state != 0)
		{
			viable = true;
		}
	}

	// Redefine start and target
	start = target;
	start->state = 3;
	start->ColorCell();
	target = grid[a][b];
	target->state = 4;
	target->ColorCell();


	// Apply movement
	matrix4 m4PositionMove = glm::translate(vector3(grid[a][b]->y * 2.0f + 1.0f, 2.1f, grid[a][b]->x * 2.0f + 1.0f));
	m_pEntityMngr->SetModelMatrix(m4PositionMove * glm::scale(vector3(2.0f)), "Steve");


	// Restart
	PathFollowing();
}

void Application::Clean()
{
	// Remove start and target
	start->state = 1;
	target->state = 1;


	// Delete prev path
	for (Cell* order : path)
	{
		order->state = 1;
		order->ColorCell();
	}
	path.clear();

	// Reset grid
	for (int x = 0; x < size; x++)
	{
		for (int y = 0; y < size; y++)
		{
			grid[x][y]->parent = nullptr;
			grid[x][y]->distance = 0;
		}
	}
}