#include "Window.h"

#include <iostream>
#include <memory>
#include <string>

namespace HW3
{

void Window::setName( const std::string& name )
{
   this->name = name;
}

void Window::addButton( std::shared_ptr< Button > button )
{
   this->buttons.push_back( std::move( button ) );
}

std::vector< std::shared_ptr< Button > > Window::getButtons() const
{
   return this->buttons;
}

Window::Window()
{

}

}
