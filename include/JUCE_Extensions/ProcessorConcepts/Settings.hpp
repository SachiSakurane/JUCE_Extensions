#pragma once

#include <juce_core/juce_core.h>

#include <concepts>
#include <string>

namespace JUCE_Extensions::ProcessorConcepts {
    template <typename T>
    concept GeneralSettings = requires() {
        { T::Name } -> std::convertible_to<juce::String>;
        { T::IsMidiEffect } -> std::convertible_to<bool>;
        { T::AcceptsMidi } -> std::convertible_to<bool>;
        { T::ProducesMidi } -> std::convertible_to<bool>;
    };

    template <typename T>
    concept HasAlternateDisplayNames = requires() {
        { T::AlternateDisplayNames } -> std::convertible_to<juce::StringArray>;
    };

    template <typename T>
    concept Settings = (GeneralSettings<T> && HasAlternateDisplayNames<T>) || GeneralSettings<T>;
}
