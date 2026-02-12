#include "Core/Graphics/Mesh.h"

Mesh::~Mesh() noexcept { }

float4x4 Mesh::getWorldMatrix() const
{
	float4x4 l_translation	= Optim::Mathematics::getMatrixTranslation(position);
	float4x4 l_rotation		= Optim::Mathematics::getMatrixFromQuaternion(rotation);
	return l_rotation * l_translation;
}

__OPTIM_INTERNAL_REGISTER_OBJECT(Mesh, Object)

OPTIM_DECLARE_PROPERTY(Mesh, position)
OPTIM_DECLARE_PROPERTY(Mesh, rotation)
OPTIM_DECLARE_PROPERTY(Mesh, scaleField)

__OPTIM_INTERNAL_REGISTER_OBJECT(SkinnedMesh, Mesh)
OPTIM_DECLARE_PROPERTY(SkinnedMesh, skinnedMeshValue)
OPTIM_DECLARE_PROPERTY(SkinnedMesh, scale)
OPTIM_DECLARE_PROPERTY(SkinnedMesh, listOfStuff)