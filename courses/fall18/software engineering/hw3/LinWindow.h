#pragma once

#include <iostream>
#include <memory>
#include "Window.h"

namespace HW3
{

class LinWindow : public Window
{
public:
   /* ----- Patterns: Factory
      We see here an example of the Factory pattern. We've hidden all constructors for this
      class. Now, clients can only create class instances by way of the New() function. In
      doing so, we ensure that, upon creation of a LinWindow, our system functions as required.
   */
   static std::shared_ptr< LinWindow > New();
   virtual void draw( std::ostream& os ) const;

private:
   // constructors
   LinWindow();
   virtual void onCreate() const;

   LinWindow( const LinWindow& ) = delete;
   LinWindow( LinWindow&& ) = delete;
};

}
