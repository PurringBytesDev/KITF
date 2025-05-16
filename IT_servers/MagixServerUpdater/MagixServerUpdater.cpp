#ifdef _WIN32
#include <windows.h> // Sleep and CreateProcess
#else
#include <unistd.h> // usleep
#include <cstdio>
#endif

void launchServer(LPSTR fileDir, LPSTR fileName)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;

	// Set up the start up info struct.
	memset(&si, 0,  sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	// Launch the child process.
	if (!CreateProcess(
		NULL,
		fileName,
		NULL, NULL,
		TRUE,
		CREATE_NEW_CONSOLE,
		NULL, fileDir,
		&si,
		&pi))
		return;

	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
}
int main()
{
	CopyFile("//115.124.111.20/ITserv$/MagixServer.exe","./MagixServer.exe",false);
	Sleep(10000);
	launchServer("./","MagixServer.exe");
}