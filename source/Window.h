#pragma once

#include <wx/wx.h>
#include <llama.h>

#include <vector> 
#include <string>
#include <thread>
#include <chrono>

#include "UI_manager.h"
#include "run_model.h"
#include "CastomEvent.h"

enum STATUS_MODEL {
	STATUS_LOAD_NOT_LOAD = -1,													// <- Модель не загружена!
	STATUS_LOAD_COMPLETE = 0,													// <- Успешная загрузка модели и готова получать сообщение от пользователя;
	STATUS_READY_GET_MESSAGE_USER = 1,
	STATUS_ERROR_PATH_EMPTY = 2,												// <- Ошибка загрузки, неверно указан путь(либо вообще пустой);
	STATUS_ERROR_LOAD_MODEL = 3,												// <- Ошибка загрузки, ошибочно загрузилась модель;
	STATUS_ERROR_LOAD_VOCAB = 4,												// <- Ошибка загрузки словаря модели;
	STATUS_ERROR_INIT_CTX = 5,													// <- Ошибка инициализации контекста модели;
	STATUS_GENERATE_OUTPUT = 6,													// <- Модель генерирует ответ;
};

class MyApp : public wxApp {
public:
	bool OnInit(); // <- Инициализация окна приложения.

};


