#include <Windows.h>
#include "DataManager.h"
#include "Func.h"
#include "Book.h"

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Library lubrary;
	while (FileMenu(lubrary)) {};
	return 0;
}