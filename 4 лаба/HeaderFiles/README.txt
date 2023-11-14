Для формирования обновленных *.h-файлов:
1. Запустить update-pciids.bat для загрузки актуального файла pci.ids
или загрузить его самостоятельно с http://pci-ids.ucw.cz/v2.2/pci.ids
2. Для выполнения шага необходим Java Runtime Environment.
Запустить build-headers.bat или вручную в командной строке выполнить:
java -jar idsparser -input pci.ids -output pci.h -template input.tpl