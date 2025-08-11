#include "InteractionWindows.h"
#include "../Utility/Utility.h"


InteractrionWindows::InteractrionWindows(std::vector<std::string> Command) {
	for (int i = 0; i < Command.size(); ++i) {
		if (Command[i] == "/volume") {
			ChangeVolume(Command[i + 1]);
		}
        else if (Command[i] == "/mode") {
            SetPowerState(Command[i + 1]);
        }
	}

}

void InteractrionWindows::SetPowerState(std::string Strstate) {
    if (Strstate == "off") {
        system("shutdown /p");
    }
    else if (Strstate == "restart") {
        system("shutdown /r /t 0");
    }
    else if (Strstate == "sleep") {
        system("shutdown /h");
    }
}

void InteractrionWindows::ChangeVolume(std::string Strvolume) {
    float volume = (float)((float)(myUtility::GetNumberInString(Strvolume))) / 100;

    CoInitialize(nullptr);

    IMMDeviceEnumerator* pEnumerator = nullptr;
    IMMDevice* pDevice = nullptr;
    IAudioEndpointVolume* pVolume = nullptr;

    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr)) {
        return;
    }

    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
    if (FAILED(hr)) {
        pEnumerator->Release();
        return;
    }

    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void**)&pVolume);
    if (FAILED(hr)) {
        pDevice->Release();
        pEnumerator->Release();
        return;
    }

    hr = pVolume->SetMasterVolumeLevelScalar(volume, nullptr); // volume from 0.0 to 1.0
    pVolume->Release();
    pDevice->Release();
    pEnumerator->Release();

    CoUninitialize();
}