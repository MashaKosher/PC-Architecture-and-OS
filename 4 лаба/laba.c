/*
    2. Если устройство не мост (0-бит поля Header Type = 0)
    вывести и расшифровать значение полей базовых регистров памяти.

    6. Если устройство не мост (0-бит поля Header Type = 0)
    вывести и расшифровать значение поля Interrupt Line.

    7. Если устройство не мост (0-бит поля Header Type = 0)
    вывести и расшифровать значение поля Interrupt Pin.
*/

#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "sys/io.h"
#include "pci.h"

#define PRIVILEGE_LEVEL 3 // Требуемый уровень доступа

#define BUS_NUM 256    // Максимальное число шин
#define DEVICE_NUM 32  // Максимальное число устройств
#define FUNCTION_NUM 8 // Максимальное число функций
//#define BASE_ADDRESS_REGISTERS_NUM 6

#define CONFIG_ADDRESS 0xCF8 // Адрес порта ввода
#define CONFIG_DATA 0xCFC    // Адрес порта вывода

#define ENABLE_BIT_SHIFT 31 // Сдвиг для значимого бита
#define BUS_SHIFT 16        // Сдвиг для шины
#define DEVICE_SHIFT 11     // Сдвиг для устройства
#define FUNCTION_SHIFT 8    // Сдвиг для функции
#define REGISTER_SHIFT 2    // Сдвиг для регистра

#define DEVICE_ID_OFFSET 0x2     // Отступ для идентификатора устройства
//надо

#define ROM_REGISTER_OFFSET 12

#define BUS_REGISTER_OFFSET 6

#define IOBASE_REGISTER_OFFSET 7

//
//#define INTERRUPT_PIN_OFFSET 0x8 // Отступ для Interrupt Pin

#define ID_REGISTER_OFFSET 0x0          // Отступ для регистра идентификаторов
#define HEADER_TYPE_REGISTER_OFFSET 0x3 // Отступ для регистра типа заголовка
//#define INTERRUPT_REGISTER_OFFSET 15    // Отступ для регистра прерываний
//#define BASE_ADDRESS_REGISTER_OFFSET 4

#define HEADER_TYPE_SHIFT 16 // Сдвиг для типа заголовка

#define PCI_VENDORS_TABLE_LENGHT (sizeof(PciVenTable) / sizeof(PCI_VENTABLE))
#define PCI_DEVICE_TABLE_LENGHT (sizeof(PciDevTable) / sizeof(PCI_DEVTABLE))

#define FUNCTION_NOT_FOUND -1
#define UNKNOWN_DEVICE_MESSAGE "UNKNOWN DEVICE"
#define UNKNOWN_VENDOR_MESSAGE "UNKNOWN VENDOR"

//#define INTERRUPT_PIN_NOT_USED_MESSAGE "NOT USED"
//#define INTERRUPT_PIN_NOT_USED 0
//#define INTERRUPT_PIN_A_MESSAGE "INTA#"
//#define INTERRUPT_PIN_A 1
//#define INTERRUPT_PIN_B_MESSAGE "INTB#"
//#define INTERRUPT_PIN_B 2
//#define INTERRUPT_PIN_C_MESSAGE "INTC#"
//#define INTERRUPT_PIN_C 3
//#define INTERRUPT_PIN_D_MESSAGE "INTD#"
//#define INTERRUPT_PIN_D 4
//#define INVALID_INTERRUPT_PIN "INVALID PIN"
//
//#define RESERVED_INTERRUPT_MESSAGE "RESERVED"
//#define IRQ_MESSAGE "IRQ%d"
//#define INVALID_INTERRUPT_LINE_MESSAGE "INVALID INTERRUPT LINE"

// Вычислить адрес, по которому необходимо обратиться
int calculateAddress(int bus, int device, int function, int _register)
{
    return (1 << ENABLE_BIT_SHIFT) | (bus << BUS_SHIFT) | (device << DEVICE_SHIFT) | (function << FUNCTION_SHIFT) | (_register << REGISTER_SHIFT);
}

// Прочитать содержимое регистра
int readRegister(int bus, int device, int function, int _register)
{
    int address = calculateAddress(bus, device, function, _register);
    outl(address, CONFIG_ADDRESS);

    return inl(CONFIG_DATA);
}

// Сменить уровень доступа, если успешно - вернуть true, иначе - вернуть false
_Bool isPrivilegeLevelChangeSuccessful(void)
{
    if (iopl(PRIVILEGE_LEVEL))
    {
        return false;
    }

    return true;
}

