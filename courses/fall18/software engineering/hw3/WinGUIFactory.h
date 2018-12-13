#pragma once

#include <memory>
#include "AbstractGUIFactory.h"
#include "WinButton.h"
#include "WinWindow.h"

namespace HW3
{

class WinGUIFactory : public AbstractGUIFactory
{
public:
   WinGUIFactory();
   virtual std::shared_ptr< Window > createWindow() const;
   virtual std::shared_ptr< Button > createButton() const;
};

}
