#include "Window.h"
#include "UI_manager.h"
#include "run_model.h"

MyFrame::~MyFrame() {
	delete model;
}

MyFrame::MyFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title), 
										  TextStat{ nullptr }, statusLoadModel{ STATUS_LOAD_NOT_LOAD } {

	model = new RunModel; // <- Выделение памяти под модель.
	
	CreateStatusBar(); // <- Создание строки состояния.

	bar = GetStatusBar(); // <- Присваиваем созданый.

	boxSizerMain = new wxBoxSizer(wxVERTICAL); // <- Главный сайзер;

	MessageDisplay(); // <- Вывод сообщений;
	ButtonDisplay(); // <- Вывод кнопок;

	this->SetSizerAndFit(boxSizerMain); // <- Добавляем в Frame главный сайзер(вboxSizerMain);

	messages = new wxRichTextCtrl(scrolledPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1, 620), wxRE_READONLY);
	wxFont font(13, wxDEFAULT, wxNORMAL, wxNORMAL, false);
	messages->SetFont(font);

	boxSizerMessage->Add(messages, 0 , wxALL | wxEXPAND , 10);

	bar->SetStatusText("Загрузка модели...");
	std::thread LoadModel_th([this]() {
		statusLoadModel = model->InitAI("C:/Users/pipet/source/repos/My_AI-Assistent/AI-Model/Qwen3-14B-Q4_K_S.gguf", 99, 8192);

		MyEventLoop evt(myEvent_CheckStatusLoadModel, this->GetId());

		evt.SetStatusLoadModel(statusLoadModel);
		wxQueueEvent(this, evt.Clone());
	});
	LoadModel_th.detach(); // <- Выделяем отдельный поток под загрузку модели.

	Button->Bind(wxEVT_BUTTON, &MyFrame::OnSendButtonClickEvent, this); // <- Привязка для обработки нажатия на клавишу и отправки сообщения.
	Bind(myEvent_CheckStatusLoadModel, &MyFrame::ShowStatusLoadModelInStatusBar, this); // <- кастомное событие.
	Bind(myEvent_MessageAiView, &MyFrame::ViewMessageAI, this);
}

void MyFrame::MessageDisplay() {
	boxSizerMessage = new wxBoxSizer(wxVERTICAL); // <- Сайзер сообщений;

	scrolledPanel = new wxPanel(this, wxID_ANY); // <- Панель для вывода сообщений.
	scrolledPanel->SetDoubleBuffered(true);

	boxSizerMain->Add(scrolledPanel, 1, wxEXPAND); // <- Добавляем в главный сайзер scrolledPanel;

	scrolledPanel->SetSizer(boxSizerMessage); // <- Добавляем сайзер для панели сообщений;
}

void MyFrame::ButtonDisplay() {
	boxSizerButton = new wxBoxSizer(wxHORIZONTAL); // <- Сайзер кнопок;

	PanelButton = new wxPanel(this, wxID_ANY); // <- панель кнопок;

	boxSizerMain->Add(PanelButton, 0, wxEXPAND); // <- Добавляем в главный сайзер PanelButton;

	// Создаем виджета ввода сообщения;
	InputText = new wxTextCtrl(PanelButton, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(600, 50), wxTE_MULTILINE);
	InputText->SetBackgroundColour(wxColour(120, 120, 120)); // <- Задаем цвет; 

	// Создаем виджет кнопка для отправки сообщения;
	Button = new wxButton(PanelButton, wxID_ANY, ">>", wxDefaultPosition, wxSize(80, 50));
	Button->SetBackgroundColour(wxColour(120, 120, 120)); // <- Задаем цвет;

	boxSizerButton->Add(InputText, 0, wxALL, 5); // <- Добавляем в сайзер кнопок виджет InputText; 
	boxSizerButton->Add(Button, 0, wxALL, 5); // <- Добавляем в сайзер кнопок виджет Button; 

	PanelButton->SetSizer(boxSizerButton); // <- Добавляем сайзер для панель кнопок;
}

void MyFrame::AddMessageUser(std::string messageUser) {
	messages->SetInsertionPointEnd();
	
	wxRichTextAttr  atrubits;
	atrubits.SetAlignment(wxTEXT_ALIGNMENT_RIGHT);

	messages->BeginStyle(atrubits);

	messageUser = "Пользователь:\n" + messageUser + "\n";

	messages->WriteText(messageUser);

	messages->EndStyle();

	messages->SetInsertionPointEnd();
	messages->WriteText("Нейросеть:\n");
}

void MyFrame::ViewMessageAI(MyEventLoop& event) {
	messages->SetInsertionPointEnd();

	wxRichTextAttr atrubits;
	atrubits.SetAlignment(wxTEXT_ALIGNMENT_LEFT);

	messages->BeginStyle(atrubits);
	
	std::string messageAI = event.GetOutputAI();
	messages->WriteText(messageAI);
	
	messages->EndStyle();
	messages->SetInsertionPointEnd();
}

void MyFrame::OnSendButtonClickEvent(wxCommandEvent& clickButton) {
	// Проверка пустая ли строка и присвоен ли ей статус готовности к общению с пользователем.
	if (!InputText->IsEmpty() && (statusLoadModel == STATUS_LOAD_COMPLETE || statusLoadModel == STATUS_READY_GET_MESSAGE_USER)) {
		std::string Message = InputText->GetValue(); // <- Достаем введенное сообщение с виджета ввода(InputText).
		InputText->Clear();

		AddMessageUser(Message); // <- Добавляем сообщения для его вывода на экран.

		bar->SetStatusText("Генерация ответа...");
		std::thread generateMessageAI_th([this, Message]() {
			statusLoadModel = STATUS_GENERATE_OUTPUT; // <- Присваиваем статус модели "генерация ответа".

			std::string prompt = model->TokenizationMessage(Message);// <- Токенизация сообщения.

			std::string outputAI = model->GenerateOutput(prompt, this);
		});
		generateMessageAI_th.detach();
		Refresh(); // <- Перерисовываем элементы (отвечает за цвета).
		Layout(); // <- Полное обновление всего окна приложения(кроме цвета).
	}
	else {
		return;
	}
}

void MyFrame::ShowStatusLoadModelInStatusBar(MyEventLoop& event) {
	switch (statusLoadModel) {
	case STATUS_LOAD_COMPLETE:
		bar->SetStatusText("Модель загружена!");
		break;
	case STATUS_ERROR_PATH_EMPTY:
		bar->SetStatusText("Ошибка: неверно указан путь до модели!");
		break;
	case STATUS_ERROR_LOAD_MODEL:
		bar->SetStatusText("Ошибка загрузки модели!");
		break;
	case STATUS_ERROR_LOAD_VOCAB:
		bar->SetStatusText("Ошибка считывания словаря!");
		break;
	case STATUS_ERROR_INIT_CTX:
		bar->SetStatusText("Ошибка инициализации контекста!");
		break;
	}
}