// Проверить, доступна ли функция
_Bool isFunctionAvailable(long idRegisterData)
{
    return idRegisterData != FUNCTION_NOT_FOUND;
}

// Вычислить идентификатор устройства
int getDeviceId(int idRegisterData)
{
    return idRegisterData >> DEVICE_ID_OFFSET * 8;
}

// Вычислить идентификатор производителя
int getVendorId(int idRegisterData)
{
    return idRegisterData & 0xFFFF;
}

// Получить имя производителя
char *getVendorName(__uint16_t vendorId)
{
    for (int i = 0; i < PCI_VENDORS_TABLE_LENGHT; i++)
    {
        if (PciVenTable[i].VendorId == vendorId)
        {
            return PciVenTable[i].VendorName;
        }
    }

    return UNKNOWN_VENDOR_MESSAGE;
}

// Получить имя устройства
char *getDeviceName(__uint16_t vendorId, __uint16_t deviceId)
{
    for (int i = 0; i < PCI_DEVICE_TABLE_LENGHT; i++)
    {
        if (PciDevTable[i].VendorId == vendorId && PciDevTable[i].DeviceId == deviceId)
        {
            return PciDevTable[i].DeviceName;
        }
    }

    return UNKNOWN_DEVICE_MESSAGE;
}

// Вывести основную информацию о функции
void printGeneralInfo(int bus, int device, int function, int idRegisterData)
{
    int deviceId = getDeviceId(idRegisterData);
    int vendorId = getVendorId(idRegisterData);
    char *deviceName = getDeviceName(vendorId, deviceId);
    char *vendorName = getVendorName(vendorId);

    printf("%02x:%02x.%x %0x %0x %s %s ", bus, device, function, vendorId, deviceId, vendorName, deviceName);
}

// Проверить, является ли функция мостом, если да - true, если нет - false
_Bool isBridge(int bus, int device, int function)
{
    int headerTypeRegisterData = readRegister(bus, device, function, HEADER_TYPE_REGISTER_OFFSET);
    return ((headerTypeRegisterData >> HEADER_TYPE_SHIFT) & 0xFF) & 1;
}





// ROM
void printROM(int bus, int device, int function)
{

    int registerData = readRegister(bus, device, function, ROM_REGISTER_OFFSET );
    if (registerData)
        {
            printf("%#1x, ROM register, ", (unsigned int)registerData >> 11);
        }
        else
        {
            printf("0x0000, unused register, ");
        }
}

void printBusNumber(int bus, int device, int function){
    int registerData = readRegister(bus, device, function, BUS_REGISTER_OFFSET );
    if (registerData){
        printf("%#1x, primary Bus number, ", (unsigned int)registerData & 0xFF);
        printf("%#1x, secondary Bus number, ", ((unsigned int)registerData >> 8) & 0xFF);
        printf("%#1x, subordinate Bus number, ", ((unsigned int)registerData >> 16) & 0xFF);
    } else{
        printf("0x0000, unused register, ");
    }

}

void printIO(int bus, int device, int function){
    int registerData = readRegister(bus, device, function, IOBASE_REGISTER_OFFSET );
    if (registerData){
        printf("%#1x, IOBase number, ", (unsigned int)registerData & 0xFF);
        printf("%#1x, IOLimit number, ", ((unsigned int)registerData >> 8) & 0xFF);
    } else{
        printf("0x0000, unused register, ");
    }
}

//
// Проверить функцию
void checkFunction(int bus, int device, int function)
{
    int idRegisterData = readRegister(bus, device, function, ID_REGISTER_OFFSET);

    if (isFunctionAvailable(idRegisterData))
    {
        printGeneralInfo(bus, device, function, idRegisterData);

        if (!isBridge(bus, device, function))
        {
            printROM(bus, device, function);

        } else{
            printBusNumber(bus, device, function);
            printIO(bus, device, function);
        }

        printf("\n");
    }
}

// Перечислить все функции, во всех устройствах, во всех шинах
void enumerate(void)
{
    for (int bus = 0; bus < BUS_NUM; bus++)
    {
        for (int device = 0; device < DEVICE_NUM; device++)
        {
            for (int function = 0; function < FUNCTION_NUM; function++)
            {
                checkFunction(bus, device, function);
            }
        }
    }
}

int main(void)
{
    //  Меняем уровень доступа, если смена не произошла - выходим из программы
    if (!isPrivilegeLevelChangeSuccessful())
    {
        printf("Input/Output privilege level change error. Try running under root user. \n");

        return 1;
    }

    enumerate();

    return 0;
}