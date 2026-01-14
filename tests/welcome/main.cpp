/*
SoLoud audio engine
Copyright (c) 2013-2014 Jari Komppa

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/

#include <iostream>
#include <print>
#include <string>

#include "soloud.h"
#include "soloud_speech.h"
#include "soloud_thread.h"
#include "soloud_wav.h"

int main() {
  // Engine and sources
  SoLoud::Soloud soloud;
  SoLoud::Speech speech;
  SoLoud::Wav wav;

  // Initialize SoLoud
  const auto init_res = soloud.init();

  if (init_res != SoLoud::SO_NO_ERROR) {
    std::println("Failed to initialize SoLoud, error: {}", init_res);
    return 1;
  }

  soloud.setVisualizationEnable(1);  // for fft calc

  std::println("Welcome to SoLoud!");

  // Load background sample
  const char* const path = "../assets/audio/windy_ambience.ogg";

  const auto res = wav.load(path);

  if (res != SoLoud::SO_NO_ERROR) {
    std::println("Failed to load audio file: {}, error: {}", path, res);
    soloud.deinit();
    return 1;
  }

  // Sample options
  wav.setLooping(1);
  const int handle = soloud.play(wav);
  soloud.setVolume(handle, 0.5f);
  soloud.setPan(handle, -0.2f);
  soloud.setRelativePlaySpeed(handle, 0.9f);

  // Configure sound source
  std::print("What is your name?\n> ");
  std::string name;
  std::getline(std::cin, name);

  // Speak the name
  speech.setText(name.c_str());
  speech.setVolume(5.0f);
  soloud.play(speech);

  // Wait until speech finishes (ambience still playing)
  while (soloud.getVoiceCount() > 1) {
    SoLoud::Thread::sleep(100);
  }

  // Exit prompt
  std::println("Done. Press Enter to quit...");
  std::cin.get();

  // Stop ambience
  soloud.stop(handle);

  // Clean up SoLoud
  soloud.deinit();

  // All done.
  return 0;
}