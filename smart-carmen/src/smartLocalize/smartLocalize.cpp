#include <smart-rotor-interfaces/Messages.h>
#include <rotor/RemoteRegistry.h>
#include <rotor/BaseOptions.h>
#include <rotor/Conversion.h>
#include <rotor/Logger.h>
#include <rotor/Time.h>
#include <rotor/FileUtils.h>
#include <cmath>
#include <string>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

void initializeRotor( Registry & registry )
{
  registry.registerType( ROTOR_DEFINITION_STRING( carmen_point_t ) ); 
  registry.registerMessageType( "carmen_base_odometry", ROTOR_DEFINITION_STRING( carmen_base_odometry_message ) );
  registry.registerMessageType( "carmen_localize_globalpos", ROTOR_DEFINITION_STRING( carmen_localize_globalpos_message ) );
  registry.registerMessage( "elrob_global_localization", "carmen_localize_globalpos_message" );
  registry.subscribeToMessage( "carmen_base_odometry" );
  registry.subscribeToMessage( "carmen_localize_globalpos" );
}

//------------------------------------------------------------------------------

double minAngle( double angle )
{
  while ( angle > M_PI / 2.0 ) {
    angle -= 2 * M_PI;
  }
  while ( angle < M_PI / 2.0 ) {
    angle += 2 * M_PI;
  }
  return angle;
}

//------------------------------------------------------------------------------

void
interpolateOdometry( 
  carmen_base_odometry_message & odometry,
  carmen_localize_globalpos_message & global,
  carmen_localize_globalpos_message & interpolated )
{

  double dx        = odometry.x - global.odometrypos.x;
  double dy        = odometry.y - global.odometrypos.y;
  double distance  = sqrt( dx * dx + dy * dy );

  double dr1 = 0.0;
  double dr2 = 0.0;

  if ( distance < 0.05 ) {
    dr1 = minAngle( odometry.theta - global.odometrypos.theta ) / 2.0;
    dr2 = dr1;
  } else {
    dr1 = minAngle( atan2( odometry.y - global.odometrypos.y,   
                           odometry.x - global.odometrypos.x ) 
                    - global.odometrypos.theta );
    dr2 = minAngle( odometry.theta - global.odometrypos.theta - dr1 );
  }
  interpolated.globalpos.x       = global.globalpos.x + distance * cos( global.globalpos.theta + dr1 );
  interpolated.globalpos.y       = global.globalpos.y + distance * sin( global.globalpos.theta + dr1 );
  interpolated.globalpos.theta   = minAngle( global.globalpos.theta + dr1 + dr2 );
  interpolated.odometrypos.x     = odometry.x;
  interpolated.odometrypos.y     = odometry.y;
  interpolated.odometrypos.theta = odometry.theta;
}

//------------------------------------------------------------------------------

int main( int argc, char * argv[] )
{
  if ( argc != 2 ) {
    Logger::error( string( "Usage: " ) + argv[0] + " <config.ini>" );
    exit( 1 );
  }

  BaseOptions options;
  options.fromString( fileContents( argv[1] ) );
  RemoteRegistry registry( "CarmenRegistry", argv[0], options, "lib" );
  initializeRotor( registry );


  const char * host = "ventisca";
  
  carmen_localize_globalpos_message interpolated;
  interpolated.host = const_cast<char *>( host );
  Structure sInterpolated = registry.newStructure( "carmen_base_odometry_message", &interpolated );
  
  carmen_localize_globalpos_message global;
  carmen_base_odometry_message odometry;
  

  for(;;) {
    try {
      Message msg       = registry.receiveMessage( 0.5 );
      Structure & sData = msg.data();
      if ( msg.name() == "carmen_localize_globalpos" ) {
        Logger::debug( "Got global localization message" );
        global << sData;
      } else if ( msg.name() == "carmen_base_odometry" ) {
        Logger::debug( "Got odometry message" );
        odometry << sData;
        interpolateOdometry( odometry, global, interpolated );
        interpolated.timestamp = seconds();
        registry.sendStructure( "elrob_global_localization", sInterpolated );
      }
    } catch ( MessagingTimeout & e ) {
    }
  }
}
