@echo off
setlocal enabledelayedexpansion

:: Nome do arquivo de saída
set output=merged.txt

:: Limpa o arquivo de saída se já existir
if exist %output% del %output%

:: Adiciona o cabeçalho ao arquivo de saída
echo Data ; Hora ; Umidade do ar (%%) ; Temperatura do ar (°C) ; Condição do solo > %output%

:: Loop para cada arquivo TXT no diretório
for %%f in (*.txt) do (
    set lineNumber=0
    for /f "usebackq delims=" %%l in ("%%f") do (
        set /a lineNumber+=1
        if !lineNumber! GTR 3 echo %%l >> %output%
    )
)

echo Arquivos mesclados em %output%.
