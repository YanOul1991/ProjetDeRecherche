#include "../Dx11VertexBuffer.h"
#include "Dx11RHI.h"

Dx11VertexBuffer::~Dx11VertexBuffer() {
}

void Dx11VertexBuffer::create(ID3D11Device* pDevice, Vertex* pVertexBuffer, uint32 elementCount) {
  OPTIM_WIN_COM_CHECK_START();

  std::vector<decltype(Vertex::position)> bufferPositions;
  std::vector<decltype(Vertex::uvCoord)>  bufferuv;
  std::vector<decltype(Vertex::normal)>   bufferNormal;

  for (uint64 i = 0; i < elementCount; i++) {
    bufferPositions.push_back(pVertexBuffer[i].position);
    bufferuv.push_back(pVertexBuffer[i].uvCoord);
    bufferNormal.push_back(pVertexBuffer[i].normal);
  }

  bufferElementCount = elementCount;
  offset             = 0;

  // DELCARE STRIDES
  stride             = sizeof(decltype(Vertex::position)); // position stride
  uvStreamStride     = sizeof(decltype(Vertex::uvCoord));  // uv coord sride
  normalStreamStride = sizeof(decltype(Vertex::normal));   // normal stride

  // BUILD POSITION STREAM
  D3D11_BUFFER_DESC      desc{};
  D3D11_SUBRESOURCE_DATA subres{};

  desc.ByteWidth           = stride * elementCount;
  desc.StructureByteStride = stride;
  desc.Usage               = D3D11_USAGE_DEFAULT;
  desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
  desc.CPUAccessFlags      = 0;
  desc.MiscFlags           = 0;
  subres.pSysMem = bufferPositions.data();
  OPTIM_TRY_DX(pDevice->CreateBuffer(&desc, &subres, &pBuffer));

  // BUILD NORMAL DATA
  desc.ByteWidth           = uvStreamStride * elementCount;
  desc.StructureByteStride = uvStreamStride;
  subres.pSysMem           = bufferuv.data();
  OPTIM_TRY_DX(pDevice->CreateBuffer(&desc, &subres, &uvStream));

  // BUILD UV DATA
  desc.ByteWidth           = normalStreamStride * elementCount;
  desc.StructureByteStride = normalStreamStride;
  subres.pSysMem           = bufferNormal.data();
  OPTIM_TRY_DX(pDevice->CreateBuffer(&desc, &subres, &normalStream));

  printf("[Dx11VertexBuffer] Vertex Buffer resource created\n");
}

void Dx11VertexBuffer::bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) {
  // NOTES
  //
  // Input Assembler supports 16 slots for Dx11
  //
  // Param 1 -> Start input slot (mostly always 0 since 0 will be the slot for positions)
  //
  // Param 2 -> buffer count
  //
  // Param 3 -> Array of pointers to buffers (ID3D11Buffer**) | eg. std::vector<ID3D11Buffer*>.data()
  //
  // Param 4 -> Stride = byte size of a single element in a buffer
  //
  // Param 5 -> Offsets | bytes offsets for each buffer (mostly stay 0 for all array)
  //
  // [DOCUMENTATION] https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetvertexbuffers
  //

  // build buffers
  /*
  std::vector<ID3D11Buffer*> l_buffers{
    pBuffer.Get(),
    uvStream.Get(),
    normalStream.Get()
  };

  std::vector<uint32> l_strides {
    stride, 
    uvStreamStride,
    normalStreamStride
  };

  std::vector<uint32> l_offsets{
    0, 0, 0
  };
  */

  std::vector<ID3D11Buffer*> l_buffers;
  std::vector<uint32> l_strides;
  std::vector<uint32> l_offsets;

  for (uint32& i : Dx11RHI::getActivePipelineInputs()) {
    if (i == 0) {
      l_buffers.push_back(pBuffer.Get());
      l_strides.push_back(stride);
      l_offsets.push_back(0);
    }

    if (i == 1) {
      l_buffers.push_back(uvStream.Get());
      l_strides.push_back(uvStreamStride);
      l_offsets.push_back(0);
    }

    if (i == 2) {
      l_buffers.push_back(normalStream.Get());
      l_strides.push_back(normalStreamStride);
      l_offsets.push_back(0);
    }
  }

  pContext->IASetVertexBuffers(0, static_cast<uint32>(l_buffers.size()), l_buffers.data(), l_strides.data(), l_offsets.data());
}