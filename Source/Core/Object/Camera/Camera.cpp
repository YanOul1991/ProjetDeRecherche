#include "Core/Object/Camera/Camera.h"

float Camera::posX	{0.0f};
float Camera::posY	{0.0f};
float Camera::posZ	{0.0f};

float Camera::pitch	{0.0f};
float Camera::yaw		{0.0f};
float Camera::roll	{0.0f};

float3 Camera::Forward	{0.0f, 0.0f, 0.0f};
float3 Camera::right		{0.0f, 0.0f, 0.0f};
float3 Camera::up				{0.0f, 0.0f, 0.0f};
