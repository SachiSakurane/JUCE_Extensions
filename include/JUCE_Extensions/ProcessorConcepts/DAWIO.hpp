#pragma once

#include <concepts>

#include <juce_audio_processors/juce_audio_processors.h>

namespace JUCE_Extensions::ProcessorConcepts {
    template <typename T>
    concept DAWIO = requires(T t) {
        { t.getTailLengthSeconds() } -> std::same_as<double>;
        { t.isBusesLayoutSupported(std::declval<const juce::AudioProcessor::BusesLayout&>()) } -> std::same_as<bool>;
    };
}
