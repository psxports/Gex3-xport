#include "xport.h"
#include "psx.h"
#include "game_functions.h"

uint32 G3Function8004CCCC(uint32 command)
{
    FUNCTION_MARKER(0x8004cccc, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 mask = r_u32(command + 4);
    uint32 pending = r_u32(audio + 0x534);
    uint32 result = pending | mask;
    w_u32(audio + 0x534, result);
    return result;
}

uint32 G3Function800487DC(void)
{
    G3EnterCritical8008A05C();
    G3StopAudioFieldClock();
    uint32 event = r_u32(0x800ad0d0);
    DisableEvent((sint32)event);
    event = r_u32(0x800ad0d0);
    CloseEvent((sint32)event);
    uint32 audio = r_u32(0x80098ff0);
    uint32 flags = r_u16(audio + 0x12);
    w_u16(audio + 0x12, flags | 2);
    G3ExitCritical8008A03C();
    SpuSetKey(0, 0x00ffffff);
    uint32 mode = G3Function80049040();
    SpuClearReverbWorkArea((sint32)(mode & 0xffff));
    SpuQuit();
    w_u32(0x80098ff0, 0);
    return 0;
}

uint32 G3Function80049040(void)
{
    FUNCTION_MARKER(0x80049040, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 bank = r_u32(audio + 0x53c);
    return r_u8(bank + 0x14);
}

uint32 G3Function8004C42C(uint32 command)
{
    FUNCTION_MARKER(0x8004c42c, "SLUS_008.06");
    uint32 voice = 0;
    uint32 handle = r_u32(command + 4);
    uint32 audio;
    uint32 entry;
    for (;;)
    {
        audio = r_u32(0x80098ff0);
        entry = audio + (((voice << 3) - voice) << 2) + 0x248;
        if (r_u8(entry + 8) == 0xd0)
        {
            uint32 current_handle = r_u32(entry + 4);
            if (current_handle == handle)
            {
                uint32 state = r_u8(audio + voice + 0x4e8);
                if (state != 0 && state != 2)
                    break;
            }
        }
        ++voice;
        if (voice >= 0x18)
            return 0;
    }
    uint32 gain = r_u8(entry + 0x0d);
    uint32 left = gain * gain;
    gain = r_u8(entry + 0x0d);
    uint32 right = gain * gain;
    uint32 mode = r_u16(audio + 0x12);
    if ((mode & 1) == 0)
    {
        uint32 pan = r_u8(command + 2);
        if (pan >= 0x41)
        {
            uint32 attenuation = (0x7f - pan) & 0xff;
            uint32 product = (uint32)(sint32)(sint16)(right & 0xffff) * attenuation;
            product *= attenuation;
            left = (uint32)((sint32)product >> 12) & 0xffff;
        }
        else if (pan < 0x3f)
        {
            pan = r_u8(command + 2);
            uint32 product = (uint32)(sint32)(sint16)(left & 0xffff) * pan;
            product *= pan;
            right = (uint32)((sint32)product >> 12) & 0xffff;
        }
    }
    uint32 tone = r_u32(entry + 0x14);
    gain = r_u8(tone + 2);
    uint32 squared = gain * gain;
    uint32 left_product = (uint32)(sint32)(sint16)(left & 0xffff) * squared;
    uint32 right_product = (uint32)(sint32)(sint16)(right & 0xffff) * squared;
    audio = r_u32(0x80098ff0);
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    mode = r_u16(audio + 0x12);
    if ((mode & 1) == 0)
    {
        tone = r_u32(entry + 0x14);
        uint32 pan = r_u8(tone + 3);
        if (pan >= 0x41)
        {
            uint32 attenuation = (0x7f - pan) & 0xff;
            uint32 product = (uint32)(sint32)(sint16)(right & 0xffff) * attenuation;
            product *= attenuation;
            left = (uint32)((sint32)product >> 12) & 0xffff;
        }
        else if (pan < 0x3f)
        {
            pan = r_u8(tone + 3);
            uint32 product = (uint32)(sint32)(sint16)(left & 0xffff) * pan;
            product *= pan;
            right = (uint32)((sint32)product >> 12) & 0xffff;
        }
    }
    gain = r_u8(command + 1);
    squared = gain * gain;
    left_product = (uint32)(sint32)(sint16)(left & 0xffff) * squared;
    right_product = (uint32)(sint32)(sint16)(right & 0xffff) * squared;
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    gain = r_u8(command + 1);
    uint32 instrument = r_u32(entry + 0x18);
    w_u8(entry + 0x0e, gain);
    gain = r_u8(instrument + 4);
    squared = gain * gain;
    left_product = (uint32)(sint32)(sint16)left * squared;
    right_product = (uint32)(sint32)(sint16)right * squared;
    audio = r_u32(0x80098ff0);
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    gain = r_u8(audio + 0x246);
    squared = gain * gain;
    left_product = (uint32)(sint32)(sint16)left * squared;
    right_product = (uint32)(sint32)(sint16)right * squared;
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    gain = r_u8(audio + 0x10);
    squared = gain * gain;
    left_product = (uint32)(sint32)(sint16)left * squared;
    right_product = (uint32)(sint32)(sint16)right * squared;
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    SpuSetVoiceVolume((sint32)voice, (sint16)left, (sint16)right);
    return 0;
}

uint32 G3Function8004CCEC(uint32 command)
{
    FUNCTION_MARKER(0x8004ccec, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 mask = r_u32(command + 4);
    uint32 pending = r_u32(audio + 0x530);
    w_u32(audio + 0x530, pending | mask);
    mask = r_u32(command + 4);
    pending = r_u32(audio + 0x534);
    uint32 result = ~mask;
    w_u32(audio + 0x534, pending & result);
    return result;
}

uint32 G3Function8004CDE4(uint32 voice, uint32 adsr_1, uint32 adsr_2)
{
    FUNCTION_MARKER(0x8004cde4, "SLUS_008.06");
    uint32 level = adsr_1 & 0x0f;
    uint32 first = adsr_1 & 0xffff;
    uint32 decay = (first >> 4) & 0x0f;
    uint32 attack = (first >> 8) & 0x7f;
    uint32 attack_mode = (adsr_1 & 0x8000) != 0 ? 5 : 1;
    uint32 release = adsr_2 & 0x1f;
    uint32 release_mode = (adsr_2 & 0x20) != 0 ? 7 : 3;
    uint32 sustain = (adsr_2 >> 6) & 0x7f;
    uint32 sustain_mode;
    if ((adsr_2 & 0x4000) != 0)
        sustain_mode = (adsr_2 & 0x8000) != 0 ? 7 : 3;
    else
        sustain_mode = (adsr_2 & 0x8000) != 0 ? 5 : 1;
    return G3Function8008C6D4(voice, attack, decay, sustain, release, level, attack_mode, sustain_mode, release_mode);
}

uint32 G3Function8004C2C0(uint32 command)
{
    FUNCTION_MARKER(0x8004c2c0, "SLUS_008.06");
    uint32 released = 0;
    uint32 handle = r_u32(command + 4);
    uint32 voice = 0;
    do
    {
        uint32 audio = r_u32(0x80098ff0);
        uint32 entry = audio + (((voice << 3) - voice) << 2) + 0x248;
        if (r_u8(entry + 8) == 0xd0)
        {
            uint32 current_handle = r_u32(entry + 4);
            if (current_handle == handle)
            {
                uint32 state = r_u8(audio + voice + 0x4e8);
                if (state != 0 && state != 2)
                {
                    uint32 bit = r_u32(entry);
                    w_u8(entry + 8, 0xff);
                    released |= bit;
                }
            }
        }
        ++voice;
    } while (voice < 0x18);
    uint32 audio = r_u32(0x80098ff0);
    uint32 pending = r_u32(audio + 0x530);
    w_u32(audio + 0x530, pending | released);
    pending = r_u32(audio + 0x534);
    uint32 result = pending & ~released;
    w_u32(audio + 0x534, result);
    return result;
}

uint32 G3Function8004C380(void)
{
    FUNCTION_MARKER(0x8004c380, "SLUS_008.06");
    uint32 released = 0;
    uint32 voice = 0;
    do
    {
        uint32 audio = r_u32(0x80098ff0);
        uint32 entry = audio + (((voice << 3) - voice) << 2) + 0x248;
        if (r_u8(entry + 8) == 0xd0)
        {
            uint32 state = r_u8(audio + voice + 0x4e8);
            if (state != 0 && state != 2)
            {
                uint32 bit = r_u32(entry);
                w_u8(entry + 8, 0xff);
                released |= bit;
            }
        }
        ++voice;
    } while (voice < 0x18);
    uint32 audio = r_u32(0x80098ff0);
    uint32 pending = r_u32(audio + 0x530);
    w_u32(audio + 0x530, pending | released);
    pending = r_u32(audio + 0x534);
    uint32 result = pending & ~released;
    w_u32(audio + 0x534, result);
    return result;
}

uint32 G3Function8004CD20(uint32 priority)
{
    FUNCTION_MARKER(0x8004cd20, "SLUS_008.06");
    uint32 minimum = 0x7fff;
    uint32 candidate = 0;
    uint32 voice = 0;
    uint32 audio = r_u32(0x80098ff0);
    do
    {
        uint32 entry = audio + (((voice << 3) - voice) << 2) + 0x248;
        uint32 flags = r_u16(entry + 0x12);
        if ((flags & 1) == 0)
        {
            uint32 state = r_u8(audio + voice + 0x4e8);
            if (state == 0)
            {
                w_u8(audio + voice + 0x4e8, 1);
                flags = r_u16(entry + 0x12);
                w_u16(entry + 0x12, flags | 2);
                return entry;
            }
            uint32 current_priority = r_u8(entry + 9);
            if (current_priority < minimum)
            {
                minimum = current_priority;
                candidate = entry;
            }
        }
        ++voice;
    } while (voice < 0x18);
    if ((priority & 0xffff) < minimum)
        return 0;
    uint32 flags = r_u16(candidate + 0x12);
    w_u16(candidate + 0x12, flags | 2);
    return candidate;
}

uint32 G3Function8004C154(uint32 command)
{
    FUNCTION_MARKER(0x8004c154, "SLUS_008.06");
    uint32 handle = r_u32(command + 4);
    uint32 audio = r_u32(0x80098ff0);
    uint32 group = (handle >> 8) & 0x0f;
    uint32 bank = r_u32(audio + 0x53c);
    uint32 tone_index = r_u16(bank + (group << 1) + 0x60);
    uint32 samples = r_u32(audio + 0x548);
    uint32 instrument_offset = r_u8(bank + group + 0x50);
    tone_index = (tone_index + (handle & 0xff)) & 0xffff;
    uint32 tones = r_u32(audio + 0x544);
    uint32 tone = tones + (tone_index << 4);
    uint32 sample_index = (uint32)(sint32)r_s16(tone + 0x0e);
    uint32 priority = r_u8(tone);
    uint32 instrument_index = r_u8(tone + 1);
    uint32 note = r_u8(tone + 6);
    uint32 instruments = r_u32(audio + 0x540);
    uint32 sample = r_u32(samples + (sample_index << 2));
    uint32 instrument = instruments + ((instrument_index + instrument_offset) << 3);
    uint32 voice_entry = G3Function8004CD20(priority);
    if (voice_entry == 0)
        return 0;
    uint32 volume = r_u8(command + 1);
    audio = r_u32(0x80098ff0);
    uint32 pan = r_u8(command + 2);
    uint32 global_gain = r_u8(audio + 0x246);
    uint32 master_gain = r_u8(audio + 0x10);
    uint32 pitch_offset = (uint32)(sint32)r_s16(command + 0x0c);
    G3Function8004CE8C(tone, sample, instrument, note, 0x7f, volume, pan, global_gain, master_gain, voice_entry, pitch_offset);
    w_u32(voice_entry + 4, handle);
    w_u8(voice_entry + 8, 0xd0);
    priority = r_u8(tone);
    w_u8(voice_entry + 0x0a, note);
    w_u8(voice_entry + 9, priority);
    instrument_index = r_u8(tone + 1);
    w_u8(voice_entry + 0x0d, 0x7f);
    w_u8(voice_entry + 0x0b, instrument_index);
    volume = r_u8(command + 1);
    w_u8(voice_entry + 0x0e, volume);
    pan = r_u8(command + 2);
    w_u32(voice_entry + 0x18, instrument);
    w_u32(voice_entry + 0x14, tone);
    w_u8(voice_entry + 0x0f, pan);
    return pan;
}

uint32 G3Function8004CE8C(uint32 tone, uint32 sample, uint32 instrument, uint32 note, uint32 gain, uint32 volume, uint32 pan, uint32 global_gain, uint32 master_gain, uint32 voice_entry, uint32 pitch_offset)
{
    FUNCTION_MARKER(0x8004ce8c, "SLUS_008.06");
    volume &= 0xffff;
    pan &= 0xffff;
    global_gain &= 0xffff;
    master_gain &= 0xffff;
    pitch_offset &= 0xffff;
    uint32 audio = r_u32(0x80098ff0);
    uint32 left = (gain * gain) & 0xffff;
    uint32 right = left;
    uint32 mode = r_u16(audio + 0x12);
    if ((mode & 1) == 0)
    {
        if (pan >= 0x41)
        {
            uint32 attenuation = (0x7f - pan) & 0xff;
            uint32 product = (uint32)(sint32)(sint16)left * attenuation;
            product *= attenuation;
            left = (uint32)((sint32)product >> 12) & 0xffff;
        }
        else if (pan < 0x3f)
        {
            uint32 product = (uint32)(sint32)(sint16)left * pan;
            product *= pan;
            right = (uint32)((sint32)product >> 12) & 0xffff;
        }
    }
    uint32 tone_gain = r_u8(tone + 2);
    uint32 squared = tone_gain * tone_gain;
    uint32 left_product = (uint32)(sint32)(sint16)left * squared;
    uint32 right_product = (uint32)(sint32)(sint16)right * squared;
    audio = r_u32(0x80098ff0);
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    mode = r_u16(audio + 0x12);
    if ((mode & 1) == 0)
    {
        uint32 tone_pan = r_u8(tone + 3);
        if (tone_pan >= 0x41)
        {
            uint32 attenuation = (0x7f - tone_pan) & 0xff;
            uint32 product = (uint32)(sint32)(sint16)right * attenuation;
            product *= attenuation;
            left = (uint32)((sint32)product >> 12) & 0xffff;
        }
        else if (tone_pan < 0x3f)
        {
            tone_pan = r_u8(tone + 3);
            uint32 product = (uint32)(sint32)(sint16)left * tone_pan;
            product *= tone_pan;
            right = (uint32)((sint32)product >> 12) & 0xffff;
        }
    }
    squared = volume * volume;
    left_product = (uint32)(sint32)(sint16)left * squared;
    right_product = (uint32)(sint32)(sint16)right * squared;
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    uint32 instrument_gain = r_u8(instrument + 4);
    squared = instrument_gain * instrument_gain;
    left_product = (uint32)(sint32)(sint16)left * squared;
    right_product = (uint32)(sint32)(sint16)right * squared;
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    squared = global_gain * global_gain;
    left_product = (uint32)(sint32)(sint16)left * squared;
    right_product = (uint32)(sint32)(sint16)right * squared;
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    squared = master_gain * master_gain;
    left_product = (uint32)(sint32)(sint16)left * squared;
    right_product = (uint32)(sint32)(sint16)right * squared;
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    uint32 voice = r_u8(voice_entry + 0x10);
    SpuSetVoiceVolume((sint32)voice, (sint16)left, (sint16)right);
    uint32 base_note = r_u8(tone + 4);
    uint32 tuning = r_u8(tone + 5);
    uint32 pitch_index = (note - (base_note - 0x3c)) & 0xffff;
    uint32 pitch;
    if ((tuning & 0x80) != 0)
    {
        tuning = r_u8(tone + 5);
        uint32 coefficient = r_u32(0x800992a0 + (pitch_index << 2));
        uint32 scaled = (((((coefficient << 1) + coefficient) << 3) + coefficient) << 2);
        uint32 product = scaled * ((0x100 - tuning) & 0xffff);
        pitch = r_u16(0x800991f4 + (pitch_index << 1));
        pitch -= product >> 23;
    }
    else
    {
        uint32 coefficient = r_u32(0x800992a0 + (pitch_index << 2));
        uint32 scaled = (((((coefficient << 1) + coefficient) << 3) + coefficient) << 2);
        tuning = r_u8(tone + 5);
        uint32 product = scaled * tuning;
        pitch = r_u16(0x800991f4 + (pitch_index << 1));
        pitch += product >> 23;
    }
    pitch += pitch_offset;
    voice = r_u8(voice_entry + 0x10);
    SpuSetVoicePitch((sint32)voice, (uint16)(pitch & 0xffff));
    voice = r_u8(voice_entry + 0x10);
    SpuSetVoiceStartAddr((sint32)voice, sample);
    voice = r_u8(voice_entry + 0x10);
    uint32 adsr_1 = r_u16(tone + 0x0a);
    uint32 adsr_2 = r_u16(tone + 0x0c);
    G3Function8004CDE4(voice, adsr_1, adsr_2);
    uint32 effect = r_u8(tone + 8);
    audio = r_u32(0x80098ff0);
    uint32 bit = r_u32(voice_entry);
    uint32 effects = r_u32(audio + 0x538);
    if (effect == 4)
        w_u32(audio + 0x538, effects | bit);
    else
        w_u32(audio + 0x538, effects & ~bit);
    audio = r_u32(0x80098ff0);
    bit = r_u32(voice_entry);
    uint32 pending = r_u32(audio + 0x534);
    uint32 result = pending | bit;
    w_u32(audio + 0x534, result);
    return result;
}

uint32 G3Function8004C78C(uint32 command)
{
    FUNCTION_MARKER(0x8004c78c, "SLUS_008.06");
    uint32 voice = 0;
    uint32 handle = r_u32(command + 4);
    uint32 audio;
    uint32 entry;
    for (;;)
    {
        audio = r_u32(0x80098ff0);
        entry = audio + (((voice << 3) - voice) << 2) + 0x248;
        if (r_u8(entry + 8) == 0xd0)
        {
            uint32 current_handle = r_u32(entry + 4);
            if (current_handle == handle)
            {
                uint32 state = r_u8(audio + voice + 0x4e8);
                if (state != 0 && state != 2)
                    break;
            }
        }
        ++voice;
        if (voice >= 0x18)
            return 0;
    }
    uint32 gain = r_u8(entry + 0x0d);
    uint32 left = gain * gain;
    gain = r_u8(entry + 0x0d);
    uint32 right = gain * gain;
    uint32 mode = r_u16(audio + 0x12);
    if ((mode & 1) == 0)
    {
        uint32 pan = r_u8(command + 2);
        if (pan >= 0x41)
        {
            uint32 attenuation = (0x7f - pan) & 0xff;
            uint32 product = (uint32)(sint32)(sint16)(right & 0xffff) * attenuation;
            product *= attenuation;
            left = (uint32)((sint32)product >> 12) & 0xffff;
        }
        else if (pan < 0x3f)
        {
            pan = r_u8(command + 2);
            uint32 product = (uint32)(sint32)(sint16)(left & 0xffff) * pan;
            product *= pan;
            right = (uint32)((sint32)product >> 12) & 0xffff;
        }
    }
    uint32 tone = r_u32(entry + 0x14);
    gain = r_u8(tone + 2);
    uint32 squared = gain * gain;
    uint32 left_product = (uint32)(sint32)(sint16)(left & 0xffff) * squared;
    uint32 right_product = (uint32)(sint32)(sint16)(right & 0xffff) * squared;
    audio = r_u32(0x80098ff0);
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    mode = r_u16(audio + 0x12);
    if ((mode & 1) == 0)
    {
        tone = r_u32(entry + 0x14);
        uint32 pan = r_u8(tone + 3);
        if (pan >= 0x41)
        {
            uint32 attenuation = (0x7f - pan) & 0xff;
            uint32 product = (uint32)(sint32)(sint16)(right & 0xffff) * attenuation;
            product *= attenuation;
            left = (uint32)((sint32)product >> 12) & 0xffff;
        }
        else if (pan < 0x3f)
        {
            pan = r_u8(tone + 3);
            uint32 product = (uint32)(sint32)(sint16)(left & 0xffff) * pan;
            product *= pan;
            right = (uint32)((sint32)product >> 12) & 0xffff;
        }
    }
    gain = r_u8(command + 1);
    squared = gain * gain;
    left_product = (uint32)(sint32)(sint16)(left & 0xffff) * squared;
    right_product = (uint32)(sint32)(sint16)(right & 0xffff) * squared;
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    gain = r_u8(command + 1);
    uint32 instrument = r_u32(entry + 0x18);
    w_u8(entry + 0x0e, gain);
    gain = r_u8(instrument + 4);
    squared = gain * gain;
    left_product = (uint32)(sint32)(sint16)left * squared;
    right_product = (uint32)(sint32)(sint16)right * squared;
    audio = r_u32(0x80098ff0);
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    gain = r_u8(audio + 0x246);
    squared = gain * gain;
    left_product = (uint32)(sint32)(sint16)left * squared;
    right_product = (uint32)(sint32)(sint16)right * squared;
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    gain = r_u8(audio + 0x10);
    squared = gain * gain;
    left_product = (uint32)(sint32)(sint16)left * squared;
    right_product = (uint32)(sint32)(sint16)right * squared;
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    SpuSetVoiceVolume((sint32)voice, (sint16)left, (sint16)right);
    tone = r_u32(entry + 0x14);
    uint32 note = r_u8(entry + 0x0a);
    uint32 base_note = r_u8(tone + 4);
    uint32 tuning = r_u8(tone + 5);
    uint32 pitch_index = (note - (base_note - 0x3c)) & 0xffff;
    uint32 pitch;
    if ((tuning & 0x80) != 0)
    {
        tuning = r_u8(tone + 5);
        uint32 coefficient = r_u32(0x800992a0 + (pitch_index << 2));
        uint32 scaled = (((((coefficient << 1) + coefficient) << 3) + coefficient) << 2);
        uint32 product = scaled * ((0x100 - tuning) & 0xffff);
        pitch = r_u16(0x800991f4 + (pitch_index << 1));
        pitch -= product >> 23;
    }
    else
    {
        uint32 coefficient = r_u32(0x800992a0 + (pitch_index << 2));
        uint32 scaled = (((((coefficient << 1) + coefficient) << 3) + coefficient) << 2);
        tuning = r_u8(tone + 5);
        uint32 product = scaled * tuning;
        pitch = r_u16(0x800991f4 + (pitch_index << 1));
        pitch += product >> 23;
    }
    uint32 pitch_offset = r_u16(command + 0x0c);
    SpuSetVoicePitch((sint32)voice, (uint16)((pitch + pitch_offset) & 0xffff));
    return 0;
}

uint32 G3Function8004CBF4(uint32 command)
{
    FUNCTION_MARKER(0x8004cbf4, "SLUS_008.06");
    uint32 callback = r_u32(command + 4);
    uint32 entry = G3Function8004CD20(0xff);
    uint32 mask = 0;
    if (entry != 0)
    {
        uint32 flags = r_u16(entry + 0x12);
        mask = r_u32(entry);
        w_u16(entry + 0x12, flags | 1);
    }
    return G3DispatchAllocatedVoice(callback, mask);
}

uint32 G3Function8004C108(uint32 command)
{
    FUNCTION_MARKER(0x8004c108, "SLUS_008.06");
    uint32 index = r_u8(command);
    if (index >= 9)
        return 0x800a0000;
    index = r_u8(command);
    uint32 target = r_u32(0x800991d0 + (index << 2));
    return G3DispatchAudioCommand(target, command);
}

uint32 G3Function80049D50(uint32 event, uint32 bank, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80049d50, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 status = r_u8(event + 4);
    uint32 target;
    if ((status & 0x80) != 0)
        target = r_u32(0x80099040 + ((status >> 2) & 0x1c));
    else
    {
        G3Function8004AC38(event, bank);
        status = r_u8(event + 4);
        uint32 index = status & 0x7f;
        if (status >= 0x4c)
            return 0;
        target = r_u32(0x800990a0 + (index << 2));
    }
    return G3DispatchAudioEvent(target, event, bank, frame);
}

uint32 G3Function8003EF1C(void)
{
    FUNCTION_MARKER(0x8003ef1c, "SLUS_008.06");
    while (SpuIsTransferCompleted(0) == 0)
    {
    }
    G3Function8004BC18();
    G3Function80049434();
    G3Function80049144();
    return G3Function80049B20();
}

uint32 G3Function8003EBE0(uint32 filename, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003ebe0, "SLUS_008.06");
    uint32 frame = guest_stack - 0x68;
    G3Function80049A18(1);
    uint32 game = r_u32(0x80097048);
    if (r_u8(game + 0x67df) != 0)
        G3Function80049144();
    game = r_u32(0x80097048);
    w_u8(game + 0x67df, 0);
    G3CopyString8008935C(frame + 0x10, filename);
    G3SdkConcat8008BFE8(frame + 0x10, 0x8009df34);
    uint32 data = G3LoadFile80064A7C(frame + 0x10, frame);
    G3Function80048DBC(data);
    G3Function800490BC();
    G3MemPackPush8004829C();
    G3CopyString8008935C(frame + 0x10, filename);
    G3SdkConcat8008BFE8(frame + 0x10, 0x8009df3c);
    data = G3LoadFile80064A7C(frame + 0x10, frame);
    G3Function80048EF0(data);
    G3MemPackPop80048320();
    G3Function80048648(frame);
    game = r_u32(0x80097048);
    uint32 target = (uint32)(sint32)r_s16(game + 0x67cc);
    G3Function800486E0(target, 0x100, 0);
    game = r_u32(0x80097048);
    w_u8(game + 0x67df, 1);
    return G3Function80049A18(0);
}

uint32 G3Function80049A18(uint32 paused)
{
    FUNCTION_MARKER(0x80049a18, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 flags = r_u16(audio + 0x12);
    if ((paused & 0xffff) != 0)
        flags |= 2;
    else
        flags &= 0xfffd;
    w_u16(audio + 0x12, flags);
    return 0;
}

uint32 G3Function8004BC18(void)
{
    FUNCTION_MARKER(0x8004bc18, "SLUS_008.06");
    G3Function8004C008(4, 0, 0, 0, 0, 0);
    return 0;
}

uint32 G3Function8004AC38(uint32 event, uint32 bank)
{
    FUNCTION_MARKER(0x8004ac38, "SLUS_008.06");
    uint32 status = r_u8(event + 4);
    if (status - 0x41 < 3)
        return 1;
    uint32 flags = r_u16(bank + 0xf4);
    if ((flags & 2) != 0)
    {
        w_u16(bank + 0xf4, flags & 0xfffd);
        uint32 audio = r_u32(0x80098ff0);
        uint32 index = r_u8(event + 5);
        uint32 value = r_u8(audio + index + 0x564);
        w_u8(event + 5, value);
        flags = r_u16(bank + 0xf4);
    }
    if ((flags & 4) != 0)
    {
        w_u16(bank + 0xf4, flags & 0xfffb);
        uint32 audio = r_u32(0x80098ff0);
        uint32 index = r_u8(event + 6);
        uint32 value = r_u8(audio + index + 0x564);
        w_u8(event + 6, value);
    }
    flags = r_u16(bank + 0xf4);
    uint32 result = flags & 0xfff7;
    if ((flags & 8) != 0)
    {
        w_u16(bank + 0xf4, result);
        uint32 audio = r_u32(0x80098ff0);
        uint32 index = r_u8(event + 7);
        result = r_u8(audio + index + 0x564);
        w_u8(event + 7, result);
    }
    return result;
}

uint32 G3Function80048D94(uint32 first, uint32 second, uint32 third, uint32 fourth)
{
    FUNCTION_MARKER(0x80048d94, "SLUS_008.06");
    return (fourth & 0xff) | ((third & 0xff) << 8) | ((second & 0xff) << 16) | (first << 24);
}

uint32 G3Function80048DBC(uint32 data)
{
    FUNCTION_MARKER(0x80048dbc, "SLUS_008.06");
    uint32 expected = G3Function80048D94(0x61, 0x53, 0x4e, 0x44);
    uint32 magic = r_u32(data);
    if (magic != expected)
        return 0x1001;
    if (r_u16(data + 8) != 0x106)
        return 0x1002;
    uint32 audio = r_u32(0x80098ff0);
    uint32 offset = r_u32(data + 4);
    uint32 instrument_count = r_u8(data + 0x0b);
    uint32 tone_count = r_u16(data + 0x0c);
    uint32 sample_count = r_u16(data + 0x0e);
    uint32 sequence_count = r_u8(data + 0x10);
    uint32 word_count = r_u16(data + 0x12);
    uint32 instruments = data + offset;
    w_u32(audio + 0x540, instruments);
    uint32 tones = instruments + (instrument_count << 3);
    w_u32(audio + 0x544, tones);
    uint32 samples = tones + (tone_count << 4);
    uint32 sequences = samples + (sample_count << 2);
    uint32 words = sequences + (sequence_count << 2);
    w_u32(audio + 0x53c, data);
    w_u32(audio + 0x548, samples);
    w_u32(audio + 0x54c, sequences);
    w_u32(audio + 0x550, words);
    sequence_count = r_u8(data + 0x10);
    uint32 payload = words + (word_count << 2);
    if (sequence_count != 0)
    {
        uint32 index = 0;
        do
        {
            uint32 slot = (index & 0xffff) << 2;
            ++index;
            uint32 destination = r_u32(audio + 0x54c);
            uint32 relative = r_u32(sequences + slot);
            w_u32(destination + slot, payload + relative);
            sequence_count = r_u8(data + 0x10);
        } while ((index & 0xffff) < sequence_count);
    }
    uint32 index = 0;
    do
    {
        audio = r_u32(0x80098ff0);
        uint32 slot = index & 0xffff;
        ++index;
        w_u8(audio + slot + 0x564, 0);
    } while ((index & 0xffff) < 0x80);
    return 0;
}

uint32 G3Function80048EF0(uint32 data)
{
    FUNCTION_MARKER(0x80048ef0, "SLUS_008.06");
    uint32 bytes = r_u32(data + 4);
    SpuSetTransferStartAddr(0x3010);
    uint32 transfer_bytes = bytes > 0x7eff0 ? 0x7eff0 : bytes;
    uint32 transfer_span = (transfer_bytes + 0x3f) & ~(uint32)0x3f;
    SpuWrite(psx_addr(data + 8, transfer_span), transfer_bytes);
    uint32 audio = r_u32(0x80098ff0);
    uint32 bank = r_u32(audio + 0x53c);
    uint32 count = r_u16(bank + 0x0e);
    if (count != 0)
    {
        uint32 index = 0;
        do
        {
            uint32 samples = r_u32(audio + 0x548);
            uint32 entry = samples + (index << 2);
            uint32 address = r_u32(entry);
            w_u32(entry, address + 0x3010);
            bank = r_u32(audio + 0x53c);
            count = r_u16(bank + 0x0e);
            ++index;
        } while ((sint32)index < (sint32)count);
    }
    while (SpuIsTransferCompleted(0) == 0)
    {
    }
    return 0;
}

uint32 G3Function80049060(void)
{
    FUNCTION_MARKER(0x80049060, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 bank = r_u32(audio + 0x53c);
    return (uint32)(sint32)r_s16(bank + 0x16);
}

uint32 G3Function80049080(void)
{
    FUNCTION_MARKER(0x80049080, "SLUS_008.06");
    uint32 remaining = 0x2710;
    do
    {
        if (SpuIsTransferCompleted(0) != 0)
            return 1;
        --remaining;
    } while (remaining != 0);
    return 0;
}

uint32 G3Function800490BC(void)
{
    FUNCTION_MARKER(0x800490bc, "SLUS_008.06");
    uint32 mode = G3Function80049040();
    G3Function8008C3A4(mode & 0xffff);
    SpuSetReverbVoice(0, 0x00ffffff);
    if (G3Function80049080() != 0)
    {
        mode = G3Function80049040();
        SpuClearReverbWorkArea((sint32)(mode & 0xffff));
    }
    uint32 left = G3Function80049060();
    left = (uint32)(sint32)(sint16)(left & 0xffff);
    uint32 right = G3Function80049060();
    right = (uint32)(sint32)(sint16)(right & 0xffff);
    G3SetReverbDepth8008C098(left, right);
    SpuSetReverb(1);
    return 0;
}

uint32 G3Function80049144(void)
{
    FUNCTION_MARKER(0x80049144, "SLUS_008.06");
    if (G3Function80049080() != 0)
    {
        uint32 mode = G3Function80049040();
        SpuClearReverbWorkArea((sint32)(mode & 0xffff));
    }
    return 0;
}

uint32 G3Function80049434(void)
{
    FUNCTION_MARKER(0x80049434, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 count = r_u8(audio + 4);
    if (count != 0)
    {
        uint32 index = 0;
        do
        {
            audio = r_u32(0x80098ff0);
            uint32 bank = r_u32(audio + ((index & 0xffff) << 2) + 0x20);
            uint32 flags = r_u16(bank + 0xf2);
            if ((flags & 1) != 0)
                G3Function800493C0(index & 0xffff);
            audio = r_u32(0x80098ff0);
            ++index;
            w_u16(bank + 0xee, 0xff);
            count = r_u8(audio + 4);
        } while ((index & 0xffff) < count);
    }
    return 0;
}

uint32 G3Function80049B20(void)
{
    FUNCTION_MARKER(0x80049b20, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 flags = r_u16(audio + 0x12);
    w_u16(audio + 0x12, flags & 0xfff3);
    return 0;
}

uint32 G3Function800493C0(uint32 index)
{
    FUNCTION_MARKER(0x800493c0, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 count = r_u8(audio + 4);
    index &= 0xffff;
    if (index < count)
    {
        uint32 bank = r_u32(audio + (index << 2) + 0x20);
        if (r_u16(bank + 0xee) != 0xff)
        {
            uint32 flags = r_u16(bank + 0xf2);
            w_u16(bank + 0xf2, flags & 0xfffe);
            G3Function80048FA4(bank);
            G3Function800496BC(index);
        }
    }
    return 0;
}

uint32 G3Function80048FA4(uint32 bank)
{
    FUNCTION_MARKER(0x80048fa4, "SLUS_008.06");
    uint32 flags = r_u16(bank + 0xf4);
    w_u16(bank + 0xf2, 0);
    w_u16(bank + 0xf4, (flags & 0x20) | 1);
    uint32 audio = r_u32(0x80098ff0);
    uint32 index = r_u16(bank + 0xee);
    uint32 sequences = r_u32(audio + 0x54c);
    uint32 sequence = r_u32(sequences + (index << 2));
    w_u16(bank + 0xda, 0);
    w_u8(bank + 0xd8, 0);
    w_u8(bank + 0xd9, 0);
    w_u32(bank + 0xe4, sequence + 0x0c);
    uint32 channel = 0;
    do
    {
        uint32 slot = channel & 0xffff;
        ++channel;
        w_u8(bank + slot + 0x126, 0xff);
        w_u8(bank + slot + 0x136, 0);
        w_u8(bank + slot + 0x146, 0x7f);
        w_u8(bank + slot + 0x156, 0x3f);
        w_u16(bank + (slot << 1) + 0x106, 0x2000);
    } while ((channel & 0xffff) < 0x10);
    uint32 result = r_u8(bank + 0xf0);
    w_u32(bank + 0xe8, bank);
    w_u8(bank + 0xec, result);
    return result;
}

uint32 G3Function800496BC(uint32 index)
{
    FUNCTION_MARKER(0x800496bc, "SLUS_008.06");
    uint32 released = 0;
    uint32 audio = r_u32(0x80098ff0);
    uint32 bank = r_u32(audio + ((index & 0xffff) << 2) + 0x20);
    uint32 voice = 0;
    do
    {
        audio = r_u32(0x80098ff0);
        uint32 entry = audio + (((voice << 3) - voice) << 2) + 0x248;
        uint32 voice_flags = r_u8(entry + 8);
        uint32 bank_flags = r_u8(bank + 0xfd);
        if ((voice_flags & 0xf0) == bank_flags)
        {
            uint32 bit = r_u32(entry);
            uint32 flags = r_u16(entry + 0x12);
            w_u8(entry + 8, 0xff);
            released |= bit;
            w_u16(entry + 0x12, flags | 2);
        }
        ++voice;
    } while (voice < 0x18);
    if (released != 0)
    {
        audio = r_u32(0x80098ff0);
        uint32 pending = r_u32(audio + 0x530);
        w_u32(audio + 0x530, pending | released);
        pending = r_u32(audio + 0x534);
        w_u32(audio + 0x534, pending & ~released);
    }
    return 0;
}

uint32 G3Function8004AA54(uint32 event, uint32 bank, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004aa54, "SLUS_008.06");
    uint32 frame = guest_stack - 0x18;
    uint32 command = r_u8(event + 5);
    uint32 target = r_u32(0x80099060 + ((command & 0x0f) << 2));
    return G3DispatchAudioEvent(target, event, bank, frame);
}

uint32 G3Function8004AA94(uint32 event, uint32 bank)
{
    FUNCTION_MARKER(0x8004aa94, "SLUS_008.06");
    uint32 channel = r_u8(event + 4);
    uint32 value = r_u8(event + 5);
    channel &= 0x0f;
    w_u8(bank + channel + 0x126, value);
    return channel;
}

uint32 G3Function8004AAB4(uint32 event, uint32 bank)
{
    FUNCTION_MARKER(0x8004aab4, "SLUS_008.06");
    uint32 channel = r_u8(event + 4);
    uint32 high = r_u8(event + 6);
    uint32 low = r_u8(event + 5);
    channel &= 0x0f;
    uint32 value = low | (high << 7);
    w_u16(bank + (channel << 1) + 0x106, value);
    return G3Function8004A854(bank, channel);
}

uint32 G3Function8004AD20(uint32 event, uint32 bank)
{
    FUNCTION_MARKER(0x8004ad20, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 index = r_u8(event + 5);
    uint32 count = r_u8(audio + 4);
    if (index < count)
    {
        uint32 target = r_u32(audio + (index << 2) + 0x20);
        w_u8(bank + 0xec, index);
        w_u32(bank + 0xe8, target);
        return target;
    }
    if (index == 0x7f)
    {
        uint32 own_index = r_u8(bank + 0xf0);
        w_u32(bank + 0xe8, bank);
        w_u8(bank + 0xec, own_index);
        return own_index;
    }
    return 0x7f;
}

uint32 G3Function8004AD74(uint32 event, uint32 bank, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004ad74, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 sequence = r_u8(event + 5);
    uint32 count = G3Function8004917C();
    if (sequence >= (count & 0xffff))
        return 0;
    uint32 index = r_u8(bank + 0xec);
    return G3Function800491AC(sequence, index, frame);
}

uint32 G3Function8004AEA0(uint32 event, uint32 bank)
{
    FUNCTION_MARKER(0x8004aea0, "SLUS_008.06");
    uint32 index = r_u8(bank + 0xec);
    return G3Function8004933C(index);
}

uint32 G3Function8004AF40(uint32 event, uint32 bank)
{
    FUNCTION_MARKER(0x8004af40, "SLUS_008.06");
    uint32 volume = r_u8(event + 5);
    uint32 target = r_u32(bank + 0xe8);
    w_u8(target + 0xfa, volume);
    target = r_u32(bank + 0xe8);
    return G3Function8004A504(target);
}

uint32 G3Function8004B44C(uint32 event)
{
    FUNCTION_MARKER(0x8004b44c, "SLUS_008.06");
    uint32 index = r_u8(event + 6);
    uint32 value = r_u8(event + 5);
    if (index >= 0x80)
        return 0x800a0000;
    uint32 target = r_u32(0x80098ff0) + index;
    w_u8(target + 0x564, value);
    return target;
}

uint32 G3Function8004B4B8(uint32 event)
{
    FUNCTION_MARKER(0x8004b4b8, "SLUS_008.06");
    uint32 index = r_u8(event + 6);
    uint32 value = r_u8(event + 5);
    if (index >= 0x80)
        return 0x800a0000;
    uint32 target = r_u32(0x80098ff0) + index;
    uint32 previous = r_u8(target + 0x564);
    uint32 result = value + previous;
    w_u8(target + 0x564, result);
    return result;
}

uint32 G3Function8004B678(uint32 event, uint32 bank)
{
    FUNCTION_MARKER(0x8004b678, "SLUS_008.06");
    uint32 label = r_u8(event + 5);
    return G3Function8004B598(bank, label);
}

uint32 G3Function8004B758(uint32 event, uint32 bank)
{
    FUNCTION_MARKER(0x8004b758, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 index = r_u8(event + 5);
    uint32 value = r_u8(audio + index + 0x564);
    uint32 threshold = r_u8(event + 6);
    uint32 label = r_u8(event + 7);
    if (value < threshold)
        return G3Function8004B598(bank, label);
    return 0;
}

uint32 G3Function8004B938(uint32 event, uint32 bank)
{
    FUNCTION_MARKER(0x8004b938, "SLUS_008.06");
    uint32 flags = r_u16(bank + 0xf4) | 4;
    w_u16(bank + 0xf4, flags);
    return flags;
}

uint32 G3Function8004B598(uint32 bank, uint32 label)
{
    FUNCTION_MARKER(0x8004b598, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 sequence_index = r_u16(bank + 0xee);
    uint32 sequence_table = r_u32(audio + 0x54c);
    uint32 label_table = r_u32(audio + 0x550);
    uint32 sequence = r_u32(sequence_table + (sequence_index << 2));
    uint32 offset = r_u32(label_table + ((label & 0xff) << 2));
    uint32 count = r_u16(bank + 0xda);
    w_u32(bank + 0xe4, sequence + offset + 0x0c);
    if (count != 0)
    {
        do
        {
            uint32 head = r_u8(bank + 0xd9);
            count = r_u16(bank + 0xda);
            uint32 elapsed = r_u32(bank + head * 12 + 0x18);
            head = r_u8(bank + 0xd9);
            w_u16(bank + 0xda, count - 1);
            uint32 time = r_u32(bank + 0xdc);
            head = (head + 1) & 0xff;
            w_u8(bank + 0xd9, head);
            w_u32(bank + 0xdc, time - elapsed);
            if (head == 0x10)
                w_u8(bank + 0xd9, 0);
            count = r_u16(bank + 0xda);
        } while (count != 0);
    }
    uint32 flags = r_u16(bank + 0xf4) & 0xffef;
    w_u16(bank + 0xf4, flags);
    return flags;
}

uint32 G3Function8004917C(void)
{
    FUNCTION_MARKER(0x8004917c, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 bank = r_u32(audio + 0x53c);
    if (bank != 0)
        return r_u8(bank + 0x10);
    return 0;
}

uint32 G3Function8004933C(uint32 index)
{
    FUNCTION_MARKER(0x8004933c, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 count = r_u8(audio + 4);
    index &= 0xffff;
    if (index < count)
    {
        uint32 bank = r_u32(audio + (index << 2) + 0x20);
        uint32 flags = r_u16(bank + 0xf2);
        if ((flags & 1) == 0)
        {
            uint32 sequence = r_u16(bank + 0xee);
            if (sequence != 0xff)
            {
                G3Function80048FA4(bank);
                flags = r_u16(bank + 0xf2);
                w_u16(bank + 0xf2, flags | 1);
            }
        }
    }
    return 0;
}

uint32 G3Function8004A854(uint32 bank, uint32 channel)
{
    FUNCTION_MARKER(0x8004a854, "SLUS_008.06");
    channel &= 0xffff;
    uint32 bend_value = r_u16(bank + (channel << 1) + 0x106);
    sint32 bend = (sint16)((bend_value - 0x2000) & 0xffff);
    uint32 voice = 0;
    do
    {
        uint32 audio = r_u32(0x80098ff0);
        uint32 entry = audio + voice * 28 + 0x248;
        uint32 group = r_u8(bank + 0xfd);
        uint32 owner = r_u8(entry + 8);
        if (owner == (group | channel))
        {
            uint32 tone = r_u32(entry + 0x14);
            uint32 range = r_u8(tone + 9);
            if (range != 0)
            {
                sint32 step = (sint32)(0x2000 / range);
                sint32 whole = bend / step;
                sint32 remainder = bend % step;
                uint32 note = r_u8(entry + 0x0a);
                uint32 base = r_u8(tone + 4);
                uint32 pitch_index = (uint32)(sint32)(sint16)((note + 0x3c - base + (uint32)whole) & 0xffff);
                sint32 slope = r_s16(0x800993f4 + (pitch_index << 1));
                sint32 interpolation_product = slope * remainder;
                uint32 tuning = r_u8(tone + 5);
                sint32 interpolation = interpolation_product / step;
                uint32 pitch;
                if ((tuning & 0x80) != 0)
                {
                    tuning = r_u8(tone + 5);
                    uint32 coefficient = r_u32(0x800992a0 + (pitch_index << 2));
                    uint32 product = (coefficient * 100) * ((0x100 - tuning) & 0xffff);
                    pitch = r_u16(0x800991f4 + (pitch_index << 1));
                    pitch -= product >> 23;
                }
                else
                {
                    uint32 coefficient = r_u32(0x800992a0 + (pitch_index << 2));
                    tuning = r_u8(tone + 5);
                    uint32 product = (coefficient * 100) * tuning;
                    pitch = r_u16(0x800991f4 + (pitch_index << 1));
                    pitch += product >> 23;
                }
                pitch += (uint32)interpolation;
                SpuSetVoicePitch((sint32)voice, (uint16)(pitch & 0xffff));
            }
        }
        ++voice;
    } while (voice < 0x18);
    return 0;
}

uint32 G3Function8004923C(uint32 sequence, uint32 output)
{
    FUNCTION_MARKER(0x8004923c, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 table = r_u32(audio + 0x54c);
    uint32 data = r_u32(table + ((sequence & 0xffff) << 2));
    uint32 value = r_u32(data + 4);
    w_u32(output, value);
    uint32 divisor = r_u16(data + 8);
    w_u16(output + 4, divisor);
    return output;
}

uint32 G3Function80049274(uint32 index, uint32 parameters)
{
    FUNCTION_MARKER(0x80049274, "SLUS_008.06");
    uint32 divisor = r_u16(parameters + 4);
    uint32 value = r_u32(parameters);
    uint32 whole = divisor != 0 ? value / divisor : 0xffffffff;
    uint32 remainder = divisor != 0 ? value % divisor : value;
    uint32 fraction = divisor != 0 ? (remainder << 16) / divisor : 0xffffffff;
    index &= 0xffff;
    uint32 audio = r_u32(0x80098ff0);
    uint32 bank = r_u32(audio + (index << 2) + 0x20);
    uint32 rate = (whole << 16) + fraction;
    w_u32(bank + 8, rate);
    uint32 mode = r_u8(audio + 5);
    uint32 clock = r_u32(0x80098fe0 + ((mode & 3) << 2));
    uint32 interval = rate != 0 ? clock / rate : 0xffffffff;
    w_u16(bank + 0x10, interval);
    mode = r_u8(audio + 5);
    rate = r_u32(bank + 8);
    clock = r_u32(0x80098fe0 + ((mode & 3) << 2));
    remainder = rate != 0 ? clock % rate : clock;
    w_u32(bank + 0x0c, remainder);
    value = r_u32(parameters);
    w_u32(bank + 0x14, value);
    divisor = r_u16(parameters + 4);
    w_u16(bank + 0x12, divisor);
    return 0;
}

uint32 G3Function80049650(uint32 index, uint32 output)
{
    FUNCTION_MARKER(0x80049650, "SLUS_008.06");
    index &= 0xffff;
    uint32 audio = r_u32(0x80098ff0);
    uint32 bank = r_u32(audio + (index << 2) + 0x20);
    uint32 value = r_u16(bank + 0xee);
    w_u16(output, value);
    value = r_u16(bank + 0xf2);
    w_u16(output + 4, value);
    value = r_u16(bank + 0xf6);
    w_u16(output + 6, value);
    value = r_u8(bank + 0xfa);
    w_u8(output + 8, value);
    value = r_u8(bank + 0xfb);
    w_u8(output + 9, value);
    value = r_u16(bank + 0xda);
    w_u16(output + 0x0a, value);
    return 0;
}

uint32 G3Function80049934(uint32 handle)
{
    FUNCTION_MARKER(0x80049934, "SLUS_008.06");
    G3Function8004C008(5, 0, 0, handle, 0, 0);
    return 0;
}

uint32 G3Function80049968(uint32 attributes)
{
    FUNCTION_MARKER(0x80049968, "SLUS_008.06");
    G3Function8004C008(6, 0, 0, attributes, 0, 0);
    return 0;
}

uint32 G3Function8004999C(uint32 enabled, uint32 mask)
{
    FUNCTION_MARKER(0x8004999c, "SLUS_008.06");
    uint32 command = enabled == 1 ? 7 : 8;
    G3Function8004C008(command, 0, 0, mask, 0, 0);
    return 0;
}

uint32 G3Function8004BBD8(uint32 handle)
{
    FUNCTION_MARKER(0x8004bbd8, "SLUS_008.06");
    G3Function8004C008(1, 0, 0, handle, 0, 0);
    return handle;
}

uint32 G3Function8004BC4C(uint32 handle)
{
    FUNCTION_MARKER(0x8004bc4c, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    uint32 voice = 0;
    do
    {
        uint32 entry = audio + voice * 28 + 0x248;
        uint32 owner = r_u8(entry + 8);
        if (owner == 0xd0)
        {
            uint32 state = r_u8(audio + voice + 0x4e8);
            if (state != 0 && state != 2)
            {
                uint32 current_handle = r_u32(entry + 4);
                if (current_handle == handle)
                    return 1;
            }
        }
        ++voice;
    } while (voice < 0x18);
    return 0;
}

uint32 G3Function8004BCD4(uint32 handle)
{
    FUNCTION_MARKER(0x8004bcd4, "SLUS_008.06");
    G3EnterCritical8008A05C();
    uint32 audio = r_u32(0x80098ff0);
    uint32 remaining = (uint32)(sint32)r_s16(audio + 0x242);
    uint32 head = r_u8(audio + 0x241);
    if (remaining != 0)
    {
        do
        {
            uint32 offset = (uint32)((sint32)(head << 16) >> 12);
            audio = r_u32(0x80098ff0);
            uint32 command = audio + offset + 0x40;
            uint32 kind = r_u8(command);
            if (kind == 0)
            {
                uint32 current_handle = r_u32(command + 4);
                if (current_handle == handle)
                {
                    G3ExitCritical8008A03C();
                    return 1;
                }
            }
            ++head;
            if ((sint16)(head & 0xffff) == 0x20)
                head = 0;
            --remaining;
        } while ((remaining & 0xffff) != 0);
    }
    G3ExitCritical8008A03C();
    return G3Function8004BC4C(handle) & 0xffff;
}

uint32 G3Function8004BD98(uint32 sound, uint32 *audio_source)
{
    FUNCTION_MARKER(0x8004bd98, "SLUS_008.06");
    uint32 audio = r_u32(0x80098ff0);
    if (audio_source != 0)
        *audio_source = audio;
    sound &= 0xffff;
    uint32 voice = 0;
    do
    {
        uint32 entry = audio + voice * 28 + 0x248;
        uint32 owner = r_u8(entry + 8);
        if (owner == 0xd0)
        {
            uint32 current_sound = r_u16(entry + 4);
            if (current_sound == sound)
            {
                uint32 state = r_u8(audio + voice + 0x4e8);
                if (state != 0 && state != 2)
                    return 1;
            }
        }
        ++voice;
    } while (voice < 0x18);
    return 0;
}

uint32 G3Function8004BE24(uint32 sound, uint32 *third_glyph)
{
    FUNCTION_MARKER(0x8004be24, "SLUS_008.06");
    G3EnterCritical8008A05C();
    uint32 audio = r_u32(0x80098ff0);
    uint32 remaining = (uint32)(sint32)r_s16(audio + 0x242);
    uint32 head = r_u8(audio + 0x241);
    if (remaining != 0)
    {
        do
        {
            uint32 offset = (uint32)((sint32)(head << 16) >> 12);
            audio = r_u32(0x80098ff0);
            uint32 command = audio + offset + 0x40;
            uint32 kind = r_u8(command);
            if (kind == 0)
            {
                uint32 current_sound = r_u16(command + 4);
                if (current_sound == (sound & 0xffff))
                {
                    G3ExitCritical8008A03C();
                    return 1;
                }
            }
            ++head;
            if ((sint16)(head & 0xffff) == 0x20)
                head = 0;
            --remaining;
        } while ((remaining & 0xffff) != 0);
    }
    G3ExitCritical8008A03C();
    return G3Function8004BD98(sound & 0xffff, third_glyph) & 0xffff;
}

uint32 G3Function8004BF74(uint32 handle, uint32 volume, uint32 pan, uint32 pitch)
{
    FUNCTION_MARKER(0x8004bf74, "SLUS_008.06");
    uint32 pitch_offset = (uint32)(sint32)(sint16)(pitch & 0xffff);
    G3Function8004C008(3, volume & 0xff, pan & 0xff, handle, 0, pitch_offset);
    return handle;
}

uint32 G3Function8004AB48(uint32 event, uint32 bank)
{
    FUNCTION_MARKER(0x8004ab48, "SLUS_008.06");
    uint32 channel = r_u8(event + 4);
    uint32 value = r_u8(event + 6);
    channel &= 0x0f;
    w_u8(bank + channel + 0x146, value);
    uint32 flags = r_u16(bank + 0xf8);
    if (((flags >> channel) & 1) != 0)
        return G3Function8004A184(bank, channel);
    return 0;
}

uint32 G3Function8004D2E0(uint32 tone, uint32 sample, uint32 instrument, uint32 note, uint32 gain, uint32 volume, uint32 pan, uint32 global_gain, uint32 master_gain, uint32 voice_entry, uint32 pitch_bend)
{
    FUNCTION_MARKER(0x8004d2e0, "SLUS_008.06");
    volume &= 0xffff;
    pan &= 0xffff;
    global_gain &= 0xffff;
    master_gain &= 0xffff;
    pitch_bend &= 0xffff;
    uint32 audio = r_u32(0x80098ff0);
    uint32 left = (gain * gain) & 0xffff;
    uint32 right = left;
    uint32 mode = r_u16(audio + 0x12);
    if ((mode & 1) == 0)
    {
        if (pan >= 0x41)
        {
            uint32 attenuation = (0x7f - pan) & 0xff;
            uint32 product = (uint32)(sint32)(sint16)left * attenuation;
            product *= attenuation;
            left = (uint32)((sint32)product >> 12) & 0xffff;
        }
        else if (pan < 0x3f)
        {
            uint32 product = (uint32)(sint32)(sint16)left * pan;
            product *= pan;
            right = (uint32)((sint32)product >> 12) & 0xffff;
        }
    }
    uint32 tone_gain = r_u8(tone + 2);
    uint32 squared = tone_gain * tone_gain;
    uint32 left_product = (uint32)(sint32)(sint16)left * squared;
    uint32 right_product = (uint32)(sint32)(sint16)right * squared;
    audio = r_u32(0x80098ff0);
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    mode = r_u16(audio + 0x12);
    if ((mode & 1) == 0)
    {
        uint32 tone_pan = r_u8(tone + 3);
        if (tone_pan >= 0x41)
        {
            uint32 attenuation = (0x7f - tone_pan) & 0xff;
            uint32 product = (uint32)(sint32)(sint16)right * attenuation;
            product *= attenuation;
            left = (uint32)((sint32)product >> 12) & 0xffff;
        }
        else if (tone_pan < 0x3f)
        {
            tone_pan = r_u8(tone + 3);
            uint32 product = (uint32)(sint32)(sint16)left * tone_pan;
            product *= tone_pan;
            right = (uint32)((sint32)product >> 12) & 0xffff;
        }
    }
    squared = volume * volume;
    left_product = (uint32)(sint32)(sint16)left * squared;
    right_product = (uint32)(sint32)(sint16)right * squared;
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    uint32 instrument_gain = r_u8(instrument + 4);
    squared = instrument_gain * instrument_gain;
    left_product = (uint32)(sint32)(sint16)left * squared;
    right_product = (uint32)(sint32)(sint16)right * squared;
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    squared = global_gain * global_gain;
    left_product = (uint32)(sint32)(sint16)left * squared;
    right_product = (uint32)(sint32)(sint16)right * squared;
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    squared = master_gain * master_gain;
    left_product = (uint32)(sint32)(sint16)left * squared;
    right_product = (uint32)(sint32)(sint16)right * squared;
    left = (left_product >> 14) & 0xffff;
    right = (right_product >> 14) & 0xffff;
    uint32 voice = r_u8(voice_entry + 0x10);
    SpuSetVoiceVolume((sint32)voice, (sint16)left, (sint16)right);
    uint32 range = r_u8(tone + 9);
    sint32 step = range != 0 ? (sint32)(0x2000 / range) : -1;
    sint32 bend = (sint16)((pitch_bend - 0x2000) & 0xffff);
    sint32 whole = bend / step;
    sint32 remainder = bend % step;
    uint32 base_note = r_u8(tone + 4);
    uint32 pitch_index = (uint32)(sint32)(sint16)((note + 0x3c - base_note + (uint32)whole) & 0xffff);
    sint32 slope = r_s16(0x800993f4 + (pitch_index << 1));
    sint32 interpolation_product = slope * remainder;
    uint32 tuning = r_u8(tone + 5);
    sint32 interpolation = interpolation_product / step;
    uint32 pitch;
    if ((tuning & 0x80) != 0)
    {
        tuning = r_u8(tone + 5);
        uint32 coefficient = r_u32(0x800992a0 + (pitch_index << 2));
        uint32 product = (coefficient * 100) * ((0x100 - tuning) & 0xffff);
        pitch = r_u16(0x800991f4 + (pitch_index << 1));
        pitch -= product >> 23;
    }
    else
    {
        uint32 coefficient = r_u32(0x800992a0 + (pitch_index << 2));
        tuning = r_u8(tone + 5);
        uint32 product = (coefficient * 100) * tuning;
        pitch = r_u16(0x800991f4 + (pitch_index << 1));
        pitch += product >> 23;
    }
    pitch += (uint32)interpolation;
    voice = r_u8(voice_entry + 0x10);
    SpuSetVoicePitch((sint32)voice, (uint16)(pitch & 0xffff));
    voice = r_u8(voice_entry + 0x10);
    SpuSetVoiceStartAddr((sint32)voice, sample);
    voice = r_u8(voice_entry + 0x10);
    uint32 adsr_1 = r_u16(tone + 0x0a);
    uint32 adsr_2 = r_u16(tone + 0x0c);
    G3Function8004CDE4(voice, adsr_1, adsr_2);
    uint32 effect = r_u8(tone + 8);
    audio = r_u32(0x80098ff0);
    uint32 bit = r_u32(voice_entry);
    uint32 effects = r_u32(audio + 0x538);
    if (effect == 4)
        w_u32(audio + 0x538, effects | bit);
    else
        w_u32(audio + 0x538, effects & ~bit);
    audio = r_u32(0x80098ff0);
    bit = r_u32(voice_entry);
    uint32 pending = r_u32(audio + 0x534);
    uint32 result = pending | bit;
    w_u32(audio + 0x534, result);
    return result;
}

uint32 G3Function80049DFC(uint32 event, uint32 bank)
{
    FUNCTION_MARKER(0x80049dfc, "SLUS_008.06");
    uint32 channel = r_u8(event + 4) & 0x0f;
    uint32 flags = r_u16(bank + 0xf6);
    if (((flags >> channel) & 1) != 0)
        return 0xff;
    uint32 channel_data = bank + channel;
    uint32 program = r_u8(channel_data + 0x126);
    if (program == 0xff)
        return 0xff;
    uint32 velocity = r_u8(event + 6);
    uint32 note = r_u8(event + 5);
    if (velocity != 0)
    {
        uint32 program_offset = r_u8(channel_data + 0x136);
        uint32 audio = r_u32(0x80098ff0);
        uint32 instruments = r_u32(audio + 0x540);
        uint32 tones = r_u32(audio + 0x544);
        uint32 instrument = instruments + ((program + program_offset) << 3);
        uint32 index = r_u16(instrument + 2);
        uint32 count = r_u16(instrument);
        if (index >= index + count)
            return 0;
        uint32 pitch_data = bank + (channel << 1);
        do
        {
            uint32 tone = tones + ((index & 0xffff) << 4);
            uint32 minimum = r_u8(tone + 6);
            if (note >= minimum)
            {
                uint32 maximum = r_u8(tone + 7);
                if (note <= maximum)
                {
                    uint32 priority = r_u8(tone);
                    uint32 bank_index = r_u8(bank + 0xf0);
                    priority = (priority - (channel - 0x0f) + (bank_index << 4)) & 0xffff;
                    uint32 entry = G3Function8004CD20(priority);
                    if (entry != 0)
                    {
                        audio = r_u32(0x80098ff0);
                        uint32 sample_index = (uint32)(sint32)r_s16(tone + 0x0e);
                        uint32 samples = r_u32(audio + 0x548);
                        uint32 range = r_u8(tone + 9);
                        uint32 sample = r_u32(samples + (sample_index << 2));
                        bool bent = false;
                        if (range != 0)
                        {
                            uint32 bend = r_u16(pitch_data + 0x106);
                            bent = bend != 0x2000;
                        }
                        velocity = r_u8(event + 6);
                        uint32 volume = r_u8(channel_data + 0x146);
                        uint32 pan = r_u8(channel_data + 0x156);
                        uint32 global_gain = r_u8(bank + 0xfa);
                        uint32 master = r_u32(bank + 0x100);
                        uint32 master_gain = r_u8(master);
                        if (bent)
                        {
                            uint32 bend = (uint32)(sint32)r_s16(pitch_data + 0x106);
                            G3Function8004D2E0(tone, sample, instrument, note, velocity, volume, pan, global_gain, master_gain, entry, bend);
                        }
                        else
                            G3Function8004CE8C(tone, sample, instrument, note, velocity, volume, pan, global_gain, master_gain, entry, 0);
                        w_u32(entry + 4, 0);
                        uint32 group = r_u8(bank + 0xfd);
                        w_u8(entry + 8, channel | group);
                        priority = r_u8(tone);
                        w_u8(entry + 0x0a, note);
                        w_u8(entry + 9, priority);
                        program = r_u8(channel_data + 0x126);
                        w_u8(entry + 0x0b, program);
                        velocity = r_u8(event + 6);
                        w_u8(entry + 0x0d, velocity);
                        volume = r_u8(channel_data + 0x146);
                        w_u8(entry + 0x0e, volume);
                        pan = r_u8(channel_data + 0x156);
                        w_u32(entry + 0x18, instrument);
                        w_u32(entry + 0x14, tone);
                        w_u8(entry + 0x0f, pan);
                    }
                }
            }
            ++index;
            uint32 first = r_u16(instrument + 2);
            count = r_u16(instrument);
            if ((index & 0xffff) >= first + count)
                return 0;
        } while (true);
    }
    uint32 voice = 0;
    do
    {
        uint32 audio = r_u32(0x80098ff0);
        uint32 entry = audio + voice * 28 + 0x248;
        uint32 group = r_u8(bank + 0xfd);
        uint32 owner = r_u8(entry + 8);
        if (owner == (group | channel))
        {
            uint32 current_note = r_u8(entry + 0x0a);
            if (current_note == note)
            {
                uint32 state = r_u8(audio + voice + 0x4e8);
                if (state != 0 && state != 2)
                {
                    uint32 released = r_u32(audio + 0x530);
                    uint32 bit = r_u32(entry);
                    w_u32(audio + 0x530, released | bit);
                    bit = r_u32(entry);
                    uint32 pending = r_u32(audio + 0x534);
                    w_u32(audio + 0x534, pending & ~bit);
                    w_u8(entry + 8, 0xff);
                }
            }
        }
        ++voice;
    } while (voice < 0x18);
    return 0;
}

uint32 G3Function8004A184(uint32 bank, uint32 channel)
{
    FUNCTION_MARKER(0x8004a184, "SLUS_008.06");
    channel &= 0xffff;
    uint32 voice = 0;
    do
    {
        uint32 audio = r_u32(0x80098ff0);
        uint32 entry = audio + (((voice << 3) - voice) << 2) + 0x248;
        uint32 bank_flags = r_u8(bank + 0xfd);
        uint32 channel_flags = r_u8(entry + 8);
        if (channel_flags == (bank_flags | channel))
        {
            uint32 gain = r_u8(entry + 0x0d);
            uint32 left = gain * gain;
            gain = r_u8(entry + 0x0d);
            uint32 right = gain * gain;
            uint32 mode = r_u16(audio + 0x12);
            if ((mode & 1) == 0)
            {
                uint32 pan = r_u8(bank + channel + 0x156);
                if (pan >= 0x41)
                {
                    uint32 attenuation = (0x7f - pan) & 0xff;
                    uint32 product = (uint32)(sint32)(sint16)(right & 0xffff) * attenuation;
                    product *= attenuation;
                    left = (uint32)((sint32)product >> 12) & 0xffff;
                }
                else if (pan < 0x3f)
                {
                    pan = r_u8(bank + channel + 0x156);
                    uint32 product = (uint32)(sint32)(sint16)(left & 0xffff) * pan;
                    product *= pan;
                    right = (uint32)((sint32)product >> 12) & 0xffff;
                }
            }
            uint32 tone = r_u32(entry + 0x14);
            gain = r_u8(tone + 2);
            uint32 squared = gain * gain;
            uint32 left_product = (uint32)(sint32)(sint16)(left & 0xffff) * squared;
            uint32 right_product = (uint32)(sint32)(sint16)(right & 0xffff) * squared;
            audio = r_u32(0x80098ff0);
            left = (left_product >> 14) & 0xffff;
            right = (right_product >> 14) & 0xffff;
            mode = r_u16(audio + 0x12);
            if ((mode & 1) == 0)
            {
                tone = r_u32(entry + 0x14);
                uint32 pan = r_u8(tone + 3);
                if (pan >= 0x41)
                {
                    uint32 attenuation = (0x7f - pan) & 0xff;
                    uint32 product = (uint32)(sint32)(sint16)(right & 0xffff) * attenuation;
                    product *= attenuation;
                    left = (uint32)((sint32)product >> 12) & 0xffff;
                }
                else if (pan < 0x3f)
                {
                    pan = r_u8(tone + 3);
                    uint32 product = (uint32)(sint32)(sint16)(left & 0xffff) * pan;
                    product *= pan;
                    right = (uint32)((sint32)product >> 12) & 0xffff;
                }
            }
            gain = r_u8(bank + channel + 0x146);
            squared = gain * gain;
            left_product = (uint32)(sint32)(sint16)(left & 0xffff) * squared;
            right_product = (uint32)(sint32)(sint16)(right & 0xffff) * squared;
            left = (left_product >> 14) & 0xffff;
            right = (right_product >> 14) & 0xffff;
            uint32 instrument = r_u32(entry + 0x18);
            gain = r_u8(instrument + 4);
            squared = gain * gain;
            left_product = (uint32)(sint32)(sint16)(left & 0xffff) * squared;
            right_product = (uint32)(sint32)(sint16)(right & 0xffff) * squared;
            left = (left_product >> 14) & 0xffff;
            right = (right_product >> 14) & 0xffff;
            gain = r_u8(bank + 0xfa);
            squared = gain * gain;
            left_product = (uint32)(sint32)(sint16)(left & 0xffff) * squared;
            right_product = (uint32)(sint32)(sint16)(right & 0xffff) * squared;
            left = (left_product >> 14) & 0xffff;
            right = (right_product >> 14) & 0xffff;
            uint32 master = r_u32(bank + 0x100);
            gain = r_u8(master);
            squared = gain * gain;
            left_product = (uint32)(sint32)(sint16)(left & 0xffff) * squared;
            right_product = (uint32)(sint32)(sint16)(right & 0xffff) * squared;
            left = (left_product >> 14) & 0xffff;
            right = (right_product >> 14) & 0xffff;
            SpuSetVoiceVolume((sint32)voice, (sint16)left, (sint16)right);
            gain = r_u8(bank + channel + 0x146);
            w_u8(entry + 0x0e, gain);
            uint32 pan = r_u8(bank + channel + 0x156);
            w_u8(entry + 0x0f, pan);
        }
        ++voice;
    } while (voice < 0x18);
    return 0;
}

uint32 G3Function800491AC(uint32 sequence, uint32 index, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800491ac, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    index &= 0xffff;
    uint32 audio = r_u32(0x80098ff0);
    uint32 bank = r_u32(audio + (index << 2) + 0x20);
    w_u16(bank + 0xee, sequence);
    G3Function80048FA4(bank);
    G3Function800496BC(index);
    uint32 interval = r_u16(bank + 0x10);
    if (interval == 0)
    {
        uint32 parameters = G3Function8004923C(sequence & 0xffff, frame + 0x10);
        G3Function80049274(index, parameters);
    }
    w_u16(bank + 0xf6, 0);
    w_u16(bank + 0xf8, 0);
    return 0;
}

uint32 G3Function8004ADD4(uint32 event, uint32 bank, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004add4, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 value = r_u8(event + 5) << 16;
    w_u32(frame + 0x10, value);
    uint32 part = r_u8(event + 6);
    value |= part << 8;
    w_u32(frame + 0x10, value);
    part = r_u8(event + 7);
    value |= part;
    w_u32(frame + 0x10, value);
    uint32 target = r_u32(bank + 0xe8);
    uint32 divisor = r_u16(target + 0x12);
    w_u16(frame + 0x14, divisor);
    uint32 index = r_u8(bank + 0xec);
    return G3Function80049274(index, frame + 0x10);
}

uint32 G3Function8004AE48(uint32 event, uint32 bank, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8004ae48, "SLUS_008.06");
    uint32 frame = guest_stack - 0x28;
    uint32 sequence = r_u8(event + 5);
    uint32 count = G3Function8004917C();
    if (sequence >= (count & 0xffff))
        return 0;
    G3Function8004923C(sequence, frame + 0x10);
    uint32 index = r_u8(bank + 0xec);
    return G3Function80049274(index, frame + 0x10);
}

uint32 G3Function8003ED08(uint32 sequence, uint32 index, uint32 guest_stack)
{
    FUNCTION_MARKER(0x8003ed08, "SLUS_008.06");
    uint32 game = r_u32(0x80097048);
    uint32 frame = guest_stack - 0x30;
    uint32 enabled = r_u8(game + 0x67df);
    if (enabled == 0)
        return 0;
    if ((sint32)index >= 2)
    {
        enabled = r_u8(game + 0x67dd);
        if (enabled == 0)
            return 0;
    }
    uint32 count = G3Function8004917C() & 0xffff;
    if ((sint32)sequence >= (sint32)count)
        sequence = 0;
    index &= 0xffff;
    G3Function80049650(index, frame + 0x10);
    uint32 flags = r_u16(frame + 0x14);
    if ((flags & 1) != 0)
    {
        uint32 current = r_u16(frame + 0x10);
        if (current == sequence)
            return current;
    }
    G3Function800491AC(sequence & 0xffff, index, frame);
    return G3Function8004933C(index);
}

void G3Function8004B670(void)
{
    FUNCTION_MARKER(0x8004b670, "SLUS_008.06");
}

uint32 G3Function800116DC(uint32 index)
{
    FUNCTION_MARKER(0x800116dc, "SLUS_008.06");
    w_u8(0x800961ac + (index << 4) + 0x0f, 1);
    return 1;
}

uint32 G3Function800116F8(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x800116f8, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    sint32 mode = r_s16(game + 0x6786);
    if (mode == 6)
    {
        uint32 buttons = r_u32(game + 0x98);
        if ((buttons & 0x0c00) != 0)
        {
            uint32 pressed = r_u16(game + 0x9c) & 0xf3ff;
            uint32 previous = r_u32(0x8009cf70);
            if (previous == 0 && pressed != 0)
            {
                uint32 index = 0;
                do
                {
                    uint32 entry = 0x800961ac + (index << 4);
                    uint32 sequence = r_u32(entry);
                    if (sequence != 0)
                    {
                        uint32 position = r_u8(entry + 0x0e);
                        uint32 expected = r_u16(sequence + (position << 1));
                        w_u8(entry + 0x0e, (expected & pressed) != 0 ? position + 1 : 0);
                        position = r_u8(entry + 0x0e);
                        sint32 length = r_s16(entry + 0x0c);
                        if ((sint32)position == length)
                            G3Function800116DC(index);
                    }
                    ++index;
                } while (index < 0x0f);
            }
            w_u32(0x8009cf70, (uint32)(sint32)(sint16)pressed);
        }
    }
    uint32 index = 0;
    do
    {
        uint32 entry = 0x800961ac + (index << 4);
        uint32 ready = r_u8(entry + 0x0f);
        if (ready != 0)
        {
            w_u8(entry + 0x0f, 0);
            w_u8(entry + 0x0e, 0);
            uint32 reset = 0;
            do
            {
                w_u8(0x800961ac + (reset << 4) + 0x0e, 0);
                ++reset;
            } while (reset < 0x0f);
            G3Function8003F1C4(0x802, 0x74, 0x40, 0x64);
            G3Function80011430(index, frame);
        }
        ++index;
    } while (index < 0x0f);
    return 0;
}

uint32 G3Function80011854(uint32 game, uint32 guest_stack)
{
    FUNCTION_MARKER(0x80011854, "SLUS_008.06");
    uint32 frame = guest_stack - 0x20;
    uint32 global_game = r_u32(0x80097048);
    uint32 flags = r_u32(global_game + 0x6834);
    uint32 controls;
    if ((flags & 0x10) == 0)
        controls = game + 0x70;
    else
    {
        controls = 0x800a0d28;
        flags = r_u32(global_game + 0x0e8);
        if ((flags & 0x40000) != 0)
            controls = game + 0x98;
    }
    sint32 mode = r_s16(game + 0x6786);
    if (mode == 0)
    {
        sint32 state = r_s16(game + 0x67ee);
        if (state == 1)
            G3Function80011F98();
    }
    else if (mode == 4)
    {
        uint32 value = r_u32(game + 0x682c);
        w_u32(0x80096000, value);
        G3Function80011AC4(game, frame);
    }
    else if (mode == 6)
        G3Function80012DC0(game, frame);
    else if (mode == 7)
        G3Function8001292C(game);
    flags = r_u32(game + 0x0e8);
    if ((flags & 0x4000) != 0)
    {
        uint32 pressed = r_u32(controls + 4);
        if ((pressed & 0x400) != 0)
            G3Function8001294C();
    }
    return G3Function800116F8(game, frame);
}
