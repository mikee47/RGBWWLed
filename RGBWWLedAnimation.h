/**
 * @file
 * @author  Patrick Jahns http://github.com/patrickjahns
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 *
 */

#ifndef RGBWWLedAnimation_h
#define RGBWWLedAnimation_h
#include "RGBWWLed.h"
#include "RGBWWLedColor.h"


class RGBWWLed;
class RGBWWLedAnimation;

class RGBWWLedAnimationQ
{
public:
	RGBWWLedAnimationQ() {};
	RGBWWLedAnimationQ(int qsize);
	~RGBWWLedAnimationQ();
	bool                isEmpty();
	bool                isFull();
	bool                push(RGBWWLedAnimation* animation);
	void				clear();
	RGBWWLedAnimation*  peek();
	RGBWWLedAnimation*  pop();


private:
	int _size, _count, _front, _back;
	RGBWWLedAnimation** q;


};

class RGBWWLedAnimation
{
public:
	virtual bool run() {return true;};
	virtual bool run(int st) {return true;};
	virtual ~RGBWWLedAnimation() {};
	virtual void setSpeed(int newspeed) {};
	virtual void setBrightness(int newbrightness) {};
};

class HSVSetOutput: public RGBWWLedAnimation
{
public:
	HSVSetOutput() {};
	HSVSetOutput(const HSVK& color, RGBWWLed* rgbled);
	void        init();
	bool        run();
	bool        run(int st);


private:
	RGBWWLed*    rgbled;
	HSVK         outputcolor;
};

class HSVTransition: public RGBWWLedAnimation
{
public:
	HSVTransition() {};
	HSVTransition(const HSVK& color, const int& tm, const int& direction, RGBWWLed* rgbled);
	HSVTransition(const HSVK& colorFrom, const HSVK& color, const int& tm, const int& direction, RGBWWLed* rgbled);
	void        init();
	bool        run();
	bool        run(int st);

private:
	HSVK        _basecolor;
	HSVK        _currentcolor;
	HSVK        _finalcolor;
	bool        _hasbasecolor;
	int         _currentstep;
	int         _steps;
	int         _huedirection;
	int         _dhue;
	int         _hueerror;
	int         _huecount;
	int         _huestep;
	int         _dsat;
	int         _saterror;
	int         _satcount;
	int         _satstep;
	int         _dval;
	int         _valerror;
	int         _valcount;
	int         _valstep;
	int         _dkelvin;
	int         _kelvinerror;
	int         _kelvincount;
	int         _kelvinstep;

	RGBWWLed*    rgbled;

	int         bresenham(int& error, int& ctr, int& dx, int& dy, int& incr, int& base, int& current);

};

#endif // RGBWWLedAnimation_h
