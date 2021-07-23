/*
  ==============================================================================

	AlertWindows.cpp
	Created: 23 Jul 2021 1:41:23pm
	Author:  admin

  ==============================================================================
*/

#include "AlertWindows.h"

APPCAlertWindows::APPCAlertWindows()
{
}

void APPCAlertWindows::open(WindowType window, const std::function<void(int)>& callbackfunction)
{
	switch (window)
	{
	case APPCAlertWindows::WindowType::PriceChoice:
		aw = new AlertWindow("Quel prix charger ?", "", AlertWindow::AlertIconType::QuestionIcon);
		aw->addButton("Prix UC", 1);
		aw->addButton("Prix station", 2);
		break;
	case APPCAlertWindows::WindowType::LoadFromCentoFuel:
		aw = new AlertWindow("Charger les informations depuis CentoFuel ?",
			L"Connexion à CentoFuel possible, charger les prix en ligne ?", AlertWindow::AlertIconType::QuestionIcon);
		aw->addButton("Charger depuis CentoFuel", 1);
		aw->addButton("Continuer en local", 2);
		break;
	default:
		break;
	}

	aw->enterModalState(true, ModalCallbackFunction::create(
		[this, callbackfunction](int r) {callbackfunction(r); aw->setVisible(false); }), true);
}
