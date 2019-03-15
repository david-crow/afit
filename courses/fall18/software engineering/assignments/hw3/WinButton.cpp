#include <iostream>
#include <memory>
#include <string>
#include "Button.h"
#include "WinButton.h"

namespace HW3
{

std::shared_ptr< WinButton > WinButton::New()
{
   return std::shared_ptr< WinButton >( new WinButton );
}

const int WinButton::drawTopEdge( std::ostream& os ) const
{
   std::string s = "+-" + this->name;

   for( int i = 0; i < this->width - static_cast< int >( this->name.length() ) - 2; i++)
      s += "-";

   s += "-+";
   os << s;
   return s.length();
}

const int WinButton::drawState( std::ostream& os ) const
{
   std::string s = "|" + this->getStateStr();

   for( int i = 0; i < this->width - static_cast< int >( this->getStateStr().length() ); i++ )
      s += " ";

   s += "|";
   os << s;
   return s.length();
}

const int WinButton::drawValue( std::ostream& os ) const
{
   std::string s = "|" + this->value;

   for( int i = 0; i < this->width - static_cast< int >( this->value.length() ); i++ )
      s += " ";

   s += "|";
   os << s;
   return s.length();
}

const int WinButton::drawBottomEdge( std::ostream& os ) const
{
   std::string s = "+";

   for( int i = 0; i < this->width; i++ )
      s += "-";

   s += "+";
   os << s;
   return s.length();
}

WinButton::WinButton() : Button()
{
   this->onCreate();
}

void WinButton::onCreate() const
{
   std::cout << "WinButton::onCreate()...\n";
}

void WinButton::drawSection( const int& i, std::ostream& os ) const
{
   switch( i )
   {
      case 0:
         this->drawTopEdge( os );
         break;
      case 1:
         this->drawState( os );
         break;
      case 2:
         this->drawValue( os );
         break;
      default:
         this->drawBottomEdge( os );
   }
}

}
