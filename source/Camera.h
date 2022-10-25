#pragma once
#include <cassert>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>

#include "Math.h"
#include "Timer.h"

#include <iostream>

namespace dae
{
	struct Camera
	{
		Camera() = default;

		Camera(const Vector3& _origin, float _fovAngle):
			origin{_origin},
			fovAngle{_fovAngle}
		{
		}


		Vector3 origin{};
		float fovAngle{ 90.f };

		Vector3 forward{Vector3::UnitZ};
		Vector3 up{Vector3::UnitY};
		Vector3 right{Vector3::UnitX};

		float totalPitch{0.f};
		float totalYaw{0.f};

		Matrix cameraToWorld{};

		Matrix CalculateCameraToWorld()
		{
			//todo: W2
			const Vector3 worldUp{ Vector3::UnitY };
			
			Vector3 rightVector{ Vector3::Cross(worldUp,forward)};
			rightVector.Normalize();

			Vector3 upVector{ Vector3::Cross(forward,rightVector) };
			upVector.Normalize();
			
			cameraToWorld[0] = { rightVector,	0 };
			cameraToWorld[1] = { upVector,		0 };
			cameraToWorld[2] = { forward,		0 };
			cameraToWorld[3] = { origin,		1 };

			return cameraToWorld;
		}

		void Update(Timer* pTimer)
		{
			const float deltaTime = pTimer->GetElapsed();

			//Keyboard Input
			const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);


			//Mouse Input
			int mouseX{}, mouseY{};
			const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

			//todo: W2
			FOVInput(pKeyboardState, deltaTime);
			MovementInput(pKeyboardState, deltaTime);

			MouseInput(mouseState, mouseX, mouseY, deltaTime);
		}

		void FOVInput(const uint8_t* pKeyboardState, float deltaTime)
		{
			// FOV Change
			const float angleSpeed{ 10.f };
			const float maxAngle{ 180 }, minAngle{ 0 };
			if (pKeyboardState[SDL_SCANCODE_UP])
			{
				// Ensurance to not overstretch
				if (fovAngle == maxAngle - 1)
				{
					return;
				}

				fovAngle += angleSpeed * deltaTime;
			}
			if (pKeyboardState[SDL_SCANCODE_DOWN])
			{
				// Ensurance to not overstretch
				if (fovAngle == minAngle + 1)
				{
					return;
				}

				fovAngle -= angleSpeed * deltaTime;
			}
		}
		void MovementInput(const uint8_t* pKeyboardState, float deltaTime)
		{
			const float movementSpeed{ 5.f };

			const Vector3 worldUp{ Vector3::UnitY };
			Vector3 rightVector{ Vector3::Cross(worldUp,forward) };
			rightVector.Normalize();

			// Z-axis
			if (pKeyboardState[SDL_SCANCODE_W])
			{
				origin += forward * movementSpeed * deltaTime;
			}
			if (pKeyboardState[SDL_SCANCODE_S])
			{
				origin -= forward * movementSpeed * deltaTime;
			}

			// Y-axis
			if (pKeyboardState[SDL_SCANCODE_Q])
			{
				origin -= up * movementSpeed * deltaTime;
			}
			if (pKeyboardState[SDL_SCANCODE_E])
			{
				origin += up * movementSpeed * deltaTime;
			}

			// X-axis
			if (pKeyboardState[SDL_SCANCODE_A])
			{
				origin -= rightVector * movementSpeed * deltaTime;
			}
			if (pKeyboardState[SDL_SCANCODE_D])
			{
				origin += rightVector * movementSpeed * deltaTime;
			}
		}

		void MouseInput(const uint32_t mouseState, int mouseX, int mouseY, float deltaTime)
		{
			const float movementSpeed{ 3.f };

			if (mouseState& SDL_BUTTON(SDL_BUTTON_RIGHT))
			{
				totalPitch -= mouseY * movementSpeed * deltaTime;
				totalYaw += mouseX * movementSpeed * deltaTime;

				const Matrix rotationMatrix{ Matrix::CreateRotation(totalPitch * TO_RADIANS,totalYaw * TO_RADIANS,0) };
				
				forward = rotationMatrix.TransformVector(Vector3::UnitZ);
				forward.Normalize();
			}
		}
	};

	
}
