#pragma once
#include <cassert>
#include <fstream>
#include "Math.h"
#include "DataTypes.h"

namespace dae
{
	namespace GeometryUtils
	{
#pragma region Sphere HitTest
		//SPHERE HIT-TESTS
		inline bool HitTest_Sphere(const Sphere& sphere, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{
			//todo W1

			const Vector3 rayDirection{ ray.direction };
			const Vector3 sphereToRay{ ray.origin - sphere.origin };
			const float sphereRadius{ sphere.radius };

			const float A{ Vector3::Dot(rayDirection,rayDirection) };
			const float B{ Vector3::Dot(2 * rayDirection,sphereToRay) };
			const float C{ Vector3::Dot(sphereToRay,sphereToRay) - (sphereRadius * sphereRadius) };

			// When no hits
			const float discriminant{ (B * B) - 4 * A * C };
			if (discriminant < 0)
			{
				if (!ignoreHitRecord)
				{
					hitRecord.didHit = false;
				}

				return false;
			}

			const float t0{ (-B + sqrtf(discriminant)) / (2 * A) };
			const float t1{ (-B - sqrtf(discriminant)) / (2 * A) };
			float useableT{ t1 };

			// Check if can later switch to t0, if it's not under rayMin
			bool canSwitch{true};
			if (t0 < ray.min)
			{
				canSwitch = false;
			}

			// When t1 is under rayMin, use other t if possible
			if (t1 < ray.min)
			{
				if (canSwitch)
				{
					useableT = t0;
				}
				else
				{
					if (!ignoreHitRecord)
					{
						hitRecord.didHit = false;
					}

					return false;
				}
				
			}

			// When over max, no hit
			if (useableT > ray.max)
			{
				if (!ignoreHitRecord)
				{
					hitRecord.didHit = false;
				}

				return false;
			}

			const Vector3 hitPoint{ ray.origin + useableT * ray.direction };
			Vector3 normal{ hitPoint - sphere.origin };
			normal.Normalize();

			// When hit
			if (!ignoreHitRecord)
			{
				hitRecord.origin = hitPoint;
				hitRecord.normal = normal;
				hitRecord.t = useableT;

				hitRecord.materialIndex = sphere.materialIndex;
				hitRecord.didHit = true;
			}

			return true;
		}

		inline bool HitTest_Sphere(const Sphere& sphere, const Ray& ray)
		{
			HitRecord temp{};
			return HitTest_Sphere(sphere, ray, temp, true);
		}
#pragma endregion
#pragma region Plane HitTest
		//PLANE HIT-TESTS
		inline bool HitTest_Plane(const Plane& plane, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{
			//todo W1

			const Vector3 rayToPlane{ plane.origin - ray.origin };
			const float distanceToPlane{ Vector3::Dot(rayToPlane,plane.normal) / Vector3::Dot(ray.direction,plane.normal) };
			
			const bool isInsideBoundaries{ ray.min < distanceToPlane && distanceToPlane < ray.max };
			if (!isInsideBoundaries)
			{
				if (!ignoreHitRecord)
				{
					hitRecord.didHit = false;
				}

				return false;
			}

			const Vector3 hitPoint{ ray.origin + distanceToPlane * ray.direction };
			if (!ignoreHitRecord)
			{
				hitRecord.origin = hitPoint;
				hitRecord.normal = plane.normal;
				hitRecord.t = distanceToPlane;

				hitRecord.materialIndex = plane.materialIndex;	
				hitRecord.didHit = true;
			}

			return true;
		}

		inline bool HitTest_Plane(const Plane& plane, const Ray& ray)
		{
			HitRecord temp{};
			return HitTest_Plane(plane, ray, temp, true);
		}
#pragma endregion
#pragma region Triangle HitTest
		//TRIANGLE HIT-TESTS
		inline bool HitTest_Triangle(const Triangle& triangle, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{
			//todo W5
			assert(false && "No Implemented Yet!");
			return false;
		}

		inline bool HitTest_Triangle(const Triangle& triangle, const Ray& ray)
		{
			HitRecord temp{};
			return HitTest_Triangle(triangle, ray, temp, true);
		}
#pragma endregion
#pragma region TriangeMesh HitTest
		inline bool HitTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
		{
			//todo W5
			assert(false && "No Implemented Yet!");
			return false;
		}

		inline bool HitTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray)
		{
			HitRecord temp{};
			return HitTest_TriangleMesh(mesh, ray, temp, true);
		}
#pragma endregion
	}

	namespace LightUtils
	{
		//Direction from target to light
		inline Vector3 GetDirectionToLight(const Light& light, const Vector3 origin)
		{
			//todo W3
			
			Vector3 originToLight{};
			// Directional Light doesn't have origin
			if (light.type == LightType::Directional)
			{
				originToLight = -light.direction;
				originToLight *= FLT_MAX;
			}
			else
			{
				originToLight = light.origin - origin;
			}

			return originToLight;
		}

		inline ColorRGB GetRadiance(const Light& light, const Vector3& target)
		{
			//todo W3
			assert(false && "No Implemented Yet!");
			return {};
		}
	}

	namespace Utils
	{
		//Just parses vertices and indices
#pragma warning(push)
#pragma warning(disable : 4505) //Warning unreferenced local function
		static bool ParseOBJ(const std::string& filename, std::vector<Vector3>& positions, std::vector<Vector3>& normals, std::vector<int>& indices)
		{
			std::ifstream file(filename);
			if (!file)
				return false;

			std::string sCommand;
			// start a while iteration ending when the end of file is reached (ios::eof)
			while (!file.eof())
			{
				//read the first word of the string, use the >> operator (istream::operator>>) 
				file >> sCommand;
				//use conditional statements to process the different commands	
				if (sCommand == "#")
				{
					// Ignore Comment
				}
				else if (sCommand == "v")
				{
					//Vertex
					float x, y, z;
					file >> x >> y >> z;
					positions.push_back({ x, y, z });
				}
				else if (sCommand == "f")
				{
					float i0, i1, i2;
					file >> i0 >> i1 >> i2;

					indices.push_back((int)i0 - 1);
					indices.push_back((int)i1 - 1);
					indices.push_back((int)i2 - 1);
				}
				//read till end of line and ignore all remaining chars
				file.ignore(1000, '\n');

				if (file.eof()) 
					break;
			}

			//Precompute normals
			for (uint64_t index = 0; index < indices.size(); index += 3)
			{
				uint32_t i0 = indices[index];
				uint32_t i1 = indices[index + 1];
				uint32_t i2 = indices[index + 2];

				Vector3 edgeV0V1 = positions[i1] - positions[i0];
				Vector3 edgeV0V2 = positions[i2] - positions[i0];
				Vector3 normal = Vector3::Cross(edgeV0V1, edgeV0V2);

				if(isnan(normal.x))
				{
					int k = 0;
				}

				normal.Normalize();
				if (isnan(normal.x))
				{
					int k = 0;
				}

				normals.push_back(normal);
			}

			return true;
		}
#pragma warning(pop)
	}
}