#include <string.h>
#include "Display.h"

Display::Display()
{

}

void Display::init()
{
  lcd.clearScreen();
  lcd.setX(35); lcd.setY(45); lcd.printStr("Garage");
  lcd.setX(32); lcd.setY(35); lcd.printStr("Weather");
  lcd.setX(32); lcd.setY(25); lcd.printStr("Station");
}

int Display::printVal( int y, const char * str, float val )
{
    char buf[32];

    lcd.setX(0);
    lcd.setY(y);

    int nch = sprintf( buf, str, val);
    for (int x=nch;x<XCHARS;x++)
        buf[x]=' ';
    buf[nch] = '\0';

    lcd.printStr( buf );

    return nch;
}

void Display::outsideTemp( float temp )
{
    int p = printVal(OUTSIDE_Y, "Outside  : %.1f C", temp);
    lcd.drawCircle(p *6 - 9, OUTSIDE_Y - 1, 1, 0);
}

void Display::insideTemp( float temp )
{
    int p = printVal(INSIDE_Y, "Inside   : %.1f C", temp);
    lcd.drawCircle(p *6 - 9, INSIDE_Y - 1, 1, 0);
}

void Display::pressure( float value )
{
    printVal(PRESSURE_Y, "Pressure : %.1f hPa", value);
}
void Display::humidity( float value )
{
    printVal(HUMIDITY_Y, "Humidity : %.1f %%", value);
}

void Display::rssi( float value )
{
    printVal(RSSI_Y, "RSSI     : %.0f dbm", value);
}
