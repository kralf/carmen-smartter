#include <smart-rotor-interfaces/Messages.h>
#include <rotor/RemoteRegistry.h>
#include <rotor/BaseOptions.h>
#include <rotor/NetUtils.h>
#include <rotor/FileUtils.h>
#include <rotor/Conversion.h>
#include <rotor/Logger.h>
#include <rotor/Time.h>
#include <cmath>
#include <iostream>
#include <string>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

void initializeRotor( Registry & registry, const string & poseMessage )
{
  registry.registerType( ROTOR_DEFINITION_STRING( carmen_point_t ) );

  registry.registerMessageType(
    "carmen_base_odometry",
    ROTOR_DEFINITION_STRING( carmen_base_odometry_message )
  );
  registry.subscribeToMessage( "carmen_base_odometry", true );

  if ( poseMessage == "locfilter_filteredpos_message" )
  {
    registry.registerMessageType(
      "locfilter_filteredpos_message",
      ROTOR_DEFINITION_STRING( locfilter_filteredpos_message )
    );
    registry.subscribeToMessage( "locfilter_filteredpos_message", true );
  }

  registry.registerMessageType(
    "axt_message",
    ROTOR_DEFINITION_STRING( axt_message )
  );
  registry.subscribeToMessage( "axt_message" );

  registry.registerType( ROTOR_DEFINITION_STRING( carmen_laser_laser_config_t ) );
  registry.registerMessageType(
    "carmen_robot_frontlaser",
    ROTOR_DEFINITION_STRING( carmen_robot_laser_message )
  );
}

//------------------------------------------------------------------------------

int main( int argc, char * argv[] )
{
  if ( argc < 2 )
  {
    cout << "Usage: " << argv[0] << " <config.ini>\n";
    exit( 1 );
  }

  string moduleName( argv[0] );

  BaseOptions options;
  options.fromString( fileContents( argv[1] ) );
  RemoteRegistry registry( "CarmenRegistry", moduleName, options, "lib" );
  string poseMessage = options.getString( moduleName, "poseMessage" );

  initializeRotor( registry, poseMessage );

  char host[100];
  strcpy( host, hostName().c_str() );

  carmen_point_t pose = { 0, 0, 0 };
  double tv = 0;
  double rv = 0;

  Structure sLaser     = registry.newStructure( "carmen_robot_laser_message" );
  sLaser["host"]       = host;

  double laserDistance = 1.812 + 0.45;
  double rayCount      = 181;
  for(;;) {
    try {
      Message msg       = registry.receiveMessage( 0.5 );
      Structure & sData = msg.data();
      if ( msg.name() == "carmen_base_odometry" ) {
        Logger::spam( "Got pose message" );
        pose.x = sData["x"];
        pose.y = sData["y"];
        pose.theta = sData["theta"];
        tv = sData["tv"];
        rv = sData["rv"];
      } else if ( msg.name() == "locfilter_filteredpos_message" ) {
        Logger::spam( "Got pose message" );
        pose = ROTOR_VARIABLE( carmen_point_t, sData["filteredpos"] );
      } else if ( msg.name() == "axt_message" ) {
        Logger::spam( "Got alasca message" );
        axt_message & data     = * reinterpret_cast<axt_message*>( sData.buffer() );

        sLaser["num_readings"]   = 181;
        sLaser["num_remissions"] = 0;
        sLaser.adjust();
        carmen_robot_laser_message & laser = * reinterpret_cast<carmen_robot_laser_message *>( sLaser.buffer() );
        laser.id             = 1;

        laser.config.laser_type         = 99;
        laser.config.start_angle        = -M_PI / 2.0;
        laser.config.angular_resolution = M_PI / (rayCount - 1);
        laser.config.fov                = M_PI;
        laser.config.maximum_range      = 80;
        laser.config.accuracy           = 0.1;
        laser.config.remission_mode     = 0;

        for ( size_t i = 0; i < rayCount; ++i ) {
          laser.range[i]    = laser.config.maximum_range * 2.0;
          laser.tooclose[i] = 0;
        }
        for ( size_t i = 0; i < data.num_points; ++i ) {
          if ( ( data.channel[i] == 2 ) && ( !data.point_status[i] ) ) {
            size_t index = int( ( atan2( data.y[i], data.x[i] ) + M_PI / 2.0 ) * (rayCount - 1 ) / M_PI );
            double r = sqrt( pow( data.x[i], 2 ) + pow( data.y[i], 2 ) );
            if ( r < laser.range[index] ) {
              laser.range[index] = r;
            }
          }
        }

        laser.laser_pose.theta = pose.theta;
        laser.laser_pose.x     = pose.x + laserDistance * cos( laser.laser_pose.theta );
        laser.laser_pose.y     = pose.y + laserDistance * sin( laser.laser_pose.theta );

        laser.robot_pose.theta = pose.theta;
        laser.robot_pose.x     = pose.x;
        laser.robot_pose.y     = pose.y;

        laser.tv               = tv;
        laser.rv               = rv;
        laser.turn_axis        = 2;

        laser.timestamp    = seconds();
        registry.sendStructure( "carmen_robot_frontlaser", sLaser );
        Logger::spam( "Carmen laser message has been sent" );
      }
    } catch ( MessagingTimeout & e ) {
    }
  }
}
