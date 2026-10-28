#ifndef G3_GAME_FUNCTIONS_H
#define G3_GAME_FUNCTIONS_H

#include "xport.h"

/* Guest addresses remain integer values at game boundaries */
uint32 G3ClearBss80019440(void);
sint32 G3MemPackInit80048130(void);
uint32 G3ConsolePut80094E58(uint32 character);
void G3ConsoleFlush80094F54(void);
uint32 G3CdClearPending80064DE8(void);
uint32 G3InitializeState8002DA58(uint32 state);
uint32 G3InitializeLevel8002DA7C(uint32 configuration, uint32 game, uint32 guest_stack);
uint32 G3LoadLevel80027348(uint32 root, uint32 level_name, uint32 game, uint32 guest_stack);
uint32 G3EventCallback8002EBE0(void);
uint32 G3MemPackPush8004829C(void);
uint32 G3MemPackPop80048320(void);
uint32 G3MemPackAllocate80048190(uint32 bytes);
uint32 G3Main8002EBE8(uint32 guest_stack);
void G3InitializeStatics8008AED8(void);
uint32 G3BigfileInitialize80064900(uint32 filename, uint32 guest_stack);
void G3Shutdown8002EDC0(uint32 guest_stack);
uint32 G3QueueRead800652C8(uint32 sector_offset, uint32 input, uint32 output, uint32 sectors, uint32 option_1c, uint32 option_18, uint32 option_20);
uint32 G3CdPump80064F44(void);
uint32 G3CdIntToPosition8008ACAC(uint32 sector, uint32 position);
uint32 G3DriverReset8008A06C(void);
uint32 G3CdDebug8008A7F0(uint32 level);
uint32 G3CdControl8008A86C(uint32 command, uint32 parameter, uint32 result);
/* SLUS_008.06:8008ADB0 uses CdPosToInt with a mapped CdlLOC */
/* SLUS_008.06:8008AC28 uses native CdGetSector with a bounded output span */
uint32 G3CdControlF8008A9A8(uint32 command, uint32 parameter);
uint32 G3CdChecksum80064EA8(uint32 request, uint32 sectors);
uint32 G3UnpackStream80065554(uint32 bytes, uint32 margin);
uint32 G3ReadRootCounter800895D4(uint32 counter);
uint32 G3ElapsedMicroseconds8003A3E4(uint32 timestamp);
void G3SetRandomState80027F9C(uint32 state);
uint32 G3PresentDisplay8002DAEC(void);
uint32 G3InitializeDisplay8002DB7C(uint32 width, uint32 height);
uint32 G3PutDrawEnv80086850(uint32 environment);
void G3DrawPrimitive80086784(uint32 primitive);
uint32 G3PutDispEnv80086910(uint32 environment);
uint32 G3SetDisplayMask80086314(uint32 enabled);
uint32 G3StateStartup8002E220(uint32 state, uint32 guest_stack);
uint32 G3InitializeClock8002DCC4(void);
uint32 G3Function80020A04(void);
uint32 G3LoadObject8002C760(uint32 filename, uint32 guest_stack);
void G3VBlankCallback(void);
void G3DrawCompleteCallback(void);
uint32 G3Function80028498(void);
uint32 G3Function8003EB30(uint32 guest_stack);
uint32 G3Function80035348(void);
uint32 G3Function8002E15C(uint32 game);
uint32 G3OnVSync8002DD34(void);
uint32 G3OnDrawComplete8002DDA8(void);
void G3OnRootCounter8001947C(void);
uint32 G3EnterCritical8008A05C(void);
void G3ExitCritical8008A03C(void);
uint32 G3SetRootCounter80089538(uint32 counter, uint32 target, uint32 mode);
uint32 G3StartRootCounter8008960C(uint32 counter);
uint32 G3ElapsedDrawTime8003A2F8(uint32 timestamp);
uint32 G3GetSaveContextSize800697D0(void);
uint32 G3GetMemoryCardSize8006898C(void);
uint32 G3InitializeSaveContext800697D8(uint32 context, uint32 game);
uint32 G3InitializeMemoryCard80068A7C(uint32 card, uint32 game);
uint32 G3GetSaveBufferSize80068B48(uint32 card);
void G3BindSaveBuffer80068B88(uint32 card, uint32 buffer);
uint32 G3GetCardCoreSize80066E98(void);
uint32 G3GetCardBufferSize800675D4(uint32 core);
uint32 G3Function80067070(uint32 core);
uint32 G3Function80067940(void);
uint32 G3Function80067C7C(void);
uint32 G3Function80067F64(void);
uint32 G3SaveDataSize8006B2F4(void);
uint32 G3ResetMemoryCard80068A60(uint32 card);
uint32 G3ConfigureCardCore80066EDC(uint32 core, uint32 title, uint32 serial, uint32 option4, uint32 option5, uint32 option6, uint32 palette, uint32 pixels, uint32 data_bytes, uint32 callback1, uint32 callback2, uint32 callback3, uint32 owner, uint32 option14);
uint32 G3FillMemory80086094(uint32 destination, uint32 value, uint32 bytes);
uint32 G3InitializeCardHeader80067948(uint32 header, uint32 title, uint32 serial, uint32 option4, uint32 option5, uint32 option6, uint32 palette, uint32 pixels);
uint32 G3InitializeCardData80067C84(uint32 data, uint32 bytes, uint32 callback1, uint32 callback2, uint32 callback3, uint32 owner);
uint32 G3InitializeCardIo80067F6C(uint32 io, uint32 callback, uint32 owner);
uint32 G3CardBlockCode80067924(uint32 blocks);
uint32 G3CopyString8008935C(uint32 destination, uint32 source);
uint32 G3EncodeCardTitle80068920(uint32 source, uint32 destination);
uint32 G3CardCharacter8006883C(uint32 character);
uint32 G3HeaderBytes80067A3C(uint32 header);
uint32 G3CardDataBytes80067D28(uint32 data);
uint32 G3WriteCardHeader80067A50(uint32 header, uint32 buffer);
uint32 G3CardChecksum80067C48(uint32 buffer, uint32 bytes);
uint32 G3ValidateCardData80067D34(uint32 data, uint32 buffer);
uint32 G3StateRun8002E370(uint32 state, uint32 incoming_second_glyph, uint32 incoming_third_glyph, uint32 guest_stack);
uint32 G3StateWarning8002E7EC(uint32 state, uint32 guest_stack);
uint32 G3Function80027B20(uint32 game, uint32 incoming_second_glyph, uint32 incoming_third_glyph, uint32 guest_stack);
uint32 G3Function80035AD4(uint32 guest_stack);
uint32 G3Function8002DE88(uint32 game, uint32 guest_stack);
uint32 G3Function8003EF1C(void);
uint32 G3Function8002DFD8(uint32 state, uint32 game);
uint32 G3LoadFile80064A7C(uint32 filename, uint32 guest_stack);
uint32 G3Function800656AC(uint32 data, uint32 bank, uint32 unused_a2, uint32 palette_x, uint32 guest_stack);
uint32 G3StateLoad8002E61C(uint32 state, uint32 guest_stack);
void G3Function8002E43C(uint32 guest_stack);
uint32 G3Function8002E59C(uint32 filename, uint32 guest_stack);
uint32 G3Function8003EBE0(uint32 filename, uint32 guest_stack);
uint32 G3Function80028544(uint32 guest_stack);
uint32 G3Function80023AC8(uint32 drm, uint32 vrm, uint32 game, uint32 guest_stack);
uint32 G3Function80020A44(uint32 object);
uint32 G3StateMovie8002E924(uint32 state, uint32 guest_stack);
uint32 G3FileBytes80064D38(uint32 filename);
uint32 G3RelocateModule800398BC(uint32 relocations, uint32 code);
uint32 G3FindArchiveFile800654AC(uint32 filename);
uint32 G3ArchiveSector80064D8C(uint32 filename);
uint32 G3ArchiveNameHash80065448(uint32 filename);
uint32 G3WaitArchiveRead8002C830(uint32 request, uint32 guest_stack);
uint32 G3MemPackRelease80048248(uint32 bytes);
uint32 G3Function8002D814(uint32 destination, uint32 source, uint32 bytes);
uint32 G3Function80065650(uint32 request);
uint32 G3Function80021A74(uint32 width);
uint32 G3Function80065D38(uint32 token);
struct G3TextMeasureOperands;
uint32 G3Function800234A0(uint32 text, uint32 position, uint32 incoming_second_glyph, uint32 incoming_third_glyph, uint32 guest_stack, G3TextMeasureOperands *operands);
uint32 G3Function800866D8(uint32 table, uint32 count);
uint32 G3Function8002D0D4(uint32 guest_stack);
uint32 G3Function80038F18(uint32 level, uint32 mode);
uint32 G3Function80023448(uint32 text, uint32 position, uint32 incoming_second_glyph, uint32 incoming_third_glyph, uint32 guest_stack, G3TextMeasureOperands *result_operands);
uint32 G3Function8002D588(uint32 incoming_second_glyph, uint32 incoming_third_glyph, uint32 guest_stack);
uint32 G3Function80022E00(uint32 guest_stack);
uint32 G3Function800194B8(uint32 value);
void G3Function800867E0(uint32 table);
uint32 G3Function80020C48(uint32 command, uint32 x, uint32 y, uint32 option, uint32 enabled);
void G3Function80022CAC(uint32 guest_stack);
uint32 G3Function80022BE4(uint32 guest_stack);
uint32 G3Function80022DEC(void);
uint32 G3Function80020D2C(uint32 entry, uint32 guest_stack);
uint32 G3Function800213A8(uint32 x, uint32 y, uint32 mode);
void G3Function8002B484(void);
uint32 G3Function80022408(uint32 character, uint32 x, uint32 y, uint32 color, uint32 guest_stack);
uint32 G3Function800210F4(uint32 character, uint32 x, uint32 y);
// Native measurement result for formatter operands
struct G3TextMeasureOperands
{
    uint32 first_glyph;
    uint32 second_glyph;
    uint32 third_glyph;
};

uint32 G3Function80022FD0(uint32 text, G3TextMeasureOperands *operands);
uint32 G3Function80022E30(uint32 x, uint32 y);
uint32 G3Function80022EB4(uint32 mode, uint32 character, uint32 default_width, G3TextMeasureOperands *operands);
uint32 G3Function80020E74(uint32 character);
uint32 G3Function80022E60(uint32 character, uint32 x_pointer, uint32 y_pointer, uint32 reset_x, uint32 line_height);
void G3Function80023030(uint32 text, uint32 mode, uint32 arguments, uint32 incoming_third_glyph, uint32 guest_stack, G3TextMeasureOperands *operands);
uint32 G3SdkSprintf80088A1C(uint32 destination, uint32 format, uint32 value);
uint32 G3Function80021AAC(void);
void G3Function80020CBC(uint32 text, uint32 argument_1, uint32 argument_2, uint32 argument_3, uint32 guest_stack, G3TextMeasureOperands *operands);
uint32 G3Function80020FCC(uint32 glyph, uint32 x, uint32 y);
uint32 G3Function8002F718(uint32 matrix);
uint32 G3Function8005B470(uint32 angle);
uint32 G3Function8005B590(uint32 angle);
void G3Function8005BD00(uint32 rotation, uint32 matrix);
uint32 G3Function80021ACC(uint32 primitives, uint32 buffer, uint32 object, uint32 order_table, uint32 guest_stack);
uint32 G3SdkPrint80085E84(uint32 format);
uint32 G3Function8005B494(uint32 angle, uint32 table);
uint32 G3Function8005B5B4(uint32 angle, uint32 table);
/* Native carrier for the original V0 and V1 results */
struct G3AnglePair
{
    uint32 cosine;
    uint32 sine;
};
G3AnglePair G3Function8005B6B0(uint32 angle);
G3AnglePair G3Function8005B6D4(uint32 angle, uint32 table);
void G3Function80020CF4(uint32 text, uint32 argument_1, uint32 argument_2, uint32 argument_3, uint32 guest_stack, G3TextMeasureOperands *operands);
uint32 G3Function80022E90(uint32 x, uint32 y);
uint32 G3Function80039010(uint32 level);
uint32 G3Function80028F84(uint32 game, uint32 incoming_second_glyph, uint32 incoming_third_glyph, uint32 guest_stack);
uint32 G3Function8002786C(uint32 game, uint32 name, uint32 value_pointer, uint32 *matched_entry);
uint32 G3Function8002D550(uint32 count);
uint32 G3Function8003F128(uint32 sound, G3TextMeasureOperands *operands);
uint32 G3Function8004BB6C(uint32 sound, uint32 volume, uint32 pan, uint32 option);
uint32 G3Function8004BFBC(uint32 sound);
void G3Function8004C008(uint32 command, uint32 volume, uint32 pan, uint32 handle, uint32 data, uint32 option);
uint32 G3Function8003EDE8(uint32 volume);
uint32 G3Function8003EE30(uint32 volume);
uint32 G3Function8003EE78(uint32 volume);
uint32 G3Function80048718(uint32 volume);
uint32 G3Function80048700(uint32 volume);
uint32 G3Function8004A504(uint32 bank);
uint32 G3Function80048344(uint32 configuration);
uint32 G3Function80048368(uint32 configuration, uint32 buffer, uint32 guest_stack);
uint32 G3Function800487A0(uint32 bank, uint32 selector);
void G3Function800485C8(uint32 callback, uint32 period);
uint32 G3Function80048648(uint32 guest_stack);
uint32 G3Function800486AC(uint32 volume);
uint32 G3Function800486E0(uint32 target, uint32 step, uint32 callback);
void G3Function8008C1E4(uint32 left, uint32 right);
uint32 G3Function80049BF8(uint32 output, uint32 bank);
uint32 G3Function8004CC40(uint32 command);
uint32 G3Function8004CCCC(uint32 command);
uint32 G3Function8004CCEC(uint32 command);
uint32 G3Function8004CDE4(uint32 voice, uint32 adsr_1, uint32 adsr_2);
uint32 G3Function8008C6D4(uint32 voice, uint32 attack, uint32 decay, uint32 sustain, uint32 release, uint32 level, uint32 attack_mode, uint32 sustain_mode, uint32 release_mode);
uint32 G3Function8004C2C0(uint32 command);
uint32 G3Function8004C380(void);
uint32 G3Function8004CD20(uint32 priority);
uint32 G3Function8004C154(uint32 command);
uint32 G3Function8004C42C(uint32 command);
uint32 G3Function8004C78C(uint32 command);
uint32 G3Function8004CBF4(uint32 command);
uint32 G3Function8004C108(uint32 command);
uint32 G3Function80049D50(uint32 event, uint32 bank, uint32 guest_stack);
uint32 G3Function8004AC38(uint32 event, uint32 bank);
uint32 G3Function800487DC(void);
uint32 G3Function80049040(void);
uint32 G3StopRootCounter8008963C(uint32 counter);
uint32 G3Function80049A18(uint32 paused);
uint32 G3Function8004BC18(void);
uint32 G3Function80049434(void);
uint32 G3Function80049144(void);
uint32 G3Function80049B20(void);
uint32 G3Function800493C0(uint32 index);
uint32 G3Function80048FA4(uint32 bank);
uint32 G3Function800496BC(uint32 index);
uint32 G3SdkConcat8008BFE8(uint32 destination, uint32 source);
uint32 G3Function80048DBC(uint32 data);
uint32 G3Function800490BC(void);
uint32 G3Function80048EF0(uint32 data);
uint32 G3Function80048D94(uint32 first, uint32 second, uint32 third, uint32 fourth);
uint32 G3Function80049060(void);
uint32 G3Function80049080(void);
/* SLUS_008.06:8008BEA8 uses SpuWrite with the original clamp and mapped source span */
uint32 G3Function8008C3A4(uint32 mode);
void G3SetReverbDepth8008C098(uint32 left, uint32 right);
uint32 G3Function8004AA54(uint32 event, uint32 bank, uint32 guest_stack);
uint32 G3Function8004AA94(uint32 event, uint32 bank);
uint32 G3Function8004AAB4(uint32 event, uint32 bank);
uint32 G3Function8004AD20(uint32 event, uint32 bank);
uint32 G3Function8004AD74(uint32 event, uint32 bank, uint32 guest_stack);
uint32 G3Function8004AEA0(uint32 event, uint32 bank);
uint32 G3Function8004AF40(uint32 event, uint32 bank);
uint32 G3Function8004B44C(uint32 event);
uint32 G3Function8004B4B8(uint32 event);
uint32 G3Function8004A854(uint32 bank, uint32 channel);
uint32 G3Function8004917C(void);
uint32 G3Function800491AC(uint32 sequence, uint32 index, uint32 guest_stack);
uint32 G3Function8004933C(uint32 index);
uint32 G3Function8004CE8C(uint32 tone, uint32 sample, uint32 instrument, uint32 note, uint32 gain, uint32 volume, uint32 pan, uint32 global_gain, uint32 master_gain, uint32 voice_entry, uint32 pitch_offset);
uint32 G3Function8002D03C(uint32 selection, uint32 count, uint32 guest_stack);
uint32 G3Function8002CF7C(uint32 first, uint32 second, uint32 y, uint32 order_table);
uint32 G3Function8002CF1C(uint32 source, uint32 destination, uint32 dimensions, uint32 order_table);

uint32 G3Function8004B678(uint32 event, uint32 bank);
uint32 G3Function8004B758(uint32 event, uint32 bank);
uint32 G3Function8004B938(uint32 event, uint32 bank);
uint32 G3Function8004B598(uint32 bank, uint32 label);

uint32 G3Function8004923C(uint32 sequence, uint32 output);

uint32 G3Function80049274(uint32 index, uint32 parameters);
uint32 G3Function80049650(uint32 index, uint32 output);
uint32 G3Function80049934(uint32 handle);
uint32 G3Function80049968(uint32 attributes);
uint32 G3Function8004999C(uint32 enabled, uint32 mask);

uint32 G3Function8004BBD8(uint32 handle);
uint32 G3Function8004BC4C(uint32 handle);
uint32 G3Function8004BCD4(uint32 handle);
uint32 G3Function8004BD98(uint32 sound, uint32 *audio_source);
uint32 G3Function8004BE24(uint32 sound, uint32 *third_glyph);
uint32 G3Function8004BF74(uint32 handle, uint32 volume, uint32 pan, uint32 pitch);

uint32 G3Function8004AB48(uint32 event, uint32 bank);
uint32 G3Function8004A184(uint32 bank, uint32 channel);
uint32 G3Function8004D2E0(uint32 tone, uint32 sample, uint32 instrument, uint32 note, uint32 gain, uint32 volume, uint32 pan, uint32 global_gain, uint32 master_gain, uint32 voice_entry, uint32 pitch_bend);

uint32 G3Function80049DFC(uint32 event, uint32 bank);

/* Guest SP is metadata, not a third original event argument */
uint32 G3DispatchAudioEvent(uint32 target, uint32 event, uint32 bank, uint32 guest_stack);
uint32 G3Function8004ADD4(uint32 event, uint32 bank, uint32 guest_stack);
uint32 G3Function8004AE48(uint32 event, uint32 bank, uint32 guest_stack);
uint32 G3Function8003ED08(uint32 sequence, uint32 index, uint32 guest_stack);

void G3Function8004B670(void);

uint32 G3Function800116DC(uint32 index);
uint32 G3Function800116F8(uint32 game, uint32 guest_stack);
uint32 G3Function80011854(uint32 game, uint32 guest_stack);
uint32 G3Function80011970(uint32 game, uint32 context, uint32 guest_stack);
void G3Function80011F98(void);
void G3Function8001294C(void);
void G3Function800126BC(void);
void G3Function800126C4(void);
void G3Function800126CC(void);
void G3Function800126D4(void);
void G3Function800126DC(void);
void G3Function800126EC(void);
void G3Function80012F94(void);
void G3Function80012F9C(void);
uint32 G3Function80011430(uint32 index, uint32 guest_stack);
uint32 G3Function8003F1C4(uint32 sound, uint32 volume, uint32 pan, uint32 option);
uint32 G3Function80011AC4(uint32 game, uint32 guest_stack);
void G3DispatchDebugMenuAction(uint32 target, uint32 selected_entry, uint32 first_glyph, uint32 second_glyph, uint32 third_glyph, uint32 guest_stack);
uint32 G3Function80012DC0(uint32 game, uint32 guest_stack);
uint32 G3Function8001292C(uint32 game);
uint32 G3Function80011FA0(uint32 game, uint32 guest_stack);

uint32 G3Function80018CC4(uint32 context, uint32 enabled, uint32 guest_stack);
uint32 G3Function80027584(uint32 state, uint32 guest_stack);
uint32 G3Function800277A4(uint32 game, uint32 name, uint32 enabled);
uint32 G3Function80047EE4(uint32 kind, uint32 value);
uint32 G3Function8002A454(uint32 object, uint32 data, uint32 guest_stack);
uint32 G3Function800290A0(uint32 pool, uint32 guest_stack);
uint32 G3Function8002A7EC(uint32 object, uint32 data, uint32 guest_stack);
uint32 G3Function800292F0(uint32 pool, uint32 object, uint32 guest_stack);
uint32 G3Function8002A748(uint32 object, uint32 guest_stack);
uint32 G3Function8002924C(uint32 pool, uint32 object, uint32 guest_stack);
uint32 G3Function8002910C(uint32 object, uint32 guest_stack);
uint32 G3Function80029294(uint32 pool, uint32 object);
uint32 G3Function80043720(uint32 object, uint32 configuration, uint32 game, uint32 first_output, uint32 guest_stack);
uint32 G3Function80061CA4(uint32 request, uint32 game, uint32 guest_stack);
uint32 G3Function80061A7C(uint32 request, uint32 object, uint32 guest_stack);
uint32 G3Function80061118(uint32 request, uint32 object, uint32 guest_stack);
uint32 G3Function80061EE0(uint32 end, uint32 start, uint32 collision, uint32 model, uint32 guest_stack);
uint32 G3Function8006218C(uint32 end, uint32 start, uint32 collision, uint32 normal, uint32 guest_stack);
uint32 G3Function8005F9F4(uint32 point, uint32 normal, uint32 alternate_point, uint32 alternate_normal, uint32 guest_stack);
uint32 G3DispatchNearestPoint(uint32 target, uint32 output, uint32 start, uint32 end, uint32 point, uint32 guest_stack);
uint32 G3DispatchLinePlane(uint32 target, uint32 output, uint32 start, uint32 end, uint32 normal, uint32 guest_stack);
void G3Function8005F8E8(sint32 start_coordinate, sint32 delta, sint32 plane, uint32 start, uint32 guest_stack);
uint32 G3Function8005F7D4(uint32 point, uint32 box);
uint32 G3Function8005F830(uint32 point, uint32 box);
uint32 G3Function8005F88C(uint32 point, uint32 box);
uint32 G3Function8005F694(uint32 point, uint32 normal, uint32 time, uint32 first_point, uint32 guest_stack);
uint32 G3DispatchBoxPlaneCheck(uint32 target, uint32 point, uint32 box, uint32 guest_stack);
uint32 G3Function8005F304(uint32 index, uint32 normals, uint32 output, uint32 guest_stack);
uint32 G3Function800415CC(uint32 game, uint32 guest_stack);
uint32 G3Function8004161C(uint32 game, uint32 guest_stack);
uint32 G3Function80045C0C(uint32 anchor, uint32 guest_stack);
uint32 G3Function80045BA0(uint32 node);
uint32 G3Function8003A9F8(uint32 object, uint32 guest_stack);

uint32 G3Function80049778(uint32 output);

uint32 G3Function800497EC(uint32 voice);

uint32 G3Function800499FC(uint32 index);

uint32 G3Function800126F4(void);

uint32 G3Function80012A14(void);

uint32 G3Function80013864(uint32 object, uint32 state, uint32 transition, uint32 guest_stack);

uint32 G3Function80013C40(uint32 value);

uint32 G3Function80013C54(uint32 object, uint32 guest_stack);

uint32 G3Function80013C94(uint32 object, uint32 state, uint32 guest_stack);

uint32 G3Function80013CDC(uint32 object, uint32 state, uint32 guest_stack);

