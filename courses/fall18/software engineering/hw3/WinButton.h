#pragma once

#include <iostream>
#include <memory>
#include <string>
#include "Button.h"

namespace HW3
{

class WinButton : public Button
{
public:
   /* ----- Patterns: Factory
      We see here an example of the Factory pattern. We've hidden all constructors for this
      class. Now, clients can only create class instances by way of the New() function. In
      doing so, we ensure that, upon creation of a WinButton, our system functions as required.
   */
   static std::shared_ptr< WinButton > New();

   // drawing function
   virtual void drawSection( const int& i, std::ostream& os ) const;

private:
   // constructors
   WinButton();
   WinButton( const WinButton& ) = delete;
   WinButton( WinButton&& ) = delete;
   virtual void onCreate() const;

   // drawing helper functions
   virtual const int drawTopEdge( std::ostream& os ) const;
   virtual const int drawState( std::ostream& os ) const;
   virtual const int drawValue( std::ostream& os ) const;
   virtual const int drawBottomEdge( std::ostream& os ) const;
};

}
