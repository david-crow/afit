#include <iostream>
#include <memory>
#include <string>
#include "LinButton.h"
#include "LinWindow.h"

namespace HW3
{

std::shared_ptr< LinWindow > LinWindow::New()
{
   return std::shared_ptr< LinWindow >( new LinWindow );
}

void LinWindow::draw( std::ostream& os ) const
{
   std::string title = "Lindow: " + this->name;
   int width = title.length() + 2;
   int button_width = 0;

   for( auto& b : this->buttons )
      button_width += b->getWidth() + 2;

   if( button_width > width )
      width = button_width;

   // top edge
   os << "<=" << title;

   if( width > static_cast< int >( title.length() ) )
      for( int i = 0; i < width - static_cast< int >( title.length() ) - 2; i++ )
         os << "=";

   os << "=>\n";

   // empty row
   os << "[";

   for( int i = 0; i < width; i++ )
      os << " ";

   os << "]\n";

   // buttons
   if( this->buttons.size() )
   {
      for( int i = 0; i < this->buttons[0]->getHeight(); i++ )
      {
         os << "[";

         for( auto& b : this->buttons )
            b->drawSection( i, os );

         for( int i = 0; i < width - button_width; i++)
            os << " ";

         os << "]\n";
      }
   }

   // bottom edge
   os << "<";

   for( int i = 0; i < width; i++ )
      os << "=";

   os << ">\n";
}

LinWindow::LinWindow() : Window()
{
   this->onCreate();
}

void LinWindow::onCreate() const
{
   std::cout << "LinWindow::onCreate()...\n";
}

}
