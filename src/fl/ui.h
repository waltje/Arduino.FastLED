
#pragma once

#include <stdint.h>

#include "fl/engine_events.h"
#include "fl/function_list.h"
#include "fl/math_macros.h"
#include "fl/namespace.h"
#include "fl/template_magic.h"
#include "fl/ui_impl.h"
#include "fl/unused.h"
#include "platforms/ui_defs.h"

namespace fl {

// If the platform is missing ui components, provide stubs.

class UISlider : public UISliderImpl {
  public:
    using Super = UISliderImpl;
    // If step is -1, it will be calculated as (max - min) / 100
    UISlider(const char *name, float value = 128.0f, float min = 1,
             float max = 255, float step = -1.f)
        : UISliderImpl(name, value, min, max, step), mListener(this) {}
    float value() const { return Super::value(); }
    float value_normalized() const {
        float min = Super::getMin();
        float max = Super::getMax();
        if (ALMOST_EQUAL(max, min, 0.0001f)) {
            return 0;
        }
        return (value() - min) / (max - min);
    }
    float getMax() const { return Super::getMax(); }
    void setValue(float value);
    operator float() const { return Super::value(); }
    operator uint8_t() const { return static_cast<uint8_t>(Super::value()); }
    operator uint16_t() const { return static_cast<uint16_t>(Super::value()); }
    operator int() const { return static_cast<int>(Super::value()); }
    template <typename T> T as() const {
        return static_cast<T>(Super::value());
    }

    int as_int() const { return static_cast<int>(Super::value()); }

    UISlider &operator=(float value) {
        Super::setValue(value);
        return *this;
    }
    UISlider &operator=(int value) {
        Super::setValue(static_cast<float>(value));
        return *this;
    }

    void addCallback(function<void(float)> callback) {
        function<void(UISlider &, float)> wrapped_cb =
            [callback](UISlider &slider, float value) {
                FASTLED_UNUSED(slider);
                callback(value);
            };
        mCallbacks.add(wrapped_cb);
        mListener.addToEngineEventsOnce();
    }

    void addCallbackEx(function<void(UISlider &, float)> callback) {
        mCallbacks.add(callback);
        mListener.addToEngineEventsOnce();
    }
    void clearCallbacks() { mCallbacks.clear(); }

  protected:
    struct Listener : public EngineEvents::Listener {
        Listener(UISlider *owner) : mOwner(owner) {
            EngineEvents::addListener(this);
        }
        ~Listener() {
            if (added) {
                EngineEvents::removeListener(this);
            }
        }
        void addToEngineEventsOnce() {
            if (added) {
                return;
            }
            EngineEvents::addListener(this);
            added = true;
        }
        void onBeginFrame() override;

      private:
        UISlider *mOwner;
        bool added = false;
    };

  private:
    FunctionList<UISlider &, float> mCallbacks;
    float mLastFrameValue = 0;
    bool mLastFramevalueValid = false;
    Listener mListener;
};

// template operator for >= against a jsSliderImpl

class UIButton : public UIButtonImpl {
  public:
    using Super = UIButtonImpl;
    UIButton(const char *name) : UIButtonImpl(name), mListener(this) {}
    ~UIButton() {}
    bool isPressed() const { return Super::isPressed(); }
    bool clicked() const { return Super::clicked(); }
    int clickedCount() const { return Super::clickedCount(); }
    operator bool() const { return Super::isPressed(); }

    void click() { Super::click(); }

    void addCallback(function<void()> callback) {
        function<void(UIButton &)> wrapped_cb = [callback](UIButton &button) {
            FASTLED_UNUSED(button);
            callback();
        };
        mCallbacks.add(wrapped_cb);
        mListener.addToEngineEventsOnce();
    }

    void addCallbackEx(function<void(UIButton &)> callback) {
        mCallbacks.add(callback);
        mListener.addToEngineEventsOnce();
    }
    void clearCallbacks() { mCallbacks.clear(); }

  protected:
    struct Listener : public EngineEvents::Listener {
        Listener(UIButton *owner) : mOwner(owner) {
            EngineEvents::addListener(this);
        }
        ~Listener() {
            if (added) {
                EngineEvents::removeListener(this);
            }
        }
        void addToEngineEventsOnce() {
            if (added) {
                return;
            }
            EngineEvents::addListener(this);
            added = true;
        }
        void onBeginFrame() override;

      private:
        UIButton *mOwner;
        bool added = false;
    };

  private:
    FunctionList<UIButton &> mCallbacks;
    bool mLastFrameClicked = false;
    Listener mListener;
};

class UICheckbox : public UICheckboxImpl {
  public:
    using Super = UICheckboxImpl;
    UICheckbox(const char *name, bool value = false)
        : UICheckboxImpl(name, value), mListener(this) {}
    ~UICheckbox() {}

