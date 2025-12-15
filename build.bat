@echo off
REM Build script for Simplified_blockchain (Windows, cmd.exe)

set INCLUDE_DIR=include
set SRC_DIR=src
set OUT=blockchain.exe

if not exist %INCLUDE_DIR% (
  echo Include directory not found: %INCLUDE_DIR%
  exit /b 1
)

g++ -I%INCLUDE_DIR% %SRC_DIR%\main.cpp %SRC_DIR%\functions.cpp %SRC_DIR%\hash.cpp -o %OUT%
if %ERRORLEVEL% neq 0 (
  echo Build failed.
  exit /b %ERRORLEVEL%
)

echo Build succeeded: %OUT%
