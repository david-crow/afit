#pragma once

#include <iostream>
#include <functional>
#include <string>

namespace HW3
{

enum class BUTTON_STATE { ON, OFF };

using valueChangeCallback = std::function< void() >;

class Button
{
public:
   // set callback to update a button's state based on another button's value change
   void listenForValueChange( const valueChangeCallback& callback );

   // mutators
   void setName( const std::string& name );
   void setValue( const std::string& value );
   void setState( const BUTTON_STATE& state );

   // accessors
   const int& getWidth() const;
   const int& getHeight() const;
   const std::string& getValue() const;
   const std::string getStateStr() const;
   const BUTTON_STATE& getState() const;

   // pure virtual drawing function
   virtual void drawSection( const int& i, std::ostream& os ) const = 0;

protected:
   // constructors
   Button();
   Button( const Button& ) = delete;
   Button( Button&& ) = delete;
   virtual void onCreate() const = 0;

   // pure virtual drawing helper functions
   virtual const int drawTopEdge( std::ostream& os ) const = 0;
   virtual const int drawState( std::ostream& os ) const = 0;
   virtual const int drawValue( std::ostream& os ) const = 0;
   virtual const int drawBottomEdge( std::ostream& os ) const = 0;

   // helper functions
   void updateWidth();

   // member variables
   std::string name;
   std::string value;
   BUTTON_STATE state;

   int width;
   const int height = 4;

   // this updates an output button's state when an input button's value changes
   valueChangeCallback onValueChangeCallback = nullptr;
};

}
