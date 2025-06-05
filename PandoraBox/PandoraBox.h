#pragma once
#include <string>

class PandoraBox {
	public:
		PandoraBox();
		void CreateRegistryValues();
		void ShowPopupBox();

	private:
		std::wstring app_name = L"PandoraBox";
		std::wstring run_key_path = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
		std::wstring run_once_key_path = L"Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce";

		std::wstring exe_file_path;    // full path including filename

		bool AreValuesSet();
		void SetValueInRegistryKey(const std::wstring& registry_path, const std::wstring& value);
		bool CheckValueInKey(const std::wstring& path);
};
