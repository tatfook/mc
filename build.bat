
rmdir build /s /q
mkdir build
xcopy /s /y D:\workspace\npl\NPLRuntime_cp_old\win64\bin\Mod\MC\Mod\MC\ Mod\MC\

xcopy /y build_win32\Release\MCImporter.dll build\
xcopy /s /y Mod\ build\Mod\
pushd build
call "..\7z.exe" a MCImporter.zip MCImporter.dll Mod\
popd

xcopy /y build\MCImporter.zip D:\Paracraft\Mod\

rem vs native terminal
rem cmake -A win32 -S ./ -B ./build_win32
rem devenv ./build_win32/MCImporter.sln /Build "Release|win32"