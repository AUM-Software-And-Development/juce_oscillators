/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Juce_oscillatorsAudioProcessor::Juce_oscillatorsAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

Juce_oscillatorsAudioProcessor::~Juce_oscillatorsAudioProcessor()
{
}

//==============================================================================
const juce::String Juce_oscillatorsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Juce_oscillatorsAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Juce_oscillatorsAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Juce_oscillatorsAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Juce_oscillatorsAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Juce_oscillatorsAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Juce_oscillatorsAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Juce_oscillatorsAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Juce_oscillatorsAudioProcessor::getProgramName (int index)
{
    return {};
}

void Juce_oscillatorsAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Juce_oscillatorsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // This section defines live buffer limitations
    juce::dsp::ProcessSpec pSpec;
    pSpec.sampleRate = sampleRate;
    pSpec.numChannels = getTotalNumOutputChannels();
    pSpec.maximumBlockSize = samplesPerBlock;

    float_oscillator_1.prepare(pSpec);
    float_oscillator_1.setFrequency(256.0f);
    float_gain_1.prepare(pSpec);
    float_gain_1.setGainLinear(0.01f);
}

void Juce_oscillatorsAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Juce_oscillatorsAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Juce_oscillatorsAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    /* Replace a typecasted array to the buffer zone */
    juce::dsp::AudioBlock<float> audioBlock {buffer};
    float_oscillator_1.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));
    float_gain_1.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));


}

//==============================================================================
bool Juce_oscillatorsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Juce_oscillatorsAudioProcessor::createEditor()
{
    return new Juce_oscillatorsAudioProcessorEditor (*this);
}

//==============================================================================
void Juce_oscillatorsAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Juce_oscillatorsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Juce_oscillatorsAudioProcessor();
}
