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

#include "vibrator-impl/Vibrator.h"

#define LOG_TAG "android.hardware.vibrator-service.rosemary"

#include <android-base/logging.h>
#include <thread>
#include <fstream>

#define PCONCAT(A, B) A/B
#define STRINGIFY_INNER(s) #s
#define STRINGIFY(s) STRINGIFY_INNER(s)
#define VIBRA_NODE(n) STRINGIFY(PCONCAT(/sys/devices/platform/aw8622, n))

namespace aidl {
namespace android {
namespace hardware {
namespace vibrator {

template <typename T>
static void write_sysfs_node(const std::string& path, const T& val) {
    std::ofstream node(path);
    node << val;
}

ndk::ScopedAStatus Vibrator::getCapabilities(int32_t* _aidl_return) {
    LOG(INFO) << "Vibrator reporting capabilities";
    *_aidl_return = IVibrator::CAP_PERFORM_CALLBACK;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::off() {
    LOG(INFO) << "Vibrator off";

    write_sysfs_node(VIBRA_NODE(index), 0);
    write_sysfs_node(VIBRA_NODE(activate), 0);

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::on(int32_t timeoutMs,
                                __attribute__((unused)) const std::shared_ptr<IVibratorCallback>& callback) {
    LOG(INFO) << "Vibrator on for timeoutMs: " << timeoutMs;
    LOG(INFO) << VIBRA_NODE(duration);
    write_sysfs_node(VIBRA_NODE(duration), timeoutMs);
    write_sysfs_node(VIBRA_NODE(activate), 1);

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::perform(Effect effect, __attribute__((unused)) EffectStrength strength,
                                     __attribute__((unused)) const std::shared_ptr<IVibratorCallback>& callback,
                                     int32_t* _aidl_return) {

    uint32_t index = 0;
    uint32_t timeoutMs = 0;

    switch (effect) {
        case Effect::TICK:
            index = WAVEFORM_TICK_EFFECT_INDEX;
            timeoutMs = WAVEFORM_TICK_EFFECT_MS;
            break;
        case Effect::TEXTURE_TICK:
            index = WAVEFORM_TEXTURE_TICK_EFFECT_INDEX;
            timeoutMs = WAVEFORM_TEXTURE_TICK_EFFECT_MS;
            break;
        case Effect::CLICK:
            index = WAVEFORM_CLICK_EFFECT_INDEX;
            timeoutMs = WAVEFORM_CLICK_EFFECT_MS;
            break;
        case Effect::HEAVY_CLICK:
            index = WAVEFORM_HEAVY_CLICK_EFFECT_INDEX;
            timeoutMs = WAVEFORM_HEAVY_CLICK_EFFECT_MS;
            break;
        case Effect::DOUBLE_CLICK:
            index = WAVEFORM_DOUBLE_CLICK_EFFECT_INDEX;
            timeoutMs = WAVEFORM_DOUBLE_CLICK_EFFECT_MS;
            break;
        case Effect::THUD:
            index = WAVEFORM_THUD_EFFECT_INDEX;
            timeoutMs = WAVEFORM_THUD_EFFECT_MS;
            break;
        case Effect::POP:
            index = WAVEFORM_TICK_EFFECT_INDEX;
            timeoutMs = WAVEFORM_POP_EFFECT_MS;
            break;
        default:
            return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    write_sysfs_node(VIBRA_NODE(index), index);
    ndk::ScopedAStatus ret = on(timeoutMs, nullptr);
        
    if (!ret.isOk()) {
        return ret;
    } else {
        *_aidl_return = timeoutMs;
    }

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::getSupportedEffects(std::vector<Effect>* _aidl_return) {
    *_aidl_return = {
        Effect::CLICK,
        Effect::TICK,
        Effect::TEXTURE_TICK,
        Effect::HEAVY_CLICK,
        Effect::DOUBLE_CLICK,
        Effect::THUD,
        Effect::POP
    };
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Vibrator::setAmplitude(__attribute__((unused)) float amplitude) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::setExternalControl(__attribute__((unused)) bool enabled) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::getCompositionDelayMax(__attribute__((unused)) int32_t* maxDelayMs) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::getCompositionSizeMax(__attribute__((unused)) int32_t* maxSize) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::getSupportedPrimitives(__attribute__((unused)) std::vector<CompositePrimitive>* supported) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::getPrimitiveDuration(__attribute__((unused)) CompositePrimitive primitive,
                                                  __attribute__((unused)) int32_t* durationMs) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::compose(__attribute__((unused)) const std::vector<CompositeEffect>& composite,
                                     __attribute__((unused)) const std::shared_ptr<IVibratorCallback>& callback) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::getSupportedAlwaysOnEffects(__attribute__((unused)) std::vector<Effect>* _aidl_return) {
    return getSupportedEffects(_aidl_return);
}

ndk::ScopedAStatus Vibrator::alwaysOnEnable(__attribute__((unused)) int32_t id, __attribute__((unused)) Effect effect, __attribute__((unused)) EffectStrength strength) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

ndk::ScopedAStatus Vibrator::alwaysOnDisable(__attribute__((unused)) int32_t id) {
    return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
}

}  // namespace vibrator
}  // namespace hardware
}  // namespace android
}  // namespace aidl
