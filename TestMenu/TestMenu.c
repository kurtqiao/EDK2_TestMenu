/*++

Copyright (c) 2012 Kurt Qiao

Module Name:

    TestMenu.c
    
Abstract:
    Test to draw a menu

Revision History

--*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>


//#define MAX_LINE 32
//#define MAX_CHAR 80
#define EFI_MAX_LINE_LENGTH 160

#define MENU_TITTLE     L"Test Tittle"
#define MENU_COPYRIGHT  L"Copyright (c) 2012"

//
// EFI Scan codes
//
#define SCAN_NULL       0x0000
#define SCAN_UP         0x0001
#define SCAN_DOWN       0x0002
#define SCAN_RIGHT      0x0003
#define SCAN_LEFT       0x0004
#define SCAN_HOME       0x0005
#define SCAN_END        0x0006
#define SCAN_INSERT     0x0007
#define SCAN_DELETE     0x0008
#define SCAN_PAGE_UP    0x0009
#define SCAN_PAGE_DOWN  0x000A
#define SCAN_F1         0x000B
#define SCAN_F2         0x000C
#define SCAN_F3         0x000D
#define SCAN_F4         0x000E
#define SCAN_F5         0x000F
#define SCAN_F6         0x0010
#define SCAN_F7         0x0011
#define SCAN_F8         0x0012
#define SCAN_F9         0x0013
#define SCAN_F10        0x0014
#define SCAN_F11        0x0015
#define SCAN_F12        0x0016
#define SCAN_ESC        0x0017
#define Key_ALT_X       0x2d00

EFI_SYSTEM_TABLE *ST;
EFI_BOOT_SERVICES  *BS;

    CHAR16 *menuItems[]         =     {L"AMenu", L"BMenu", L"CMenu"};
    CHAR16 *menuItemCapital[]   =     {L"A", L"B",L"C"};
    CHAR16 *tailItemkeys[]      =     {L"ESC", L"PGDN/PGUP", L"F1"};
    CHAR16 *tailItems[]         =     {L"Exit", L"Next",L"AMenu"};
    CHAR16 *subMenu[]           =     {L"submenu1",L"about   "};

UINTN   mCols;
UINTN   mRows;

UINTN EfiColor[] = {
    EFI_BLACK,  
    EFI_BLUE,    
    EFI_GREEN,   
    EFI_CYAN,    
    EFI_RED,     
    EFI_MAGENTA, 
    EFI_BROWN,   
    EFI_LIGHTGRAY,                 
    EFI_DARKGRAY,        
    EFI_LIGHTBLUE,       
    EFI_LIGHTGREEN,      
    EFI_LIGHTCYAN,       
    EFI_LIGHTRED,        
    EFI_LIGHTMAGENTA,    
    EFI_YELLOW,          
    EFI_WHITE           
};

UINTN   EfiBackGroundColor[] = {
    EFI_BACKGROUND_BLACK,        
    EFI_BACKGROUND_BLUE,         
    EFI_BACKGROUND_GREEN,        
    EFI_BACKGROUND_CYAN,         
    EFI_BACKGROUND_RED,          
    EFI_BACKGROUND_MAGENTA,      
    EFI_BACKGROUND_BROWN,        
    EFI_BACKGROUND_LIGHTGRAY
};

EFI_STATUS
TestPrintAt (
  IN  UINTN             Col,
  IN  UINTN             Row,
  IN  CHAR16            *String
  );

EFI_STATUS
MenuHeader(
    VOID
  );
EFI_STATUS
PrintTittle(
    VOID
  );
EFI_STATUS
PrintTail(
    VOID
  );
EFI_STATUS
ClearArea(
  IN  UINTN             Col0,
  IN  UINTN             Row0,
  IN  UINTN             Col1,
  IN  UINTN             Row1,
  IN  UINTN             TextAttribute
  );
EFI_STATUS 
DrawBOX(
    UINTN startx,
    UINTN starty,
    UINTN high,
    UINTN width,
    UINTN TextAttribute
);
EFI_STATUS
PrintSubMenu();

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
    UINTN                     index;
    EFI_INPUT_KEY             Key;

    ST = SystemTable;
    BS = SystemTable->BootServices;
//    InitializeLib (ImageHandle, SystemTable);
  ST->ConOut->QueryMode (
                ST->ConOut,
                ST->ConOut->Mode->Mode,
                &mCols,
                &mRows
                );
    //
    //disable cursor
    //
    ST->ConOut->EnableCursor(ST->ConOut,FALSE);
    ST->ConOut->SetAttribute(ST->ConOut,EFI_WHITE|EFI_BACKGROUND_BLACK);
    ST->ConOut->ClearScreen(ST->ConOut);
    //PrintXY(0,0,NULL,NULL,L"Test Menu %d * %d, mode = %d", mCols, mRows, ST->ConOut->Mode->Mode);
    
    PrintTittle();
    MenuHeader();
    ClearArea(0,2, 79, 23, EFI_WHITE|EFI_BACKGROUND_BLUE);
    
    PrintTail();

    while(TRUE){

        BS->WaitForEvent(1, &ST->ConIn->WaitForKey, &index);
        ST->ConIn->ReadKeyStroke(ST->ConIn, &Key);
        switch(Key.ScanCode)
        {

            case SCAN_ESC:
            ST->ConOut->SetAttribute (ST->ConOut,EFI_WHITE|EFI_BACKGROUND_BLACK);
            ST->ConOut->ClearScreen (ST->ConOut);
            return EFI_SUCCESS;
            break;
            case SCAN_F1:
            PrintSubMenu();
            break;
            case SCAN_NULL:
            break;
            default:
            break;
        }

    }
    
    return EFI_SUCCESS;
}


EFI_STATUS
TestPrintAt (
  IN  UINTN             Col,
  IN  UINTN             Row,
  IN  CHAR16            *String
  )
/*++

Routine Description:

  Simply Set Cursor Position and then Output the String at current Cursor.

Arguments:

  Col               - the column position that the cursor to be set.
  Row               - the Row position that the cursor to be set
  String            - is the Pointer the String to be Printed.

Returns:

  EFI_SUCCESS       - the String was output to device successfully.
  EFI_DEVICE_ERROR

--*/
{
  EFI_STATUS Status;

  Status = ST->ConOut->SetCursorPosition (ST->ConOut, Col, Row);
  if (!EFI_ERROR(Status)) {
    Status = ST->ConOut->OutputString (ST->ConOut, String);
  }
  return Status;
}

