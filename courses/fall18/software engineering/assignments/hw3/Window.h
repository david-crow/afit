#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Button.h"

namespace HW3
{

class Window
{
public:
   // mutators
   void setName( const std::string& name );
   void addButton( std::shared_ptr< Button > button );

   // accessor
   std::vector< std::shared_ptr< Button > > getButtons() const;

   // pure virtual functions
   virtual void draw( std::ostream& os ) const = 0;

protected:
   // constructors
   Window();
   Window( const Window& ) = delete;
   Window( Window&& ) = delete;
   virtual void onCreate() const = 0;

   // member variables
   /* ----- Patterns: Composite
      This is an example of the Composite pattern. Here, we see that our Windows each contain
      instances of the Button class. This means our clients don't need to directly interact with
      the Buttons; instead, the available Window functions encompass the necessary Button functions.
   */
   std::vector< std::shared_ptr< Button > > buttons;
   std::string name;
};

}
