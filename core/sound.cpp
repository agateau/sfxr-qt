#include "sound.h"

Sound::Sound(QObject* parent)
    : BaseSound(parent) {
    resetParams();
}

void Sound::resetParams() {
    setWaveType(0);

    setBaseFrequency(0.3f);
    setMinFrequency(0.0f);
    setSlide(0.0f);
    setDeltaSlide(0.0f);
    setSquareDuty(0.0f);
    setDutySweep(0.0f);

    setVibratoDepth(0.0f);
    setVibratoSpeed(0.0f);
    p_vib_delay = 0.0f;

    setAttackTime(0.0f);
    setSustainTime(0.3f);
    setDecayTime(0.4f);
    setSustainPunch(0.0f);

    filter_on = false;
    setLpFilterResonance(0.0f);
    setLpFilterCutoff(1.0f);
    setLpFilterCutoffSweep(0.0f);
    setHpFilterCutoff(0.0f);
    setHpFilterCutoffSweep(0.0f);

    setPhaserOffset(0.0f);
    setPhaserSweep(0.0f);

    setRepeatSpeed(0.0f);

    setChangeSpeed(0.0f);
    setChangeAmount(0.0f);
}

bool Sound::loadSettings(char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        return false;
    }

    int version = 0;
    fread(&version, 1, sizeof(int), file);
    if (version != 100 && version != 101 && version != 102) {
        return false;
    }

    fread(&wave_type, 1, sizeof(int), file);

    sound_vol = 0.5f;
    if (version == 102) {
        fread(&sound_vol, 1, sizeof(float), file);
    }

    fread(&p_base_freq, 1, sizeof(float), file);
    fread(&p_freq_limit, 1, sizeof(float), file);
    fread(&p_freq_ramp, 1, sizeof(float), file);
    if (version >= 101) {
        fread(&p_freq_dramp, 1, sizeof(float), file);
    }
    fread(&p_duty, 1, sizeof(float), file);
    fread(&p_duty_ramp, 1, sizeof(float), file);

    fread(&p_vib_strength, 1, sizeof(float), file);
    fread(&p_vib_speed, 1, sizeof(float), file);
    fread(&p_vib_delay, 1, sizeof(float), file);

    fread(&p_env_attack, 1, sizeof(float), file);
    fread(&p_env_sustain, 1, sizeof(float), file);
    fread(&p_env_decay, 1, sizeof(float), file);
    fread(&p_env_punch, 1, sizeof(float), file);

    fread(&filter_on, 1, sizeof(bool), file);
    fread(&p_lpf_resonance, 1, sizeof(float), file);
    fread(&p_lpf_freq, 1, sizeof(float), file);
    fread(&p_lpf_ramp, 1, sizeof(float), file);
    fread(&p_hpf_freq, 1, sizeof(float), file);
    fread(&p_hpf_ramp, 1, sizeof(float), file);

    fread(&p_pha_offset, 1, sizeof(float), file);
    fread(&p_pha_ramp, 1, sizeof(float), file);

    fread(&p_repeat_speed, 1, sizeof(float), file);

    if (version >= 101) {
        fread(&p_arp_speed, 1, sizeof(float), file);
        fread(&p_arp_mod, 1, sizeof(float), file);
    }

    fclose(file);
    return true;
}

bool Sound::saveSettings(char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        return false;
    }

    int version = 102;
    fwrite(&version, 1, sizeof(int), file);

    fwrite(&wave_type, 1, sizeof(int), file);

    fwrite(&sound_vol, 1, sizeof(float), file);

    fwrite(&p_base_freq, 1, sizeof(float), file);
    fwrite(&p_freq_limit, 1, sizeof(float), file);
    fwrite(&p_freq_ramp, 1, sizeof(float), file);
    fwrite(&p_freq_dramp, 1, sizeof(float), file);
    fwrite(&p_duty, 1, sizeof(float), file);
    fwrite(&p_duty_ramp, 1, sizeof(float), file);

    fwrite(&p_vib_strength, 1, sizeof(float), file);
    fwrite(&p_vib_speed, 1, sizeof(float), file);
    fwrite(&p_vib_delay, 1, sizeof(float), file);

    fwrite(&p_env_attack, 1, sizeof(float), file);
    fwrite(&p_env_sustain, 1, sizeof(float), file);
    fwrite(&p_env_decay, 1, sizeof(float), file);
    fwrite(&p_env_punch, 1, sizeof(float), file);

    fwrite(&filter_on, 1, sizeof(bool), file);
    fwrite(&p_lpf_resonance, 1, sizeof(float), file);
    fwrite(&p_lpf_freq, 1, sizeof(float), file);
    fwrite(&p_lpf_ramp, 1, sizeof(float), file);
    fwrite(&p_hpf_freq, 1, sizeof(float), file);
    fwrite(&p_hpf_ramp, 1, sizeof(float), file);

    fwrite(&p_pha_offset, 1, sizeof(float), file);
    fwrite(&p_pha_ramp, 1, sizeof(float), file);

    fwrite(&p_repeat_speed, 1, sizeof(float), file);

    fwrite(&p_arp_speed, 1, sizeof(float), file);
    fwrite(&p_arp_mod, 1, sizeof(float), file);

    fclose(file);
    return true;
}
