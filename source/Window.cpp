#include "Window.h"

bool MyApp::OnInit() {
	// Создаем окно и задаем ему название "My Assistent"
	MyFrame* mainFrame = new MyFrame("My Assistent");
	
	//  Задаем параметры окна:
	//  - Размер 700 на 700 пикселей;
	//  - Цвет окна белый;
	//  - Отображение окна по цетру;
	mainFrame->SetClientSize(wxSize(700,700));
	mainFrame->SetBackgroundColour(wxColour(255, 255, 255));
	mainFrame->Center();
	mainFrame->SetDoubleBuffered(true);

	// Отображаем окно.
	mainFrame->Show();
	return true;
}

wxIMPLEMENT_APP(MyApp);


