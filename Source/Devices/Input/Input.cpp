#include "Devices/Input/Input.h"
#include "Application/Application.h"

Input* Input::sm_pSingleton{ nullptr };

Input* Input::Initalize(Application* _pApplication, void* _hWindowHandle)
{
	/*
		If already registered return nullptr
	*/
	if (sm_pSingleton != nullptr) 
		return nullptr;

	// Instanciate Input Class
	sm_pSingleton = new Input{};

	/*
		Windows OS specifics
	*/
#if defined(WINDOWS_OS)
	/*
	UINT deviceCount{ 0 };
	GetRawInputDeviceList(nullptr, &deviceCount, sizeof(RAWINPUTDEVICELIST));

	RAWINPUTDEVICELIST* deviceList = new RAWINPUTDEVICELIST[deviceCount];

	GetRawInputDeviceList(deviceList, &deviceCount, sizeof(RAWINPUTDEVICELIST));
	for (UINT i = 0; i < deviceCount; i++)
	{
		UINT nameSize = 0;
		GetRawInputDeviceInfo(deviceList[i].hDevice, RIDI_DEVICENAME, nullptr, &nameSize);

		WCHAR* name = new WCHAR[nameSize];
		GetRawInputDeviceInfo(deviceList[i].hDevice, RIDI_DEVICENAME, name, &nameSize);

		RID_DEVICE_INFO info{};
		info.cbSize = sizeof(RID_DEVICE_INFO);
		UINT infoSize = sizeof(RID_DEVICE_INFO);

		GetRawInputDeviceInfoW(deviceList[i].hDevice, RIDI_DEVICEINFO, &info, &infoSize);

		std::cout << std::hex << "Device Info: \n" <<
			"\tVendor ID: " << info.hid.dwVendorId << "\n" <<
			"\tProduct ID: " << info.hid.dwProductId << "\n" <<
			"\tVersion Number: " << info.hid.dwVersionNumber << "\n" <<
			"\tUsage: " << info.hid.usUsage << "\n" <<
			"\tUsage Page: " << info.hid.usUsagePage << "\n\n";

		delete[] name;
	}

	delete[] deviceList;
	*/

	// Instanciate Raw Input
	RAWINPUTDEVICE rid[3];

	// Mouse
	rid[0].usUsagePage = 0x01;
	rid[0].usUsage = 0x02;
	rid[0].dwFlags = RIDEV_INPUTSINK;
	rid[0].hwndTarget = reinterpret_cast<HWND>(_hWindowHandle);

	// Keyboard
	rid[1].usUsagePage = 0x01;
	rid[1].usUsage = 0x06;
	rid[1].dwFlags = RIDEV_INPUTSINK;
	rid[1].hwndTarget = reinterpret_cast<HWND>(_hWindowHandle);

	// Gamepad
	rid[2].usUsagePage = 0x01;
	rid[2].usUsage = 0x05;
	rid[2].dwFlags = RIDEV_INPUTSINK;
	rid[2].hwndTarget = reinterpret_cast<HWND>(_hWindowHandle);

	// On success return Input instance pointer
	// On Failure delete Input class instance and return nullptr
	
	if (!RegisterRawInputDevices(rid, 1, sizeof(RAWINPUTDEVICE)))
	{
		DWORD error = GetLastError();
		delete(sm_pSingleton);
		sm_pSingleton = nullptr;
		std::cout << "Failed to register Raw input devices. Last Error Code: " << error << '\n';
	}
	else
	{
		std::cout << "Registered Raw Inputs!!!\n";
	}

	return sm_pSingleton;
#endif
}