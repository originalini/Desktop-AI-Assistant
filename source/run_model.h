#pragma once

#include "Window.h"

class MyFrame;

class RunModel {
private:
	std::string model_path;				// <- Путь до модели "C\\:..."
	int ngl;							// <- Количество слоев загружаемых в видеокарту или ОЗУ
	int n_ctx;							// <- Размер контекста диалога.

	llama_model_params model_params;	// <- Параметры модели
	llama_model* model;					// <- Сама модель
	const llama_vocab* vocab;			// <- Словарь модели
	llama_context_params ctx_params;	// <- Параметры контекста модели
	llama_context* ctx;					// <- Сам контекст
	llama_sampler* smpl;				// <- Параметры для отбора предсказаных токенов(температура и т.д)

	std::vector<llama_chat_message> messages; // <- Чат с нейросетью
	std::vector<char> formatted;
	int prev_len;
public:
	RunModel();
	
	~RunModel();

	/*	
	* @brief Инициализация и загрузка модели в оперативную память устройства.
	* 
	* @param path - путь до модели;
	* @param n_gpu_layers - количество слоев загружаемых в видеокарту устройства;
	* @param n_context - размер контекста(чата) с нейросетью;
	* 
	* @return Возвращается результат работы функции, смотреть список STATUS_LOAD_MODEl
	*/
	int InitAI(std::string path, int n_gpu_layers, int n_context);

	/*
	* @brief Токенизация и приведение сообщения в подходящий формат для нейросети.
	* 
	* @param message - сообщение пользователя;
	* 
	* @return - Возвращается текст приведенный в формате для подачи в нейросеть.
	*/
	std::string TokenizationMessage(std::string message);

	/// <summary>
	/// Генерация сообщения нейросетью.
	/// </summary>
	/// <param name="prompt"> - отправленное пользователем сообщения(заранее приведенное в формат 
	/// для нейросети функцией - TokenizationMessage());</param>
	/// <returns>Возвращается сгенерированный ответ нейросетью </returns>
	std::string GenerateOutput(std::string prompt, MyFrame* frame);
};