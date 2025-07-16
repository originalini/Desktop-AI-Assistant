#include "CastomEvent.h"

// Привязка кастомного события для его дальнейшей обработки
wxDEFINE_EVENT(myEvent_CheckStatusLoadModel, MyEventLoop);

// Привязка кастомного события для его дальнейшей обработки
wxDEFINE_EVENT(myEvent_GenerateMessage, MyEventLoop); 

// Привязка кастомного события для его дальнейшей обработки
wxDEFINE_EVENT(myEvent_MessageAiView, MyEventLoop);


MyEventLoop::MyEventLoop(wxEventType type, int id) : wxCommandEvent(type, id), StatusLoadModel{ -1 }, indexGenerating{false} {}

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
