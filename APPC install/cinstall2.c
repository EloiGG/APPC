#include <stdlib.h>

int main()
{
	system("vcinstall.bat");
	system("@echo off");
	system("@setlocal enableextensions");
	system("@cd /d \"%~dp0\"");
	system("if not exist c:\\TEMP MD C:\\TEMP");
	system("copy /Y vcruntime140_1.dll C:\\TEMP");
	system("copy /Y \"C:\\TEMP\vcruntime140_1.dll\" \"C:\\Windows\\System32\\\"");
	system("del  c:\\TEMP\\vcruntime140_1.dll");

	return 0;
}