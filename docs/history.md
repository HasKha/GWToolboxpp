---
layout: default
---

# Version History
Previous releases are provided as dll files. In order to use it, you need to use a different launcher which must be placed in the same directory as the dll. If you are looking for the latest version, go to the [Home Page](./) instead.

[AutoIt Launcher](https://raw.githubusercontent.com/HasKha/GWToolboxpp/master/AutoitLauncher/Inject.au3) - Source, requires AutoIt3. Right click -> Save link as...

## Version 3.42
* [Fix] Reverted custom markers appearing in outposts. Fault Misty.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/3.42_Release/GWToolbox.dll)

## Version 3.4
* [New] Changed all color inputs to use a slightly different control; now able to click on the color swatch to choose instead of inputting RGBA manually
* [New] Added available dialog IDs to dialog section of info window when talking to an NPC
* [New] Added color thresholds to health widget and distance widget
* [New] Modified/tidied up "Settings" window to make stuff easier to find, see following changes:
* [Minor] Merged "Chat Filter" section into "Chat Settings" section
* [Minor] Merged "Inventory Management" section into "Inventory Settings" section
* [Minor] Merged "Toolbox" section into "Toolbox Settings" section
* [Minor] Merged "Party Window" section into "Party Settings" section
* [Minor] Moved party related function from "Game Settings" into "Party Settings"
* [Minor] Copied lunar and alcohol related settings from "Pcons" into "Game Settings"
* [Minor] Moved chat related options from "Game Settings" into "Chat Settings"
* [Minor] Merged "Discord" and "Twitch" sections into "Third Party Integration" section
* [Minor] Alphabetised all window and widget sections inside "Settings" window
* [Minor] Alphabetised all module checkboxes inside "Toolbox Settings" section, set into columns to better use space
* [Minor] Moved inventory related options from "Game Settings" into "Inventory Settings"
* [Minor] Added chat filter checkbox for "Not enough energy/adrenaline" messages
* [Fix] Fixed bug preventing "show enable" pcons button to load setting from file
* [Fix] Fixed bug preventing custom minimap markers in outposts
* [Fix] Fixed potential crash when cancelling salvage process
* [Fix] Fixed trophies not appearing as salvage options
* [Fix] Fixed bug preventing hero builds being sent to chat in explorable areas
* [Fix] Fixed a bug causing alcohol to be spammed when the alcohol widget is disabled in toolbox

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/3.4_Release/GWToolbox.dll)

## Version 3.2
* [New] Added DirectX distributable check inside GWToolbox at runtime; will now display a message with a link to the DirectX download page instead of just not launching at all.
* [New] Added option to hide bond monitor in outpost (default showing)
* [New] Added option to save objective timer runs to disk (default save)
* [New] Added option to hide date/time of objective timer runs (default hidden)
* [New] Added option to hide runs from previous days (default hidden)
* [Minor] Don't maintain Trade Chat websocket connection when the window is collapsed
* [Minor] Objective timer runs now save to `%localappdata%/GWToolboxpp/runs` to avoid cluttering the main folder
* [Minor] Added option to only use superior salvage kits when using `/salvage` command
* [Minor] Added on-hover item descriptions to "Salvage all?" dialog
* [Minor] Removed extra new line when `/tp` error message is displayed in chat
* [Minor] Fixed a display bug in minimap aor effect colors showing as RGBA instead of ARGB
* [Minor] Don't print target hotkey to emote chat by default
* [Minor] Removed option to swap functionality of Ctrl + Click on minimap; caused issues when shift was held
* [Minor] Disable Pcons on map change set to off by default
* [Minor] Pcon refillers hidden by default
* [Minor] Added option to hide city pcons in explorable areas (default visible)
* [Minor] Removed DoA snakes from default party window NPCs
* [Fix] Re-added in-game option to manually check for updates
* [Fix] Fixed `auto age2 on /age` setting being the same as `auto /age2 on vanquish` setting
* [Fix] Fixed bug causing salvage all process to only salvage the first item in a stack
* [Fix] Fixed bug causing identify blue/purple/gold to identify other rarities
* [Fix] Fixed bug causing salvage all crash when inventory is full
* [Fix] Fixed bug causing salvage all to identify unsalvagable trophies as salvagable
* [Fix] Fixed potential crash when displaying the "Salvage all?" dialog
* [Fix] Fixed crash when adding a new custom minimap agent whilst the matching NPC is within range.
* [Fix] Fixed IRC timeout after 3 minutes when connected to Twitch

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/3.2_Release/GWToolbox.dll)

