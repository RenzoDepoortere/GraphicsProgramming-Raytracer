//External includes
#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "Renderer.h"
#include "Math.h"
#include "Matrix.h"
#include "Material.h"
#include "Scene.h"
#include "Utils.h"

using namespace dae;

Renderer::Renderer(SDL_Window * pWindow) :
	m_pWindow(pWindow),
	m_pBuffer(SDL_GetWindowSurface(pWindow))
{
	//Initialize
	SDL_GetWindowSize(pWindow, &m_Width, &m_Height);
	m_pBufferPixels = static_cast<uint32_t*>(m_pBuffer->pixels);
}

void Renderer::Render(Scene* pScene) const
{
	Camera& camera = pScene->GetCamera();
	auto& materials = pScene->GetMaterials();
	auto& lights = pScene->GetLights();


	Vector3 rayDirection{};
	rayDirection.z = camera.forward.z;
	float cx{};
	float cy{};
	const float aspectRatio{ float(m_Width) / m_Height };

	for (int px{}; px < m_Width; ++px)
	{
		for (int py{}; py < m_Height; ++py)
		{
			const float pxc{ px + 0.5f };
			const float pyc{ py + 0.5f };

			const float fovAngle{ std::tanf(camera.fovAngle * TO_RADIANS / 2)};

			const Matrix cameraToWorld{ camera.CalculateCameraToWorld() };

			// Calculate rasterSpace to cameraSpace
			cx = ((2 * pxc / m_Width) - 1) * (aspectRatio * fovAngle);
			cy = float(1 - (2 * pyc / m_Height)) * fovAngle;

			// Calculate rayDirectionVector
			rayDirection = cx * Vector3::UnitX + cy * Vector3::UnitY + Vector3::UnitZ;
			rayDirection.Normalize();

			// Transform rayDirection
			rayDirection = cameraToWorld.TransformVector(rayDirection);

			// Show results with color
			const Ray viewRay{ camera.origin, rayDirection };
			ColorRGB finalColor{};
			HitRecord closestHit{};

			pScene->GetClosestHit(viewRay, closestHit);
			if (closestHit.didHit)
			{
				Ray hitToLight{};
				hitToLight.origin = closestHit.origin;

				Vector3 hitToLightDirection{};
				float hitToLightDirectionMagnitude{};

				bool isOccluded{ false };

				// Check if hitToLight is obstructed
				for (size_t idx{}; idx < lights.size(); idx++)
				{
					hitToLightDirection = LightUtils::GetDirectionToLight(lights[idx], closestHit.origin);
					hitToLightDirectionMagnitude = hitToLightDirection.Magnitude();

					hitToLight.direction = hitToLightDirection;
					hitToLight.direction.Normalize();

					hitToLight.max = hitToLightDirectionMagnitude;

					// If nothing obstructs, there is light and breaks loop
					const bool isInsideBoundaries{ viewRay.min < hitToLightDirectionMagnitude
													&& hitToLightDirectionMagnitude < viewRay.max };

					// If inside boundaries and hits something, light = obstructed
					if (isInsideBoundaries)
					{
						isOccluded = pScene->DoesHit(hitToLight);
					}
				}

				// Darken when isOccluded, otherwise normal color
				finalColor = materials[closestHit.materialIndex]->Shade();
				if (isOccluded)
				{
					finalColor *= 0.5f;
				}
			}

			//Update Color in Buffer
			finalColor.MaxToOne();

			m_pBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBuffer->format,
				static_cast<uint8_t>(finalColor.r * 255),
				static_cast<uint8_t>(finalColor.g * 255),
				static_cast<uint8_t>(finalColor.b * 255));
		}
	}

	//@END
	//Update SDL Surface
	SDL_UpdateWindowSurface(m_pWindow);
}

bool Renderer::SaveBufferToImage() const
{
	return SDL_SaveBMP(m_pBuffer, "RayTracing_Buffer.bmp");
}
