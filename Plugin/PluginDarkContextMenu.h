/*
  Copyright (C) 2019-2022 oZone
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

#pragma once

constexpr DWORD VER_1809 = 17763; // Windows 10 1809 (October 2018 Update)
constexpr DWORD VER_1903 = 18362; // Windows 10 1903 (May 2019 Update)

constexpr DWORD BUILD_WIN11 = 22000; // Windows 11 first "stable" build

struct Measure {
    HWND hWnd = nullptr;
    bool mode = false;
    bool force = false;
    bool tooltips = false;
    int countTips = -1;
    DWORD ownerProcessID = 0;
    std::vector<HWND> hTips;
    bool validVer = false;
    bool error = false;
};

enum class PreferredAppMode {
    Default,
    AllowDark,
    ForceDark,
    ForceLight,
    Max
};

inline bool IsAtLeastWin10Build(DWORD buildNumber);

void SetDarkMode(struct Measure* skin, HMODULE hUxtheme);
void SetTheme(struct Measure* measure);
void GetTooltips(struct Measure* measure);
inline void SetThemeForTooltips(struct Measure* measure);
inline void SetDefaultMeasureValues(struct Measure* measure);
