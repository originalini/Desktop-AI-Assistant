#include "Window.h"

class InteractrionWindows {
private:
	// Регулировка звук
	void ChangeVolume(std::string Strvolume);

	// Изменение состояния пк(выкл, спящий, перезагркузка)
	void SetPowerState(std::string StrState);

public:
	InteractrionWindows(std::vector<std::string> Command);


};