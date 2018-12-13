#include "Button.h"

#include <iostream>
#include <string>

namespace HW3
{

void Button::listenForValueChange( const valueChangeCallback& callback )
{
   this->onValueChangeCallback = callback;
}

void Button::setName( const std::string& name )
{
   this->name = name;
   this->updateWidth();
}

void Button::setValue( const std::string& value )
{
   this->value = value;
   this->updateWidth();

   if( this->onValueChangeCallback )
      this->onValueChangeCallback();
}

void Button::setState( const BUTTON_STATE& state )
{
   this->state = state;
   this->updateWidth();
}

const int& Button::getWidth() const
{
   return this->width;
}

const int& Button::getHeight() const
{
   return this->height;
}

const std::string& Button::getValue() const
{
   return this->value;
}

const std::string Button::getStateStr() const
{
   return this->state == BUTTON_STATE::ON ? "ON" : "OFF";
}

const BUTTON_STATE& Button::getState() const
{
   return this->state;
}

Button::Button()
{
   this->name = "";
   this->value = "";
   this->state = BUTTON_STATE::OFF;
   this->width = 0;
}

void Button::updateWidth()
{
   if( this->name.length() + 2 >= this->value.length() && this->name.length() + 2 >= this->getStateStr().length() )
   {
      this->width = this->name.length() + 2;
      return;
   }
   else if( this->value.length() >= this->getStateStr().length() )
   {
      this->width = this->value.length();
      return;
   }

   this->width = this->getStateStr().length();
}

}
