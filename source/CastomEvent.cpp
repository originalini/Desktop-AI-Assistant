#include "CastomEvent.h"

// Кастомное событие для проверки статуса загрузки(работы)
wxDEFINE_EVENT(myEvent_CheckStatusLoadModel, MyEventLoop);

// Кастомное событие для генерации сообщения нейросетью
wxDEFINE_EVENT(myEvent_GenerateMessage, MyEventLoop); 

// Кастомное событие для отображения сообщения нейросети
wxDEFINE_EVENT(myEvent_MessageAiView, MyEventLoop);


MyEventLoop::MyEventLoop(wxEventType type, int id) : wxCommandEvent(type, id), StatusLoadModel{ -1 } {}

void MyEventLoop::SetOutputAI(std::string outputAI) {
	this->outputAI = outputAI;
}

std::string MyEventLoop::GetOutputAI() {
	return this->outputAI;
}

void MyEventLoop::SetStatusLoadModel(int StatusLoadModel) {
	this->StatusLoadModel = StatusLoadModel;
}

int MyEventLoop::GetStatusLoadModel() {
	return StatusLoadModel;
}
