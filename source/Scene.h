#pragma once
#include <string>
#include <vector>

#include "Math.h"
#include "DataTypes.h"
#include "Camera.h"

namespace dae
{
	//Forward Declarations
	class Timer;
	class Material;
	struct Plane;
	struct Sphere;
	struct Light;

	//Scene Base Class
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		Scene(const Scene&) = delete;
		Scene(Scene&&) noexcept = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) noexcept = delete;

		virtual void Initialize() = 0;
		virtual void Update(dae::Timer* pTimer)
		{
			m_Camera.Update(pTimer);
		}

		Camera& GetCamera() { return m_Camera; }
		void GetClosestHit(const Ray& ray, HitRecord& closestHit) const;
		bool DoesHit(const Ray& ray) const;

		const std::vector<Plane>& GetPlaneGeometries() const { return m_PlaneGeometries; }
		const std::vector<Sphere>& GetSphereGeometries() const { return m_SphereGeometries; }
		const std::vector<Light>& GetLights() const { return m_Lights; }
		const std::vector<Material*> GetMaterials() const { return m_Materials; }

	protected:
		std::string	sceneName;

		std::vector<Plane> m_PlaneGeometries{};
		std::vector<Sphere> m_SphereGeometries{};
		std::vector<TriangleMesh> m_TriangleMeshGeometries{};
		std::vector<Light> m_Lights{};
		std::vector<Material*> m_Materials{};

		// Temp (Individual Triangle Testing)
		// std::vector<Triangle> m_Triangles{};

		Camera m_Camera{};

		Sphere* AddSphere(const Vector3& origin, float radius, unsigned char materialIndex = 0);
		Plane* AddPlane(const Vector3& origin, const Vector3& normal, unsigned char materialIndex = 0);
		TriangleMesh* AddTriangleMesh(TriangleCullMode cullMode, unsigned char materialIndex = 0);

		Light* AddPointLight(const Vector3& origin, float intensity, const ColorRGB& color);
		Light* AddDirectionalLight(const Vector3& direction, float intensity, const ColorRGB& color);
		unsigned char AddMaterial(Material* pMaterial);
	};

	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 1 Scene
	class Scene_W1 final : public Scene
	{
	public:
		Scene_W1() = default;
		~Scene_W1() override = default;

		Scene_W1(const Scene_W1&) = delete;
		Scene_W1(Scene_W1&&) noexcept = delete;
		Scene_W1& operator=(const Scene_W1&) = delete;
		Scene_W1& operator=(Scene_W1&&) noexcept = delete;

		void Initialize() override;
	};

	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 2 Scene
	class Scene_W2 final : public Scene
	{
	public:
		Scene_W2() = default;
		~Scene_W2() override = default;

		Scene_W2(const Scene_W2&) = delete;
		Scene_W2(Scene_W2&&) noexcept = delete;
		Scene_W2& operator=(const Scene_W2&) = delete;
		Scene_W2& operator=(Scene_W2&&) noexcept = delete;

		void Initialize() override;
	};

	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 3 Test Scene
	class Scene_W3_TestScene final : public Scene
	{
	public:
		Scene_W3_TestScene() = default;
		~Scene_W3_TestScene() override = default;

		Scene_W3_TestScene(const Scene_W3_TestScene&) = delete;
		Scene_W3_TestScene(Scene_W3_TestScene&&) noexcept = delete;
		Scene_W3_TestScene& operator=(const Scene_W3_TestScene&) = delete;
		Scene_W3_TestScene& operator=(Scene_W3_TestScene&&) noexcept = delete;

		void Initialize() override;
	};

	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 3 Scene
	class Scene_W3 final : public Scene
	{
	public:
		Scene_W3() = default;
		~Scene_W3() override = default;

		Scene_W3(const Scene_W3&) = delete;
		Scene_W3(Scene_W3&&) noexcept = delete;
		Scene_W3& operator=(const Scene_W3&) = delete;
		Scene_W3& operator=(Scene_W3&&) noexcept = delete;

		void Initialize() override;
	};

	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 4 Test Scene
	class TestScene_W4 final : public Scene
	{
	public:
		TestScene_W4() = default;
		~TestScene_W4() override = default;

		TestScene_W4(const TestScene_W4&) = delete;
		TestScene_W4(TestScene_W4&&) noexcept = delete;
		TestScene_W4& operator=(const TestScene_W4&) = delete;
		TestScene_W4& operator=(TestScene_W4&&) noexcept = delete;

		void Initialize() override;
		void Update(Timer* pTimer) override;

	private:
		TriangleMesh* pMesh{ nullptr };
	};

	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 4 Reference Scene
	class ReferenceScene_W4 final : public Scene
	{
	public:
		ReferenceScene_W4() = default;
		~ReferenceScene_W4() override = default;

		ReferenceScene_W4(const ReferenceScene_W4&) = delete;
		ReferenceScene_W4(ReferenceScene_W4&&) noexcept = delete;
		ReferenceScene_W4& operator=(const ReferenceScene_W4&) = delete;
		ReferenceScene_W4& operator=(ReferenceScene_W4&&) noexcept = delete;

		void Initialize() override;
		void Update(Timer* pTimer) override;

	private:
		TriangleMesh* m_Meshes[3]{nullptr};
	};

	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 4 Bunny Scene
	class BunnyScene_W4 final : public Scene
	{
	public:
		BunnyScene_W4() = default;
		~BunnyScene_W4() override = default;

		BunnyScene_W4(const BunnyScene_W4&) = delete;
		BunnyScene_W4(BunnyScene_W4&&) noexcept = delete;
		BunnyScene_W4& operator=(const BunnyScene_W4&) = delete;
		BunnyScene_W4& operator=(BunnyScene_W4&&) noexcept = delete;

		void Initialize() override;
		void Update(Timer* pTimer) override;

	private:
		TriangleMesh* pMesh{ nullptr };
	};
}