## Version 3.1
* [Fix] Fix bug where the launcher would ask everytime to download the toolbox.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/3.1_Release/GWToolbox.dll)

## Version 3.0
* [New] Merged [3vcloud](https://github.com/3vcloud) fork and added him as maintainer to GWToolbox. The following changes are made to GWToolbox.
* [New] `/loadbuild` command added, allows player to load builds from the GWToolbox Builds window by name or build code. See help section of GWToolbox in-game for more info.
* [New] `/pingitem` command added, allows player to ping any currently equipped item. See help section of GWToolbox in-game for more info.
* [New] `/transmo` command updated to allow players to transform into a list of pre-defined NPCs by name instead of just the one target. "/transmo reset" will remove the current NPC skin on the player. See help section of GWToolbox in-game for more info.
* [New] `/transmoparty` command added, similar syntax to `/transmo`.
* [New] `/transmotarget` command added, similar syntax to `/transmo`.
* [New] `/dance` adds Collector's Edition glow to player hands.
* [New] `/dancenew` on Factions/NF professions shows Collector's Edition dance emote instead of standard one.
* [New] Added "Check for Updates" button to Toolbox Settings section. If enabled, it will auto-check every 10 minutes instead of just the first load.
* [New] Added "Emotes" and "Other" to Chat Colors section of Game Settings.
* [New] Added "show alias on whisper" to Friend List window, adds player alias in brackets when sending or receiving a whisper (default false).
* [New] Added "You gain x faction" chat filter option.
* [New] Added `/addfriend <character_name>` and `/removefriend <character_name|alias>` chat commands.
* [New] Added `/online`, `/away`, `/busy`, `/dnd`, `/offline` chat commands - see Help section for details.
* [New] Added `/target hos` and `/target ee` commands, see help section for info.
* [New] Added 24h Deep module - use /deep24h command to toggle.
* [New] Added `/identify all`, `/identify blue`, `/identify purple`, `/identify gold` chat commands.
* [New] Added `/pc <search_term>` chat command to do a price check from chat.
* [New] Added `/salvage all`, `/salvage blue`, `/salvage purple` chat commands.
* [New] Added ability to view a player's guild info via Target section of Info Window.
* [New] Added AoE circles for Maelstrom, Chaos storm, Lava font, Savannah heat, Breath of fire, Bed of coals, Churning earth.
* [New] Added auto-accept party invites when ticked option for faster party reorder.
* [New] Added auto-accept party join requests when ticked option for faster party reorder.
* [New] Added boss by profession color to Minimap.
* [New] Added challenge mission chat filter option.
* [New] Added ctrl + click context menu to Salvage and ID kits - allows user to salvage/identify groups of items at once.
* [New] Added Flame Trap, Spike Trap and Barbed Trap to aoe effects on minimap.
* [New] Added option in Game Settings to change Guild Wars window title to name of current character.
* [New] Added option to add player number to player names in explorable areas.
* [New] Added option to hide bonds, health, party damage and instance timer widgets in outpost.
* [New] Added option to show hidden NPCs on Minimap.
* [New] Added option to skip entering character name when donating faction.
* [New] Added option to trigger a hotkey on entering explorable/outpost.
* [New] Added pcons per character option, default turned on.
* [New] Added pcons to GWToolbox Builds window; pcons can now be assigned to a particular build and auto-enabled when loaded.
* [New] Added seconds to the clock widget.
* [New] Added spirit timers to Timer widget; default is EoE and QZ. Other spirit timers available in settings.
* [New] Automatic `/age` on vanquish option.
* [New] Automatic `/age2` on `/age` option.
* [New] Better crash handling; more GWToolbox related crashes will be caught and dumped in the GWToolboxpp folder, and will show a message on-screen.
* [New] Ctrl + enter to whisper targeted player.
* [New] Friend List window/module added.
* [New] Modified trade alerts; added option to only show trade alerts in chat when in Kamadan ae1.
* [New] Objective timer now saves runs to disk in json format, and auto-collapses previous runs when a new one starts.
* [New] Support for Chinese character set; add "Font_ChineseTraditional.ttf" to GWToolboxpp folder to load automatically.
* [New] Support for Japanese character set; add "Font_Japanese.ttf" to GWToolboxpp folder to load automatically.
* [New] Support for Korean character set; add "Font_Korean.ttf" to GWToolboxpp folder to load automatically.
* [New] Support for Russian character set; add "Font_Cyrillic.ttf" to GWToolboxpp folder to load automatically.
* [New] Trade Window changes: removed dependency on kamadan.decltype.org - players can now use this window as normal when in Kamadan ae1 even if decltype.org stops working.
* [New] Trade window now uses [https://kamadan.gwtoolbox.com](https://kamadan.gwtoolbox.com) for incoming trade chat. Rebased this module to be similar to vanilla.
* [New] Updater module re-enabled; will now give update notifications for this version of toolbox.
* [Minor] `/tb <window_name>` now matches partial window names.
* [Minor] `/tp gh` and `/gh` now check whether you're already in GH before leaving/entering as applicable.
* [Minor] Added "morah" and "surmia" `/tp` commands.
* [Minor] Added client-side checks before trying to travel e.g. same district, to avoid the need for packet sending.
* [Minor] Added current online status to top of friend list.
* [Minor] Added daily quest chat command help section.
* [Minor] Added date/time to headers in Objective timer, and [Failed] to identify failed runs.
* [Minor] Added debug info about current camera position to info window.
* [Minor] Added option to hide friends list when in outpost/explorable.
* [Minor] Added option to only show alcohol widget when using alcohol.
* [Minor] Added option to toggle ctrl + click functionality on minimap. (e.g. ctrl + click to ping, click to target)
* [Minor] Changed `/pingitem` and `/armor` descriptions to include armor rating, infused and +1 stacking attribute if applicable.
* [Minor] Fixed `/tp` errors not always displaying on-screen.
* [Minor] Fixed a bug causing some trade messages to be blocked in Kamadan when filtered using the trade window.
* [Minor] Friend list is now amended when saved, not overwritten. This allows multiple accounts to maintain details about mutual friends.
* [Minor] Hide `/deep24h` setting from settings window when not enabled.
* [Minor] Performance tweaks and housekeeping for chat filters.
* [Minor] Performance tweaks to resign log and trade window, removed some redundant code blocks.
* [Minor] Remove "I'm wielding" text when pinging weapons with GWToolbox.
* [Minor] Removed "n platinum, n gold" from pinged item descriptions.
* [Minor] Removed armor descriptions when using `/pingitem` command. Will add headpiece +1 info in a later release.
* [Minor] Removed threading code from Friend list, a bit more stable now.
* [Minor] Removed threading code with refilling pcons, a bit more stable now.
* [Minor] Reverted Trade Window to vanilla 2.27. Tweaks to layout widths, fixed bug when trying to whisper player's with unicode chars in their name.
* [Minor] Stability changes and shutdown fixes.
* [Minor] Stability/failover changes for Updater module. Will update current dll wherever it is on disk, not just from GWToolboxpp folder.
* [Fix] Adjusted AoE range for churning earth from adjacent to nearby.
* [Fix] Fixed a crash when trying to click on Minimap whilst observing PvP matches.
* [Fix] Fixed an issue that didn't do a fresh reload from ini file when clicking "Load Now" in tb settings.
* [Fix] Fixed issues with loading and saving to file.
* [Fix] Fixed trade alerts by regex not working.
* [Fix] Fixes resign log not working when player numbers are turned on in an explorable area.
* [Fix] Reduced flashing GW window spam when being invited to a party.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/3.0_Release/GWToolbox.dll)

## Version 2.35
* [Fix] Fixes following April 22 update.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.35_Release/GWToolbox.dll)

