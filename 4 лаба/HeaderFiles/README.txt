��� ������������ ����������� *.h-������:
1. ��������� update-pciids.bat ��� �������� ����������� ����� pci.ids
��� ��������� ��� �������������� � http://pci-ids.ucw.cz/v2.2/pci.ids
2. ��� ���������� ���� ��������� Java Runtime Environment.
��������� build-headers.bat ��� ������� � ��������� ������ ���������:
java -jar idsparser -input pci.ids -output pci.h -template input.tpl