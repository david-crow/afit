#include <iostream>
#include <memory>
#include <string>
#include "Button.h"
#include "LinButton.h"

namespace HW3
{

std::shared_ptr< LinButton > LinButton::New()
{
   return std::shared_ptr< LinButton >( new LinButton );
}

const int LinButton::drawTopEdge( std::ostream& os ) const
{
   std::string s = "<=";

   for( int i = 0; i < this->width - static_cast< int >( this->name.length() ) - 2; i++)
      s += "=";

   s += this->name + "=>";
   os << s;
   return s.length();
}

const int LinButton::drawState( std::ostream& os ) const
{
   std::string s = "[";

   for( int i = 0; i < this->width - static_cast< int >( this->getStateStr().length() ); i++ )
      s += " ";

   s += this->getStateStr() + "]";
   os << s;
   return s.length();
}

const int LinButton::drawValue( std::ostream& os ) const
{
   std::string s = "[";

   for( int i = 0; i < this->width - static_cast< int >( this->value.length() ); i++ )
      s += " ";

   s += this->value + "]";
   os << s;
   return s.length();
}

const int LinButton::drawBottomEdge( std::ostream& os ) const
{
   std::string s = "<";

   for( int i = 0; i < this->width; i++ )
      s += "=";

   s += ">";
   os << s;
   return s.length();
}

LinButton::LinButton() : Button()
{
   this->onCreate();
}

void LinButton::onCreate() const
{
   std::cout << "LinButton::onCreate()...\n";
}

void LinButton::drawSection( const int& i, std::ostream& os ) const
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
