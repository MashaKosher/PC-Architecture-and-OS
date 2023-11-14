// %ignore%
/*
* Строки со следующими ключами будут заменены:
* %vendorList% - списком производителей,
* %deviceList% - списком устройств,
* %subsystemList% - списком подсистем устройств.
*/
// %ignore% позволяет исключить строки из выходного файла.
/*
* Если поля Version и Date выше существенно отличаются от текущей даты - самое
* время скачать актуальную версию файла pci.ids с http://pci-ids.ucw.cz/v2.2/pci.ids
* и при помощи idsparser.jar заново сгенерировать необходимые *.h-файлы.
* Подробная инструкция - в файле README.
*/

typedef struct __PCI_VENTABLE
{
	unsigned short VendorId;
	char *VendorName;
} PCI_VENTABLE, *PPCI_VENTABLE;

PCI_VENTABLE PciVenTable[] =
{
	// %vendorList%
};

#define	PCI_VENTABLE_LEN (sizeof(PciVenTable)/sizeof(PCI_VENTABLE))

typedef struct __PCI_DEVTABLE
{
	unsigned short VendorId;
	unsigned short DeviceId;
	char *DeviceName;
} PCI_DEVTABLE, *PPCI_DEVTABLE;

PCI_DEVTABLE PciDevTable[] =
{
	// %deviceList%
};

#define	PCI_DEVTABLE_LEN (sizeof(PciDevTable)/sizeof(PCI_DEVTABLE))

typedef struct __PCI_SUBTABLE
{
	unsigned short VendorId;
	unsigned short DeviceId;
	unsigned short SubVendorId;
	unsigned short SubDeviceId;
	char *SubDeviceName;
} PCI_SUBTABLE, *PPCI_SUBTABLE;

PCI_SUBTABLE PciSubTable[] =
{
	// %subsystemList%
};

#define PCI_SUBTABLE_LEN (sizeof(PciSubTable)/sizeof(PCI_SUBTABLE))