EFI_STATUS
PrintTittle(
    VOID
  )
{

    ClearArea(0,0,79,0, EFI_YELLOW|EFI_BACKGROUND_BLACK);
    
    ST->ConOut->SetCursorPosition (ST->ConOut, 30, 0);
    Print(MENU_TITTLE);
    ST->ConOut->SetAttribute (ST->ConOut,EFI_LIGHTBLUE|EFI_BACKGROUND_BLACK);
    ST->ConOut->SetCursorPosition(ST->ConOut, 77-StrLen(MENU_COPYRIGHT), 0);
    Print(MENU_COPYRIGHT);


    return EFI_SUCCESS;

}

EFI_STATUS
PrintTail(
    VOID
  )
{

    UINTN x, i;

    ClearArea(0,23,79,23, EFI_WHITE|EFI_BACKGROUND_BLACK);
  x = 1;

  for(i=0; i<3; i++)
  {
    ST->ConOut->SetCursorPosition(ST->ConOut, x, 23);
    ST->ConOut->SetAttribute (ST->ConOut,EFI_WHITE|EFI_BACKGROUND_BLACK);
    Print(tailItemkeys[i]);
    x = x + StrLen(tailItemkeys[i]) + 2;
    ST->ConOut->SetCursorPosition(ST->ConOut, x, 23);
    ST->ConOut->SetAttribute (ST->ConOut,EFI_LIGHTGRAY|EFI_BACKGROUND_BLACK);
    Print(tailItems[i]);
    x = x + StrLen(tailItems[i]) +2;
  }
    return EFI_SUCCESS;
}


