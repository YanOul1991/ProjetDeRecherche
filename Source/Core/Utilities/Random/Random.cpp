#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Utilities/Random/Random.h"

#ifdef OS_WINDOWS
#include <rpc.h>
#pragma comment(lib, "Rpcrt4.lib")
#endif // OS_WINDOWS

SGuid CORE_API Optim::Random::getGetGuid()
{
	SGuid value{};
	UUID uuid{};
	UuidCreate(&uuid);
	memcpy(value.data, &uuid, 16);
	return value;
}
