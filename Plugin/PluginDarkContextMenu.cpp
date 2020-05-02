/*
  Copyright (C) 2019-2020 oZone
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "StdAfx.h"
#include "PluginDarkContextMenu.h"

// Note: GetString, ExecuteBang and an unnamed function for use as a section variable
// have been commented out. Uncomment any functions as needed.
// For more information, see the SDK docs: https://docs.rainmeter.net/developers/plugin/cpp/

inline bool IsAtLeastWin10Build(DWORD buildNumber)
{
    if (!IsWindows10OrGreater()) {
        return false;
    }

    auto mask = VerSetConditionMask(0, VER_BUILDNUMBER, VER_GREATER_EQUAL);

    OSVERSIONINFOEXW osvi;
    osvi.dwOSVersionInfoSize = sizeof(osvi);
    osvi.dwBuildNumber = buildNumber;
    return VerifyVersionInfo(&osvi, VER_BUILDNUMBER, mask) != FALSE;
}

void SetDarkMode(struct Measure* skin, HMODULE hUxtheme)
{
    const auto ord135 = GetProcAddress(hUxtheme, MAKEINTRESOURCEA(135));
    auto useDarkMode = skin->mode;

    if (IsAtLeastWin10Build(VER_1903)) {
        using SPAM = PreferredAppMode (WINAPI*)(PreferredAppMode appMode);
        const auto _SetPreferredAppMode = reinterpret_cast<SPAM>(ord135);

        if (_SetPreferredAppMode != nullptr) {
            PreferredAppMode mode;
            if (skin->force) {
                mode = useDarkMode ? PreferredAppMode::ForceDark : PreferredAppMode::ForceLight;
            }
            else {
                mode = useDarkMode ? PreferredAppMode::AllowDark : PreferredAppMode::Default;
            }
            _SetPreferredAppMode(mode);
        }
    }
    else {
        using ADMFA = bool (WINAPI*)(bool allow);
        const auto _AllowDarkModeForApp = reinterpret_cast<ADMFA>(ord135);

        if (_AllowDarkModeForApp != nullptr) {
            _AllowDarkModeForApp(useDarkMode);
        }
    }
}

void SetTheme(struct Measure* measure)
{
    if (!measure->validVer || measure->error) {
        return;
    }

    const auto hUxtheme = LoadLibraryEx(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_USER_DIRS | LOAD_LIBRARY_SEARCH_SYSTEM32);

    if (hUxtheme == nullptr) {
        measure->error = true;
        return;
    }

    using ADMFW = bool (WINAPI*)(HWND hWnd, bool allow);
    using FMT = void (WINAPI*)();

    const auto _AllowDarkModeForWindow = reinterpret_cast<ADMFW>(GetProcAddress(hUxtheme, MAKEINTRESOURCEA(133)));
    const auto _FlushMenuThemes = reinterpret_cast<FMT>(GetProcAddress(hUxtheme, MAKEINTRESOURCEA(136)));

    if (_AllowDarkModeForWindow != nullptr && _FlushMenuThemes != nullptr) {
        _AllowDarkModeForWindow(measure->hWnd, measure->mode);
        SetDarkMode(measure, hUxtheme);
        _FlushMenuThemes();
        SetWindowTheme(measure->hWnd, measure->mode ? L"DarkMode_Explorer" : nullptr, nullptr);
    }

    FreeLibrary(hUxtheme);
}

void GetTooltips(struct Measure* measure)
{
    HWND hTooltip = nullptr;
    measure->countTips = 0;
    do {
        hTooltip = FindWindowEx(nullptr, hTooltip, nullptr, nullptr);
        DWORD checkProcessID = 0;
        GetWindowThreadProcessId(hTooltip, &checkProcessID);

        if (checkProcessID == measure->ownerProcessID) {
            WCHAR className[64] = { 0 };

            if (GetWindow(hTooltip, GW_OWNER) == measure->hWnd
                && GetClassName(hTooltip, className, _countof(className)) > 0
                && wcscmp(className, TOOLTIPS_CLASS) == 0)
            {
                measure->hTips.push_back(hTooltip);
                measure->countTips += 1;
            }
        }
    } while (hTooltip != nullptr);
}

void SetThemeForTooltips(struct Measure* measure)
{
    for (auto hTooltips : measure->hTips) {
        SetWindowTheme(hTooltips, L"DarkMode_Explorer", nullptr);
    }
}

void SetDefaultMeasureValues(struct Measure* measure)
{
    measure->mode = false;
    measure->force = false;
}

PLUGIN_EXPORT void Initialize(void** data, void* rm)
{
    auto measure = new Measure;
    *data = measure;

    if (!IsAtLeastWin10Build(VER_1809)) {
        RmLog(rm, LOG_WARNING, L"Wrong Windows version, need at least Windows 10 1809 (October 2018 Update, 10.0.17763)");
        return;
    }

    measure->validVer = true;
    measure->hWnd = RmGetSkinWindow(rm);
    measure->mode = RmReadInt(rm, L"DarkMode", 0) > 0;
    measure->force = RmReadInt(rm, L"Force", 0) > 0;
    measure->tooltips = RmReadInt(rm, L"Tooltips", 0) > 0;

    if (measure->tooltips) {
        if (measure->mode) {
            GetWindowThreadProcessId(measure->hWnd, &measure->ownerProcessID);
        }
    }
    else {
        SetTheme(measure);
    }

    if (measure->error) {
        RmLog(rm, LOG_ERROR, L"Could not load library uxtheme.dll.");
        SetDefaultMeasureValues(measure);
    }
}

PLUGIN_EXPORT void Reload(void* data, void* /*rm*/, double* /*maxValue*/)
{
    auto measure = static_cast<Measure*>(data);
    if (measure->countTips != -1 && measure->tooltips && measure->mode) {
        GetTooltips(measure);
    }
}

PLUGIN_EXPORT double Update(void* data)
{
    auto measure = static_cast<Measure*>(data);
    if (!measure->validVer || measure->error) {
        return -1.0;
    }

    if (measure->tooltips && measure->mode) {
        if (measure->countTips == -1) {
            GetTooltips(measure);
        }
        if (measure->countTips > 0) {
            SetThemeForTooltips(measure);
        }
        return static_cast<double>(measure->countTips);
    }

    return (measure->mode ? 1.0 : 0.0);
}

PLUGIN_EXPORT LPCWSTR GetString(void* data)
{
    auto measure = static_cast<Measure*>(data);
    if (!measure->validVer || measure->error) {
        return L"Error";
    }

    if (measure->tooltips) {
        return (measure->mode ? L"Dark Tooltips" : L"Light Tooltips");
    }

    return (measure->mode ? L"Dark" : L"Light");
}

//PLUGIN_EXPORT void ExecuteBang(void* data, LPCWSTR args)
//{
//	Measure* measure = static_cast<Measure*>(data);
//}

//PLUGIN_EXPORT LPCWSTR (void* data, const int argc, const WCHAR* argv[])
//{
//	Measure* measure = static_cast<Measure*>(data);
//	return nullptr;
//}

PLUGIN_EXPORT void Finalize(void* data)
{
    auto measure = static_cast<Measure*>(data);
    if (!measure->validVer) {
        RmLog(LOG_WARNING, L"Wrong Windows version, need at least Windows 10 1809 (October 2018 Update, 10.0.17763)");
    }
    else if (measure->error) {
        RmLog(LOG_ERROR, L"Could not load library uxtheme.dll.");
    }
    else if (!measure->tooltips) {
        SetDefaultMeasureValues(measure);
        SetTheme(measure);
    }
    delete measure;
}
