@echo off
setlocal

set SRC_LIB=lib\src
set INC_LIB=lib\include
set LIB_OUT=lib
set OBJ_OUT=obj
set CLASES=clases

if not exist %OBJ_OUT% mkdir %OBJ_OUT%

echo --------------------------------------------------
echo 1) Compilando la libreria de negocio
echo --------------------------------------------------
for %%F in (
  "%SRC_LIB%\bd\sqlite3.c"
  "%SRC_LIB%\entidades\paciente.c"
  "%SRC_LIB%\entidades\historial.c"
  "%SRC_LIB%\entidades\cita.c"
  "%SRC_LIB%\entidades\usuario.c"
  "%SRC_LIB%\menu\funcionesMenu.c"
  "%SRC_LIB%\menu\log.c"
  "%SRC_LIB%\menu\menus.c"
  "lib\src\hospital.c"
) do (
  echo Compilando %%~nxF
  gcc -c "%%F" -I"%INC_LIB%" -o "%OBJ_OUT%\%%~nF.o" || goto :Error
)

echo --------------------------------------------------
echo 1.5) Compilando clases C++ (orientadas a objetos)
echo --------------------------------------------------
for %%F in (
  "%CLASES%\Persona.cpp"
  "%CLASES%\Paciente.cpp"
  "%CLASES%\Medico.cpp"
  "%CLASES%\HistorialMedico.cpp"
  "%CLASES%\Cita.cpp"
) do (
  echo Compilando %%~nxF
  g++ -c "%%F" -I"%INC_LIB%" -I"%CLASES%" -o "%OBJ_OUT%\%%~nF.o" || goto :Error
)
echo --------------------------------------------------
echo 1.6) Compilando funciones (funciones_*.cpp)
echo --------------------------------------------------
for %%F in (
  "funciones\\funciones_pacientes.cpp"
  "funciones\\funciones_citas.cpp"
  "funciones\\funciones_historiales.cpp"
) do (
  echo Compilando %%~nxF
  g++ -c "%%F" -I"%INC_LIB%" -I"clases" -o "obj\\%%~nF.o" || goto :Error
)

echo --------------------------------------------------
echo 2) Empaquetando libhospital.a
echo --------------------------------------------------
if exist "%LIB_OUT%\libhospital.a" del /q "%LIB_OUT%\libhospital.a"
for %%F in (%OBJ_OUT%\*.o) do (
  echo Agregando %%~nxF
  ar rcs "%LIB_OUT%\libhospital.a" "%%F" || goto :Error
)

echo --------------------------------------------------
echo 3) Compilando Servidor.exe
echo --------------------------------------------------
g++ ^
  servidor/main.cpp ^
  servidor/protocolo.cpp ^
  servidor/login.cpp ^
  -I"%INC_LIB%" -I"%CLASES%" ^
  -L"%LIB_OUT%" -lhospital -lws2_32 ^
  -o servidor.exe || goto :Error

echo --------------------------------------------------
echo 4) Compilando Cliente.exe
echo --------------------------------------------------
g++ ^
  cliente\main.cpp ^
  cliente\protocolo.cpp ^
  cliente\menu.cpp ^
  cliente\menu_login.cpp ^
  -I"%INC_LIB%" -I"%CLASES%" ^
  -L"%LIB_OUT%" -lhospital -lws2_32 ^
  -o cliente.exe || goto :Error

echo.
echo ====== BUILD COMPLETADO ======
pause
endlocal
exit /b 0

:Error
echo.
echo !!! ERROR DETECTADO. Abortando.
pause
exit /b 1

