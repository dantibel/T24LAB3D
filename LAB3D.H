/* Belov Danil, 10-6, 05.11.21 */

#include <windows.h>
#include "commondf.h"

VOID PutPixel( INT X, INT Y, DWORD Color );

VOID LabInit( VOID );
VOID LabKeyboard( INT Key );
VOID LabStep( DOUBLE SyncTime, DOUBLE DeltaTime );