## Version 2.34
* [Fix] Fix crashes with enemies count in info window.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.34_Release/GWToolbox.dll)

## Version 2.33
* [Fix] Fix crashes when trying to drop gold coins.
* [Fix] Fix a long due bug that caused random crashes, often when using the minimap.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.33_Release/GWToolbox.dll)

## Version 2.32
* [Fix] Fix crashes caused because of bad header values.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.32_Release/GWToolbox.dll)

## Version 2.31
* [Fix] Fixes following February 14 update.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.31_Release/GWToolbox.dll)

## Version 2.30
* [Fix] Fix bug with launcher asking to download every time.
* [Fix] Main window options are now accessible again.
* [Fix] Fixed crash dump generation.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.30_Release/GWToolbox.dll)

## Version 2.29
* [New] Added Discord 'Rich Presence' integration.
* [Fix] Fixes following December 13th update.
* [Fix] Pcons are not used in cinematic anymore.
* [Minor] Minor improvements to `/tp <town name>` command.
* [Minor] Guild Wars will ask permission to travel when in a group with other players.
* [Minor] Vanquish Widget can now be ctrl+clicked to print the vanquish information to group chat.
* [Minor] Now, '/resignlog' only prints name of players who didn't resigned yet.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.29_Release/GWToolbox.dll)

