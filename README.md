# Auto-Tv-Out


<p align="center"><img src="./assets/logo.png" width="600"/></p>

A PSP plugin that automatically enables TVout when you have a AV cable connected. This plugin will ensure that TVOut remains enabled should it get disabled for any reason.

**If you use the PSPlaceable please see the PSPlaceable section below.**

## Demo from a cold boot


https://github.com/user-attachments/assets/0f46cb5a-038d-47bd-9841-010a28d38b1c



## Demo of connecting and re-connecting the cable



https://github.com/user-attachments/assets/83404e08-234a-49e7-ad44-3fb0e51ed5fc



---

## Installation

* Download `auto_tvout.prx` from the [Latest Release](https://github.com/ste2425/Auto-Tv-Out/releases/latest) and place it in `/SEPLUGINS`
* Update the `PLUGINS.TXT` file and add the following lines to the bottom
  * `pops, ms0:/seplugins/auto_tvout.prx, 1`
  * `vsh, ms0:/seplugins/auto_tvout.prx, 1`
  * `game, ms0:/seplugins/auto_tvout.prx, 1`
* Save then safely unmount the PSP.

## Why

I am working on a project [PSP Bluetooth Controller's](https://github.com/ste2425/PSP-Bluetooth-Plugin) which eventually will involve a dock you connect the PSP to.

One of the main goals is that when connected to the dock TVOut should automatically be enabled. I realized other consolizer builds would benefit from this too so turned it into its own plugin.

Historically consolizers would rely on holding the display button until TVOut is enabled. However should it get disabled for any reason it will not be re-enabled. This plugin fixes that.

## Known Working Environments

This plugin does *not* have a specific CFW requirement. 

I have tested on the following combinations of system. Obviously `1000` model does not support TVOut but i have tested on mine to verify it doesn't cause the console to crash.

| CFW | OFW | Motherboard Version |
| -- | -- | -- |
| LME-2.3 | 6.61 | TA-081v2 (01g) |
| ARK-4 | 6.61 | TA-081v2 (01g) |
| ARK-5 | 6.61 | TA-081v2 (01g) |
| ARK-5 | 6.61 | TA-088v3 (02g) |
| ARK-5 | 6.60 | TA-088v3 (02g) |
| ARK-4 | 6.60 | TA-088v3 (02g) |
| ARK-4 | 6.60 | TA-095v1 (09g) |

## PSPlaceable

If you have a PSPlaceable it also attempts to set the TVOut by holding the display button. They should both work together but it would be best to update to version `2.4.0`. This adds a new configuration setting on its `settings` page to stop the PSPlaceable enabling TVOut. Leaving it up to the plugin.

Web app: https://ste2425.github.io/PSP-Bluetooth/

I recommend using the plugin and disabling it in the Web App.

## How It Works

I have tried in the past to solve this problem making direct DVE calls. I got something partially working. However it had graphical errors and didn't have the standard state management. If you removed the AVCable or pressed the display button the LCD does not re-enable.

Instead i changed tacts and get my `impose` PRX into AI and told it `Tell me what happens when the Display button is pressed`. It essentially saying `a flag is set, a background process checks that flag and compares it against a tick counter. When the counter reaches a threshold it calls internal method XYZ`

So i asked for the offset of that internal method and the first few MIPS commands it executes and built a plugin to find the method based on the offset. I then verified the MIPS commands to be sure i had the correct method.

Lastly i extracted all the variants of the `impose` PRX from the 6.61 and 6.60 updaters to verify the offsets are the same and AI said yes, so who am i to argue.

# Acknowledgements

* PSP Homebrew Discord community. Without their help i would never have been able to achieve anything on the PSP.

# AI usage

Im not getting into the AI debate but i know some have strong opinions on th subject.

So to be clear AI did not write this plugin, i did. I have used AI tooling as stated below.

* Making the logo, im not a designer. If a human would like to make a logo for this project i would love to use that instead.
* Reversing the `Impose` prx. 
