#include <iostream>
#include <memory>
#include <string>
#include "WinButton.h"
#include "WinWindow.h"

namespace HW3
{

std::shared_ptr< WinWindow > WinWindow::New()
{
   return std::shared_ptr< WinWindow>( new WinWindow );
}

void WinWindow::draw( std::ostream& os ) const
{
   std::string title = "WinWindow: " + this->name;
   int width = title.length() + 3;
   int button_width = 0;

   for( auto& b : this->buttons )
      button_width += b->getWidth() + 2;

   if( button_width > width )
      width = button_width;

   // top edge
   os << "+---" << title;

   if( width > static_cast< int >( title.length() ) )
      for( int i = 0; i < width - static_cast< int >( title.length() ) - 3; i++ )
         os << "-";

   os << "+\n";

   // empty row
   os << "|";

   for( int i = 0; i < width; i++ )
      os << " ";

   os << "|\n";

   // buttons
   if( this->buttons.size() )
   {
      for( int i = 0; i < this->buttons[0]->getHeight(); i++ )
      {
         os << "|";

         for( auto& b : this->buttons )
            b->drawSection( i, os );

         for( int i = 0; i < width - button_width; i++)
            os << " ";

         os << "|\n";
      }
   }

   // bottom edge
   os << "+";

   for( int i = 0; i < width; i++ )
      os << "-";

   os << "+\n";
}

WinWindow::WinWindow() : Window()
{
   this->onCreate();
}

void WinWindow::onCreate() const
{
   std::cout << "WinWindow::onCreate()...\n";
}

}
