[Setup]
AppId={{A1B2C3D4-E5F6-7890-ABCD-EF1234567890}
AppName=Chamchi GCS
AppVersion=0.1.0
AppPublisher=chamchiC
AppPublisherURL=https://github.com/chamchiC/chamchi_GCS
DefaultDirName={autopf}\Chamchi GCS
DefaultGroupName=Chamchi GCS
OutputDir=C:\Users\coslb
OutputBaseFilename=chamchi_GCS_v0.1.0_setup
Compression=lzma2
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "korean"; MessagesFile: "compiler:Languages\Korean.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "바탕화면에 바로가기 만들기"; GroupDescription: "추가 옵션:"

[Files]
Source: "C:\chamchi_GCS\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\Chamchi GCS"; Filename: "{app}\chamchi_GCS.exe"
Name: "{group}\Chamchi GCS 제거"; Filename: "{uninstallexe}"
Name: "{commondesktop}\Chamchi GCS"; Filename: "{app}\chamchi_GCS.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\chamchi_GCS.exe"; Description: "Chamchi GCS 실행"; Flags: nowait postinstall skipifsilent
