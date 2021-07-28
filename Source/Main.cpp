/*
  ==============================================================================

	This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/
#include <JuceHeader.h>
#include "MainComponent.h"
#include "JSON.h"
#include "Networking.h"
#include "Price.h"
#include "GPIO.h"

//==============================================================================
class APPCApplication : public juce::JUCEApplication
{
public:
	//==============================================================================
	APPCApplication() {  }

	const juce::String getApplicationName() override { return ProjectInfo::projectName; }
	const juce::String getApplicationVersion() override { return ProjectInfo::versionString; }
	bool moreThanOneInstanceAllowed() override { return false; }

	//==============================================================================
	void initialise(const juce::String& commandLine) override
	{
		/*Core::get().setConfigJSON(File::getCurrentWorkingDirectory().getChildFile("lastconfig.config"));
		Core::get().loadInformationsFromJSON();*/
		//================================ SI ON VEUT CHARGER LE DERNIER PRIX ============================
		/*if (File::getCurrentWorkingDirectory().getChildFile("lastprices.prices").existsAsFile()) {
			PriceSave lastPrices(File::getCurrentWorkingDirectory().getChildFile("lastprices.prices"));
			for (int i = 0; i < lastPrices.getNumPrices(); ++i)
				Core::get().setPrice(i, lastPrices.getPrice(i));
			for (int i = lastPrices.getNumPrices(); i < Core::MAX_PRICES; ++i)
				Core::get().setPrice(i, Price("0"));
		}
		else*/

		if (File::getCurrentWorkingDirectory().getChildFile("init.config").existsAsFile()) {
			Log::write("Chargement du fichier init.config");
			Log::ln();
			auto& c = Core::get();
			c.setConfigJSON(File::getCurrentWorkingDirectory().getChildFile("init.config"));
			c.loadInformationsFromJSON();
			c.loadInformationsFromNetwork();
		}

		mainWindow.reset(new MainWindow(getApplicationName()));

	}

	void shutdown() override
	{
		mainWindow = nullptr; // (deletes our window)
		Core::get().kill();
	}

	//==============================================================================
	void systemRequestedQuit() override
	{
		Core::get().savePriceSave(File::getCurrentWorkingDirectory().getChildFile("lastprices.prices"));
		Core::get().saveConfigJSON(File::getCurrentWorkingDirectory().getChildFile("lastconfig.config"));
		File logDir(File::getCurrentWorkingDirectory().getChildFile("Logs"));
		logDir.createDirectory();
		const String& filename = "Log_APPC_" + Time::getCurrentTime().toString(true, true, true, true).replaceCharacters(" :", "_-") + ".txt";
		File log(logDir.getChildFile(filename));
		auto r = log.create();
		log.replaceWithText(String("test"));
		quit();
	}

	void anotherInstanceStarted(const juce::String& commandLine) override
	{
	}

	//==============================================================================
	/*
		This class implements the desktop window that contains an instance of
		our MainComponent class.
	*/
	class MainWindow : public juce::DocumentWindow
	{
	public:
		MainWindow(juce::String name)
			: DocumentWindow(name,
				juce::Desktop::getInstance().getDefaultLookAndFeel()
				.findColour(juce::ResizableWindow::backgroundColourId),
				DocumentWindow::allButtons)
		{
			setUsingNativeTitleBar(true);
			setContentOwned(new MainComponent(), true);

#if JUCE_IOS || JUCE_ANDROID
			setFullScreen(true);
#else
			setResizable(true, true);
			centreWithSize(getWidth(), getHeight());
#endif

			setVisible(true);
		}

		void closeButtonPressed() override
		{
			// This is called when the user tries to close this window. Here, we'll just
			// ask the app to quit when this happens, but you can change this to do
			// whatever you need.
			JUCEApplication::getInstance()->systemRequestedQuit();
		}

		/* Note: Be careful if you override any DocumentWindow methods - the base
		   class uses a lot of them, so by overriding you might break its functionality.
		   It's best to do all your work in your content component instead, but if
		   you really have to override any DocumentWindow methods, make sure your
		   subclass also calls the superclass's method.
		*/

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
	};

private:
	std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(APPCApplication)
