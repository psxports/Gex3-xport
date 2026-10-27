#include "xport.h"
#include "psx.h"
#include "game_functions.h"

/* Attach the game sequencer to the common physical field timeline */
struct G3AudioFieldClock
{
    uint32 period;
    uint32 lines;
    uint32 phase;
    uint32 generation;
    bool active;
    bool polling;
};

static G3AudioFieldClock audio_field_clock;

void G3StartAudioFieldClock(uint32 period)
{
    audio_field_clock.active = false;
    ++audio_field_clock.generation;
    audio_field_clock.period = (period & 0xffff) != 0 ? period & 0xffff : 0x10000;
    audio_field_clock.lines = GetVideoMode() == MODE_PAL ? 314 : 263;
    uint32 field = (uint32)VSync(-1);
    audio_field_clock.phase = (uint32)((uint64)field * audio_field_clock.lines % audio_field_clock.period);
    audio_field_clock.active = true;
}

void G3StopAudioFieldClock(void)
{
    audio_field_clock.active = false;
    ++audio_field_clock.generation;
}

void G3OnAudioField(void)
{
    if (!audio_field_clock.active || audio_field_clock.polling || !psx_callbacks_allowed())
        return;
    audio_field_clock.polling = true;
    uint32 generation = audio_field_clock.generation;
    uint64 lines = audio_field_clock.phase + audio_field_clock.lines;
    uint64 ticks = lines / audio_field_clock.period;
    audio_field_clock.phase = (uint32)(lines % audio_field_clock.period);
    while (ticks != 0 && audio_field_clock.active && audio_field_clock.generation == generation)
    {
        --ticks;
        DeliverEvent(0xf2000001, 2);
    }
    audio_field_clock.polling = false;
}

