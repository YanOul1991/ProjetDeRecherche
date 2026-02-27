/*
 * Physics.cpp
 */

#include "./Physics.h"

#include "Core/Graphics/Mesh.h"
#include "Core/Math/OptimMathematics.h"
#include "Core/Object/Camera/Camera.h"

struct RaycastHit {
  RaycastHit(const float3& _origin, const float3& _point, const float& _distance, UniquePtr<Mesh>* _mesh) :
      origin(_origin), point(_point), distance(_distance), mesh(_mesh) {
  }

  RaycastHit& operator=(const RaycastHit& other) {
    if (this != &other) {
      origin   = other.origin;
      point    = other.point;
      distance = other.distance;
      mesh     = other.mesh;
    }

    return *this;
  }

  float3 origin;   // The origin of the ray.
  float3 point;    // The point where the collision occured.
  float  distance; // Distance from origin to collision.

  // Reference to UniquePtr<Mesh> of the
  // Mesh object that was hit
  UniquePtr<Mesh>* mesh;
};

static UniquePtr<Mesh>* GetClosestRaycastHit(std::vector<RaycastHit>& hits) {
  RaycastHit closestHit = hits[0];

  for (auto& hit : hits) {
    if (hit.distance < closestHit.distance) {
      closestHit = hit;
    }
  }

  return closestHit.mesh;
}

Raycast Optim::Physics::ScreenToRaycast(float posX, float posY, float width, float height) {
  float ndcX = (2 * (posX) / static_cast<float>(width)) - 1.0f;
  float ndcY = 1.0f - (2 * (posY) / static_cast<float>(height));

  float4 nearPoint = { ndcX, ndcY, 0.0f, 1.0f };
  float4 farPoint  = { ndcX, ndcY, 1.0f, 1.0f };

  // Manually compute projectionMatrix.
  // Eventually, Camera class should have a function to output the result.

  const float     a      = (float)width / float(height); // aspect ratio ratio
  constexpr float fov    = mathConst::PI / 3.0f;         // Field of view
  constexpr float n      = 0.1f;                         // near clip
  constexpr float f      = 1000.0f;                      // far clip
  const float     yScale = 1.0f / (tan(fov / 2.0f));

  float4x4 perspectiveMatrix = float4x4{
    yScale / a, 0, 0, 0, 0, yScale, 0, 0, 0, 0, f / (n - f), -1, 0, 0, (n * f) / (n - f), 0
  };

  perspectiveMatrix = Optim::Mathematics::getMatrixTranspose(perspectiveMatrix);

  float4x4 viewMatrix = Camera::getViewMatrix().transpose();

  float4x4 viewProjectionInverse = Optim::Mathematics::getMatrixInverse(perspectiveMatrix * viewMatrix);

  float4 posNear = viewProjectionInverse * nearPoint;
  float4 posFar  = viewProjectionInverse * farPoint;

  float3 rayOrigin      = { posNear.x / posNear.w, posNear.y / posNear.w, posNear.z / posNear.w };
  float3 rayFarPosition = { posFar.x / posFar.w, posFar.y / posFar.w, posFar.z / posFar.w };

  float3 rayDirection = normalize(rayFarPosition - rayOrigin);

  return Raycast(rayOrigin, rayFarPosition, rayDirection);
}


UniquePtr<Mesh>* Optim::Physics::GetCollision(const Raycast& raycast, std::vector<UniquePtr<Mesh>>& meshList) {
  std::vector<RaycastHit> hits;

  for (auto& pMesh : meshList) {
    float4x4 worldTransform = pMesh->getWorldMatrix().transpose();

    // ***
    // this loop section could be put inside
    // a helper function for readability.

    for (int i = 0; i < (int)((float)pMesh->indexCount / 3); i++) {
      float tHit = 0;

      float3 O = float3{ 0, 0, 0 };
      float3 D = raycast.direction;

      float4 v0Local = Optim::Mathematics::getFloat4FromFloat3(pMesh->vertices[pMesh->indices[3 * i]].position);
      float4 v1Local = Optim::Mathematics::getFloat4FromFloat3(pMesh->vertices[pMesh->indices[3 * i + 1]].position);
      float4 v2Local = Optim::Mathematics::getFloat4FromFloat3(pMesh->vertices[pMesh->indices[3 * i + 2]].position);

      float3 v0 = Optim::Mathematics::getFloat3Part(worldTransform * v0Local);
      float3 v1 = Optim::Mathematics::getFloat3Part(worldTransform * v1Local);
      float3 v2 = Optim::Mathematics::getFloat3Part(worldTransform * v2Local);

      // Compute triangle edges
      float3 e1 = v1 - v0;
      float3 e2 = v2 - v0;

      constexpr float EPS = 1E-8F;

      float3 p   = cross(raycast.direction, e2); // Get Vector perpendicular to ray direction and second triangle edge
      float  det = dotProduct(e1, p);            // Get determinant to check if ray is parallel to triangle

      if (fabsf(det) < EPS) {
        // Ray is parralel to the triangle
        continue;
      }

      float invDet = 1.0f / det;

      float3 t = raycast.origin - v0; // Vector from triangle first vertex to ray origin

      // Compute barycentric coordinate u
      float u = dotProduct(t, p) * invDet;

      if (u < 0.0f || u > 1.0f) {
        continue;
      }

      float3 q = cross(t, e1);

      float v = dotProduct(raycast.direction, q) * invDet;
      if (v < 0.0f || u + v > 1.0f) {
        continue;
      }

      // Collision distance from camera
      tHit = dotProduct(e2, q) * invDet;

      if (tHit > EPS) {
        RaycastHit hit(raycast.origin, tHit * Camera::forward, tHit, &pMesh);
        hits.push_back(hit);
      }
    }
  }

  if (hits.size() > 0) {
    return GetClosestRaycastHit(hits);
  }

  return nullptr;
}
