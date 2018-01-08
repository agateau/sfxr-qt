#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <QObject>

#include <basesynthesizer.h>

class QTimer;

class Synthesizer : public BaseSynthesizer {
    Q_OBJECT
public:
    explicit Synthesizer(QObject* parent = nullptr);
    ~Synthesizer();

    // Generators
    // TODO: Move outside once every changes are done through
    // setters
    Q_INVOKABLE void generatePickup();
    Q_INVOKABLE void generateLaser();
    Q_INVOKABLE void generateExplosion();
    Q_INVOKABLE void generatePowerup();
    Q_INVOKABLE void generateHitHurt();
    Q_INVOKABLE void generateJump();
    Q_INVOKABLE void generateBlipSelect();

    // Params
    void ResetParams();
    bool LoadSettings(char* filename);
    bool SaveSettings(char* filename);
    bool ExportWAV(char* filename);

    // Play
    void ResetSample(bool restart);
    void PlaySample();
    void SynthSample(int length, float* buffer, FILE* file);

    void playCallback(unsigned char* stream, int len);

private:
    // Fields editable from the outside
    float p_vib_delay;

    bool filter_on;

    float sound_vol = 0.5f;

    bool playing_sample = false;

    int wav_bits = 16;
    int wav_freq = 44100;

    // Internal
    int phase;
    double fperiod;
    double fmaxperiod;
    double fslide;
    double fdslide;
    int period;
    float square_duty;
    float square_slide;
    int env_stage;
    int env_time;
    int env_length[3];
    float env_vol;
    float fphase;
    float fdphase;
    int iphase;
    float phaser_buffer[1024];
    int ipp;
    float noise_buffer[32];
    float fltp;
    float fltdp;
    float fltw;
    float fltw_d;
    float fltdmp;
    float fltphp;
    float flthp;
    float flthp_d;
    float vib_phase;
    float vib_speed;
    float vib_amp;
    int rep_time;
    int rep_limit;
    int arp_time;
    int arp_limit;
    double arp_mod;

    bool mute_stream;
    int file_sampleswritten;
    float filesample = 0.0f;
    int fileacc = 0;

    QTimer* mPlayTimer;
    void Init();
    void schedulePlay();
};

#endif // SYNTHESIZER_H
