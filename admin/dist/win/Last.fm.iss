; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[CustomMessages]
Version=2.1.34


[Setup]
OutputBaseFilename=Last.fm-2.1.34
VersionInfoVersion=2.1.34
VersionInfoTextVersion=2.1.34
AppName="Last.fm Scrobbler"
AppVerName="Last.fm Scrobbler {cm:Version}"
VersionInfoDescription=Last.fm Installer
AppPublisher=Last.fm
AppPublisherURL=http://www.last.fm
AppSupportURL=http://www.last.fm
AppUpdatesURL=http://www.last.fm
AppCopyright=Copyright 2012 Last.fm Ltd. (C)
DefaultDirName={pf}\Last.fm
UsePreviousAppDir=yes
DefaultGroupName=Last.fm
OutputDir=.
AllowNoIcons=yes
Compression=lzma
SolidCompression=yes
DisableReadyPage=yes
DirExistsWarning=no
DisableFinishedPage=no
ShowLanguageDialog=yes
WizardImageFile=wizard.bmp
WizardSmallImageFile=wizard_small.bmp
SetupIconFile=installer.ico
UninstallDisplayIcon="{app}\Last.fm Scrobbler.exe"
AppModifyPath="{app}\UninsHs.exe" /m0=LastFM
WizardImageBackColor=$ffffff
WizardImageStretch=no
AppMutex=Lastfm-F396D8C8-9595-4f48-A319-48DCB827AD8F, Audioscrobbler-7BC5FBA0-A70A-406e-A50B-235D5AFE67FB

; This should stay the same across versions for the installer to treat it as the same program.
; It will then work to install however many updates and then run the uninstall script for
; the first version.
AppId=LastFM

[Languages]
; The first string is an internal code that we can set to whatever we feel like
Name: "en"; MessagesFile: "compiler:Default.isl"
;Name: "fr"; MessagesFile: "compiler:French.isl"
;Name: "it"; MessagesFile: "compiler:Italian.isl"
;Name: "de"; MessagesFile: "compiler:Deutsch.isl"
;Name: "es"; MessagesFile: "compiler:Spanish.isl"
;Name: "pt"; MessagesFile: "compiler:Portuguese - Brasil.isl"
;Name: "pl"; MessagesFile: "compiler:Polish.isl"
;Name: "ru"; MessagesFile: "compiler:Russian.isl"
;Name: "jp"; MessagesFile: "compiler:Japanese.isl"
;Name: "chs"; MessagesFile: "compiler:Simplified Chinese.isl"
;Name: "tr"; MessagesFile: "compiler:Turkish.isl"
;Name: "se"; MessagesFile: "compiler:Swedish.isl"


