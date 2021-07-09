#include "GPIO.h"

GPIO::GPIO() : OK(false)
{
    if (SusiLibInitialize() == SUSI_STATUS_SUCCESS)
        if (SusiGPIOSetDirection(SUSI_ID_GPIO_BANK(0), 0b11111111, uint32_t(PinType::Output)) == SUSI_STATUS_SUCCESS)
            if (SusiGPIOSetLevel(SUSI_ID_GPIO_BANK(0), 0b11111111, uint32_t(Level::Low)) == SUSI_STATUS_SUCCESS) {
                Log::get().write("GPIO OK\n");
                OK = true;
            }
            else Log::get().write("GPIO ERROR : SET LEVEL\n");
        else Log::get().write("GPIO ERROR : SET DIRECTION\n");
    else Log::get().write("GPIO ERROR : CANNOT INIT\n"); 
    Log::get().ln();
    Log::get().update();
}

GPIO::~GPIO()
{
    SusiLibUninitialize();
}

bool GPIO::setPinType(PinNumber number, PinType type)
{
    if (!OK) return false;

    Log::get().write("Changement de fonction de la pin GPIO");
    Log::get().write(String((int)number));  
    Log::get().write(" qui passe en mode ");
    Log::get().write(String(type == PinType::Input ? "input" : "output"));
    Log::get().ln();
    Log::get().update();
    return SusiGPIOSetDirection(SUSI_ID_GPIO((int)number), 1, (uint32_t)type) == SUSI_STATUS_SUCCESS;
}

bool GPIO::setPinLevel(PinNumber number, Level level)
{
    if (!OK) return false;    
    
    Log::get().write("Changement du niveau de la pin GPIO");
    Log::get().write(String((int)number));
    Log::get().write(" qui devient ");
    Log::get().write(String((int)level));
    Log::get().ln();
    Log::get().update();
    return SusiGPIOSetLevel(SUSI_ID_GPIO((int)number), 1, (uint32_t)level) == SUSI_STATUS_SUCCESS;
}

GPIO::Level GPIO::getPinLevel(PinNumber number, bool& status)
{
    if (!OK) return Level::Low;
    GPIO::Level l;
    status = SusiGPIOGetLevel(SUSI_ID_GPIO((int)number), 1, (uint32_t*)&l) == SUSI_STATUS_SUCCESS;
    Log::get().write(CharPointer_UTF16(L"R\u00E9up\u00E9ration du niveau de la pin "));
    Log::get().write(String((int)number));
    Log::get().write(". Niveau ");
    Log::get().write(String((int)l));
    Log::get().ln();
    Log::get().update();
    return l;
}
