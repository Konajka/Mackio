/**
 * @brief Library for easy use of rotary encoder with rotation velocity support.
 *
 * @author https://github.com/Konajka
 * @version 0.1 2019-05-13
 *      Base implementation
 * @version 0.2 2019-05-13
 *      Base rotation support.
 *      Base switch support.
 * @version 0.3 2019-05-13
 *      Added click and long click support.
 * @version 0.4 2019-05-14
 *      Added velocity support.
 * @version 0.5 2019-06-21
 *      Removed ROTARY_ENCODER_CLICKS_SUPPORT direcitve.
 * @version 0.6 2019-07-01
 *      Fixed blind click firing after long click fired.
 * @version 0.7 2019-07-03
 *      Added switch debouncing.
 *      Updated doc comments.
 * @version 1.0 2019-07-04
 *      Stable version
 */

#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include <Arduino.h>

// Long click minimal time
#define ROTARY_ENCODER_LONG_CLICK_MILLIS 450

//
#define ROTARY_ENCODER_SWITCH_DEBOUNCE_TIME 30

// Direction definition
enum RotaryEncoderDirection { left, right };
struct RotaryEncoderOnChangeEvent {
    const RotaryEncoderDirection direction;
    const word velocity;
};
// Direction change event
typedef void (*RotaryEncoderOnChange) (RotaryEncoderOnChangeEvent);

// Switch state definition
enum RotaryEncoderSwitchAction { press, release };
struct RotaryEncoderOnSwitchEvent {
    const RotaryEncoderSwitchAction action;
};
// Switch state change event
typedef void (*RotaryEncoderOnSwitch) (RotaryEncoderSwitchAction);

// Click and long-click event
typedef void (*RotaryEncoderOnClick) ();
typedef void (*RotaryEncoderOnLongClick) ();

/**
 * @brief Rotary encoder controller class.
 */
class RotaryEncoder 
{
    private:
        // Initialization flag
        bool initialized = false;
    
        // PIN definitions
        uint8_t _PIN_CLOCK;
        uint8_t _PIN_DATA;
        uint8_t _PIN_SWITCH;
        
        // Last values memory
        int _pinClockRetain;
        int _pinDataRetain;
        int _pinSwitchRetain;
        
        // Events
        RotaryEncoderOnChange _onChange = NULL;
        RotaryEncoderOnSwitch _onSwitch = NULL;
        RotaryEncoderOnClick _onClick = NULL;
        RotaryEncoderOnLongClick _onLongClick = NULL;
        
        // Measuring velocity
        unsigned long _velocityTime = 0;
                
        // Long click time measuring
        unsigned long _switchPressTime = 0;

        // Long click event fired flag
        bool _longClickFired = false;

        // Switch debounce time measure
        long _lastDebounceSwitchTime;

        // Switch debouncing state
        int _lastDebounceSwitchState = HIGH;

        /**
         * @brief Gets switch state after debouncing.
         * @param state Debounced state is set after debouncing.
         * @return Returns true if switch click is debounced and final state stored into
         * state or false if not debounced yet.
         */
        bool getDebouncedSwitchState(int &state) {
            int debounceSwitchState = digitalRead(_PIN_SWITCH);

            if (debounceSwitchState != _lastDebounceSwitchState) {
                _lastDebounceSwitchTime = millis();
            }
            _lastDebounceSwitchState = debounceSwitchState;

            long switchDebounceTime = millis() - _lastDebounceSwitchTime;
            if (switchDebounceTime > ROTARY_ENCODER_SWITCH_DEBOUNCE_TIME) {
                state = debounceSwitchState;
                return true;
            } else {
                return false;
            }
        }

    protected:
    
        /**
         * @brief Calls onChange event if assigned.
         * @param direction Rotation direction info.
         * @param velocity Time spend from last event in millis.
         */ 
        void doOnChange(RotaryEncoderDirection direction, word velocity) {
            if (_onChange != NULL) {
                RotaryEncoderOnChangeEvent event = {
                    direction,
                    velocity
                };
                _onChange(event);
            }
        }

        /**
         * @brief Calls onSwitch event if assigned.
         * @param action Encoder switch info.
         */ 
        void doOnSwitch(RotaryEncoderSwitchAction action) {
            if (_onSwitch != NULL) {
                RotaryEncoderSwitchAction event = {
                    action
                };
                _onSwitch(event);
            }
        }
        
        /**
         * @brief Calls onClick event if assigned.
         */ 
        void doOnClick() {
            if (_onClick != NULL) {
                _onClick();
            }
        }    
        
