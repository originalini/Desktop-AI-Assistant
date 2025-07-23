#pragma once

#include "Window.h"

class MyEventLoop;
class RunModel;

class MyFrame : public wxFrame {
private:

	// @brief Поле для отображния и прокручивания сообщений.
	wxRichTextCtrl* messages;

	// @brief Строка состояния для отображения отладочной информации.
	wxStatusBar* bar;

	// @brief Панель для вывода сообщений либо пользователя, либо сгенерированных нейросетью.
	wxPanel* scrolledPanel;

	// @brief Панель для отображения элементов управления(зона для написания смс и кнопка для отправки).
	wxPanel* PanelButton;

	// @brief Элемент упралвения ввод текста в определенной области
	wxTextCtrl* InputText;

	// @brief Элемент управления кнопка, для отправки сообщения.
	wxButton* Button;

	// @brief Главный BoxSizer для привязки элементов к определонному месту.
	wxBoxSizer* boxSizerMain;

	// @brief Второстепенный BoxSizer для привязки кнопоки отправки и зоны ввода сообщения.
	wxBoxSizer* boxSizerButton;

	// @brief Второстепенный BoxSizer для привязки отправленных сообщений.
	wxBoxSizer* boxSizerMessage;

	// @brief Переменная для вывода введенного текста.
	std::vector<wxStaticText*> TextStat;

	// @brief Функция для обработки вывода сообщения.
	void MessageDisplay();

	// @brief Функция для обработки вывода кнопок и виджетов.
	void ButtonDisplay();

	// @brief Добавляет сообщение пользователя для дальнейшей обработки.
	// @param messageUser - сообщение пользователя 
	void AddMessageUser(std::string messageUser); // Добавления сообщения пользователя.

	// @brief Добавляет сообщения нейросети на экран вывода.
	void ViewMessageAI(MyEventLoop& event);

	// @brief Проверка статуса загрузки модели и вывод результата в строку состояния.
	// @param event - используется для обработки кастомных событий;
	void ShowStatusLoadModelInStatusBar(MyEventLoop& event);

public:
	MyFrame(const wxString&);
	~MyFrame();

	/// Хранит статус загрузки модели.
	int statusLoadModel;

	/// @brief Класс для загрузки модели.
	RunModel* model;

	/*
	* @brief Функция обрабатываюшая событие нажатия на кнопку отправки.
	*
	* При срабатывании отправляет сообщение и добавляет его в массив TextStat для дальнейшей обработки.
	*
	* @param clickButton - данный параметр используется для обработки событий нажатия на кнопку;
	*/
	void OnSendButtonClickEvent(wxCommandEvent& clickButton);
};