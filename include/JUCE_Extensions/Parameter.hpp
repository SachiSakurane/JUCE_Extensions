#pragma once

#include <string>
#include <functional>

#include <riw/algorithm/value_range.hpp>

namespace JUCE_Extensions
{
    template <class Type>
    class Parameter {};

    template<>
    class Parameter<float>
    {
    public:
        using value_type = float;

        Parameter(
            const std::string& id, const std::string& name, value_type default_value, const riw::value_range<value_type>& range,
            std::function<juce::String(float, int)> string_function = nullptr)
        : id_(id), name_(name), default_value_(default_value), range_(range), string_function_(std::move(string_function))
        {}

        [[nodiscard]] std::unique_ptr<juce::AudioParameterFloat> makeParameter() const
        {
            return std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID{id_, JucePlugin_VersionCode},
                name_,
                juce::NormalisableRange<value_type>(range_.min, range_.max),
                static_cast<value_type>(default_value_),
                juce::String(),
                juce::AudioProcessorParameter::genericParameter,
                string_function_);
        }

        value_type getValue (juce::AudioProcessorValueTreeState &parameter) const
        {
            return static_cast<value_type>(*parameter.getRawParameterValue(id_));
        }

        template <class Attachment, class Control>
        void setAttachment(std::unique_ptr<Attachment>& attachment, juce::AudioProcessorValueTreeState& parameter, Control& control) const
        {
            attachment = std::make_unique<Attachment>(parameter, id_, control);
        }

        riw::value_range<value_type> getRange() const { return range_; }
        const std::string& getID() const { return id_; }

    private:
        const std::string id_;
        const std::string name_;
        const value_type default_value_;
        const riw::value_range<value_type> range_;
        std::function<juce::String(float, int)> string_function_;

    };

    template <>
    class Parameter<bool>
    {
    public:
        using value_type = bool;

        Parameter(const std::string& id, const std::string& name, bool default_value)
            : id_(id), name_(name), default_value_(default_value)
        {}

        std::unique_ptr<juce::AudioParameterBool> makeParameter() const
        {
            return std::make_unique<juce::AudioParameterBool>(
                    juce::ParameterID{id_, JucePlugin_VersionCode},
                    name_,
                    default_value_,
                    juce::String());
        }

        bool getValue (juce::AudioProcessorValueTreeState &parameter) const
        {
            return static_cast<bool>(*parameter.getRawParameterValue(id_));
        }

        template <class Attachment, class Control>
        void setAttachment(std::unique_ptr<Attachment>& attachment, juce::AudioProcessorValueTreeState& parameter, Control& control) const
        {
            attachment = std::make_unique<Attachment>(parameter, id_, control);
        }

        const std::string& getID() const { return id_; }

    private:
        const std::string id_;
        const std::string name_;
        const bool default_value_;
    };
}
