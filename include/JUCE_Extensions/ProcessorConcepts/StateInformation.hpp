#pragma once

#include <juce_core/juce_core.h>

namespace JUCE_Extensions::ProcessorConcepts {
    template <typename T>
    concept StateInformation = requires(T t) {
        { t.getStateInformation(std::declval<juce::MemoryBlock&>()) };
        { t.setStateInformation(std::declval<const void*>(), std::declval<int>()) };
    };
}
