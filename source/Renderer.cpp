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

			// Calculate rasterSpace to cameraSpace
			cx = ((2 * pxc / m_Width) - 1) * aspectRatio;
			cy = float(1 - (2 * pyc / m_Height));

			// Calculate rayDirectionVector
			rayDirection = cx * camera.right + cy * camera.up + camera.forward;
			rayDirection.Normalize();

			// TESTS

			// TEST_RAY
			Ray testRay{ { 0,0,0 }, rayDirection };
			
			// ColorRGB finalColor{ rayDirection.x, rayDirection.y, rayDirection.z };
			 
			// TEST_SPHERE
			ColorRGB finalColor{};
			HitRecord closestHit{};
			//Sphere testSphere{ {0,0,100.f},50.f,0 };

			pScene->GetClosestHit(testRay, closestHit);
			//GeometryUtils::HitTest_Sphere(testSphere, testRay, closestHit);
			
			if (closestHit.didHit)
			{
				finalColor = materials[closestHit.materialIndex]->Shade();

				/* const float scaled_t = (closestHit.t - 50.f) / 40.f;
				 finalColor = { scaled_t,scaled_t,scaled_t };*/
			}


			float gradient = px / static_cast<float>(m_Width);
			gradient += py / static_cast<float>(m_Width);
			gradient /= 2.0f;

			//ColorRGB finalColor{ gradient, gradient, gradient };

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
