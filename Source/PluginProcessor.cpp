/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
EuclidCombinatorAudioProcessor::EuclidCombinatorAudioProcessor()
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

EuclidCombinatorAudioProcessor::~EuclidCombinatorAudioProcessor()
{
}

//==============================================================================
const juce::String EuclidCombinatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool EuclidCombinatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool EuclidCombinatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool EuclidCombinatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double EuclidCombinatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int EuclidCombinatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int EuclidCombinatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void EuclidCombinatorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String EuclidCombinatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void EuclidCombinatorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void EuclidCombinatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void EuclidCombinatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool EuclidCombinatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void EuclidCombinatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    int numSamples = buffer.getNumSamples();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamples);
    
    FillPositionData(mTransportData);
   
    bool hasAdvanced = false;
    if (hasAdvanced)
    {
        auto on = juce::MidiMessage::noteOn(1, 36, 1.f);
        auto off = juce::MidiMessage::noteOff(1, 36, 1.f);
        
        midiMessages.addEvent(on, 0);
        midiMessages.addEvent(off, 22050);
    }
}

void EuclidCombinatorAudioProcessor::FillPositionData(TransportData& data)
{
    AudioPlayHead* playHead = getPlayHead();
    auto positionInfo = playHead->getPosition();
    
    if(positionInfo->getBpm().hasValue())
    {
        data.bpm = static_cast<double>(*positionInfo->getBpm());
    }
    
    if(positionInfo->getTimeInSeconds().hasValue())
    {
        data.time = static_cast<double>(*positionInfo->getTimeInSeconds());
    }
    
    if(positionInfo->getPpqPosition().hasValue())
    {
        data.ppq = static_cast<double>(*positionInfo->getPpqPosition());
    }
    
    data.isPlaying = static_cast<bool>(positionInfo->getIsPlaying());
    
    if(positionInfo->getTimeSignature().hasValue())
    {
        auto timeSig = *positionInfo->getTimeSignature();
        int barLength = timeSig.numerator;
        
        int beatCount = static_cast<int>(data.time / (60.0 / data.bpm));
        data.beat =  beatCount % barLength;
        data.bar = static_cast<int> (beatCount / barLength);
    }
}

//==============================================================================
bool EuclidCombinatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* EuclidCombinatorAudioProcessor::createEditor()
{
    return new EuclidCombinatorAudioProcessorEditor (*this);
}

//==============================================================================
void EuclidCombinatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void EuclidCombinatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new EuclidCombinatorAudioProcessor();
}
