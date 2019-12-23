# Populate this list with basename (without extension) of source files
set(PLUGIN_TARGETS
    mdaAmbience
    mdaBandisto
    mdaBeatBox
    mdaCombo
    mdaDe-ess
    mdaDegrade
    mdaDelay
    mdaDetune
    mdaDither
    mdaDubDelay
    mdaDX10
    mdaDynamics
    mdaEnvelope
    mdaEPiano
    mdaImage
    mdaJX10
    mdaLeslie
    mdaLimiter
    mdaLooplex
    mdaLoudness
    mdaMultiBand
    mdaOverdrive
    mdaPiano
    mdaRezFilter
    mdaRingMod
    mdaRoundPan
    mdaShepard
    mdaSplitter
    mdaStereo
    mdaSubSynth
    mdaTalkBox
    mdaTestTone
    mdaThruZero
    mdaTracker
    mdaTransient
    mdaVocInput
    mdaVocoder
)

foreach(plugin IN LISTS PLUGIN_TARGETS)
    add_vst2_plugin_target(${plugin})
endforeach()

