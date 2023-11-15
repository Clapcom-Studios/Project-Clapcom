#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "Log.h"
#include "External/MathGeoLib/include/Math/Quat.h"

#include "External/Optick/include/optick.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 2.0f, 8.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
	ViewMatrix = IdentityMatrix;

	CalculateViewMatrix();

	// Frustrum

	editorCamera = new CCamera(nullptr);
	editorCamera->SetPos(0.0f, 2.0f, 8.0f);

	LOG("Creating ModuleCamera3D");

}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	OPTICK_EVENT();

	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	float3 newPos(0,0,0);
	float speed = 20.0f * dt;

	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed *= 2;

	// Mouse wheel Zoom In and Zoom Out handling

	if (App->input->GetMouseZ() > 0) newPos -= Z * speed;
	if (App->input->GetMouseZ() < 0) newPos += Z * speed;

	// Mouse wheel pressed while dragging movement handling

	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT) {

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 1.6f * dt;

		float DeltaX = (float)dx * Sensitivity;
		float DeltaY = (float)dy * Sensitivity;

		newPos -= Y * speed * DeltaY;
		newPos += X * speed * DeltaX;

	}

	// Mouse motion ----------------

	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_IDLE)
	{
		//if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
		//if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.35f * dt;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			float3 rotationAxis(0.0f, 1.0f, 0.0f);
			Quat rotationQuat = Quat::RotateAxisAngle(rotationAxis, DeltaX);

			X = rotationQuat * X;
			Y = rotationQuat * Y;
			Z = rotationQuat * Z;
		}
		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Quat rotationQuat = Quat::RotateAxisAngle(X, DeltaY);

			Y = rotationQuat * Y;
			Z = rotationQuat * Z;

			if(Y.y < 0.0f)
			{
				Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = Z.Cross(X);
			}

		}

		Position = Reference + Z * Position.Length();
		
	}

	Position += newPos;
	Reference += newPos;

	if ((App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) && App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_IDLE) {

		// Center camera to 0,0,0 when pressing Left Alt

		Reference = float3(0.0f, 0.0f, 0.0f);
		LookAt(Reference);

	}
	else {

		// Orbital camera FPS when we aren't pressing Left Alt

		Reference = Position;

	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	Plane planes[6];
	editorCamera->GetFrustumPlanes(planes);
	editorCamera->DrawFrustumPlanes(planes);

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3&Position, const float3&Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Z.Cross(X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const float3&Spot)
{
	Reference = Spot;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Z.Cross(X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3&Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return ViewMatrix.M;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	//todo: USE MATHGEOLIB here BEFORE 1st delivery! (TIP: Use MathGeoLib/Geometry/Frustum.h, view and projection matrices are managed internally.)
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -(X.Dot(Position)), -(Y.Dot(Position)), -(Z.Dot(Position)), 1.0f);
}