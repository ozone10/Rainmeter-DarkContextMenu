# Rainmeter-DarkContextMenu

[![Build status](https://img.shields.io/github/workflow/status/ozone10/Rainmeter-DarkContextMenu/Build/master?logo=Github)](https://github.com/ozone10/Rainmeter-DarkContextMenu)
[![Build status](https://img.shields.io/appveyor/ci/ozone10/Rainmeter-DarkContextMenu/master?logo=Appveyor)](https://ci.appveyor.com/project/ozone10/rainmeter-darkcontextmenu/branch/master)
[![Codacy Badge](https://img.shields.io/codacy/grade/ab209215919e4b2ca6b87b1a81a83b41?logo=Codacy)](https://www.codacy.com/manual/ozone10/Rainmeter-DarkContextMenu?utm_source=github.com&utm_medium=referral&utm_content=ozone10/Rainmeter-DarkContextMenu&utm_campaign=Badge_Grade)
[![Latest release](https://img.shields.io/github/v/release/ozone10/Rainmeter-DarkContextMenu?include_prereleases)](https://github.com/ozone10/Rainmeter-DarkContextMenu/releases/latest)
[![Total downloads](https://img.shields.io/github/downloads/ozone10/Rainmeter-DarkContextMenu/total.svg)](https://github.com/ozone10/Rainmeter-DarkContextMenu/releases)
[![Licence](https://img.shields.io/github/license/ozone10/Rainmeter-DarkContextMenu?color=9cf)](https://www.gnu.org/licenses/gpl-3.0.en.html)

[Rainmeter](https://www.rainmeter.net) plugin that allows to use Windows 10 dark theme for every context menu in Rainmeter (not only skins but also Rainmeter program itself) and on skin tooltips.

Minimal requirements are Windows 10 build 10.0.17763, 1809 (October 2018 update), and enabled dark theme for apps.  
Since Windows 10 build 10.0.18362, 1903 (May 2019 update) it is possible to use dark theme ignoring system wide theme for apps.

For more information, check the [Rainmeter forum](https://forum.rainmeter.net/viewtopic.php?f=128&t=33028).

Check my other Rainmeter plugin [TranslucentRM](https://github.com/ozone10/Rainmeter-TranslucentRM).

* * *

<p align="center">
  <img src="https://i.imgur.com/3gbFxfI.png">
  <br><br>
  <img src="https://i.imgur.com/SIAmcZY.png">
</p>

* * *

## Options

- **DarkMode** - Option to choose theme.

  - Value **0** - light theme, default value.
  - Value **1** - dark theme.

- **Force** - Option to force theme.
    Require at least Windows 10 build 10.0.18362, 1903 (May 2019 update).

  - Value **0** - rely on system wide theme for apps, default value.
  - Value **1** - force to use selected theme.

- **Tooltips** - Option to use theme on skin tooltips.
  - Value **0** - use theme on every Rainmeter context menu, default value.
  - Value **1** - use theme on skin tooltips, will ignore `Force` option.

* * *

## Examples

- **Example 1:**
    Always use dark theme.

```ini
[DarkMode]
Measure=Plugin
Plugin=DarkContextMenu
DarkMode=1
Force=1
```

- **Example 2:**
    Use theme based on system wide theme for apps.  
    Need to restart Rainmeter, if system wide theme for apps is changed.

```ini
[DarkMode]
Measure=Plugin
Plugin=DarkContextMenu
DarkMode=1
Force=0
```

- **Example 3:**
    Use dark theme on current skin tooltips.  

```ini
[DarkTooltips]
Measure=Plugin
Plugin=DarkContextMenu
DarkMode=1
Tooltips=1
```
