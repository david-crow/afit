#pragma once

#include <memory>
#include "AbstractGUIFactory.h"
#include "LinButton.h"
#include "LinWindow.h"

namespace HW3
{

class LinGUIFactory : public AbstractGUIFactory
{
public:
   LinGUIFactory();
   virtual std::shared_ptr< Window > createWindow() const;
   virtual std::shared_ptr< Button > createButton() const;
};

}
