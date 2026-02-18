#include "Core/Utilities/Random/Random.h"

#ifdef OS_WINDOWS
  #include "Core/Defines/Windows/windowsAPI.h"

  #include <rpc.h>
  #pragma comment(lib, "Rpcrt4.lib")
#endif // OS_WINDOWS

SGuid CORE_API Optim::Random::getGetGuid() {
#ifdef OS_WINDOWS
  SGuid value{};
  UUID  uuid{};
  UuidCreate(&uuid);
  memcpy(value.data, &uuid, 16);
  return value;
#endif
}
