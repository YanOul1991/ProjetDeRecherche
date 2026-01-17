/* ======================================================================================
 *  IDirectX11Resource.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Utilities/Random/Random.h"

/*
 * @brief
 * Base class for all DirectX11 managed resources types.
 * 
 * @brief
 * >>>> This class should not be instanciated by itself.
 * Only derived classes should be instanciated <<<<
*/
class IDirectX11Resource
{
public:
  inline IDirectX11Resource() {
    guid = Optim::Random::getGetGuid();
    printf("New DirectX11Resource created with GUID : ");
    guid.sPrint();
    printf("\n");
  };

  virtual ~IDirectX11Resource(){}

  /*
   * @brief
   * Initialize and allocates resources on the GPU.
   * 
   * @param pDevice
   * A pointer to an existing ID3D11Device.
  */
  virtual void init(ID3D11Device* pDevice) = 0;

  /*
   * @brief
   * Binds the resources on the graphics pipeline so
   * it would be used for rendering.
   * 
   * @param pContext
   * A pointer to an existing ID3D11DeviceContext.
  */
  virtual void bind(ID3D11DeviceContext* pContext) = 0;


  /*
   * @brief Get the guid of the IDirectX11Resource object;
   * 
   * @return 
   * SGuid : the guid of the object
  */
  inline SGuid getId() const {
    return guid;
  }

protected:
  SGuid guid{};
};