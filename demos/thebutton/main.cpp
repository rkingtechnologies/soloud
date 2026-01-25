/*
SoLoud audio engine
Copyright (c) 2013-2018 Jari Komppa

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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "common/window.h"
#include "imgui.h"
#include "soloud.h"
#include "soloud_wav.h"

namespace {

class RadioSet {
  SoLoud::Soloud* mSoloud;
  SoLoud::Bus* mBus;
  SoLoud::AudioSource** mSource;
  SoLoud::AudioSource* mAck;
  unsigned int mSourceCount;
  unsigned int mSourceCountMax;
  unsigned int mAckLength;

 public:
  RadioSet() {
    mSource = 0;
    mSourceCount = 0;
    mSoloud = 0;
    mSourceCountMax = 0;
    mAck = 0;
    mBus = 0;
    mAckLength = 0;
  }

  SoLoud::result init(SoLoud::Soloud& aSoloud, SoLoud::Bus* aBus) {
    mSoloud = &aSoloud;
    mBus = aBus;
    return SoLoud::SO_NO_ERROR;
  }

  SoLoud::result setAck(SoLoud::AudioSource& aAudioSource, unsigned int aAckLength) {
    mAck = &aAudioSource;
    mAckLength = aAckLength;
    return SoLoud::SO_NO_ERROR;
  }

  SoLoud::result clearAck() {
    mAck = 0;
    return SoLoud::SO_NO_ERROR;
  }

  SoLoud::result attach(SoLoud::AudioSource& aAudioSource) {
    unsigned int i;
    for (i = 0; i < mSourceCount; i++) {
      if (&aAudioSource == mSource[i]) {
        return SoLoud::INVALID_PARAMETER;
      }
    }

    if (mSourceCount == mSourceCountMax) {
      mSourceCountMax += 16;
      SoLoud::AudioSource** t = new SoLoud::AudioSource*[mSourceCountMax];
      for (i = 0; i < mSourceCount; i++) {
        t[i] = mSource[i];
      }
      delete[] mSource;
      mSource = t;
    }

    mSource[mSourceCount] = &aAudioSource;
    mSourceCount++;
    return SoLoud::SO_NO_ERROR;
  }

  SoLoud::handle play(SoLoud::AudioSource& aAudioSource) {
    // try to attach just in case we don't already have this
    attach(aAudioSource);

    bool found = false;
    unsigned int i;
    for (i = 0; i < mSourceCount; i++) {
      if (mSoloud->countAudioSource(*mSource[i]) > 0) {
        mSoloud->stopAudioSource(*mSource[i]);
        found = true;
      }
    }

    int delay = 0;

    if (mAck && found) {
      if (mBus) {
        mBus->play(*mAck);
      } else {
        mSoloud->play(*mAck);
      }
      delay = mAckLength;
    }

    int res;

    if (mBus) {
      res = mBus->play(aAudioSource, -1, 0, true);
    } else {
      res = mSoloud->play(aAudioSource, -1, 0, true);
    }
    // delay the sample by however long ack is
    mSoloud->setDelaySamples(res, delay);
    mSoloud->setPause(res, false);
    return res;
  }
};

SoLoud::Soloud gSoloud;
RadioSet gRadioSet;
SoLoud::Wav gPhrase[12];
int gCycles;
int gNextEvent;

void RenderTheButton() {
  const auto tick = ImGui::GetTime() * 1000;

  if (tick > gNextEvent) {
    gRadioSet.play(gPhrase[gCycles]);
    gNextEvent = tick + 5000;
    gCycles = (gCycles + 1) % 9;
  }

  float* buf = gSoloud.getWave();
  float* fft = gSoloud.calcFFT();

  ImGui::SeparatorText("Output");
  ImGui::PlotLines("##Wave", buf, 256, 0, "Wave", -1, 1, ImVec2(264, 80));
  ImGui::PlotHistogram(
    "##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);
  ImGui::Text("Active voices    : %d", gSoloud.getActiveVoiceCount());
  ImGui::Text(
    "Thanks to Anthony Salter for\n"
    "voice acting!");

  ImGui::SeparatorText("Control");
  if (ImGui::Button("The button", ImVec2(300, 200))) {
    gRadioSet.play(gPhrase[10]);
    gNextEvent = tick + 5000;
  }
}

void InitAudio() {
  gCycles = 0;
  gNextEvent = 0;
  gSoloud.init(
    SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION);
  gRadioSet.init(gSoloud, NULL);
  gPhrase[0].load("../assets/audio/thebutton/button1.mp3");
  gPhrase[1].load("../assets/audio/thebutton/button2.mp3");
  gPhrase[2].load("../assets/audio/thebutton/button3.mp3");
  gPhrase[3].load("../assets/audio/thebutton/button4.mp3");
  gPhrase[4].load("../assets/audio/thebutton/button5.mp3");
  gPhrase[5].load("../assets/audio/thebutton/cough.mp3");
  gPhrase[6].load("../assets/audio/thebutton/button6.mp3");
  gPhrase[7].load("../assets/audio/thebutton/button7.mp3");
  gPhrase[8].load("../assets/audio/thebutton/button1.mp3");
  gPhrase[9].load("../assets/audio/thebutton/sigh.mp3");
  gPhrase[10].load("../assets/audio/thebutton/thankyou.mp3");
  gPhrase[11].load("../assets/audio/thebutton/ack.ogg");

  int i;
  for (i = 0; i < 11; i++) {
    gRadioSet.attach(gPhrase[i]);
  }
  gRadioSet.setAck(gPhrase[11], gPhrase[11].mSampleCount);
}

class Content : public soloud::tests::common::Renderable {
 public:
  void Render(soloud::tests::common::Window* window) override {
    static bool running = true;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    ImGui::Begin("##SpeechFilterDemo", &running,
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_AlwaysAutoResize);

    if (!running) {
      window->Close();
    }

    RenderTheButton();

    ImGui::End();
  }
};

}  // namespace

int main() {
  InitAudio();

  soloud::tests::common::Window window({"The Button Demo", 640, 720});
  window.AddRenderable<Content>();
  window.Run();
}