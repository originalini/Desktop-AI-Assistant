#pragma once

#include <wx/wx.h>
#include <wx/richtext/richtextctrl.h>
#include <llama.h>

#include <vector> 
#include <string>
#include <thread>
#include <chrono>

#include "UI_manager.h"
#include "run_model.h"
#include "CastomEvent.h"

enum STATUS_MODEL {
	LOAD_NOT_LOAD = -1,													// <- Модель не загружена!
	LOAD_COMPLETE = 0,													// <- Успешная загрузка модели и готова получать сообщение от пользователя;
	READY_GET_MESSAGE_USER = 1,
	ERROR_PATH_EMPTY = 2,												// <- Ошибка загрузки, неверно указан путь(либо вообще пустой);
	ERROR_LOAD_MODEL = 3,												// <- Ошибка загрузки, ошибочно загрузилась модель;
	ERROR_LOAD_VOCAB = 4,												// <- Ошибка загрузки словаря модели;
	ERROR_INIT_CTX = 5,													// <- Ошибка инициализации контекста модели;
	GENERATE_OUTPUT = 6,												// <- Модель генерирует ответ;
	EXIT_THREAD_WIN_CLOSE = 7											// <- Завершение потока закрытием программы
};

class MyApp : public wxApp {
public:
	bool OnInit(); // <- Инициализация окна приложения.

};