## Version 2.27
* [New] Added support for adrenaline skill with /useskill command.
* [New] Added /resignlog command.
* [New] You can now use the command /tp and /to with the prefix of an outpost to teleport to the outpost.
* [Fix] Reduced the connection spams to kamadan.decltype.org when the site is down.
* [Fix] Fixed bug when game process name wasn't "Gw.exe".
* [Fix] Fixed bug related to player status introduced in recent game update.
* [Minor] Several improvements in GWCA and updated 3rd party libraries.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.27_Release/GWToolbox.dll)

## Version 2.26
* [Fix] Fixed bug with friend status message.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.26_Release/GWToolbox.dll)

## Version 2.25
* [New] Added option to disable minimap mouse capture in outpost.
* [Fix] Fixed bugs related to GW June 12th update.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.25_Release/GWToolbox.dll)

## Version 2.24
* [Fix] Fixed several crash bugs related to GW May 1st update. 
* [Note] Toolbox borderless functionality is current disabled. It might be fixed or removed in the future. For now, please use the in-game borderless that you can find in GW Options -> Graphics -> Resolution.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.24_Release/GWToolbox.dll)

## Version 2.23
* [New] Added an option to disable the prompt when selling green and gold items.
* [New] Added an option to show a notification when a friend comes online or goes offline.
* [Fix] Fixed ctrl+click with new storage panels.
* [Fix] Fixed bug when selling rare materials with a quantity smaller than 10.
* [Minor] Extended `/flag` command to flag at a given coordinate.
* [Minor] Added more bosses to the minimap.
* [Minor] Added map ids of the event versions of Kamadan to disable the trade chat.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.23_Release/GWToolbox.dll)

## Version 2.22.1
* [Fix] Material buying and auto-manage gold

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.22.1_Release/GWToolbox.dll)

## Version 2.22
* [Fix] General fixes for the February 5th update.
* [Fix] Pcons not finding proper quantities
* [Fix] Ctrl+Click storage/inventory item quick-move

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.22_Release/GWToolbox.dll)

## Version 2.21
* [Fix] Added Year of the Pig lunars to lunar pop list.
* [New] Added an option to ctrl+click on target health (widget) to print it.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.21_Release/GWToolbox.dll)


## Version 2.20
* [Fix] Fixed a bug with the bond monitor.
* [New] The old `click to use` option in bond monitor is now split into `click to cast` and `click to drop`.
* [New] `Item not found` message will now show in the Emotes chat channel.
* [New] `/afk` will now remove the message set by `/afk message`.
* [New] Added an option to not set normal or hard mode when loading hero builds.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.20_Release/GWToolbox.dll)

## Version 2.19
* [New] Reapply Title hotkey now applies the appropriate title for the area, not just Lightbringer title.
* [New] Added an option to allow mouse clickthrough in the minimap.
* [New] Added an optional timer for dungeon traps.
* [Fix] Fixed a bug where toolbox would consider "Alt" key down after alt-tabbing.
* [Fix] Kegs now are correctly counted in the alcohol counter.
* [Fix] Quest marker animation now adapts to the framerate.
* [Fix] Removed Deep and Urgoz travel buttons, as they are not longer possible.
* [Fix] Fixed bug with bond monitor where it would not update to party size.
* [Fix] Fixed bug where Jora and Keiran were inversed in hero builds.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.19_Release/GWToolbox.dll)

