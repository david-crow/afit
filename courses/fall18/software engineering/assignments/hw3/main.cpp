// Alternatively, use a command-line argument
// #define osWINDOWS
// #define osLINUX

#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "FourPinExternInput.h"
#include "AbstractGUIFactory.h"
#include "LinGUIFactory.h"
#include "WinGUIFactory.h"
#include "Button.h"
#include "Window.h"
#include "LinButton.h"
#include "LinWindow.h"
#include "WinButton.h"
#include "WinWindow.h"

using namespace HW3;

void testDrawingWindowsAndButtons();
void testMainProgram();
void drawAscii();

int main(int argc, char* argv[])
{
   if( argc >= 2 && std::string{ argv[1] } == "test" )
      testDrawingWindowsAndButtons();
   else
      testMainProgram();

   return 0;
}

void testMainProgram()
{
   const int num_buttons = num_pins;

   // allows us to instantiate windows and buttons
   AbstractGUIFactory* factory = nullptr;

   #ifdef osWINDOWS
      factory = new WinGUIFactory();
   #elif defined osLINUX
      factory = new LinGUIFactory();
   #endif

   // input window
   auto input = factory->createWindow();
   input->setName( "Inputs" );

   // input buttons
   for( int i = num_buttons - 1; i >= 0; i-- )
   {
      auto b = factory->createButton();
      b->setName( "In  " + std::to_string( i ) );
      b->setState( BUTTON_STATE::ON );
      input->addButton( std::move( b ) );
   }

   // output window
   auto output = factory->createWindow();
   output->setName( "Outputs" );

   // output buttons
   for( int i = num_buttons - 1; i >= 0; i-- )
   {
      auto b = factory->createButton();
      b->setName( "Out " + std::to_string( i ) );
      b->setValue( "0" );
      output->addButton( std::move( b ) );
   }

   // read user input
   std::cout << "Enter a 4-bit mask as a set of space\n"
             << " separated 1 or 0 values\n"
             << " High bit to low bit, ex:  \"1 1 1 1\"\n"
             << " This mask will be applied\n"
             << " to all 4 hardware input pins: ";

   std::vector< int > mask( num_buttons );

   for( int i = 0; i < num_buttons; i++ )
      std::cin >> mask[ i ];

   // this eats any extra white space of input characters after the mask has been read in.
   std::cout << "Ready to mask digital inputs. Iterating\n"
             << " through all pin inputs now...\n";
   std::string s;
   std::getline( std::cin, s );

   // apply the mask to the output buttons
   for( auto i = 0; i < num_buttons; i++ )
      output->getButtons()[ i ]->setState( mask[ i ] == 1 ? BUTTON_STATE::ON : BUTTON_STATE::OFF );

   /* ----- Patterns: Observer, Adapter
      Here, we see the Observer pattern. Input buttons subscribe to the hardware pins, and, when a pin's
      value changes, it notifies all subscribers. Its subscribed input button can then update its value.

      We use a lambda to provide this functionality. Because the classes are necessarily different, we must apply the
      Adapter pattern. In doing so, we are able to convert a pin's "high" or "low" value to a button's "1" or "0" value.
   */
   for( int i = 0; i < num_buttons; i++ )
   {
      // callback to change button value on hardware pin change
      auto onPinChange = [ i, &input ]( const std::array< PIN_VAL, num_pins >& pins )
      {
         input->getButtons()[ i ]->setValue( pins[ i ] == PIN_VAL::HIGH ? "1" : "0" );
      };

      FourPinExternInput::listenForPinChange( onPinChange );
   }

   /* ----- Patterns: Observer
      This is another instance of the Observer pattern. Each output button subscribes to its corresponding input button. When
      an input button changes its value, it notifies all subscribers (in this case, it notifies its corresponding output button)
      of the change. The output button can then update its own value.

      Of course, we don't want that hard-coded, so we use a lambda to provide this functionality. This lambda captures an input
      button and its corresponding output button. In doing so, we ensure that none of the buttons update each other. Instead,
      the lambda updates values as necessary.
   */
   for( int i = 0; i < num_buttons; i++ )
   {
      // callback to change output button value on input value change
      auto onValueChange = [ i, &input, &output ]()
      {
         if( output->getButtons()[ i ]->getState() == BUTTON_STATE::ON )
            output->getButtons()[ i ]->setValue( input->getButtons()[ i ]->getValue() );
      };

      input->getButtons()[ i ]->listenForValueChange( onValueChange );
   }

   for( int i = 0; i < 17; i++ )
   {
      FourPinExternInput::receiveNewPinState();
      input->draw( std::cout );
      output->draw( std::cout );
      std::cout << std::endl;
      std::cin.get();
   }

   delete factory;
   drawAscii();
}

void testDrawingWindowsAndButtons()
{
   //Do NOT touch any code in this function. Your output must identically match my output.
   AbstractGUIFactory* f = nullptr;

#ifdef osWINDOWS
   f = new WinGUIFactory();
#elif defined osLINUX
   f = new LinGUIFactory();
#endif

   auto w = f->createWindow();
   w->setName( "This is a big, long name" );

   {
      auto b = f->createButton();
      w->addButton( b );
      b->setName( "My Big Button name" );
      b->setState( BUTTON_STATE::OFF );
      b->setValue( "This is my value as a button..." );
   }

   w->draw( std::cout );

   {
      auto b = f->createButton();
      w->addButton( b );
      b->setName( "Small" );
      b->setState( BUTTON_STATE::ON );
      b->setValue( "Short" );
   }

   {
      auto b = f->createButton();
      w->addButton( b );
      b->setName( "My other button's name" );
      b->setState( BUTTON_STATE::ON );
      b->setValue( "Here is another value" );
   }

   w->draw( std::cout );

   w->setName( "This is a very long title, it should be longer than the buttons..." );
   w->draw( std::cout );
   std::cin.get();

   delete f;
}

void drawAscii()
{
   std::cout << "...\n\n";

   std::ifstream in( "logo.txt" );
   std::string str;

   while( std::getline( in, str ) )
      std::cout << str << std::endl;

   std::cout << "\n";
}
