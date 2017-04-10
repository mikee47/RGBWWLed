#pragma once

#include "../../SmingCore/SmingCore.h"
#include "RGBWWconst.h"


class AbsOrRelValue {
public:
	enum class Type {
		Raw,   // 0 - 1023
		Hue,   // -> 0 - 360
		Percent, // 0 - 100
		Ct // no check
	};

	enum class Mode {
		Absolute,
		Relative,
	};

	AbsOrRelValue() {
	}

	AbsOrRelValue(const char* value, Type type = Type::Percent) : _type(type){
	    Serial.printf("ABS: %s\n", value);
		String str;
		str.setString(value);
		float val;
		if (str.startsWith("+") || str.startsWith("-")) {
	        Serial.printf("ABS2-1\n");
			_mode = Mode::Relative;
			val = str.substring(1).toFloat();
			if (str.startsWith("-"))
			    val *= -1;
		}
		else {
			_mode = Mode::Absolute;
			val = str.toFloat();
            Serial.printf("ABS2-2: %f\n", val);
		}
        Serial.printf("ABS2-3: %d\n", type);

        setValueByType(val);

		Serial.printf("ABS4: %d\n", _value);
	}

    AbsOrRelValue(const AbsOrRelValue& o) {
        _value = o.getValue();
        _mode = o.getMode();
        _type = o.getType();
    }

    /**
     * Accepts CHANNEL output range value
     */
    AbsOrRelValue(int value) {
        _value = value;
    }

    /**
     * Accepts CHANNEL output range value
     */
    AbsOrRelValue(int value, Mode mode) : _mode(mode) {
        _value = value;
    }

    /**
     * Accepts INPUT range, will be converted to output range value
     */
    AbsOrRelValue(int value, Type type) : _type(type) {
       setValueByType(value);
    }


    bool operator==(const AbsOrRelValue& obj) const {
        return (_mode == obj.getMode()) && (this->_value == obj.getValue());
    }

    operator int() const {
        return _value;
    }

    Type getType() const {
        return _type;
    }

	Mode getMode() const {
		return _mode;
	}

	int getValue() const {
		return _value;
	}

	int getFinalValue(int val) {
		switch(_mode) {
		case Mode::Relative:
			return fixRangeLimits(_value + val);
		case Mode::Absolute:
			return _value;
		}
	}

private:
	void setValueByType(float value) {
        switch(_type) {
        case Type::Hue:
        	value = (value / 360.0) * RGBWW_CALC_HUEWHEELMAX;
            break;
        case Type::Percent:
        	value = (value / 100.0) * RGBWW_CALC_MAXVAL;
            break;
        }

        Serial.printf("setValueByType: %f\n", value);
        _value = static_cast<int>(value + 0.5f);

        if (_mode != Mode::Relative)
            _value = fixRangeLimits(_value);
	}

	int fixRangeLimits(int val) {
	    switch(_type) {
        case Type::Raw:
            val = constrain(val, 0, 1023);
            break;
        case Type::Hue:
            if (val < 0)
                val = RGBWW_CALC_HUEWHEELMAX + val;
            else
                val = val % RGBWW_CALC_HUEWHEELMAX;
            break;
        case Type::Percent:
            val = constrain(val, 0, RGBWW_CALC_MAXVAL);
            break;
        }
	    return val;
	}

	AbsOrRelValue::Mode _mode = Mode::Absolute;
	Type _type = Type::Percent;
	int _value = 0;
};

template< typename T >
class Optional {
public:
	Optional() : _hasValue(false) {
	}

	Optional<T>(T value) : _value(value), _hasValue(true) {
	}

	T& operator=(const T& obj) {
		_value = obj;
		_hasValue = true;
	}

	const T& getValue() const {
		return _value;
	}

    bool operator==(const Optional<T>& obj) const {
        return (this->hasValue() == obj.hasValue()) && (this->getValue() == obj.getValue());
    }

    bool operator==(const T& obj) const {
        return this->hasValue() && (_value == obj);
    }

    bool operator!=(const T& obj) const {
        return !(*this == obj);
    }

    bool operator<(const T& obj) const {
        return this->hasValue() && (_value < obj);
    }

    bool operator>(const T& obj) const {
        return this->hasValue() && (_value > obj);
    }

    operator const T&() const {
    	return _value;
    }

	void clear() {
		_hasValue;
	}

	bool hasValue() const {
		return _hasValue;
	}
private:
	T _value;
	bool _hasValue = false;
};

enum class ColorMode {
	Hsv,
	Raw,
};

enum class CtrlChannel {
	None,

    Hue,
    Sat,
    Val,
    ColorTemp,

    Red,
    Green,
    Blue,
    ColdWhite,
    WarmWhite,
};

struct BresenhamValues {
    int delta, error, count, step;
};

enum class QueuePolicy {
    Invalid,
    FrontReset, // queue to front and let the original anim run from the beginning afterwards
    Front, // queue to front and the current animation will continue where it was interrupted
    Back,
    Single,
};

