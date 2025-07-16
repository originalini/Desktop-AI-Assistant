#pragma once

#include "Window.h"

wxDECLARE_EVENT(myEvent_CheckStatusLoadModel, MyEventLoop);
wxDECLARE_EVENT(myEvent_GenerateMessage, MyEventLoop);
wxDECLARE_EVENT(myEvent_MessageAiView, MyEventLoop);

class MyEventLoop : public wxCommandEvent {
public:
	MyEventLoop(wxEventType type, int id);
	virtual wxEvent* Clone() const { return new MyEventLoop(*this); }


	bool indexGenerating;

	/// <summary>
	/// Устанавливает сгенерированный нейросетью текст.
	/// </summary>
	/// <param name="outputAI"></param>
	void SetOutputAI(std::string outputAI);
	
	
	// @brief Функция отвечает за взятие сгенерированного сообщения нейросетью. 
	// @return Возвращает сгенерированный неросетью текст
	std::string GetOutputAI();

	// @brief Установка статуса загрузки модели. 
	// @param StatusLoadModel - содержит статус загрузки модели 
	void SetStatusLoadModel(int StatusLoadModel);

	// @brief Возвращает статус загрузки модели
	// @return Возврат статуса загрузки модели
	int GetStatusLoadModel();
private:

	// @brief Хранит в себе статус загрузки модели.(Смотреть "enum STATUS_LOAD_MODEL") 
	int StatusLoadModel;


	// @brief Хранит сгенерированный текст нейросети.
	std::string outputAI;
};