## Version 2.18
* [New] `/tb`, `/hide` and `/show` do not require quotes for windows with multiple names. 
* [New] Added an option (in Game Settings) to only show non-learned skills when using a tome.
* [New] Added an afk message to `/afk <message>`. Toolbox will automatically reply with that message if you get PMd while afk.
* [New] Toolbox will now not render during cinematics.
* [New] Added an option to automatically skip cinematics.
* [New] Added an option to alt+click on the minimap to move to that location.
* [New] Added an option to reduce the visual impact in the minimap when agents get spam pinged.
* [New] Added an optional timer for Deep Aspects. You can enable it in Timer settings.
* [Fix] Fixed a bug where the bond monitor would not properly update the party size.
* [Fix] Fixed a bug where toolbox could only control 7 heroes.
* [Fix] Fixed the Dhuum start trigger in the Objective Timer.
* [Fix] Fixed bug where Recall line would not know where to go after your Recall target despawned.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.18_Release/GWToolbox.dll)

## Version 2.17
* [New] Options to change game messages color. (Under GameSettings)
* [Fix] The "x" in Objective Timer will now delete the record.
* [Fix] Crash bug when zoning into an explorable.
* [Fix] Bug were Materials window wouldn't save his settings.
* [Minor] You can now choose which column to show in Objective Timer.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.17_Release/GWToolbox.dll)

## Version 2.16
* [New] Objective timer for DoA, UW, FoW
* [New] Option to automatically withdraw and deposit gold while buying materials. (disable by default)
* [New] Option to ctrl+click on instance timer to `/age`. (disable by default)
* [New] Command `/resize width height` to resize Guild Wars window. (Only works with borderless)
* [Fix] Bug were GWToolbox UI was reseted when opening Guild Wars.
* [Fix] Materials window won't stop buying materials anymore.
* [Fix] Bug with "Tick is a toggle" that required a restart to work.
* [Fix] Bug where toolbox wouldn't get input if UI was disable.
* [Fix] "All flag" will be shown even if you only have henchmans in your group.
* [Minor] Chat timestamps will work for older messages. (pre-inject)
* [Minor] The real name of mercenary hero will be used in hero builds instead of "Mercenary Hero X".

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.16_Release/GWToolbox.dll)

## Version 2.15
* [Fix] Fixed toolbox functionality after June 07 2018 game update.
* [New] GWToolbox will hide itself if you hide the game user interface. Will also work with "Shift+Print Screen".
* [New] You can now send whisper with hotkeys. In the message text field use the format: ```<target name>,<message>```.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.15_Release/GWToolbox.dll)

## Version 2.14
* [Fix] Fixed toolbox functionality after May 9 2018 game update.
* [Fix] Fixed bug with ctrl+click & dyes.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.14_Release/GWToolbox.dll)

## Version 2.13
* [Fix] Fixed toolbox functionality after May 9 2018 game update.
* [New] The damage report will now print the names of the npcs.
* [Minor] You can now use iso alpha 2 country codes for district with the command `/tp`.
* [Minor] You can now target player with the command `/target "Player Name"`. (with quotes)

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.13_Release/GWToolbox.dll)

## Version 2.12.1
* [Fix] Removed Borderless Windowed mode, use official implementation instead :)
* [Fix] Fixed crash bug on toolbox start.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.12.1_Release/GWToolbox.dll)

## Version 2.12
* [Fix] Fixed toolbox functionality after April 30 2018 game update.
* [New] Added an option in the chat filter to match messages with regular expressions.
* [New] Improved custom agent rendering functionality. 
* [Fix] Empty lines in the chat filter will not ignore everything any more.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.12_Release/GWToolbox.dll)

## Version 2.11
* [New] Added the option to set custom color for specific agents in the minimap. 
* [Fix] Fixed toolbox functionality after April 23 2018 game update.
* [Fix] Improved overall robustness.
* [Fix] Improved ctrl+click (move items) behaviors.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.11_Release/GWToolbox.dll)

## Version 2.10
* [Fix] Fixed toolbox functionality after April 2 2018 game update.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.10_Release/GWToolbox.dll)

