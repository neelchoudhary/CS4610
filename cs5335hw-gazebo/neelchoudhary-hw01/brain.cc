
#include <iostream>
#include <math.h>

#include "robot.hh"

using std::cout;
using std::endl;

const double goal_x = 20.0;
const double goal_y = 0.0;
bool done = false;

void
callback(Robot* robot)
{
    /*
    cout << endl;
    cout << "robot x =" << robot->pos_x << endl;
    cout << "robot y =" << robot->pos_y << endl;
    */
    double dx = goal_x - robot->pos_x;
    double dy = goal_y - robot->pos_y;
    if (abs(dx) < 0.75 && abs(dy) < 0.75) {
        cout << "we win!" << endl;
        robot->set_vel(0.0);
        robot->set_turn(0.0);
        robot->done();
        return;
    }

    double goal_angle = atan2(dy, dx);

    bool turn = false;
    bool turnEnd = false;

    for (LaserHit hit : robot->hits) {
        if (hit.range < 3) {
		if (hit.angle < .17 && hit.angle > -.17) {
			turn = true;
		}
		if (hit.angle >= .17 && hit.angle < 1.57) {
			turnEnd = true;
		}
        }
    }

    if (turn) {
        robot->set_vel(4.0);
        robot->set_turn(0.8);
    } else if (turnEnd) {
	robot->set_vel(15.0);	
	robot->set_turn(0);
    }
    else {
	robot->set_vel(15.0);
	double turn_dir = goal_angle - robot->pos_t;
	double buffer = .2;
	if (turn_dir > buffer) {
		// turn left
		robot->set_turn(-.5);
	} else if (turn_dir < -1 * buffer) {
		// turn right
		robot->set_turn(.5);
	} else {
		// go straight
		robot->set_turn(0);
	}
    }
}

int
main(int argc, char* argv[])
{
    cout << "making robot" << endl;
    Robot robot(argc, argv, callback);
    robot.do_stuff();

    return 0;
}