        /**
         * @brief Calls onLongClick event if assigned.
         */ 
        void doOnLongClick() {
            if (_onLongClick != NULL) {
                _onLongClick();
            }
        }              

    public:
    
        /**
         * @brief Controller constructor, assigns encoder pins.
         */
        RotaryEncoder(uint8_t PIN_CLOCK, uint8_t PIN_DATA, uint8_t PIN_SWITCH) {
            _PIN_CLOCK = PIN_CLOCK;
            _PIN_DATA = PIN_DATA;
            _PIN_SWITCH = PIN_SWITCH;
        }

        /**
         * @brief Gets onChange callback.
         * @return Returns assigned callback.
         */
        RotaryEncoderOnChange getOnChange()  {
            return _onChange;
        }

        /**
         * @brief Assigns onChange event callback.
         * @param onChange callback to assign.
         */
        void setOnChange(RotaryEncoderOnChange onChange)  {
            _onChange = onChange;
        }

        /**
         * @brief Gets onSwitch callback.
         * @return Returns assigned callback.
         */
        RotaryEncoderOnSwitch getOnSwitch() {
            return _onSwitch;
        }

        /**
         * @brief Assigns onSwitch event callback.
         * @param onSwitch callback to assign.
         */
        void setOnSwitch(RotaryEncoderOnSwitch onSwitch) {
            _onSwitch = onSwitch;
        }
        
        /**
         * @brief Gets onClick callback.
         * @return Returns assigned callback.
         */
        RotaryEncoderOnClick getOnClick() {
            return _onClick;
        }

        /**
         * @brief Assigns onClick event callback.
         * @param onClick callback to assign.
         */
        void setOnClick(RotaryEncoderOnClick onClick) {
            _onClick = onClick;
        } 
        
        /**
         * @brief Gets onLongClick callback.
         * @return Returns assigned callback.
         */
        RotaryEncoderOnClick getOnLongClick() {
            return _onLongClick;
        }

        /**
         * @brief Assigns onLongClick event callback.
         * @param onLongClick callback to assign.
         */
        void setOnLongClick(RotaryEncoderOnLongClick onLongClick) {
            _onLongClick = onLongClick;
        }    

        /**
         * @brief Initializes controller. Call this once before encoder use.
         */
        void begin() {
            initialized = true;
        
            // Clock pin in pull-up mode
            pinMode(_PIN_CLOCK, INPUT);
            digitalWrite(_PIN_CLOCK, HIGH);
            
            // Data pin in pull-up mode
            pinMode(_PIN_DATA, INPUT);
            digitalWrite(_PIN_DATA, HIGH);
            
            // Switch pin in pull-up mode
            pinMode(_PIN_SWITCH, INPUT_PULLUP);

            // First read
            _pinClockRetain = digitalRead(_PIN_CLOCK);
            _pinDataRetain = digitalRead(_PIN_DATA);
            _pinSwitchRetain = digitalRead(_PIN_SWITCH);
            
            _velocityTime = millis();
        }

        /**
         * @brief Updates encoder state and calls callbacks if any assigned. Call this
         * repeatly in `loop()` method.
         * Note: Call `begin()` before this call or nothing s happens.
         */
        void update() {
            if (!initialized) {
                return;
            }
        
            // Detect rotation
            int pinClk = digitalRead(_PIN_CLOCK);
            if (_pinClockRetain != pinClk) {
                _pinClockRetain = pinClk;
                if (pinClk == HIGH) {
                    unsigned long velocityMeasure = millis();
                    unsigned long velocity = velocityMeasure - _velocityTime;
                    int pinDt = digitalRead(_PIN_DATA);
                    if (pinClk == pinDt) {                        
                        doOnChange(right, velocity);
                    } else {
                        doOnChange(left, velocity);
                    }
                    _velocityTime = velocityMeasure;
                }
            }
            
            // Detect long click
            if (_switchPressTime > 0 && !_longClickFired) {
                long delta = millis() - _switchPressTime;
                if (delta > ROTARY_ENCODER_LONG_CLICK_MILLIS) {
                    _longClickFired = true;
                    doOnLongClick();
                }                
            }

            // Detect switch press or release
            int pinSw;
            if (getDebouncedSwitchState(pinSw)) {
                if (_pinSwitchRetain != pinSw) {
                    _pinSwitchRetain = pinSw;
                    if (pinSw == HIGH) {
                        if (!_longClickFired) {
                            doOnClick();
                        }
                        _switchPressTime = 0;
                        _longClickFired = false;
                        doOnSwitch(release);
                    } else {
                        _switchPressTime = millis();
                        _longClickFired = false;
                        doOnSwitch(press);
                    }
                }
            }
        }
};

#endif