## Version 2.9
* [Fix] Fixed toolbox functionality after March 23 2018 game update.
* [New] Added a new Trade chat window containing Kamadan Trade Chat. Powered by `https://kamadan.decltype.org/`.
* [New] Added a Soul Tormentor counter to the Info window. 
* [New] Improved camera unlock movement functionality and added an option to keep camera height fixed while moving.
* [New] Minimap targeting (Control+Click) now ignores all but the targetable minipets.
* [New] Added an option to move items to/from chest by Control+click.
* [New] Added an option to maintain `/cam fov` after a map change.
* [New] Each `/useskill` will now reset the skills to use, not add to the previous ones.
* [Fix] Fixed `/flag` functionality. Added `/flag all` and `/flag clear`. 
* [Fix] Fixed a bug where hero flags would not show in the minimap. Reduced size of the circle of individual hero flag. 
* [Fix] Fixed a bug where settings would not be loaded/saved properly on systems with a username containing non-standard characters.
* [Fix] Fixed a bug where pcons would sometimes not be used if the player had no other effects. 
* [Fix] Fixed a crash bug when sending Hero Builds to <No Hero>.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.9_Release/GWToolbox.dll)

## Version 2.8
* [Fix] Fixed a crash bug in the hero build window. 

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.8_Release/GWToolbox.dll)

