/* ----- Patterns: Abstract Factory
   This class, clearly, is an example of the Abstract Factory pattern. Because we've made an
   abstract base class, we've ensured our LinGUIFactory and WinGUIFactory classes contain the
   same functionality. In doing so, we've made it easier for our clients to interact with the
   classes, and we've also made it easier for us to extend our program.
*/

#pragma once

#include <memory>
#include "Button.h"
#include "Window.h"

namespace HW3
{

class AbstractGUIFactory
{
public:
   virtual ~AbstractGUIFactory() {}
   virtual std::shared_ptr< Window > createWindow() const = 0;
   virtual std::shared_ptr< Button > createButton() const = 0;
};

}
