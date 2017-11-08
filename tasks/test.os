#Использовать 1testrunner
#Использовать tool1cd

Процедура ПровестиТестирование()
	
	РасположениеTool1CD.ПутьTool1CD(ОбъединитьПути(ТекущийКаталог(), "bin", "Debug", "tool1cd.exe"));

	Тестер = Новый Тестер;

	КаталогПроекта = ОбъединитьПути(ТекущийСценарий().Каталог, "..");

	ФайлРезультатовТестовПакета = Новый Файл(ОбъединитьПути(КаталогПроекта,"test-reports"));
	Если Не ФайлРезультатовТестовПакета.Существует() Тогда
		СоздатьКаталог(ФайлРезультатовТестовПакета.ПолноеИмя);
	КонецЕслИ;
	
	КаталогТестов = Новый Файл(ОбъединитьПути(КаталогПроекта, "tests"));
	
	РезультатТестирования = Тестер.ТестироватьКаталог(КаталогТестов, ФайлРезультатовТестовПакета);

КонецПроцедуры

ПровестиТестирование();
