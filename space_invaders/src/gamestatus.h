/*
 * gamestatus.h
 *
 *  Created on: Oct 15, 2014
 *      Author: superman
 */

#ifndef GAMESTATUS_H_
#define GAMESTATUS_H_

#define HIT_JUMPING_JACK 0
#define HIT_BIG_SQUID 1
#define HIT_SMALL_SQUID 2
#define HIT_RED_SPACESHIP 3

#define POINTS_FOR_JUMPING_JACK 20
#define POINTS_FOR_BIG_SQUID 10
#define POINTS_FOR_SMALL_SQUID 40
#define LOW_SCORE_FOR_RED_ALIEN 50
#define HIGH_SCORE_FOR_RED_ALIEN 300
#define SCORE_INCREMENTS_FOR_RED_ALIEN 50

#define INITIAL_SCORE 0
#define INITIAL_LIVES 3
#define MAX_LIVES 6

int getRedSpaceshipScore();



#endif /* GAMESTATUS_H_ */