EFI_STATUS
MenuHeader(
    VOID
  )
/*++

Routine Description:

  create the menupage header.

--*/
{
    UINTN i, x;


    ClearArea(0,1,79,1, EFI_BLACK|EFI_BACKGROUND_LIGHTGRAY);
    x = 2;
  for(i=0; i<3; i++)
  {
    ST->ConOut->SetCursorPosition(ST->ConOut, x, 1);
    ST->ConOut->SetAttribute (ST->ConOut,EFI_BLACK|EFI_BACKGROUND_LIGHTGRAY);
    Print(menuItems[i]);

    ST->ConOut->SetCursorPosition(ST->ConOut, x, 1);
    ST->ConOut->SetAttribute (ST->ConOut,EFI_RED|EFI_BACKGROUND_LIGHTGRAY);
    Print(menuItemCapital[i]);

    x = x + StrLen(menuItems[i])+ StrLen(menuItemCapital[i]) +2;
  }

  return EFI_SUCCESS;

 }

EFI_STATUS
PrintAbout()
{
    UINTN                 index;
    EFI_INPUT_KEY         Key;
    BOOLEAN subwait;

    ClearArea(20, 5, 20+30, 5+5, EFI_BLACK|EFI_BACKGROUND_LIGHTGRAY);
    DrawBOX(21, 6, 10, 49, EFI_BLACK|EFI_BACKGROUND_LIGHTGRAY);
    ST->ConOut->SetCursorPosition(ST->ConOut, 20+10, 5);
    ST->ConOut->SetAttribute (ST->ConOut,EFI_BLACK|EFI_BACKGROUND_LIGHTGRAY);
    Print(L"About");
    ST->ConOut->SetCursorPosition(ST->ConOut, 27, 8);
    Print(L"Test Menu V1.0");
    ST->ConOut->SetCursorPosition(ST->ConOut, 35, 9);
    Print(L"--by KurtQiao");

    subwait = TRUE;
    while(subwait){

        BS->WaitForEvent(1, &ST->ConIn->WaitForKey, &index);
        ST->ConIn->ReadKeyStroke(ST->ConIn, &Key);
        switch(Key.ScanCode)
        {
            case SCAN_ESC:
            subwait = FALSE;
            break;
            
            default:
            break;
        }
    }

    ClearArea(20, 5, 20+30, 5+5, EFI_BLACK|EFI_BACKGROUND_BLUE);
    return EFI_SUCCESS;
}