[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

; The OnlyBelowVersion flag disables this on Vista as an admin-run installer can't install a quick launch
; icon to the standard user's folder location. Sucks.
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0, 6;

[Dirs]
Name: "{localappdata}\Last.fm\Client"

[Files]
; Main files
Source: "..\..\..\_bin\Last.fm Scrobbler.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\_bin\iPodScrobbler.exe"; DestDir: "{app}"; Flags: ignoreversion

;libraries
Source: "..\..\..\_bin\lastfm.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\_bin\lastfm_fingerprint.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\_bin\unicorn.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\_bin\listener.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\_bin\logger.dll"; DestDir: "{app}"; Flags: ignoreversion

;Visual Studio 2010 redistributable packages
Source: "%VSDIR%\VC\redist\x86\Microsoft.VC100.CRT\*"; DestDir: "{app}"; Flags: ignoreversion
Source: "%VSDIR%\VC\redist\x86\Microsoft.VC100.ATL\*"; DestDir: "{app}"; Flags: ignoreversion

;Qt binaries
Source: "%QTDIR%\bin\QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "%QTDIR%\bin\QtGui4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "%QTDIR%\bin\QtNetwork4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "%QTDIR%\bin\QtXml4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "%QTDIR%\bin\QtSql4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "%QTDIR%\bin\QtWebKit4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\_bin\phonon.dll"; DestDir: "{app}"; Flags: ignoreversion

;image formats plugins
Source: "%QTDIR%\plugins\imageformats\qjpeg4.dll"; DestDir: "{app}\plugins\imageformats"; Flags: ignoreversion
Source: "%QTDIR%\plugins\imageformats\qgif4.dll"; DestDir: "{app}\plugins\imageformats"; Flags: ignoreversion
Source: "%QTDIR%\plugins\imageformats\qmng4.dll"; DestDir: "{app}\plugins\imageformats"; Flags: ignoreversion

;phonon backend plugin
Source: "..\..\..\_bin\plugins\phonon_backend\phonon_vlc.dll"; DestDir: "{app}\plugins\phonon_backend"; Flags: ignoreversion

;sqlite plugin
Source: "%QTDIR%\plugins\sqldrivers\qsqlite4.dll"; DestDir: "{app}\plugins\sqldrivers"; Flags: ignoreversion

; vlc libraries
Source: "..\..\..\_bin\libvlccore.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\_bin\libvlc.dll"; DestDir: "{app}"; Flags: ignoreversion

; vlc plugins
Source: "..\..\..\_bin\plugins\access\libaccess_http_plugin.dll"; DestDir: "{app}\plugins\access"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\audio_filter\liba52tofloat32_plugin.dll"; DestDir: "{app}\plugins\audio_filter"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\audio_filter\liba52tospdif_plugin.dll"; DestDir: "{app}\plugins\audio_filter"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\audio_filter\libaudio_format_plugin.dll"; DestDir: "{app}\plugins\audio_filter"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\audio_filter\libconverter_fixed_plugin.dll"; DestDir: "{app}\plugins\audio_filter"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\audio_filter\libdolby_surround_decoder_plugin.dll"; DestDir: "{app}\plugins\audio_filter"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\audio_filter\libdtstofloat32_plugin.dll"; DestDir: "{app}\plugins\audio_filter"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\audio_filter\libdtstospdif_plugin.dll"; DestDir: "{app}\plugins\audio_filter"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\audio_filter\libmpgatofixed32_plugin.dll"; DestDir: "{app}\plugins\audio_filter"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\audio_filter\libsamplerate_plugin.dll"; DestDir: "{app}\plugins\audio_filter"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\audio_filter\libscaletempo_plugin.dll"; DestDir: "{app}\plugins\audio_filter"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\audio_filter\libsimple_channel_mixer_plugin.dll"; DestDir: "{app}\plugins\audio_filter"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\audio_filter\libspeex_resampler_plugin.dll"; DestDir: "{app}\plugins\audio_filter"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\audio_filter\libtrivial_channel_mixer_plugin.dll"; DestDir: "{app}\plugins\audio_filter"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\audio_filter\libugly_resampler_plugin.dll"; DestDir: "{app}\plugins\audio_filter"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\audio_mixer\libfloat32_mixer_plugin.dll"; DestDir: "{app}\plugins\audio_mixer"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\audio_output\libaout_directx_plugin.dll"; DestDir: "{app}\plugins\audio_output"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\codec\libmpeg_audio_plugin.dll"; DestDir: "{app}\plugins\codec"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\demux\libes_plugin.dll"; DestDir: "{app}\plugins\demux"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\misc\liblogger_plugin.dll"; DestDir: "{app}\plugins\misc"; Flags: ignoreversion

;media player plugin installers
Source: "..\..\..\_bin\plugins\FooPlugin0.9.4Setup_2.3.1.3.exe"; DestDir: "{app}\plugins"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\iTunesPluginWinSetup_6.0.5.4.exe"; DestDir: "{app}\plugins"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\WinampPluginSetup_2.1.0.11.exe"; DestDir: "{app}\plugins"; Flags: ignoreversion
Source: "..\..\..\_bin\plugins\WmpPluginSetup_2.1.0.8.exe"; DestDir: "{app}\plugins"; Flags: ignoreversion

;3rd party
Source: "..\..\..\_bin\libfftw3f-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\_bin\libsamplerate-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\_bin\avcodec-54.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\_bin\avformat-54.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\_bin\avutil-52.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\..\..\_bin\swresample-0.dll"; DestDir: "{app}"; Flags: ignoreversion

;The stylesheets
Source: "..\..\..\app\client\Last.fm Scrobbler.css"; DestDir: "{app}"; Flags: ignoreversion

;The translations
Source: "..\..\..\i18n\*.qm"; DestDir: "{app}\i18n"; Flags: ignoreversion
Source: "%QTDIR%\translations\qt_de.qm"; DestDir: "{app}\i18n"; Flags: ignoreversion
Source: "%QTDIR%\translations\qt_es.qm"; DestDir: "{app}\i18n"; Flags: ignoreversion
Source: "%QTDIR%\translations\qt_fr.qm"; DestDir: "{app}\i18n"; Flags: ignoreversion
;Source: "%QTDIR%\translations\qt_it.qm"; DestDir: "{app}\i18n"; Flags: ignoreversion
Source: "%QTDIR%\translations\qt_ja.qm"; DestDir: "{app}\i18n"; Flags: ignoreversion
Source: "%QTDIR%\translations\qt_pl.qm"; DestDir: "{app}\i18n"; Flags: ignoreversion
Source: "%QTDIR%\translations\qt_pt.qm"; DestDir: "{app}\i18n"; Flags: ignoreversion
Source: "%QTDIR%\translations\qt_ru.qm"; DestDir: "{app}\i18n"; Flags: ignoreversion
Source: "%QTDIR%\translations\qt_sv.qm"; DestDir: "{app}\i18n"; Flags: ignoreversion
;Source: "%QTDIR%\translations\qt_tr.qm"; DestDir: "{app}\i18n"; Flags: ignoreversion
Source: "%QTDIR%\translations\qt_zh_CN.qm"; DestDir: "{app}\i18n"; Flags: ignoreversion

;The add/modify/remove file
Source: "UninsHs.exe"; DestDir: "{app}"; Flags: onlyifdoesntexist

;Text files?

[Registry]
; The Path is looked for in both places by plugins
Root: HKCU; Subkey: "Software\Last.fm\Client"; ValueType: string; ValueName: "Version"; ValueData: "{cm:Version}"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Last.fm\Client"; ValueType: string; ValueName: "Path"; ValueData: "{app}\Last.fm Scrobbler.exe"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Last.fm\Last.fm"; ValueType: string; ValueName: "Version"; ValueData: "{cm:Version}"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Last.fm\Last.fm"; ValueType: string; ValueName: "Path"; ValueData: "{app}\Last.fm Scrobbler.exe"; Flags: uninsdeletekey

; Register last.fm protocol only if it isn't already
Root: HKCR; Subkey: "lastfm"; ValueType: string; ValueName: ""; ValueData: "URL:lastfm"; Flags: uninsdeletekey
Root: HKCR; Subkey: "lastfm"; ValueType: string; ValueName: "URL Protocol"; ValueData: ""; Flags: uninsdeletekey
Root: HKCR; Subkey: "lastfm\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\Last.fm Scrobbler.exe"" ""%1"""; Flags: uninsdeletekey

; WinSparkle - This stop it asking the user if they want to check for updates on first run.
Root: HKCU; Subkey: "Software\Last.fm\Last.fm Desktop App\WinSparkle"; ValueType: string; ValueName: "CheckForUpdates"; ValueData: "1"; Flags: uninsdeletekey

; This is just for deleting keys at uninstall
Root: HKCU; Subkey: "Software\Last.fm"; Flags: dontcreatekey uninsdeletekey
Root: HKLM; Subkey: "Software\Last.fm"; Flags: dontcreatekey uninsdeletekey

[Icons]
Name: "{group}\Last.fm Scrobbler"; Filename: "{app}\Last.fm Scrobbler.exe"
Name: "{commondesktop}\Last.fm Scrobbler"; Filename: "{app}\Last.fm Scrobbler.exe"; Tasks: desktopicon

;Uninstall
Name: "{group}\Uninstall Last.fm Scrobbler"; Filename: "{app}\UninsHs.exe"; Parameters: "/u0=LastFM"

; The OnlyBelowVersion flag disables this on Vista as an admin-run installer can't install a quick launch
; icon to the standard user's folder location. Sucks.
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Last.fm Scrobbler"; Filename: "{app}\Last.fm Scrobbler.exe"; OnlyBelowVersion: 0,6; Tasks: quicklaunchicon


[Run]
Filename: "{app}\Last.fm Scrobbler.exe"; Description: "Start Last.fm Scrobbler now?"; Flags: nowait postinstall
Filename: "{app}\UninsHs.exe"; Parameters: "/r0=LastFM,{language},{srcexe},{app}\Installer.exe"; Flags: runminimized runhidden nowait

[InstallDelete]
;All the files that are not in fixed components (so the Radio compontent is actually removed on modify)
Type: Files; Name: "{app}\libFLAC_dynamic.dll"
Type: Files; Name: "{app}\libogg.dll"
Type: Files; Name: "{app}\libvorbis.dll"
Type: Files; Name: "{app}\libvorbisfile.dll"

Type: Files; Name: "{app}\WinSparkle.dll"

Type: Files; Name: "{app}\avutil-51.dll";
Type: Files; Name: "{app}\Last.fm.exe"
Type: Files; Name: "{app}\Last.fm Scrobbler.exe"
Type: Files; Name: "{app}\phonon4.dll"
Type: Files; Name: "{app}\phonon.dll"
Type: Files; Name: "{app}\Last.fm.css"
Type: filesandordirs; Name: "{app}\phonon_backend"
Type: filesandordirs; Name: "{app}\sqldrivers"
Type: dirifempty; Name: "{app}\imageformats"
Type: dirifempty; Name: "{app}\phonon_backend"
Type: dirifempty; Name: "{app}\sqldrivers"
Type: filesandordirs; Name: "{app}\plugins"

;Visual Studio 2008 redistributable packages
Type: Files; Name: "{app}\msvcp90.dll"
Type: Files; Name: "{app}\msvcm90.dll"
Type: Files; Name: "{app}\Microsoft.VC90.CRT.manifest"
Type: Files; Name: "{app}\Microsoft.VC90.ATL.manifest"
Type: Files; Name: "{app}\atl90.dll"

Type: Files; Name: "{app}\Last.fm.css"
Type: Files; Name: "{app}\Last.fm Scrobbler.css"
Type: Files; Name: "{commondesktop}\Last.fm"
Type: Files; Name: "{commondesktop}\Last.fm.lnk"
Type: Files; Name: "{commondesktop}\Last.fm Scrobbler"
Type: Files; Name: "{commondesktop}\Last.fm Scrobbler.lnk"
Type: Files; Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Last.fm"
Type: Files; Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Last.fm Scrobbler"
Type: Files; Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Last.fm.lnk"
Type: Files; Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Last.fm Scrobbler.lnk"
Type: filesandordirs; Name: "{group}"

;Files from the olde app
Type: Files; Name: "{app}\Cleaner.exe"
Type: Files; Name: "{app}\CrashReporter.exe"
Type: Files; Name: "{app}\killer.exe"
Type: Files; Name: "{app}\LastFM.exe"
Type: Files; Name: "{app}\LastFM.exe.config"
Type: Files; Name: "{app}\Updater.exe"
Type: Files; Name: "{app}\ChangeLog.txt"
Type: Files; Name: "{app}\Moose1.dll"
Type: Files; Name: "{app}\LastFmFingerprint1.dll"
Type: Files; Name: "{app}\LastFmTools1.dll"
Type: Files; Name: "{app}\breakpad.dll"
Type: Files; Name: "{app}\srv_httpinput.dll"
Type: Files; Name: "{app}\srv_madtranscode.dll"
Type: Files; Name: "{app}\srv_rtaudioplayback.dll"
Type: Files; Name: "{app}\unins000.dat"
Type: Files; Name: "{app}\unins000.exe"
Type: Files; Name: "{app}\zlibwapi.dll"
Type: Files; Name: "{app}\VistaLib32.dll"
Type: Files; Name: "{app}\VistaLib64.dll"
Type: filesandordirs; Name: "{app}\data"
Type: filesandordirs; Name: "{app}\imageformats"
Type: filesandordirs; Name: "{app}\Microsoft.VC80.CRT"

; This is the LAST step of uninstallation
[UninstallDelete]
;remove folders, if they are empty
Type: files; Name: "{app}\Installer.exe"
Type: dirifempty; Name: "{app}"

Type: filesandordirs; Name: "{localappdata}\Last.fm"

; This should be possible to delete as we're waiting until all the plugin uninstallers have been run.
Type: files; Name: "{commonappdata}\Last.fm\Client\uninst.bat"
Type: files; Name: "{commonappdata}\Last.fm\Client\uninst2.bat"
Type: filesandordirs; Name: "{commonappdata}\Last.fm\Client"
Type: dirifempty; Name: "{commonappdata}\Last.fm"


; This is the FIRST step of uninstallation
[UninstallRun]
Filename: "{app}\Last.fm.exe"; Parameters: "--exit"; Flags: skipifdoesntexist
Filename: "{app}\Last.fm Scrobbler.exe"; Parameters: "--exit"; Flags: skipifdoesntexist

[Code]
// Global variables
var g_firstRun: Boolean;

// This must be called before the install and its value stored
function IsUpgrade(): Boolean;
var
  sPrevPath: String;
begin
  sPrevPath := '';
  if not RegQueryStringValue(HKLM, 'Software\Microsoft\Windows\CurrentVersion\Uninstall\LastFM_is1', 'UninstallString', sPrevpath) then
    RegQueryStringValue(HKCU, 'Software\Microsoft\Windows\CurrentVersion\Uninstall\LastFM_is1', 'UninstallString', sPrevpath);
  Result := (sPrevPath <> '');
end;

function ShouldSkipPage(PageID: Integer): Boolean;
begin
  if PageID = wpFinished then
  begin
    // We skip the final screen if it's first run and go straight into config wizard
    if g_firstRun then
      Result := TRUE
  end;
  
  // This skip page code is for add/modify/remove
  if Pos('/SP-', UpperCase(GetCmdTail)) > 0 then
    case PageID of
      wpLicense, wpPassword, wpInfoBefore, wpUserInfo,
      wpSelectDir, wpSelectProgramGroup, wpInfoAfter:
        Result := True;
  end;
end;


procedure ExitApp(FileName: String);
var
  processExitCode: Integer;
begin
  if ExecAsOriginalUser( ExpandConstant(FileName), '--exit', '', SW_SHOW, ewWaitUntilTerminated, processExitCode) then
  begin
    // *high five*
  end
  else begin
    //MsgBox( 'Failed to stop ' + ExpandConstant(FileName), mbInformation, MB_OK );
  end;
end;


procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssInstall then
    begin
      ExitApp( '{app}\Last.fm.exe' );
      ExitApp( '{app}\Last.fm Scrobbler.exe' );
    end;
end;

function InitializeSetup(): Boolean;
begin
  // Need to evaluate and store this before any installation has been done
  g_firstRun := not IsUpgrade();

  // Run setup
  Result := TRUE;
end;




