#include <memory>
#include "WinGUIFactory.h"
#include "WinButton.h"
#include "WinWindow.h"

namespace HW3
{

WinGUIFactory::WinGUIFactory()
{
   std::cout << "WinGUIFactory::WinGUIFactory()...\n";
}

std::shared_ptr< Window > WinGUIFactory::createWindow() const
{
   return std::static_pointer_cast< Window >( WinWindow::New() );
}

std::shared_ptr< Button > WinGUIFactory::createButton() const
{
   return std::static_pointer_cast< Button >( WinButton::New() );
}

}
