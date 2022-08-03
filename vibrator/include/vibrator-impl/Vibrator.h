/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <aidl/android/hardware/vibrator/BnVibrator.h>



// Values inspired from cannon(g) vibrator HAL
// https://github.com/StatiXOS/android_device_xiaomi_cannon/commit/f95eff04bbaa6bbbbd340a200ce029ffb889738e

// Define durations for waveforms
static constexpr uint32_t WAVEFORM_TICK_EFFECT_MS = 10;
static constexpr uint32_t WAVEFORM_TEXTURE_TICK_EFFECT_MS = 20;
static constexpr uint32_t WAVEFORM_CLICK_EFFECT_MS = 15;
static constexpr uint32_t WAVEFORM_HEAVY_CLICK_EFFECT_MS = 30;
static constexpr uint32_t WAVEFORM_DOUBLE_CLICK_EFFECT_MS = 60;
static constexpr uint32_t WAVEFORM_THUD_EFFECT_MS = 35;
static constexpr uint32_t WAVEFORM_POP_EFFECT_MS = 15;

// Select waveform index from firmware through index list
static constexpr uint32_t WAVEFORM_TICK_EFFECT_INDEX = 1;
static constexpr uint32_t WAVEFORM_TEXTURE_TICK_EFFECT_INDEX = 4;
static constexpr uint32_t WAVEFORM_CLICK_EFFECT_INDEX = 2;
static constexpr uint32_t WAVEFORM_HEAVY_CLICK_EFFECT_INDEX = 5;
static constexpr uint32_t WAVEFORM_DOUBLE_CLICK_EFFECT_INDEX = 6;
static constexpr uint32_t WAVEFORM_THUD_EFFECT_INDEX = 7;

namespace aidl {
namespace android {
namespace hardware {
namespace vibrator {

class Vibrator : public BnVibrator {
    ndk::ScopedAStatus getCapabilities(int32_t* _aidl_return) override;
    ndk::ScopedAStatus off() override;
    ndk::ScopedAStatus on(int32_t timeoutMs,
                          const std::shared_ptr<IVibratorCallback>& callback) override;
    ndk::ScopedAStatus perform(Effect effect, EffectStrength strength,
                               const std::shared_ptr<IVibratorCallback>& callback,
                               int32_t* _aidl_return) override;
    ndk::ScopedAStatus getSupportedEffects(std::vector<Effect>* _aidl_return) override;
    ndk::ScopedAStatus setAmplitude(float amplitude) override;
    ndk::ScopedAStatus setExternalControl(bool enabled) override;
    ndk::ScopedAStatus getCompositionDelayMax(int32_t* maxDelayMs);
    ndk::ScopedAStatus getCompositionSizeMax(int32_t* maxSize);
    ndk::ScopedAStatus getSupportedPrimitives(std::vector<CompositePrimitive>* supported) override;
    ndk::ScopedAStatus getPrimitiveDuration(CompositePrimitive primitive,
                                            int32_t* durationMs) override;
    ndk::ScopedAStatus compose(const std::vector<CompositeEffect>& composite,
                               const std::shared_ptr<IVibratorCallback>& callback) override;
    ndk::ScopedAStatus getSupportedAlwaysOnEffects(std::vector<Effect>* _aidl_return) override;
    ndk::ScopedAStatus alwaysOnEnable(int32_t id, Effect effect, EffectStrength strength) override;
    ndk::ScopedAStatus alwaysOnDisable(int32_t id) override;
    
    ndk::ScopedAStatus getResonantFrequency(float *resonantFreqHz) override;
    ndk::ScopedAStatus getQFactor(float *qFactor) override;
    ndk::ScopedAStatus getFrequencyResolution(float *freqResolutionHz) override;
    ndk::ScopedAStatus getFrequencyMinimum(float *freqMinimumHz) override;
    ndk::ScopedAStatus getBandwidthAmplitudeMap(std::vector<float> *_aidl_return) override;
    ndk::ScopedAStatus getPwlePrimitiveDurationMax(int32_t *durationMs) override;
    ndk::ScopedAStatus getPwleCompositionSizeMax(int32_t *maxSize) override;
    ndk::ScopedAStatus getSupportedBraking(std::vector<Braking>* supported) override;
    ndk::ScopedAStatus composePwle(const std::vector<PrimitivePwle> &composite,
                                   const std::shared_ptr<IVibratorCallback> &callback) override;
};

}  // namespace vibrator
}  // namespace hardware
}  // namespace android
}  // namespace aidl
