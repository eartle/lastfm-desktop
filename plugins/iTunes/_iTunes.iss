; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[CustomMessages]
Version=6.0.5.4

[Setup]
OutputBaseFilename=iTunesPluginWinSetup_6.0.5.4
; setup.exe version
VersionInfoVersion=6.0.5.4
VersionInfoTextVersion=6.0.5.4
AppName=Last.fm iTunes Plugin
AppVerName=Last.fm iTunes Plugin {cm:Version}
VersionInfoDescription=Last.fm iTunes Plugin Installer
AppPublisher=Last.fm
AppPublisherURL=http://www.last.fm
AppSupportURL=http://www.last.fm
AppUpdatesURL=http://www.last.fm
AppCopyright=Copyright  Ltd (c)
DefaultDirName="{pf}\iTunes\Plug-Ins"
UsePreviousAppDir=yes
UninstallFilesDir={commonappdata}\Last.fm\Client\UninstITW
OutputDir=.
Compression=lzma
SolidCompression=yes
DirExistsWarning=no
DisableReadyPage=yes
; Keep this the same across versions, even if they're incompatible. That will ensure
; uninstallation works fine after many upgrades. Can't use GUID as it'll break backward
; compatibility.
AppId=Audioscrobbler iTunes Plugin
CreateUninstallRegKey=no

[Dirs]
Name: "{localappdata}\Last.fm\Client"

[Registry]
; The name of the final subkey here must match the one in plugins.data
Root: HKLM; Subkey: "Software\Last.fm\Client\Plugins\itw"; ValueType: string; ValueName: "Version"; ValueData: "{cm:Version}"; Flags: uninsdeletekey
Root: HKLM; Subkey: "Software\Last.fm\Client\Plugins\itw"; ValueType: string; ValueName: "Name"; ValueData: "iTunes"; Flags: uninsdeletekey
Root: HKLM; Subkey: "Software\Last.fm\Client\Plugins\itw"; ValueType: string; ValueName: "Path"; ValueData: "{app}\itw_scrobbler.dll"; Flags: uninsdeletekey

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[InstallDelete]
; This is the name of the old old plugin
Type: files; Name: "{app}\iScrobbleWin.dll"
; This is the name of the old ASS plugin
Type: files; Name: "{app}\audioscrobbler.dll"

[UninstallDelete]
; For legacy reasons
Type: files; Name: "{app}\audioscrobbler.log"

; Try and delete the localappdata log for the case where the user running the uninstaller is the same as the plugin user
Type: files; Name: "{localappdata}\Last.fm\Client\iTunesPlugin.log"

[Files]
Source: "Release\itw_scrobbler.dll"; DestDir: "{app}"; Flags: ignoreversion

[Run]

[Code]
procedure CurStepChanged(CurStep: TSetupStep);
var
  batfile: String;
  batcontent: String;
  uninstaller: String;
  alreadyAdded: Boolean;
  cmdToAdd: String;
begin
  if (CurStep = ssPostInstall) then
  begin
    //MsgBox('postinstall', mbInformation, MB_OK);

    batfile := ExpandConstant('{commonappdata}\Last.fm\Client\uninst2.bat');
    LoadStringFromFile(batfile, batcontent);
    //MsgBox('loaded string: ' + batcontent, mbInformation, MB_OK);

    uninstaller := ExpandConstant('{uninstallexe}');
    //MsgBox('uninstaller pre-OEM: ' + uninstaller, mbInformation, MB_OK);

    alreadyAdded := (Pos(uninstaller, batcontent) <> 0);
    if (alreadyAdded = False) then
    begin
      cmdToAdd := uninstaller + #13#10;
      //MsgBox('not present, will add: ' + cmdToAdd, mbInformation, MB_OK);

      SaveStringToFile(batfile, cmdToAdd, True)
    end;

  end;

end;