    operator bool() const { return value(); }
    explicit operator int() const { return static_cast<int>(value()); }
    UICheckbox &operator=(bool value) {
        impl() = value;
        return *this;
    }

    void addCallback(function<void(bool)> callback) {
        function<void(UICheckbox &, bool)> wrapped_cb =
            [callback](UICheckbox &checkbox, bool value) {
                FASTLED_UNUSED(checkbox);
                callback(value);
            };
        mCallbacks.add(wrapped_cb);
        mListener.addToEngineEventsOnce();
    }

    void addCallbackEx(function<void(UICheckbox &, bool)> callback) {
        mCallbacks.add(callback);
        mListener.addToEngineEventsOnce();
    }
    void clearCallbacks() { mCallbacks.clear(); }

  protected:
    struct Listener : public EngineEvents::Listener {
        Listener(UICheckbox *owner) : mOwner(owner) {
            EngineEvents::addListener(this);
        }
        ~Listener() {
            if (added) {
                EngineEvents::removeListener(this);
            }
        }
        void addToEngineEventsOnce() {
            if (added) {
                return;
            }
            EngineEvents::addListener(this);
            added = true;
        }
        void onBeginFrame() override;

      private:
        UICheckbox *mOwner;
        bool added = false;
    };

  private:
    Super &impl() { return *this; }
    FunctionList<UICheckbox &, bool> mCallbacks;
    bool mLastFrameValue = false;
    bool mLastFrameValueValid = false;
    Listener mListener;
};

class UINumberField : public UINumberFieldImpl {
  public:
    using Super = UINumberFieldImpl;
    UINumberField(const char *name, double value, double min = 0,
                  double max = 100)
        : UINumberFieldImpl(name, value, min, max), mListener(this) {}
    ~UINumberField() {}
    double value() const { return Super::value(); }
    void setValue(double value) { Super::setValue(value); }
    operator double() const { return Super::value(); }
    operator int() const { return static_cast<int>(Super::value()); }
    UINumberField &operator=(double value) {
        setValue(value);
        return *this;
    }
    UINumberField &operator=(int value) {
        setValue(static_cast<double>(value));
        return *this;
    }

    void addCallback(function<void(double)> callback) {
        function<void(UINumberField &, double)> wrapped_cb =
            [callback](UINumberField &checkbox, bool value) {
                FASTLED_UNUSED(checkbox);
                callback(value);
            };
        mCallbacks.add(wrapped_cb);
        mListener.addToEngineEventsOnce();
    }

    void addCallbackEx(function<void(UINumberField &, double)> callback) {
        mCallbacks.add(callback);
        mListener.addToEngineEventsOnce();
    }
    void clearCallbacks() { mCallbacks.clear(); }

  private:
    struct Listener : public EngineEvents::Listener {
        Listener(UINumberField *owner) : mOwner(owner) {
            EngineEvents::addListener(this);
        }
        ~Listener() {
            if (added) {
                EngineEvents::removeListener(this);
            }
        }
        void addToEngineEventsOnce() {
            if (added) {
                return;
            }
            EngineEvents::addListener(this);
            added = true;
        }
        void onBeginFrame() override;

      private:
        UINumberField *mOwner;
        bool added = false;
    };

    Listener mListener;
    double mLastFrameValue = 0;
    bool mLastFrameValueValid = false;
    FunctionList<UINumberField &, double> mCallbacks;

    Super &impl() { return *this; }
};

class UITitle : public UITitleImpl {
  public:
    UITitle(const char *name) : UITitleImpl(name) {}
    ~UITitle() {}
};

class UIDescription : public UIDescriptionImpl {
  public:
    UIDescription(const char *name) : UIDescriptionImpl(name) {}
    ~UIDescription() {}
};

#define FASTLED_UI_DEFINE_OPERATORS(UI_CLASS)                                  \
    FASTLED_DEFINE_POD_COMPARISON_OPERATOR(UI_CLASS, >=)                       \
    FASTLED_DEFINE_POD_COMPARISON_OPERATOR(UI_CLASS, <=)                       \
    FASTLED_DEFINE_POD_COMPARISON_OPERATOR(UI_CLASS, >)                        \
    FASTLED_DEFINE_POD_COMPARISON_OPERATOR(UI_CLASS, <)                        \
    FASTLED_DEFINE_POD_COMPARISON_OPERATOR(UI_CLASS, ==)                       \
    FASTLED_DEFINE_POD_COMPARISON_OPERATOR(UI_CLASS, !=)

FASTLED_UI_DEFINE_OPERATORS(UISlider);
FASTLED_UI_DEFINE_OPERATORS(UINumberField);
FASTLED_UI_DEFINE_OPERATORS(UICheckbox);
FASTLED_UI_DEFINE_OPERATORS(UIButton);

} // end namespace fl
