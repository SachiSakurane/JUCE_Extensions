#pragma once

#include <JUCE_Extensions/ProcessorConcepts/DAWIO.hpp>
#include <JUCE_Extensions/ProcessorConcepts/Program.hpp>
#include <JUCE_Extensions/ProcessorConcepts/Settings.hpp>
#include <JUCE_Extensions/ProcessorConcepts/StateInformation.hpp>

#include <cassert>

#include <juce_audio_processors/juce_audio_processors.h>

namespace JUCE_Extensions
{
    template <
        JUCE_Extensions::ProcessorConcepts::Settings SettingsType,
        JUCE_Extensions::ProcessorConcepts::DAWIO DAWIOType,
        JUCE_Extensions::ProcessorConcepts::Program ProgramType,
        JUCE_Extensions::ProcessorConcepts::StateInformation StateInformationType
    >
    class Processor : public juce::AudioProcessor
    {
    public:
        using ProcessorType = Processor<
            SettingsType,
            DAWIOType,
            ProgramType,
            StateInformationType>;

        Processor (
            const BusesProperties &ioLayouts,
            std::unique_ptr<DAWIOType> &&_daw_io = std::make_unique<DAWIOType>(),
            std::unique_ptr<ProgramType> &&_program = std::make_unique<ProgramType>(),
            std::unique_ptr<StateInformationType> &&_state_infortmation = std::make_unique<StateInformationType>()) :
                juce::AudioProcessor {ioLayouts},
                daw_io {std::move(_daw_io)},
                program {std::move(_program)},
                state_infortmation {std::move(_state_infortmation)}
        {
            assert(daw_io);
            assert(program);
            assert(state_infortmation);
        }

        const juce::String getName () const final
        {
            return SettingsType::Name;
        }

        juce::StringArray getAlternateDisplayNames () const final
        {
            if constexpr (JUCE_Extensions::ProcessorConcepts::HasAlternateDisplayNames<SettingsType>) {
                return SettingsType::AlternateDisplayNames;
            }
            return juce::StringArray();
        }

        bool acceptsMidi () const final
        {
            return SettingsType::AcceptsMidi;
        }

        bool producesMidi () const final
        {
            return SettingsType::ProducesMidi;
        }

        bool isMidiEffect () const final
        {
            return SettingsType::IsMidiEffect;
        }

        #ifndef JucePlugin_PreferredChannelConfigurations
        bool isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout &layouts) const final
        {
            return daw_io->isBusesLayoutSupported(layouts);
        }
        #endif

        double getTailLengthSeconds () const final
        {
            return daw_io->getTailLengthSeconds();
        }

        void getStateInformation (juce::MemoryBlock &destData) final
        {
            state_infortmation->getStateInformation(destData);
        }

        void setStateInformation (const void *data, int sizeInBytes) final
        {
            state_infortmation->setStateInformation(data, sizeInBytes);
        }

        int getNumPrograms () final
        {
            return program->getNumPrograms();
        }

        int getCurrentProgram () final
        {
            return program->getCurrentProgram();
        }

        void setCurrentProgram (int index) final
        {
            program->setCurrentProgram(index);
        }

        const juce::String getProgramName (int index) final
        {
            return program->getProgramName(index);
        }

        void changeProgramName (int index, const juce::String &newName) final
        {
            program->changeProgramName(index, newName);
        }

        DAWIOType& getDawIO() const { return *daw_io; }
        ProgramType& getProgram() const { return *program; }
        StateInformationType& getStateInformation() const { return *state_infortmation; }

    private:
        std::unique_ptr<DAWIOType> daw_io;
        std::unique_ptr<ProgramType> program;
        std::unique_ptr<StateInformationType> state_infortmation;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Processor)
    };
}
