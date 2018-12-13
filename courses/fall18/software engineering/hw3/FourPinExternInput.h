/* ----- Patterns: Singleton, Observer
   This class is an example of the Singleton pattern. We can't - and shouldn't - create FourPinExternInput instances.
   Instead of declaring any objects, we maintain this class and use its functions like in the Singleton pattern. In
   doing so, we ensure we don't have multiple sets of hardware pins, and this guarantees the correctness of our windows.

   Additionally, we see pieces of the Observer pattern in this class. As explained in main.cpp, we use the Observer
   pattern to subscribe input buttons to the hardware pins. We use FourPinExternInput::onPinChangeCallbacks and
   FourPinExternInput::listenForPinChange() to complete the work started elsewhere (that is, to contain the "glue code"
   used to notify and update buttons).
*/

#pragma once

#include <array>
#include <functional>
#include <vector>

namespace HW3
{

const int num_pins = 4;

enum class PIN_NUM : int { PIN_0 = 0, PIN_1, PIN_2, PIN_3 };
enum class PIN_VAL : int { LOW = 0, HIGH };

using pinChangeCallback = std::function< void( const std::array< PIN_VAL, num_pins > & pins ) >;

class FourPinExternInput
{
public:
   //Reads the input pins again. For simplicity, we simulate a new value by incrementing an internal counter that wraps around the domain [0,15] (4 bits).
   //Do NOT modify this method
   static void receiveNewPinState();

   //Implement a method called listenForPinChange( ... ));
   //It should take as a parameter a function that consumes an array of PIN_VAL. When a change to the pins occurs,
   //each function passed to this method should be called back and be passed the latest pin state.
   static void listenForPinChange( const pinChangeCallback& callback );
   
private:
   static std::array< PIN_VAL, num_pins > pins;
   static std::vector< pinChangeCallback > onPinChangeCallbacks;
};

}
