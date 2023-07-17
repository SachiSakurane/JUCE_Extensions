#pragma once

#include <string>
#include <functional>

#include <riw/algorithm/value_range.hpp>

namespace JUCE_Extensions
{
    template <class Type>
    class Parameter
    {
    };

    template <>
    class Parameter<float>
    {
    public:
        using value_type = float;

        Parameter(
            const std::string &_id,
            const std::string &_name,
            value_type _default_value,
            const riw::value_range<value_type> &_range,
            value_type _center_for_skew,
            std::function<juce::String(float, int)> _string_function = nullptr)
            : id{_id}, name{_name}, default_value{_default_value}, range{_range}, center_for_skew{_center_for_skew},
              string_function{std::move(_string_function)}
        {
        }

        [[nodiscard]] std::unique_ptr<juce::AudioParameterFloat> makeParameter() const
        {
            auto normalisable_range = juce::NormalisableRange<value_type>(range.min, range.max);
            normalisable_range.setSkewForCentre(center_for_skew);

            return std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID{id, JucePlugin_VersionCode},
                name,
                normalisable_range,
                static_cast<value_type>(default_value),
                juce::String(),
                juce::AudioProcessorParameter::genericParameter,
                string_function);
        }

        value_type getValue(juce::AudioProcessorValueTreeState &parameter) const
        {
            return static_cast<value_type>(*parameter.getRawParameterValue(id));
        }

        void setValue(juce::AudioProcessorValueTreeState &parameter, value_type value) const
        {
            if (juce::AudioProcessorParameter *p = parameter.getParameter(id))
            {
                const float newValue = parameter
                                           .getParameterRange(id)
                                           .convertTo0to1(value);

                if (p->getValue() != newValue)
                {
                    p->setValueNotifyingHost(newValue);
                }
            }
        }

        template <class Attachment, class Control>
        void setAttachment(std::unique_ptr<Attachment> &attachment, juce::AudioProcessorValueTreeState &parameter, Control &control) const
        {
            attachment = std::make_unique<Attachment>(parameter, id, control);
        }

        riw::value_range<value_type> getRange() const { return range; }
        const std::string &getID() const { return id; }

        const std::string id;
        const std::string name;
        const value_type default_value;
        const riw::value_range<value_type> range;
        const value_type center_for_skew;

    private:
        std::function<juce::String(float, int)> string_function;
    };

    template <>
    class Parameter<bool>
    {
    public:
        using value_type = bool;

        Parameter(const std::string &_id, const std::string &_name, bool _default_value)
            : id{_id}, name{_name}, default_value{_default_value}
        {
        }

        std::unique_ptr<juce::AudioParameterBool> makeParameter() const
        {
            return std::make_unique<juce::AudioParameterBool>(
                juce::ParameterID{id, JucePlugin_VersionCode},
                name,
                default_value,
                juce::String());
        }

        bool getValue(const juce::AudioProcessorValueTreeState &parameter) const
        {
            return static_cast<bool>(*parameter.getRawParameterValue(id));
        }

        void setValue(juce::AudioProcessorValueTreeState &parameter, bool flag) const
        {
            if (juce::AudioProcessorParameter *p = parameter.getParameter(id))
            {
                const float newValue = parameter
                                           .getParameterRange(id)
                                           .convertTo0to1(flag ? 1.f : 0.f);

                if (p->getValue() != newValue)
                {
                    p->setValueNotifyingHost(newValue);
                }
            }
        }

        template <class Attachment, class Control>
        void setAttachment(std::unique_ptr<Attachment> &attachment, juce::AudioProcessorValueTreeState &parameter, Control &control) const
        {
            attachment = std::make_unique<Attachment>(parameter, id, control);
        }

        const std::string &getID() const { return id; }
        const std::string id;
        const std::string name;
        const bool default_value;

    private:
    };
}