/* Four original message arguments plus separate guest SP metadata */
uint32 G3DispatchStateEvent(uint32 target, uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3Function80053F98(uint32 object, uint32 sender, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3Function80053F40(uint32 state);
uint32 G3Function8004E2D0(uint32 transition);
void G3Function80015098(uint32 state);

uint32 G3Function80014900(void);

uint32 G3Function800140E4(uint32 object, uint32 index, uint32 value, uint32 guest_stack);

void G3Function80014288(uint32 object, uint32 direction);

uint32 G3Function80014298(uint32 object, uint32 value, uint32 guest_stack);

uint32 G3Function8005ADA0(uint32 matrix, uint32 input, uint32 output);

uint32 G3Function80014164(uint32 object);

uint32 G3Function8002F744(uint32 vector);

uint32 G3Function80013D38(uint32 object, uint32 guest_stack);
uint32 G3Function8001380C(uint32 object, uint32 width, uint32 guest_stack);
uint32 G3SdkRandom80027FA8(void);
uint32 G3Function800145D0(uint32 objects, uint32 count, uint32 guest_stack);
uint32 G3Function80023A68(uint32 game);
uint32 G3Function80012FA4(uint32 object, uint32 guest_stack);
uint32 G3Function8002C50C(uint32 level);
uint32 G3Function8001471C(uint32 game, uint32 guest_stack);
uint32 G3Function80038F6C(uint32 name, uint32 guest_stack);
uint32 G3StringLength8008929C(uint32 text);
uint32 G3Lowercase800894BC(uint32 character);
uint32 G3CompareString80085EC4(uint32 first, uint32 second);
uint32 G3Function800351EC(uint32 descriptor, uint32 data, uint32 base);
void G3Function80081C58(void);
uint32 G3Function800356E4(void);
uint32 G3Function80045B74(uint32 anchor, uint32 node);
uint32 G3Function80035250(uint32 request);
uint32 G3Function80065770(uint32 image, uint32 x, uint32 y, uint32 width, uint32 guest_stack);
uint32 G3LoadImage800865C0(uint32 rectangle, uint32 pixels);
uint32 G3Function80027168(uint32 incoming_v0);
uint32 G3Function80065674(uint32 filename, uint32 guest_stack);
uint32 G3Function8003A114(uint32 handle_pointer, uint32 incoming_v0);
uint32 G3Function8003F18C(uint32 handle);
uint32 G3Function80046AD0(void);
void G3Function8002C704(uint32 object, uint32 filename, uint32 guest_stack);
uint32 G3Function80039980(uint32 module);
void G3Function8001946C(uint32 zsf3, uint32 zsf4);
uint32 G3Function80030414(void);
uint32 G3Function8007633C(uint32 pool, uint32 count, uint32 element_bytes);
uint32 G3Function800763A4(uint32 pool);
uint32 G3Function80023850(uint32 game, uint32 level);
uint32 G3Function800752B8(void);
uint32 G3Function800752D8(void);
uint32 G3Function80075264(uint32 pool, uint32 stride, uint32 count);
uint32 G3Function800414B0(uint32 storage);
uint32 G3Function80029010(uint32 state, uint32 records);
uint32 G3Function80039A3C(uint32 level);
uint32 G3Function80039884(uint32 records);
uint32 G3Function8003ACC8(uint32 level, uint32 guest_stack);
uint32 G3Function8003AC34(void);
uint32 G3Function8003AADC(uint32 level, uint32 guest_stack);
uint32 G3Function8003AC68(uint32 level);
uint32 G3Function8003AA48(uint32 name);
uint32 G3Function80039B78(uint32 name);
uint32 G3Function800647A4(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3Function800647E4(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8007C5D8(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3Function8007C734(uint32 object);
uint32 G3Function8007C800(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8007CC8C(uint32 object, uint32 game);
uint32 G3Function8007C6B8(uint32 object, uint32 value);
uint32 G3Function8007C728(uint32 object);
uint32 G3Function8007C71C(uint32 object);
uint32 G3Function800740B0(uint32 object);
uint32 G3Function80039AA4(uint32 level, uint32 guest_stack);
uint32 G3Function80063BB4(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3Function80063C10(uint32 object);
uint32 G3Function80063C2C(uint32 object, uint32 guest_stack);
uint32 G3Function8002A9AC(uint32 object, uint32 guest_stack);
uint32 G3Function800292B4(uint32 state, uint32 object);
uint32 G3Function8003772C(uint32 object, uint32 guest_stack);
uint32 G3Function8003742C(uint32 object, uint32 guest_stack);
uint32 G3Function800659B0(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3Function80065A6C(uint32 object, uint32 guest_stack);
uint32 G3Function80065B18(uint32 object, uint32 game, uint32 guest_stack);
void G3Function80065D14(uint32 object, uint32 game, uint32 payload, uint32 guest_stack);
uint32 G3Function8006B8CC(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3Function8006BBF4(uint32 object, uint32 guest_stack);
uint32 G3Function80076450(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3Function8007657C(uint32 object, uint32 guest_stack);
uint32 G3Function8007662C(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function80076B64(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function800769D0(uint32 object, uint32 payload, uint32 guest_stack);
uint32 G3Function80076568(uint32 object);
uint32 G3Function8005A2C8(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3Function8005A308(uint32 object, uint32 guest_stack);

uint32 G3Function80045CAC(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3Function800193F8(uint32 mask, uint32 object, uint32 guest_stack);
uint32 G3Function8001932C(uint32 mask, uint32 object, uint32 guest_stack);
uint32 G3Function80018DE0(uint32 object, uint32 sequence, uint32 guest_stack);
uint32 G3Function80045E7C(uint32 game);
uint32 G3Function80045E2C(uint32 game);
uint32 G3Function80046514(uint32 object);
uint32 G3Function800463AC(uint32 object, uint32 guest_stack);
uint32 G3Function800462D4(uint32 game, uint32 value, uint32 guest_stack);
uint32 G3Function800465F8(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function80046458(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function80036D1C(uint32 object, uint32 upper, uint32 lower, uint32 guest_stack);
uint32 G3Function80046384(uint32 object);
uint32 G3Function8002AB10(uint32 records, uint32 count, uint32 guest_stack);
uint32 G3Function80065D54(uint32 object, uint32 game, uint32 message, uint32 guest_stack);
uint32 G3Function80066978(uint32 object, uint32 game, uint32 extension, uint32 descriptor, uint32 guest_stack);
uint32 G3Function80066AE8(uint32 object, uint32 game, uint32 extension, uint32 descriptor, uint32 guest_stack);
uint32 G3Function80066D68(uint32 object, uint32 game, uint32 extension, uint32 descriptor, uint32 guest_stack);
uint32 G3Function80066958(uint32 object, uint32 extension, uint32 guest_stack);
uint32 G3Function8006688C(uint32 object, uint32 extension, uint32 guest_stack);
uint32 G3Function80035E80(uint32 game);
uint32 G3Function80035E14(uint32 name);
uint32 G3Function80035AC8(void);
uint32 G3Function800810FC(void);
uint32 G3Function80081154(void);
uint32 G3Function8008095C(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3Function800809E4(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function80080A58(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function80080FC8(uint32 object);
uint32 G3Function8003E518(uint32 object, uint32 sound, uint32 pitch, uint32 volume, uint32 guest_stack);
uint32 G3Function800751B4(uint32 guest_stack);
uint32 G3Function80074E08(uint32 object);
uint32 G3Function80074BD0(uint32 object, uint32 state);
uint32 G3Function80074B6C(uint32 state);
uint32 G3Function80074F64(uint32 object, uint32 state, uint32 guest_stack);
uint32 G3Function80074F28(uint32 object, uint32 guest_stack);
uint32 G3Function80030454(uint32 object, uint32 guest_stack);
uint32 G3Function80043E70(uint32 object, uint32 use_first);
uint32 G3Function80030AEC(uint32 records, uint32 count);
void G3Function80030900(uint32 object, uint32 model);
void G3Function80030954(uint32 object, uint32 count);
uint32 G3Function80030710(uint32 object, uint32 model, uint32 descriptor, uint32 guest_stack);
uint32 G3Function800763E0(uint32 pool);
uint32 G3Function80030A2C(uint32 data, uint32 bit_offset, uint32 reader);
uint32 G3Function80030A64(uint32 reader);
uint32 G3Function80030F94(uint32 reader, uint32 destination, uint32 guest_stack);
uint32 G3Function8006D9D8(uint32 guest_stack);
uint32 G3Function8006DB98(void);
uint32 G3Function8006DC34(void);
uint32 G3Function8006D93C(uint32 type, uint32 model);
uint32 G3Function80030E9C(uint32 reader, uint32 destination);
uint32 G3Function80030EC8(uint32 reader, uint32 destination);
uint32 G3Function80030F00(uint32 reader, uint32 destination);
uint32 G3Function80030F58(uint32 reader, uint32 destination);
uint32 G3DispatchModelDecoder(uint32 target, uint32 reader, uint32 destination, uint32 selector, uint32 guest_stack);
uint32 G3Function80030BDC(uint32 reader);
uint32 G3Function80030C10(uint32 reader);
uint32 G3Function80030C2C(uint32 reader);
uint32 G3Function80030D8C(uint32 reader);
uint32 G3Function8004D8B8(uint32 object, uint32 settings, uint32 guest_stack);
uint32 G3Function80056064(void);
uint32 G3Function80056044(uint32 enabled);
uint32 G3Function80058298(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3Function8005624C(uint32 object, uint32 guest_stack);
uint32 G3Function800564A4(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function80056894(uint32 object, uint32 game, uint32 payload, uint32 guest_stack);
uint32 G3Function80056924(void);
uint32 G3Function80056934(uint32 payload, uint32 guest_stack);
uint32 G3Function800772D8(uint32 object, uint32 angle);
uint32 G3Function80052B10(uint32 mode, uint32 origin, uint32 output, uint32 guest_stack);
uint32 G3Function80052514(uint32 origin, uint32 radius, uint32 angle, uint32 output, uint32 guest_stack);
uint32 G3Function8002F75C(uint32 vector, uint32 angle);
uint32 G3Function800560CC(uint32 source);
uint32 G3Function800571F8(uint32 angle, uint32 distance);
uint32 G3Function80058798(uint32 duration);
uint32 G3Function8005617C(uint32 object, uint32 game, uint32 payload, uint32 guest_stack);
uint32 G3Function80059E70(uint32 object, uint32 state, uint32 guest_stack);
uint32 G3Function80057A34(uint32 object, uint32 game, uint32 payload, uint32 guest_stack);
uint32 G3Function8006238C(uint32 world, uint32 end, uint32 start, uint32 direction, uint32 guest_stack);
uint32 G3Function8005798C(uint32 object, uint32 guest_stack);
uint32 G3Function800524E4(void);
uint32 G3Function80050C5C(uint32 vector, uint32 guest_stack);
uint32 G3Function80050B94(uint32 vector, uint32 guest_stack);
uint32 G3Function800560FC(uint32 vector, uint32 guest_stack);
uint32 G3Function8002F8E4(uint32 vector, uint32 angle);
uint32 G3Function8005664C(uint32 output, uint32 state, uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function80056A74(uint32 state, uint32 guest_stack);
uint32 G3Function8005A1D8(uint32 output, uint32 first, uint32 second, uint32 guest_stack);
uint32 G3Function800144CC(uint32 output, uint32 first, uint32 second, uint32 guest_stack);
uint32 G3Function8002FA50(uint32 vector, uint32 guest_stack);
uint32 G3Function8002F448(uint32 value, uint32 scale, uint32 guest_stack);
uint32 G3Function80056464(void);
uint32 G3Function80055544(uint32 guest_stack);
uint32 G3Function800563A0(uint32 object, uint32 guest_stack);
uint32 G3Function8005A248(uint32 object, uint32 packed);
uint32 G3Function80016F00(void);
uint32 G3PrintCameraDiagnostic(uint32 rotation, uint32 tilt, uint32 relative_x, uint32 guest_stack);
uint32 G3Function80050CE4(uint32 vector, uint32 guest_stack);
uint32 G3Function80058B54(void);
uint32 G3Function80057284(uint32 output, uint32 state, uint32 object, uint32 guest_stack);
uint32 G3Function800587E0(uint32 position, uint32 state, uint32 delta, uint32 guest_stack);
uint32 G3Function8002F9A8(uint32 output, uint32 length, uint32 vector, uint32 guest_stack);
uint32 G3Function80064680(uint32 target, uint32 current, uint32 limit, uint32 guest_stack);
uint32 G3Function80064638(uint32 target, uint32 current, uint32 limit, uint32 increment, uint32 guest_stack);
uint32 G3Function8006459C(uint32 target, uint32 current, uint32 limit);
uint32 G3Function8005851C(uint32 object, uint32 state, uint32 delta, uint32 guest_stack);
uint32 G3Function80058680(uint32 object, uint32 state, uint32 delta, uint32 guest_stack);
uint32 G3Function80053C78(uint32 position, uint32 object, uint32 state, uint32 delta, uint32 guest_stack);
uint32 G3Function80058F48(uint32 position, uint32 state, uint32 delta, uint32 guest_stack);
uint32 G3Function80059624(uint32 position, uint32 state, uint32 delta, uint32 guest_stack);
uint32 G3Function80059740(uint32 position, uint32 state, uint32 delta, uint32 scale, uint32 guest_stack);
uint32 G3Function8002FEE0(uint32 vector, uint32 guest_stack);
uint32 G3Function80059AF8(uint32 position, uint32 state, uint32 delta, uint32 guest_stack);
uint32 G3Function800599E0(uint32 position, uint32 state, uint32 delta, uint32 guest_stack);
uint32 G3Function80058D50(uint32 position, uint32 state, uint32 guest_stack);
uint32 G3Function80058BA4(uint32 vector, uint32 position, uint32 state, uint32 guest_stack);
uint32 G3Function80058ED0(uint32 state, uint32 guest_stack);
uint32 G3Function80078834(uint32 object, uint32 guest_stack);
uint32 G3Function800536AC(uint32 position, uint32 state, uint32 bounds, uint32 delta, uint32 guest_stack);
uint32 G3Function80059200(uint32 position, uint32 state, uint32 delta, uint32 guest_stack);
uint32 G3Function80059B18(uint32 position, uint32 state, uint32 delta, uint32 guest_stack);
uint32 G3Function80057040(uint32 object, uint32 state, uint32 position, uint32 delta, uint32 guest_stack);
uint32 G3Function80059D94(uint32 position, uint32 object, uint32 delta, uint32 guest_stack);
uint32 G3Function80014F38(uint32 position, uint32 delta, uint32 guest_stack);
uint32 G3Function800594EC(uint32 angle, uint32 guest_stack);
uint32 G3Function8005A088(uint32 output, uint32 target, uint32 angle, uint32 tilt, uint32 guest_stack);
uint32 G3Function80059FD8(uint32 vector, uint32 tilt, uint32 guest_stack);
uint32 G3Function8002F368(uint32 value, uint32 guest_stack);
uint32 G3Function80038B48(uint32 first, uint32 second);
uint32 G3Function8002F240(uint32 y, uint32 x, uint32 guest_stack);

uint32 G3Function8005F3A8(uint32 object, uint32 delta);
uint32 G3Function8006DD7C(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8006F468(uint32 game, uint32 guest_stack);
uint32 G3Function80073B9C(uint32 game, uint32 magnitude, uint32 face, uint32 guest_stack);
uint32 G3Function8007408C(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function80073CE8(uint32 game, uint32 magnitude, uint32 face, uint32 guest_stack);
uint32 G3Function80073E44(uint32 object, uint32 game, uint32 mode, uint32 force, uint32 guest_stack);
uint32 G3Function8005F47C(uint32 object, uint32 delta, uint32 incoming_v0);
uint32 G3Function8006C610(uint32 game);
uint32 G3Function8007446C(uint32 game);
uint32 G3Function8007835C(uint32 object, uint32 position, uint32 game, uint32 guest_stack);
uint32 G3Function80075218(uint32 object, uint32 position, uint32 game, uint32 guest_stack);

uint32 G3Function800786F0(uint32 object);
uint32 G3Function8006F358(uint32 state, uint32 payload, uint32 flag);
uint32 G3Function80077648(uint32 vector);
uint32 G3Function8007801C(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3Function800779B0(uint32 game, uint32 guest_stack);
void G3Function8006E1B0(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function800734C8(uint32 object, uint32 guest_stack);
uint32 G3Function80072FB0(uint32 object, uint32 parameters, uint32 guest_stack);
void G3Function80045804(uint32 position, uint32 parameters, uint32 guest_stack);
uint32 G3Function8004257C(uint32 descriptor, uint32 data, uint32 position, uint32 velocity, uint32 guest_stack);
uint32 G3Function80045288(uint32 position, uint32 velocity, uint32 data, uint32 angle, uint32 guest_stack);
uint32 G3Function80070B54(uint32 game, uint32 animations, uint32 guest_stack);
uint32 G3Function800727A4(uint32 object, uint32 game, uint32 animations, uint32 guest_stack);
uint32 G3Function8007246C(uint32 object, uint32 game, uint32 state, uint32 input, uint32 guest_stack);
uint32 G3Function8007225C(uint32 output);
uint32 G3Function8006EA4C(uint32 object, uint32 game, uint32 state, uint32 height, uint32 guest_stack);
uint32 G3Function8006E6F4(uint32 object, uint32 contact, uint32 guest_stack);
uint32 G3Function8005ACE4(uint32 output, uint32 first, uint32 second, uint32 magnitude, uint32 guest_stack);
uint32 G3Function80044320(uint32 object, uint32 pool, uint32 guest_stack);
void G3Function8004D8A0(uint32 unused_camera, uint32 mode);
uint32 G3Function80073854(uint32 object, uint32 player, uint32 state, uint32 mode, uint32 guest_stack);
uint32 G3Function80073D44(uint32 game, uint32 guest_stack);
uint32 G3Function80074380(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 guest_stack);
uint32 G3Function800777C0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 guest_stack);
uint32 G3Function8006DFB8(uint32 object, uint32 game, uint32 result_address, uint32 argument3, uint32 guest_stack);
uint32 G3Function8006F50C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 guest_stack);
uint32 G3Function80076CF8(uint32 angle);
uint32 G3Function80076D3C(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 guest_stack);
uint32 G3Function800770A0(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 guest_stack);
void G3Function8007DE64(uint32 object, uint32 game, uint32 output, uint32 argument_3, uint32 guest_stack);
uint32 G3Function80080544(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 guest_stack);
void G3Function800714E0(uint32 object, uint32 output, uint32 argument2, uint32 argument3, uint32 guest_stack);
uint32 G3Function800788CC(uint32 a0, uint32 a1, uint32 a2, uint32 a3, uint32 guest_stack);

uint32 G3Function8003ACFC(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3Function8003B120(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8003B48C(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8004DABC(uint32 camera, uint32 guest_stack);
uint32 G3Function8004DB2C(uint32 camera, uint32 guest_stack);
uint32 G3Function8003B29C(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function80052060(uint32 camera, uint32 delay, uint32 guest_stack);
uint32 G3Function800520CC(void);
uint32 G3Function8003CE80(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function80083424(uint32 object, uint32 guest_stack);
uint32 G3Function8003C8C0(uint32 object, uint32 state, uint32 game, uint32 guest_stack);
uint32 G3Function8003FAE4(uint32 state, uint32 links, uint32 guest_stack);
uint32 G3Function8003FA8C(uint32 path, uint32 state);
uint32 G3Function80082DE4(uint32 object, uint32 state, uint32 direction, uint32 guest_stack);
uint32 G3Function80082B34(uint32 object, uint32 links, uint32 state, uint32 direction, uint32 guest_stack);
uint32 G3Function80082C10(uint32 object, uint32 links, uint32 state, uint32 direction, uint32 guest_stack);
uint32 G3Function80082D30(uint32 object, uint32 links, uint32 state, uint32 direction, uint32 guest_stack);
uint32 G3Function80085AB0(uint32 commands, uint32 name, uint32 guest_stack);
uint32 G3Function8003C614(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8003B0D0(uint32 object, uint32 guest_stack);
uint32 G3Function80029F0C(uint32 object, uint32 selector, uint32 guest_stack);
uint32 G3Function8002FBF8(uint32 vector, uint32 guest_stack);
uint32 G3Function8003E314(uint32 object, uint32 guest_stack);
uint32 G3Function8003BA38(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8003B8A8(uint32 object, uint32 game, uint32 payload, uint32 guest_stack);
uint32 G3Function8003CD64(uint32 payload, uint32 incoming_v0);
void G3Function8003C4F8(uint32 object, uint32 game, uint32 payload, uint32 guest_stack);
uint32 G3Function8003B9A4(uint32 object, uint32 guest_stack);
uint32 G3Function80082648(uint32 object, uint32 selector, uint32 guest_stack);
uint32 G3Function8003C520(uint32 object, uint32 value, uint32 guest_stack);
uint32 G3Function8003CCE8(uint32 object, uint32 game, uint32 payload, uint32 guest_stack);

uint32 G3Function8008321C(uint32 object, uint32 selector, uint32 state, uint32 guest_stack);
uint32 G3Function8008345C(uint32 object, uint32 linked_output, uint32 model_output);

uint32 G3DispatchObjectEvent(uint32 target, uint32 object, uint32 game, uint32 message, uint32 payload, uint32 phase, uint32 guest_stack);

uint32 G3Function80083578(uint32 object);
void G3Function8003BA9C(uint32 object, uint32 commands, uint32 guest_stack);
uint32 G3Function8003B030(uint32 object, uint32 selector);
uint32 G3Function800370E8(uint32 object, uint32 mask, uint32 guest_stack);
uint32 G3Function80037150(uint32 object, uint32 mask, uint32 guest_stack);
uint32 G3Function80085B84(uint32 cursor, uint32 guest_stack);
uint32 G3Function80082988(uint32 object, uint32 state, uint32 game, uint32 guest_stack);
uint32 G3Function8003CDA8(uint32 object, uint32 game, uint32 incoming_v0, uint32 guest_stack);

uint32 G3Function8008354C(uint32 object, uint32 linked_source, uint32 model_source);
uint32 G3Function8003F93C(uint32 channel, uint32 state, uint32 key, uint32 guest_stack);
uint32 G3Function80041008(uint32 channel, uint32 state, uint32 output, uint32 guest_stack);
uint32 G3Function80083038(uint32 object, uint32 vector, uint32 incoming_v0, uint32 guest_stack);
uint32 G3Function8003FC34(uint32 path, uint32 state, uint32 guest_stack);
uint32 G3Function8003FC94(uint32 path, uint32 state, uint32 guest_stack);
uint32 G3Function80041230(uint32 path, uint32 state, uint32 guest_stack);
uint32 G3Function8004136C(uint32 path, uint32 state, uint32 guest_stack);
uint32 G3Function8003F4A0(uint32 path, uint32 state, uint32 mode, uint32 guest_stack, uint32 *converted_product);
uint32 G3Function800828D4(uint32 object, uint32 incoming_v0, uint32 guest_stack);
uint32 G3Function800622EC(uint32 terrain, uint32 start, uint32 end, uint32 object, uint32 guest_stack);
uint32 G3Function800622C0(uint32 object, uint32 guest_stack);
uint32 G3Function80063594(uint32 object);
uint32 G3Function800635FC(uint32 context, uint32 level, uint32 guest_stack);
uint32 G3Function80063664(uint32 context, uint32 level, uint32 flags, uint32 argument3, uint32 guest_stack);
uint32 G3Function800634FC(uint32 position, uint32 radius, uint32 bounds);
uint32 G3Function80062C4C(uint32 current, uint32 previous, uint32 geometry, uint32 output, uint32 guest_stack);
uint32 G3Function80062BEC(uint32 vector, uint32 guest_stack);
uint32 G3Function8006340C(uint32 current, uint32 previous, uint32 geometry, uint32 output, uint32 guest_stack);
uint32 G3Function80063434(uint32 output, uint32 normal, uint32 distance, uint32 matrix);
uint32 G3Function80060044(void);
uint32 G3Function80060074(uint32 object, uint32 game, uint32 message, uint32 contact, uint32 guest_stack);
void G3Function80060EA8(uint32 output, uint32 position, uint32 radius, uint32 guest_stack);
uint32 G3Function8006083C(uint32 output, uint32 position, uint32 radius, uint32 argument3, uint32 guest_stack);
uint32 G3Function80060C8C(uint32 output, uint32 position, uint32 radius, uint32 argument3, uint32 guest_stack);
uint32 G3Function80060B88(uint32 first, uint32 second, uint32 angle, uint32 guest_stack);
uint32 G3Function80060188(uint32 index, uint32 edge, uint32 records, uint32 count, uint32 guest_stack);
uint32 G3Function800602E0(uint32 first, uint32 second, uint32 guest_stack);
uint32 G3Function80060280(uint32 first, uint32 second);
uint32 G3Function80060468(uint32 position, uint32 radius, uint32 face, uint32 output, uint32 guest_stack);
uint32 G3Function800607C8(uint32 position, uint32 records, uint32 count, uint32 radius, uint32 guest_stack);
uint32 G3Function80060684(uint32 position, uint32 records, uint32 count, uint32 radius, uint32 guest_stack);
uint32 G3Function80062358(uint32 terrain, uint32 start, uint32 end, uint32 object, uint32 guest_stack);
uint32 G3Function8005F518(uint32 output, uint32 start, uint32 end, uint32 normal, uint32 guest_stack);
uint32 G3Function8005F164(uint32 first, uint32 second, uint32 third, uint32 point, uint32 guest_stack);
uint32 G3Function8005F008(uint32 triangle, sint32 x, sint32 y);

uint32 G3Function80020714(uint32 alpha, uint32 first, uint32 second, uint32 output, uint32 guest_stack);
uint32 G3Function8003F290(uint32 output, uint32 alpha, uint32 first, uint32 second, uint32 guest_stack);
uint32 G3Function80020258(uint32 input, uint32 output, uint32 order, uint32 guest_stack);

uint32 G3Function80020608(uint32 matrix, uint32 quaternion, uint32 guest_stack);
uint32 G3Function800202A4(uint32 matrix, uint32 output, uint32 order, uint32 guest_stack);

uint32 G3Function8003C864(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8003D270(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function800313E0(uint32 output, uint32 matrix, uint32 guest_stack);
uint32 G3Function800314D0(uint32 output, uint32 matrix, uint32 guest_stack);
uint32 G3Function80031474(uint32 matrix, uint32 column, uint32 guest_stack);
uint32 G3Function800436F0(uint32 node, uint32 game, uint32 guest_stack);
uint32 G3Function80041578(uint32 node, uint32 game, uint32 guest_stack);
uint32 G3Function8003C6E0(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8003C548(uint32 contact, uint32 first_type, uint32 second_type);
uint32 G3Function8003CBC0(uint32 object, uint32 state, uint32 payload, uint32 guest_stack);
uint32 G3Function80038798(uint32 object, uint32 selector, uint32 random_range, uint32 guest_stack);
uint32 G3Function800376A4(uint32 object, uint32 guest_stack);
uint32 G3Function8003742C(uint32 object, uint32 guest_stack);
uint32 G3Function800374A4(uint32 object, uint32 guest_stack);
uint32 G3Function8003763C(uint32 object, uint32 guest_stack);
uint32 G3Function8003D390(uint32 object, uint32 guest_stack);
uint32 G3DispatchObjectEffect(uint32 target, uint32 object, uint32 effect, uint32 guest_stack);
uint32 G3Function800392A0(uint32 object, uint32 guest_stack);
uint32 G3Function800374FC(uint32 object, uint32 incoming_v0, uint32 guest_stack);
uint32 G3Function800460E8(uint32 object, uint32 x, uint32 y, uint32 z, uint32 guest_stack);
uint32 G3Function80046064(uint32 object);
uint32 G3Function800376D8(uint32 object, uint32 guest_stack);
uint32 G3Function80038318(uint32 object, uint32 selector, uint32 guest_stack);
uint32 G3Function80082854(uint32 object, uint32 guest_stack);
uint32 G3Function80037EAC(uint32 object, uint32 guest_stack);
uint32 G3Function80081A90(uint32 kind, uint32 mode, uint32 index, uint32 strength, uint32 guest_stack);
uint32 G3Function8008194C(uint32 kind, uint32 mode, uint32 index, uint32 strength, uint32 guest_stack);
uint32 G3Function80081754(uint32 kind, uint32 index, uint32 allocate, uint32 guest_stack);
uint32 G3Function8004197C(uint32 effect, uint32 pool, uint32 guest_stack);
uint32 G3Function80041DE0(uint32 effect, uint32 pool, uint32 guest_stack);
uint32 G3Function80041880(uint32 effect, uint32 pool, uint32 guest_stack);
uint32 G3Function80044B60(uint32 effect, uint32 guest_stack);
uint32 G3Function800444E4(uint32 position, uint32 resource, uint32 pool, uint32 count, uint32 guest_stack);
uint32 G3Function80044688(uint32 position, uint32 resource, uint32 pool, uint32 count, uint32 guest_stack);
uint32 G3Function80041664(uint32 effect, uint32 face, uint32 resource, uint32 pool, uint32 guest_stack);
uint32 G3Function80041CE4(uint32 output, uint32 transform, uint32 position, uint32 mode, uint32 guest_stack);
uint32 G3Function8002F0C4(uint32 y, uint32 x, uint32 guest_stack);
uint32 G3Function8005BA5C(uint32 angle, uint32 matrix, uint32 guest_stack);
uint32 G3Function8005B94C(uint32 angle, uint32 matrix, uint32 guest_stack);
uint32 G3Function8005BB6C(uint32 angle, uint32 matrix, uint32 guest_stack);
uint32 G3Function80037D10(uint32 object, uint32 guest_stack);
uint32 G3Function8003778C(uint32 object, uint32 callback, uint32 flags, uint32 selector, uint32 guest_stack);
uint32 G3Function80037F5C(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function80037094(uint32 object);
uint32 G3Function80039BF4(uint32 object, uint32 callback, uint32 guest_stack);
uint32 G3Function80037C6C(uint32 object, uint32 guest_stack);
uint32 G3Function80037C08(uint32 object, uint32 guest_stack);
uint32 G3Function800378B8(uint32 object, uint32 angle);
uint32 G3Function80037824(uint32 object, uint32 strength, uint32 radius);
uint32 G3Function80037CB8(uint32 variant, uint32 guest_stack);
uint32 G3Function80037964(uint32 object, uint32 resource, uint32 count, uint32 radius, uint32 guest_stack);
uint32 G3Function80037A20(uint32 object, uint32 resource, uint32 count, uint32 radius, uint32 incoming_v0, uint32 guest_stack);
uint32 G3Function800378D0(uint32 object, uint32 resource, uint32 count, uint32 x, uint32 incoming_v0, uint32 guest_stack);
uint32 G3Function800449F0(uint32 effect, uint32 x, uint32 y, uint32 z, uint32 incoming_v0);
uint32 G3Function80044A40(uint32 effect, uint32 x, uint32 y, uint32 z, uint32 incoming_v0);
uint32 G3Function80037DBC(uint32 object, uint32 guest_stack);
uint32 G3Function800425CC(uint32 object, uint32 mode, uint32 vector, uint32 option, uint32 guest_stack);
uint32 G3Function80041A14(uint32 effect, uint32 callback, uint32 owner, uint32 object, uint32 guest_stack);
uint32 G3Function80041764(uint32 effect, uint32 face, uint32 model, uint32 pool, uint32 guest_stack);
uint32 G3Function80045920(uint32 effect, uint32 guest_stack);
uint32 G3DispatchFaceInitializer(uint32 target, uint32 effect, uint32 callback, uint32 owner, uint32 object, uint32 guest_stack);
uint32 G3Function8004297C(uint32 object, uint32 mode, uint32 vector, uint32 option, uint32 guest_stack);

uint32 G3Function8002F290(uint32 cosine, uint32 guest_stack);

uint32 G3Function8003FB7C(uint32 channel, uint32 state, uint32 guest_stack);
uint32 G3Function8003FBB0(uint32 channel, uint32 state, uint32 guest_stack);

uint32 G3Function80018D1C(uint32 object, uint32 sequence, uint32 mode, uint32 guest_stack);
uint32 G3Function80039158(uint32 effect, uint32 pool, uint32 guest_stack);
uint32 G3Function800440D0(uint32 object, uint32 pool, uint32 x, uint32 y, uint32 guest_stack);
uint32 G3Function80024254(uint32 game, uint32 incoming_v0, uint32 guest_stack);
uint32 G3Function8003EF64(uint32 guest_stack);
void G3Function8004D8B0(uint32 camera, uint32 value);
uint32 G3Function8002ACC8(uint32 guest_stack);
uint32 G3Function8002AC44(uint32 game);
void G3Function80039B18(uint32 level, uint32 guest_stack);
uint32 G3Function8002ABB8(uint32 records, uint32 count, uint32 guest_stack);
uint32 G3Function80018E00(uint32 writes, uint32 incoming_v0);
uint32 G3Function800150A8(uint32 guest_stack);
uint32 G3Function8006FF1C(uint32 player, uint32 game, uint32 state, uint32 guest_stack);
uint32 G3Function8004DF4C(uint32 enabled, uint32 incoming_v0);
uint32 G3Function8002BCA8(uint32 state, uint32 guest_stack);
uint32 G3Function8002B428(uint32 level, uint32 guest_stack);
uint32 G3Function800151AC(uint32 guest_stack);
void G3Function800657C0(void);
void G3Function80081010(void);
void G3Function80028F78(void);
uint32 G3Function80046B1C(uint32 index);
uint32 G3Function80023AAC(uint32 game, uint32 guest_stack);
uint32 G3Function800359EC(uint32 key, uint32 enabled, uint32 guest_stack);
uint32 G3Function8002937C(uint32 pool, uint32 object, uint32 guest_stack);
uint32 G3Function800305B0(uint32 object, uint32 incoming_v0, uint32 guest_stack);
uint32 G3Function800309E8(uint32 head, uint32 incoming_v0, uint32 guest_stack);
uint32 G3Function800309A4(uint32 head, uint32 incoming_v0, uint32 guest_stack);
uint32 G3Function80076418(uint32 pool, uint32 entry);
uint32 G3Function800352AC(uint32 mode);
uint32 G3Function8003592C(uint32 key, uint32 index, uint32 guest_stack);
uint32 G3Function80035594(uint32 key, uint32 mode, uint32 guest_stack);
uint32 G3Function80035784(uint32 key, uint32 index, uint32 guest_stack);
uint32 G3Function800653CC(uint32 filename, uint32 sector, uint32 destination, uint32 mode, uint32 guest_stack);
uint32 G3Function80027FDC(void);
uint32 G3Function8003DC08(uint32 object, uint32 request, uint32 guest_stack);
uint32 G3Function8003E0AC(uint32 object, uint32 request, uint32 guest_stack);
uint32 G3Function80081194(uint32 origin, uint32 position, uint32 outer_radius, uint32 inner_radius, uint32 guest_stack);
uint32 G3Function8001F3BC(uint32 packet, uint32 order_table, uint32 level, uint32 guest_stack);
uint32 G3Function8001EB54(uint32 vertex0, uint32 vertex1, uint32 vertex2, uint32 uv0, uint32 guest_stack);
uint32 G3Function80016ECC(void);
uint32 G3Function8003F108(uint32 sound);
uint32 G3Function8004BEE8(uint32 sound);
uint32 G3Function80035648(uint32 key_output, uint32 mode_output, uint32 guest_stack);
uint32 G3Function800358C0(uint32 key, uint32 divisor, uint32 guest_stack);
uint32 G3Function8003525C(void);
uint32 G3Function800353B8(uint32 guest_stack);
uint32 G3Function8003556C(void);
uint32 G3SdkSprintfFourStrings80088A1C(uint32 destination, uint32 format, uint32 first, uint32 second, uint32 third, uint32 fourth);
uint32 G3Function80048870(uint32 guest_stack);
void G3DispatchAudioFade(uint32 target, uint32 guest_stack);
uint32 G3Function8003F088(uint32 handle);
uint32 G3Function8003F0A8(uint32 handle);
uint32 G3Function8003F0C8(uint32 sound);
uint32 G3Function8003F0E8(uint32 sound, uint32 *third_glyph);
uint32 G3Function8003F22C(uint32 handle, uint32 volume, uint32 pan, uint32 pitch);
uint32 G3Function80035A80(void);
uint32 G3Function80035AA4(void);
uint32 G3Function80035F38(uint32 game, uint32 guest_stack);
uint32 G3Function8003F8A4(uint32 path, uint32 state, uint32 guest_stack, G3TextMeasureOperands *operands);
uint32 G3Function800361FC(uint32 instance, uint32 distance);
uint32 G3Function80036278(uint32 direction, uint32 output, uint32 scale, uint32 guest_stack);
void G3Function8002F630(uint32 axis, uint32 matrix, uint32 angle, uint32 guest_stack);
uint32 G3Function80038AA8(uint32 angle_address, uint32 target, uint32 step);
uint32 G3Function80036738(uint32 instance, uint32 target, uint32 distance, uint32 turn, uint32 aligned_output, uint32 guest_stack);
uint32 G3Function8003639C(uint32 instance, uint32 turn, uint32 speed, uint32 target, uint32 flags, uint32 guest_stack);
uint32 G3Function80036988(uint32 instance, uint32 target, uint32 distance, uint32 turn, uint32 threshold, uint32 guest_stack);
uint32 G3Function80036B80(uint32 source, uint32 target, uint32 speed, uint32 gravity, uint32 minimum, uint32 maximum, uint32 vertical_output, uint32 speed_output, uint32 guest_stack);
uint32 G3Function8003465C(uint32 matrix, uint32 direction, uint32 position, uint32 guest_stack);
void G3Function8002FF58(uint32 matrix, uint32 direction, uint32 guest_stack);
uint32 G3Function80034730(uint32 vertices, uint32 output, uint32 model, uint32 transform, uint32 matrices);
uint32 G3Function8003499C(uint32 vertices, uint32 model, uint32 transform, uint32 matrices);
uint32 G3Function80034FA4(uint32 sequence, uint32 time, uint32 *second_glyph, uint32 *third_glyph);
uint32 G3Function80035044(uint32 sequence, uint32 time, uint32 packets, uint32 order_table);
uint32 G3Function8003420C(uint32 game, uint32 order_table, uint32 guest_stack);
uint32 G3Function800338E4(uint32 object, uint32 descriptor, uint32 depth);
void G3Function8002B168(uint32 object, uint32 guest_stack);
uint32 G3Function80033BA8(uint32 object, uint32 scene, uint32 vertices, uint32 packets, uint32 order_table, uint32 overlay, uint32 depth, uint32 guest_stack);
uint32 G3Function80034BD0(uint32 object, uint32 scene, uint32 vertices, uint32 packets, uint32 order_table, uint32 overlay, uint32 depth, uint32 guest_stack);
uint32 G3Function8002B4B8(uint32 scene, uint32 object, uint32 packets, uint32 order_table, uint32 guest_stack);
void G3DispatchObjectRender(uint32 target, uint32 scene, uint32 object, uint32 vertices, uint32 packets, uint32 order_table, uint32 guest_stack);
uint32 G3Function80033950(void);
void G3Function80033A54(uint32 enabled);
uint32 G3DispatchPrimitiveRender(uint32 target, uint32 model, uint32 object, uint32 vertices, uint32 matrix, uint32 packets, uint32 order_table, uint32 guest_stack);
uint32 G3Function80033B18(uint32 descriptor, uint32 model);
uint32 G3Function800331A8(uint32 destination, uint32 matrix, uint32 plane, uint32 guest_stack);
uint32 G3Function800330E4(uint32 object, uint32 matrices, uint32 model);
uint32 G3Function80031EAC(uint32 object);
uint32 G3Function800315E8(uint32 object, uint32 matrices, uint32 model, uint32 guest_stack);
uint32 G3Function800317F0(uint32 object, uint32 matrices, uint32 model, uint32 guest_stack);
void G3Function80031A14(uint32 segment, uint32 matrix, uint32 parent_matrix, uint32 object, uint32 scaled, uint32 guest_stack);
uint32 G3Function8002FD3C(uint32 vector);
uint32 G3Function80031F5C(uint32 object);
void G3Function800322CC(uint32 game, uint32 guest_stack);
void G3Function800328A0(uint32 object, uint32 descriptor, uint32 matrices, uint32 model, uint32 guest_stack);
void G3Function80030520(uint32 object, uint32 model, uint32 animation, uint32 guest_stack);
void G3Function800305E8(uint32 object, uint32 model, uint32 animation, uint32 guest_stack);
uint32 G3Function80030B54(uint32 context, uint32 guest_stack);
uint32 G3DispatchChannelValue(uint32 target, uint32 context, uint32 guest_stack);
void G3Function8005BC88(uint32 rotation, uint32 matrix, uint32 guest_stack);
uint32 G3Function8005B8D4(uint32 angle, uint32 matrix);
uint32 G3Function800333F4(uint32 source, uint32 vertices, uint32 model, uint32 scene, uint32 matrices, uint32 overlay, uint32 first_segment, uint32 guest_stack);
uint32 G3Function800194C4(uint32 source, uint32 vertices, uint32 model, uint32 scene, uint32 matrices, uint32 guest_stack);
uint32 G3Function8001D544(uint32 model, uint32 vertices, uint32 packets, uint32 order_table, uint32 red, uint32 green, uint32 blue, uint32 guest_stack);
uint32 G3Function8001D770(uint32 model, uint32 vertices, uint32 packets, uint32 order_table, uint32 red, uint32 green, uint32 blue, uint32 guest_stack);
uint32 G3Function8001D180(uint32 model, uint32 vertices, uint32 packets, uint32 order_table, uint32 guest_stack);
uint32 G3Function800339B8(uint32 guest_stack);
uint32 G3Function80063C6C(uint32 model, uint32 vertices, uint32 packets, uint32 order_table, uint32 guest_stack);
uint32 G3DispatchPolygonRender(uint32 target, uint32 model, uint32 vertices, uint32 packets, uint32 order_table, uint32 guest_stack);
uint32 G3Function8005ADF0(uint32 matrix, uint32 scale);

void G3Function80026D54(uint32 game, uint32 guest_stack);
void G3Function8004D908(uint32 destination);
uint32 G3Function8004D9C0(void);
uint32 G3Function8004D9F0(void);
void G3Function8004DA00(uint32 destination);
void G3Function8004DA30(uint32 destination);
uint32 G3Function8004DA68(uint32 position);
void G3Function80030FF0(uint32 camera, uint32 guest_stack);
uint32 G3Function8008BCA8(uint32 matrix, uint32 translation);
uint32 G3Function8004D968(uint32 camera, uint32 guest_stack);
void G3Function80026904(void);
uint32 G3Function8002690C(uint32 game, uint32 guest_stack);
uint32 G3Function800269A0(uint32 game);
void G3Function80026E48(uint32 game, uint32 guest_stack);
uint32 G3Function80026E80(uint32 game, uint32 guest_stack);
uint32 G3Function80026EC4(uint32 game, uint32 guest_stack);
void G3Function80026EFC(void);
uint32 G3Function80026F04(uint32 game);
void G3Function80026F20(void);
uint32 G3Function80026F28(uint32 game, uint32 guest_stack);
void G3Function80026948(uint32 game, uint32 guest_stack);
void G3Function8007776C(uint32 guest_stack);
void G3Function800271F8(uint32 game, uint32 guest_stack);
void G3Function80026A34(uint32 game, uint32 guest_stack);
void G3Function80026A9C(uint32 game, uint32 guest_stack);
void G3Function80026C00(uint32 game, uint32 guest_stack);
void G3Function80026C54(uint32 game, uint32 guest_stack);
void G3AnimateVisibleStreams8005B0F0(uint32 scene, uint32 camera);
void G3Function80026CAC(uint32 game, uint32 guest_stack);
void G3Function80026CEC(uint32 game, uint32 guest_stack);
void G3Function80026D8C(uint32 game, uint32 guest_stack);
uint32 G3Function8002B0F0(uint32 game);
void G3Function800657D8(uint32 game, uint32 guest_stack);
void G3Function800268B8(uint32 game, uint32 guest_stack);
uint32 G3Function800812A0(uint32 guest_stack);
uint32 G3Function80039F5C(uint32 owner, uint32 state, uint32 request, uint32 guest_stack);
void G3Function80029784(uint32 game, uint32 guest_stack);
void G3Function80029690(uint32 pool, uint32 mark_resource, uint32 guest_stack);
uint32 G3Function80029574(uint32 pool, uint32 object, uint32 guest_stack);
void G3Function800429B4(uint32 owner, uint32 guest_stack);
void G3Function800756C0(uint32 guest_stack);
void G3Function80029D88(uint32 pool, uint32 guest_stack);
uint32 G3Function80018E40(uint32 guest_stack);
uint32 G3DispatchTimedScriptPoll(uint32 target, uint32 object, uint32 payload, uint32 guest_stack);
void G3Function8005DF0C(uint32 pool, uint32 guest_stack);
uint32 G3Function8005DD9C(uint32 first, uint32 second, uint32 guest_stack);
uint32 G3Function8005CBCC(uint32 first, uint32 second, uint32 phase, uint32 guest_stack);
uint32 G3Function80016ED4(void);
void G3Function800209F8(uint32 value);
uint32 G3Function80025644(uint32 effect, uint32 pool, uint32 guest_stack);
uint32 G3Function80014220(uint32 object, uint32 mask);
uint32 G3Function80014254(uint32 object, uint32 mask);
uint32 G3Function800142F4(uint32 object, uint32 flags, uint32 guest_stack);
void G3Function80014450(uint32 source);
uint32 G3Function80014460(uint32 object, uint32 first, uint32 second);
uint32 G3Function800153FC(uint32 object, uint32 command, uint32 guest_stack);
uint32 G3Function8001543C(uint32 context, uint32 command);
uint32 G3Function800154A4(uint32 context, uint32 command);
uint32 G3Function800154D4(void);
uint32 G3Function800154DC(void);
uint32 G3Function800154E4(uint32 context, uint32 command);
uint32 G3Function80015510(uint32 context, uint32 command);
uint32 G3Function800155D4(void);
uint32 G3Function80015614(uint32 context, uint32 command, uint32 guest_stack);
uint32 G3Function80015644(uint32 context, uint32 command);
uint32 G3Function80015668(uint32 context, uint32 command);
uint32 G3Function8001580C(uint32 context, uint32 command);
uint32 G3Function8001583C(uint32 context, uint32 command);
uint32 G3Function8001589C(void);
uint32 G3Function800158A4(void);
uint32 G3Function80015BA8(uint32 object, uint32 command, uint32 guest_stack);
uint32 G3Function80015CFC(void);
uint32 G3Function80015D04(uint32 context, uint32 command);
uint32 G3Function80015D40(uint32 context, uint32 command);
uint32 G3Function80015E30(uint32 context, uint32 command, uint32 guest_stack);
uint32 G3Function80015E64(uint32 context, uint32 command, uint32 guest_stack);
uint32 G3Function80015E98(uint32 context, uint32 command, uint32 guest_stack);
uint32 G3Function80015ECC(uint32 context, uint32 command, uint32 guest_stack);
uint32 G3Function80015F0C(uint32 context, uint32 command, uint32 guest_stack);
uint32 G3Function80016174(uint32 guest_stack);
uint32 G3Function80016248(void);
uint32 G3Function80016310(uint32 context, uint32 command);
uint32 G3Function80016330(uint32 source_object, uint32 command, uint32 guest_stack);
uint32 G3Function80016574(uint32 context, uint32 command, uint32 guest_stack);
uint32 G3Function800165B8(uint32 source_object, uint32 command, uint32 guest_stack);
uint32 G3Function80016820(void);
uint32 G3Function80016828(void);
uint32 G3Function80016888(void);
uint32 G3Function8001698C(uint32 context, uint32 command);
uint32 G3Function800169C8(uint32 context, uint32 command);
uint32 G3Function80016A04(uint32 context, uint32 command);
uint32 G3Function80016A24(uint32 context, uint32 command);
uint32 G3Function80016A68(uint32 context, uint32 command);
uint32 G3Function80016AA0(uint32 context, uint32 command);
uint32 G3Function80016AE4(uint32 context, uint32 command);
void G3Function8005EF40(uint32 pool, uint32 level, uint32 guest_stack);
void G3Function8002B9D4(uint32 game, uint32 guest_stack);
uint32 G3Function8006EB20(uint32 game, uint32 guest_stack);
uint32 G3Function8006EFE0(uint32 object, uint32 game, uint32 data, uint32 argument3, uint32 guest_stack);
uint32 G3Function8006F11C(uint32 request, uint32 normal, uint32 guest_stack);
uint32 G3Function8007767C(void);
uint32 G3Function8004DACC(void);
uint32 G3Function80027118(uint32 game, uint32 guest_stack);
void G3Function80011A84(uint32 guest_stack);
void G3Function800128D4(uint32 guest_stack);
void G3Function80012A28(uint32 guest_stack);
void G3Function80012AB4(uint32 game, uint32 guest_stack);
void G3Function80012C5C(uint32 game, uint32 guest_stack);
void G3Function800265AC(uint32 game, uint32 guest_stack);
void G3Function80026680(uint32 game, uint32 guest_stack);
void G3Function800266B4(uint32 game, uint32 guest_stack);
void G3Function800266F8(uint32 game, uint32 guest_stack);
void G3Function8002678C(void);
void G3Function80026794(uint32 game, uint32 guest_stack);
uint32 G3Function80011A58(uint32 entry);
uint32 G3Function8003EEAC(uint32 guest_stack);
void G3Function8006A580(uint32 resource, uint32 guest_stack);
uint32 G3Function80027CA4(uint32 guest_stack);
void G3Function80081038(uint32 guest_stack);
uint32 G3Function80027E6C(uint32 guest_stack);
uint32 G3Function8003EEE4(uint32 guest_stack);
void G3Function800810A8(uint32 guest_stack);
uint32 G3Function80025FC4(uint32 game);
void G3Function80026018(uint32 game, uint32 guest_stack);
void G3Function80026090(uint32 game, uint32 guest_stack);
uint32 G3Function80027F5C(void);
uint32 G3Function80027F7C(void);
void G3Function80026128(uint32 game, uint32 guest_stack);
void G3Function8002616C(uint32 game, uint32 guest_stack);
void G3Function80026190(uint32 game, uint32 guest_stack);
void G3Function800261F0(uint32 game, uint32 guest_stack);
void G3Function80026280(uint32 game, uint32 guest_stack);
void G3Function800262C8(uint32 game, uint32 guest_stack);
void G3Function80026384(uint32 game, uint32 guest_stack);
void G3Function800263CC(uint32 game, uint32 guest_stack);
void G3Function80026438(void);
uint32 G3Function80026440(uint32 game);
void G3Function8002651C(uint32 game, uint32 guest_stack);
void G3Function8002BD64(void);
void G3Function8002BD6C(void);
void G3Function8002BE94(uint32 camera, uint32 scene, uint32 context, uint32 guest_stack);
void G3Function8002BFF0(uint32 context, uint32 pool, uint32 guest_stack);
void G3Function8002463C(uint32 game, uint32 order_table, uint32 guest_stack);
void G3Function800297A8(uint32 game, uint32 guest_stack);
void G3Function8005AECC(uint32 level, uint32 camera, uint32 context, uint32 guest_stack);
uint32 G3Function8003A9C0(uint32 level, uint32 camera, uint32 context, uint32 guest_stack);
void G3Function80042ACC(uint32 owner, uint32 game, uint32 order_table, uint32 guest_stack);
void G3Function80075418(uint32 game, uint32 order_table, uint32 guest_stack);
uint32 G3Function8001E924(uint32 packets, uint32 order_table, uint32 guest_stack);
void G3Function80025BA8(uint32 game, uint32 guest_stack);
void G3DispatchSceneRender(uint32 target, uint32 scene, uint32 camera, uint32 packets, uint32 order_table, uint32 guest_stack);
// Native terrain packet traversal state
struct G3TerrainRenderState
{
    uint32 packet_cursor;
    uint32 packet_limit;
    uint32 model_entry;
    uint32 model_end;
    uint32 colors;
};
// Native subdivision color, projection and packet data
struct G3TrianglePacketState
{
    uint32 packet_cursor;
    uint32 first_green;
    uint32 edge_extent;
    uint32 second_red;
    uint32 projection;
    uint32 face_flags;
    uint32 bound;
    uint32 address_mask;
    uint32 depth_offset;
    uint32 colors;
};
uint32 G3Function80084C68(G3TrianglePacketState *triangle, uint32 flag_mask, uint32 packet_tag, uint32 order_table, uint32 scratch, uint32 return_site, uint32 guest_stack);
uint32 G3Function80084A64(uint32 models, uint32 level, uint32 camera, uint32 packets, uint32 guest_stack);
void G3Function800838DC(G3TerrainRenderState *render, uint32 vertices, uint32 order_table, uint32 return_site, uint32 guest_stack);
uint32 G3DispatchTerrainRender(uint32 target, uint32 context, uint32 level, uint32 camera, uint32 packets, uint32 order_table, uint32 guest_stack);
void G3Function800258D0(uint32 resource, uint32 game, uint32 count, uint32 vertical_range, uint32 position, uint32 horizontal_range, uint32 guest_stack);
uint32 G3Function8004242C(uint32 owner, uint32 data, uint32 position, uint32 rotation, uint32 guest_stack);
uint32 G3Function80042230(uint32 effect, uint32 callback, uint32 storage, uint32 argument3, uint32 guest_stack);
uint32 G3DispatchEffectInitializer(uint32 target, uint32 effect, uint32 callback, uint32 storage, uint32 argument3, uint32 guest_stack);
uint32 G3Function80044BC4(uint32 effect, uint32 callback, uint32 storage, uint32 argument3, uint32 guest_stack);
void G3Function8004D938(uint32 source);
uint32 G3Function8001E140(uint32 color, uint32 x0, uint32 y0, uint32 x1, uint32 y1, uint32 x2, uint32 y2, uint32 x3, uint32 y3, uint32 pool, uint32 order);
uint32 G3Function8001E25C(uint32 x0, uint32 y0, uint32 x1, uint32 y1, uint32 x2, uint32 y2, uint32 x3, uint32 y3, uint32 red, uint32 green, uint32 blue, uint32 blend, uint32 pool, uint32 order);
uint32 G3Function8001E340(uint32 radius, uint32 pool, uint32 order_table, uint32 guest_stack);
uint32 G3Function8001E7F4(uint32 x0, uint32 y0, uint32 x1, uint32 y1, uint32 pool, uint32 order_table, uint32 guest_stack);

uint32 G3Function80065948(uint32 guest_stack);

uint32 G3Function80065E14(uint32 object, uint32 table, uint32 resource, uint32 guest_stack);

uint32 G3Function80065E70(uint32 object, uint32 table, uint32 values, uint32 guest_stack);

uint32 G3Function80066814(uint32 object, uint32 extension, uint32 guest_stack);

uint32 G3Function80066CC8(uint32 object, uint32 guest_stack);

uint32 G3Function80067010(uint32 state, uint32 value);
uint32 G3Function80067020(uint32 state, uint32 mode);
uint32 G3Function800670C4(uint32 state, uint32 index);
uint32 G3Function80067110(uint32 state, uint32 index);

uint32 G3Function8006713C(uint32 state);

uint32 G3Function80067A34(uint32 header);


uint32 G3Function800675F4(uint32 state, uint32 base, uint32 guest_stack);
uint32 G3Function80067640(uint32 state, uint32 guest_stack);
uint32 G3Function8006802C(uint32 io, uint32 guest_stack);
uint32 G3Function80068068(uint32 io, uint32 guest_stack);

uint32 G3Function80067A28(uint32 header);


uint32 G3Function80067148(uint32 state);
uint32 G3Function800674F4(uint32 state);

uint32 G3Function80067F4C(uint32 value);
uint32 G3Function80067F54(uint32 value);
uint32 G3Function80067F5C(uint32 value);

uint32 G3Function80067FE8(uint32 io, uint32 guest_stack);


uint32 G3Function80016B28(uint32 context, uint32 command);

uint32 G3Function80016BDC(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function80016C4C(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function80016D44(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function80016D80(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function80016DE4(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function80016E20(uint32 context, uint32 command);

uint32 G3Function80016E64(uint32 context, uint32 command);

uint32 G3Function80016EA8(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function80016EDC(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function8006F55C(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function80016F50(uint32 context, uint32 command, uint32 guest_stack);

uint32 G3Function8001722C(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function8001725C(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function8004DC44(void);

uint32 G3Function8003F068(uint32 guest_stack);
uint32 G3Function8003F004(uint32 guest_stack);
uint32 G3Function8003F040(uint32 guest_stack);
uint32 G3Function8003EF8C(uint32 guest_stack);

uint32 G3Function80017384(uint32 context, uint32 command);

uint32 G3Function800173E0(uint32 context, uint32 command);

uint32 G3Function80017410(uint32 context, uint32 command, uint32 guest_stack);

uint32 G3Function800174B4(uint32 context, uint32 command);

uint32 G3Function800174D8(uint32 context, uint32 command, uint32 guest_stack);

uint32 G3Function80017518(uint32 context, uint32 command, uint32 guest_stack);

uint32 G3Function80017598(void);

uint32 G3Function800175A0(void);

uint32 G3Function800175A8(void);

uint32 G3Function800789C0(uint32 object, uint32 game, uint32 state, uint32 guest_stack);

uint32 G3Function8007A19C(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007A738(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007A9A8(uint32 object, uint32 game, uint32 state, uint32 guest_stack);

uint32 G3Function8007AE78(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function80084ED8(uint32 object, uint32 resource, uint32 guest_stack);

uint32 G3Function80017680(uint32 effect, uint32 callback, uint32 storage, uint32 guest_stack);

uint32 G3Function8001785C(uint32 effect, uint32 pool, uint32 guest_stack);

uint32 G3Function80038B74(uint32 object, uint32 x, uint32 y, uint32 z, uint32 guest_stack);

uint32 G3Function80017958(uint32 object, uint32 command, uint32 guest_stack);

void G3Function8004D860(uint32 camera, uint32 target);
void G3Function8004D870(uint32 camera, uint32 target);

uint32 G3Function80017D00(uint32 context, uint32 command);

uint32 G3Function80017DC0(void);

uint32 G3Function80017DC8(void);

uint32 G3Function80017DD0(uint32 context, uint32 command, uint32 guest_stack);

uint32 G3Function80017DF8(uint32 context, uint32 command, uint32 guest_stack);

uint32 G3Function80017E20(void);

uint32 G3Function80017E40(void);

uint32 G3Function80017E60(void);

uint32 G3Function80017E80(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function80017FA4(uint32 context, uint32 command);

uint32 G3Function80017FD4(uint32 context, uint32 command);

uint32 G3Function8001801C(uint32 context, uint32 command);

uint32 G3Function80018034(uint32 context, uint32 command);

uint32 G3Function80018078(uint32 context, uint32 command);

uint32 G3Function80018108(uint32 context, uint32 command);

uint32 G3Function800181AC(uint32 context, uint32 command);

uint32 G3Function8001823C(uint32 context, uint32 command);

uint32 G3Function8001829C(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function800182BC(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function80018314(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function8001836C(uint32 context, uint32 command);

uint32 G3Function800183E8(uint32 context, uint32 command);

uint32 G3Function80018474(void);

uint32 G3Function8001847C(uint32 context, uint32 command);

uint32 G3Function8007B108(uint32 player, uint32 object, uint32 value, uint32 animation, uint32 guest_stack);
uint32 G3Function8006B90C(uint32 source, uint32 target, uint32 property, uint32 duration, uint32 guest_stack);

uint32 G3Function800184CC(uint32 context, uint32 command);

uint32 G3Function80018608(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function8001863C(uint32 object, uint32 command);

uint32 G3Function80018688(uint32 context, uint32 command);

uint32 G3Function800186DC(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function8001877C(uint32 object, uint32 command);

uint32 G3Function800187B0(void);

uint32 G3Function800187DC(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function8001893C(void);

uint32 G3Function800189D4(uint32 context, uint32 command, uint32 guest_stack);

uint32 G3Function80018A40(uint32 context, uint32 command, uint32 guest_stack);

uint32 G3Function80018A84(uint32 context, uint32 command, uint32 guest_stack);

uint32 G3Function80018AD4(uint32 guest_stack);

uint32 G3Function80018B1C(uint32 context, uint32 command, uint32 guest_stack);

uint32 G3Function80081B7C(uint32 kind, uint32 index, uint32 guest_stack);

uint32 G3Function80081BD4(uint32 kind, uint32 index, uint32 strength, uint32 guest_stack);

uint32 G3Function80039F1C(uint32 object, uint32 duration, uint32 direction);

uint32 G3Function80018B90(uint32 context, uint32 command, uint32 guest_stack);

uint32 G3Function80018C00(uint32 object, uint32 command, uint32 guest_stack);

uint32 G3Function80018C7C(uint32 context, uint32 command);

uint32 G3Function80018CA4(void);

uint32 G3Function80036144(uint32 first, uint32 second);

uint32 G3Function80036158(void);

uint32 G3Function8006BD2C(uint32 effect);

uint32 G3Function8007B188(uint32 player);

uint32 G3Function800245E8(uint32 selection, uint32 guest_stack);

uint32 G3Function80027C48(uint32 owner, uint32 offset);

void G3Function80028524(void);

void G3Function8002E218(void);

uint32 G3Function80027B4C(uint32 owner, uint32 current, uint32 stop, uint32 guest_stack);

void G3Function8002ED54(void);

uint32 G3Function8002F820(uint32 vector, uint32 angle);

uint32 G3Function8002FDE4(uint32 vector, uint32 guest_stack);

uint32 G3Function8002FE30(uint32 left, uint32 right, uint32 guest_stack);

uint32 G3Function8002FE88(uint32 left, uint32 right, uint32 guest_stack);

uint32 G3Function80036168(uint32 object);

void G3Function800361D4(void);

uint32 G3Function800361DC(uint32 object, uint32 animation, uint32 frame_value, uint32 guest_stack);

uint32 G3Function80037348(uint32 object, uint32 animation, uint32 frame_value, uint32 guest_stack);

uint32 G3Function80037258(uint32 object, uint32 requested, uint32 repeat, uint32 guest_stack);

uint32 G3Function800371B8(uint32 object);

uint32 G3Function80037210(uint32 object);

uint32 G3Function800373AC(uint32 object, uint32 repeat, uint32 guest_stack);

uint32 G3Function80038864(uint32 object, uint32 guest_stack);

uint32 G3Function80036908(uint32 value, uint32 angle);

uint32 G3Function80039C48(uint32 object, uint32 radius, uint32 output, uint32 guest_stack);

uint32 G3Function80049A60(void);

uint32 G3Function80049B44(void);

uint32 G3Function8003D6F0(uint32 object, uint32 data, uint32 guest_stack);

uint32 G3Function8003D7F4(uint32 object, uint32 data, uint32 guest_stack);

void G3Function8003E30C(void);

void G3Function8003EB28(void);

uint32 G3Function8003ECE0(void);

void G3Function80049DF4(void);

void G3Function8004AAFC(void);

uint32 G3Function8004AB04(uint32 command, uint32 bank);

uint32 G3Function8004AB98(uint32 command, uint32 bank);

void G3Function8004AC30(void);

uint32 G3Function8004AD00(uint32 command, uint32 bank);

uint32 G3Function80049578(uint32 index, uint32 guest_stack);

uint32 G3Function800495CC(uint32 index);

uint32 G3Function80049810(uint32 index, uint32 mask);

uint32 G3Function80049900(uint32 index, uint32 mask);

uint32 G3Function8004AEC4(uint32 command, uint32 context);

uint32 G3Function8004AEE8(uint32 command, uint32 context, uint32 guest_stack);

uint32 G3Function8004AF0C(uint32 command, uint32 context);

uint32 G3Function8004AF74(uint32 command, uint32 context);

uint32 G3Function8004AFA8(uint32 command, uint32 context);

uint32 G3Function8004AFE4(uint32 command, uint32 context);

uint32 G3Function8004B020(uint32 command, uint32 context);

uint32 G3Function8004B040(uint32 command, uint32 context);

uint32 G3Function8004B064(uint32 command, uint32 context);

uint32 G3Function8004B094(uint32 command, uint32 context);

uint32 G3Function8004B0C4(uint32 command, uint32 context);

uint32 G3Function8003DA70(uint32 object, uint32 data, uint32 guest_stack);

uint32 G3Function8003DE64(uint32 object, uint32 data, uint32 guest_stack);

uint32 G3Function80049534(uint32 index);

uint32 G3Function800494E0(uint32 index);

uint32 G3Function80049628(uint32 index);

uint32 G3Function8004B0F4(uint32 command, uint32 context);

uint32 G3Function8004B174(uint32 command, uint32 context);

uint32 G3Function8003F400(uint32 descriptor, uint32 limit);

uint32 G3Function8004B1C4(uint32 command, uint32 context);

uint32 G3Function8004B214(uint32 command, uint32 context);

uint32 G3Function8004B244(uint32 command, uint32 context);

uint32 G3Function8004B2B4(uint32 command, uint32 context);

uint32 G3Function8004B2E4(uint32 command, uint32 context);

uint32 G3Function8004B338(uint32 command, uint32 context);

uint32 G3Function8004B36C(uint32 command, uint32 context);

uint32 G3Function8004B3A8(uint32 command, uint32 context);

uint32 G3Function8004B3EC(uint32 command, uint32 context);

uint32 G3Function8004B41C(uint32 command, uint32 context);

uint32 G3Function8004B478(uint32 command);

uint32 G3Function8004B4F0(uint32 command);

uint32 G3Function8004B528(uint32 command);

uint32 G3Function8004B560(uint32 command);

uint32 G3Function8004B6A0(uint32 command, uint32 context);

uint32 G3Function8004B6C8(uint32 command, uint32 context);

uint32 G3Function8004B710(uint32 command, uint32 context);

uint32 G3Function8004B7A4(uint32 command, uint32 context);

uint32 G3Function8004B7F0(uint32 command, uint32 context);

uint32 G3Function8004B83C(uint32 command, uint32 context);

uint32 G3Function8004B888(uint32 command, uint32 context);

uint32 G3Function8004B8D4(uint32 command, uint32 context);

uint32 G3Function8004B924(uint32 command, uint32 context);

uint32 G3Function8004B94C(uint32 command, uint32 context);

uint32 G3Function8004B9C0(uint32 command, uint32 guest_stack);

uint32 G3Function8004BA78(uint32 command, uint32 guest_stack);

uint32 G3Function8004BAE8(uint32 command, uint32 guest_stack);

uint32 G3Function8004D9B0(void);

uint32 G3Function8004DAE4(void);

uint32 G3Function8005177C(uint32 position, uint32 argument1, uint32 target, uint32 offset, uint32 guest_stack);

uint32 G3Function8005820C(uint32 enabled, uint32 value);

uint32 G3Function8005AB7C(uint32 object, uint32 guest_stack);

uint32 G3Function800689AC(uint32 state);

uint32 G3Function80068C54(uint32 context, uint32 selected, uint32 incoming_third_glyph, uint32 guest_stack);

uint32 G3Function80068CF0(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);





uint32 G3Function80068FD8(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);

uint32 G3Function8004806C(uint32 forward);

uint32 G3Function8006B258(uint32 context, uint32 restore);

uint32 G3Function8006B290(uint32 context, uint32 event);

uint32 G3Function80069764(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);

uint32 G3Function800699AC(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);

uint32 G3Function800699E4(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);

uint32 G3Function80069D40(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);

uint32 G3Function80069DD4(uint32 context, uint32 selected, uint32 guest_stack);

uint32 G3Function80069E3C(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);

uint32 G3Function80069E8C(uint32 context, uint32 selected, uint32 guest_stack);

uint32 G3Function80069F0C(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);

uint32 G3Function80069F54(uint32 context, uint32 selected, uint32 guest_stack);

uint32 G3Function80069FD4(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);

uint32 G3Function80080FD4(uint32 index);

uint32 G3Function8006ADC8(uint32 context);

uint32 G3Function8006AE20(uint32 context);

uint32 G3Function8006AE40(uint32 context);

uint32 G3Function8006AED8(uint32 context, uint32 index);

uint32 G3Function8006A01C(uint32 context, uint32 index, uint32 title, uint32 guest_stack);

uint32 G3Function8006A088(uint32 context, uint32 selected, uint32 guest_stack);

uint32 G3Function8006A1FC(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);

uint32 G3Function8006A244(uint32 context, uint32 selected, uint32 guest_stack);

uint32 G3Function8006AD74(uint32 context);

uint32 G3Function8006AD9C(uint32 context);

uint32 G3Function8006A3F8(uint32 context, uint32 selected, uint32 guest_stack);

uint32 G3Function8006ADE8(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);

uint32 G3Function8006AE60(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);

uint32 G3Function8006AF68(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);

uint32 G3Function8006B08C(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);

uint32 G3Function8006B0CC(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);

uint32 G3Function8006B170(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);

uint32 G3Function8006954C(uint32 context, uint32 incoming_third_glyph, uint32 guest_stack);

uint32 G3Function8006F2BC(uint32 object, uint32 game, uint32 guest_stack);

uint32 G3Function8006BE58(uint32 object, uint32 guest_stack);

uint32 G3Function80045F40(uint32 owner);




uint32 G3Function8007620C(uint32 guest_stack);
uint32 G3Function800762C0(uint32 object);
uint32 G3Function80052088(uint32 camera, uint32 guest_stack);
uint32 G3Function80055014(uint32 guest_stack);
uint32 G3Function80051FE4(uint32 camera, uint32 guest_stack);
uint32 G3Function80054034(uint32 first, uint32 second, uint32 camera, uint32 context, uint32 source, uint32 mode, uint32 guest_stack);

uint32 G3Function80014970(uint32 object, uint32 game, uint32 guest_stack);

void G3Function800775F4(uint32 object, uint32 data);
uint32 G3Function8002A1C8(uint32 object, uint32 rate, uint32 guest_stack);

uint32 G3Function8006BFB8(uint32 object, uint32 data);

uint32 G3Function80080134(uint32 object, uint32 data);
void G3Function8006F34C(uint32 animation, uint32 key, uint32 duration);
uint32 G3Function8006D8C4(uint32 object, uint32 game, uint32 data, uint32 controls, uint32 guest_stack);
void G3Function800737E4(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function80074AB0(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8007E548(uint32 object, uint32 game, uint32 data, uint32 guest_stack);
uint32 G3Function8006C498(uint32 object, uint32 game, uint32 data, uint32 guest_stack);
void G3DispatchPlayerUpdate(uint32 target, uint32 object, uint32 game, uint32 data, uint32 controls, uint32 guest_stack);

uint32 G3Function80081E08(uint32 linked, uint32 game, uint32 guest_stack);
uint32 G3Function8006CBB4(uint32 data);
uint32 G3Function8006D6A0(uint32 object, uint32 game, uint32 data, uint32 controls, uint32 guest_stack);
uint32 G3Function8006D820(uint32 object, uint32 game, uint32 data, uint32 controls, uint32 guest_stack);

uint32 G3Function80046B00(uint32 index);
uint32 G3Function80043D98(uint32 object, uint32 selection, uint32 guest_stack);
uint32 G3Function80073630(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function800726FC(uint32 object, uint32 game, uint32 data, uint32 guest_stack);

uint32 G3Function8006CC04(uint32 game, uint32 phase, uint32 guest_stack);
uint32 G3Function8006D670(uint32 game);
uint32 G3Function8006CF20(uint32 game, uint32 guest_stack);
uint32 G3Function8006D4D0(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function80027644(uint32 root, uint32 name, uint32 game, uint32 guest_stack);
uint32 G3SdkSprintfStringInteger80088A1C(uint32 destination, uint32 format, uint32 string, uint32 value);
uint32 G3Function80046B38(void);
void G3Function8006CC88(uint32 list);
uint32 G3Function8006CCC0(uint32 state, uint32 position, uint32 guest_stack);
uint32 G3Function8006CD64(uint32 game, uint32 state, uint32 position, uint32 guest_stack);
uint32 G3Function8006CE84(uint32 state, uint32 position, uint32 guest_stack);
uint32 G3Function8006C634(void);
uint32 G3Function8006CA8C(uint32 resource, uint32 position, uint32 model, uint32 guest_stack);
uint32 G3Function8006CD18(uint32 game, uint32 index);
uint32 G3Function8006D020(uint32 game, uint32 guest_stack, G3TextMeasureOperands *operands);
uint32 G3Function8006D188(uint32 game, uint32 guest_stack);
void G3Function8006D3F8(void);
uint32 G3SdkSprintfNoValues80088A1C(uint32 destination, uint32 format);
uint32 G3SdkSprintfTwoIntegers80088A1C(uint32 destination, uint32 format, uint32 first, uint32 second, uint32 *last_field_address);
uint32 G3Function8006F620(uint32 object, uint32 game, uint32 guest_stack);
void G3Function80043CF8(uint32 object, uint32 old_value, uint32 new_value, uint32 guest_stack, uint32 *return_value);
uint32 G3Function8006DAF4(void);
uint32 G3Function8006F6D8(uint32 object, uint32 resource, uint32 enabled, uint32 guest_stack);
void G3Function80039834(uint32 model);
uint32 G3Function80043C68(uint32 old_value, uint32 new_value, uint32 model);
uint32 G3Function800775C8(uint32 object, uint32 game);
uint32 G3Function800214EC(uint32 parameters, uint32 resource, uint32 *second_glyph, uint32 *third_glyph);
uint32 G3Function8002151C(uint32 x, uint32 y, uint32 parameters, uint32 resource, uint32 *second_glyph, uint32 *third_glyph);
uint32 G3Function800509E0(uint32 vector, uint32 game, uint32 guest_stack);
uint32 G3Function80050A78(uint32 angle, uint32 guest_stack);
uint32 G3Function8002F540(uint32 first, uint32 second, uint32 guest_stack);
uint32 G3Function8001FA7C(uint32 angles, uint32 output, uint32 guest_stack);
uint32 G3Function800274D8(uint32 root, uint32 name, uint32 text);
uint32 G3Function80012720(uint32 guest_stack);
uint32 G3Function8001278C(uint32 guest_stack);
uint32 G3Function80027904(uint32 records, uint32 key_pointer);
uint32 G3Function80014F00(uint32 game);
void G3Function800151D4(uint32 placement, uint32 guest_stack);
void G3Function80015238(uint32 placement, uint32 guest_stack);
uint32 G3Function800152A4(uint32 context, uint32 command, uint32 guest_stack);
uint32 G3Function800152D4(uint32 context, uint32 command, uint32 guest_stack);
uint32 G3Function80015304(uint32 object, uint32 kind, uint32 first, uint32 second, uint32 guest_stack);
uint32 G3Function800153C8(uint32 object, uint32 command, uint32 guest_stack);
uint32 G3Function800175B0(uint32 context, uint32 command, uint32 guest_stack);
uint32 G3Function80017618(uint32 context, uint32 command, uint32 guest_stack);
uint32 G3Function8001553C(uint32 object, uint32 command, uint32 guest_stack);
uint32 G3Function800155DC(uint32 context, uint32 command);
uint32 G3Function80015600(uint32 object, uint32 command);
uint32 G3Function800157DC(uint32 context, uint32 command, uint32 guest_stack);
uint32 G3Function8001586C(uint32 context, uint32 command);
uint32 G3Function800158AC(uint32 context, uint32 command, uint32 guest_stack);
uint32 G3Function8004D784(uint32 camera, uint32 game, uint32 delta, uint32 guest_stack);
uint32 G3Function80015B50(uint32 context, uint32 delta, uint32 guest_stack);
uint32 G3Function80015CCC(uint32 context, uint32 command);
uint32 G3Function800499E0(uint32 index, uint32 value);
uint32 G3Function8003EDC4(uint32 index, uint32 value);
uint32 G3Function8003EAB0(uint32 command, uint32 guest_stack);
uint32 G3Function80015DF0(uint32 context, uint32 command, uint32 guest_stack);
uint32 G3Function80015F4C(uint32 context, uint32 command);
uint32 G3Function8001609C(uint32 context, uint32 command, uint32 guest_stack);
uint32 G3Function800168E8(uint32 context, uint32 command);
uint32 G3Function80016908(uint32 context, uint32 command);
uint32 G3Function80016928(uint32 object);
uint32 G3Function80016954(uint32 object);
uint32 G3Function80016970(uint32 context, uint32 command);
uint32 G3Function80016A44(uint32 context, uint32 command);
uint32 G3Function80016B6C(uint32 object, uint32 command, uint32 guest_stack);
uint32 G3Function80016CD0(uint32 object, uint32 command, uint32 guest_stack);
uint32 G3Function80016DBC(uint32 object, uint32 command, uint32 guest_stack);
uint32 G3Function80016F24(void);
uint32 G3Function8005A5F4(uint32 object, uint32 amount, uint32 guest_stack);
uint32 G3Function8001727C(uint32 context, uint32 command, uint32 guest_stack);
uint32 G3Function80017D80(uint32 context, uint32 command);
uint32 G3Function80017FF8(uint32 context, uint32 command);
uint32 G3Function8005A700(uint32 object, uint32 parameters, uint32 amount, uint32 angle, uint32 guest_stack);
uint32 G3Function8005A544(uint32 guest_stack);
uint32 G3Function8005AAAC(uint32 object, uint32 vector);
uint32 G3Function8005AB48(uint32 object);
uint32 G3Function80017EE0(uint32 context, uint32 command);
uint32 G3Function80017F40(uint32 context, uint32 command, uint32 guest_stack);
uint32 G3Function80018280(uint32 context, uint32 command);
uint32 G3Function80018508(uint32 context, uint32 command);
uint32 G3Function80018558(uint32 context, uint32 command);
uint32 G3Function800185A0(uint32 context, uint32 command);
uint32 G3Function800185E4(uint32 object, uint32 command);
uint32 G3Function8001887C(uint32 object, uint32 command);
uint32 G3Function800192D8(uint32 entries, uint32 mask);
uint32 G3Function8001E1E8(uint32 packet, uint32 mode, uint32 vertices);
uint32 G3Function80021A3C(uint32 parameters, uint32 *second_glyph, uint32 *third_glyph);
uint32 G3Function8002A6F4(uint32 object, uint32 index, uint32 guest_stack);
uint32 G3Function80029938(uint32 placement, uint32 guest_stack);
uint32 G3Function8002A930(uint32 placement, uint32 guest_stack);
uint32 G3Function8003A4B8(uint32 point, uint32 camera, uint32 guest_stack);
uint32 G3Function80045C44(uint32 destination, uint32 source, uint32 guest_stack);
uint32 G3Function8002BD74(uint32 camera, uint32 scene, uint32 active, uint32 inactive, uint32 guest_stack);
void G3Function8002CD84(uint32 parameters, uint32 text, uint32 incoming_second_glyph, uint32 incoming_third_glyph, uint32 guest_stack, uint32 *second_glyph, uint32 *third_glyph);
uint32 G3Function8002CEBC(uint32 parameters, uint32 text, uint32 incoming_second_glyph, uint32 incoming_third_glyph, uint32 guest_stack, uint32 *second_glyph, uint32 *third_glyph);
uint32 G3Function8002FAB0(uint32 vector, uint32 guest_stack);
uint32 G3Function8002FB10(uint32 vector);
uint32 G3Function8002FD98(uint32 vector, uint32 guest_stack);
uint32 G3Function8003774C(uint32 object, uint32 game, uint32 event, uint32 guest_stack);
uint32 G3Function80038A4C(uint32 value_pointer, uint32 target, uint32 step);
uint32 G3Function80039DFC(uint32 placement);
uint32 G3Function8002AA0C(uint32 placement, uint32 camera, uint32 guest_stack);
uint32 G3Function8003A618(uint32 leaf, uint32 guest_stack);
uint32 G3Function8003A7A8(uint32 root, uint32 camera, uint32 guest_stack);
uint32 G3Function8003E7E4(uint32 object, uint32 handle, uint32 pitch, uint32 volume, uint32 guest_stack);
uint32 G3Function8007E100(uint32 object, uint32 guest_stack);
uint32 G3Function8006F25C(uint32 request);
void G3Function8002AD3C(uint32 record, uint32 guest_stack);
uint32 G3Function8002BEEC(uint32 object, uint32 sphere, uint32 guest_stack);
uint32 G3Function8002DE24(uint32 guest_stack);
uint32 G3Function800388C8(uint32 contact, uint32 output, uint32 game, uint32 guest_stack);
uint32 G3Function80038D80(uint32 position, uint32 target, uint32 threshold, uint32 guest_stack);
uint32 G3ScaleMatrixL8008BCD8(uint32 matrix, uint32 scale);
uint32 G3Function80028004(uint32 output, uint32 input, uint32 index);
uint32 G3Function800285D0(uint32 output, uint32 current, uint32 previous);
uint32 G3Function80028A78(uint32 buttons);
void G3Function800271C0(void);
uint32 G3Function80028AC4(uint32 output, uint32 guest_stack);
uint32 G3Function80080610(uint32 object, uint32 game, uint32 event, uint32 guest_stack);
uint32 G3Function800806A8(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8008082C(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8006C6B4(uint32 game, uint32 model, uint32 guest_stack);
uint32 G3Function8006C774(uint32 game, uint32 previous_count, uint32 previous_flags, uint32 guest_stack);
uint32 G3Function80081C84(uint32 object, uint32 game, uint32 event, uint32 guest_stack);
uint32 G3Function80081E90(uint32 object);
uint32 G3Function80085CAC(uint32 destination, uint32 properties, uint32 key, uint32 fallback, uint32 incoming_v0);
uint32 G3Function80081FAC(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8003A150(uint32 minutes, uint32 seconds, uint32 ticks, uint32 guest_stack);
uint32 G3Function80052310(uint32 camera, uint32 output);
uint32 G3Function80084E1C(uint32 object, uint32 game, uint32 event, uint32 payload, uint32 guest_stack);
uint32 G3Function80084FC8(uint32 object, uint32 guest_stack);
uint32 G3Function800858C8(uint32 object, uint32 game, uint32 configuration, uint32 guest_stack);
uint32 G3Function8008504C(uint32 object, uint32 game, uint32 argument2, uint32 argument3, uint32 guest_stack);
uint32 G3Function80085340(uint32 object, uint32 game, uint32 argument2, uint32 argument3, uint32 guest_stack);
uint32 G3Function80055F18(uint32 object, uint32 event, uint32 payload, uint32 guest_stack);
uint32 G3Function80055580(void);
uint32 G3Function80055B40(uint32 object, uint32 guest_stack);
uint32 G3Function80055BAC(void);
uint32 G3Function80055990(uint32 state, uint32 position, uint32 guest_stack);
uint32 G3Function80055A24(void);
uint32 G3Function800555AC(void);
uint32 G3Function80055940(uint32 state, uint32 rotation, uint32 position, uint32 guest_stack);
uint32 G3Function80055674(uint32 output, uint32 position, uint32 guest_stack);
uint32 G3Function800558B4(uint32 output, uint32 state, uint32 position, uint32 guest_stack);
uint32 G3Function8004F5E4(uint32 object, uint32 game, uint32 event, uint32 payload, uint32 guest_stack);
uint32 G3Function8004EA20(void);
uint32 G3Function8004EF14(uint32 guest_stack);
uint32 G3Function8004EFCC(uint32 game, uint32 guest_stack);
uint32 G3Function8004EA60(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8004F3D4(uint32 game, uint32 guest_stack);
uint32 G3Function8004F488(uint32 state, uint32 guest_stack);
uint32 G3Function8005235C(uint32 object, uint32 game, uint32 event, uint32 payload, uint32 guest_stack);
uint32 G3Function80051864(void);
uint32 G3Function8005180C(uint32 object, uint32 event, uint32 guest_stack);
uint32 G3Function8005169C(void);
uint32 G3Function8005210C(uint32 incoming_v0);
uint32 G3Function80051890(uint32 object, uint32 guest_stack);
uint32 G3Function80051A10(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8005222C(uint32 mode, uint32 angle, uint32 target, uint32 velocity, uint32 guest_stack);
uint32 G3Function80052114(uint32 mode, uint32 value, uint32 target, uint32 velocity, uint32 guest_stack, uint32 incoming_v0);
uint32 G3Function8007DEAC(uint32 object, uint32 pitch, uint32 yaw, uint32 game, uint32 guest_stack);
uint32 G3Function8007DE0C(uint32 pitch, uint32 yaw);
uint32 G3Function8004DA94(uint32 output, uint32 count);
uint32 G3Function8004E5F8(uint32 object, uint32 game, uint32 event, uint32 payload, uint32 guest_stack);
uint32 G3Function8004DF5C(void);
uint32 G3Function8004DF88(uint32 object, uint32 mode);
uint32 G3Function8004E03C(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8004E1F4(uint32 object, uint32 guest_stack);
uint32 G3Function8004E260(uint32 object, uint32 payload, uint32 guest_stack);
uint32 G3Function8004E2F4(uint32 position, uint32 rotation, uint32 previous_position, uint32 previous_rotation, uint32 guest_stack);
uint32 G3Function8005402C(void);
uint32 G3Function80050024(uint32 object, uint32 game, uint32 event, uint32 payload, uint32 guest_stack);
uint32 G3Function8004F764(void);
uint32 G3Function8004F790(uint32 output, uint32 guest_stack);
uint32 G3Function8004F8C8(uint32 object, uint32 guest_stack);
uint32 G3Function8004FE9C(uint32 object, uint32 guest_stack);
uint32 G3Function8004FBB8(uint32 output, uint32 path, uint32 state, uint32 focus, uint32 guest_stack);
uint32 G3Function8004002C(uint32 path, uint32 callback, uint32 context, uint32 guest_stack);
uint32 G3Function80040220(uint32 position, uint32 guest_stack);
uint32 G3Function8003FCF4(uint32 path, uint32 callback, uint32 context, uint32 step, uint32 guest_stack);
uint32 G3Function80040B70(uint32 path, uint32 distance, uint32 state, uint32 guest_stack);
uint32 G3Function8004F85C(uint32 position, uint32 context, uint32 guest_stack);
uint32 G3Function8004FAAC(uint32 position, uint32 context, uint32 guest_stack);
uint32 G3Function8004FAC8(uint32 position, uint32 context, uint32 incoming_v1, uint32 incoming_lo, uint32 guest_stack);
uint32 G3DispatchPathScore(uint32 target, uint32 position, uint32 state, uint32 context, uint32 guest_stack);
uint32 G3Function8003FBE4(uint32 path, uint32 state);
uint32 G3Function8004E790(void);
uint32 G3Function8004E7BC(uint32 object, uint32 mode);
uint32 G3Function8004E860(uint32 object, uint32 guest_stack);
uint32 G3Function8004E994(uint32 object, uint32 event, uint32 payload, uint32 guest_stack);
uint32 G3Function800551D0(uint32 object, uint32 event, uint32 payload, uint32 guest_stack);
uint32 G3Function80055194(void);
uint32 G3Function80055150(uint32 incoming_v0);
uint32 G3Function80055170(uint32 position, uint32 rotation, uint32 blend, uint32 argument3, uint32 guest_stack);
uint32 G3Function800126E4(uint32 incoming_v0);
uint32 G3Function8006479C(uint32 incoming_v0);
uint32 G3Function8005153C(uint32 object, uint32 game, uint32 event, uint32 payload, uint32 guest_stack);
uint32 G3Function80050F64(void);
uint32 G3Function80054DB8(uint32 object, uint32 rotation, uint32 position, uint32 owner_rotation, uint32 guest_stack);
uint32 G3Function80054660(uint32 position, uint32 rotation, uint32 blend, uint32 argument3, uint32 guest_stack);
uint32 G3Function80050F90(uint32 object, uint32 game, uint32 argument2, uint32 argument3, uint32 guest_stack);
uint32 G3Function800512C0(uint32 object, uint32 game, uint32 argument2, uint32 argument3, uint32 guest_stack);
uint32 G3Function800512F4(uint32 object, uint32 game, uint32 argument2, uint32 argument3, uint32 guest_stack);
uint32 G3Function80050FE8(uint32 object, uint32 game, uint32 argument2, uint32 argument3, uint32 guest_stack);
uint32 G3Function800547F0(uint32 polar, uint32 output);
uint32 G3Function8005489C(uint32 direction, uint32 current, uint32 target);
uint32 G3Function80054918(uint32 angle, uint32 radius, uint32 height);
uint32 G3Function80054A14(uint32 angle, uint32 guest_stack);
uint32 G3Function8005437C(uint32 object, uint32 rotation, uint32 position, uint32 owner_rotation, uint32 guest_stack);
uint32 G3Function80054C38(uint32 distance, uint32 velocity, uint32 guest_stack);
uint32 G3Function80054C6C(uint32 velocity, uint32 polar);
uint32 G3DispatchScriptCommand(uint32 target, uint32 object, uint32 command, uint32 guest_stack);
uint32 G3Function8004F044(uint32 camera, uint32 command, uint32 guest_stack);
uint32 G3DispatchPoolCallback(uint32 target, uint32 effect, uint32 owner, uint32 guest_stack);
void G3Function8005EB00(uint32 object, uint32 level, uint32 guest_stack);
uint32 G3Function80075A28(uint32 effect, uint32 guest_stack);
void G3Function80075314(uint32 effect, uint32 guest_stack);
uint32 G3Function80075B54(uint32 effect, uint32 guest_stack);
uint32 G3Function80075F28(uint32 effect, uint32 guest_stack);
uint32 G3Function800760C8(uint32 effect, uint32 guest_stack);
uint32 G3Function8004DB64(uint32 camera, uint32 command, uint32 guest_stack);
uint32 G3Function8004DC7C(uint32 command);
uint32 G3Function8004DD0C(uint32 camera, uint32 command, uint32 guest_stack);
uint32 G3Function8004FF60(uint32 camera, uint32 command, uint32 guest_stack);
uint32 G3Function80057B70(uint32 camera, uint32 command, uint32 guest_stack);
uint32 G3Function80051314(uint32 camera, uint32 command, uint32 guest_stack);
uint32 G3Function80055BBC(uint32 camera, uint32 command, uint32 guest_stack);
uint32 G3Function80057C44(uint32 camera, uint32 command, uint32 guest_stack);
uint32 G3Function8002AF90(uint32 command);
uint32 G3Function80027960(uint32 command, uint32 guest_stack);
uint32 G3Function800158E4(uint32 object, uint32 command, uint32 guest_stack);
uint32 G3Function8004D880(uint32 guest_stack, G3TextMeasureOperands *operands);
uint32 G3Function8004DAFC(void);
uint32 G3Function8004F584(uint32 guest_stack, G3TextMeasureOperands *operands);
uint32 G3Function8004F5D4(void);
uint32 G3Function8006A538(uint32 context, uint32 guest_stack, G3TextMeasureOperands *operands);
uint32 G3Function8006B83C(uint32 context);
uint32 G3Function8006A5C0(uint32 context);
uint32 G3Function800697F0(uint32 context);
void G3Function80069848(uint32 context, uint32 callback, uint32 data, uint32 argument, uint32 guest_stack, uint32 *return_value);
void G3Function80069894(uint32 context, uint32 callback, uint32 data, uint32 argument, uint32 guest_stack);
uint32 G3Function80069984(uint32 context, uint32 value);
uint32 G3Function80069A1C(uint32 context, uint32 incoming_v0);
uint32 G3Function80069A4C(uint32 context, uint32 index, uint32 text, uint32 value, uint32 guest_stack);
uint32 G3Function8006AB04(uint32 context, uint32 position, uint32 text, uint32 selected, uint32 guest_stack);
uint32 G3Function8006AC48(uint32 context, uint32 guest_stack);
uint32 G3Function800275E0(uint32 guest_stack);
uint32 G3Function80051748(uint32 object, uint32 guest_stack);
uint32 G3Function800516C8(uint32 state, uint32 guest_stack);
uint32 G3Function80061078(uint32 output, uint32 position, uint32 target, uint32 radius, uint32 guest_stack);
uint32 G3Function80060F3C(uint32 output, uint32 context, uint32 argument2, uint32 argument3, uint32 guest_stack);
uint32 G3Function800836B0(uint32 object, uint32 game, uint32 event, uint32 guest_stack);
uint32 G3Function800837A8(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8004512C(uint32 effect, uint32 pool, uint32 guest_stack);
uint32 G3Function80044E1C(uint32 effect);
uint32 G3Function8004508C(uint32 effect, uint32 pool, uint32 guest_stack);
uint32 G3Function80044F14(uint32 effect, uint32 pool, uint32 guest_stack);
uint32 G3Function80039DBC(sint32 value, sint32 factor);
uint32 G3Function80044F7C(uint32 effect, uint32 pool, uint32 guest_stack);
uint32 G3Function8004502C(uint32 effect, uint32 pool, uint32 guest_stack);
uint32 G3DispatchTimedObjectCallback(uint32 target, uint32 object, uint32 argument1, uint32 argument2, uint32 effect, uint32 guest_stack);
uint32 G3Function800453A0(uint32 position, uint32 velocity, uint32 data, uint32 angle, uint32 guest_stack);
uint32 G3Function800453D0(uint32 position, uint32 velocity, uint32 data, uint32 angle, uint32 guest_stack);
uint32 G3Function80045448(uint32 position, uint32 velocity, uint32 data, uint32 factor, uint32 guest_stack);
uint32 G3Function800454F4(uint32 position, uint32 velocity, uint32 data, uint32 object, uint32 guest_stack);
void G3Function800455A0(uint32 position, sint32 count, uint32 radius, uint32 parameters, uint32 guest_stack);
uint32 G3Function800456EC(uint32 effect, uint32 pool, uint32 guest_stack);
void G3Function80045900(uint32 position, uint32 parameters, uint32 guest_stack);
uint32 G3Function800459D8(uint32 effect, uint32 pool, uint32 guest_stack);
uint32 G3Function80043F7C(uint32 effect, uint32 pool, uint32 guest_stack);
uint32 G3Function80044068(uint32 effect, uint32 size, uint32 divisor, uint32 lifetime);
uint32 G3Function80073604(uint32 object, uint32 size, uint32 flags);
void G3Function80044A90(uint32 effect, uint32 rotation, uint32 velocity);
void G3Function80044878(uint32 effect, uint32 size, uint32 *return_value);
void G3Function80044360(uint32 object, uint32 resource, uint32 center_resource, uint32 pool, uint32 guest_stack);
uint32 G3Function80044564(uint32 binding, uint32 resource, uint32 storage, uint32 pool, uint32 guest_stack);
uint32 G3Function80046540(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function8006C914(uint32 game, uint32 guest_stack);
uint32 G3Function80047DF4(uint32 state, uint32 level);
uint32 G3Function80046B70(uint32 x, uint32 y, uint32 model, uint32 scale, uint32 guest_stack);
uint32 G3Function80047190(uint32 state, uint32 parameters, uint32 single_position, uint32 double_position);
uint32 G3Function80046FD0(uint32 x, uint32 y, sint32 value, sint32 total, uint32 guest_stack);
uint32 G3Function80047FC8(uint32 level);
uint32 G3Function80047394(uint32 game, uint32 guest_stack);
uint32 G3Function80040280(uint32 channel, uint32 position, uint32 state, uint32 incoming_s3, uint32 guest_stack);
uint32 G3Function80040654(uint32 channel, uint32 position, uint32 initial_position, uint32 initial_state, uint32 guest_stack);
uint32 G3Function80040A40(uint32 channel, uint32 position, uint32 output_state, uint32 output_position, uint32 guest_stack);
uint32 G3Function8006869C(uint32 io, uint32 guest_stack);
uint32 G3Function80068780(uint32 io, uint32 guest_stack);
uint32 G3Function8008EAC8(uint32 destination, uint32 source, sint32 count);
uint32 G3Function8008DCC0(uint32 channel, uint32 name, uint32 buffer, uint32 offset, uint32 bytes);
uint32 G3Function8008DEE0(uint32 channel, uint32 name, uint32 buffer, uint32 offset, uint32 bytes);
uint32 G3Function8008E490(uint32 channel, uint32 name, uint32 blocks);
uint32 G3Function8008E698(uint32 channel, uint32 name);
void G3Function8006A624(void);
uint32 G3Function800689B8(uint32 state, uint32 guest_stack);
uint32 G3Function80068A10(uint32 state, uint32 guest_stack);
uint32 G3Function80068BA4(uint32 state);
uint32 G3Function80068BAC(uint32 state);
uint32 G3Function80068BB8(uint32 state);
uint32 G3Function80068BCC(uint32 state);
uint32 G3Function8006A600(uint32 context);
uint32 G3Function8006A62C(uint32 *third_glyph);
uint32 G3Function8006A654(uint32 event, uint32 *third_glyph);
uint32 G3Function8006B2B4(uint32 context, uint32 guest_stack);
void G3Function80068BE0(uint32 state, uint32 value);
uint32 G3Function80068BE8(uint32 context, uint32 guest_stack);
uint32 G3Function80068C1C(uint32 context, uint32 argument, uint32 event, uint32 guest_stack);
uint32 G3Function800698C4(uint32 context, uint32 callback, uint32 data, uint32 argument, uint32 incoming_v0, uint32 guest_stack);
uint32 G3Function80069000(uint32 context, uint32 selection, uint32 incoming_third_glyph, uint32 guest_stack);
sint32 G3NativeSaveGame(uint32 state, uint32 guest_stack);
sint32 G3NativeAutosaveGame(uint32 game, uint32 guest_stack);
sint32 G3NativeLoadGame(uint32 state, uint32 guest_stack);
uint32 G3NativeSaveResultPage(uint32 context, uint32 callback, uint32 incoming_third_glyph, uint32 guest_stack);
uint32 G3Function800694B4(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);
uint32 G3Function8006AAD4(uint32 context, uint32 text, uint32 incoming_third_glyph, uint32 guest_stack);
uint32 G3Function80069BC8(uint32 context, uint32 selected, uint32 minimum, uint32 maximum, uint32 incoming_v0, uint32 guest_stack);
uint32 G3Function8006ACD4(uint32 context);
uint32 G3Function8006983C(uint32 context);
uint32 G3Function80069B00(uint32 context, uint32 selected, uint32 event, uint32 incoming_v0, uint32 guest_stack);
uint32 G3Function80069918(uint32 context, uint32 incoming_v0, uint32 guest_stack);
uint32 G3Function80068E98(uint32 context, uint32 guest_stack);
void G3ResetMenuText(uint32 guest_stack);
uint32 G3Function80069B48(uint32 context, uint32 incoming_v0, uint32 guest_stack);
uint32 G3DispatchMenuPageCallback(uint32 target, uint32 context, uint32 selected, uint32 second_glyph, uint32 third_glyph, uint32 guest_stack);
uint32 G3DispatchMenuRowCallback(uint32 target, uint32 context, uint32 selected, uint32 event, uint32 guest_stack);
void G3DispatchMenuExitCallback(uint32 target, uint32 context, uint32 guest_stack);
void G3Function8006A6BC(void);
uint32 G3Function8006A9E4(uint32 game, uint32 text, uint32 incoming_third_glyph, uint32 guest_stack);
uint32 G3Function8006A6C4(uint32 game, uint32 y, uint32 text, uint32 incoming_third_glyph, uint32 guest_stack, G3TextMeasureOperands *operands);
uint32 G3Function80069804(uint32 context);
uint32 G3Function8006A308(uint32 context, uint32 argument, uint32 event, uint32 guest_stack);
uint32 G3Function8006A350(uint32 context, uint32 selected, uint32 guest_stack);
uint32 G3Function8006A5E0(uint32 state);
uint32 G3Function8006A72C(uint32 text);
uint32 G3Function8006A774(uint32 text, uint32 parentheses, uint32 *text_end, uint32 *source_end);
uint32 G3Function8008102C(void);
uint32 G3Function8004DF3C(void);
uint32 G3Function8006B58C(uint32 state, uint32 output, uint32 guest_stack);
uint32 G3Function8006B77C(uint32 state, uint32 payload, uint32 guest_stack);
uint32 G3Function8008101C(void);
uint32 G3Function8004DF00(uint32 enabled, uint32 guest_stack);
uint32 G3Function8006B2FC(uint32 game, uint32 payload, uint32 guest_stack);
void G3Function80068BC4(uint32 state, uint32 value);
void G3Function80068BD8(uint32 state, uint32 value);
uint32 G3Function8006B3A0(uint32 state, uint32 payload, uint32 mode, uint32 guest_stack);
uint32 G3Function8006A824(uint32 game, uint32 y, uint32 text, uint32 selected, uint32 size, uint32 guest_stack);
uint32 G3Function8006C110(uint32 scene, uint32 parent, uint32 vertices, uint32 packets, uint32 guest_stack);
void G3Function8006D4C8(void);
uint32 G3Function80071490(uint32 object);
uint32 G3Function8007205C(uint32 object, uint32 game, uint32 state, uint32 guest_stack);
void G3Function8004D9D0(uint32 object, uint32 value, uint32 guest_stack);
uint32 G3Function8007193C(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function80071834(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function80072D00(uint32 object, uint32 argument1, uint32 state, uint32 argument3, uint32 guest_stack);

uint32 G3Function80078684(uint32 object);
void G3Function80073560(uint32 object, uint32 pool, uint32 guest_stack);

uint32 G3Function800764F4(uint32 object);

uint32 G3Function8007855C(uint32 contact, uint32 game);
uint32 G3Function80039E48(uint32 object, uint32 game, uint32 value, uint32 guest_stack);

uint32 G3Function800772F0(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3Function80081D14(uint32 object, uint32 game, uint32 kind, uint32 value, uint32 guest_stack);

uint32 G3Function80076F60(uint32 face, uint32 object);

uint32 G3Function80077594(uint32 object, uint32 mode, uint32 game);

uint32 G3Function8007F2D0(uint32 controls, uint32 object, uint32 guest_stack);

uint32 G3Function8004D9E0(void);
uint32 G3Function8007F3C0(uint32 object, uint32 game, uint32 state, uint32 guest_stack);

uint32 G3Function80079178(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007CF04(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007D99C(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007CE9C(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007CD24(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007D0D0(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007D190(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007D244(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007DB40(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007DB80(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack, uint32 incoming_v0);

uint32 G3Function8007D7A8(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function80078740(uint32 object, uint32 game, uint32 state, uint32 guest_stack);
uint32 G3Function80078790(uint32 object, uint32 game, uint32 state, uint32 guest_stack);

uint32 G3Function8007FE80(uint32 object, uint32 game, uint32 state);
uint32 G3Function8007F934(uint32 object);

uint32 G3Function8007FA24(uint32 object, uint32 game, uint32 state, uint32 guest_stack);

uint32 G3Function8007F214(uint32 object);
uint32 G3Function8007E1C8(uint32 state, uint32 yaw, uint32 pitch, uint32 yaw_step, uint32 guest_stack);
uint32 G3Function8007E270(uint32 object, uint32 game, uint32 state, uint32 guest_stack);

uint32 G3Function8007DB14(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);
uint32 G3Function8007E444(uint32 object, uint32 game, uint32 state, uint32 argument3, uint32 guest_stack);
uint32 G3Function8007A2FC(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function800796E0(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007D9EC(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007D818(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007D1E8(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007D4EC(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007DC44(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007D2F0(uint32 start, uint32 end, uint32 state, uint32 mode, uint32 guest_stack);

uint32 G3Function800792BC(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007CF70(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function80079430(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function80078E40(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function80080DE4(uint32 object, uint32 game, uint32 count, uint32 size, uint32 guest_stack);

uint32 G3Function8005E7B4(uint32 output, uint32 box, uint32 point);

uint32 G3Function8005E888(uint32 point, uint32 center, uint32 normal);

uint32 G3Function8005E3C8(uint32 box, uint32 point, uint32 previous, uint32 normal, uint32 guest_stack);

uint32 G3Function8003D4E4(uint32 object, uint32 descriptor, uint32 guest_stack);

uint32 G3Function80079F40(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function80078C04(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8004DB1C(void);

uint32 G3Function80052CE8(uint32 search, uint32 direction);

uint32 G3Function80052BAC(uint32 search, uint32 guest_stack);

uint32 G3Function80079E70(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function800787E0(uint32 value, uint32 target, uint32 step);

uint32 G3Function80079BC0(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function80039040(uint32 guest_stack, uint32 *second_glyph, uint32 *third_glyph);

uint32 G3Function800833EC(uint32 object);

uint32 G3Function8007993C(uint32 object, uint32 game, uint32 state);

uint32 G3Function8007A894(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function80072E6C(uint32 object, uint32 game, uint32 guest_stack);

uint32 G3Function8007A7F0(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function80079A90(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function800836F0(uint32 object, uint32 mode, uint32 scale, uint32 kind, uint32 guest_stack);

uint32 G3Function80074194(uint32 object, uint32 game, uint32 payload, uint32 guest_stack);

uint32 G3Function80015F70(uint32 object, uint32 parameters, uint32 guest_stack);

uint32 G3Function8008004C(uint32 object, uint32 game, uint32 state, uint32 controls);

uint32 G3Function80052454(uint32 destination, uint32 angle, uint32 source, uint32 offset);

uint32 G3Function8003D5E8(uint32 object, uint32 descriptor, uint32 guest_stack);

uint32 G3Function8003D8FC(uint32 object, uint32 descriptor, uint32 guest_stack);

uint32 G3Function8001CE1C(uint32 mesh, uint32 camera, uint32 context, uint32 ordering_table);




uint32 G3Function8008018C(uint32 object, uint32 game, uint32 state, uint32 guest_stack);

uint32 G3Function8007B1A8(uint32 object, uint32 state, uint32 guest_stack);

uint32 G3Function8007B280(uint32 object, uint32 state, uint32 guest_stack);

uint32 G3Function800757B0(uint32 object, uint32 mode, uint32 extent, uint32 kind, uint32 guest_stack);

uint32 G3Function8007597C(uint32 effect, uint32 mode, uint32 parameters, uint32 guest_stack, uint32 incoming_v0);

uint32 G3Function80075924(uint32 effect, uint32 first_color, uint32 second_color, uint32 incoming_v0);

uint32 G3Function80075AC0(uint32 effect, uint32 parameters, uint32 guest_stack);

uint32 G3Function80075D28(uint32 effect, uint32 parameters, uint32 guest_stack);

void G3Function800760C0(void);

uint32 G3Function80012DA0(uint32 guest_stack);

uint32 G3Function80012D38(uint32 game, uint32 guest_stack);

uint32 G3Function80012954(uint32 guest_stack);
uint32 G3Function800129CC(uint32 guest_stack);

uint32 G3Function80012830(uint32 guest_stack);

uint32 G3Function800528F4(uint32 origin, uint32 start_angle, uint32 target_angle, uint32 start_radius, uint32 guest_stack);

uint32 G3Function80052D44(uint32 state, uint32 origin, uint32 target, uint32 pivot, uint32 guest_stack);

uint32 G3Function8005309C(uint32 position, uint32 pivot, uint32 state, uint32 output, uint32 guest_stack);

uint32 G3Function80052E74(uint32 pivot, uint32 point, uint32 settings, uint32 guest_stack);
uint32 G3Function80053E00(uint32 position, uint32 pivot, uint32 state, uint32 output, uint32 guest_stack);

uint32 G3Function80056C10(uint32 projected, uint32 length, uint32 origin, uint32 target, uint32 guest_stack);

uint32 G3Function80056EA0(uint32 output, uint32 start, uint32 end, uint32 delta, uint32 guest_stack);
uint32 G3Function80056D30(uint32 point, uint32 origin, uint32 projected, uint32 normal, uint32 delta, uint32 guest_stack);

uint32 G3Function8005FC70(uint32 output, uint32 start, uint32 end, uint32 normal);

uint32 G3Function8005FE44(uint32 context, uint32 game, uint32 guest_stack);
uint32 G3Function8005FE88(uint32 context, uint32 game, uint32 mask, uint32 guest_stack);
uint32 G3Function8005C1A8(uint32 context, uint32 object, uint32 guest_stack);

uint32 G3Function800715BC(uint32 output);

uint32 G3Function80071640(uint32 object, uint32 guest_stack);

uint32 G3Function80074044(uint32 object, uint32 game, uint32 guest_stack);

uint32 G3Function80074068(uint32 object, uint32 game, uint32 guest_stack);

uint32 G3Function800740BC(uint32 object, uint32 game, uint32 payload, uint32 guest_stack);

uint32 G3Function8007429C(uint32 object, uint32 game, uint32 guest_stack);

uint32 G3Function80074598(uint32 object, uint32 game, uint32 guest_stack);

uint32 G3Function80074490(uint32 object, uint32 game, uint32 guest_stack);

uint32 G3Function80074994(uint32 object, uint32 game, uint32 guest_stack);

uint32 G3Function80074888(uint32 object, uint32 game, uint32 data, uint32 guest_stack);

uint32 G3Function80074600(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function800762F4(uint32 object);

uint32 G3Function80078DE8(uint32 object, uint32 game, uint32 state);

uint32 G3Function8007A708(uint32 object, uint32 game, uint32 state);

uint32 G3Function80072A4C(uint32 object, uint32 game, uint32 state, uint32 guest_stack);

uint32 G3Function8007ACF0(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function80072B0C(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function800760F4(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007AF30(uint32 object, uint32 game, uint32 state);

uint32 G3Function8007AFBC(uint32 object, uint32 game, uint32 state);

uint32 G3Function8007B010(uint32 object, uint32 game, uint32 state, uint32 guest_stack);

uint32 G3Function8007BA50(uint32 object, uint32 game, uint32 state, uint32 controls);

uint32 G3Function8007D284(uint32 object, uint32 game, uint32 state, uint32 controls);

uint32 G3Function80079594(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007A5E8(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007AC3C(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007E7C0(uint32 object, uint32 linked, uint32 guest_stack);

uint32 G3Function8007EB0C(uint32 object, uint32 linked, uint32 guest_stack);

uint32 G3Function8007E6B0(uint32 object, uint32 linked, uint32 guest_stack);

uint32 G3Function8007E92C(uint32 object, uint32 linked, uint32 guest_stack);

uint32 G3Function8007EC48(uint32 object, uint32 linked, uint32 guest_stack);

uint32 G3Function8007E758(uint32 object, uint32 linked, uint32 guest_stack);

uint32 G3Function8007EFF8(uint32 object, uint32 game, uint32 state, uint32 guest_stack);

uint32 G3Function8007F1A4(uint32 object, uint32 guest_stack);

uint32 G3Function8007B618(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function8007B75C(uint32 object, uint32 game, uint32 state, uint32 guest_stack);

uint32 G3Function8007B918(uint32 object, uint32 game, uint32 state, uint32 guest_stack);

uint32 G3Function8007C4AC(uint32 state, uint32 guest_stack);

uint32 G3Function800708FC(uint32 object, uint32 level, uint32 guest_stack);

uint32 G3Function80070524(uint32 object, uint32 state, uint32 guest_stack);

uint32 G3Function800283E8(uint32 controller, uint32 negative_x, uint32 positive_x, uint32 negative_y, uint32 guest_stack);

uint32 G3Function8007BAB8(uint32 object, uint32 game, uint32 state, uint32 guest_stack);

uint32 G3Function8007BE98(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

uint32 G3Function80064BE0(uint32 filename, uint32 destination, uint32 guest_stack);

uint32 G3Function8001FE14(uint32 angles, uint32 output, uint32 guest_stack);

void G3Function8003F5B8(uint32 source_path, uint32 source_state, uint32 target_path, uint32 target_state, uint32 incoming_fraction, uint32 guest_stack);

uint32 G3Function80082F30(uint32 object, uint32 paths, uint32 state, uint32 distance, uint32 incoming_fraction, uint32 guest_stack);

uint32 G3Function800500C0(uint32 direction, uint32 reference, uint32 owner, uint32 guest_stack);

uint32 G3Function800172DC(uint32 context, uint32 command, uint32 guest_stack);

uint32 G3Function8004ABE8(uint32 command, uint32 context, uint32 guest_stack);

uint32 G3Function8004B960(uint32 command, uint32 context, uint32 guest_stack);

void G3DispatchScriptValue(uint32 target, uint32 key, uint32 value, uint32 command, uint32 selector, uint32 guest_stack);

uint32 G3DispatchAudioParameter(uint32 target, uint32 owner, uint32 channel, uint32 parameter, uint32 value, uint32 guest_stack);

uint32 G3Function800188E4(uint32 guest_stack);

uint32 G3Function80018980(uint32 guest_stack);

void G3Function8007AE0C(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);

void G3Function8004AA4C(void);

void G3Function8004AAAC(void);

void G3Function8004ADC4(void);

void G3Function8004ADCC(void);

void G3Function8004AE40(void);

void G3Function8004AF30(void);

void G3Function8004AF38(void);

void G3Function8004B1A4(void);

void G3Function8004B1AC(void);

void G3Function8004B1B4(void);

void G3Function8004B1BC(void);

void G3Function8004B1DC(void);

void G3Function8004B1E4(void);

void G3Function8004B1EC(void);

void G3Function8004B1F4(void);

void G3Function8004B1FC(void);

void G3Function8004B204(void);

void G3Function8004B20C(void);

void G3Function8004B3A0(void);

void G3Function8004B3DC(void);

void G3Function8004B3E4(void);

void G3Function8004B658(void);

void G3Function8004B660(void);

void G3Function8004B668(void);


uint32 G3Function80045A28(uint32 position, uint32 velocity, uint32 lifetime, uint32 red, uint32 creation_gate, uint32 guest_stack);

uint32 G3ReinitializeFileIo80064894(void);

uint32 G3Function800199B0(uint32 packet, uint32 order_table, uint32 face_flags, uint32 material, uint32 blend, uint32 uv0, uint32 uv1, uint32 uv2);
uint32 G3Function80019FC8(uint32 model, uint32 instance, uint32 vertices, uint32 matrix, uint32 pool, uint32 order_table, uint32 guest_stack);
uint32 G3Function80085DFC(uint32 cursor_pointer, uint32 kind_pointer, uint32 data_pointer, uint32 size_pointer);
uint32 G3Function80085D40(uint32 cursor_pointer, uint32 kind, uint32 data_pointer, uint32 size_pointer, uint32 stop_kind, uint32 guest_stack);

// Complete level modules and separately reviewed native helpers
uint32 G3AirpodEvent(uint32 object, uint32 message, uint32 module_base, uint32 guest_stack);
void G3AirpodConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack);
void G3AirpodInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
void G3AirpodUpdate(uint32 object, uint32 guest_stack);
void G3AirpodCollide(uint32 object);
uint32 G3AlfredEvent(uint32 object, uint32 message, uint32 guest_stack);
void G3AlfredConfigure(uint32 object, uint32 commands, uint32 guest_stack);
void G3AlfredInitialize(uint32 object, uint32 guest_stack);
void G3AlfredFacePlayer(uint32 object, uint32 distance_squared, uint32 guest_stack);
uint32 G3AlfredSpeechFrame(uint32 object);
void G3AlfredUpdate(uint32 object, uint32 guest_stack);
void G3AlfredCollide(uint32 object, uint32 guest_stack);
uint32 G3AllwhackEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3AllwhackSetState(uint32 object, uint32 mode, uint32 at_end);
void G3AllwhackInitialize(uint32 object, uint32 game, uint32 module_base);
uint32 G3AllwhackDistanceXY(uint32 object, uint32 position);
void G3AllwhackCommit(uint32 placement);
void G3AllwhackBurst8(uint32 object, uint32 guest_stack);
void G3AllwhackBurst2(uint32 object, uint32 guest_stack);
void G3AllwhackBurst0(uint32 object, uint32 guest_stack);
void G3AllwhackBurst4(uint32 object, uint32 guest_stack);
void G3AllwhackBurst5(uint32 object, uint32 guest_stack);
void G3AllwhackBurst3(uint32 object, uint32 guest_stack);
void G3AllwhackHit(uint32 object, uint32 payload, uint32 guest_stack);
void G3AllwhackChildren(uint32 object, uint32 module_base, uint32 guest_stack);
void G3AllwhackSpawnChild(uint32 object, uint32 resource, uint32 output, uint32 lateral, uint32 guest_stack);
void G3AllwhackCollide(uint32 object, uint32 guest_stack);
void G3AllwhackUpdate(uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3AnimatedBombEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3AnimatedBombInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3AnimatedBombRandom(uint32 bound);
void G3AnimatedBombBurst(uint32 object, uint32 module_base, uint32 guest_stack);
void G3AnimatedBombUpdate(uint32 object, uint32 module_base, uint32 guest_stack);
void G3AnimatedBombCollide(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3AnimatedBombDamage(uint32 object, uint32 module_base, uint32 guest_stack);
void G3AnimatedBombParticleUpdate(uint32 object, uint32 guest_stack);
void G3AnimatedBombParticleCollide(uint32 object, uint32 game, uint32 guest_stack);
void G3AnimatedBombFragmentUpdate(uint32 object, uint32 guest_stack);
void G3AnimatedBombFragmentCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3AniguntEvent(uint32 object, uint32 message, uint32 module_base, uint32 guest_stack);
void G3AniguntSetAnimation(uint32 object, uint32 animation);
void G3AniguntInitialize(uint32 object, uint32 module_base);
void G3AniguntUpdate(uint32 object, uint32 guest_stack);
uint32 G3AniguntPlayerBelow(uint32 object);
uint32 G3AniguntContainsXY(uint32 object, uint32 point);
void G3AniguntUpdateReversible(uint32 object, uint32 guest_stack);
void G3AniguntCollide(uint32 object, uint32 guest_stack);
void G3AniguntUpdateIdle(uint32 object, uint32 guest_stack);
void G3AniguntUpdateActive(uint32 object, uint32 guest_stack);
void G3AniguntAnimateInside(uint32 object, uint32 guest_stack);
void G3AniguntAnimateOutside(uint32 object, uint32 guest_stack);
void G3AniguntShootSpread(uint32 object, uint32 offset, uint32 guest_stack);
void G3AniguntShootBomb(uint32 object, uint32 offset, uint32 guest_stack);
void G3AniguntShootAlternating(uint32 object, uint32 guest_stack);
uint32 G3AttachEvent(uint32 object, uint32 message, uint32 guest_stack);
void G3AttachInitialize(uint32 object, uint32 guest_stack);
void G3AttachUpdate(uint32 object, uint32 guest_stack);
uint32 G3AttachedObject80036FE4(uint32 object, sint32 index);
uint32 G3AttachmentIndex80037028(uint32 object);
uint32 G3ClackerEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack);
void G3ClackerInitialize(uint32 object);
void G3ClackerCollide(uint32 object);
void G3ClackerUpdate(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3ShittrnEvent(uint32 module_base, uint32 object, uint32 message, uint32 guest_stack);
void G3ShittrnUpdate(uint32 module_base, uint32 object, uint32 guest_stack);
uint32 G3BarrelEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3BarrelRestorePosition(uint32 placement);
void G3BarrelInitialize(uint32 object, uint32 state, uint32 settings, uint32 module_base);
uint32 G3BarrelOverlaps(uint32 object, uint32 other, uint32 settings);
uint32 G3BarrelFindLinked(uint32 object, uint32 settings, uint32 module_base);
uint32 G3BarrelMoveAllowed(uint32 object, uint32 delta);
void G3BarrelStopFall(uint32 object, uint32 state);
void G3BarrelRestorePlacements(uint32 game, uint32 module_base);
uint32 G3PlayerAttacksContact800785AC(uint32 contact, uint32 game);
void G3BarrelUpdate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3BarrelCollide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
uint32 G3BirdApproachHeight80036E9C(uint32 speed, uint32 target, uint32 object);
uint32 G3BirdEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3BirdInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3BirdTable(uint32 variant, uint32 mode, uint32 phase, uint32 module_base);
void G3BirdSetMode(uint32 object, uint32 mode, uint32 force, uint32 module_base, uint32 guest_stack);
void G3BirdAnimate(uint32 object, uint32 module_base, uint32 guest_stack);
void G3BirdWander(uint32 object, uint32 guest_stack);
void G3BirdIdle(uint32 object, uint32 guest_stack);
void G3BirdRetreat(uint32 object, uint32 module_base, uint32 guest_stack);
void G3BirdPursue(uint32 object, uint32 guest_stack);
void G3BirdAttack(uint32 object, uint32 guest_stack);
void G3BirdHurt(uint32 object, uint32 guest_stack);
void G3BirdUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
uint32 G3BirdMoveTo(uint32 object, uint32 speed, uint32 turn, uint32 target, uint32 flags, uint32 guest_stack);
uint32 G3BirdTurnTo(uint32 object, uint32 point, uint32 guest_stack);
uint32 G3BirdHomeRange(uint32 object);
uint32 G3BirdAttackRange(uint32 object, uint32 guest_stack);
uint32 G3BirdNear(uint32 object, uint32 point, uint32 radius);
uint32 G3BirdContainsXY(uint32 object, uint32 point);
void G3BirdClampBounds(uint32 object);
void G3BirdCollide(uint32 object, uint32 game, uint32 guest_stack);
void G3BirdFragments(uint32 object, uint32 guest_stack);
void G3BirdDamage(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3BirdPersistentEffect(uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3BoingerEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3PlayerBounceOverride80078728(uint32 player);
uint32 G3BoingerTriangle(uint32 value);
void G3BoingerInitialize(uint32 object, uint32 guest_stack);
void G3BoingerUpdate(uint32 object, uint32 game, uint32 guest_stack);
void G3BoingerCollide(uint32 object, uint32 game, uint32 guest_stack);
void G3BoingerConfigure(uint32 object, uint32 payload);
uint32 G3BomberEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
void G3BomberConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack);
void G3BomberInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
void G3BomberPredictPoint(uint32 output, uint32 factor);
void G3BomberCollide(uint32 object, uint32 module_base, uint32 guest_stack);
void G3BomberUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
uint32 G3BouncerEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3BouncerInitialize(uint32 object);
void G3BouncerSpring(uint32 mode, uint32 position, uint32 target, uint32 velocity, uint32 guest_stack);
void G3BouncerDamage(uint32 object, uint32 payload, uint32 guest_stack);
uint32 G3BouncerCollide(uint32 object, uint32 game, uint32 guest_stack);
void G3BouncerUpdate(uint32 object, uint32 guest_stack);
uint32 G3BrkblokEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3BrkblokPersist(uint32 object);
void G3BrkblokInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
void G3BrkblokUpdate(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3BrkblokDropAbove(uint32 object, uint32 guest_stack);
void G3BrkblokRestorePosition(uint32 placement);
void G3BrkblokRestorePlacements(uint32 game, uint32 module_base);
void G3BrkblokTriangle(uint32 effect, uint32 callback, uint32 storage, uint32 guest_stack);
uint32 G3BrkblokFragmentTick(uint32 effect, uint32 pool, uint32 guest_stack);
void G3BrkblokShatter(uint32 object, uint32 pool, uint32 module_base, uint32 guest_stack);
void G3BrkblokParticles(uint32 object, uint32 module_base, uint32 guest_stack);
void G3BrkblokConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack);
void G3BrkblokCommand(uint32 object, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3BrkblokCollide(uint32 object, uint32 game, uint32 guest_stack);
void G3BrkblokDamage(uint32 object, uint32 game, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3BombTimerEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3BombTimerInitialize(uint32 object, uint32 game, uint32 state, uint32 data);
void G3BombTimerRemainingText(uint32 destination, uint32 format, uint32 value, uint32 guest_stack);
void G3BombTimerUpdate(uint32 object, uint32 game, uint32 state, uint32 data, uint32 module_base, uint32 guest_stack);
void G3BombTimerConfigure(uint32 object, uint32 properties, uint32 module_base, uint32 guest_stack);
uint32 G3BurroEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3BurroInitialize(uint32 object, uint32 game, uint32 state, uint32 guest_stack);
void G3BurroDust(uint32 position, uint32 angle, uint32 scale, uint32 module_base, uint32 guest_stack);
uint32 G3BurroSteer(uint32 input, uint32 object, uint32 state, uint32 settings, uint32 guest_stack);
void G3BurroSpeed(uint32 direction, uint32 state, uint32 settings);
void G3BurroAnimate(uint32 object, uint32 game, uint32 state, uint32 module_base, uint32 guest_stack);
void G3BurroBank(uint32 object, uint32 state, uint32 settings, uint32 guest_stack);
void G3BurroSlope(uint32 object, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3BurroMove(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 guest_stack);
void G3BurroFlags(uint32 game, uint32 state);
void G3BurroSlide(uint32 object, uint32 state, uint32 settings, uint32 guest_stack);
void G3BurroClearKnockback(uint32 state);
void G3BurroJump(uint32 object, uint32 game, uint32 state, uint32 settings);
void G3BurroAttack(uint32 object, uint32 game, uint32 state);
void G3BurroVertical(uint32 object, uint32 state, uint32 settings);
void G3BurroDismount(uint32 object, uint32 game, uint32 guest_stack);
void G3BurroRide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3BurroUpdate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3BurroCollide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 guest_stack);
uint32 G3CactusEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
void G3CactusInitialize(uint32 object);
uint32 G3CactusMoveToward(uint32 object, uint32 target, uint32 speed, uint32 guest_stack);
void G3CactusUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3CactusCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3CamelEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3CamelInitialize(uint32 object, uint32 game, uint32 state, uint32 guest_stack);
void G3CamelDust(uint32 position, uint32 heading, uint32 speed, uint32 module_base, uint32 guest_stack);
uint32 G3CamelSteer(uint32 game, uint32 object, uint32 state, uint32 settings, uint32 guest_stack);
void G3CamelSpeed(uint32 input, uint32 state, uint32 settings);
void G3CamelAnimate(uint32 object, uint32 game, uint32 state, uint32 module_base, uint32 guest_stack);
void G3CamelBank(uint32 object, uint32 state, uint32 settings, uint32 guest_stack);
void G3CamelAlign(uint32 object, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3CamelMove(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3CamelFooting(uint32 state);
void G3CamelFollowPlatform(uint32 object, uint32 state, uint32 settings, uint32 guest_stack);
void G3CamelReact(uint32 object, uint32 game, uint32 state);
void G3CamelRequestJump(uint32 object, uint32 game, uint32 state, uint32 settings);
void G3CamelGround(uint32 object, uint32 state, uint32 settings);
void G3CamelReleaseAttachment(uint32 game, uint32 guest_stack);
void G3CamelRide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3CamelUpdate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3CamelCollide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 guest_stack);
void G3CamelDamage(uint32 object, uint32 game, uint32 state, uint32 guest_stack);
uint32 G3CanbalbEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
void G3CanbalbInitialize(uint32 object);
void G3CanbalbTrail(uint32 object, uint32 guest_stack);
void G3CanbalbUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3CanbalbExplode(uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3CanbalbCollide(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3CanbalbScan(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
uint32 G3CannonBallEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
void G3CannonBallStop(uint32 object);
void G3CannonBallInitialize(uint32 object, uint32 game, uint32 guest_stack);
void G3CannonBallUpdate(uint32 object, uint32 guest_stack);
void G3CannonBallCollide(uint32 object, uint32 game, uint32 guest_stack);
void G3CannonBallSetSpin(uint32 object, uint32 value);
void G3CannonBallSetAxis(uint32 object, uint32 value);
void G3CannonBallSetVelocity(uint32 object, uint32 vector);
void G3CannonBallSetAcceleration(uint32 object, uint32 vector);
void G3CannonBallSetPolarVelocity(uint32 object, uint32 parameters, uint32 guest_stack);
void G3CannonBallSetCylindricalVelocity(uint32 object, uint32 parameters, uint32 guest_stack);
uint32 G3ChickenEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
void G3ChickenSetState(uint32 object, uint32 mode);
void G3ChickenInitialize(uint32 object);
void G3ChickenClamp(uint32 object);
uint32 G3ChickenCanMove(uint32 object, uint32 x, uint32 y);
uint32 G3ChickenNearXY(uint32 object, uint32 point, uint32 radius);
void G3ChickenWander(uint32 object, uint32 guest_stack);
uint32 G3ChickenIdle(uint32 object, uint32 guest_stack);
uint32 G3ChickenHitState(uint32 object, uint32 guest_stack);
void G3ChickenFlee(uint32 object, uint32 guest_stack);
uint32 G3ChickenFleeState(uint32 object, uint32 guest_stack);
void G3ChickenUpdate(uint32 object, uint32 game, uint32 guest_stack);
void G3ChickenSpawnFeathers(uint32 object, uint32 module_base, uint32 guest_stack);
void G3ChickenHit(uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3ChunkEvent(uint32 object, uint32 message, uint32 module_base, uint32 guest_stack);
void G3ChunkInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
void G3ChunkUpdate(uint32 object, uint32 guest_stack);
void G3ChunkNoOperation(void);
void G3ChunkConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack);
uint32 G3ChunkyEvent(uint32 object, uint32 message, uint32 module_base, uint32 guest_stack);
void G3ChunkyInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
void G3ChunkyScatter(uint32 object, uint32 guest_stack);
void G3ChunkyScatterSame(uint32 object, uint32 guest_stack);
void G3ChunkyUpdate(uint32 object, uint32 guest_stack);
void G3ChunkyDestroy(uint32 object, uint32 guest_stack);
void G3ChunkySplit(uint32 object, uint32 guest_stack);
void G3ChunkyCollide(uint32 object, uint32 guest_stack);
uint32 G3ChunkyRender(uint32 camera, uint32 object, uint32 packets, uint32 guest_stack);
uint32 G3ChunkyRenderFaces(uint32 object, uint32 segment, uint32 packets, uint32 model, uint32 guest_stack);
uint32 G3CoinEvent(uint32 module_base, uint32 object, uint32 game, uint32 message, uint32 guest_stack);
void G3CoinInitialize(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3CoinCollect(uint32 module_base, uint32 object, uint32 game, uint32 guest_stack);
void G3CoinCollide(uint32 module_base, uint32 object, uint32 game, uint32 guest_stack);
void G3CoinUpdate(uint32 object, uint32 game, uint32 guest_stack);
void G3BlinkCollectedObject800389E0(uint32 object, sint32 duration, sint32 counter);
uint32 G3CoverEvent(uint32 module_base, uint32 object, uint32 message, uint32 guest_stack);
void G3CoverInitialize(uint32 module_base, uint32 object);
void G3CoverUpdate(uint32 object, uint32 guest_stack);
uint32 G3CoverRender(uint32 scene, uint32 object, uint32 vertices, uint32 packets, uint32 guest_stack);
uint32 G3CowboyEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3CowboyInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
void G3CowboyShoot(uint32 object, uint32 game, uint32 offset, uint32 bone, uint32 guest_stack);
void G3CowboyCollide(uint32 object, uint32 game, uint32 guest_stack);
void G3CowboyUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
uint32 G3CowboyBEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3CowboyBInitialize(uint32 object, uint32 module_base);
void G3CowboyBCollide(uint32 object, uint32 game, uint32 guest_stack);
void G3CowboyBUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
uint32 G3CrumblingPlatformEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
void G3CrumblingPlatformInitialize(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3CrumblingPlatformMovePath(uint32 object, uint32 guest_stack);
void G3CrumblingPlatformUpdate(uint32 object, uint32 game, uint32 guest_stack);
void G3CrumblingPlatformCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3CountdownEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3CountdownPlacement(uint32 intro);
uint32 G3CountdownRecalculate(uint32 state);
void G3CountdownInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
void G3CountdownUpdate(uint32 object, uint32 game, uint32 guest_stack);
void G3CountdownConfigure(uint32 object, uint32 properties, uint32 module_base, uint32 guest_stack);
uint32 G3DoorEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack);
void G3DoorInitialize(uint32 object, uint32 guest_stack);
void G3DoorOpenNegative(uint32 object, uint32 pivot_xy, uint32 pivot_z, uint32 guest_stack);
void G3DoorClosePositive(uint32 object, uint32 pivot_xy, uint32 pivot_z, uint32 guest_stack);
void G3DoorOpenPositive(uint32 object, uint32 pivot_xy, uint32 pivot_z, uint32 guest_stack);
void G3DoorCloseNegative(uint32 object, uint32 pivot_xy, uint32 pivot_z, uint32 guest_stack);
void G3DoorUpdate(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3EbfireEvent(uint32 object, uint32 message, uint32 module_base, uint32 guest_stack);
void G3EbfireInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
void G3EbfireUpdate(uint32 object, uint32 guest_stack);
uint32 G3EbfireDistance(uint32 object);
void G3EbfireRotate(uint32 object, uint32 guest_stack);
void G3EbfireSpawn(uint32 object, uint32 guest_stack);
void G3EbfireVector(uint32 object, uint32 vector, uint32 guest_stack);
uint32 G3EgenEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3EgenConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack);
void G3EgenInitialize(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3EgenUpdate(uint32 object, uint32 game, uint32 guest_stack);
void G3EgenChildReleased(uint32 object);
void G3EgenCommand(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack);
uint32 G3FallgenEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack);
void G3FallgenInitialize(uint32 object, uint32 guest_stack);
uint32 G3FallgenSpawn(uint32 object, uint32 resource, uint32 player, uint32 guest_stack);
void G3FallgenUpdate(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3FallingObjectEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3FallingObjectConfigure(uint32 state, uint32 commands, uint32 module_base, uint32 guest_stack);
void G3FallingObjectInitialize(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3FallingObjectEmit(uint32 position, uint32 resource, uint32 module_base, uint32 guest_stack);
void G3FallingObjectUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3FallingObjectCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3FanEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
uint32 G3FanEngage(uint32 object);
uint32 G3FanDisengage(uint32 object);
void G3FanRelease(uint32 object);
void G3FanInitialize(uint32 object);
void G3FanFinish(void);
void G3FanUpdate(uint32 object, uint32 game, uint32 guest_stack);
void G3FanEmit(uint32 object, uint32 state, uint32 index, uint32 guest_stack);
void G3FanPlayerParticles(uint32 object, uint32 game, uint32 state, uint32 position, uint32 guest_stack);
void G3FanAffectPlayer(uint32 object, uint32 game, uint32 state, uint32 guest_stack);
uint32 G3FishEvent(uint32 object, uint32 message, uint32 guest_stack);
void G3FishSelect(uint32 object, uint32 mode);
void G3FishInitialize(uint32 object, uint32 guest_stack);
uint32 G3FishInsideXY(uint32 object, uint32 dx, uint32 dy);
void G3FishEvade(uint32 object, uint32 guest_stack);
uint32 G3FishIdle(uint32 object, uint32 guest_stack);
uint32 G3FishAlert(uint32 object, uint32 guest_stack);
uint32 G3FishDead(uint32 object, uint32 guest_stack);
void G3FishUpdate(uint32 object, uint32 guest_stack);
void G3FishDamage(uint32 object, uint32 guest_stack);
void G3FishClamp(uint32 object);
uint32 G3FishNearTarget(uint32 object, uint32 target, uint32 radius);
void G3FishRoam(uint32 object, uint32 guest_stack);
uint32 G3LogoEvent(uint32 module_base, uint32 object, uint32 game, uint32 message, uint32 guest_stack);
void G3LogoInitialize(uint32 module_base, uint32 object, uint32 game);
void G3LogoUpdate(uint32 module_base, uint32 object, uint32 game, uint32 guest_stack);
void G3LogoPosition(uint32 module_base, uint32 object, uint32 game, uint32 guest_stack);
uint32 G3LogoRender(uint32 object, uint32 vertices, uint32 packets, uint32 guest_stack);
uint32 G3IntroEvent(uint32 module_base, uint32 object, uint32 message, uint32 guest_stack);
void G3IntroInitialize(uint32 object);
void G3IntroPhaseZero(uint32 object, uint32 guest_stack);
void G3IntroPhaseOne(uint32 module_base, uint32 object, uint32 guest_stack);
void G3IntroPhaseTwo(uint32 object, uint32 guest_stack);
uint32 G3FunPlatformEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
void G3FunPlatformInitialize(uint32 object, uint32 game, uint32 guest_stack);
void G3FunPlatformMovePath(uint32 object, uint32 path, uint32 cursor, uint32 output, uint32 guest_stack);
void G3FunPlatformUpdate(uint32 object, uint32 game, uint32 guest_stack);
void G3FunPlatformCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3EffectGeneratorEvent(uint32 object, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3EffectGeneratorShutdown(uint32 object, uint32 guest_stack);
void G3EffectGeneratorInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3EffectGeneratorOscillate(uint32 effect, uint32 pool, uint32 guest_stack);
void G3EffectGeneratorEmit(uint32 object, uint32 resource, uint32 module_base, uint32 guest_stack);
void G3EffectGeneratorUpdate(uint32 object, uint32 module_base, uint32 guest_stack);
void G3EffectGeneratorTrigger(uint32 object, uint32 payload);
uint32 G3GatgunEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
void G3GatgunInitialize(uint32 object, uint32 state);
uint32 G3GatgunMoveToward(uint32 output, uint32 first, uint32 second, uint32 step);
uint32 G3GatgunTurnToward(uint32 output, uint32 first, uint32 second, uint32 step);
void G3GatgunUpdateRotation(uint32 object, uint32 state, uint32 guest_stack);
void G3GatgunReadControls(uint32 game, uint32 state, uint32 controls);
void G3GatgunShoot(uint32 object, uint32 game, uint32 controls, uint32 module_base, uint32 guest_stack);
void G3GatgunCollide(uint32 object, uint32 game);
void G3GatgunUpdate(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 module_base, uint32 guest_stack);
uint32 G3GatorEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3GatorInitialize(uint32 object, uint32 game, uint32 state, uint32 guest_stack);
uint32 G3GatorSteer(uint32 input, uint32 object, uint32 state, uint32 settings, uint32 guest_stack);
void G3GatorSpeed(uint32 direction, uint32 state, uint32 settings);
void G3GatorAnimate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3GatorBank(uint32 object, uint32 state, uint32 settings, uint32 guest_stack);
void G3GatorSlope(uint32 object, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3GatorMove(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3GatorPath(uint32 object, uint32 state, uint32 guest_stack);
void G3GatorFlags(uint32 object, uint32 game, uint32 state);
void G3GatorKnockback(uint32 object, uint32 game, uint32 state, uint32 settings);
void G3GatorAttack(uint32 object, uint32 game, uint32 state, uint32 settings);
void G3GatorVertical(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 guest_stack);
void G3GatorRide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3GatorUpdate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3GatorCollide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3GatorReverseCollision(uint32 object, uint32 game, uint32 state, uint32 payload, uint32 guest_stack);
void G3GatorSplash800458F8(uint32 position, uint32 guest_stack);
uint32 G3GangCannonEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3GangCannonConfigure(uint32 object, uint32 properties, uint32 guest_stack);
void G3GangCannonInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
void G3GangCannonEnable(uint32 object, uint32 game, uint32 guest_stack);
void G3GangCannonDisable(uint32 object, uint32 game, uint32 guest_stack);
void G3GangCannonUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3GangCannonCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3GuyEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3GuyInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3GuyTable(uint32 variant, uint32 mode, uint32 phase, uint32 module_base);
void G3GuySetMode(uint32 object, uint32 mode, uint32 force, uint32 module_base, uint32 guest_stack);
void G3GuyDestroy(uint32 object, uint32 guest_stack);
void G3GuyAnimate(uint32 object, uint32 module_base, uint32 guest_stack);
void G3GuyIdle(uint32 object, uint32 guest_stack);
void G3GuyApproach(uint32 object, uint32 guest_stack);
void G3GuySpawnProjectile(uint32 object, uint32 guest_stack);
void G3GuyRanged(uint32 object, uint32 guest_stack);
void G3GuyMelee(uint32 object, uint32 guest_stack);
void G3GuyPlayerContact(uint32 object, uint32 game, uint32 guest_stack);
void G3GuyHurt(uint32 object, uint32 module_base, uint32 guest_stack);
void G3GuyChase(uint32 object, uint32 guest_stack);
void G3GuyEndTornado(uint32 object, uint32 module_base, uint32 guest_stack);
void G3GuyTornadoContact(uint32 object, uint32 game, uint32 guest_stack);
void G3GuyWanderTornado(uint32 object, uint32 module_base, uint32 guest_stack);
void G3GuyActiveTornado(uint32 object, uint32 module_base, uint32 guest_stack);
void G3GuyHurtTornado(uint32 object, uint32 module_base, uint32 guest_stack);
void G3GuyShrink(uint32 object, uint32 guest_stack);
void G3GuyUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
uint32 G3GuyContains(uint32 object, uint32 point);
void G3GuyClampBounds(uint32 object);
uint32 G3GuyNear(uint32 object, uint32 point, uint32 radius);
void G3GuyTurn(uint32 object, uint32 target, uint32 step);
uint32 G3GuyAngleTo(uint32 object, uint32 point, uint32 guest_stack);
uint32 G3GuyDistance(uint32 object, uint32 point);
void G3GuyDamage(uint32 object, uint32 module_base, uint32 guest_stack);
void G3GuyCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3GxSpitEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
void G3GxSpitInitialize(uint32 object);
void G3GxSpitTrail(uint32 object, uint32 guest_stack);
void G3GxSpitUpdate(uint32 object, uint32 guest_stack);
void G3GxSpitDefaultCollide(uint32 object, uint32 game, uint32 guest_stack);
void G3GxSpitIceCollide(uint32 object, uint32 game, uint32 guest_stack);
void G3GxSpitFireCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3HankEvent0000(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3HankNextField0140(uint32 cursor, uint32 remaining, uint32 guest_stack);
void G3HankLoadCore01CC(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack);
void G3HankLoadIdle0584(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack);
void G3HankLoadPursuit08A4(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack);
void G3HankLoadAttack0B30(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack);
void G3HankLoadLeap0C9C(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack);
void G3HankLoadSpecial0FCC(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack);
void G3HankLoadShooter1364(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 module_base, uint32 guest_stack);
void G3HankLoadHurt15E4(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack);
void G3HankLoadMove178C(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack);
void G3HankLoadRun1BCC(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack);
void G3HankLoadDeath1EEC(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack);
void G3HankLoadAnimationList2348(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack);
void G3HankLoadBounds240C(uint32 state, uint32 settings, uint32 cursor, uint32 remaining, uint32 guest_stack);
uint32 G3HankLoadConfiguration2564(uint32 state, uint32 resource, uint32 settings, uint32 bytes, uint32 module_base, uint32 guest_stack);
void G3HankSelectAnimation27EC(uint32 state, uint32 animation, uint32 preserve, uint32 guest_stack);
void G3HankUpdateAnimation2A1C(uint32 state, uint32 selector, uint32 completed, uint32 guest_stack);
uint32 G3HankAnimationReady2CC4(uint32 state);
void G3HankFinishAnimation2CFC(uint32 state);
uint32 G3HankTraceVertical2D28(uint32 state, uint32 settings, uint32 floor, uint32 relative_floor, uint32 guest_stack);
uint32 G3HankSnapFloor2DFC(uint32 state, uint32 guest_stack);
uint32 G3HankUpdateFloor2E68(uint32 state, uint32 module_base, uint32 guest_stack);
void G3HankEnterAttached3020(uint32 state, uint32 guest_stack);
void G3HankEnterAttachment308C(uint32 state, uint32 guest_stack);
void G3HankEnterPursuit33BC(uint32 state, uint32 guest_stack);
void G3HankEnterAlert3404(uint32 state, uint32 guest_stack);
void G3HankEnterAttack3440(uint32 state, uint32 guest_stack);
void G3HankEnterIdle34B0(uint32 state, uint32 guest_stack);
void G3HankEnterLeap356C(uint32 state, uint32 guest_stack);
void G3HankEnterSpecial35A4(uint32 state, uint32 guest_stack);
void G3HankEnterFall35DC(uint32 state, uint32 guest_stack);
void G3HankEnterMove3614(uint32 state, uint32 guest_stack);
void G3HankEnterStop3654(uint32 state, uint32 guest_stack);
void G3HankEnterTurn3688(uint32 state, uint32 guest_stack);
void G3HankEnterWait36BC(uint32 state, uint32 guest_stack);
void G3HankEnterRun36F4(uint32 state, uint32 guest_stack);
void G3HankEnterPause372C(uint32 state, uint32 guest_stack);
void G3HankEnterHurt3760(uint32 state, uint32 guest_stack);
void G3HankEnterKnockback3794(uint32 state, uint32 direction, uint32 guest_stack);
void G3HankEnterDying37E0(uint32 state, uint32 guest_stack);
void G3HankEnterDeath381C(uint32 state, uint32 guest_stack);
void G3HankEnterDeathTail3850(uint32 state, uint32 guest_stack);
void G3HankEnterRemoved3884(uint32 state);
void G3HankChooseInitial3898(uint32 state, uint32 guest_stack);
void G3HankChooseHurt3994(uint32 state, uint32 forced, uint32 guest_stack);
void G3HankInitialize3A74(uint32 object, uint32 module_base, uint32 guest_stack);
void G3HankNotifyEnable4150(uint32 owner, uint32 guest_stack);
void G3HankNotifyDisable41A4(uint32 owner, uint32 guest_stack);
uint32 G3HankClampBounds4204(uint32 state, uint32 bounds);
void G3HankSpawnDeath428C(uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3HankChooseDeath4520(uint32 state, uint32 primary, uint32 secondary, uint32 module_base, uint32 guest_stack);
uint32 G3HankAnimationMatches45E8(uint32 state, uint32 count, uint32 ranges);
uint32 G3HankProbeMove4720(uint32 state, uint32 distance, uint32 bounds, uint32 module_base, uint32 guest_stack);
uint32 G3HankDistanceSquared47F8(uint32 state, uint32 vector);
uint32 G3HankAtOrigin486C(uint32 state, uint32 include_height);
void G3HankAdvanceChannel48BC(uint32 channel, uint32 initial, uint32 count, uint32 output, uint32 guest_stack);
uint32 G3HankTurnToward49B4(uint32 angle, uint32 target, uint32 step, uint32 tolerance, uint32 guest_stack);
void G3HankUpdateAttached4AA8(uint32 state, uint32 module_base, uint32 guest_stack);
void G3HankUpdateAttachment5000(uint32 state, uint32 module_base, uint32 guest_stack);
void G3HankUpdatePursuit5284(uint32 state, uint32 module_base, uint32 guest_stack);
void G3HankUpdateAlert5878(uint32 state, uint32 module_base, uint32 guest_stack);
void G3HankChooseAfterAnimation5A9C(uint32 state, uint32 guest_stack);
void G3HankUpdateWait5C44(uint32 state, uint32 module_base, uint32 guest_stack);
void G3HankUpdateIdle5CB0(uint32 state, uint32 module_base, uint32 guest_stack);
void G3HankUpdateShooter60C8(uint32 state, uint32 game, uint32 module_base, uint32 guest_stack);
void G3HankPlayerOffset6388(uint32 output, uint32 scale, uint32 guest_stack);
void G3HankUpdateLeap6460(uint32 state, uint32 game, uint32 module_base, uint32 guest_stack);
void G3HankUpdateFall6828(uint32 state, uint32 game, uint32 module_base, uint32 guest_stack);
void G3HankUpdateMove69F4(uint32 state, uint32 module_base, uint32 guest_stack);
void G3HankUpdateStop6E40(uint32 state, uint32 module_base, uint32 guest_stack);
void G3HankUpdateTurn7018(uint32 state, uint32 module_base, uint32 guest_stack);
void G3HankUpdateReturn7140(uint32 state, uint32 module_base, uint32 guest_stack);
void G3HankUpdateRun7394(uint32 state, uint32 module_base, uint32 guest_stack);
void G3HankUpdatePause7824(uint32 state, uint32 module_base, uint32 guest_stack);
void G3HankUpdateHurt7A48(uint32 state, uint32 guest_stack);
void G3HankUpdateKnockback7A8C(uint32 state, uint32 guest_stack);
void G3HankUpdateDying7B90(uint32 state, uint32 module_base, uint32 guest_stack);
void G3HankUpdateDeath7CC0(uint32 state, uint32 module_base, uint32 guest_stack);
void G3HankUpdateDeathTail7D10(uint32 state, uint32 module_base, uint32 guest_stack);
void G3HankUpdateRemoved7D44(uint32 state, uint32 guest_stack);
void G3HankUpdate7E08(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3HankDamage80F4(uint32 object, uint32 module_base, uint32 guest_stack);
void G3HankContact81E8(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3HankCommand8458(uint32 object, uint32 command, uint32 module_base);
uint32 G3HitchupEvent(uint32 object, uint32 message, uint32 guest_stack);
void G3HitchupConfigure(uint32 object, uint32 commands, uint32 guest_stack);
void G3HitchupInitialize(uint32 object, uint32 guest_stack);
void G3HitchupUpdate(uint32 object, uint32 guest_stack);
void G3HitchupCollide(void);
uint32 G3IceCubeEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
void G3IceCubeInitialize(uint32 object);
void G3IceCubeRestore(uint32 object);
uint32 G3IceCubeDetach(uint32 object, uint32 guest_stack);
void G3IceCubeUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3IceCubeBreak(uint32 object, uint32 game, uint32 pieces, uint32 module_base, uint32 guest_stack);
void G3IceCubeCollide(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
uint32 G3KangaEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3KangaInitialize(uint32 object, uint32 game, uint32 state, uint32 guest_stack);
void G3KangaDust(uint32 position, uint32 heading, uint32 speed, uint32 module_base, uint32 guest_stack);
uint32 G3KangaSteer(uint32 game, uint32 object, uint32 state, uint32 settings, uint32 guest_stack);
void G3KangaSpeed(uint32 heading, uint32 state, uint32 settings);
void G3KangaAnimate(uint32 object, uint32 game, uint32 state, uint32 module_base, uint32 guest_stack);
void G3KangaMove(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3KangaFooting(uint32 game, uint32 state);
void G3KangaFollowPlatform(uint32 object, uint32 state, uint32 settings, uint32 guest_stack);
void G3KangaReact(uint32 object, uint32 state);
void G3KangaRequestJump(uint32 object, uint32 game, uint32 state, uint32 settings);
void G3KangaRequestAttack(uint32 object, uint32 game, uint32 state);
void G3KangaJump(uint32 object, uint32 state, uint32 settings, uint32 module_base);
void G3KangaRide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3KangaUpdate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3KangaCollide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 guest_stack);
uint32 G3PlayerLauncherState80077578(uint32 object);
uint32 G3LauncherEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
void G3LauncherConfigure(uint32 object, uint32 commands, uint32 guest_stack);
void G3LauncherInitialize(uint32 object, uint32 guest_stack);
void G3LauncherUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3LauncherCollide(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3LauncherPrepare(uint32 object, uint32 game);
uint32 G3LauncherHasBusyPeer(uint32 object, uint32 module_base);
void G3LauncherStartJump(uint32 object, uint32 game, uint32 guest_stack);
void G3LauncherStartLift(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3LeafGeneratorEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3LeafGeneratorInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3LeafEffectInitialize(uint32 effect, uint32 callback, uint32 storage, uint32 guest_stack);
uint32 G3LeafEffectUpdate(uint32 effect, uint32 pool, uint32 guest_stack);
void G3LeafGeneratorEmit(uint32 object, uint32 position, uint32 module_base, uint32 guest_stack);
void G3LeafGeneratorUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3LeafGeneratorCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3LennyBEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
void G3LennyBConfigure(uint32 object, uint32 commands, uint32 guest_stack);
void G3LennyBInitialize(uint32 object, uint32 guest_stack);
void G3LennyBPredictPoint(uint32 output, uint32 factor);
uint32 G3LennyBActivate(uint32 object);
void G3LennyBUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3LennyBCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3LitballEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
void G3LitballConfigure(uint32 state, uint32 commands, uint32 module_base, uint32 guest_stack);
void G3LitballInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
void G3LitballUpdate(uint32 object, uint32 game, uint32 guest_stack);
void G3LitballCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3LookyEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
void G3LookyInitialize(uint32 object, uint32 module_base);
void G3LookyUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3LookyPosition(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
uint32 G3LookyRender(uint32 object, uint32 vertices, uint32 packets, uint32 guest_stack);
uint32 G3MissileEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3MissileOrientation(uint32 output, uint32 heading, uint32 pitch, uint32 guest_stack);
void G3MissileInitialize(uint32 object);
void G3MissileTrajectory(uint32 object, uint32 guest_stack);
void G3MissileExplode(uint32 object, uint32 guest_stack);
void G3MissileUpdate(uint32 object, uint32 guest_stack);
void G3MissileCollide(uint32 object, uint32 guest_stack);
uint32 G3MonsterEntry0000(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3MonsterEvent0020(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3MonsterInitialize024C(uint32 object, uint32 module_base, uint32 guest_stack);
void G3MonsterNoop0604();
void G3MonsterUpdate060C(uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3MonsterValidate0938(uint32 object);
void G3MonsterNoop0A30();
void G3MonsterUpdateCounters0A38(uint32 counters, uint32 state, uint32 object, uint32 guest_stack);
uint32 G3MonsterFindProperty0C60(uint32 resource, uint32 name, uint32 module_base);
void G3MonsterRunSequence0E7C(uint32 object, uint32 model, uint32 guest_stack);
void G3MonsterRunProperty0EB4(uint32 object, uint32 name, uint32 module_base, uint32 guest_stack);
void G3MonsterCaptureState0EF8(uint32 snapshot, uint32 object, uint32 position);
uint32 G3MonsterShouldTransition0F7C(uint32 state, uint32 model, uint32 configuration, uint32 module_base, uint32 guest_stack);
void G3MonsterCopyStateVector117C(uint32 state, uint32 output, uint32 guest_stack);
uint32 G3MonsterCheckRange1278(uint32 object, uint32 guest_stack);
uint32 G3MonsterUpdateRange1408(uint32 object, uint32 guest_stack);
void G3MonsterPackCoordinates149C(uint32 source, uint32 destination);
uint32 G3MonsterPackedDistance14C0(uint32 position, uint32 packed, uint32 guest_stack);
void G3MonsterUpdateStall1550(uint32 state, uint32 position, uint32 guest_stack);
void G3MonsterMoveList1634(uint32 object, uint32 index);
uint32 G3MonsterSelectValue1684(uint32 object);
void G3MonsterResume16CC(uint32 object, uint32 guest_stack);
void G3MonsterSuspend1710(uint32 object, uint32 guest_stack);
void G3MonsterMoveDefault1730(uint32 object, uint32 guest_stack);
void G3MonsterMoveReduced1780(uint32 object, uint32 guest_stack);
uint32 G3MonsterClearLine17E0(uint32 start, uint32 end, uint32 guest_stack);
void G3MonsterUpdateLineFlag1874(uint32 start, uint32 end, uint32 state, uint32 guest_stack);
void G3MonsterStaggerLine18C0(uint32 start, uint32 end, uint32 state, uint32 phase, uint32 guest_stack);
void G3MonsterInitializeChain191C(uint32 descriptor, uint32 model, uint32 module_base);
void G3MonsterQuarterTurn19A8(uint32 vector);
uint32 G3MonsterResolveAnimation19C0(uint32 animation, uint32 descriptor);
void G3MonsterValidateAnimations1A30(uint32 descriptor, uint32 model);
void G3MonsterClearBytes1ABC(uint32 destination);
void G3MonsterClearPointers1AEC(uint32 destination);
void G3MonsterBindAnimations1B24(uint32 model, uint32 module_base);
void G3MonsterClearCommands2060(uint32 table);
void G3MonsterBuildDefaults20B0(uint32 model);
void G3MonsterDefaultPointers2238(uint32 table);
void G3MonsterValidateGroup229C(uint32 group, uint32 descriptor);
void G3MonsterValidateGroups231C(uint32 descriptor, uint32 model);
void G3MonsterInitializeResource2380(uint32 descriptor, uint32 model, uint32 module_base);
uint32 G3MonsterLookup23EC(uint32 kind, uint32 first_index, uint32 second_index, uint32 table);
uint32 G3MonsterAdvanceBounded244C(uint32 step, uint32 position, uint32 settings, uint32 scale);
void G3MonsterSampleMotion2524(uint32 animation, uint32 time, uint32 output, uint32 guest_stack);
void G3MonsterScaleVector26A0(uint32 vector, uint32 scale);
void G3MonsterAnimationMotion271C(uint32 animation, uint32 output, uint32 guest_stack);
void G3MonsterLookupMotion2784(uint32 state, uint32 output, uint32 table, uint32 guest_stack);
uint32 G3MonsterSelectAnimation27EC(uint32 phase, uint32 output, uint32 state, uint32 model);
void G3MonsterProjectSeparation2888(uint32 output, uint32 vector, uint32 normal, uint32 guest_stack);
uint32 G3MonsterFacingContact2A84(uint32 rotation, uint32 vector, uint32 guest_stack);
void G3MonsterDeflectVelocity2B34(uint32 velocity, uint32 delta, uint32 guest_stack);
uint32 G3MonsterClassifyContact2C0C(uint32 output, uint32 contact, uint32 side);
void G3MonsterBuildDamagePacket2CA0(uint32 packet, uint32 object, uint32 target_position, uint32 contact, uint32 guest_stack);
void G3MonsterAttackContact2DA8(uint32 own_class, uint32 other_class, uint32 contact, uint32 object, uint32 guest_stack);
void G3MonsterSeparateContact2F1C(uint32 contact, uint32 object, uint32 guest_stack);
void G3MonsterCollide313C(uint32 object, uint32 guest_stack);
void G3MonsterInitializePacket31F8(uint32 packet);
uint32 G3MonsterCheckFlags322C(uint32 state, uint32 model);
uint32 G3MonsterDamage3284(uint32 object, uint32 packet, uint32 guest_stack);
uint32 G3MonsterRequestRemoval35FC(uint32 object, uint32 guest_stack);
void G3MonsterTraceFloor3650(uint32 output, uint32 object, uint32 top, uint32 bottom, uint32 guest_stack);
void G3MonsterBoxFloor3750(uint32 output, uint32 object, uint32 module_base, uint32 guest_stack);
void G3MonsterQueryFloor37D8(uint32 output, uint32 object, uint32 top, uint32 bottom, uint32 module_base, uint32 guest_stack);
void G3MonsterQueryTerrain3820(uint32 output, uint32 object, uint32 position, uint32 bottom, uint32 guest_stack);
void G3MonsterCaptureFloor3908(uint32 object, uint32 guest_stack);
void G3MonsterResolveFloor39C8(uint32 object, uint32 model, uint32 module_base, uint32 guest_stack);
uint32 G3MonsterClampBox3C08(uint32 position, uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3MonsterTraceHorizontal3D8C(uint32 position, uint32 previous, uint32 guest_stack);
void G3MonsterResolveHorizontal3DE4(uint32 object, uint32 radius, uint32 height, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MonsterResolveSides3F14(uint32 object, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MonsterPrePhysics3F9C(uint32 object, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MonsterPostPhysics4094(uint32 object, uint32 model, uint32 module_base, uint32 guest_stack);
uint32 G3MonsterTransition414C(uint32 object, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MonsterPrepareState4230(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MonsterPrepareMode42E0(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MonsterDispatchMode43B4(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MonsterActiveMode43FC(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MonsterIdleMode447C(uint32 state);
void G3MonsterDispatchCommand4520(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MonsterClearFlag4660(uint32 state);
uint32 G3MonsterApplyTransition4678(uint32 state, uint32 configuration, uint32 model, uint32 object, uint32 module_base, uint32 guest_stack);
void G3MonsterSelectState4A9C(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MonsterResetCommand4FEC(uint32 command, uint32 state);
void G3MonsterSwitchCommand5008(uint32 command, uint32 state, uint32 model, uint32 guest_stack);
void G3MonsterSetCommand509C(uint32 command);
void G3MonsterSwitchCommand50A8(uint32 command);
void G3MonsterSwitchCommand50CC(uint32 command);
void G3MonsterSwitchCommand50F0(uint32 command, uint32 state);
void G3MonsterSwitchCommand514C(uint32 command, uint32 state);
void G3MonsterSwitchCommand51E4(uint32 command, uint32 state);
void G3MonsterSwitchCommand524C(uint32 command);
void G3MonsterSwitchCommand528C(uint32 command, uint32 state, uint32 model, uint32 flags, uint32 guest_stack);
void G3MonsterSetCommand54B4(uint32 command);
void G3MonsterSetCommand54C0(uint32 command);
void G3MonsterSwitchCommand54CC(uint32 command, uint32 model, uint32 guest_stack);
uint32 G3MonsterAngleBand56D4(uint32 angle);
uint32 G3MonsterSelectAlternate5718(uint32 state, uint32 snapshot, uint32 table, uint32 difference, uint32 guest_stack);
uint32 G3MonsterCheckAdvance5870(uint32 state, uint32 snapshot, uint32 model, uint32 health, uint32 guest_stack);
uint32 G3MonsterCheckRetreat5994(uint32 state, uint32 snapshot, uint32 model, uint32 health, uint32 guest_stack);
uint32 G3MonsterCheckSpecial5AB0(uint32 state, uint32 snapshot, uint32 model, uint32 health, uint32 guest_stack);
uint32 G3MonsterCheckAlternate5B0C(uint32 state, uint32 model, uint32 guest_stack);
uint32 G3MonsterConstantFromFlags5BA4(uint32 state, uint32 guest_stack);
uint32 G3MonsterShouldWake5BD0(uint32 settings, uint32 configuration, uint32 state, uint32 model, uint32 module_base, uint32 guest_stack);
uint32 G3MonsterCheckWake5D20(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack);
uint32 G3MonsterMinimum5E34(uint32 values);
void G3MonsterExtendPath5E58(uint32 output, uint32 state, uint32 model, uint32 angle, uint32 guest_stack);
void G3MonsterFriction5F78(uint32 velocity, uint32 time, uint32 guest_stack);
void G3MonsterVelocityDisplacement613C(uint32 output, uint32 velocity, uint32 time, uint32 guest_stack);
void G3MonsterYawDisplacement6218(uint32 output, uint32 state, uint32 time, uint32 guest_stack);
uint32 G3MonsterPositiveHalf62FC(uint32 value);
uint32 G3MonsterCheckDisplacement6308(uint32 distance, uint32 table, uint32 guest_stack);
uint32 G3MonsterSelectCommand635C(uint32 angle, uint32 distance, uint32 threshold, uint32 model, uint32 guest_stack);
void G3MonsterUpdatePath6488(uint32 state, uint32 model, uint32 guest_stack);
uint32 G3MonsterConstant6550();
uint32 G3MonsterSelectValue6558(uint32 object);
uint32 G3MonsterSelectDistance6594(uint32 kind, uint32 previous, uint32 mode, uint32 model, uint32 guest_stack);
void G3MonsterMeasureDistances6638(uint32 output, uint32 origin, uint32 first, uint32 target, uint32 guest_stack);
void G3MonsterMeasureAngles6880(uint32 output, uint32 heading, uint32 yaw, uint32 origin, uint32 guest_stack);
void G3MonsterSelectAngles6AD4(uint32 first, uint32 second, uint32 state);
void G3MonsterUpdateTarget6BD8(uint32 object, uint32 state, uint32 snapshot, uint32 output, uint32 guest_stack);
uint32 G3MonsterMotionDistance6C84(uint32 kind, uint32 table, uint32 guest_stack);
uint32 G3MonsterCanTurn6D20(uint32 kind, uint32 angle, uint32 table, uint32 guest_stack);
uint32 G3MonsterSelectTurn6DD4(uint32 kind, uint32 angle, uint32 table, uint32 guest_stack);
void G3MonsterSelectHeading6E98(uint32 output, uint32 origin, uint32 target, uint32 minimum, uint32 guest_stack);
uint32 G3MonsterTurnToward6F44(uint32 target, uint32 origin, uint32 settings, uint32 minimum, uint32 guest_stack);
uint32 G3MonsterStepDisplacement700C(uint32 time, uint32 rotation, uint32 position, uint32 state, uint32 guest_stack);
uint32 G3MonsterStep7178(uint32 time, uint32 object, uint32 state, uint32 mode, uint32 guest_stack);
void G3MonsterNoop72D0();
uint32 G3MonsterCopyVector72D8(uint32 destination, uint32 source);
uint32 G3MonsterConstant7304();
uint32 G3MonsterMatchBounds730C(uint32 bounds, uint32 values);
uint32 G3MonsterMatchFlags73B8(uint32 resource, uint32 state);
uint32 G3MonsterMatchGroup7414(uint32 resource, uint32 values, uint32 state, uint32 kind, uint32 guest_stack);
uint32 G3MonsterFindGroup751C(uint32 output, uint32 values, uint32 model, uint32 state, uint32 guest_stack);
uint32 G3MonsterCountGroup7618(uint32 index, uint32 model);
uint32 G3MonsterFindTrigger7670(uint32 kind, uint32 animation);
void G3MonsterSpawnTrigger76C4(uint32 trigger, uint32 object, uint32 state, uint32 guest_stack);
void G3MonsterPlayerFlag7808(uint32 trigger);
void G3MonsterPlayerDamage7858(uint32 guest_stack);
void G3MonsterApplyTriggerFlags78CC(uint32 trigger, uint32 state);
void G3MonsterTriggerProperty7920(uint32 trigger, uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3MonsterCrossedFrame7948(uint32 previous, uint32 current, uint32 trigger_time, uint32 animation);
void G3MonsterTriggerFrame7A3C(uint32 previous, uint32 current, uint32 trigger, uint32 animation, uint32 module_base, uint32 guest_stack);
void G3MonsterTriggerFrames7B2C(uint32 previous, uint32 current, uint32 animation, uint32 object, uint32 module_base, uint32 guest_stack);
void G3MonsterNoop7BC8();
uint32 G3MorphFaceEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
void G3MorphFaceConfigure(uint32 object, uint32 properties, uint32 guest_stack);
void G3MorphFaceInitialize(uint32 object, uint32 game, uint32 guest_stack);
void G3MorphFaceUpdate(uint32 object, uint32 game, uint32 guest_stack);
void G3MorphFaceCollide(uint32 object, uint32 game, uint32 guest_stack);
void G3MstrdbgExtraAnimationDiagnostic(uint32 name, uint32 module_base, uint32 guest_stack);
void G3MstrdbgLiteralDiagnostic(uint32 text, uint32 guest_stack);
void G3MstrdbgStringDiagnostic(uint32 text, uint32 value, uint32 guest_stack);
void G3MstrdbgNameDiagnostic(uint32 text, uint32 name, uint32 model, uint32 guest_stack);
void G3MstrdbgPursuitLiteralDiagnostic(uint32 text, uint32 guest_stack);
void G3MstrdbgBrainLiteralDiagnostic(uint32 text, uint32 guest_stack);
void G3MstrdbgBrainStringDiagnostic(uint32 text, uint32 name, uint32 guest_stack);
void G3MstrdbgBrainTwoWordDiagnostic(uint32 text, uint32 first, uint32 second, uint32 guest_stack);
uint32 G3MstrdbgEntry0000(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3MstrdbgInitialize0020(uint32 object, uint32 diagnostic_argument, uint32 module_base, uint32 guest_stack);
void G3MstrdbgNoop03D8(void);
void G3MstrdbgUpdate03E0(uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgValidate072C(uint32 object, uint32 diagnostic_argument, uint32 module_base, uint32 guest_stack);
void G3MstrdbgNoop08BC(void);
void G3MstrdbgUpdateCounters08C4(uint32 counters, uint32 state, uint32 object, uint32 guest_stack);
uint32 G3MstrdbgFindExtra0AEC(uint32 settings, uint32 tag, uint32 module_base, uint32 guest_stack);
void G3MstrdbgRunSequence0D6C(uint32 object, uint32 model, uint32 guest_stack);
void G3MstrdbgRunProperty0DA4(uint32 object, uint32 name, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgBuildContext0DE8(uint32 context, uint32 object, uint32 target);
uint32 G3MstrdbgNeedsDecision0E6C(uint32 state, uint32 settings, uint32 configuration, uint32 module_base, uint32 guest_stack);
void G3MstrdbgUpdateHeading106C(uint32 state, uint32 output, uint32 guest_stack);
uint32 G3MstrdbgCheckRange1168(uint32 object, uint32 guest_stack);
uint32 G3MstrdbgUpdateDormancy12F8(uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgPackCoordinates13D4(uint32 position, uint32 output);
uint32 G3MstrdbgApproximateDistance13F8(uint32 position, uint32 packed, uint32 guest_stack);
void G3MstrdbgUpdateMovementWatch1488(uint32 state, uint32 position, uint32 guest_stack);
uint32 G3MstrdbgMoveList156C(uint32 object, uint32 index, uint32 guest_stack);
uint32 G3MstrdbgListKind15BC(uint32 object);
uint32 G3MstrdbgSelectList1604(uint32 object, uint32 guest_stack);
uint32 G3MstrdbgEnterListZero1648(uint32 object, uint32 guest_stack);
uint32 G3MstrdbgEnterPrimary1668(uint32 object, uint32 guest_stack);
uint32 G3MstrdbgEnterSecondary16B8(uint32 object, uint32 guest_stack);
uint32 G3MstrdbgSightClear1718(uint32 start, uint32 end, uint32 guest_stack);
uint32 G3MstrdbgUpdateSight17AC(uint32 start, uint32 end, uint32 state, uint32 guest_stack);
void G3MstrdbgStaggerSight17F8(uint32 start, uint32 end, uint32 state, uint32 phase, uint32 guest_stack);
void G3MstrdbgSetupChain1854(uint32 resource, uint32 settings, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgRotateQuarter18E0(uint32 vector);
uint32 G3MstrdbgValidateRange18F8(uint32 state, uint32 settings);
void G3MstrdbgValidateAnimations1968(uint32 resource, uint32 settings, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgClearFlags1A24(uint32 flags);
uint32 G3MstrdbgClearSlots1A54(uint32 state);
void G3MstrdbgBindAnimations1A8C(uint32 settings, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgClearAnimationBlocks2008(uint32 settings, uint32 guest_stack);
uint32 G3MstrdbgDefaultAnimations2058(uint32 settings);
void G3MstrdbgBindDefaults21E0(uint32 settings);
void G3MstrdbgValidateSequence2244(uint32 sequence, uint32 resource, uint32 guest_stack);
void G3MstrdbgValidateSequences22C4(uint32 resource, uint32 settings, uint32 guest_stack);
void G3MstrdbgSetupAnimations2328(uint32 resource, uint32 settings, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgLookup2394(uint32 kind, uint32 first_index, uint32 second_index, uint32 table);
uint32 G3MstrdbgAdvanceBounded23F4(uint32 step, uint32 position, uint32 settings, uint32 scale);
void G3MstrdbgSampleMotion24CC(uint32 animation, uint32 time, uint32 output, uint32 guest_stack);
void G3MstrdbgScaleVector2648(uint32 vector, uint32 scale);
void G3MstrdbgAnimationMotion26C4(uint32 animation, uint32 output, uint32 guest_stack);
void G3MstrdbgLookupMotion272C(uint32 state, uint32 output, uint32 table, uint32 guest_stack);
uint32 G3MstrdbgSelectAnimation2794(uint32 phase, uint32 output, uint32 state, uint32 model);
void G3MstrdbgProjectSeparation2830(uint32 output, uint32 vector, uint32 normal, uint32 guest_stack);
uint32 G3MstrdbgFacingContact2A2C(uint32 rotation, uint32 vector, uint32 guest_stack);
void G3MstrdbgDeflectVelocity2ADC(uint32 velocity, uint32 delta, uint32 guest_stack);
uint32 G3MstrdbgClassifyContact2BB4(uint32 output, uint32 contact, uint32 side);
void G3MstrdbgBuildDamagePacket2C48(uint32 packet, uint32 object, uint32 target_position, uint32 contact, uint32 guest_stack);
void G3MstrdbgAttackContact2D50(uint32 own_class, uint32 other_class, uint32 contact, uint32 object, uint32 guest_stack);
void G3MstrdbgSeparateContact2EC4(uint32 contact, uint32 object, uint32 guest_stack);
void G3MstrdbgCollide30E4(uint32 object, uint32 guest_stack);
void G3MstrdbgInitializePacket31A0(uint32 packet);
uint32 G3MstrdbgCheckFlags31D4(uint32 state, uint32 model);
uint32 G3MstrdbgDamage322C(uint32 object, uint32 packet, uint32 guest_stack);
uint32 G3MstrdbgRequestRemoval35A4(uint32 object, uint32 guest_stack);
void G3MstrdbgTraceFloor35F8(uint32 output, uint32 object, uint32 top, uint32 bottom, uint32 guest_stack);
void G3MstrdbgBoxFloor36F8(uint32 output, uint32 object, uint32 module_base, uint32 guest_stack);
void G3MstrdbgQueryFloor3780(uint32 output, uint32 object, uint32 top, uint32 bottom, uint32 module_base, uint32 guest_stack);
void G3MstrdbgQueryTerrain37C8(uint32 output, uint32 object, uint32 position, uint32 bottom, uint32 guest_stack);
void G3MstrdbgCaptureFloor38B0(uint32 object, uint32 guest_stack);
void G3MstrdbgResolveFloor3970(uint32 object, uint32 model, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgClampBox3BB0(uint32 position, uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgTraceHorizontal3D34(uint32 position, uint32 previous, uint32 guest_stack);
void G3MstrdbgResolveHorizontal3D8C(uint32 object, uint32 radius, uint32 height, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MstrdbgResolveSides3EBC(uint32 object, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MstrdbgUpdateFloor3F44(uint32 object, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3MstrdbgPostPhysics403C(uint32 object, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MstrdbgPrintCommand40F4(uint32 command, uint32 module_base, uint32 guest_stack);
void G3MstrdbgPrintMode4298(uint32 mode, uint32 module_base, uint32 guest_stack);
void G3MstrdbgPrintBehavior43AC(uint32 descriptor, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MstrdbgPrintState4414(uint32 descriptor, uint32 state, uint32 model, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgFindTrigger44AC(uint32 kind, uint32 animation);
void G3MstrdbgSpawnTrigger4500(uint32 trigger, uint32 object, uint32 state, uint32 guest_stack);
void G3MstrdbgPlayerFlag4644(uint32 trigger);
void G3MstrdbgPlayerDamage4694(uint32 guest_stack);
void G3MstrdbgApplyTriggerFlags4708(uint32 trigger, uint32 state);
void G3MstrdbgTriggerProperty475C(uint32 trigger, uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgCrossedFrame4784(uint32 previous, uint32 current, uint32 trigger_time, uint32 animation);
void G3MstrdbgTriggerFrame4878(uint32 previous, uint32 current, uint32 trigger, uint32 animation, uint32 module_base, uint32 guest_stack);
void G3MstrdbgTriggerFrames4968(uint32 previous, uint32 current, uint32 animation, uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgDecide4A04(uint32 object, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MstrdbgSelectState4B00(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MstrdbgPrepareMode4BC4(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MstrdbgUpdateMainMode4C98(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MstrdbgUpdateActive4CE0(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MstrdbgUpdateWander4D60(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MstrdbgDispatchCommand4E38(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgClearBehaviorFlag4F8C(uint32 state);
uint32 G3MstrdbgApplyTransition4FA4(uint32 state, uint32 configuration, uint32 model, uint32 object, uint32 module_base, uint32 guest_stack);
void G3MstrdbgUpdatePursuit53C8(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgFinishCommand5910(uint32 command, uint32 state);
void G3MstrdbgSwitchCommand592C(uint32 command, uint32 state, uint32 model, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgCommandTwo59C0(uint32 command);
uint32 G3MstrdbgCommandFour59CC(uint32 command);
uint32 G3MstrdbgCommandNineteen59F0(uint32 command);
void G3MstrdbgSwitchCommand5A14(uint32 command, uint32 state);
void G3MstrdbgSwitchCommand5A70(uint32 command, uint32 state);
void G3MstrdbgSwitchCommand5B08(uint32 command, uint32 state);
void G3MstrdbgSwitchCommand5B70(uint32 command);
void G3MstrdbgSwitchCommand5BB0(uint32 command, uint32 state, uint32 model, uint32 flags, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgFinishCommand5D9C(uint32 command);
uint32 G3MstrdbgFinishCommand5DA8(uint32 command);
void G3MstrdbgSwitchTriggered5DB4(uint32 command, uint32 model, uint32 diagnostic_argument, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgTurnBias5FCC(uint32 value);
uint32 G3MstrdbgSelectAlternate6010(uint32 state, uint32 snapshot, uint32 table, uint32 difference, uint32 guest_stack);
uint32 G3MstrdbgCheckAdvance6168(uint32 state, uint32 snapshot, uint32 model, uint32 health, uint32 guest_stack);
uint32 G3MstrdbgCheckRetreat628C(uint32 state, uint32 snapshot, uint32 model, uint32 health, uint32 guest_stack);
uint32 G3MstrdbgCheckSpecial63A8(uint32 state, uint32 snapshot, uint32 model, uint32 health, uint32 guest_stack);
uint32 G3MstrdbgCheckAlternate6404(uint32 state, uint32 model, uint32 guest_stack);
uint32 G3MstrdbgCheckTerrain649C(uint32 first, uint32 second, uint32 state, uint32 guest_stack);
uint32 G3MstrdbgShouldWake64C8(uint32 settings, uint32 configuration, uint32 state, uint32 model, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgCheckWake6618(uint32 state, uint32 configuration, uint32 model, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgMinimum672C(uint32 range);
void G3MstrdbgExtendPath6750(uint32 output, uint32 state, uint32 model, uint32 angle, uint32 guest_stack);
void G3MstrdbgFriction6870(uint32 velocity, uint32 time, uint32 guest_stack);
void G3MstrdbgVelocityDisplacement6A34(uint32 output, uint32 velocity, uint32 time, uint32 guest_stack);
void G3MstrdbgYawDisplacement6B10(uint32 output, uint32 state, uint32 time, uint32 guest_stack);
uint32 G3MstrdbgPositiveHalf6BF4(uint32 value);
uint32 G3MstrdbgCheckDisplacement6C00(uint32 distance, uint32 table, uint32 guest_stack);
uint32 G3MstrdbgSelectCommand6C54(uint32 angle, uint32 distance, uint32 threshold, uint32 model, uint32 guest_stack);
void G3MstrdbgUpdatePath6D80(uint32 state, uint32 model, uint32 guest_stack);
uint32 G3MstrdbgSelectManeuver6E48(uint32 model, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgSelectValue6E8C(uint32 model, uint32 diagnostic_first, uint32 diagnostic_second, uint32 diagnostic_third, uint32 module_base, uint32 guest_stack, bool preserve_diagnostic_homes);
uint32 G3MstrdbgSelectDistance6EE0(uint32 kind, uint32 previous, uint32 mode, uint32 model, uint32 module_base, uint32 guest_stack);
void G3MstrdbgMeasureDistances6F84(uint32 output, uint32 origin, uint32 first, uint32 target, uint32 guest_stack);
void G3MstrdbgMeasureAngles71CC(uint32 output, uint32 heading, uint32 yaw, uint32 origin, uint32 guest_stack);
void G3MstrdbgSelectAngles7420(uint32 first, uint32 second, uint32 state);
void G3MstrdbgUpdateTarget7524(uint32 object, uint32 state, uint32 snapshot, uint32 output, uint32 guest_stack);
uint32 G3MstrdbgMotionDistance75D0(uint32 kind, uint32 table, uint32 guest_stack);
uint32 G3MstrdbgCanTurn766C(uint32 kind, uint32 angle, uint32 table, uint32 guest_stack);
uint32 G3MstrdbgSelectTurn7720(uint32 kind, uint32 angle, uint32 table, uint32 guest_stack);
void G3MstrdbgSelectHeading77E4(uint32 output, uint32 origin, uint32 target, uint32 minimum, uint32 guest_stack);
uint32 G3MstrdbgTurnToward7890(uint32 target, uint32 origin, uint32 settings, uint32 minimum, uint32 guest_stack);
uint32 G3MstrdbgStepDisplacement7958(uint32 time, uint32 rotation, uint32 position, uint32 state, uint32 guest_stack);
uint32 G3MstrdbgStep7AC4(uint32 time, uint32 object, uint32 state, uint32 mode, uint32 guest_stack);
uint32 G3MstrdbgEvent7C1C(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3MstrdbgNoop7E48(void);
uint32 G3MstrdbgCopyVector7E50(uint32 output, uint32 source);
uint32 G3MstrdbgAccept7E7C(void);
uint32 G3MstrdbgTriggerRange7E84(uint32 limits, uint32 position);
uint32 G3MstrdbgTriggerFlags7F30(uint32 trigger, uint32 state);
uint32 G3MstrdbgTriggerMatches7F8C(uint32 trigger, uint32 position, uint32 state, uint32 mask, uint32 guest_stack);
uint32 G3MstrdbgFindTriggered8094(uint32 output, uint32 position, uint32 settings, uint32 state, uint32 module_base, uint32 guest_stack);
uint32 G3MstrdbgCountEntries81A4(uint32 index, uint32 settings);
uint32 G3PirateEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3PirateConfigure(uint32 object, uint32 commands, uint32 guest_stack);
void G3PirateInitialize(uint32 object, uint32 guest_stack);
uint32 G3PirateNearbyBall(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3PirateReact(uint32 object);
void G3PirateUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3PirateCollide(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
uint32 G3PlankEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3PlankConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack);
void G3PlankInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
void G3PlankUpdate(uint32 object, uint32 guest_stack);
void G3PlankPush(uint32 object);
uint32 G3PongballEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
uint32 G3FindLevelResource800397B0(uint32 name, uint32 game);
void G3PongballConfigure(uint32 object, uint32 commands, uint32 guest_stack);
void G3PongballInitialize(uint32 object, uint32 game, uint32 guest_stack);
void G3PongballLaunch(uint32 object, uint32 data, uint32 guest_stack);
void G3PongballDestroy(uint32 object, uint32 guest_stack);
void G3PongballUpdate(uint32 object, uint32 guest_stack);
void G3PongballCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3ProxsigEvent(uint32 object, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3ProxsigDistance(uint32 vector, uint32 state, uint32 guest_stack);
void G3ProxsigDefinitions(uint32 resource, uint32 module_base, uint32 guest_stack);
void G3ProxsigInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
void G3ProxsigUpdate(uint32 object, uint32 guest_stack);
void G3ProxsigConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack);
uint32 G3RahstafEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack);
void G3RahstafInitialize(uint32 object);
void G3RahstafUpdate(uint32 object, uint32 guest_stack);
void G3RahstafCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3SantaEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
void G3SantaConfigure(uint32 object, uint32 commands, uint32 guest_stack);
void G3SantaInitialize(uint32 object, uint32 guest_stack);
void G3SantaCollide(uint32 object, uint32 module_base);
void G3SantaUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
uint32 G3ScannonEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
void G3ScannonInitialize(uint32 object, uint32 state);
uint32 G3ScannonMoveToward(uint32 output, uint32 first, uint32 second, uint32 step);
uint32 G3ScannonTurnToward(uint32 output, uint32 first, uint32 second, uint32 step);
void G3ScannonUpdateRotation(uint32 object, uint32 state, uint32 guest_stack);
void G3ScannonReadControls(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 guest_stack);
void G3ScannonAlignPlayer(uint32 object, uint32 controls, uint32 game, uint32 guest_stack);
void G3ScannonCollide(uint32 object, uint32 game);
void G3ScannonShoot(uint32 object, uint32 state, uint32 controls, uint32 module_base, uint32 guest_stack);
void G3ScannonUpdate(uint32 object, uint32 game, uint32 state, uint32 controls, uint32 module_base, uint32 guest_stack);
uint32 G3SchestEvent(uint32 object, uint32 message, uint32 payload, uint32 guest_stack);
void G3SchestConfigure(uint32 object, uint32 commands, uint32 guest_stack);
void G3SchestInitialize(uint32 object, uint32 guest_stack);
void G3SchestUpdate(uint32 object, uint32 guest_stack);
void G3SchestCollide(uint32 object, uint32 guest_stack);
uint32 G3ScrankEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack);
void G3ScrankConfigure(uint32 object, uint32 commands, uint32 guest_stack);
void G3ScrankInitialize(uint32 object, uint32 guest_stack);
void G3ScrankUpdate(uint32 object, uint32 game, uint32 guest_stack);
void G3ScrankCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3ShooterEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3ShooterConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack);
void G3ShooterInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
void G3ShooterTrack(uint32 object, uint32 game, uint32 guest_stack);
void G3ShooterCenter(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3ShooterInside(uint32 object);
void G3ShooterEffects(uint32 state, uint32 position, uint32 guest_stack);
void G3ShooterActivate(uint32 object, uint32 game, uint32 payload, uint32 guest_stack);
void G3ShooterImpact(uint32 object, uint32 hit, uint32 game, uint32 guest_stack);
void G3ShooterUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
uint32 G3SnowWeaponEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3RemainingPlacementCount80045FE8(uint32 object);
void G3SnowWeaponSelect(uint32 object, uint32 mode, uint32 at_end);
void G3SnowWeaponInitialize(uint32 object);
void G3SnowWeaponUpdate(uint32 object, uint32 guest_stack);
void G3SnowWeaponEmitSnow(uint32 object, uint32 guest_stack);
void G3SnowWeaponEmitFire(uint32 object, uint32 guest_stack);
void G3SnowWeaponContact(void);
void G3SnowWeaponTrigger(uint32 object);
void G3SnowWeaponPlacement(uint32 intro);
uint32 G3SnowboardEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3SnowboardInitialize(uint32 state, uint32 module_base);
void G3SnowboardDust(uint32 position, uint32 heading, uint32 speed, uint32 module_base, uint32 guest_stack);
uint32 G3SnowboardSteer(uint32 game, uint32 object, uint32 state, uint32 settings, uint32 guest_stack);
void G3SnowboardSpeed(uint32 input, uint32 state, uint32 settings, uint32 guest_stack);
void G3SnowboardAnimate(uint32 object, uint32 game, uint32 state, uint32 module_base, uint32 guest_stack);
void G3SnowboardBank(uint32 object, uint32 state, uint32 settings, uint32 guest_stack);
void G3SnowboardAlign(uint32 object, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3SnowboardMove(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3SnowboardFooting(uint32 state);
void G3SnowboardFollowPlatform(uint32 object, uint32 state, uint32 settings, uint32 guest_stack);
void G3SnowboardRequestJump(uint32 object, uint32 game, uint32 state, uint32 settings);
void G3SnowboardTrick(uint32 game);
void G3SnowboardGround(uint32 object, uint32 game, uint32 state, uint32 settings);
void G3SnowboardReleaseAttachment(uint32 game, uint32 guest_stack);
void G3SnowboardRide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3SnowboardUpdate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3SnowboardCollide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 guest_stack);
void G3SnowboardDamage(uint32 object, uint32 game, uint32 state, uint32 guest_stack);
uint32 G3SpnplatEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3SpnplatInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
void G3SpnplatUpdate(uint32 object, uint32 guest_stack);
void G3SpnplatCollide(uint32 object, uint32 game, uint32 guest_stack);
void G3SpnplatConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack);
uint32 G3SuckyEvent(uint32 object, uint32 game, uint32 message, uint32 guest_stack);
void G3SuckyInitialize(uint32 object);
void G3SuckyUpdate(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3SuckyProject(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3TankEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3TankInitialize(uint32 object, uint32 state, uint32 settings);
uint32 G3TankSteer(uint32 game, uint32 object, uint32 state, uint32 settings, uint32 guest_stack);
void G3TankSpeed(uint32 input, uint32 state, uint32 settings);
void G3TankAnimate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3TankIdle(uint32 object, uint32 state, uint32 settings);
void G3TankGround(uint32 object, uint32 state, uint32 settings);
void G3TankCleanup(uint32 object, uint32 state);
void G3TankDust(uint32 position, uint32 heading, uint32 speed, uint32 module_base, uint32 guest_stack);
void G3TankAlign(uint32 object, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3TankMove(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3TankFooting(uint32 game, uint32 state, uint32 guest_stack);
void G3TankFollowPlatform(uint32 object, uint32 state, uint32 settings, uint32 guest_stack);
void G3TankFire(uint32 object, uint32 settings, uint32 guest_stack);
void G3TankRide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3TankUpdate(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
void G3TankCollide(uint32 object, uint32 game, uint32 state, uint32 settings, uint32 module_base, uint32 guest_stack);
uint32 G3TeeterEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
void G3TeeterInitialize(uint32 object, uint32 module_base);
void G3TeeterUpdate(uint32 object, uint32 game, uint32 guest_stack);
void G3TeeterCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3TformrEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
void G3TformrInitialize(uint32 object, uint32 module_base);
void G3TformrUpdate(uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3TformrUsesOuterBounds(uint32 object);
void G3TformrTurnToward(uint32 object, uint32 target, uint32 step);
uint32 G3TformrHeadingTo(uint32 object, uint32 point, uint32 guest_stack);
void G3TformrClamp(uint32 object);
void G3TformrCollide(uint32 object, uint32 game, uint32 guest_stack);
void G3TformrHit(uint32 object, uint32 guest_stack);
uint32 G3TformrContains(uint32 object, uint32 point, uint32 outer);
uint32 G3TformrNearXY(uint32 object, uint32 point, uint32 radius);
uint32 G3TformrSetAnimation(uint32 object, uint32 animation, uint32 frame);
uint32 G3PlayerUnavailable8007863C(void);
uint32 G3TformrAnimate(uint32 object, uint32 turn_hint, uint32 module_base, uint32 guest_stack);
void G3TformrReturnHome(uint32 object, uint32 guest_stack);
void G3TformrMove(uint32 object, uint32 module_base, uint32 guest_stack);
void G3TformrShoot(uint32 object, uint32 side, uint32 guest_stack);
uint32 G3TrackerEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
void G3TrackerConfigure(uint32 state, uint32 data);
void G3TrackerInitialize(uint32 object, uint32 game, uint32 guest_stack);
void G3TrackerTrail(uint32 state, uint32 data, uint32 object, uint32 guest_stack);
void G3TrackerUpdate(uint32 object, uint32 game, uint32 guest_stack);
void G3TrackerCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3TumbleEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3PathCenter80040AD4(uint32 channel, uint32 output);
void G3TumbleConfigure(uint32 object, uint32 properties, uint32 module_base, uint32 guest_stack);
void G3TumbleInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
void G3TumbleUpdate(uint32 object, uint32 game, uint32 guest_stack);
void G3TumbleCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3UtakerEvent(uint32 object, uint32 game, uint32 message, uint32 module_base, uint32 guest_stack);
void G3UtakerConfigure(uint32 object, uint32 commands, uint32 guest_stack);
void G3UtakerInitialize(uint32 object, uint32 guest_stack);
void G3UtakerUpdate(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
void G3UtakerCollide(uint32 object, uint32 game, uint32 module_base, uint32 guest_stack);
uint32 G3VaultCodeEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
uint32 G3AttachmentCount80036FC0(uint32 object);
void G3VaultCodeInitialize(uint32 state, uint32 data);
void G3VaultCodeCollect(uint32 object, uint32 state, uint32 data, uint32 guest_stack);
void G3VaultCodeUpdate(uint32 object, uint32 game, uint32 state, uint32 data, uint32 module_base, uint32 guest_stack);
void G3VaultCodeDisplay(uint32 state);
uint32 G3VaultCodeCheck(uint32 state, uint32 data, uint32 guest_stack);
uint32 G3VaultCodeInput(uint32 game, uint32 state, uint32 data, uint32 module_base, uint32 guest_stack);
uint32 G3WakballEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
void G3WakballConfigure(uint32 object, uint32 commands, uint32 guest_stack);
void G3WakballInitialize(uint32 object, uint32 game, uint32 guest_stack);
void G3WakballLaunch(uint32 object, uint32 data, uint32 guest_stack);
void G3WakballDestroy(uint32 object, uint32 guest_stack);
void G3WakballUpdate(uint32 object, uint32 guest_stack);
void G3WakballCollide(uint32 object, uint32 game, uint32 guest_stack);
uint32 G3WaterFanEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 module_base, uint32 guest_stack);
void G3WaterFanConfigure(uint32 object, uint32 commands, uint32 module_base, uint32 guest_stack);
void G3WaterFanInitialize(uint32 object, uint32 module_base, uint32 guest_stack);
uint32 G3WaterFanForceAt(uint32 object, uint32 position, uint32 output, uint32 guest_stack);
void G3WaterFanUpdate(uint32 object, uint32 game, uint32 guest_stack);
void G3WaterFanAffect(uint32 object, uint32 effect, uint32 guest_stack);
void G3WaterFanCollide(void);
uint32 G3ZiplineEvent(uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
void G3ZiplineConfigure(uint32 object, uint32 commands, uint32 guest_stack);
void G3ZiplineInitialize(uint32 object, uint32 guest_stack);
void G3ZiplineUpdate(uint32 object);
void G3ZiplineCollide(uint32 object, uint32 game, uint32 guest_stack);

// Native module and event routing services
void G3LevelCodeRegistryReset(void);
void G3LevelCodeInvalidateSpan(uint32 address, uint32 bytes);
void G3LevelCodePop(uint32 previous_depth);
void G3LevelCodeRegister(uint32 archive_entry, uint32 module, uint32 allocation_end, uint32 allocation_depth);
bool G3LevelCodeResolve(uint32 target, uint32 *identity_id, uint32 *module_base, uint32 *block_offset);
uint32 G3InvokeReviewedLevelEntry(uint32 identity_id, uint32 module_base, uint32 object, uint32 game, uint32 message, uint32 payload, uint32 guest_stack);
sint32 G3NativeMainEvent(void);
sint32 G3NativeClockEvent(void);

void G3DispatchMainState(uint32 target, uint32 state, uint32 guest_stack);

uint32 G3DispatchAudioCommand(uint32 target, uint32 command);

sint32 G3NativeAudioEvent(void);

sint32 G3NativeWriteSlot(uint32 state, uint32 record);
sint32 G3NativeReadSlot(uint32 state, uint32 record);
uint32 G3NativeSaveSlotsPage(uint32 context, uint32 selected, uint32 third_glyph, uint32 guest_stack);
uint32 G3Function80068EC0(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);
uint32 G3Function80068F1C(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);

uint32 G3Function80068D90(uint32 context, uint32 selected, uint32 incoming_third_glyph, uint32 guest_stack);
uint32 G3Function80068E2C(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);
uint32 G3Function80068D38(uint32 context, uint32 selected, uint32 event, uint32 guest_stack);

uint32 G3DispatchAllocatedVoice(uint32 target, uint32 mask);

void G3StartAudioFieldClock(uint32 period);
void G3StopAudioFieldClock(void);
void G3OnAudioField(void);

#endif
