#include "ConRegistry.hpp"
#include "WinVersion.hpp"
#include "VTConverter.hpp"
#include "String.hpp"
#include <strsafe.h>

namespace MinConsoleNative
{
    //*************************************************************
    //
    //  RegDelnodeRecurse()
    //
    //  Purpose:    Deletes a registry key and all its subkeys / values.
    //
    //  Parameters: hKeyRoot    -   Root key
    //              lpSubKey    -   SubKey to delete
    //
    //  Return:     TRUE if successful.
    //              FALSE if an error occurs.
    //
    //*************************************************************
    bool RegDelnodeRecurse(HKEY hKeyRoot, LPTSTR lpSubKey)
    {
        LPTSTR lpEnd;
        LONG lResult;
        DWORD dwSize;
        TCHAR szName[MAX_PATH];
        HKEY hKey;
        FILETIME ftWrite;

        // First, see if we can delete the key without having
        // to recurse.

        lResult = RegDeleteKey(hKeyRoot, lpSubKey);

        if (lResult == ERROR_SUCCESS)
            return TRUE;

        lResult = RegOpenKeyEx(hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);

        if (lResult != ERROR_SUCCESS)
        {
            if (lResult == ERROR_FILE_NOT_FOUND)
            {
                //printf("Key not found.\n");
                return TRUE;
            }
            else
            {
                //printf("Error opening key.\n");
                return FALSE;
            }
        }

        // Check for an ending slash and add one if it is missing.

        lpEnd = lpSubKey + lstrlen(lpSubKey);

        if (*(lpEnd - 1) != TEXT('\\'))
        {
            *lpEnd = TEXT('\\');
            lpEnd++;
            *lpEnd = TEXT('\0');
        }

        // Enumerate the keys

        dwSize = MAX_PATH;
        lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
            NULL, NULL, &ftWrite);

        if (lResult == ERROR_SUCCESS)
        {
            do
            {

                *lpEnd = TEXT('\0');
                StringCchCat(lpSubKey, MAX_PATH * 2, szName);

                if (!RegDelnodeRecurse(hKeyRoot, lpSubKey))
                {
                    break;
                }

                dwSize = MAX_PATH;

                lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
                    NULL, NULL, &ftWrite);

            }
            while (lResult == ERROR_SUCCESS);
        }

        lpEnd--;
        *lpEnd = TEXT('\0');

        RegCloseKey(hKey);

        // Try again to delete the key.

        lResult = RegDeleteKey(hKeyRoot, lpSubKey);

        if (lResult == ERROR_SUCCESS)
            return TRUE;

        return FALSE;
    }

    //*************************************************************
    //
    //  RegDelnode()
    //
    //  Purpose:    Deletes a registry key and all its subkeys / values.
    //
    //  Parameters: hKeyRoot    -   Root key
    //              lpSubKey    -   SubKey to delete
    //
    //  Return:     TRUE if successful.
    //              FALSE if an error occurs.
    //
    //*************************************************************
    bool RegDelnode(HKEY hKeyRoot, LPCTSTR lpSubKey)
    {
        TCHAR szDelKey[MAX_PATH * 2];
        StringCchCopy(szDelKey, MAX_PATH * 2, lpSubKey);

        return RegDelnodeRecurse(hKeyRoot, szDelKey);
    }

    bool GetDWORDRegKey(HKEY hKey, const std::wstring& strValueName, DWORD& nValue, DWORD nDefaultValue)
    {
        nValue = nDefaultValue;
        DWORD dwBufferSize(sizeof(DWORD));
        DWORD nResult(0);
        LONG nError = ::RegQueryValueExW(hKey,
            strValueName.c_str(),
            0,
            NULL,
            reinterpret_cast<LPBYTE>(&nResult),
            &dwBufferSize);
        if (ERROR_SUCCESS == nError)
        {
            nValue = nResult;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool GetBoolRegKey(HKEY hKey, const std::wstring& strValueName, bool& bValue, bool bDefaultValue)
    {
        DWORD nDefValue((bDefaultValue) ? 1 : 0);
        DWORD nResult(nDefValue);
        LONG nError = GetDWORDRegKey(hKey, strValueName.c_str(), nResult, nDefValue);
        if (ERROR_SUCCESS == nError)
        {
            bValue = (nResult != 0) ? true : false;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool GetStringRegKey(HKEY hKey, const std::wstring& strValueName, std::wstring& strValue, const std::wstring& strDefaultValue)
    {
        strValue = strDefaultValue;
        WCHAR szBuffer[512];
        DWORD dwBufferSize = sizeof(szBuffer);
        ULONG nError = RegQueryValueExW(hKey, strValueName.c_str(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
        if (ERROR_SUCCESS == nError)
        {
            strValue = szBuffer;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool SetDWORDRegKey(HKEY hKey, const std::wstring& valueName, const DWORD data)
    {
        LSTATUS retCode = ::RegSetValueExW(
            hKey,
            valueName.c_str(),
            0, // reserved
            REG_DWORD,
            reinterpret_cast<const BYTE*>(&data),
            sizeof(data)
        );
        return retCode == ERROR_SUCCESS;
    }

    EXPORT_FUNC_EX(bool) MinIsLegacyConsole()
    {
        //Windows10以下都是旧版控制台
        if (!winVersion.IsWindows10OrLater())
        {
            return true;
        }

        HKEY key;
        RegOpenKeyExW(HKEY_CURRENT_USER, L"Console", 0, KEY_READ, &key);

        bool value;
        bool hasValue = GetBoolRegKey(key, L"ForceV2", value, false);

        //找到了ForceV2则返回相反结果
        if (hasValue)
        {
            return !value;
        }
        //如果找不到ForceV2则判断是否支持VT, 如果支持VT一定不是旧版控制台
        else
        {
            return !VTConverter::VTSupport();
        }
    }

    EXPORT_FUNC_EX(bool) MinEnableLegacyConsole(bool enable)
    {
        HKEY key;
        RegOpenKeyExW(HKEY_CURRENT_USER, L"Console", 0, KEY_READ | KEY_WRITE, &key);

        return SetDWORDRegKey(key, L"ForceV2", !enable);
    }

    EXPORT_FUNC_EX(bool) MinDeleteConsoleRegistry()
    {
        return RegDelnode(HKEY_CURRENT_USER, L"Console");
    }

    EXPORT_FUNC_EX(bool) MinWTIsDefaultConsole()
    {
        HKEY key;
        RegOpenKeyExW(HKEY_CURRENT_USER, L"Console\\%%Startup", 0, KEY_READ, &key);

        std::wstring v1;
        std::wstring v2;

        bool suc1 = GetStringRegKey(key, L"DelegationConsole", v1, L"");
        bool suc2 = GetStringRegKey(key, L"DelegationConsole", v2, L"");

        if (suc1 && suc2)
        {
            //即使有值也必须检查值, 不能为{00000000-0000-0000-0000-000000000000}
            if (!String::Compare(v1, L"{00000000-0000-0000-0000-000000000000}") && !String::Compare(v2, L"{00000000-0000-0000-0000-000000000000}"))
            {
                return true;
            }
        }
        return false;
    }

    bool ConRegistry::IsLegacyConsole()
    {
        return MinIsLegacyConsole();
    }

    bool ConRegistry::EnableLegacyConsole(bool enable)
    {
        return MinEnableLegacyConsole(enable);
    }

    bool ConRegistry::DeleteConsoleRegistry()
    {
        return MinDeleteConsoleRegistry();
    }

    bool ConRegistry::WTIsDefaultConsole()
    {
        return MinWTIsDefaultConsole();
    }
}