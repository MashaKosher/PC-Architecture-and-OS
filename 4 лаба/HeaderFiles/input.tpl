// %ignore%
/*
* ������ �� ���������� ������� ����� ��������:
* %vendorList% - ������� ��������������,
* %deviceList% - ������� ���������,
* %subsystemList% - ������� ��������� ���������.
*/
// %ignore% ��������� ��������� ������ �� ��������� �����.
/*
* ���� ���� Version � Date ���� ����������� ���������� �� ������� ���� - �����
* ����� ������� ���������� ������ ����� pci.ids � http://pci-ids.ucw.cz/v2.2/pci.ids
* � ��� ������ idsparser.jar ������ ������������� ����������� *.h-�����.
* ��������� ���������� - � ����� README.
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