EFI_STATUS
PrintSubMenu()
{
    UINTN i,x,y;
    BOOLEAN subwait;
    UINTN                 index;
    EFI_INPUT_KEY         Key;

    ClearArea(1, 2, 10, 5, EFI_BLACK|EFI_BACKGROUND_LIGHTGRAY);
    DrawBOX(1, 2, 5, 10, EFI_BLACK|EFI_BACKGROUND_LIGHTGRAY);

    x=2;
    y=3;
    for(i=0;i<2;i++)
    {
        ST->ConOut->SetCursorPosition(ST->ConOut, x, y+i);
        Print(subMenu[i]);
    }
    //highlight submenu
    i = 0;
    
    subwait = TRUE;
    while(subwait){

        ST->ConOut->SetCursorPosition(ST->ConOut, x, y);
        ST->ConOut->SetAttribute (ST->ConOut,EFI_WHITE|EFI_BACKGROUND_BLACK);
        Print(subMenu[i]);

        BS->WaitForEvent(1, &ST->ConIn->WaitForKey, &index);
        ST->ConIn->ReadKeyStroke(ST->ConIn, &Key);

        ST->ConOut->SetCursorPosition(ST->ConOut, x, y);
        ST->ConOut->SetAttribute (ST->ConOut,EFI_BLACK|EFI_BACKGROUND_LIGHTGRAY);
        Print(subMenu[i]);

        switch(Key.ScanCode)
        {
            case SCAN_ESC:
            subwait = FALSE;
            break;
            case SCAN_DOWN:
            case SCAN_PAGE_DOWN:
                y++;
                i++;
                if (y > 4)
                {    y = 4; i = 1;}
            break;
            case SCAN_UP:
            case SCAN_PAGE_UP:
                y--;
                i--;
                if (y < 3)
                {    y = 3; i = 0;}
            break;
            case SCAN_NULL:
            switch(Key.UnicodeChar) {
              case CHAR_CARRIAGE_RETURN:
                if (i == 1)
                {    ClearArea(1, 2, 10, 5, EFI_BLACK|EFI_BACKGROUND_BLUE);
                    PrintAbout();
                    subwait = FALSE;
                }
                break;
              //case CHAR_TAB:
              //  break;
              default:
                break;
            }
            break;
            default:
            break;
        }

    }

    ClearArea(1, 2, 10, 5, EFI_BLACK|EFI_BACKGROUND_BLUE);

    return EFI_SUCCESS;

}
EFI_STATUS
ClearArea (
  IN  UINTN             Col0,
  IN  UINTN             Row0,
  IN  UINTN             Col1,
  IN  UINTN             Row1,
  IN  UINTN             TextAttribute
  )
{
      EFI_STATUS Status;
      CHAR16           Buffer[EFI_MAX_LINE_LENGTH+1];
      UINTN            Index;
      UINTN            Y0;
      UINTN            X0;
      UINTN            Y1;
      UINTN            X1;

      X0 = Col0;
      Y0 = Row0;
      X1 = Col1;
      Y1 = Row1;

   Status = ST->ConOut->SetAttribute (ST->ConOut,TextAttribute);

   for (Index = 0; Index < X1 - X0 + 1; Index++) {
      Buffer[Index] = L' ';
   }
      Buffer[Index] = L'\0';
   for (Index = 0; Index < Y1 - Y0 + 1; Index++) {
    ST->ConOut->SetCursorPosition(ST->ConOut, X0, Y0 + Index);
    Status = TestPrintAt (X0, Y0 + Index, Buffer); 
    if (EFI_ERROR (Status)) {
      break;
    }
   }
  return Status;
 }

EFI_STATUS 
DrawBOX(UINTN startx,UINTN starty,UINTN high,UINTN width, UINTN TextAttribute)
{ 
    UINTN i;

    ST->ConOut->SetAttribute (ST->ConOut,TextAttribute);

    ST->ConOut->SetCursorPosition(ST->ConOut,startx,starty);
    Print(L"%c", BOXDRAW_DOWN_RIGHT); 
    for (i=startx+1;i<width;i++) Print(L"%c", BOXDRAW_HORIZONTAL); 
    Print(L"%c", BOXDRAW_DOWN_LEFT); 
    for( i=starty+1;i<high;i++)
    {
    ST->ConOut->SetCursorPosition(ST->ConOut,startx,i);
    Print(L"%c", BOXDRAW_VERTICAL); 
    ST->ConOut->SetCursorPosition(ST->ConOut,width,i);
    Print(L"%c", BOXDRAW_VERTICAL); 
    }
    ST->ConOut->SetCursorPosition(ST->ConOut,startx,high);    

    Print(L"%c", BOXDRAW_UP_RIGHT); 
    for (i=startx+1;i<width;i++) Print(L"%c", BOXDRAW_HORIZONTAL); 
    Print(L"%c", BOXDRAW_UP_LEFT); 

    return EFI_SUCCESS;
}
