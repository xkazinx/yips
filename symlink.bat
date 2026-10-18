mkdir %0\..\deploy\x86\
mklink /d %0\..\deploy\x86\data %0\..\deploy\data
mklink %0\..\deploy\x86\openal32.dll "%LIBPATH%\SFML-2.5.1\bin\openal32.dll"
mklink %0\..\deploy\x86\sfml-graphics-2.dll "%LIBPATH%\SFML-2.5.1-Win32\bin\sfml-graphics-2.dll"
mklink %0\..\deploy\x86\sfml-graphics-d-2.dll "%LIBPATH%\SFML-2.5.1-Win32\bin\sfml-graphics-d-2.dll"
mklink %0\..\deploy\x86\sfml-system-2.dll "%LIBPATH%\SFML-2.5.1-Win32\bin\sfml-system-2.dll"
mklink %0\..\deploy\x86\sfml-system-d-2.dll "%LIBPATH%\SFML-2.5.1-Win32\bin\sfml-system-d-2.dll"
mklink %0\..\deploy\x86\sfml-window-2.dll "%LIBPATH%\SFML-2.5.1-Win32\bin\sfml-window-2.dll"
mklink %0\..\deploy\x86\sfml-window-d-2.dll "%LIBPATH%\SFML-2.5.1-Win32\bin\sfml-window-d-2.dll"
mkdir %0\..\deploy\x64\
mklink /d %0\..\deploy\x64\data %0\..\deploy\data
mklink %0\..\deploy\x64\openal32.dll "%LIBPATH%\SFML-2.5.1-x64\bin\openal32.dll"
mklink %0\..\deploy\x64\sfml-graphics-2.dll "%LIBPATH%\SFML-2.5.1-x64\bin\sfml-graphics-2.dll"
mklink %0\..\deploy\x64\sfml-graphics-d-2.dll "%LIBPATH%\SFML-2.5.1-x64\bin\sfml-graphics-d-2.dll"
mklink %0\..\deploy\x64\sfml-system-2.dll "%LIBPATH%\SFML-2.5.1-x64\bin\sfml-system-2.dll"
mklink %0\..\deploy\x64\sfml-system-d-2.dll "%LIBPATH%\SFML-2.5.1-x64\bin\sfml-system-d-2.dll"
mklink %0\..\deploy\x64\sfml-window-2.dll "%LIBPATH%\SFML-2.5.1-x64\bin\sfml-window-2.dll"
mklink %0\..\deploy\x64\sfml-window-d-2.dll "%LIBPATH%\SFML-2.5.1-x64\bin\sfml-window-d-2.dll"
pause