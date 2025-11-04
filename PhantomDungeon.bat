@echo off

set BASE=%~dp0

cd "%BASE%Servidor/bin/Debug"
start Servidor.exe

cd "%BASE%Cliente/bin/Debug"

set PATH=%PATH%;%BASE%libs\SDL2_linkNeededs
set PATH=%PATH%;%BASE%libs\SDL2_more

start Prototipito.exe