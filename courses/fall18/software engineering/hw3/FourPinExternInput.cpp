#include "FourPinExternInput.h"

#include <iostream>
#include <array>
#include <functional>

namespace HW3
{

std::array< PIN_VAL, num_pins > FourPinExternInput::pins;
std::vector< pinChangeCallback > FourPinExternInput::onPinChangeCallbacks;

void FourPinExternInput::listenForPinChange( const pinChangeCallback& callback )
{
   FourPinExternInput::onPinChangeCallbacks.push_back( callback );
}

void FourPinExternInput::receiveNewPinState()
{
   //This internally simulates reading from pins... It sets the internal pins to a
   //new value via idx. It is an incrementing 4 bit number. We interpret it as unsigned.

   static constexpr PIN_VAL o = PIN_VAL::LOW;
   static constexpr PIN_VAL l = PIN_VAL::HIGH;

   static std::array< std::array< PIN_VAL, num_pins >, num_pins * num_pins > data =
   { { { o,o,o,o }, //0
       { o,o,o,l }, //1
       { o,o,l,o }, //2
       { o,o,l,l }, //3
       { o,l,o,o }, //4
       { o,l,o,l }, //5
       { o,l,l,o }, //6
       { o,l,l,l }, //7
       { l,o,o,o }, //8
       { l,o,o,l }, //9
       { l,o,l,o }, //10
       { l,o,l,l }, //11
       { l,l,o,o }, //12
       { l,l,o,l }, //13
       { l,l,l,o }, //14
       { l,l,l,l }  //15
   } };

   static int idx = -1;

   idx = ( idx + 1 ) % data.size();

   //idx represents the index of the new value read from the FourPinExternalInput::pins. Let's set the FourPinExternalInput::pins to the value at data[idx]
   pins = data[ idx ];

   //All our pins have changed, time to notify any subscribers who want to observe this event...
   for( auto& callback : onPinChangeCallbacks )
      callback( pins );
}

}
