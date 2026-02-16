// IDx11Resource.h

#include "Core/Defines/DirectX/msDx11.h"
#include "Core/OptimEngine.h"

class IDx11Resource
{
 public:
  virtual ~IDx11Resource() = 0;
  virtual void bind(ID3D11DeviceContext* pDevice) = 0;
};