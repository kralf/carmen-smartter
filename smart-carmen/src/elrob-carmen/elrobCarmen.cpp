#include "Messages.h"
#include <rotor/RemoteRegistry.h>
#include <rotor/BaseOptions.h>
#include <rotor/NetUtils.h>
#include <rotor/Conversion.h>
#include <rotor/Logger.h>
#include <rotor/Time.h>
#include <cmath>
#include <iostream>
#include <string>

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

void initializeRotor( Registry & registry )
{
  registry.registerType( ROTOR_DEFINITION_STRING( carmen_point_t ) ); 
  registry.registerMessageType( "carmen_localize_globalpos", ROTOR_DEFINITION_STRING( carmen_localize_globalpos_message ) );
  registry.subscribeToMessage( "carmen_localize_globalpos" );
  registry.registerMessageType( "carmen_base_odometry", ROTOR_DEFINITION_STRING( carmen_base_odometry_message ) );
  registry.subscribeToMessage( "carmen_base_odometry" );
  registry.registerMessageType( "axt_message", ROTOR_DEFINITION_STRING( axt_message ) );
  registry.subscribeToMessage( "axt_message" );

  registry.registerType( ROTOR_DEFINITION_STRING( carmen_laser_laser_config_t ) );
  registry.registerMessageType( "carmen_robot_frontlaser", ROTOR_DEFINITION_STRING( carmen_robot_laser_message ) );
}

//------------------------------------------------------------------------------

int main( int argc, char * argv[] )
{
  BaseOptions options;
  options.setInt( "elrob_carmen", "loggingLevel", 3 );
  RemoteRegistry registry( "CarmenRegistry", "elrob_carmen", options, "lib" );
  initializeRotor( registry );
  
  char host[100];
  strcpy( host, hostName().c_str() );

  carmen_localize_globalpos_message global;
  carmen_base_odometry_message  odometry;
  Structure sLaser     = registry.newStructure( "carmen_robot_laser_message" );

  double laserDistance = 1.812 + 0.45;
  double rayCount      = 181;
  for(;;) {
    try {
      Message msg       = registry.receiveMessage( 0.5 );
      Structure & sData = msg.data();
      if ( msg.name() == "carmen_base_odometry" ) {
        Logger::spam( "Got localization message" );
        odometry = * reinterpret_cast<carmen_base_odometry_message *>( sData.buffer() );
      } else if ( msg.name() == "carmen_localize_globalpos" ) {
        Logger::spam( "Got globalpos message" );
        global = *reinterpret_cast<carmen_localize_globalpos_message *>( sData.buffer() );
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
          if ( data.channel[i] == 2 ) {
            size_t index = int( ( atan2( data.y[i], data.x[i] ) + M_PI / 2.0 ) * (rayCount - 1 ) / M_PI );
            double r = sqrt( pow( data.x[i], 2 ) + pow( data.y[i], 2 ) );
            if ( r < laser.range[index] ) {
              laser.range[index] = r;
            }
          }
        }
/*        laser.laser_pose.theta = global.globalpos.theta;
        laser.laser_pose.x     = global.globalpos.x + laserDistance * cos( laser.laser_pose.theta );
        laser.laser_pose.y     = global.globalpos.y + laserDistance * sin( laser.laser_pose.theta );
        
        laser.robot_pose.theta = global.globalpos.theta;
        laser.robot_pose.x     = global.globalpos.x;
        laser.robot_pose.y     = global.globalpos.y;*/
        
        laser.laser_pose.theta = odometry.theta;
        laser.laser_pose.x     = odometry.x + laserDistance * cos( laser.laser_pose.theta );
        laser.laser_pose.y     = odometry.y + laserDistance * sin( laser.laser_pose.theta );
        
        laser.robot_pose.theta = odometry.theta;
        laser.robot_pose.x     = odometry.x;
        laser.robot_pose.y     = odometry.y;
        
        laser.tv               = odometry.tv;
        laser.rv               = odometry.rv;
        laser.turn_axis        = 2;
        
        laser.timestamp    = seconds();
        registry.sendStructure( "carmen_robot_frontlaser", sLaser );
        Logger::spam( "Carmen laser message has been sent" );
      }
    } catch ( MessagingTimeout & e ) {
    }
  }
}
