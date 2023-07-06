#pragma once

#include <juce_core/juce_core.h>

#include <concepts>

namespace JUCE_Extensions::ProcessorConcepts {
    template <typename T>
    concept Program = requires(T t, const T ct) {
        { ct.getNumPrograms() } -> std::same_as<int>;
        { ct.getCurrentProgram() } -> std::same_as<int>;
        { t.setCurrentProgram(std::declval<int>()) };
        { ct.getProgramName(std::declval<int>()) } -> std::convertible_to<juce::String>;
        { t.changeProgramName(std::declval<int>(), std::declval<juce::String>()) };
    };
}