/*
  ==============================================================================

	AlertWindows.cpp
	Created: 23 Jul 2021 1:41:23pm
	Author:  admin

  ==============================================================================
*/

#include "AlertWindows.h"

APPCAlertWindows::APPCAlertWindows() : aw(nullptr)
{
}

void APPCAlertWindows::open(WindowType window, const String& message, const std::function<void(int)>& callbackfunction)
{
	switch (window)
	{
	case APPCAlertWindows::WindowType::PriceChoice:
		aw = new AlertWindow("Quel prix charger ?", message.length() ? message : "", AlertWindow::AlertIconType::QuestionIcon);
		aw->addButton("Prix UC", 1);
		aw->addButton("Prix station", 2);
		break;
	case APPCAlertWindows::WindowType::LoadFromCentoFuel:
		aw = new AlertWindow("Charger les informations depuis CentoFuel ?", message.length() ? message :
			L"Connexion à CentoFuel possible, charger les prix en ligne ?", AlertWindow::AlertIconType::QuestionIcon);
		aw->addButton("Charger depuis CentoFuel", 1);
		aw->addButton("Continuer en local", 2);
		break;
	case APPCAlertWindows::WindowType::NoConnection:
		aw = new AlertWindow(L"Pas de connexion détectée !", message.length() ? message :
			L"Connexion à CentoFuel impossible", AlertWindow::AlertIconType::WarningIcon);
		aw->addButton("Ok", 1);
		break;
	default:
		break;
	}
	aw->setAlwaysOnTop(true);
	aw->enterModalState(true, ModalCallbackFunction::create(
		[this, callbackfunction](int r) {if (callbackfunction) callbackfunction(r); aw->setVisible(false); }), true);
}

void APPCAlertWindows::close()
{
	aw->setVisible(false);
	delete aw;
}
