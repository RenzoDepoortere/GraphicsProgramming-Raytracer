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
			
			// Sphere-origin to ray-origin
			const Vector3 tc{ sphere.origin - ray.origin };							
			
			// Calculate distances
			const float TP{ Vector3::Dot(tc , ray.direction) };
			const float CP{ sqrtf(tc.SqrMagnitude() - powf(TP,2.f)) };

			if (CP > sphere.radius)
			{
				if (!ignoreHitRecord)
				{
					hitRecord.origin = ray.origin;
					hitRecord.normal = ray.direction;
					hitRecord.t = FLT_MAX;

					hitRecord.materialIndex = 0;
					hitRecord.didHit = false;
				}

				return false;
			}

			// Calculate distance betw P and IntersectionPoint
			const float IP{ sqrtf(powf(sphere.radius,2.f) - powf(CP,2.f)) };
			
			// Distance from T to IntersectionPoint
			const float IT{ TP - IP };	

			const Vector3 interSectionPoint{ ray.origin + IT * ray.direction };

			if (!ignoreHitRecord)
			{
				hitRecord.origin = ray.origin;
				hitRecord.normal = ray.direction;
				hitRecord.t = IT;

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
			
			const Vector3 unitX{ Vector3::UnitX };
			const Vector3 unitY{ Vector3::UnitY };
			Vector3 planeDirection{};

			const bool equalX{ plane.normal.x == unitX.x || plane.normal.x == -unitX.x };
			if (equalX)
			{
				planeDirection = Vector3::Cross(unitY, plane.normal);
			}
			else
			{
				planeDirection = Vector3::Cross(unitX, plane.normal);
			}
				
			const Vector3 crossCheck{ Vector3::Cross(planeDirection,ray.direction) };
			if (crossCheck.Magnitude() == 0)
			{
				if (!ignoreHitRecord)
				{
					hitRecord.origin = ray.origin;
					hitRecord.normal = ray.direction;
					hitRecord.t = FLT_MAX;

					hitRecord.materialIndex = 0;
					hitRecord.didHit = false;
				}

				return false;
			}

			/*Vector3 intersectingPoint{};
			intersectingPoint.x = plane.origin.x * ray.direction.x / ray.origin.x;
			intersectingPoint.y = plane.origin.y * ray.direction.y / ray.origin.y;
			intersectingPoint.z = plane.origin.z * ray.direction.z / ray.origin.z;

			const Vector3 rayHitPiece{ intersectingPoint - ray.origin };*/
			const float distanceToPlane{ (Vector3::Dot(plane.normal,plane.origin) - Vector3::Dot(plane.normal,ray.origin))
										/ Vector3::Dot(plane.normal,ray.direction) };
			
			if (!ignoreHitRecord)
			{
				hitRecord.origin = ray.origin;
				hitRecord.normal = ray.direction;
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
			assert(false && "No Implemented Yet!");
			return {};
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