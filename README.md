# Auto-Tv-Out


<p align="center"><img src="./assets/logo.png" width="600"/></p>

A PSP plugin that automatically enables TVout when you have a AV cable connected. It will monitor that TVOut i enabled and should it be disabled (such as Homebrew triggering a reboot) it will be re-enabled.

## Demo from a cold boot


## Demo of connecting and re-connecting the cable


---

## Installation

* Download `auto_tvout.prx` from the [Latest Release](https://github.com/ste2425/Auto-Tv-Out/releases/latest) and place it in `/SEPLUGINS`
* Update the `PLUGINS.TXT` file and add the following lines to the bottom
  * `pops, ms0:/seplugins/auto_tvout.prx, 1`
  * `vsh, ms0:/seplugins/auto_tvout.prx, 1`
  * `game, ms0:/seplugins/auto_tvout.prx, 1`
* Save then safely unmount the PSP.

## Why

Part of my [PSP Bluetooth Controller](https://github.com/ste2425/PSP-Bluetooth-Plugin) i neeed a way to enable TVOut when the console is connected to a dock i will be designing. I realised this would be useful for anyone building Cosolizers such as the PSPlaceable and it offers a better solution.

Currently you have two options which have their flaws.

### Mess with DVE methods

Manually triggering DVE methods to enable TVOut. The downside is its complexity, you need to deal with video modes, resolutions and managing state. 

### Trigger screen button press
You coulg triger a screen button press. This is what the PSPlaceable does, but it is slow. From a cold boot you need to wait 7 seconds or so before the console recognises the button, then hold for a further 5 seconds before TVOut is enabled.

This plugin is faster and more reliable, should TVOut be disabled for any reason this plugin will automatically re-enable it.

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

## How It Works

The `sceImpose` PRX manages the button screen button. I fed the plugin into AI asking it to figure out what happens with that button. It found that the handler for that button simply sets a flag and there is a background process monitoring the flag. This process calls a internal method after a given timeout.

So i find that internal method ait its specific offset and call it manually.

# Acknowledgements

* PSP Homebrew Discord community. Without their help i would never have been able to achieve anything on the PSP.

# AI usage

Im not getting into the AI debate but i know some have strong opinions on th subject.

So to be clear AI did not write this plugin, i did. I have used AI tooling as stated below.

* Making the logo, im not a designer. If a human would like to make a logo for this project i would love to use that instead.
* Reversing the `Impose` prx. I gave it the plugin and asked it to tell me the logic around the display button. I then wrote the plugin based on what it told me.
