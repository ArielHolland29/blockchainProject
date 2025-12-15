@echo off
REM clear_transactions.bat - delete transactions\tx*.txt files
setlocal enabledelayedexpansion

if not exist "transactions" (
  echo No transactions directory found. Nothing to delete.
  endlocal
  exit /b 0
)

pushd transactions >nul 2>&1
echo Deleting files matching tx*.txt in %cd% ...
set deleted=0
for %%F in (tx*.txt) do (
  if exist "%%F" (
    del /q "%%F"
    if not exist "%%F" set /a deleted+=1
  )
)
echo Deleted %deleted% files.
popd >nul 2>&1
endlocal
exit /b 0