## Version 2.7
* [New] Can now omit the town in `/tp [town] [district] to change district. 
* [Fix] Fixed the "Close other windows..." option.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.7_Release/GWToolbox.dll)

## Version 2.6
* [New] You can now add or remove any window to the main toolbox window - check Settings -> Toolbox Settings. 
* [New] Added a new Window to create and load hero team builds.
* [New] Added support for the new Lunars (year of the dog). 
* [New] Added cursor fix: mouse should no longer jump between monitors when right clicking. 
* [New] You can now use multiple skills with `/useskill`.
* [New] Added rings of fortune messages to the chat filter. 
* [Fix] Alcohol monitor is now only visible in explorable areas.
* [Fix] You can now once again set up hotkeys on mouse buttons x1 and x2 (forward and back).
* [Fix] Fixed various small issues. 

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.6_Release/GWToolbox.dll)

## Version 2.5
* [Fix] Fixed bug where toolbox window positions would reset when restoring from an alt-tabbed fullscreen GW.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.5_Release/GWToolbox.dll)

## Version 2.4
* [Fix] Fixed bug where GW would freeze when launching Toolbox while in fullscreen mode.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.4_Release/GWToolbox.dll)

## Version 2.3
* [New] Added `/load <build name>` command to load builds from your saved templates.
* [New] Added an option to display chat message timestamps.
* [New] Added an option to retain chat history after changing map.
* [New] Added an option to restore GW window from mimized state and bring to focus when zoning into explorables.
* [New] Added an alcohol duration widget. 
* [Fix] Fixed several minor bugs.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.3_Release/GWToolbox.dll)

## Version 2.2
* [New] You can now /tp gh [tag] to any guild in your alliance.
* [New] Added options to make Guild Wars flash in the taskbar when invited to a party or when your party zones.
* [New] Added options to automatically set away after a delay and/or online when back.
* [New] "Use item" hotkey can now use items from the chest in outposts.
* [Fix] VQ counter now only appears in areas that can be vanquished.
* [Fix] /tp fav now starts counting at 1 instead of 0.
* [Fix] Automatically changing URLs to templates now also works in PMs.
* [Fix] Fixed bug where hotkeys would not save properly.
* [Fix] Quest marker will now appear correctly on the minimap.
* [Fix] Fixed several typos and minor issues.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.2_Release/GWToolbox.dll)

## Version 2.1
In this patch we change the update server to GitHub and fix minor bugs from 2.0 and add a Vanquish counter.

* [New] Toolbox can now self-update (or not). Check the options in Settings->Toolbox Settings
* [New] Added a vanquish counter
* [New] /scwiki now accepts parameters, (e.g. /scwiki doa)
* [New] you can now /tp fav to any number of favorites (e.g. /tp fav5).
* [New] Added an option to flash your guild wars taskbar when receiving a private message
* [New] Added an option to automatically add [;xx] to links you write in chat, but only if they are in their own message!
* [Fix] Custom dialog now works properly
* [Fix] Fixed several typos and minor issues

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.1_Release/GWToolbox.dll)

## Version 2.0 - New interface
* [New] Rewrote interface
* [New] Materials Panel
* [New] Chat filter by content
* [New] Custom Minimap markers
* [New] Can completely disable individual toolbox features
* [New] Clock
* [New] Notepad

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/2.0_Release/GWToolbox.dll)

## Version 1.11
* [Minor] Added support for Year of the Rooster Lunars
* [Minor] Added lunar usage messages to the chat filter
* [Minor] Added “No one hears you…” message to the chat filter
* [Fix] Improved precision of drawings and pings on the minimap
* [Fix] Improved rendering order in the minimap, players should now be more visible

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/1.11_Release/GWToolbox.dll)

## Version 1.10
* [Fix] Toolbox and timer now show on top and handle clicks on top of the minimap

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/1.10_Release/GWToolbox.dll)

## Version 1.9 – Minimap Update
**IMPORTANT:** GWToolbox does no longer support GWMultiLaunch. Using GWToolbox on clients launched by GWMultiLaunch will probably cause crashes or unexpected behavior. Check https://github.com/GregLando113/gwlauncher/releases for an alternative.

* [New] Added minimap, more info here: tools.fbgmguild.com/gwtoolbox/info/minimap/
* [Minor] Edit Build panel will save whether it’s moved on top or not
* [Minor] Toolbox will now wait for you to log in into a character, so it can be launched early with no issues.
* [New] Added a hotkey for reapplying Lightbringer title (Execute… hotkey)
* [Fix] Toolbox will not make you pm Toolbox when you press delete after a Toolbox message. Toolbox won’t mind the lack of pms, don’t worry.
* [Fix] Fixed some crash bugs

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/1.9_Release/GWToolbox.dll)

## Version 1.8
* [Fix] Fixed a crash bug related to Borderless Window

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/1.8_Release/GWToolbox.dll)

## Version 1.7
* [New] Added an option to turn Guild Wars into Borderless fullscreen window.
* [New] Added a simple /useskill command to use a skill on recharge.
* [New] Added an option to hide server chat spam such as item drop, pick-up, 9-rings spam and “skills updated to pvp”.
* [New] Added an option to enable or disable Toolbox adjusting widget positions on window resize.
* [Minor] General user interface improvements.
* [Minor] Target hotkey will not target dead agents anymore (e.g. a dead Boo).
* [Minor] Added a few known ferry dialogs.

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/1.7_Release/GWToolbox.dll)

## Version 1.6
* [Minor] Added support for Lunars [Year of the Monkey]

## Version 1.5
* [Fix] /tp to ae1, ee1 and eg1 will actually port to district 1
* [Fix] Fixed bug where tb would crash on initialization in char selection screen
* [Minor] increased coindrop interval to 500ms (from 400)

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/1.5_Release/GWToolbox.dll)

## Version 1.4
* [New] Added chat commands
* [New] Added an option to enable or disable opening of template links
* [New] The “Tick” functionality in the default party window now acts as a toggle
* [New] Implemented an anchor system. Toolbox windows will stick to left/center/right and top/center/bottom when gw window is resized. Added a setting to enable or disable this functionality.
* [Minor] Party damage window now works with heroes
* [Fix] Party damage window now ignores damage inflicted to allies (e.g. from Life Bond)
* [Fix] Toolbox now properly resize its drawing area after a gw window resize

[Download](https://github.com/HasKha/GWToolboxpp/releases/download/1.4_Release/GWToolbox.dll)

## Version 1.3
* [Fix] Better fix on crash bug by toggling pcons on loading and char selection screen, added flag in launcher to check update messages

## Version 1.2
* [Fix] Fixed a problem with enabling pcons while on a loading screen crashing the client

## Version 1.1
* [New] Toolbox will now detect GWA2 on launch and notify the user instead of crashing the client
* [Fix] Fixed bug where toolbox would not save an empty build or template

## Version 1.0
First GWToolbox++ Release! Rewrote Toolbox from AutoIt to C++ with in-game rendering. Reimplemented most existing features with several improvements.
* [New] Pcons
* [New] Hotkeys
* [New] Builds
* [New] Fast Travel
* [New] Dialogs
* [New] Info, Distance, Health, Timer

[back](./)
