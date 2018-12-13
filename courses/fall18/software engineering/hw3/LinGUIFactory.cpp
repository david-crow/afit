#include <memory>
#include "LinGUIFactory.h"
#include "LinButton.h"
#include "LinWindow.h"

namespace HW3
{

LinGUIFactory::LinGUIFactory()
{
   std::cout << "LinGUIFactory::LinGUIFactory()...\n";
}

std::shared_ptr< Window > LinGUIFactory::createWindow() const
{
   return std::static_pointer_cast< Window >( LinWindow::New() );
}

std::shared_ptr< Button > LinGUIFactory::createButton() const
{
   return std::static_pointer_cast< Button >( LinButton::New() );
}

}
