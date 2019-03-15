#pragma once

#include "MotionStrategy.h"

namespace Aftr
{

//Use physics, calculus, algebra, and grit to compute the new position...
//Also, ensure the bool return parameter gets set to TRUE once the missile reaches its target; until that time, it needs to be FALSE.
//Read up on std::tuple and std::tie to learn how C++ can return multiple parameters.
//Also, ensure this code doesn't divide by zero or behave unexpectedly if bad input is given.
//Also, you are computing stuff with doubles. At the end of this method use static_cast<float>( myDouble ) to cast a double to 
//a float.
std::tuple<Vector,bool> MotionParabolic::computePosition( double dt_sec, double range_m, double totalTime_sec, double headingDeg )
{
   if (totalTime_sec <= 0)
      totalTime_sec = ZERO;

   double horz_velocity = range_m / totalTime_sec; // x = Vt --> V = x / t
   double horz_distance = horz_velocity * dt_sec;

   double x_loc = horz_distance * cos(headingDeg * PI / 180); // cos(θ) = x / s
   double y_loc = -1 * horz_distance * sin(headingDeg * PI / 180); // sin(θ) = y / s

   double vert_velocity = -1 * GRAVITY_ACCELERATION * (totalTime_sec / 2); // Vf = 0 = Vi + at --> Vi = -at
   double z_loc = dt_sec * (vert_velocity + (GRAVITY_ACCELERATION / 2) * dt_sec); // s = Vi * t + 1/2 * at^2 = t * (Vi + 1/2(gt))

   return std::make_tuple(
       Vector{static_cast<float>(x_loc), static_cast<float>(y_loc), static_cast<float>(z_loc)},
       dt_sec >= totalTime_sec ? true : false
    );
}


//You've already computed everything you need to find this value in the method above. You can just recompute it here.
double MotionParabolic::getLaunchAngle( double range_m, double totalTime_sec )
{
   if (totalTime_sec <= 0)
      totalTime_sec = ZERO;

   double vert_velocity = -1 * GRAVITY_ACCELERATION * (totalTime_sec / 2); // Vf = 0 = Vi + at --> Vi = -at
   double horz_velocity = range_m / totalTime_sec; // x = Vt --> V = x / t

   return atan(vert_velocity / horz_velocity);
}

std::string MotionParabolic::toString( double range_m, double totalTime_sec, double headingDeg ) const
{
   //Expected output looks like this:
   //Printing Trajectory info...
   //V_horz( m / s ) is  22.5166
   //V_vert( m / s ) is  12.9983
   //Max Alt( m )  is 8.61134
   //Landing Position is( 59.669, 0.000, 0.000 )

   //do some math here...
   if (totalTime_sec <= 0)
      totalTime_sec = ZERO;

   double V_horz = range_m / totalTime_sec; // V = s / t
   double V_vert = -1 * GRAVITY_ACCELERATION / 2 * totalTime_sec; // s = 0 = Vi * t + 1/2 * at^2 --> Vi = -at / 2
   double maxAltM = -1 * V_vert * V_vert / (GRAVITY_ACCELERATION * 2); // Vf^2 = 0 = Vi^2 + 2as --> s = -Vi^2 / 2a
   double xComp = range_m * cos(headingDeg * PI / 180); // cos(θ) = x / s
   double yComp = -1 * range_m * sin(headingDeg * PI / 180); // sin(θ) = y / s

   //The output below can eventually be commented in and used, once those variables are declared and populated.

   //use a std::stringstream object to print out the following information
   std::stringstream ss;
   ss << "V_horz (m/s) is  " << V_horz << "\n";
   ss << "V_vert (m/s) is  " << V_vert << "\n";
   ss << "Max Alt (m)  is " << maxAltM << "\n";
   ss << "Landing Position is " << Vector{ static_cast<float>( xComp ), static_cast<float>( yComp ), 0 }.toString() << "\n";

   return ss.str();
}

} //namespace Aftr

